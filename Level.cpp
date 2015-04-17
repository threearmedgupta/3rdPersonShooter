#include "Level.h"

Level::Level(Ogre::SceneManager* smptr):
  x(0),
  y(0),
  z(0)/*,
  start(0)/*,
  levelTemplate(0)*/

{
  smp=smptr;
}

Tile * Level::getTile(int xi, int yi, int zi)
{

  //short tileID=tile_map[xi + yi*xi + zi*yi*xi]; //linear to 3d storage
  //return tile_palette[tileID];
}
Ogre::Vector3 Level::getStart()
{
  return start;
}

//simple test platform
int Level::generate(int xi, int yi, int zi)
{
  for(int i=0; i < xi;i++)
    for(int j=0; j < yi;j++)
      for(int k=0; i < zi;k++)
        tile_map.push_back(1);
}
