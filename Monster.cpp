#include "Monster.h"

/*
Monster should have a specified range of "sight", if player
walks into range of sight, monster will pick up speed and follow
the player as long as the player is still in range
*/

//Add monsters during createscene()
//Get monster position, divide x/z by 5 and get current tile type

Monster::Monster(Ogre::SceneManager* sMgr, Ogre::Vector3 spawn_point)
{
	sceneMgr = sMgr;

	//Monster's Vision/Location
	m_seePlayer = false;
	m_lastSeenPosition = Ogre::Vector3::ZERO;
	m_destinationVector = Ogre::Vector3::ZERO;
	m_directionVector = Ogre::Vector3::ZERO;
	m_distance = 0;
	m_walkSpeed = 1.0f;

	//Monster's Timers
	m_attackTimeout = 1000.0;
	m_pursuitTimeout = 10000.0;
	m_damageTimeout = 200.0;
	m_damaged = false;

	//Monster's Data
	m_health = 5.0;
	m_range = 4000.0;
	m_attackRange = 15.0;
	m_state = STATE_WANDER;

	m_entity = sceneMgr->createEntity("ninja.mesh");
	m_entity->setMaterialName("Ninja");

	name="ninja";

	//Copied from Player.cpp

    //restitution=1.0;//avg of allowable extremes

    position = btVector3(0,0,0);
    inertia= btVector3(0,0,0);
    rotation=btQuaternion(0,0,0,1);

    mass=1000.0f;
    // restitution=1.0;
    // friction=0;

    ms = new btDefaultMotionState(btTransform(rotation, position));

	rootNode = sceneMgr->getRootSceneNode()->createChildSceneNode();

	//Set Monster's spawn position
	btVector3 btspawn;
	//cout << "\nMonster Spawns at: [" << spawn_point.x << ", " << spawn_point.y << ", " << spawn_point.z << "]\n";
	rootNode->setPosition(spawn_point);
	//btspawn = btVector3(m_spawnPt1.x,m_spawnPt1.y,m_spawnPt1.z);
	btspawn = btVector3(spawn_point.x, spawn_point.y, spawn_point.z);
	position = btspawn;
	//setWalkList();

	rootNode->setScale(0.015, 0.015, 0.015);
    rootNode->attachObject(m_entity);


    //linear velocity
    Ogre::Vector3 posOgre= Ogre::Vector3(position.getX(),position.getY(),position.getZ());
    m_directionVector = m_destinationVector - posOgre; //set new direction vector
    m_distance = m_directionVector.normalise();

	//bullet stuff
	shape = new btBoxShape(btVector3(.5,1.9,.5));
    shape->calculateLocalInertia(mass,inertia);

    ms = new btDefaultMotionState(btTransform(rotation, position));

    btRigidBody::btRigidBodyConstructionInfo bodyCI(mass, ms, shape, inertia);

    //add other physics constants
    bodyCI.m_restitution=restitution;
    bodyCI.m_friction=friction;

    body = new btRigidBody(bodyCI);
    //body->setActivationState(DISABLE_DEACTIVATION);
    btTransform btt;
    ms->getWorldTransform(btt);

    monstersound = new MonsterSound();
}

Monster::~Monster()
{
	sceneMgr->destroySceneNode(rootNode);
	delete body;
    delete ms;
}

void Monster::updateTransform(){
	btTransform tr;
    ms->getWorldTransform(tr);
		btVector3 origin = tr.getOrigin();
		if(origin.getY()<-.4){
			//cout <<(float)((int)(origin.getY()/5.0))*5.0-2.2<<"it aint where it should be\n";
			origin.setY(-.4);
			tr.setOrigin(origin);
			ms->setWorldTransform(tr);
		}
		ms->getWorldTransform(tr);
		origin=tr.getOrigin();
		//cout << origin.getY()<<"\n";


    rootNode->setPosition(tr.getOrigin().getX(),
                          tr.getOrigin().getY(),
                          tr.getOrigin().getZ());
    position=btVector3(tr.getOrigin().getX(),
                       tr.getOrigin().getY(),
                       tr.getOrigin().getZ());

	  float theta = atan(m_directionVector.z/(m_directionVector.x*(-1.0)));
		if(m_directionVector.x>0)
			theta+=3.14159;
		theta+=3.14159/2.;
		if(theta < 0)
			theta+=3.14159*2;
		if(theta> 3.14159*2)
			theta-=3.14159*2;

		//cout << "theta:"<<theta<<"\n";
    Ogre::Quaternion quat = Ogre::Quaternion(Ogre::Radian(theta),Ogre::Vector3::UNIT_Y);
    rootNode->setOrientation(quat);
    rootNode->translate(0,-1.9,0);


}

void Monster::changeState(MONSTER_STATE state, Level* l, Player* player)
{

	int x = rand() % l->x*5 + 1;
	int y = player->getY();
    int z = rand() % l->y*5 + 1;

    m_state = state;
	if(m_state == STATE_WANDER)
	{
		cout << "\nCHANGE TO STATE_WANDER\n";
		m_walkSpeed = 5.0f;
		changeDestination(l);
		m_animState = m_entity->getAnimationState("Walk");

	}
	else if (m_state == STATE_CHILL)
	{
		m_animState = m_entity->getAnimationState("Idle1");
		//zero out the m_walkList so he stops moving around
	}
	else if(m_state == STATE_ATTACK)
	{
		cout << "\nCHANGE TO STATE_ATTACK\n";
		m_walkSpeed = 6.0f;
		monstersound->monster_aggro();
		changeDestination(l, player);
		m_animState = m_entity->getAnimationState("Attack2");
		//btVector3 player_pos = player->getPosbt();
		//m_destinationVector = Ogre::Vector3(player_pos.getX(), player_pos.getY(), player_pos.getZ());
	}

	//m_animState = m_entity->getAnimationState("Walk");

	//cout << "\nMonster's destination: " << m_destinationVector.x << ", " << m_destinationVector.y << ", " << m_destinationVector.z << "\n";
	m_animState->setLoop(true);
	m_animState->setEnabled(true);

	//IS THIS NECESSARY?
	//m_animState->addTime(evt.timeSinceLastFrame);

}

void Monster::updateMonsters(Level* level, const Ogre::FrameEvent& evt)
{
	//smooth out movement

	Ogre::Real move = m_walkSpeed * evt.timeSinceLastFrame;
	//m_distance -= move;

	//update distance left to travel, using monster's current position
	Ogre::Vector3 posOgre= Ogre::Vector3(position.getX(),position.getY(),position.getZ());
	m_distance = (posOgre-m_destinationVector).length();


	//check if monster reached destination
	if(m_distance <= 0.1f)
	{

		//cout << "\nDestination reached\n";
		rootNode->setPosition(m_destinationVector); //place monster at destination
		position=btVector3(m_destinationVector.x,m_destinationVector.y,m_destinationVector.z);

		m_directionVector = Ogre::Vector3::ZERO; //set to 0 so next part of path can be started
		changeDestination(level);

    	//linear velocity
    	m_directionVector = m_destinationVector - rootNode->getPosition(); //set new direction vector
    	m_distance = m_directionVector.normalise();

		/*
		if(m_directionVector==Ogre::Vector3(0,0,0)){
			cout << "zero norm\n";
			m_directionVector=Ogre::Vector3::ZERO;
			m_distance = m_directionVector.normalise();
		}
		//else
			//m_distance = m_directionVector.normalise();
		*/
		/*
		//Rotation code
		Ogre::Vector3 src = rootNode->getOrientation()*Ogre::Vector3::UNIT_Z;

		if ((1.0f + src.dotProduct(m_directionVector)) < 0.0001f)
		{
			rootNode->yaw(Ogre::Degree(180));
		}
		else
		{
			Ogre::Quaternion quat = src.getRotationTo(m_directionVector);
			rootNode->rotate(quat);
		}
		*/
	}
	else
	{
		//rootNode->translate(m_directionVector * move);
		//Ogre::Vector3 cur_pos = rootNode->getPosition();
		//position = btVector3(cur_pos.x, cur_pos.y, cur_pos.z);
	}

	//cout << "\nMonster position: " << rootNode->getPosition().x << " " << rootNode->getPosition().z << "\n";
	//cout << "\nBullet box position: " << position.getX() << " " << position.getZ() << "\n";

	m_animState->addTime(evt.timeSinceLastFrame);
}

void Monster::killMonster()
{
	cout << "\nMONSTER HIT -- DIE\n";
	m_animState = m_entity->getAnimationState("Death1");
	m_animState->setEnabled(true);

	rootNode->detachObject(m_entity);

}

void Monster::changeDestination(Level* level, Player* p)
{

	Ogre::Vector3 posOgre= Ogre::Vector3(position.getX(),position.getY(),position.getZ());//not sure why we need this

	if (p == NULL)
	{
		//Monster will pick a random destination

		//not following the player
		int x = rand() % level->x*5 + 1;
		int z = rand() % level->y*5 + 1;
		m_destinationVector = Ogre::Vector3(x*-1, position.getY(), z); //update y position to constant
	}
	else
	{
		//update to player's position
		btVector3 player_pos = p->getPosbt();
		m_destinationVector = Ogre::Vector3(player_pos.getX(), player_pos.getY(), player_pos.getZ());

	}

    //cout << "\nMonster's destination: " << m_destinationVector.x << ", " << m_destinationVector.y << ", " << m_destinationVector.z << "\n";

    //linear velocity
    m_directionVector = m_destinationVector - rootNode->getPosition(); //set new direction vector
    m_distance = m_directionVector.normalise();
}

/* 04/22 - Begin adding monsters to the scene from BaseApplication.cpp */
