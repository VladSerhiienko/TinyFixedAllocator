cmake_minimum_required(VERSION 3.4.1)

include(ExternalProject)

set(CMAKE_OSX_DEPLOYMENT_TARGET "10.15" CACHE STRING "Minimum OS X deployment version")
project (TinyFixedAllocator CXX)

message(STATUS "CMAKE_SYSTEM_INFO_FILE = ${CMAKE_SYSTEM_INFO_FILE}")
message(STATUS "CMAKE_SYSTEM_NAME = ${CMAKE_SYSTEM_NAME}")
message(STATUS "CMAKE_SYSTEM_PROCESSOR = ${CMAKE_SYSTEM_PROCESSOR}")
message(STATUS "CMAKE_SYSTEM = ${CMAKE_SYSTEM}")
message(STATUS "CMAKE_SOURCE_DIR = ${CMAKE_SOURCE_DIR}")
message(STATUS "CMAKE_BINARY_DIR = ${CMAKE_BINARY_DIR}")
message(STATUS "CMAKE_GENERATOR = ${CMAKE_GENERATOR}")

#
#
# platform decisions
#
#

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -D_DEBUG -DNV_EXTENSIONS=1")
set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -DNDEBUG -DNV_EXTENSIONS=1")

include(CheckCXXCompilerFlag)
CHECK_CXX_COMPILER_FLAG("/std:c++latest" COMPILER_SUPPORTS_CXXLATEST)
CHECK_CXX_COMPILER_FLAG("-std=c++17" COMPILER_SUPPORTS_CXX17)
CHECK_CXX_COMPILER_FLAG("-std=c++11" COMPILER_SUPPORTS_CXX11)

if (COMPILER_SUPPORTS_CXXLATEST)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /std:c++latest")
elseif(COMPILER_SUPPORTS_CXX17)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++17")
elseif(COMPILER_SUPPORTS_CXX11)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
endif()

if(NOT DEFINED TINY_FIXED_ALLOCATOR_BUILD_WITH_COVERAGE)
    set(TINY_FIXED_ALLOCATOR_BUILD_WITH_COVERAGE FALSE)
endif()

if (${TINY_FIXED_ALLOCATOR_BUILD_WITH_COVERAGE})
    message(STATUS "Enabling coverage.")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g ")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O0")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fprofile-arcs")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -ftest-coverage")
    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} --coverage")
endif()

get_filename_component(BUILD_FOLDER_SUFFIX ${CMAKE_BINARY_DIR} NAME)
message(STATUS "BUILD_FOLDER_SUFFIX = ${BUILD_FOLDER_SUFFIX}")

set(default_cmake_args -G "${CMAKE_GENERATOR}")

#
#
# googletest
#
#

ExternalProject_Add(
    googletest
    GIT_REPOSITORY "git@github.com:google/googletest.git"
    GIT_TAG "release-1.10.0"
    SOURCE_DIR "${CMAKE_SOURCE_DIR}/dependencies/googletest"
    UPDATE_COMMAND ""
    PATCH_COMMAND ""
    CMAKE_ARGS ${default_cmake_args} -Dgtest_force_shared_crt:BOOL=ON
    TEST_COMMAND ""
    INSTALL_COMMAND ""
    LOG_DOWNLOAD ON
)

ExternalProject_Get_Property(googletest SOURCE_DIR)
ExternalProject_Get_Property(googletest BINARY_DIR)
set(googletest_source_dir ${SOURCE_DIR})
set(googletest_binary_dir ${BINARY_DIR})
message(STATUS "googletest_source_dir = ${googletest_source_dir}")
message(STATUS "googletest_binary_dir = ${googletest_binary_dir}")

#
#
# taskflow
#
#

ExternalProject_Add(
    taskflow
    GIT_REPOSITORY "git@github.com:cpp-taskflow/cpp-taskflow.git"
    GIT_TAG "master"
    SOURCE_DIR "${CMAKE_SOURCE_DIR}/dependencies/cpp-taskflow"
    CONFIGURE_COMMAND ""
    BUILD_COMMAND ""
    INSTALL_COMMAND ""
    UPDATE_COMMAND ""
    PATCH_COMMAND ""
    LOG_DOWNLOAD ON
)

ExternalProject_Get_Property(taskflow SOURCE_DIR)
set(taskflow_source_dir ${SOURCE_DIR})
message(STATUS "taskflow_source_dir = ${taskflow_source_dir}")

add_executable(
    TinyFixedAllocatorTests
    ${CMAKE_SOURCE_DIR}/src/TinyFixedAllocator.hh
    ${CMAKE_SOURCE_DIR}/test/TinyFixedAllocatorTest.cc
    )

target_include_directories(
    TinyFixedAllocatorTests
    PUBLIC
    ${CMAKE_SOURCE_DIR}/dependencies/googletest/googlemock/include
    ${CMAKE_SOURCE_DIR}/dependencies/googletest/googletest/include
    ${CMAKE_SOURCE_DIR}/src
    ${CMAKE_SOURCE_DIR}/test
    ${taskflow_source_dir}
    )

add_dependencies(
    TinyFixedAllocatorTests
    googletest
    taskflow
)

# TODO(vserhiienko): Add Windows, Linux.
if (${TINY_FIXED_ALLOCATOR_BUILD_WITH_COVERAGE})
    target_link_libraries(
        TinyFixedAllocatorTests
        ${googletest_binary_dir}/lib/libgmock.a
        ${googletest_binary_dir}/lib/libgtest.a
        ${googletest_binary_dir}/lib/libgtest_main.a
        )
else()
    target_link_libraries(
        TinyFixedAllocatorTests
        debug ${googletest_binary_dir}/lib/Debug/libgmockd.a
        debug ${googletest_binary_dir}/lib/Debug/libgtestd.a
        debug ${googletest_binary_dir}/lib/Debug/libgtest_maind.a
        optimized ${googletest_binary_dir}/lib/Release/libgmock.a
        optimized ${googletest_binary_dir}/lib/Release/libgtest.a
        optimized ${googletest_binary_dir}/lib/Release/libgtest_main.a
        )
endif()

set_target_properties(
    TinyFixedAllocatorTests
    PROPERTIES
    VS_DEBUGGER_WORKING_DIRECTORY
    "$(OutDir)"
)

set_property(GLOBAL PROPERTY USE_FOLDERS ON)
set(PREDEFINED_TARGETS_FOLDER "CustomTargets")
