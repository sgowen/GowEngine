# For more information about using CMake with Android Studio, read the
# documentation: https://d.android.com/studio/projects/add-native-code.html

cmake_minimum_required(VERSION 3.22.1)

project("gowengine")

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -std=c++17 -fno-rtti -fno-unwind-tables")
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -O2")
set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -O3")

set(engine_source_dir ${CMAKE_CURRENT_SOURCE_DIR}/../../../../../..)

file(GLOB ENGINE_CPP_FILES "${engine_source_dir}/*/*/*.cpp")

include_directories(${engine_source_dir}/../deps/file-compat)
include_directories(${engine_source_dir}/../deps/Lua/lua-5.4.6/include)
include_directories(${engine_source_dir}/../deps/rapidjson/include)
include_directories(${engine_source_dir}/../src/deps/sol2/include)
include_directories(${engine_source_dir}/../deps/stb)
include_directories(${engine_source_dir})
include_directories(${engine_source_dir}/../include)
include_directories(${CMAKE_CURRENT_SOURCE_DIR})

# Creates your game shared library. The name must be the same as the
# one used for loading in your Kotlin/Java or AndroidManifest.txt files.
add_library(gowengine SHARED
        ${ENGINE_CPP_FILES})

# box2d
set(box2d_DIR ${engine_source_dir}/../deps/box2d)
option(BOX2D_BUILD_UNIT_TESTS "Build the Box2D unit tests" OFF)
option(BUILD_SHARED_LIBS "Build shared libraries" OFF)
option(BOX2D_BUILD_TESTBED "Build the Box2D testbed" OFF)
option(BOX2D_BUILD_DOCS "Build the Box2D documentation" OFF)
add_subdirectory(${box2d_DIR} ./box2d-bin)
include_directories(${box2d_DIR}/include)

# glfm
set(glfm_DIR ${engine_source_dir}/../deps/glfm)
option(BUILD_SHARED_LIBS "Build shared libraries" OFF)
add_subdirectory(${glfm_DIR} ./glfm-bin)
include_directories(${glfm_DIR}/include)

# lua
set(lua_DIR ${engine_source_dir}/../deps/Lua)
option(LUA_BUILD_AS_CXX "Build lua as C++" ON)
option(LUA_ENABLE_SHARED "Build dynamic liblua" OFF)
option(LUA_ENABLE_TESTING "Build and run tests" OFF)
add_subdirectory(${lua_DIR} ./lua-bin)
include_directories(${lua_DIR}/lua-5.4.6/include)

# libsndfile
set(libsndfile_DIR ${engine_source_dir}/../deps/libsndfile)
option(BUILD_SHARED_LIBS "Build shared libraries" ON)
option(BUILD_TESTING "Build testing" OFF)
add_subdirectory(${libsndfile_DIR} ./libsndfile-bin)
include_directories(${libsndfile_DIR}/include)

# openal-soft
set(openal_soft_DIR ${engine_source_dir}/../deps/openal-soft)
add_subdirectory(${openal_soft_DIR} ./openal-soft-bin)
include_directories(${openal_soft_DIR}/include)

# Configure libraries CMake uses to link your target library.
target_link_libraries(gowengine
        EGL
        GLESv2
        android
        log
        box2d
        glfm
        lua_static
        sndfile
        OpenSLES
        OpenAL)
