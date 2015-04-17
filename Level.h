#pragma once

#include <OgreRoot.h>
#include <vector>

//#include "Tile.h"
//#include "LevelTemplate"

using std::vector;

class Tile;
class LevelTemplate;

class Level
{
  //static class variables
  static vector<Tile*> tile_palette;//actual tiles
  Ogre::SceneManager* smp;

  //instance variables
  vector<short> tile_map;//3d tile index
  int x,y,z;
  Ogre::Vector3 start;
  LevelTemplate* levelTemplate;

public:
  Level(Ogre::SceneManager* smptr);
  Tile * getTile(int, int, int);
  Ogre::Vector3 getStart();
  int generate(int xi=1, int yi=1, int zi=1);
};
