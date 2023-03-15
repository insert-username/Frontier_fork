
add_library(FRONTIER_MFA SHARED mfa1List.h)
set_target_properties(FRONTIER_MFA PROPERTIES LINKER_LANGUAGE CXX)
target_include_directories(FRONTIER_MFA
        PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR})

add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/tst)