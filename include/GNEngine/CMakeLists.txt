# GNEngineRootPath.h.in으로 GNEngineRootPath.h 생성. 소스에서 프로젝트 경로 지정용.
configure_file(${CMAKE_CURRENT_SOURCE_DIR}/GNEngineRootPath.h.in ${CMAKE_BINARY_DIR}/GNEngineRootPath.h @ONLY)
include_directories(${CMAKE_BINARY_DIR})

add_subdirectory(manager)
add_subdirectory(system)
add_subdirectory(component)
add_subdirectory(resource)