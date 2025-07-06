
#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>
#include <iostream>
#include <memory>

// The original file included this, so we'll keep it.
#include "TextureManager.h"

// deleters for SDL resources
struct SDL_Window_Deleter {
    void operator()(SDL_Window* window) const {
        SDL_DestroyWindow(window);
    }
};
struct SDL_Renderer_Deleter {
    void operator()(SDL_Renderer* renderer) const {
        SDL_DestroyRenderer(renderer);
    }
};

// Use unique_ptr with custom deleters for automatic resource management
using UniqueSDL_Window = std::unique_ptr<SDL_Window, SDL_Window_Deleter>;
using UniqueSDL_Renderer = std::unique_ptr<SDL_Renderer, SDL_Renderer_Deleter>;

class Application {
public:
    /* Construct Managers*/
    TextureManager textureManager;
    
    Application() = default; /* Default constructor */
    
    /* Initialize SDL, window, and renderer */
    int init() {
        
        if (SDL_Init(SDL_INIT_VIDEO || SDL_INIT_AUDIO) == 0) { /* Initialize SDL systems*/
            std::cerr << "SDL_Init Error: " << SDL_GetError() << std::endl;
            return -1;
        }
        
        m_window = UniqueSDL_Window(SDL_CreateWindow("Test main callback ...", 800, 600, SDL_WINDOW_RESIZABLE));
        if (!m_window) {
            SDL_Log("SDL_CreateWindow Error: %s", SDL_GetError());
            SDL_Quit();
            return -1;
        }
        m_renderer = UniqueSDL_Renderer(SDL_CreateRenderer(m_window.get(), nullptr));
        if (!m_renderer) {
            SDL_Log("SDL_CreateRenderer Error: %s", SDL_GetError());
            SDL_Quit();
            return -1;
        }

        /* Initialize Managers*/
        textureManager.initialize(m_renderer.get());
        textureManager.loadTexture("example_png.png", 0, 0, 1, 1);

        return 0; /* Success */
    }
    
    /* Cleanup SDL resources */
    void quit() {
        std::cout << "Cleaning up and quitting..." << std::endl;
        
        m_renderer.reset(); /* Destroys renderer */
        m_window.reset();   /* Destroys window */

        SDL_Quit();
    }

    /* Accessors for renderer and window */
    SDL_Renderer* getRenderer() const { return m_renderer.get(); }
    SDL_Window* getWindow() const { return m_window.get(); }

private:
    UniqueSDL_Window m_window;
    UniqueSDL_Renderer m_renderer;
};

/* Main application entry point for SDL_RunApp */
static int SDLCALL AppMain(int argc, char *argv[]) {
    Application app;
    
    if (app.init() != 0) {
        std::cerr << "Application initialization failed." << std::endl;
        return -1;
    }
     
    SDL_Event event;
    bool quit = false;

    /* Main loop */
    while (!quit) { 

        while (SDL_PollEvent(&event)) { /* Processing Events */
            if (event.type == SDL_EVENT_QUIT) {
                quit = true;
                
            } else if (event.type == SDL_EVENT_KEY_DOWN) {
                /* Log the key that was pressed */
                std::cout << "Key pressed: " << SDL_GetKeyName(event.key.key) << std::endl;
            }
        }

        /* Get current keyboard state */
        const bool* keyboardState = SDL_GetKeyboardState(nullptr);

        /* Check for multiple key presses */
        if (keyboardState[SDL_SCANCODE_W] && keyboardState[SDL_SCANCODE_A]) {
            SDL_Log("W and A are pressed!");
        }

        /* Clear the previous frame*/
        SDL_RenderClear(app.getRenderer());
                
        
        SDL_SetRenderDrawColor(app.getRenderer(), 255, 255, 255, 255);
        app.textureManager.renderTextureAll();
        
        /* Render the current frame on screen */
        SDL_RenderPresent(app.getRenderer());
    }
    
    app.quit();
    return 0;
}

int main(int argc, char* argv[]) {
    const int result = SDL_RunApp(argc, argv, AppMain, nullptr);
    
    if (result != 0) {
        std::cerr << "Application exited with an error." << std::endl;
    } else {
        std::cout << "Application exited successfully." << std::endl;
    }

    return result;
}