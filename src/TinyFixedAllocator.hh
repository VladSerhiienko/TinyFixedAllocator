#include <vector>
#include <iostream>
#include <atomic>
#include <mutex>
#include <shared_mutex>

namespace apemode {
namespace defaults {

struct SpinLock {
    std::atomic_flag atomic_flag = {false};
    void lock() { while (atomic_flag.test_and_set(std::memory_order_acquire)); }
    void unlock() { atomic_flag.clear(std::memory_order_release); }
    void lock_shared() { lock(); }
    void unlock_shared() { unlock(); }
};

struct ByteSpan {
    uint8_t* dataPtr = nullptr;
    size_t dataSize = 0;

    constexpr ByteSpan() = default;
    constexpr ByteSpan(const ByteSpan& rhs) = default;
    constexpr ByteSpan& operator=(const ByteSpan&) = default;
    ~ByteSpan() = default;

    constexpr ByteSpan(uint8_t* dataPtr, size_t dataSize) : dataPtr(dataPtr), dataSize(dataSize) {}

    constexpr uint8_t* data() const { return dataPtr; }
    constexpr size_t size() const { return dataSize; }
    constexpr bool empty() const { return 0 == dataSize; }
};

struct EmptyLock {
    constexpr explicit EmptyLock() = default;
    ~EmptyLock() = default;
};

struct EmptyLockGuard {
    template <typename L>
    constexpr explicit EmptyLockGuard(const L&) {}
    ~EmptyLockGuard() = default;
};

struct DefaultSingleThreadedLockPolicy {
    using Lock = EmptyLock;
    using UniqueLockGuard = EmptyLockGuard;
    using SharedLockGuard = EmptyLockGuard;
};

struct DefaultMultiThreadedLockPolicy {
    using Lock = SpinLock;
    using UniqueLockGuard = std::unique_lock<Lock>;
    using SharedLockGuard = std::shared_lock<Lock>;
};

struct DefaultExceptionPolicy {
    static constexpr bool NoexceptFree = false;

    template <typename E = std::runtime_error, typename ...Args>
    [[noreturn]] static void raiseError(Args&& ...args) {
        throw E(std::forward<Args>(args)...);
    }
};
}

template <typename SizeType>
struct FixedAllocatorRange {
    using size_type = SizeType;
    size_type offset = 0;
    size_type size = 0;
};

template <typename SizeType,
          typename ContainerType,
          typename RangeVectorType = std::vector<FixedAllocatorRange<SizeType>>,
          typename ExceptionPolicy = defaults::DefaultExceptionPolicy,
          typename LockPolicy = defaults::DefaultSingleThreadedLockPolicy>
struct FixedAllocator {
    using size_type = SizeType;
    using range_type = FixedAllocatorRange<SizeType>;
    static constexpr size_type headerSize = static_cast<size_type>(sizeof(size_type));

    const ContainerType container{};
    RangeVectorType freeBufferRanges{};
    mutable typename LockPolicy::Lock lock{};

    explicit FixedAllocator(const ContainerType& c) : container(c) { init(); }
    explicit FixedAllocator(ContainerType&& c) : container(std::move(c)) { init(); }

    void init() {
        assert(container.size() < std::numeric_limits<size_type>::max());
        freeBufferRanges.clear();
        freeBufferRanges.push_back({0, static_cast<size_type>(container.size())});
    }

    defaults::ByteSpan allocByteSpan(size_type size) {
        if (container.empty()) { return {}; }

        typename LockPolicy::UniqueLockGuard lockGuard(lock);
        if (freeBufferRanges.empty()) { return {}; }

        size_type chunkSize = size + headerSize;

        auto rangeIt = freeBufferRanges.begin();
        for (; rangeIt != freeBufferRanges.end(); ++rangeIt) {
            auto& r = *rangeIt;

            if (r.size >= chunkSize) {
                uint8_t* headerPtr = container.data() + r.offset;
                *reinterpret_cast<size_type*>(headerPtr) = chunkSize;
                
                uint8_t* allocPtr = headerPtr + headerSize;
                defaults::ByteSpan occupiedSpan(allocPtr, size);

                r.offset += chunkSize;
                r.size -= chunkSize;

                if (r.size == 0) { freeBufferRanges.erase(rangeIt); }
                return occupiedSpan;
            }
        }

        return {};
    }

    void* alloc(size_type size) {
        auto allocatedSpan = allocByteSpan(size);
        return allocatedSpan.data();
    }

    bool freeRange(range_type rr) noexcept(ExceptionPolicy::NoexceptFree) {
        typename LockPolicy::UniqueLockGuard lockGuard(lock);

        //
        // Special fast-exit case, when we can simply add a new free range and succeed.
        //
        
        if (freeBufferRanges.empty()) {
            freeBufferRanges.push_back(rr);
            return true;
        }

        size_type rrEnd = rr.offset + rr.size;

        auto rangeIt = freeBufferRanges.begin();
        for (; rangeIt != freeBufferRanges.end(); ++rangeIt) {
            auto& r = *rangeIt;
            
            //
            // Extend the right free range from the left side.
            // Maybe merge with the previous block.
            //
            
            if (rrEnd == r.offset) {
                r.offset = rr.offset;
                r.size += rr.size;
                
                if (rangeIt != freeBufferRanges.begin()) {
                    auto prevRangeIt = rangeIt - 1;
                    if (prevRangeIt != freeBufferRanges.end()) {
                        auto& pr = *prevRangeIt;
                        auto prEnd = pr.offset + pr.size;
                        if (prEnd == r.offset) {
                            pr.size += r.size;
                            freeBufferRanges.erase(rangeIt);
                        }
                    }
                }
                
                return true;
            }
            
            //
            // Extend the left free range from the right side.
            // Maybe merge with the next block.
            //

            auto rEnd = r.offset + r.size;
            if (rEnd == rr.offset) {
                r.size += rr.size;
                rEnd = r.offset + r.size;

                auto nextRangeIt = rangeIt + 1;
                if (nextRangeIt != freeBufferRanges.end()) {
                    auto& nr = *nextRangeIt;
                    if (nr.offset == rEnd) {
                        r.size += nr.size;
                        freeBufferRanges.erase(nextRangeIt);
                    }
                }

                return true;
            }
        }
        
        //
        // Insert the new range.
        //

        rangeIt = freeBufferRanges.begin();
        auto prevRangeIt = freeBufferRanges.end();
        for (; rangeIt != freeBufferRanges.end(); prevRangeIt = rangeIt++) {
            auto& r = *rangeIt;

            if (r.offset > rr.offset) {
                if (prevRangeIt == freeBufferRanges.end()) {
                    freeBufferRanges.insert(rangeIt, rr);
                    return true;
                }

                auto& pr = *prevRangeIt;
                auto prEnd = pr.offset + pr.size;

                if (prEnd < rr.offset && r.offset > rrEnd) {
                    freeBufferRanges.insert(rangeIt, rr);
                    return true;
                }
            }
        }
        
        //
        // Check for double-free error.
        //
            
        rangeIt = freeBufferRanges.begin();
        for (; rangeIt != freeBufferRanges.end(); ++rangeIt) {
            auto& r = *rangeIt;
            auto rEnd = r.offset + r.size;
            
            if (r.offset <= rr.offset && rEnd > rr.offset) {
                return false;
            }
            
            if (r.offset <= rrEnd && rEnd > rrEnd) {
                return false;
            }
        }
        
        //
        // Append the new range.
        //

        freeBufferRanges.push_back(rr);
        return true;
    }

    void free(void* dataPtr) noexcept(ExceptionPolicy::NoexceptFree) {
        if (!dataPtr || container.empty()) { return; }
        
        auto c = container.data();
        auto cEnd = c + container.size();

        if (dataPtr < c || dataPtr >= cEnd) {
            ExceptionPolicy::template raiseError<std::runtime_error>("Memory range is out of bounds.");
        }
        
        uintptr_t allocAddress = reinterpret_cast<uintptr_t>(dataPtr);
        uintptr_t containerDataAddress = reinterpret_cast<uintptr_t>(container.data());
        uintptr_t offset = allocAddress - containerDataAddress;
        
        if (offset < headerSize) { assert(false); return; }
        offset -= headerSize;
        
        uint8_t* headerPtr = reinterpret_cast<uint8_t*>(dataPtr) - headerSize;
        
        range_type r = {};
        r.offset = static_cast<size_type>(offset);
        r.size = static_cast<size_type>(*reinterpret_cast<size_type*>(headerPtr));

        if (!r.size || !freeRange(r)) {
            ExceptionPolicy::template raiseError<std::runtime_error>("Memory range is already free.");
        }
    }

    size_type totalOccupiedSpace() const {
        return container.size() - totalFreeSpace();
    }

    size_type totalFreeSpace() const {
        typename LockPolicy::SharedLockGuard lockGuard(lock);

        size_type totalFreeSize = 0;
        auto rangeIt = freeBufferRanges.begin();
        for (; rangeIt != freeBufferRanges.end(); ++rangeIt) {
            auto& r = *rangeIt;
            totalFreeSize += r.size;
        }

        return totalFreeSize;
    }

    void dumpState(std::ostream& out = std::cout) const {
        typename LockPolicy::SharedLockGuard lockGuard(lock);
        
        out << ">>> ----------" << std::endl;
        out << __FUNCTION__ << ":" << std::endl;
        out << "wholeBuffer={ptr=" << (void*)(container.data()) << ", size=" << container.size() << "}" << std::endl;
        out << "FreeRanges=[";
        
        size_type totalFreeSize = 0;
        auto rangeIt = freeBufferRanges.begin();
        for (; rangeIt != freeBufferRanges.end(); ++rangeIt) {
            auto& r = *rangeIt;
            totalFreeSize += r.size;
            out << "{offset=" << r.offset << ",size=" << r.size << "},";
        }
        
        out << "]" << std::endl;
        out << "occupied :" << (container.size() - totalFreeSize) << std::endl;
        out << "available:" << (totalFreeSize) << std::endl;
        out << "<<< ----------" << std::endl;
    }
    
    bool good() const {
        typename LockPolicy::SharedLockGuard lockGuard(lock);
        
        auto rangeIt = freeBufferRanges.begin();
        auto prevRangeIt = freeBufferRanges.end();
        for (; rangeIt != freeBufferRanges.end(); prevRangeIt = rangeIt++) {
            auto& r = *rangeIt;
            
            if (r.offset >= container.size()) { return false; }
            if (r.size > container.size()) { return false; }
            
            if (prevRangeIt != freeBufferRanges.end()) {
                auto& pr = *prevRangeIt;
                auto prEnd = pr.offset + pr.size;
                
                if (prEnd >= r.offset) { return false; }
            }
        }
        
        return true;
    }
};

}
