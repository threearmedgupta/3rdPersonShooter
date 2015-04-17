#include "GameObject.h"
#include <stdio.h>
#include <assert.h>
#include <string>

using std::cout;

GameObject::GameObject() :
    name(0),
    rootNode(0),
    geom(0),
    simulator(0),
    ms(0),
    shape(0),
    body(0),
    position(0.0f,0.0f,0.0f),
    rotation(0.0f,0.0f,0.0f,0.0f){
}

void GameObject::setPos(btVector3 pos){
  position=pos;
}


void GameObject::addToSimulator() {
    simulator->addObject(this);
}

void GameObject::updateTransform(){

    btTransform tr;
    ms->getWorldTransform(tr);
    if(this->name.compare("Court")!=0){
    rootNode->setPosition(tr.getOrigin().getX(),
                          tr.getOrigin().getY(),
                          tr.getOrigin().getZ());
    }
    position=btVector3(tr.getOrigin().getX(),
                       tr.getOrigin().getY(),
                       tr.getOrigin().getZ());
}

void GameObject::setTransform(Ogre::Vector3 tr){
    const btTransform trans=btTransform(rotation,btVector3(tr.x,tr.y,tr.z));
    ms->setWorldTransform(trans);
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
