# GNEngine

GNEngine is a 2D game engine developed using Modern C++23 and SDL3.

## Features

- **DOD + ECS system**: Very efficient system. 
- **Event-Driven System**: Flexibly handles various events through an `
- **2D Rendering**: Renders textures and animation to the screen.
- **Sound**: Plays and manages WAV, MP3, and OGG format sounds via OpenAL Soft (with dr_libs and stb_vorbis for decoding).
Can use stereo sound to 3D sound.
- **Scene Management**: Easily switch and manage game scenes with a `SceneManager`.


## Used libraries

- SDL3 (v3.2.18): [https://github.com/libsdl-org/SDL](https://github.com/libsdl-org/SDL)
- SDL3_image (v3.2.4): [https://github.com/libsdl-org/SDL_image](https://github.com/libsdl-org/SDL_image)
- SDL3_ttf (v3.2.2): [https://github.com/libsdl-org/SDL_ttf](https://github.com/libsdl-org/SDL_ttf)
- OpenAL Soft (v1.24.3): [https://github.com/kcat/openal-soft](https://github.com/kcat/openal-soft)
- for decoding sound files :
    - dr_wav(v0.14.0): [https://github.com/mackron/dr_libs](https://github.com/mackron/dr_libs) 
    - dr_mp3(v0.7.1): [https://github.com/mackron/dr_libs](https://github.com/mackron/dr_libs)
    - dr_flac(v0.13.0): [https://github.com/mackron/dr_libs](https://github.com/nothings/dr_libs)
    - stb_vorbis(v1.22): [https://github.com/nothings/stb](https://github.com/nothings/stb)
- nlohmann/json(3.12.0): [https://github.com/nlohmann/json](https://github.com/nlohmann/json)
- google/flatBuffer(): [https://github.com/google/flatbuffers](https://github.com/google/flatbuffers)
- box2D(3.1.1): [https://github.com/erincatto/box2d](https://github.com/erincatto/box2d)

## Build

This project is built using CMake and `CMakePresets.json`. It requires GNU g++ (v15.1.0) and ninja (v1.13.0) or compatible compilers and build systems.

1.  **Configure CMake:**
    ```bash
    cmake --preset custom-preset
    ```

2.  **Build:**
    ```bash
    cmake --build out/build/custom-preset
    ```

## Buile and Run the Example

### Buile

```bash
cmake --build out/build/custom-preset --target {A folder name of example}
```

### Run

```bash
out/build/custom-preset/Debug/{A file name of example}.exe
```

## Directory Structure

- `src/GNEngine/`: All source codes of GNEngine headers.
- `include/engine/`: All header files of GNEngine. 
  - `core`: core files of engine.
  - `system/`: Perform engine logics
  - `manager/`: Contains various manager classes for core engine functionalities.
  - `component`: Defines the base component and other concrete components like TransformComponent.
  - `resurce/`: General purpose resources (images, fonts, sounds, etc.)
- `lib/`: External libraries to use