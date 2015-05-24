#pragma once

#include <Ogre.h>
#include "Simulator.h"
#include <btBulletDynamicsCommon.h>

class OgreMotionState;

class GameObject {
protected:
    Ogre::String name;
    Ogre::SceneManager* sceneMgr;
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
    Ogre::Vector3 m_directionVector;
    Ogre::Real power;
    bool hit;
    GameObject();
    void addToSimulator();
    btRigidBody* getBody();
    void setSimulator(Simulator* insim);
    void printpos();
    Ogre::SceneNode* getNode();
    Ogre::String getName();
    Ogre::SceneManager* getSMP();
    void updateTransform();
    void setTransform(Ogre::Vector3 tr);
    void setTransform(btVector3 tr);
    Ogre::Vector3 getPos();
    btVector3 getPosbt();
    void setPos(btVector3 pos);
    void setRotation(btQuaternion rot);
    Ogre::Entity* getEntity();

};
