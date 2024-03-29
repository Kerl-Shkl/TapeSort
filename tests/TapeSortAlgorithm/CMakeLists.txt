function(create_tapesort_test SOURCE_NAME)
    get_filename_component(TARGET_NAME ${SOURCE_NAME} NAME_WE)
    add_executable(${TARGET_NAME})
    target_sources(${TARGET_NAME} PRIVATE "src/${SOURCE_NAME}")
    target_link_libraries(${TARGET_NAME} PRIVATE TapeSortAlgorithm GTest::gtest_main)
    gtest_discover_tests(${TARGET_NAME})
endfunction()

set(TEST_FILES MergeTapesTest.cpp TapeSortMergeToDestinationTest.cpp
    TapeSortMergeToIntermTest.cpp TapeSortTest.cpp)
foreach(TEST_FILE ${TEST_FILES})
    create_tapesort_test(${TEST_FILE})
endforeach()
