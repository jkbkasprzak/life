cmake_minimum_required(VERSION 3.24)

project(life VERSION 0.1.0)

option(BUILD_SHARED_LIBS "Build shared library" ON)
option(ENABLE_TESTING "Enable testing" ON)

set(CMAKE_C_STANDARD 99)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_COMPILE_WARNING_AS_ERROR ON)

set(TARGET_NAME ${CMAKE_PROJECT_NAME})
file(GLOB_RECURSE SRC_FILES
  include/*.h
  src/*.h
  src/*.c
)
add_library(${TARGET_NAME} ${SRC_FILES})
target_include_directories(${TARGET_NAME} PUBLIC include)
target_compile_definitions(${TARGET_NAME} PRIVATE GOL_BUILD)
if(BUILD_SHARED_LIBS)
target_compile_definitions(${TARGET_NAME} PUBLIC GOL_SHARED)
endif()

if(ENABLE_TESTING)
  enable_testing()
  add_subdirectory(tests)
endif()
