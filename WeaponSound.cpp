#include "WeaponSound.h"
#include <iostream>

WeaponSound::WeaponSound(const int weapon) {
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
}
WeaponSound::~WeaponSound() {
	closeMedia();
}

void WeaponSound::pause()
{
    Mix_Pause(-1);
}

void WeaponSound::unpause()
{
    Mix_Resume(-1);
}

bool WeaponSound::loadMedia()
{

    //Load music
    gunshot = Mix_LoadWAV( "gunshot.wav" );
    if( gunshot == NULL )
    {
        cout << Mix_GetError();
        printf( "Failed to load beat music! SDL_mixer Error: %s\n", Mix_GetError() );
        success = false;
    }

    reload_sound = Mix_LoadWAV( "reload.wav" );
    if( reload_sound == NULL )
    {
        cout << Mix_GetError();
        printf( "Failed to load beat music! SDL_mixer Error: %s\n", Mix_GetError() );
        success = false;
    }

    switch_sound = Mix_LoadWAV( "switchweapon.wav" );
    if( switch_sound == NULL )
    {
        cout << Mix_GetError();
        printf( "Failed to load beat music! SDL_mixer Error: %s\n", Mix_GetError() );
        success = false;
    }

    return success;
}

void WeaponSound::fire()
{
    cout << "FIRE\n";
    Mix_PlayChannel( -1, gunshot, 0 );
}

void WeaponSound::switch_weapon()
{
    Mix_HaltChannel ( -1 );
    Mix_PlayChannel ( -1, switch_sound, 0 );
}

void WeaponSound::reload()
{
    cout << "Reload\n";
    Mix_PlayChannel ( -1, reload_sound, 0 );
}

void WeaponSound::closeMedia()
{
    Mix_FreeChunk( gunshot );
    gunshot = NULL;
    Mix_FreeChunk( reload_sound );
    reload_sound = NULL;
    Mix_FreeChunk( switch_sound );
    switch_sound = NULL;
    //Quit SDL subsystems
    Mix_Quit();
    SDL_Quit();
}
