file(GLOB SOURCE_FILES src/*.cpp)
file(GLOB HEADER_FILES include/*.h)

add_library(Tape ${SOURCE_FILES} ${HEADER_FILES})
target_include_directories(Tape PUBLIC include)
