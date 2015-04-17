/*
  TILE: 5m cubes
*/

#pragma once

#include <string>
#include <OgreRoot.h>

#define TSIZE 5; //cube dimensions
#define WALLSIZE 0.3; //wall thickness
#define TCOUNT 2;

using std::string;

class Tile
{
  static const string wallMesh;

  //static
  static const int x = TSIZE;
  static const int y = TSIZE;
  static const int z = TSIZE;

  //structures

  void genTile(int id);

public:
  Tile(int id=0);

  //return pointer to first tile
  Tile* getList();
};
