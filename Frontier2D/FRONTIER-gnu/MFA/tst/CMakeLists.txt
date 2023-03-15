enable_testing()

add_executable(FRONTIER_MFA_Tests
        ListTests.cpp
        VertexTests.cpp
        EdgeTests.cpp
        DegValuesTests.cpp
        GraphTests.cpp
        ClusterTests.cpp)
target_link_libraries(FRONTIER_MFA_Tests GTest::gtest_main FRONTIER_MFA)
