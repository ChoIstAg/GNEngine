cmake_minimum_required(VERSION 3.25)
project(GNEngine VERSION 0.1.0 LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_CXX_FLAGS "-std=c++23")

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/$<CONFIGURATION>")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/$<CONFIGURATION>")

# SDL 경로 설정
set(SDL3_DIR "${CMAKE_SOURCE_DIR}/lib/SDL3-3.2.18/x86_64-w64-mingw32/lib/cmake/SDL3")
set(SDL3_DLL_DIR "${CMAKE_SOURCE_DIR}/lib/SDL3-3.2.18/x86_64-w64-mingw32/bin")
set(SDL3_INCLUDE_DIR "${CMAKE_SOURCE_DIR}/lib/SDL3-3.2.18/x86_64-w64-mingw32/include")
set(SDL3_image_DIR "${CMAKE_SOURCE_DIR}/lib/SDL3_image-3.2.4/x86_64-w64-mingw32/lib/cmake/SDL3_image")
set(SDL3_image_DLL_DIR "${CMAKE_SOURCE_DIR}/lib/SDL3_image-3.2.4/x86_64-w64-mingw32/bin")
set(SDL3_image_INCLUDE_DIR "${CMAKE_SOURCE_DIR}/lib/SDL3_image-3.2.4/x86_64-w64-mingw32/include")
set(SDL3_ttf_DIR "${CMAKE_SOURCE_DIR}/lib/SDL3_ttf-3.2.2/x86_64-w64-mingw32/lib/cmake/SDL3_ttf")
set(SDL3_ttf_DLL_DIR "${CMAKE_SOURCE_DIR}/lib/SDL3_ttf-3.2.2/x86_64-w64-mingw32/bin")
set(SDL3_ttf_INCLUDE_DIR "${CMAKE_SOURCE_DIR}/lib/SDL3_ttf-3.2.2/x86_64-w64-mingw32/include")

# OpenAL Soft 경로 설정
set(OPENAL_DIR "${CMAKE_SOURCE_DIR}/lib/openal-soft-1.24.3-bin")

# MinGW 런타임 경로 설정
set(MINGW_BIN_DIR "C:/Development_Environment/code/MSYS2/ucrt64/bin")

# config.h 생성. 소스에서 프로젝트 경로 지정용.
configure_file(${CMAKE_SOURCE_DIR}/include/config.h.in ${CMAKE_BINARY_DIR}/config.h @ONLY)
include_directories(${CMAKE_BINARY_DIR})

# SDL 패키지 찾기
find_package(SDL3 CONFIG REQUIRED)
find_package(SDL3_image CONFIG REQUIRED)
find_package(SDL3_ttf CONFIG REQUIRED)

# GNEngine 라이브러리 정의
add_library(GNEngine SHARED
    # 엔진 소스 파일들을 여기에 추가
    src/engine/component/AnimationComponent.cpp
    src/engine/component/PlayerAnimationControllerComponent.cpp
    src/engine/component/PlayerMovementComponent.cpp
    src/engine/component/RenderComponent.cpp
    src/engine/component/SoundComponent.cpp
    src/engine/manager/AnimationManager.cpp
    src/engine/manager/EntityManager.cpp
    src/engine/manager/InputManager.cpp
    src/engine/manager/RenderManager.cpp
    src/engine/manager/SceneManager.cpp
    src/engine/manager/SoundManager.cpp
    src/engine/manager/SystemManager.cpp
    src/engine/manager/TextManager.cpp
    src/engine/manager/TextureManager.cpp
    src/engine/prefab/PlayerPrefab.cpp
    src/engine/resource/Animation.cpp
    src/engine/resource/Sound.cpp
    src/engine/resource/Text.cpp
    src/engine/system/AccelerationResetSystem.cpp
    src/engine/system/AnimationSystem.cpp
    src/engine/system/CameraSystem.cpp
    src/engine/system/InputSystem.cpp
    src/engine/system/InputToAccelerationSystem.cpp
    src/engine/system/MovementSystem.cpp
    src/engine/system/PlayerAnimationControlSystem.cpp
    src/engine/system/RenderSystem.cpp
    src/engine/system/SoundSystem.cpp
)

# GNEngine 라이브러리 인클루드 디렉터리 설정
target_include_directories(GNEngine PUBLIC
    "${CMAKE_SOURCE_DIR}/include"
    "${CMAKE_SOURCE_DIR}/lib"
    "${SDL3_INCLUDE_DIR}"
    "${SDL3_image_INCLUDE_DIR}"
    "${SDL3_ttf_INCLUDE_DIR}"
    "${OPENAL_DIR}/include"
    "${CMAKE_SOURCE_DIR}/lib/dr_libs"
    "${CMAKE_SOURCE_DIR}/lib/ogg"
    "${CMAKE_SOURCE_DIR}/lib/nlohmann"
    "${CMAKE_SOURCE_DIR}/src/engine/component"
    "${CMAKE_SOURCE_DIR}/src/engine/core"
    "${CMAKE_SOURCE_DIR}/src/engine/manager"
    "${CMAKE_SOURCE_DIR}/src/engine/prefab"
    "${CMAKE_SOURCE_DIR}/src/engine/resource"
    "${CMAKE_SOURCE_DIR}/src/engine/system"
)

# GNEngine 라이브러리 링크 설정
target_link_libraries(GNEngine PUBLIC
    SDL3::SDL3
    SDL3_image::SDL3_image
    SDL3_ttf::SDL3_ttf
    "${OPENAL_DIR}/libs/Win64/OpenAL32.lib"
    stdc++fs
)

# GNEngine_EXPORTS 정의 (DLL 빌드 시 필요)
target_compile_definitions(GNEngine PRIVATE GNEngine_EXPORTS)

# DLL 복사
add_custom_command(TARGET GNEngine POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
    "${SDL3_DLL_DIR}/SDL3.dll"
    $<TARGET_FILE_DIR:GNEngine>
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
    "${SDL3_image_DLL_DIR}/SDL3_image.dll"
    $<TARGET_FILE_DIR:GNEngine>
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
    "${SDL3_ttf_DLL_DIR}/SDL3_ttf.dll"
    $<TARGET_FILE_DIR:GNEngine>
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
    "${OPENAL_DIR}/bin/Win64/soft_oal.dll"
    $<TARGET_FILE_DIR:GNEngine>/OpenAL32.dll
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
    "${MINGW_BIN_DIR}/libgcc_s_seh-1.dll"
    $<TARGET_FILE_DIR:GNEngine>
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
    "${MINGW_BIN_DIR}/libstdc++-6.dll"
    $<TARGET_FILE_DIR:GNEngine>
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
    "${MINGW_BIN_DIR}/libwinpthread-1.dll"
    $<TARGET_FILE_DIR:GNEngine>
    COMMENT "Copying dependent DLLs for GNEngine library."
)


# 애플리케이션 디렉터리 추가
add_subdirectory(app)

# Remove target_link_directories as it might not be needed with direct target linking
# target_link_directories(GNEngine PUBLIC ${CMAKE_LIBRARY_OUTPUT_DIRECTORY})

# target_compile_definitions
target_compile_definitions(GNEngine PUBLIC
    PROJECT_ROOT_PATH="${CMAKE_SOURCE_DIR}"
)
