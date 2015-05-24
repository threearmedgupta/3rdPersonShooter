#include "BGMusic.h"
#include <iostream>

BGMusic::BGMusic() {
	//Initialize SDL
    if( SDL_Init( SDL_INIT_VIDEO | SDL_INIT_AUDIO ) < 0 )
    {
        printf( "SDL could not initialize! SDL Error: %s\n", SDL_GetError() );
        success = false;
    }

    //Initialize SDL_mixer
    if( Mix_OpenAudio( 44100, MIX_DEFAULT_FORMAT, 2, 2048 ) < 0 )
    {
        printf( "SDL_mixer could not initialize! SDL_mixer Error: %s\n", Mix_GetError() );
        success = false;
    }
    if (!loadMedia())
    {
        printf( "Failed to load media!\n");
    }
    if( Mix_PlayingMusic() == 0 )
	{
        cout << "HELLO\n";
		//Play the music
		Mix_PlayMusic( bgmusic, -1 );
	}
}
BGMusic::~BGMusic() {
	closeMedia();
}

bool BGMusic::loadMedia()
{
    //Load music
    bgmusic = Mix_LoadMUS( "bgmusic.wav" );
    if( bgmusic == NULL )
    {
        cout << Mix_GetError();
        printf( "Failed to load beat music! SDL_mixer Error: %s\n", Mix_GetError() );
        success = false;
    }

    start_sound = Mix_LoadWAV( "start_gong.wav" );
    if( start_sound == NULL )
    {
        cout << Mix_GetError();
        printf( "Failed to load beat music! SDL_mixer Error: %s\n", Mix_GetError() );
        success = false;
    }

    victory_sound = Mix_LoadWAV( "victory_sound.wav" );
    if( victory_sound == NULL )
    {
        cout << Mix_GetError();
        printf( "Failed to load beat music! SDL_mixer Error: %s\n", Mix_GetError() );
        success = false;
    }

    lose_sound = Mix_LoadWAV( "lose_sound.wav" );
    if( lose_sound == NULL )
    {
        cout << Mix_GetError();
        printf( "Failed to load beat music! SDL_mixer Error: %s\n", Mix_GetError() );
        success = false;
    }

    return success;
}

void BGMusic::playOrPause()
{
    //If the music is paused
    if( Mix_PausedMusic() == 1 )
    {
        //Resume the music
        Mix_ResumeMusic();
    }
    //If the music is playing
    else
    {
        //Pause the music
        Mix_PauseMusic();
    }
}

void BGMusic::start()
{
    Mix_PlayChannel( -1, start_sound, 0 );
}

void BGMusic::win()
{
    Mix_PlayChannel( -1, victory_sound, 0 );
}

void BGMusic::lose()
{
    Mix_PlayChannel( -1, lose_sound, 0 );
}

void BGMusic::closeMedia()
{
    Mix_FreeMusic( bgmusic );
    bgmusic = NULL;
    Mix_FreeChunk( start_sound );
    start_sound = NULL;
    Mix_FreeChunk( victory_sound );
    victory_sound = NULL;
    Mix_FreeChunk( lose_sound );
    lose_sound = NULL;
    Mix_FreeChunk( aggro_sound );
    aggro_sound = NULL;
    //Quit SDL subsystems
    Mix_Quit();
    SDL_Quit();
}
