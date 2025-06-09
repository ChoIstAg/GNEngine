#define SDL_MAIN_USE_CALLBACKS 1  /* use the callbacks instead of main() */
#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>
#include <SDL3_image/SDL_image.h>
//#include <SDL3_ttf/SDL_ttf.h>

#include <iostream>
#include <vector>
#include <memory>
#include <string>
//#include <fstream>

//#include "texture.hpp"


#define PROJECT_PATH R"(c:\Development\code\project\sdl3_test\asset\)" //R"()"는 \를 처리 X

constexpr int WINDOW_WIDTH = 1280;
constexpr int WINDOW_HEIGHT = 720;


/* We will use this renderer to draw into this window every frame. */
typedef struct {
    SDL_Window* window = nullptr;
    SDL_Renderer* renderer = nullptr;
} AppState;
static AppState* app_state = new AppState(); /* Allocate AppState */


class Texture {
private:
    SDL_Surface* surface;
    SDL_Texture* texture;
    std::string file_path; 
    SDL_FRect dst_rect; // Destination rectangle for rendering. It was 
    
    float x, y, scale_x, scale_y, width, height;
    
    public:

    explicit Texture(std::string file_name, float x, float y, float scale_x, float scale_y) /*initalizer*/
    : x(x), y(y), scale_x(x), scale_y(y), surface(nullptr), texture(nullptr) {
   
        // Construct the full path to the img file
        file_path = PROJECT_PATH + file_name;
        std::cout << "Loading texture from: " << file_path << std::endl;
        

        /* Check if the file extension is ".bmp" or ".png" */
        if(file_name.substr(file_name.size() - 4, 4) == ".bmp"){
            // Load the BMP file into a surface
            surface = SDL_LoadBMP(file_path.c_str());
            if (surface == NULL) { //checking error
                SDL_Log("SDL_LoadBMP failed: %s", SDL_GetError());
                return;
            }
        }
        else if(file_name.substr(file_name.size() - 4, 4) == ".png"){
            // Load the PNG file into a surface
            surface = IMG_Load(file_path.c_str());
            if (surface == NULL) {
                SDL_Log("IMG_Load failed: %s", SDL_GetError());
                return;
            }
        } 
        else {
            SDL_Log("Unsupported file format: %s", file_path.c_str());
            return;
        }
        
        // Create a texture from the surface
        texture = SDL_CreateTextureFromSurface(app_state->renderer, surface);
        if (texture == NULL) {
            SDL_Log("SDL_CreateTextureFromSurface failed: %s", SDL_GetError());
        }
        
        
        // Set the size and position of the texture
        SDL_GetTextureSize(texture, &width, &height);
        dst_rect = {x, y, width * scale_x, height * scale_y};
        std::cout << "dst_rect.w = " << dst_rect.w << " dst.rect.h = " << dst_rect.h << std::endl;
    }
    
    ~Texture() {
        // Free objects
        SDL_DestroySurface(surface);
        SDL_DestroyTexture(texture);
        std::cout << "Texture destroyed" << std::endl;
    }
    
    void render_texture() {
        if(texture) {
            SDL_RenderTexture(app_state->renderer, texture, NULL, &dst_rect);
        }
    }

    void moving_texture(int direction) {
        switch(direction) {;
            case 0 : // Move right
                dst_rect.x += 100.0f; // Move right
                break;
            case 1: // Move left
                dst_rect.x -= 100.0f; // Move left
                break;
            case 2: // Move down
                dst_rect.y += 100.0f; // Move down
                break;
            case 3: // Move up%
                dst_rect.y -= 100.0f; // Move up
                break;
        }
    }

    int get_position(){
        return dst_rect.x, dst_rect.y;
    }

    void turn_texture(float angle) {
        
    }
};  
std::vector<std::unique_ptr<Texture>> textures; // Use a vector of unique_ptr to manage the Texture objects
static int selected_texture = 0; // Variable to store the selected texture


/* This function runs once at startup. */
SDL_AppResult SDL_AppInit(void** appstate, int argc, char* argv[]) {
    /* Initialize SDL */

    if(!app_state) {
        return SDL_APP_FAILURE;
    }

    
    if(!SDL_CreateWindowAndRenderer("SDL3 test main", WINDOW_WIDTH, WINDOW_HEIGHT, 0, &app_state->window, &app_state->renderer)) {
        SDL_Log("Couldn't create window/renderer: %s", SDL_GetError());
        return SDL_APP_FAILURE;   
    }


    textures.emplace_back(std::make_unique<Texture>("example_png.png", 500.0f, 0.0f, 1.0f, 1.0f));
    textures.emplace_back(std::make_unique<Texture>("example_bmp.bmp", 100.0f, 0.0f, 1.0f, 1.0f));
    
    return SDL_APP_CONTINUE;
}

/* This function runs once per frame, and is the heart of the program. */

SDL_AppResult SDL_AppIterate(void* appstate) {
    /* Clear the screen */
    //SDL_SetRenderDrawColor(app_state->renderer, 0, 0, 0, 255);
    SDL_RenderClear(app_state->renderer);
    
    
    /*Render the texture*/
    for (auto& tex : textures) {
        tex->render_texture();
        
        tex->get_position();
    }
    
    /* Update the screen*/
    SDL_RenderPresent(app_state->renderer);
    
    return SDL_APP_CONTINUE;
}

/* This function runs when a new event (mouse input, keypresses, Focus, etc) occurs. */
SDL_AppResult SDL_AppEvent(void* appstate, SDL_Event* event) {

    switch(event->type) {
    case SDL_EVENT_QUIT:
        return SDL_APP_SUCCESS; /* Successful termination */

    case SDL_EVENT_KEY_DOWN:

        if (event->key.key == SDLK_ESCAPE) {
            return SDL_APP_SUCCESS; /* Successful termination */
        }
        
        
        if(event->key.key == SDLK_1){
            selected_texture = 1;
        }
        if(event->key.key == SDLK_2){
            selected_texture = 2;
        }

        if(event->key.key == SDLK_RIGHT){
            textures[selected_texture]->moving_texture(0); // Move right
        }
        if(event->key.key == SDLK_LEFT){
            textures[selected_texture]->moving_texture(1); // Move left
        }
        if(event->key.key == SDLK_DOWN){
            textures[selected_texture]->moving_texture(2); // Move down
        }
        if(event->key.key == SDLK_UP){
            textures[selected_texture]->moving_texture(3); // Move up
        }
        if(event->key.key == SDLK_SPACE){
            std::cout << "Selected texture: " << selected_texture << std::endl;
        }
        break;
        if(event->key.key == SDLK_T){
            textures[selected_texture]->turn_texture(90.0f); // Turn the texture
        }

    case SDL_EVENT_KEY_UP:
        if (event->key.key == SDLK_ESCAPE) {
            return SDL_APP_SUCCESS; /* Successful termination */
        }

        break;
    }

    return SDL_APP_CONTINUE;
}

/* This function runs once at shutdown. */
void SDL_AppQuit(void* appstate, SDL_AppResult result) {

    delete app_state; // Free the AppState object

    std::cout << "SDL_AppQuit called" << std::endl;


    SDL_Quit();
}