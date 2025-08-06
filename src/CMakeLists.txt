# engine 디렉토리와 그 하위의 모든 .cpp 파일들을 찾아서 ENGINE_SOURCES 변수에 저장
file(GLOB_RECURSE ENGINE_SYSTEM_SOURCES "${CMAKE_SOURCE_DIR}/include/engine/system/*.cpp")
file(GLOB_RECURSE ENGINE_MANAGER_SOURCES "${CMAKE_SOURCE_DIR}/include/engine/manager/*.cpp")
file(GLOB_RECURSE ENGINE_COMPONENT_SOURCES "${CMAKE_SOURCE_DIR}/include/engine/component/*.cpp")
file(GLOB ENGINE_RESOURCE_SOURCES "${CMAKE_SOURCE_DIR}/include/engine/resource/*.cpp")
file(GLOB_RECURSE ENGINE_PREFAB_SOURCES "${CMAKE_SOURCE_DIR}/include/engine/prefab/*.cpp")

add_executable(GNEngine 
    main.cpp
    Application.cpp
    
    # Scenes
    scene/TestScene.cpp
    scene/MainMenuScene.cpp

    # engine의 모든 소스 파일 추가
    ${ENGINE_SYSTEM_SOURCES}
    ${ENGINE_MANAGER_SOURCES}
    ${ENGINE_COMPONENT_SOURCES}
    ${ENGINE_RESOURCE_SOURCES}
    ${ENGINE_PREFAB_SOURCES}
)

add_subdirectory(scene)

# GNEngine 타겟에 필요한 include 경로를 설정합니다.
target_include_directories(GNEngine PRIVATE 
    ${CMAKE_SOURCE_DIR}/src
    ${CMAKE_SOURCE_DIR}/include
    ${CMAKE_SOURCE_DIR}/include/engine/core
    ${CMAKE_SOURCE_DIR}/include/engine/system
    ${CMAKE_SOURCE_DIR}/include/engine/manager
    ${CMAKE_SOURCE_DIR}/include/engine/component
    ${CMAKE_SOURCE_DIR}/include/engine/resource
    ${CMAKE_SOURCE_DIR}/include/engine/prefab

    ${SDL3_INCLUDE_DIR}
    ${SDL3_image_INCLUDE_DIR}
    ${SDL3_ttf_INCLUDE_DIR}
)

# GNEngine 타겟이 링크해야 할 라이브러리들을 설정합니다.
target_link_libraries(GNEngine PRIVATE
    SDL3::SDL3
    SDL3_image::SDL3_image
    SDL3_ttf::SDL3_ttf
)

# Copy SDL3.dll to the output directory after building the target
# This is necessary for the executable to run, as it depends on SDL3.dll
add_custom_command(TARGET GNEngine POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different

    ${SDL3_DLL_DIR}/SDL3.dll
    ${SDL3_image_DLL_DIR}/SDL3_image.dll
    ${SDL3_ttf_DLL_DIR}/SDL3_ttf.dll

    $<TARGET_FILE_DIR:GNEngine>
)

# OpenAL DLL을 빌드 파일에 복사
add_custom_command(TARGET GNEngine POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
    "${OPENAL_DIR}/router/Win64/OpenAL32.dll"
    $<TARGET_FILE_DIR:GNEngine>
)
add_custom_command(TARGET GNEngine POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
    "${OPENAL_DIR}/bin/Win64/soft_oal.dll"
    $<TARGET_FILE_DIR:GNEngine>
)