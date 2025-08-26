cmake_minimum_required(VERSION 3.25)

# 애플리케이션 소스 파일
set(APP_SOURCES
    Application.cpp
    main.cpp

    # -- Scene --
    scene/MainMenuScene.cpp
    scene/TestScene.cpp
    scene/LogoScene.cpp

    prefab/PlayerPrefab.cpp
)

# 애플리케이션 실행 파일 정의
add_executable(GNEngineApp ${APP_SOURCES})

# 애플리케이션 인클루드 디렉터리 설정
target_include_directories(GNEngineApp PRIVATE
    ${CMAKE_SOURCE_DIR}/include
    ${CMAKE_CURRENT_SOURCE_DIR}
)

# AppRootPath.h.in으로 AppRootPath.h 생성. 소스에서 프로젝트 경로 지정용.
configure_file(${CMAKE_CURRENT_SOURCE_DIR}/AppRootPath.h.in ${CMAKE_BINARY_DIR}/AppRootPath.h @ONLY)

# 애플리케이션 링크 라이브러리 설정
target_link_libraries(GNEngineApp PRIVATE GNEngine)

# AppRootPath.h가 생성되는 디렉토리를 GNEngineApp의 include 경로에 추가
target_include_directories(GNEngineApp PRIVATE ${CMAKE_BINARY_DIR})