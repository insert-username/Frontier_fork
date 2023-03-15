enable_testing()

add_executable(FRONTIER_MFA_ListTest ListTests.cpp)
target_link_libraries(FRONTIER_MFA_ListTest GTest::gtest_main FRONTIER_MFA)
