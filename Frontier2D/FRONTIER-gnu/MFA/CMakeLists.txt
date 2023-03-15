
add_library(FRONTIER_MFA SHARED
        GlobalState.cpp
        entities/Vertex.cpp
        entities/Edge.cpp
        entities/DegValues.cpp
        entities/Cluster.cpp
        entities/Graph.cpp)
set_target_properties(FRONTIER_MFA PROPERTIES LINKER_LANGUAGE CXX)
target_include_directories(FRONTIER_MFA
        PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR})

add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/tst)