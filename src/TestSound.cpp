#define SDL_MAIN_USE_CALLBACKS 1
#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>
#include <string>
#include "config.h"

/* We will use this renderer to draw into this window every frame. */
struct AppState {
    int WINDOW_WIDTH = 1280;
    int WINDOW_HEIGHT = 720;

    SDL_Window* window = nullptr;
    SDL_Renderer* renderer = nullptr;

    // 오디오 관련
    SDL_AudioSpec wavSpec; //format knowledge(semplerate, channal)
    Uint8* wavBuffer = nullptr; //raw real audio data
    Uint32 wavLength = 0;
    SDL_AudioStream* stream = nullptr; //
};
static AppState *appState = new AppState(); /*allocate AppState */

/* This function runs once at startup. */
SDL_AppResult SDL_AppInit(void **appstate, int argc, char *argv[]){
    if (!appState) {
        return SDL_APP_FAILURE;
    }

    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) == 0) { //SDL 오디오 시스템 초기화
        SDL_Log("SDL_Init failed: %s", SDL_GetError());
        return SDL_APP_FAILURE;
    }
    
    if (!SDL_CreateWindowAndRenderer("play sound (example)", appState->WINDOW_WIDTH, appState->WINDOW_HEIGHT, 0, &appState->window, &appState->renderer)) {
        SDL_Log("Couldn't create window/renderer: %s", SDL_GetError());
        return SDL_APP_FAILURE;
    }

    // wav 파일 로드
    std::string wavPath = std::string(SOUND_ASSET_ROOT_PATH) + "pressure.wav";
    if (SDL_LoadWAV(wavPath.c_str(), &appState->wavSpec, &appState->wavBuffer, &appState->wavLength) == 0) {
        SDL_Log("Failed to load WAV file: %s, Error: %s", wavPath.c_str(), SDL_GetError());
        return SDL_APP_FAILURE;
    }

    // 오디오 스트림 생성
    appState->stream = SDL_OpenAudioDeviceStream(SDL_AUDIO_DEVICE_DEFAULT_PLAYBACK, &appState->wavSpec, nullptr, nullptr);
    if (!appState->stream) {
        SDL_Log("Failed to open audio stream: %s", SDL_GetError());
        SDL_free(appState->wavBuffer);
        return SDL_APP_FAILURE;
    }

    // 스트림 시작 (기본적으로 일시정지 상태이므로 재생 시작)
    SDL_ResumeAudioStreamDevice(appState->stream);

    return SDL_APP_CONTINUE;
}

/* This function runs once per frame, and is the heart of the program. */
SDL_AppResult SDL_AppIterate(void *appstate){
    SDL_RenderClear(appState->renderer);

    // 오디오 스트림에 데이터 공급 (한 번만 넣으면 반복 재생 X, 필요시 반복 구현)
    if (appState->stream && SDL_GetAudioStreamQueued(appState->stream) < int(appState->wavLength)) {
        SDL_PutAudioStreamData(appState->stream, appState->wavBuffer, appState->wavLength);
    }

    SDL_RenderPresent(appState->renderer);
    return SDL_APP_CONTINUE;
}

/* This function runs when a new event (mouse input, keypresses, etc) occurs. */
SDL_AppResult SDL_AppEvent(void *appstate, SDL_Event *event){
    switch(event->type){
        case SDL_EVENT_QUIT:
            return SDL_APP_SUCCESS;
        case SDL_EVENT_KEY_DOWN:
            if(event->key.key == SDLK_ESCAPE) { return SDL_APP_SUCCESS; }
            if(event->key.key == SDLK_SPACE) { 
                // 오디오 스트림이 실행 중(=일시정지 상태가 아님)일 때만 일시정지
                if (appState->stream != nullptr && !SDL_AudioStreamDevicePaused(appState->stream)) {
                    SDL_PauseAudioStreamDevice(appState->stream);
                }
                else {
                    SDL_ResumeAudioStreamDevice(appState->stream);
                }
            }
            break;
    }
    return SDL_APP_CONTINUE;
}

/* This function runs once at shutdown. */
void SDL_AppQuit(void *appstate, SDL_AppResult result){
    if (appState->stream) {
        SDL_DestroyAudioStream(appState->stream);
    }
    if (appState->wavBuffer) {
        SDL_free(appState->wavBuffer);
    }
    
    delete appState;
}