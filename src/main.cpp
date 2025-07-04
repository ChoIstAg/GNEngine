#include <SDL3/SDL.h>
#include <SDL3_image/SDL_image.h>
#include <iostream>
#include <memory> // For std::unique_ptr

// The original file included this, so we'll keep it.
#include "TextureManager.h"

// Custom deleters for SDL resources
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
    Application() = default; // Default constructor

    // Initialize SDL, window, and renderer
    int init() {
        std::cout << "Initializing application..." << std::endl;

        if (SDL_Init(SDL_INIT_VIDEO) != 0) {
            std::cerr << "SDL_Init Error: " << SDL_GetError() << std::endl;
            return -1;
        }

        m_window = UniqueSDL_Window(SDL_CreateWindow("C++23 Lambda with SDL_RunApp", 800, 600, 0));
        if (!m_window) {
            std::cerr << "SDL_CreateWindow Error: " << SDL_GetError() << std::endl;
            SDL_Quit();
            return -1;
        }

        m_renderer = UniqueSDL_Renderer(SDL_CreateRenderer(m_window.get(), nullptr, SDL_RENDERER_ACCELERATED));
        if (!m_renderer) {
            std::cerr << "SDL_CreateRenderer Error: " << SDL_GetError() << std::endl;
            SDL_Quit();
            return -1;
        }
        return 0; // Success
    }

    // Cleanup SDL resources
    void quit() {
        std::cout << "Cleaning up and quitting..." << std::endl;
        m_renderer.reset(); // Destroys renderer
        m_window.reset();   // Destroys window
        IMG_Quit();
        SDL_Quit();
    }

    // Accessors for renderer and window
    SDL_Renderer* getRenderer() const { return m_renderer.get(); }
    SDL_Window* getWindow() const { return m_window.get(); }

private:
    UniqueSDL_Window m_window;
    UniqueSDL_Renderer m_renderer;
};

int main(int argc, char* argv[]) {
    Application app;

    const int result = SDL_RunApp(argc, argv, &app,
        // app_init: Called once at the beginning.
        [](void* app_state, int, char*[]) -> int {
            auto* app_instance = static_cast<Application*>(app_state);
            return app_instance->init();
        },
        // app_event: Called for each event.
        [](void* app_state, const SDL_Event* event) -> int {
            // The app_state is not used in this simple event handler, but is available.
            // auto* app_instance = static_cast<Application*>(app_state);
            if (event->type == SDL_EVENT_QUIT) {
                return 1; // Signal to quit the application
            }
            return 0; // Event handled, continue
        },
        // app_iterate: Called once per frame.
        [](void* app_state) -> int {
            auto* app_instance = static_cast<Application*>(app_state);

            // Set background color (dark blue)
            SDL_SetRenderDrawColor(app_instance->getRenderer(), 20, 20, 40, 255);
            SDL_RenderClear(app_instance->getRenderer());

            // --- Rendering happens here ---

            // Present the new frame
            SDL_RenderPresent(app_instance->getRenderer());

            return 0; // Continue running
        },
        // app_quit: Called once at the end.
        [](void* app_state) -> void {
            auto* app_instance = static_cast<Application*>(app_state);
            app_instance->quit();
        }
    );

    if (result != 0) {
        std::cerr << "Application exited with an error." << std::endl;
    } else {
        std::cout << "Application exited successfully." << std::endl;
    }

    return result;
}