#include <SDL2/SDL_messagebox.h>
#include <SDL2/SDL_render.h>
#include <stdexcept>
#define SDL_MAIN_HANDLED

#include "Camera.h"
#include "AssetManager.h"

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_mixer.h>

#include <chrono>
#include <thread>
#include <string>
#include <iostream>

Camera* camera = nullptr;

SDL_Window* window = nullptr;
SDL_Renderer* renderer = nullptr;

bool isRunning = true;

bool isCooldown = true;
bool isFireKeyDown = false;

void loadResources();
void unloadResources();
void init();
void clean();
void update(float deltaTime);
void render(const Camera& camera);
void mainLoop();

int main(int argc, char** argv) {
    init();
    mainLoop();
    clean();

    return 0;
}

void init() {
    SDL_Init(SDL_INIT_EVERYTHING);
    IMG_Init(IMG_INIT_JPG | IMG_INIT_PNG);
    Mix_Init(MIX_INIT_MP3);
    TTF_Init();

    Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048);
    Mix_AllocateChannels(32);

    window = SDL_CreateWindow("Atlas", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 
                              1280, 720,
                              SDL_WINDOW_SHOWN);
    if (!window) {
        std::cerr << "Window creation failed: " << SDL_GetError() << std::endl;
        return;
    }

    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (!renderer) {
        std::cerr << "Renderer creation failed: " << SDL_GetError() << std::endl;
        return;
    }

    SDL_ShowCursor(SDL_DISABLE);

    try {
        AssetManager::instance()->load(renderer);
    } catch (const std::runtime_error& e) {
        const std::string errMsg = "Can't load assets: " + std::string(e.what());
        SDL_ShowSimpleMessageBox(
            SDL_MESSAGEBOX_ERROR, 
            "Loading assets failed", 
            errMsg.c_str(), 
            window);
    }

    camera = new Camera(renderer);
}

void clean() {
    delete camera;

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);

    TTF_Quit();
    Mix_Quit();
    IMG_Quit();
    SDL_Quit();
}

void mainLoop() {
    using namespace std::chrono;

    SDL_Event event;

    const nanoseconds frameDuration(100000000 / 144);
    steady_clock::time_point lastTick = steady_clock::now();

    while (isRunning) {
        while (SDL_PollEvent(&event)) {
            switch (event.type) {
                case SDL_QUIT:
                    isRunning = false; 
                    break;
            }
        }
        steady_clock::time_point frameStart = steady_clock::now();
        duration<float> deltaTime = duration<float>(frameStart - lastTick);

        update(deltaTime.count());

        render(*camera);

        SDL_RenderPresent(renderer);

        lastTick = frameStart;
        nanoseconds sleepDuration = frameDuration - (steady_clock::now() - frameStart);
        if (sleepDuration > nanoseconds(0)) std::this_thread::sleep_for(sleepDuration);
    }
}

void update(float deltaTime) {
    camera->update(deltaTime);

}

void render(const Camera& camera) {
    {
        int widthBg, heightBg;

        static SDL_Texture* texBackground = AssetManager::instance()->findTexture("background");

        SDL_QueryTexture(texBackground, nullptr, nullptr, &widthBg, &heightBg);
        const SDL_FRect rectBg = {
            (1280 - widthBg) / 2.0f,
            (720 - heightBg) / 2.0f,
            (float)widthBg, (float)heightBg
        };
        camera.renderTexture(texBackground, nullptr, &rectBg, 0, nullptr);
    } 
}

