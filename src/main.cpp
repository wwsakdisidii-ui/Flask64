#include <SDL2/SDL.h>
#include <iostream>

int main(int argc, char* argv[]) {
    // Initialize SDL2 Video and Gamepad subsystems
    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_GAMECONTROLLER) < 0) {
        std::cerr << "SDL could not initialize! SDL_Error: " << SDL_GetError() << std::endl;
        return -1;
    }

    // Create the emulator window context
    SDL_Window* window = SDL_CreateWindow(
        "Custom N64 Emulator Instance",
        SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
        640, 480,
        SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE
    );

    if (!window) {
        std::cerr << "Window could not be created! SDL_Error: " << SDL_GetError() << std::endl;
        SDL_Quit();
        return -1;
    }

    std::cout << "[SUCCESS] Emulator Executable Started Successfully!" << std::endl;
    if (argc > 1) {
        std::cout << "Target ROM/Hack Queued: " << argv[1] << std::endl;
    } else {
        std::cout << "No ROM specified. Running empty core loop..." << std::endl;
    }

    // Basic Application execution window lifecycle loop
    bool isRunning = true;
    SDL_Event event;

    while (isRunning) {
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                isRunning = false;
            }
            // Handle emulator hotkeys (e.g., press ESC to close the engine)
            else if (event.type == SDL_KEYDOWN) {
                if (event.key.keysym.sym == SDLK_ESCAPE) {
                    isRunning = false;
                }
            }
        }
        
        // Delay to prevent the thread from consuming 100% CPU when idle
        SDL_Delay(16); 
    }

    // Clean up window memory allocations on close
    SDL_DestroyWindow(window);
    SDL_Quit();
    
    return 0;
}
