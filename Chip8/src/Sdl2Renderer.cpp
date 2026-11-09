#include "Sdl2Renderer.h"
#include <cassert>


Sdl2Renderer::Sdl2Renderer(int32_t width, int32_t height, int32_t scaleFactor, int32_t fps) :
    m_isValid(false),
    m_shouldExit(false),
    m_width(width),
    m_height(height),
    m_scaleFactor(scaleFactor),
    m_fps(fps),
    m_winSurface(NULL),
    m_window(NULL)
{
    InitializeSdl2();
}

Sdl2Renderer::~Sdl2Renderer()
{
    SDL_FreeSurface(m_gameSurface);

    // this also destroys m_winSurface
	SDL_DestroyWindow(m_window);
	SDL_Quit();
}

void Sdl2Renderer::InitializeSdl2()
{
    // Initialize SDL. SDL_Init will return -1 if it fails.
	if ( SDL_Init( SDL_INIT_VIDEO | SDL_INIT_EVENTS ) < 0 ) {
		printf("Error initializing SDL: %s\n", SDL_GetError());
        // @TODO: What do you want to do about errors?
        return;
	} 

    // Create our window
	m_window = SDL_CreateWindow( "Chip8 SDL2", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, m_width * m_scaleFactor, m_height * m_scaleFactor, SDL_WINDOW_SHOWN );

    // Make sure creating the window succeeded
	if ( !m_window ) {
        printf("Error creating window: %s\n", SDL_GetError());
        // @TODO: What do you want to do about errors?
		return;
	}

    // Get the surface from the window
	m_winSurface = SDL_GetWindowSurface(m_window);

    // Make sure getting the surface succeeded
	if ( !m_winSurface ) {
		printf("Error getting surface: %s\n", SDL_GetError());
        // @TODO: What do you want to do about errors?
		return;
	}

    m_gameSurface = SDL_CreateRGBSurfaceWithFormat(
        0,
        m_width,
        m_height,
        32,
        SDL_PIXELFORMAT_RGBA32
    );

    // Make sure creating the surface succeeded
	if ( !m_gameSurface ) {
		printf("Error getting surface: %s\n", SDL_GetError());
        // @TODO: What do you want to do about errors?
		return;
	}


    m_isValid = true;
}

bool Sdl2Renderer::IsValid(){ return m_isValid; }

bool Sdl2Renderer::ShouldExit()
{
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        if (event.type == SDL_QUIT) return true;
    }
    return false;
}

void Sdl2Renderer::RenderScreen(bool** screen)
{
    const Uint32 black = SDL_MapRGB(
        m_gameSurface->format, 0, 0, 0
    );

    const Uint32 white = SDL_MapRGB(
        m_gameSurface->format, 255, 255, 255
    );

    // Clear the screen first
    SDL_FillRect(m_gameSurface, NULL, black);
    SDL_FillRect(m_winSurface, NULL, black);

    SDL_Rect pixel = {0, 0, 1, 1};
    for(int x=0; x < m_width; ++x)
    {
        for(int y=0; y < m_height; ++y)
        {
            if(screen[x][y])
            {
                pixel.x = x;
                pixel.y = y;
                SDL_FillRect( m_gameSurface, &pixel, white );
            }
        }
    }

    SDL_Rect destination = { 0, 0, m_width * m_scaleFactor, m_height * m_scaleFactor};
    SDL_BlitScaled(
        m_gameSurface,
        nullptr,
        m_winSurface,
        &destination
    );

    SDL_UpdateWindowSurface(m_window);
    SDL_Delay(1000 / m_fps);  
}

void Sdl2Renderer::PollEvents(Chip8Event* eventQueue, size_t* eventCount)
{
    *eventCount = 0;
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        switch(event.type)
        {
            case SDL_QUIT:
                eventQueue[(*eventCount)++] = Chip8Event::EXIT;
                break;

            case SDL_WINDOWEVENT:
                if(event.window.event == SDL_WINDOWEVENT_SIZE_CHANGED)
                {
                    m_winSurface = SDL_GetWindowSurface(m_window);
                }
                break;
        }
    }

}
