#pragma once

#include <string>
#include <stdio.h>
#include <OgreRoot.h>
#include <OgreEntity.h>
#include <OgreSceneManager.h>
#include <btBulletDynamicsCommon.h>

#define TSIZE 5.0 //cube dimensions
#define WALLSIZE 0.3 //wall thickness
#define TCOUNT 2

using std::string;
using std::cout;

struct TileInput
{
  int n:1;//=0;
  int s:1;//=0;
  int e:1;//=0;
  int w:1;//=0;
  int ne:1;//=0;
  int se:1;//=0;
  int sw:1;//=0;
  int nw:1;//=0;
};

class Tile
{
  static const string wallMesh;
  Ogre::SceneNode * levelNode;
  Ogre::SceneManager * smp;
  btCompoundShape* lvlBodies;

  TileInput defaultTile,nTile,sTile,wTile,eTile;
  TileInput n_eTile, n_wTile, s_eTile, s_wTile;
  TileInput n_s_eTile, n_s_wTile, n_e_wTile, s_e_wTile;

  btCollisionShape* boxShape;

  //static
  static const int x = TSIZE;
  static const int y = TSIZE;
  static const int z = TSIZE;

  void bindTileEntity(TileInput t, string id, Ogre::Vector3 tileOffset);
  void makeWall(string pos, string name, Ogre::SceneNode * tn, Ogre::Vector3 cs, Ogre::Vector3 offset);

public:

  Tile(Ogre::SceneNode * lvlptr, Ogre::SceneManager * smptr, btCompoundShape* tb);
  Tile();
  void genTile(int id, int xi, int yi, int zi);
  //return pointer to first tile
  Tile* getList();
};

btVector3 o2bVector3(Ogre::Vector3 in);
