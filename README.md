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


## Key Technologies

- Modern C++23
- CMake
- SDL3 (v3.2.18)
- SDL3_image (v3.2.4)
- SDL3_ttf (v3.2.2)
- OpenAL Soft (v1.24.3)
    (with dr_wav(v0.14.0) and dr_mp3(v0.7.1) and stb_vorbis(v1.22) for file decoding)

## Build & Run

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