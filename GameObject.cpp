#include "GameObject.h"
#include <stdio.h>
#include <assert.h>
#include <string>

using std::cout;

GameObject::GameObject() :
    name(""),
    rootNode(0),
    geom(0),
    simulator(0),
    ms(0),
    shape(0),
    body(0),
    position(0.0f,0.0f,0.0f),
    rotation(0.0f,0.0f,0.0f,0.0f),
    hit(0)
{}

void GameObject::setPos(btVector3 pos){
  position=pos;
}
Ogre::SceneManager* GameObject::getSMP(){
  return sceneMgr;
}


void GameObject::addToSimulator() {
    simulator->addObject(this);
}

void GameObject::updateTransform(){

    btTransform tr;
    ms->getWorldTransform(tr);
    //cout << "updating "<<this->name<<" rootNode:"<<this->rootNode<<" position:"<<tr.getOrigin().getX() <<" "<<tr.getOrigin().getY() <<" "<<tr.getOrigin().getZ() <<" " <<"\n";
    rootNode->setPosition(tr.getOrigin().getX(),
                          tr.getOrigin().getY(),
                          tr.getOrigin().getZ());

    //Ogre::Vector3 currentFacing = rotation * Ogre::Vector3::UNIT_Z;
    //Ogre::Quaternion quat = currentFacing.getRotationTo(Ogre::Vector3::UNIT_Z);


    if(name.compare("player")==0){
      Ogre::Quaternion quat = Ogre::Quaternion(rotation.getW(),rotation.getX(),rotation.getY(),rotation.getZ());
      rootNode->setOrientation(quat);
    }
    position=btVector3(tr.getOrigin().getX(),
                       tr.getOrigin().getY(),
                       tr.getOrigin().getZ());
  if(name.compare("bullet")==0){
    rootNode->yaw(Ogre::Degree(1));
  }
}

void GameObject::setTransform(Ogre::Vector3 tr){
    const btTransform trans=btTransform(rotation,btVector3(tr.x,tr.y,tr.z));
    ms->setWorldTransform(trans);
}

btVector3 GameObject::getPosbt(){
  return position;
}

void GameObject::setTransform(btVector3 tr){
    const btTransform trans=btTransform(rotation,tr);
    ms->setWorldTransform(trans);
}

void GameObject::setSimulator(Simulator * insim){
    simulator=insim;
}

btRigidBody* GameObject::getBody(){
    return body;
}

void GameObject::printpos(){
    cout << "{"<< position.x()<<","<< position.y()<<","<< position.z()<<"}\n";
}

Ogre::SceneNode* GameObject::getNode(){
    return rootNode;
}
Ogre::String GameObject::getName(){
    return name;
}

Ogre::Vector3 GameObject::getPos(){
    return rootNode->getPosition();
}

void GameObject::setRotation(btQuaternion rot){
  rotation=rot;
}

Ogre::Entity* GameObject::getEntity()
{
  return geom;
}
