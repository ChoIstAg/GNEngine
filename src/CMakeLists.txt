# engine 디렉토리와 그 하위의 모든 .cpp 파일들을 찾아서 ENGINE_SOURCES 변수에 저장
file(GLOB_RECURSE ENGINE_SOURCES ${CMAKE_SOURCE_DIR}/include/engine/*.cpp)

add_executable(GNEngine 
    main.cpp

    MainCallback.cpp
    Application.cpp

    # engine의 모든 소스 파일 추가
    #${ENGINE_SOURCES} 주석화 사유: 오류가 해결 안된 소스 파일 때문에 컴파일 에러 발생함.

    ../include/engine/event/InputManager.cpp
    #../include/engine/event/RenderManager.cpp
    
    ../include/engine/object/BlankObject.cpp

    ../include/engine/resource/Texture.cpp
    ../include/engine/resource/TextureManager.cpp

    #../include/engine/scene/SceneManager.cpp
)

# GNEngine 타겟에 필요한 include 경로를 설정합니다.
target_include_directories(GNEngine PRIVATE 
    ${CMAKE_SOURCE_DIR}/src
    ${CMAKE_SOURCE_DIR}/include
    ${SDL3_INCLUDE_DIR}
    # SDL_image와 SDL_ttf의 include 경로는 find_package를 통해 자동으로 처리될 수 있으나, 명시적으로 추가해 줄 수도 있습니다.
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