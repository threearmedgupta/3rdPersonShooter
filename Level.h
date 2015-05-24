#pragma once

#include <OgreRoot.h>
#include <vector>
#include <stdio.h>
#include <btBulletDynamicsCommon.h>
#include <GameObject.h>
#include <Tile.h>
//#include "LevelTemplate"

using std::vector;
using std::cout;

//class Tile;
//class LevelTemplate;

class Level:public GameObject{

  
  //static class variables

  btCompoundShape* tileBodies;//actual tiles
  //Ogre::SceneManager* smp;
  //Ogre::SceneNode * levelNode;

  Tile *tileset;

  Ogre::Vector3 start;
  //LevelTemplate* levelTemplate;

public:
  void proceduralLevelGen(int nRooms);
  void generateRoom(int xi,int yi, int tilex=0, int tiley=0);
  void printLevel();
  Level();
  Level(Ogre::SceneManager* smptr);
  ~Level();
  void testLevelGen();
  short getTile(int xi, int yi, int zi=0);
  void setTile(short val, int xi, int yi, int zi=0);
  Ogre::Vector3 getStart();
  void constructLevel();
  int generateLevel(int xi=1, int yi=1, int zi=1);

  //Added for Monster code
  int getTileMapSize();

  int num_monsters;
  int num_monsters_left;


  int x,y,z;
  
  //instance variables
  vector<short> tile_map;//3d tile index

};
