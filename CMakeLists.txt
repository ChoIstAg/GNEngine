cmake_minimum_required(VERSION 3.25)
project(Noma_s_home VERSION 0.1.0 LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/$<CONFIGURATION>")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/$<CONFIGURATION>")

# SDL 경로 설정
set(SDL3_DIR "${CMAKE_SOURCE_DIR}/include/lib/SDL3/x86_64-w64-mingw32/lib/cmake/SDL3")
set(SDL3_INCLUDE_DIR "${CMAKE_SOURCE_DIR}/include/lib/SDL3/x86_64-w64-mingw32/include")
set(SDL3_image_DIR "${CMAKE_SOURCE_DIR}/include/lib/SDL3_image/x86_64-w64-mingw32/lib/cmake/SDL3_image")
set(SDL3_ttf_DIR "${CMAKE_SOURCE_DIR}/include/lib/SDL3_ttf/x86_64-w64-mingw32/lib/cmake/SDL3_ttf")

# config.h 생성
#configure_file(${CMAKE_SOURCE_DIR}/include/config.h ${CMAKE_BINARY_DIR}/config.h @ONLY) -> 오류.
configure_file(${CMAKE_SOURCE_DIR}/include/config.h.in ${CMAKE_BINARY_DIR}/config.h @ONLY)
include_directories(${CMAKE_BINARY_DIR})

add_subdirectory(src)

# SDL 패키지 찾기
find_package(SDL3 CONFIG REQUIRED)
find_package(SDL3_image CONFIG REQUIRED)
find_package(SDL3_ttf CONFIG REQUIRED)

# target_include_directories
target_include_directories(Noma-s_home
PUBLIC
    #includ engine lib
    "${CMAKE_SOURCE_DIR}/include/engine"

    "${SDL3_INCLUDE_DIR}"
    "${CMAKE_SOURCE_DIR}/include/lib/SDL3_image/x86_64-w64-mingw32/include"
    "${CMAKE_SOURCE_DIR}/include/lib/SDL3_ttf/x86_64-w64-mingw32/include"
)

# target_link_libraries
target_link_libraries(Noma-s_home 
PUBLIC 
    SDL3::SDL3
    SDL3_image::SDL3_image
    SDL3_ttf::SDL3_ttf
)

# target_compile_definitions
target_compile_definitions(Noma-s_home
PUBLIC
    PROJECT_ROOT_PATH="${CMAKE_SOURCE_DIR}"
)