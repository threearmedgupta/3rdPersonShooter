#pragma once

#include "GameObject.h"
// #include "Sound.h"
#include <SDL/SDL_mixer.h>
#include <SDL/SDL.h>

using std::cout;

class BGMusic{
public:  
	BGMusic();
	virtual ~BGMusic();
	bool loadMedia();
	void closeMedia();
	void playOrPause();
	void start();
	void win();
	void lose();
	void monster_aggro();

protected:	
	int x = 0;

private:
	//Loading success flag
	bool success = true;

	//The music that will be played
	Mix_Music *bgmusic = NULL;
	Mix_Chunk *start_sound = NULL;
	Mix_Chunk *victory_sound = NULL;
	Mix_Chunk *lose_sound = NULL;
	Mix_Chunk *aggro_sound = NULL;
};
