#pragma once

#include <Ogre.h>
#include <GameObject.h>

class Player:public GameObject{
public:
    btScalar w,l,h;
    btVector3 pos0,pos1,pos2;
    Player(Ogre::SceneManager* smp,Ogre::String name="player");
    void setPos(Ogre::Vector3 pos);
    void setPos(btVector3 pos);
    btScalar getX();
    btScalar getY();
    btScalar getZ();
};
