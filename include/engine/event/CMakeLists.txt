target_sources(GNEngine
PRIVATE
    #EventManager.cpp .cpp 구현 파일은 없음. .h 헤더에서 모두 구현되어있음. 
    EventListnerComponent.cpp
    InputManager.cpp
    RenderManager.cpp
)