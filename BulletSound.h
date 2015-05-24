#pragma once

#include "GameObject.h"
// #include "Sound.h"
#include <SDL/SDL_mixer.h>
#include <SDL/SDL.h>

using std::cout;

class BulletSound{
public:  
	BulletSound();
	virtual ~BulletSound();
	bool loadMedia();
	void closeMedia();
	void strike();
	void miss();

protected:	
	int x = 0;

private:
	//Loading success flag
	bool success = true;

	//The music that will be played
	Mix_Chunk *hit_enemy = NULL;
	Mix_Chunk *hit_wall = NULL;
};
