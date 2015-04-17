#pragma once

#include <Ogre.h>
#include "Simulator.h"
#include <btBulletDynamicsCommon.h>

class OgreMotionState;

class GameObject {
protected:
    Ogre::String name;
    Ogre::SceneNode* rootNode;
    Ogre::Entity* geom;

    Simulator* simulator;
    btMotionState* ms;
    btCollisionShape* shape;
    btRigidBody* body;

    //physical properties
    btScalar mass;
    btScalar friction;
    btScalar restitution;
    btVector3 position;
    btVector3 inertia;
    btQuaternion rotation;

public:
    GameObject();
    void addToSimulator();
    btRigidBody* getBody();
    void setSimulator(Simulator* insim);
    void printpos();
    Ogre::SceneNode* getNode();
    Ogre::String getName();
    void updateTransform();
    void setTransform(Ogre::Vector3 tr);
    void setTransform(btVector3 tr);
    Ogre::Vector3 getPos();
    void setPos(btVector3 pos);

};
