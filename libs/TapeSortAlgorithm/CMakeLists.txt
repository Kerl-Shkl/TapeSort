file(GLOB SOURCE_FILES src/*.cpp)
file(GLOB HEADER_FILES include/*.h)

add_library(TapeSortAlgorithm ${SOURCE_FILES} ${HEADER_FILES})
target_link_libraries(TapeSortAlgorithm PUBLIC tape)
target_include_directories(TapeSortAlgorithm PUBLIC include)
