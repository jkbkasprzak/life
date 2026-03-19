set(TARGET_NAME life_core)

file(GLOB_RECURSE SRC_FILES
  include/*.h
  src/*.h
  src/*.c
)
add_library(${TARGET_NAME} ${SRC_FILES})
target_include_directories(${TARGET_NAME} PUBLIC include)

add_subdirectory(tests)
