# GNEngine

GNEngine is a 2D game engine developed using Modern C++23 and SDL3.

## Features

- **Component-Based Architecture:** Allows for creating complex game objects by attaching reusable and independent components.
- **Event-Driven System:** Flexibly handles various events through an `EventManager`.
- **Input Management:** Detects and processes keyboard input with an `InputManager`.
- **2D Rendering:** Renders textures to the screen using a `RenderManager`.
- **Resource Management:**
    - `TextManager`: Loads TTF fonts to render text.
    - `TextureManager`: Loads and manages image files as textures.
    - `AnimationManager`: Creates and controls animations from sprite sheets or individual images.
    - `SoundManager`: Plays and manages WAV, MP3, and OGG format sounds via OpenAL Soft (with dr_libs and stb_vorbis for decoding).
- **Scene Management:** Easily switch and manage game scenes with a `SceneManager`.


## Used libraries

- SDL3 (v3.2.18): [https://github.com/libsdl-org/SDL](https://github.com/libsdl-org/SDL)
- SDL3_image (v3.2.4): [https://github.com/libsdl-org/SDL_image](https://github.com/libsdl-org/SDL_image)
- SDL3_ttf (v3.2.2): [https://github.com/libsdl-org/SDL_ttf](https://github.com/libsdl-org/SDL_ttf)
- OpenAL Soft (v1.24.3): [https://github.com/kcat/openal-soft](https://github.com/kcat/openal-soft)
- for decoding sound files :
    - dr_wav(v0.14.0): [https://github.com/mackron/dr_libs](https://github.com/mackron/dr_libs) 
    - dr_mp3(v0.7.1): [https://github.com/mackron/dr_libs](https://github.com/mackron/dr_libs)
    - dr_flac(v0.13.0): [https://github.com/mackron/dr_libs](https://github.com/mackron/dr_libs)
    - stb_vorbis(v1.22): [https://github.com/nothings/stb](https://github.com/nothings/stb)
- nlohmann/json(3.12.0): [https://github.com/nlohmann/json](https://github.com/nlohmann/json)

## Build & Run
This project use GNU g++(v15.1.0) and ninja(v1.13.0).

This project is built using CMake and `CMakePresets.json`.

1.  **Configure CMake:**
    ```bash
    cmake --preset custom-preset
    ```

2.  **Build:**
    ```bash
    cmake --build out/build/custom-preset
    ```

3.  **Run:**
    ```bash
    ./out/build/"Custom configure preset"/GNEngine.exe
    ```

## Directory Structure

- `src/`: Main application source code
- `include/engine/`: Core engine logic
- `include/engine/manager`: Contains various manager classes for core engine functionalities.
- `include/engine/component`: Defines the base component and other concrete components like TransformComponent.
- `asset/`: Game resources (images, fonts, sounds, etc.)
- `lib/`: External libraries