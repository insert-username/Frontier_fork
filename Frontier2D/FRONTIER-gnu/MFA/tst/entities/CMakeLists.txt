
add_executable(FRONTIER_MFA_EntityTests
        ListTests.cpp
        VertexTests.cpp
        EdgeTests.cpp
        DegValuesTests.cpp
        GraphTests.cpp
        ClusterTests.cpp)
target_link_libraries(FRONTIER_MFA_EntityTests GTest::gtest_main FRONTIER_MFA)
