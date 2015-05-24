#include "Tile.h"
#include <string>

const std::string Tile::wallMesh="cube.mesh";

Tile::Tile(){
  levelNode=(Ogre::SceneNode *)2;//what?
}

Tile::Tile(Ogre::SceneNode * lvlptr, Ogre::SceneManager * smptr, btCompoundShape* tb):
  levelNode(lvlptr),
  smp(smptr),
  lvlBodies(tb)
{
  defaultTile={0,0,0,0,0,0,0,0};
  nTile={1,0,0,0,0,0,0,0};
  sTile={0,1,0,0,0,0,0,0};
  eTile={0,0,1,0,0,0,0,0};
  wTile={0,0,0,1,0,0,0,0};
  n_eTile={1,0,1,0,0,0,0,0};
  n_wTile={1,0,0,1,0,0,0,0};
  s_eTile={0,1,1,0,0,0,0,0};
  s_wTile={0,1,0,1,0,0,0,0};
  n_s_eTile={1,1,1,0,0,0,0,0};
  n_s_wTile={1,1,0,1,0,0,0,0};
  n_e_wTile={1,0,1,1,0,0,0,0};
  s_e_wTile={0,1,1,1,0,0,0,0};


  boxShape = new btBoxShape(btVector3(TSIZE/2.0,WALLSIZE/2.0,TSIZE/2.0));

  cout << "\n\nTILE MADE"<<levelNode<<"\n\n";
}

void Tile::genTile(int id, int xi, int yi, int zi){


  //0: empty tile:
  string tileID = std::to_string(xi)+",";
  tileID += std::to_string(yi)+",";
  tileID += std::to_string(zi);



  //calculate positional offset of tile--x and y plane is floor
  Ogre::Vector3 offset=Ogre::Vector3(xi*TSIZE,zi*TSIZE,yi*TSIZE);

  cout << "generating " << tileID <<"="<<id<<" at ";
  cout << offset.x << "," << offset.y << "," << offset.z <<"\n";
/*
  tiles:
    0. no tile  1. no wall  2. north  3. south 4. east  5. west
    6. north-e  7. north-w  8. south-e  9. south-w
    10. nse 11. nsw 12. new 13. sew
*/
  switch(id){
    case(0)://no tile
      break;
    case(1):
      bindTileEntity(defaultTile, tileID,offset);
      break;
   case(2):
    bindTileEntity(nTile, tileID,offset);
    break;
   case(3):
    bindTileEntity(sTile, tileID,offset);
    break;
   case(4):
    bindTileEntity(eTile, tileID,offset);
    break;
   case(5):
     bindTileEntity(wTile, tileID,offset);
     break;
   case(6):
    bindTileEntity(n_eTile, tileID,offset);
    break;
  case(7):
    bindTileEntity(n_wTile, tileID,offset);
   break;
   case(8):
    bindTileEntity(s_eTile, tileID,offset);
    break;
    case(9):
     bindTileEntity(s_wTile, tileID,offset);
     break;
    case(10):
    bindTileEntity(n_s_eTile, tileID,offset);
    break;
    case(11):
    bindTileEntity(n_s_wTile, tileID,offset);
    break;
    case(12):
    bindTileEntity(n_e_wTile, tileID,offset);
    break;
    case(13):
    bindTileEntity(s_e_wTile, tileID,offset);
    break;

   default:
    break;
  }
  //1: no wall
}

void Tile::bindTileEntity(TileInput t, std::string id, Ogre::Vector3 tileOffset){
  //cube transform to rectangular prism floor centered at origin
  tileOffset.x*=-1;
  cout << "BINDING TILE "<<id<<"\n";

  float cubescale=TSIZE/100.0;// 5m rooms/ 100unit cube
  float xscale=cubescale;
  float yscale=cubescale*(WALLSIZE/TSIZE);
  float zscale=cubescale;

  Ogre::Vector3 cubeScale=Ogre::Vector3(xscale,yscale,zscale);
  Ogre::Quaternion rq;

  Ogre::Vector3 floorTranslate=Ogre::Vector3(0,-((float)y/2.0)+((float)WALLSIZE/2.0),0);
  Ogre::Vector3 ceilingTranslate=floorTranslate*(-1);

  //create root tile entity
  std::string tilename = "Tile_";
  tilename += id;

  Ogre::SceneNode* tileNode = levelNode->createChildSceneNode(tilename + "_node");

//create floor
  //visuals
  Ogre::SceneNode* fNode=tileNode->createChildSceneNode(tilename+"_f_node");
  Ogre::Entity* entity = smp->createEntity(tilename+"_floor","cube.mesh");
  fNode->attachObject(entity);
  fNode->scale(cubeScale);
  fNode->translate(floorTranslate);
  entity->setMaterialName("tiledfloor");
  //physics bodies
  btTransform tf;
  tf.setIdentity();
  tf.setOrigin(o2bVector3(tileOffset+floorTranslate));
  lvlBodies->addChildShape(tf,boxShape);

//create ceiling


  float wallscale=(1.0-WALLSIZE/TSIZE);
  float wallscale2x=(1.0-2.0*WALLSIZE/TSIZE);

  //create walls
  if(t.n){
     makeWall("n", tilename + "_north_wall",tileNode,
      Ogre::Vector3(cubeScale.x,cubeScale.y,cubeScale.z*wallscale), tileOffset);
   }
  if(t.s){
    makeWall("s", tilename + "_south_wall",tileNode,
      Ogre::Vector3(cubeScale.x,cubeScale.y,cubeScale.z*wallscale), tileOffset);
  }
  //if(t.e){  makeWall("e", tilename + "_east_wall",tileNode,cubeScale);   }
  //if(t.w){  makeWall("w", tilename + "_west_wall",tileNode,cubeScale);  }

  if(t.e && !t.n && !t.s){
    makeWall("e", tilename + "_east_wall",tileNode,
      Ogre::Vector3(cubeScale.x*wallscale,cubeScale.y,cubeScale.z), tileOffset);
  }
  if(t.w && !t.n && !t.s){
    makeWall("w", tilename + "_west_wall",tileNode,
      Ogre::Vector3(cubeScale.x*wallscale,cubeScale.y,cubeScale.z), tileOffset);
  }

  if(t.e && (t.n || t.s)){
    //cubeScale.x=cubescale*wallscale;
    //cubeScale.z=cubescale*wallscale2x;
    makeWall("e", tilename + "_east_wall",tileNode,
      Ogre::Vector3(cubeScale.x*wallscale,cubeScale.y,cubeScale.z*wallscale/*2x*/), tileOffset);
  }
  if(t.w && (t.n || t.s)){
    //cubeScale.x=cubescale*wallscale;
    //cubeScale.z=cubescale*wallscale2x;
    makeWall("w", tilename + "_west_wall",tileNode,
      Ogre::Vector3(cubeScale.x*wallscale,cubeScale.y,cubeScale.z*wallscale/*2x*/), tileOffset);

  }

  tileNode->translate(tileOffset);
}

void Tile::makeWall(string pos, string name,
                    Ogre::SceneNode * tn, Ogre::Vector3 cs, Ogre::Vector3 offset){ //rotation
  Ogre::Vector3 tv;
  Ogre::Quaternion rq;
  Ogre::Vector3 xv,yv,zv;
  xv=Ogre::Vector3(1,0,0);
  yv=Ogre::Vector3(0,1,0);
  zv=Ogre::Vector3(0,0,1);

  if(!pos.compare("n")){
    tv=Ogre::Vector3(0,WALLSIZE/2.0,((float)z/2.0)-((float)WALLSIZE/2.0));
    rq=Ogre::Quaternion(Ogre::Degree(90),xv);
  }
  else if(!pos.compare("s")){
    tv=Ogre::Vector3(0,WALLSIZE/2.0,-1*((float)z/2.0)+((float)WALLSIZE/2.0));
    rq=Ogre::Quaternion(Ogre::Degree(90),xv);
  }
  else if(!pos.compare("e")){
    tv=Ogre::Vector3(-1*((float)x/2.0)+((float)WALLSIZE/2.0),WALLSIZE/2.0,0);
    rq=Ogre::Quaternion(Ogre::Degree(90),zv);
  }
  else if(!pos.compare("w")){
    tv=Ogre::Vector3(((float)x/2.0)-((float)WALLSIZE/2.0),WALLSIZE/2.0,0);
    rq=Ogre::Quaternion(Ogre::Degree(90),zv);
  }


  Ogre::SceneNode* node=tn->createChildSceneNode(name+"_node");
  Ogre::Entity* entity = smp->createEntity(name,"cube.mesh");
  node->attachObject(entity);
  entity->setMaterialName("stonewall");
  node->scale(cs);
  node->translate(tv);
  node->rotate(rq);

  //physics bodies
  btTransform tf;
  tf.setIdentity();
  tf.setRotation(btQuaternion(rq.x,rq.y,rq.z,rq.w));
  tf.setOrigin(o2bVector3(offset+tv));
  lvlBodies->addChildShape(tf,boxShape);

}
/*
btVector3 o2bVector3(Ogre::Vector3 in){
  return btVector3(in.x,in.y,in.z);
}*/

//void makeSmallWall
