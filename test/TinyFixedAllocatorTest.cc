
#include <TinyFixedAllocator.hh>
#include <gtest/gtest.h>

namespace {

class TinyFixedAllocatorTest : public testing::Test {
public:
    TinyFixedAllocatorTest() = default;
    ~TinyFixedAllocatorTest() = default;
    void SetUp() override {}
    void TearDown() override {}
};

using namespace apemode;
using namespace apemode::defaults;

TEST_F(TinyFixedAllocatorTest, TinyFixedAllocatorThrowTest) {
    std::vector<uint8_t> vectorBuffer = {};
    vectorBuffer.resize(4096);

    ByteSpan span(vectorBuffer.data(), vectorBuffer.size());
    FixedAllocator<uint16_t, ByteSpan> fixedAllocator(span);

    auto _0 = fixedAllocator.alloc(1022); EXPECT_TRUE(fixedAllocator.good());
    auto _1 = fixedAllocator.alloc(1022); EXPECT_TRUE(fixedAllocator.good());
    auto _2 = fixedAllocator.alloc(1022); EXPECT_TRUE(fixedAllocator.good());
    auto _3 = fixedAllocator.alloc(1022); EXPECT_TRUE(fixedAllocator.good());
    
    EXPECT_TRUE(fixedAllocator.totalFreeSpace() == 0);
    EXPECT_TRUE(fixedAllocator.totalOccupiedSpace() == vectorBuffer.size());

    EXPECT_NE(nullptr, _0);
    EXPECT_NE(nullptr, _1);
    EXPECT_NE(nullptr, _2);
    EXPECT_NE(nullptr, _3);

    EXPECT_NO_THROW(fixedAllocator.free(_3)); EXPECT_TRUE(fixedAllocator.good());
    EXPECT_NO_THROW(fixedAllocator.free(_2)); EXPECT_TRUE(fixedAllocator.good());
    EXPECT_NO_THROW(fixedAllocator.free(_1)); EXPECT_TRUE(fixedAllocator.good());
    EXPECT_NO_THROW(fixedAllocator.free(_0)); EXPECT_TRUE(fixedAllocator.good());
    
    EXPECT_TRUE(fixedAllocator.totalFreeSpace() == vectorBuffer.size());
    EXPECT_TRUE(fixedAllocator.totalOccupiedSpace() == 0);

    EXPECT_ANY_THROW(fixedAllocator.free(_2)); EXPECT_TRUE(fixedAllocator.good());
    EXPECT_ANY_THROW(fixedAllocator.free(_2)); EXPECT_TRUE(fixedAllocator.good());
    EXPECT_ANY_THROW(fixedAllocator.free(_1)); EXPECT_TRUE(fixedAllocator.good());
    EXPECT_ANY_THROW(fixedAllocator.free(_1)); EXPECT_TRUE(fixedAllocator.good());
    EXPECT_ANY_THROW(fixedAllocator.free(_0)); EXPECT_TRUE(fixedAllocator.good());
    EXPECT_ANY_THROW(fixedAllocator.free(_1)); EXPECT_TRUE(fixedAllocator.good());
    EXPECT_ANY_THROW(fixedAllocator.free(_1)); EXPECT_TRUE(fixedAllocator.good());
    EXPECT_ANY_THROW(fixedAllocator.free(_3)); EXPECT_TRUE(fixedAllocator.good());
    EXPECT_ANY_THROW(fixedAllocator.free(_2)); EXPECT_TRUE(fixedAllocator.good());
    EXPECT_ANY_THROW(fixedAllocator.free(_0)); EXPECT_TRUE(fixedAllocator.good());
    EXPECT_ANY_THROW(fixedAllocator.free(_0)); EXPECT_TRUE(fixedAllocator.good());
    EXPECT_ANY_THROW(fixedAllocator.free(_0)); EXPECT_TRUE(fixedAllocator.good());
    EXPECT_ANY_THROW(fixedAllocator.free(_3)); EXPECT_TRUE(fixedAllocator.good());
    EXPECT_ANY_THROW(fixedAllocator.free(_0)); EXPECT_TRUE(fixedAllocator.good());
    EXPECT_ANY_THROW(fixedAllocator.free(_0)); EXPECT_TRUE(fixedAllocator.good());
    
    EXPECT_TRUE(fixedAllocator.totalFreeSpace() == vectorBuffer.size());
    EXPECT_TRUE(fixedAllocator.totalOccupiedSpace() == 0);
}

TEST_F(TinyFixedAllocatorTest, TinyFixedAllocatorMegaTest) {
    std::vector<uint8_t> vectorBuffer = {};
    vectorBuffer.resize(4096);

    ByteSpan span(vectorBuffer.data(), vectorBuffer.size());
    FixedAllocator<uint16_t, ByteSpan> fixedAllocator(span);

    auto _0 = fixedAllocator.alloc(1022); EXPECT_TRUE(fixedAllocator.good());
    auto _1 = fixedAllocator.alloc(1022); EXPECT_TRUE(fixedAllocator.good());
    auto _2 = fixedAllocator.alloc(1022); EXPECT_TRUE(fixedAllocator.good());
    auto _3 = fixedAllocator.alloc(1022); EXPECT_TRUE(fixedAllocator.good());
    
    EXPECT_TRUE(fixedAllocator.totalFreeSpace() == 0);
    EXPECT_TRUE(fixedAllocator.totalOccupiedSpace() == vectorBuffer.size());

    EXPECT_NE(nullptr, _0);
    EXPECT_NE(nullptr, _1);
    EXPECT_NE(nullptr, _2);
    EXPECT_NE(nullptr, _3);

    EXPECT_NO_THROW(fixedAllocator.free(_3)); EXPECT_TRUE(fixedAllocator.good());
    EXPECT_NO_THROW(fixedAllocator.free(_2)); EXPECT_TRUE(fixedAllocator.good());
    EXPECT_NO_THROW(fixedAllocator.free(_1)); EXPECT_TRUE(fixedAllocator.good());
    EXPECT_NO_THROW(fixedAllocator.free(_0)); EXPECT_TRUE(fixedAllocator.good());
    
    EXPECT_TRUE(fixedAllocator.totalFreeSpace() == vectorBuffer.size());
    EXPECT_TRUE(fixedAllocator.totalOccupiedSpace() == 0);
    
    //
    //
    //

    _0 = fixedAllocator.alloc(1022); EXPECT_TRUE(fixedAllocator.good());
    _1 = fixedAllocator.alloc(1022); EXPECT_TRUE(fixedAllocator.good());
    _2 = fixedAllocator.alloc(1022); EXPECT_TRUE(fixedAllocator.good());
    _3 = fixedAllocator.alloc(1022); EXPECT_TRUE(fixedAllocator.good());
    
    EXPECT_TRUE(fixedAllocator.totalFreeSpace() == 0);
    EXPECT_TRUE(fixedAllocator.totalOccupiedSpace() == vectorBuffer.size());

    EXPECT_NE(nullptr, _0);
    EXPECT_NE(nullptr, _1);
    EXPECT_NE(nullptr, _2);
    EXPECT_NE(nullptr, _3);

    EXPECT_NO_THROW(fixedAllocator.free(_0)); EXPECT_TRUE(fixedAllocator.good());
    EXPECT_NO_THROW(fixedAllocator.free(_1)); EXPECT_TRUE(fixedAllocator.good());
    EXPECT_NO_THROW(fixedAllocator.free(_2)); EXPECT_TRUE(fixedAllocator.good());
    EXPECT_NO_THROW(fixedAllocator.free(_3)); EXPECT_TRUE(fixedAllocator.good());
    
    EXPECT_TRUE(fixedAllocator.totalFreeSpace() == vectorBuffer.size());
    EXPECT_TRUE(fixedAllocator.totalOccupiedSpace() == 0);
    
    //
    //
    //

    _0 = fixedAllocator.alloc(1022); EXPECT_TRUE(fixedAllocator.good());
    _1 = fixedAllocator.alloc(1022); EXPECT_TRUE(fixedAllocator.good());
    _2 = fixedAllocator.alloc(1022); EXPECT_TRUE(fixedAllocator.good());
    _3 = fixedAllocator.alloc(1022); EXPECT_TRUE(fixedAllocator.good());
    
    EXPECT_TRUE(fixedAllocator.totalFreeSpace() == 0);
    EXPECT_TRUE(fixedAllocator.totalOccupiedSpace() == vectorBuffer.size());

    EXPECT_NE(nullptr, _0);
    EXPECT_NE(nullptr, _1);
    EXPECT_NE(nullptr, _2);
    EXPECT_NE(nullptr, _3);

    EXPECT_NO_THROW(fixedAllocator.free(_0)); EXPECT_TRUE(fixedAllocator.good());
    EXPECT_NO_THROW(fixedAllocator.free(_2)); EXPECT_TRUE(fixedAllocator.good());
    EXPECT_NO_THROW(fixedAllocator.free(_1)); EXPECT_TRUE(fixedAllocator.good());
    EXPECT_NO_THROW(fixedAllocator.free(_3)); EXPECT_TRUE(fixedAllocator.good());
    
    EXPECT_TRUE(fixedAllocator.totalFreeSpace() == vectorBuffer.size());
    EXPECT_TRUE(fixedAllocator.totalOccupiedSpace() == 0);
    
    //
    //
    //

    _0 = fixedAllocator.alloc(1022); EXPECT_TRUE(fixedAllocator.good());
    _1 = fixedAllocator.alloc(1022); EXPECT_TRUE(fixedAllocator.good());
    _2 = fixedAllocator.alloc(1022); EXPECT_TRUE(fixedAllocator.good());
    _3 = fixedAllocator.alloc(1022); EXPECT_TRUE(fixedAllocator.good());
    
    EXPECT_TRUE(fixedAllocator.totalFreeSpace() == 0);
    EXPECT_TRUE(fixedAllocator.totalOccupiedSpace() == vectorBuffer.size());

    EXPECT_NE(nullptr, _0);
    EXPECT_NE(nullptr, _1);
    EXPECT_NE(nullptr, _2);
    EXPECT_NE(nullptr, _3);

    EXPECT_NO_THROW(fixedAllocator.free(_0)); EXPECT_TRUE(fixedAllocator.good());
    EXPECT_NO_THROW(fixedAllocator.free(_3)); EXPECT_TRUE(fixedAllocator.good());
    EXPECT_NO_THROW(fixedAllocator.free(_1)); EXPECT_TRUE(fixedAllocator.good());
    EXPECT_NO_THROW(fixedAllocator.free(_2)); EXPECT_TRUE(fixedAllocator.good());
    
    EXPECT_TRUE(fixedAllocator.totalFreeSpace() == vectorBuffer.size());
    EXPECT_TRUE(fixedAllocator.totalOccupiedSpace() == 0);
    
    //
    //
    //

    _0 = fixedAllocator.alloc(1022); EXPECT_TRUE(fixedAllocator.good());
    _1 = fixedAllocator.alloc(1022); EXPECT_TRUE(fixedAllocator.good());
    _2 = fixedAllocator.alloc(1022); EXPECT_TRUE(fixedAllocator.good());
    _3 = fixedAllocator.alloc(1022); EXPECT_TRUE(fixedAllocator.good());
    
    EXPECT_TRUE(fixedAllocator.totalFreeSpace() == 0);
    EXPECT_TRUE(fixedAllocator.totalOccupiedSpace() == vectorBuffer.size());

    EXPECT_NE(nullptr, _0);
    EXPECT_NE(nullptr, _1);
    EXPECT_NE(nullptr, _2);
    EXPECT_NE(nullptr, _3);

    EXPECT_NO_THROW(fixedAllocator.free(_0)); EXPECT_TRUE(fixedAllocator.good());
    EXPECT_NO_THROW(fixedAllocator.free(_1)); EXPECT_TRUE(fixedAllocator.good());
    EXPECT_NO_THROW(fixedAllocator.free(_3)); EXPECT_TRUE(fixedAllocator.good());
    EXPECT_NO_THROW(fixedAllocator.free(_2)); EXPECT_TRUE(fixedAllocator.good());
    
    EXPECT_TRUE(fixedAllocator.totalFreeSpace() == vectorBuffer.size());
    EXPECT_TRUE(fixedAllocator.totalOccupiedSpace() == 0);
    
    //
    //
    //

    _0 = fixedAllocator.alloc(1022); EXPECT_TRUE(fixedAllocator.good());
    _1 = fixedAllocator.alloc(1022); EXPECT_TRUE(fixedAllocator.good());
    _2 = fixedAllocator.alloc(1022); EXPECT_TRUE(fixedAllocator.good());
    _3 = fixedAllocator.alloc(1022); EXPECT_TRUE(fixedAllocator.good());
    
    EXPECT_TRUE(fixedAllocator.totalFreeSpace() == 0);
    EXPECT_TRUE(fixedAllocator.totalOccupiedSpace() == vectorBuffer.size());

    EXPECT_NE(nullptr, _0);
    EXPECT_NE(nullptr, _1);
    EXPECT_NE(nullptr, _2);
    EXPECT_NE(nullptr, _3);

    EXPECT_NO_THROW(fixedAllocator.free(_0)); EXPECT_TRUE(fixedAllocator.good());
    EXPECT_NO_THROW(fixedAllocator.free(_2)); EXPECT_TRUE(fixedAllocator.good());
    EXPECT_NO_THROW(fixedAllocator.free(_3)); EXPECT_TRUE(fixedAllocator.good());
    EXPECT_NO_THROW(fixedAllocator.free(_1)); EXPECT_TRUE(fixedAllocator.good());
    
    EXPECT_TRUE(fixedAllocator.totalFreeSpace() == vectorBuffer.size());
    EXPECT_TRUE(fixedAllocator.totalOccupiedSpace() == 0);
    
    //
    //
    //
    
    _0 = fixedAllocator.alloc(1022); EXPECT_TRUE(fixedAllocator.good());
    _1 = fixedAllocator.alloc(1022); EXPECT_TRUE(fixedAllocator.good());
    
    EXPECT_NO_THROW(fixedAllocator.free(_0)); EXPECT_TRUE(fixedAllocator.good());
    
    _2 = fixedAllocator.alloc(1022); EXPECT_TRUE(fixedAllocator.good());
    
    EXPECT_NO_THROW(fixedAllocator.free(_1)); EXPECT_TRUE(fixedAllocator.good());
    EXPECT_NO_THROW(fixedAllocator.free(_2)); EXPECT_TRUE(fixedAllocator.good());
    
    _0 = fixedAllocator.alloc(1022); EXPECT_TRUE(fixedAllocator.good());
    _1 = fixedAllocator.alloc(1022); EXPECT_TRUE(fixedAllocator.good());
    _2 = fixedAllocator.alloc(1022); EXPECT_TRUE(fixedAllocator.good());
    
    EXPECT_NO_THROW(fixedAllocator.free(_1)); EXPECT_TRUE(fixedAllocator.good());
    EXPECT_NO_THROW(fixedAllocator.free(_2)); EXPECT_TRUE(fixedAllocator.good());
    
    _1 = fixedAllocator.alloc(1022); EXPECT_TRUE(fixedAllocator.good());
    
    EXPECT_NO_THROW(fixedAllocator.free(_0)); EXPECT_TRUE(fixedAllocator.good());
    EXPECT_NO_THROW(fixedAllocator.free(_1)); EXPECT_TRUE(fixedAllocator.good());
    
    _2 = fixedAllocator.alloc(1022);
    
    EXPECT_NO_THROW(fixedAllocator.free(_2)); EXPECT_TRUE(fixedAllocator.good());
    
    EXPECT_TRUE(fixedAllocator.totalFreeSpace() == vectorBuffer.size());
    EXPECT_TRUE(fixedAllocator.totalOccupiedSpace() == 0);
    
    //
    //
    //
    
    _0 = fixedAllocator.alloc(1022); EXPECT_TRUE(fixedAllocator.good());
    _1 = fixedAllocator.alloc(1022); EXPECT_TRUE(fixedAllocator.good());
    _2 = fixedAllocator.alloc(1022); EXPECT_TRUE(fixedAllocator.good());
    _3 = fixedAllocator.alloc(1022); EXPECT_TRUE(fixedAllocator.good());
    
    EXPECT_TRUE(fixedAllocator.totalFreeSpace() == 0);
    EXPECT_TRUE(fixedAllocator.totalOccupiedSpace() == vectorBuffer.size());
    
    EXPECT_NO_THROW(fixedAllocator.free(_2)); EXPECT_TRUE(fixedAllocator.good());
    _2 = fixedAllocator.alloc(1022); EXPECT_TRUE(fixedAllocator.good());
    
    EXPECT_NO_THROW(fixedAllocator.free(_2)); EXPECT_TRUE(fixedAllocator.good());
    _2 = fixedAllocator.alloc(1022); EXPECT_TRUE(fixedAllocator.good());
    
    EXPECT_TRUE(fixedAllocator.totalFreeSpace() == 0);
    EXPECT_TRUE(fixedAllocator.totalOccupiedSpace() == vectorBuffer.size());
    
    EXPECT_NO_THROW(fixedAllocator.free(_3)); EXPECT_TRUE(fixedAllocator.good());
    _3 = fixedAllocator.alloc(1022); EXPECT_TRUE(fixedAllocator.good());
    
    EXPECT_NO_THROW(fixedAllocator.free(_3)); EXPECT_TRUE(fixedAllocator.good());
    _3 = fixedAllocator.alloc(1022); EXPECT_TRUE(fixedAllocator.good());
    
    EXPECT_TRUE(fixedAllocator.totalFreeSpace() == 0);
    EXPECT_TRUE(fixedAllocator.totalOccupiedSpace() == vectorBuffer.size());
    
    EXPECT_NO_THROW(fixedAllocator.free(_1)); EXPECT_TRUE(fixedAllocator.good());
    _1 = fixedAllocator.alloc(1022); EXPECT_TRUE(fixedAllocator.good());
    
    EXPECT_NO_THROW(fixedAllocator.free(_1)); EXPECT_TRUE(fixedAllocator.good());
    _1 = fixedAllocator.alloc(1022); EXPECT_TRUE(fixedAllocator.good());
    
    EXPECT_TRUE(fixedAllocator.totalFreeSpace() == 0);
    EXPECT_TRUE(fixedAllocator.totalOccupiedSpace() == vectorBuffer.size());
    
    EXPECT_NO_THROW(fixedAllocator.free(_0)); EXPECT_TRUE(fixedAllocator.good());
    _0 = fixedAllocator.alloc(1022); EXPECT_TRUE(fixedAllocator.good());
    
    EXPECT_NO_THROW(fixedAllocator.free(_0)); EXPECT_TRUE(fixedAllocator.good());
    _0 = fixedAllocator.alloc(1022); EXPECT_TRUE(fixedAllocator.good());
    
    EXPECT_TRUE(fixedAllocator.totalFreeSpace() == 0);
    EXPECT_TRUE(fixedAllocator.totalOccupiedSpace() == vectorBuffer.size());
    
    EXPECT_NO_THROW(fixedAllocator.free(_0)); EXPECT_TRUE(fixedAllocator.good());
    EXPECT_NO_THROW(fixedAllocator.free(_2)); EXPECT_TRUE(fixedAllocator.good());
    EXPECT_NO_THROW(fixedAllocator.free(_3)); EXPECT_TRUE(fixedAllocator.good());
    EXPECT_NO_THROW(fixedAllocator.free(_1)); EXPECT_TRUE(fixedAllocator.good());
    
    EXPECT_TRUE(fixedAllocator.totalFreeSpace() == vectorBuffer.size());
    EXPECT_TRUE(fixedAllocator.totalOccupiedSpace() == 0);
    
    //
    //
    //
    
    _0 = fixedAllocator.alloc(1022); EXPECT_TRUE(fixedAllocator.good());
    _1 = fixedAllocator.alloc(1022); EXPECT_TRUE(fixedAllocator.good());
    _2 = fixedAllocator.alloc(1022); EXPECT_TRUE(fixedAllocator.good());
    _3 = fixedAllocator.alloc(1022); EXPECT_TRUE(fixedAllocator.good());
    
    EXPECT_TRUE(fixedAllocator.totalFreeSpace() == 0);
    EXPECT_TRUE(fixedAllocator.totalOccupiedSpace() == vectorBuffer.size());
    
    EXPECT_NO_THROW(fixedAllocator.free(_2)); EXPECT_TRUE(fixedAllocator.good());
    EXPECT_NO_THROW(fixedAllocator.free(_1)); EXPECT_TRUE(fixedAllocator.good());
    
    EXPECT_TRUE(fixedAllocator.totalFreeSpace() == (vectorBuffer.size() / 2));
    EXPECT_TRUE(fixedAllocator.totalOccupiedSpace() == (vectorBuffer.size() / 2));
    
    _1 = fixedAllocator.alloc(2046); EXPECT_TRUE(fixedAllocator.good());
    
    EXPECT_TRUE(fixedAllocator.totalFreeSpace() == 0);
    EXPECT_TRUE(fixedAllocator.totalOccupiedSpace() == vectorBuffer.size());
    
    EXPECT_NO_THROW(fixedAllocator.free(_0)); EXPECT_TRUE(fixedAllocator.good());
    EXPECT_NO_THROW(fixedAllocator.free(_1)); EXPECT_TRUE(fixedAllocator.good());
    EXPECT_NO_THROW(fixedAllocator.free(_3)); EXPECT_TRUE(fixedAllocator.good());
    
    EXPECT_TRUE(fixedAllocator.totalFreeSpace() == vectorBuffer.size());
    EXPECT_TRUE(fixedAllocator.totalOccupiedSpace() == 0);
    
    //
    //
    //
    
    _0 = fixedAllocator.alloc(1022); EXPECT_TRUE(fixedAllocator.good());
    _1 = fixedAllocator.alloc(1022); EXPECT_TRUE(fixedAllocator.good());
    _2 = fixedAllocator.alloc(1022); EXPECT_TRUE(fixedAllocator.good());
    _3 = fixedAllocator.alloc(1022); EXPECT_TRUE(fixedAllocator.good());
    
    EXPECT_TRUE(fixedAllocator.totalFreeSpace() == 0);
    EXPECT_TRUE(fixedAllocator.totalOccupiedSpace() == vectorBuffer.size());
    
    EXPECT_NO_THROW(fixedAllocator.free(_1)); EXPECT_TRUE(fixedAllocator.good());
    EXPECT_NO_THROW(fixedAllocator.free(_0)); EXPECT_TRUE(fixedAllocator.good());
    
    _0 = fixedAllocator.alloc(2046); EXPECT_TRUE(fixedAllocator.good());
    
    EXPECT_TRUE(fixedAllocator.totalFreeSpace() == 0);
    EXPECT_TRUE(fixedAllocator.totalOccupiedSpace() == vectorBuffer.size());
    
    EXPECT_NO_THROW(fixedAllocator.free(_0)); EXPECT_TRUE(fixedAllocator.good());
    EXPECT_NO_THROW(fixedAllocator.free(_2)); EXPECT_TRUE(fixedAllocator.good());
    EXPECT_NO_THROW(fixedAllocator.free(_3)); EXPECT_TRUE(fixedAllocator.good());
    
    EXPECT_TRUE(fixedAllocator.totalFreeSpace() == vectorBuffer.size());
    EXPECT_TRUE(fixedAllocator.totalOccupiedSpace() == 0);
    
    //
    //
    //
    
    _0 = fixedAllocator.alloc(1022); EXPECT_TRUE(fixedAllocator.good());
    _1 = fixedAllocator.alloc(1022); EXPECT_TRUE(fixedAllocator.good());
    _2 = fixedAllocator.alloc(1022); EXPECT_TRUE(fixedAllocator.good());
    _3 = fixedAllocator.alloc(1022); EXPECT_TRUE(fixedAllocator.good());
    
    EXPECT_TRUE(fixedAllocator.totalFreeSpace() == 0);
    EXPECT_TRUE(fixedAllocator.totalOccupiedSpace() == vectorBuffer.size());
    
    EXPECT_NO_THROW(fixedAllocator.free(_2)); EXPECT_TRUE(fixedAllocator.good());
    EXPECT_NO_THROW(fixedAllocator.free(_3)); EXPECT_TRUE(fixedAllocator.good());
    
    _2 = fixedAllocator.alloc(2046); EXPECT_TRUE(fixedAllocator.good());
    
    EXPECT_TRUE(fixedAllocator.totalFreeSpace() == 0);
    EXPECT_TRUE(fixedAllocator.totalOccupiedSpace() == vectorBuffer.size());
    
    EXPECT_NO_THROW(fixedAllocator.free(_0)); EXPECT_TRUE(fixedAllocator.good());
    EXPECT_NO_THROW(fixedAllocator.free(_1)); EXPECT_TRUE(fixedAllocator.good());
    EXPECT_NO_THROW(fixedAllocator.free(_2)); EXPECT_TRUE(fixedAllocator.good());
    
    EXPECT_TRUE(fixedAllocator.totalFreeSpace() == vectorBuffer.size());
    EXPECT_TRUE(fixedAllocator.totalOccupiedSpace() == 0);
    
    //
    //
    //
}

} // namespace
