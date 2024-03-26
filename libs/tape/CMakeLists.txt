file(GLOB SOURCE_FILES src/*.cpp)
file(GLOB HEADER_FILES include/*.h)

add_library(tape ${SOURCE_FILES} ${HEADER_FILES})
target_include_directories(tape PUBLIC include)
