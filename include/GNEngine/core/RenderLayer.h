#pragma once

enum class RenderLayer {
    BACKGROUND_FAR, // Sky
    BACKGROUND_MID, 
    BACKGROUND_NEAR,
    GAME_OBJECT,
    CRUCIAL_GAME_OBJECT, // Player
    FOREGROUND,
    VERY_CLOSE_FOREGROUND,
    GAME_EFFECT,
    CRUCIAL_GAME_EFFECT,
    UI,
    UI_EFFECT,
    SCENE_EFFECT, // Fade effect
    CRUCIAL_UI,  // Mouse Pointer
    COUNT // A number of Layer
};

