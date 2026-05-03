set(TARGET_NAME life_func_tests)

file(GLOB_RECURSE SRC_FILES
  *.hpp
  *.cpp
)
add_executable(${TARGET_NAME} ${SRC_FILES})
target_link_libraries(${TARGET_NAME} PRIVATE life::life GTest::gtest_main)

include(GoogleTest)
gtest_discover_tests(${TARGET_NAME})
