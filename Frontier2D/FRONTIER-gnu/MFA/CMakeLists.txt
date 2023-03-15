
add_library(FRONTIER_MFA SHARED
        entities/GlobalState.h
        entities/Vertex.cpp
        entities/Edge.cpp
        entities/DegValues.cpp
        entities/Cluster.cpp
        entities/Graph.cpp
        entities/Util.cpp
        output/DRPrintUtils.cpp
        algo/DistributeTree.cpp
        algo/Distribution.cpp
        algo/Graph.cpp
        algo/Groups.cpp
        algo/MinimalDense.cpp
        solver/Solver.cpp)
set_target_properties(FRONTIER_MFA PROPERTIES LINKER_LANGUAGE CXX)
target_include_directories(FRONTIER_MFA
        PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR})

add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/tst)