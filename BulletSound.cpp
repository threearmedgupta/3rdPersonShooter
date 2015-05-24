#include "BulletSound.h"
#include <iostream>

BulletSound::BulletSound() {
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
BulletSound::~BulletSound() {
	closeMedia();
}

bool BulletSound::loadMedia()
{

    //Load music
    hit_enemy = Mix_LoadWAV( "hit_enemy.wav" );
    if( hit_enemy == NULL )
    {
        cout << Mix_GetError();
        printf( "Failed to load beat music! SDL_mixer Error: %s\n", Mix_GetError() );
        success = false;
    }

    hit_wall = Mix_LoadWAV( "bullet_miss.wav" );
    if( hit_wall == NULL )
    {
        cout << Mix_GetError();
        printf( "Failed to load beat music! SDL_mixer Error: %s\n", Mix_GetError() );
        success = false;
    }

    return success;
}

void BulletSound::strike()
{
    cout << "\n\n HIT ENEMY SOUND \n\n";
    Mix_PlayChannel( -1, hit_enemy, 0 );
}

void BulletSound::miss()
{
    Mix_PlayChannel ( -1, hit_wall, 0 );
}

void BulletSound::closeMedia()
{
    Mix_FreeChunk( hit_enemy );
    hit_enemy = NULL;
    Mix_FreeChunk( hit_wall );
    hit_wall = NULL;
    //Quit SDL subsystems
    Mix_Quit();
    SDL_Quit();
}
