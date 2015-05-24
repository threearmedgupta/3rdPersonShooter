#include <Player.h>
#include <btBulletDynamicsCommon.h>

using std::cout;

//multiplayer stuff

Player::Player(Ogre::SceneManager* smp, Ogre::String inname, Ogre::Vector3 startPos):
  l(25),
  w(25),
  h(50)
  // playerState(PlayerState::NoFire),
  // weapon(Weapon0)
{
    //set scene manager pointer
    sceneMgr=smp;
    playerLV=btVector3(0,0,0);

    //physical constants
    l=.5;
    w=.6;
    h=1.6;

    ori=btVector3(startPos.x,startPos.y,startPos.z);
    cout << "player start position: "<<startPos.x<<" "<<startPos.y<<" "<<startPos.z<<"\n";

    restitution=1.0;//avg of allowable extremes

    name=inname;

    position = btVector3(ori);
    inertia= btVector3(0,0,0);
    rotation=btQuaternion(0,0,0,1);

    mass=1000.0f;
    restitution=1.0;
    friction=0;
    player_health = 10.0;

    //bind player

    rootNode=sceneMgr->getRootSceneNode()->createChildSceneNode(name + "Node");
    p_entity=sceneMgr->createEntity(inname,"ninja.mesh");
    rootNode->attachObject(p_entity);

    //purdiness
    p_entity->setCastShadows(true);
    std::string namest = name;
    if(namest.compare("player")==0)
      p_entity->setMaterialName("redninja");
    else
        p_entity->setMaterialName("PlayerWire2");
    rootNode->translate(position.getX(),position.getY()-1.2,position.getZ());
    rootNode->scale(1.2/100.,1.2/100.,1.2/100.);

    //bt values
    shape = new btBoxShape(btVector3(w/2.,h/2.,l/2.));
    shape->calculateLocalInertia(mass,inertia);



    ms = new btDefaultMotionState(btTransform(rotation, position));

    btRigidBody::btRigidBodyConstructionInfo bodyCI(mass, ms, shape, inertia);

    //add other physics constants
    bodyCI.m_restitution=restitution;
    bodyCI.m_friction=friction;
    bodyCI.m_rollingFriction=0.2;//!!!

    body = new btRigidBody(bodyCI);
    body->setActivationState(DISABLE_DEACTIVATION);
    btTransform btt;
    ms->getWorldTransform(btt);
    btVector3 org=btt.getOrigin();
}

void Player::setLV(btVector3 lvin){
  playerLV=lvin;
}

void Player::updateTransform(){
  btTransform tr;
  ms->getWorldTransform(tr);
  btVector3 origin = tr.getOrigin();
  float mod = -2.1;
  if(origin.getY()<mod || origin.getY()>mod){
    //cout <<(float)((int)(origin.getY()/5.0))*5.0-2.2<<"it aint where it should be\n";
    origin.setY(mod);
    tr.setOrigin(origin);
    ms->setWorldTransform(tr);
  }
  ms->getWorldTransform(tr);
  origin=tr.getOrigin();
  cout << origin.getY()<<"\n";

  rootNode->setPosition(tr.getOrigin().getX(),
                        tr.getOrigin().getY(),
                        tr.getOrigin().getZ());
 //rootNode->translate(0,-0.5,0);

 Ogre::Quaternion quat = Ogre::Quaternion(rotation.getW(),rotation.getX(),rotation.getY(),rotation.getZ());
 rootNode->setOrientation(quat);
 position=btVector3(tr.getOrigin().getX(),
                     tr.getOrigin().getY(),
                     tr.getOrigin().getZ());

}

void Player::setLV(Ogre::Vector3 lvin){

  playerLV=btVector3(lvin.x,lvin.y,lvin.z);
}

void Player::setPos(Ogre::Vector3 pos){
    position = btVector3(pos.x,pos.y,pos.z);
}

void Player::setPos(btVector3 pos){
  position=pos;
}

btScalar Player::getX(){
    return position.getX();
}
btScalar Player::getY(){
    return position.getY();
}
btScalar Player::getZ(){
    return position.getZ();
}
