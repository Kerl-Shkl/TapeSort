file(GLOB SOURCE_FILES src/*.cpp)
file(GLOB HEADER_FILES include/*.h)

add_library(TapeSort ${SOURCE_FILES} ${HEADER_FILES})
target_link_libraries(TapeSort PUBLIC Tape)
target_include_directories(TapeSort PUBLIC include)
