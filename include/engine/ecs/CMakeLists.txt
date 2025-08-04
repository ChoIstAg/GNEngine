add_library(engine_ecs
    EntityManager.cpp
)

target_include_directories(engine_ecs PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})