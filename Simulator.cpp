#include "Simulator.h"
#include "GameObject.h"
#include "Monster.h"
#include "Weapon.h"
#include <stdio.h>
#include <vector>

using std::vector;

using std::cout;

struct ContactCallback:public btCollisionWorld::ContactResultCallback
{
  GameObject* ptr, *a,*b;
  ContactCallback(GameObject* ptri) {ptr=ptri;}

  btScalar addSingleResult(btManifoldPoint& cp,
      const btCollisionObjectWrapper* colObj0Wrap,
      int partId0,
      int index0,
      const btCollisionObjectWrapper* colObj1Wrap,
      int partId1,
      int/*t*/ index1)
  {
      //cout << a->getName() << " hit "<<b->getName()<< "\n";
      //return 666;
    b->hit=true;
  }
  void setAB(GameObject* ai,GameObject* bi){
    a=ai;
    b=bi;
  }
};

Simulator::Simulator(){
    //initialize bullet world
    collisionConfig = new btDefaultCollisionConfiguration();
    dispatcher = new btCollisionDispatcher(collisionConfig);
    broadphase = new btDbvtBroadphase();
    solver = new btSequentialImpulseConstraintSolver();
    dynamicsWorld = new btDiscreteDynamicsWorld( dispatcher, broadphase, solver,collisionConfig);
    //create world
    dynamicsWorld->setGravity(btVector3(0,-100, 0));//386.09 inches/s^2 = 9.8m/s^2
    monsters_killed = 0;
    lvl_monsters_killed = 0;
    lives = 3;
    lvl_lives = 3;
    //contact callback context
    ccp=new ContactCallback(objList[0]);

}

void Simulator::stepSimulation(const Ogre::FrameEvent& evt, const Ogre::Real elapsedTime, int maxSubSteps, const Ogre::Real fixedTimestep) {
    //cout << objList.size()<<"\n";
    //cout << "about to step\n";

    dynamicsWorld->stepSimulation(elapsedTime,maxSubSteps,fixedTimestep);

    //cout << "stepped\n";

    btVector3 start, end;
    GameObject * a = objList[1];//level
    GameObject * p = objList[0];//player
    //p->printpos();
    //update in ogre
    vector<int> deadObjects;
    for(int i=0; i < objList.size();i++){ // step through all objects
        if((objList[i]->getName()).compare("bullet")==0){
          ccp->setAB(a,objList[i]);
          dynamicsWorld->contactPairTest(a->getBody(),objList[i]->getBody(),*ccp);
          if (((Bullet*)objList[i])->hit)
          {
            deadObjects.push_back(i);
            ((Bullet *)objList[i])->bulletsound->miss();
          }
          else if (((Bullet*)objList[i])->lifeTimer->getMilliseconds() >= ((Bullet*)objList[i])->weapon->lifetime)
          {
            deadObjects.push_back(i);
          }
          else
          {
            for(int j = 2; j < objList.size(); j++)
            {

              if((objList[j]->getName()).compare("ninja")==0)
              {
                //Check for collision between ninja, bullet
                ccp->setAB(objList[j], objList[i]);

                dynamicsWorld->contactPairTest(objList[j]->getBody(), objList[i]->getBody(), *ccp);
                if((objList[i])->hit)
                {
                  //bullet hit the ninja
                  ((Bullet *)objList[i])->bulletsound->strike();
                  deadObjects.push_back(i);
                  break;
                  //((Monster*)objList[j])->killMonster();
                }
              }

            }
          }
          if (!objList[i]->hit)
            objList[i]->updateTransform();
        }
        //Monster Code
        else if((objList[i]->getName()).compare("ninja") == 0)
        {
          bool monster_alive = true;

          //Check if monster collided with the player
          ccp->setAB(objList[i], objList[0]);
          dynamicsWorld->contactPairTest(objList[i]->getBody(), objList[0]->getBody(), *ccp);

          //Reduce player's health if collision occurred
          if ((objList[0])->hit){
            ((Player*)objList[0])->player_health -= 1.0;
          }

          //Loop through objList to check if monster got shot
          for(int j = 2; j < objList.size(); j++)
          {
            //Only concerned about bullet objects
            if((objList[j]->getName()).compare("bullet")==0)
            {

              //Check for collision between (bullet, monster)
              ccp->setAB(objList[j], objList[i]);
              dynamicsWorld->contactPairTest(objList[j]->getBody(), objList[i]->getBody(), *ccp);

              //If monster got hit by bullet, reduce its health or kill the monster
              if((objList[i])->hit)
              {
                //deduct weapon attack value from monster health
                //check if health is < 0, if so kill it
                ((Monster*)objList[i])->m_health -= ((Bullet*)objList[j])->power;

                //bullet hit the ninja
                if(((Monster*)objList[i])->m_health <= 0.0 && monster_alive == true)
                {
                  deadObjects.push_back(i);
                  monster_alive = false;
                  //((Monster*)objList[j])->killMonster();
                }

                objList[i]->hit = false;
              }
              //else
                //objList[i]->updateTransform();
            }
          }

          if (monster_alive == true)
          {

            /* PUT THIS IN MONSTER.CPP */
            Monster* m = (Monster*)objList[i];

            btVector3 monster_pos = m->getPosbt();
            btVector3 player_pos = p->getPosbt();

            btScalar personal_space = (monster_pos - player_pos).length(); //player's distance from monster

            //Based on monster's states, decide what to do
            if(m->m_state != Monster::STATE_ATTACK && personal_space <= m->m_attackRange)
            {
              cout << "Player detected, attack!\n";
              //monster currently not in attack state but player entered his attackRange

              //if close enough, monster should attack the player
              m->changeState(Monster::STATE_ATTACK, (Level*) a, (Player*) p);

              //follow the player
              //m->changeDestination((Level*) a, (Player*) p);
            }
            else if(m->m_state == Monster::STATE_ATTACK && personal_space <= m->m_attackRange)
            {
              //cout << "Player moved, follow him!\n";
              //monster currently in attack state and player is still in attackRange

              //updates to player's new position
              m->changeDestination((Level*) a, (Player*) p);
            }
            else if (m->m_state == Monster::STATE_ATTACK && personal_space >= m->m_attackRange)
            {

              cout << "Lost the player, I'll just wander\n";
              //monster currently in attack state but player left his attackRange

              //Monster goes back to wandering, picks random destination
              m->changeState(Monster::STATE_WANDER, (Level*) a, (Player*) p);
              //m->changeDestination(((Level*)a));
            }
            else //if (m->m_state != Monster::STATE_ATTACK)
            {
              //cout << "Raycast\n";
              //Monster is wandering around, be checking for walls/obstacles etc.

              //Ray from monster's current position to his destination
              end = btVector3(m->m_destinationVector.x, m->m_destinationVector.y, m->m_destinationVector.z);

              //Create call back object and run rayTest
              btCollisionWorld::ClosestRayResultCallback rayCallBack(monster_pos, end);
              dynamicsWorld->rayTest(monster_pos, end, rayCallBack);

              //Check whether ray hit any obstacle
              if(rayCallBack.hasHit())
              {
                //cout << "Hit something\n";
                btVector3 hit = rayCallBack.m_hitPointWorld; //get position of obstacle in ray's way
                btScalar distance = (monster_pos - hit).length(); //calculate distance to obstacle

                //cout << "\nrayCallBack hit something \n" << distance << " meters away!!\n";

                //If ninja is close enough to obstacle, he changes his destination
                if (distance < 5.0f)
                {
                  cout << "\nMonster saw an obstacle (wall), must change destination\n";
                  m->changeDestination(((Level*)a));
                }

              }
            }
            ((Monster*)objList[i])->updateMonsters((Level*)a, evt);
            ((Monster *)objList[i])->updateTransform();

          }

          //objList[i]->updateTransform();
        }

        else if((objList[i]->getName()).compare("player") == 0){
          ((Player*)objList[i])->updateTransform();
        }

        //===========
        //*/
        else {
          objList[i]->updateTransform();
        }

        //cout << objList[i]->getName()<<":";
        //objList[i]->printpos();

    }
    //cout << "for list one ended\n";
    for(int i=deadObjects.size()-1; i >=0;i--){
      if((objList[deadObjects[i]]->getName()).compare("bullet")==0)
      {
        Bullet * go=(Bullet*)objList[deadObjects[i]];
         //delete objList[deadObjects[i]];
        go->getSMP()->destroySceneNode(go->getNode());
        objList.erase(objList.begin()+deadObjects[i]);
      }
      else if ((objList[deadObjects[i]]->getName()).compare("ninja")==0)
      {
        Monster* go=(Monster*)objList[deadObjects[i]];
        monsters_killed++;
        if (monsters_killed%10==0)
          lives++;
        go->monstersound->death(monsters_killed);
        //delete objList[deadObjects[i]];

        go->getSMP()->destroySceneNode(go->getNode());
        ((Level*)objList[1])->num_monsters_left--;
        objList.erase(objList.begin()+deadObjects[i]);
      }


    }

    //cout << "end stepsim\n";
}

btDiscreteDynamicsWorld* Simulator::getWorld(){
    return dynamicsWorld;
}

void Simulator::addObject (GameObject* o) {
  //cout << "\nADDING"<< o<<"\n";
    objList.push_back(o);
    //cout << "\ngetting body...   \n";
    dynamicsWorld->addRigidBody(o->getBody());
    //cout << " ... got body\n";
}

GameObject * Simulator::getObjectPtr(int i){
        return objList[i];
}

void Simulator::clearObjectList ()
{
  for (int i = objList.size() - 1; i >= 2; i--)
  {
    cout << i << "\n";
    GameObject * go=(GameObject*)objList[i];
    cout << "Deleting Object: " << go->getName() << "\n";
         //delete objList[deadObjects[i]];
    go->getSMP()->destroySceneNode(go->getNode());
    objList.erase(objList.begin()+i);
  }
}

int Simulator::getObjectListSize(){
  return objList.size();
}
