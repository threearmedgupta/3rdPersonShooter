#include <Level.h>
#include <stdlib.h>
#include "GameEnums.h"

Level::Level(){
}

Level::Level(Ogre::SceneManager* smptr)
{
  cout <<"\n\nCreating level...\n\n";
  x=y=z=0;
  btTransform bt;bt.setIdentity();
  ms = new btDefaultMotionState(bt);
  sceneMgr=smptr;
  mass=0;
  name="Level";
  rootNode = sceneMgr->getRootSceneNode()->createChildSceneNode("Level_node");
  tileBodies = new btCompoundShape();
  tileset=new Tile(rootNode, sceneMgr,tileBodies);
  start=Ogre::Vector3(0,0,0);
  num_monsters = 3;
  num_monsters_left = num_monsters;
  cout << "\n\nLEVEL MADE: "<<rootNode<<"\n\n";


}

//Added for Monster code
int Level::getTileMapSize()
{
  return tile_map.size();
}

short Level::getTile(int xi, int yi, int zi)
{
  return tile_map[xi + yi*x + zi*y*x]; //linear to 3d storage
}

void Level::setTile(short val, int xi, int yi, int zi)
{
  tile_map[xi + yi*x + zi*y*x]=val; //linear to 3d storage
}

Ogre::Vector3 Level::getStart()
{
  return start;
}

void Level::testLevelGen(){
  tile_map.clear();
  tile_map.resize(4,0);
  x=2;y=2;z=1;
  tile_map[0]=11;tile_map[1]=8;tile_map[2]=11;tile_map[3]=6;
}

//create entities and such from level data
void Level::constructLevel(){
  //Ogre::Vector3 offset;//where to put tile
  //std::string tileID="defaultID";

  cout << "\nCONSTRUCTING LEVEL\n";
  for(int k=0; k < z; k++){
    for(int j=0; j < y; j++){
      for(int i=0; i < x; i++){
        tileset->genTile(getTile(i,j,k),i,j,k);

      }
    }
  }
  btTransform transform;
  transform.setIdentity();
  ms = new btDefaultMotionState( transform );
  btVector3 localInertia(0,0,0);
  tileBodies->calculateLocalInertia(mass,localInertia);

  btRigidBody::btRigidBodyConstructionInfo comp_cInfo( mass, ms, tileBodies, localInertia );
  body= new btRigidBody( comp_cInfo );
}//void constructLevel()

/*
  tiles:
    0. no tile  1. no wall  2. north  3. south 4. east  5. west
    6. north-e  7. north-w  8. south-e  9. south-w
    10. nse 11. nsw 12. new 13. sew
*/

void Level::proceduralLevelGen(int nRooms){
  vector<Ogre::Vector4> rooms;
  // vector4:<xbl,ybl,x,y>
  tile_map.clear();

  int roomx=0;
  int midx=0;
  int roomy=0;
  int midy=0;

  //room 1
  roomx=2*(rand()%2+1)+1;
  roomy=2*(rand()%2+1)+1;

  x=2*roomx;
  y=2*roomy;
  z=2;

  midx=roomx/2;
  midy=roomy/2;

  tile_map.resize(roomx*roomy*4*2,0);

  cout << getTile(0,0)<<"\n";
  rooms.push_back(Ogre::Vector4(roomx,roomy,0,0));
  rooms.push_back(Ogre::Vector4(roomx,roomy,roomx,0));
  rooms.push_back(Ogre::Vector4(roomx,roomy,0,roomy));
  rooms.push_back(Ogre::Vector4(roomx,roomy,roomx,roomy));

  cout << rooms[0].w << rooms[0].x << rooms[0].y << rooms[0].z<<"\n";

//just make a grid for now
  for(int i=0; i < rooms.size();i++){
    generateRoom(rooms[i].x,rooms[i].y,rooms[i].z,rooms[i].w);
  }
  short nwt = (short)Tiles::NOWALL;
  setTile(nwt,midx,roomy-1);
  setTile(nwt,midx,roomy);

  setTile(nwt,midx+roomx,roomy-1);
  setTile(nwt,midx+roomx,roomy);

  setTile(nwt,roomx-1,midy);
  setTile(nwt,roomx,midy);

  setTile(nwt,roomx-1,midy+roomy);
  setTile(nwt,roomx,midy+roomy);

  int halfsz = tile_map.size()/2;
  for(int i=0;i<halfsz;i++){
    cout<<i%(roomx*2)<<","<<i/(roomx*2)<<":"<<i%(roomx*2)<<","<<i/(roomx*2)<<"\n";
    setTile(getTile(i%(roomx*2),i/(roomx*2)),i%(roomx*2),i/(roomx*2),1);
  }

}

void Level::generateRoom(int xi,int yi, int tilex, int tiley){
  //x=xi;y=yi;z=1;
  if(xi<=1 || yi<=1){
    cout << "Level::generateRoom bounds error\n";
    exit(0);
  }
  cout << "\nGENERATING ROOM\n";
  short tileR=1;

  /*for(int j=tiley; j < yi+tiley; j++){
    for(int i=tilex; i < xi+tilex; i++){
      if(i==tilex){
        if(j==tiley)
          tileR=((short)Tiles::SW);
        else if(j==(yi-1+tiley))
          tileR=((short)Tiles::NW);
        else
          tileR=((short)Tiles::W);
      }
      else if(i==xi-1+tilex){
        if(j==tiley)
          tileR=((short)Tiles::SE);
        else if(j==(tiley+yi-1))
          tileR=((short)Tiles::NE);
        else
        tileR=((short)Tiles::E);
      }
      else if(j==tiley){
        tileR=((short)Tiles::S);
      }
      else if(j==tiley+yi-1){
        tileR=((short)Tiles::N);
      }
      else
        tileR=((short)Tiles::NOWALL);
      tile_map.push_back(tileR);
      cout << tileR;
      tileR=1;
    }
  }*/
  for(int j=0; j < yi; j++){
    for(int i=0; i < xi; i++){
      if(i==0){
        if(j==0)
          tileR=((short)Tiles::SW);
        else if(j==(yi-1))
          tileR=((short)Tiles::NW);
        else
          tileR=((short)Tiles::W);
      }
      else if(i==xi-1){
        if(j==0)
          tileR=((short)Tiles::SE);
        else if(j==(yi-1))
          tileR=((short)Tiles::NE);
        else
        tileR=((short)Tiles::E);
      }
      else if(j==0){
        tileR=((short)Tiles::S);
      }
      else if(j==yi-1){
        tileR=((short)Tiles::N);
      }
      else
        tileR=((short)Tiles::NOWALL);
      cout << "before setTile\n";
      cout << "["<<i+tilex<<","<<j+tiley<<"]=\n";
      cout << getTile(i+tilex,j+tiley)<<"\n";
      //setTile(,0,tileR);
      setTile(tileR,i+tilex,j+tiley);
      cout << tileR;
      tileR=1;
    }
  }


}

//simple test platform
int Level::generateLevel(int xi, int yi, int zi)
{
  x=xi;y=yi;z=zi;
  cout << "\nGENERATING LEVEL\n";
  for(int i=0; i < xi;i++){
    for(int j=0; j < yi;j++){
      for(int k=0; k < zi;k++){
        short tile = rand()%13+1;
        tile_map.push_back(tile);
  } } }
  cout << "\n\n LEVEL GENERATED \n\n";
}

Level::~Level(){
  delete(tileset);
}

void Level::printLevel(){
  for(int k=0; k<z;k++){
    cout<<"\nLevel:"<<k<<"\n";
    for(int i=0; i<y;i++){
      for(int j=0; j<x;j++){
        cout <<"["<<j<<i<<k<<"="<<getTile(j,i,k)<<"]";
      }
      cout <<"\n";
    }
  }
}
