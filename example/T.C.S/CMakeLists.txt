cmake_minimum_required(VERSION 3.25)

# 애플리케이션 소스 파일
set(TOP_VIEW_GAME_SOURCES
    main.cpp
    Application.cpp

    # -- Scene --
    scene/MainMenuScene.cpp
    scene/TestScene.cpp
    scene/LogoScene.cpp

    # -- Prefab --
    prefab/PlayerPrefab.cpp
)

# 애플리케이션 실행 파일 정의
add_executable(top_view_game ${TOP_VIEW_GAME_SOURCES})

# 애플리케이션 인클루드 디렉터리 설정
target_include_directories(top_view_game PRIVATE
    ${CMAKE_SOURCE_DIR}/include
    ${CMAKE_CURRENT_SOURCE_DIR}
)

# AppRootPath.h.in으로 AppRootPath.h 생성. 소스에서 프로젝트 경로 지정용.
configure_file(${CMAKE_CURRENT_SOURCE_DIR}/AppRootPath.h.in ${CMAKE_BINARY_DIR}/generated/AppRootPath.h @ONLY)

# 애플리케이션 링크 라이브러리 설정
target_link_libraries(top_view_game PRIVATE GNEngine)

# AppRootPath.h가 생성되는 디렉토리를 top_view_game의 include 경로에 추가
target_include_directories(top_view_game PRIVATE ${CMAKE_BINARY_DIR}/generated)