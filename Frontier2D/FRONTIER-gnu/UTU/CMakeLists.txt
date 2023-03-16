add_library(FRONTIER_UTU SHARED
        ${CMAKE_CURRENT_SOURCE_DIR}/UTU.cpp)

target_link_libraries(FRONTIER_UTU FRONTIER_MFA)