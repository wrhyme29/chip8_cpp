#ifndef SDL2_RENDERER_H
#define SDL2_RENDERER_H

#include <SDL.h>

#include "Chip8Events.h"

#define SDL_MAX_QUEUED_EVENTS 65535

class Sdl2Renderer
{
public:
    Sdl2Renderer(int32_t width, int32_t height, int32_t scaleFactor, int32_t fps);
    ~Sdl2Renderer();
    bool IsValid();
    bool ShouldExit();
    void RenderScreen(bool** screen);
    void PollEvents(Chip8Event* eventQueue, size_t* eventCount);

private:
    void InitializeSdl2();

    bool m_shouldExit;
    bool m_isValid;
    int32_t m_fps;
    int32_t m_scaleFactor;
    int32_t m_height;
    int32_t m_width;
    SDL_Surface* m_gameSurface;
    SDL_Surface* m_winSurface;
	SDL_Window* m_window;
};

#endif // SDL2_RENDERER_H
