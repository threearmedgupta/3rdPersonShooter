#include "Tile.h"

const std::string Tile::wallMesh="cube.mesh";

Tile::Tile(int id)
{
  genTile(id);

}

void Tile::genTile(int id){
  //0: empty tile:
  switch(id){
    case(0):
      break;
    case(1):
      //Ogre::Entity * ep=
      break;
    default:
      break;
  }
  //1: no wall
}
