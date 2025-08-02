# engine 디렉토리와 그 하위의 모든 .cpp 파일들을 찾아서 ENGINE_SOURCES 변수에 저장
add_executable(GNEngine 
    main.cpp
    Application.cpp
    
    # 모든 Scene 추가
    scene/TestScene.cpp


    # engine의 모든 소스 파일 추가
    #${ENGINE_SOURCES} 주석화 사유: 오류가 해결 안된 소스 파일 때문에 컴파일 에러 발생함.
    ../include/engine/manager/event/InputManager.cpp
    ../include/engine/manager/render/RenderManager.cpp
    
    ../include/engine/manager/sound/SoundManager.cpp
    ../include/engine/resource/sound/Sound.cpp
    ../include/engine/manager/resource/TextureManager.cpp
    ../include/engine/manager/resource/TextManager.cpp
    ../include/engine/resource/text/Text.cpp

    ../include/engine/component/SoundComponent.cpp
    ../include/engine/component/RenderComponent.cpp
    ../include/engine/component/TextComponent.cpp

    ../include/engine/object/TestObject.cpp
    ../include/engine/object/TextObject.cpp

    ../include/engine/manager/scene/SceneManager.cpp
)

add_subdirectory(scene)

# GNEngine 타겟에 필요한 include 경로를 설정합니다.
target_include_directories(GNEngine PRIVATE 
    ${CMAKE_SOURCE_DIR}/src
    ${CMAKE_SOURCE_DIR}/include
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

# 빌드 후 OpenAL DLL과 구현 DLL을 실행 파일 디렉토리로 복사합니다.
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
