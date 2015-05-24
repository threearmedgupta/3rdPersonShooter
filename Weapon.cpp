#include "Weapon.h"

Weapon::Weapon(const int weapon)
{

	float bulletsize = 0.1;
  mat = "metal";

	//physics geom

	mass = 100;
	reloadTimer = new Ogre::Timer();
	fireTimer = new Ogre::Timer();
	switchTimer = new Ogre::Timer();
	weaponsound = new WeaponSound(0);
	reloadBool = false;
	switchtime = 500;
	bulletSize=0.1;
	bulletSpeed=20.0;
	count=0;
	lifetime = 10000;
	switch (weapon){
		case WeaponState::Weapon1:
		{
			name = "Lead Shiruken";
			power = 1;
			ammo = 10;
			total_ammo = 100000;
			ammo_cap = 10;
			total_ammo_cap = 100000;
			firetime = 350;
			reloadtime = 1500;
			bulletSize=0.075;
			mass=10;
			spf = 1;
			break;
		}
		case WeaponState::Weapon2:
		{
			name = "Lightweight Shiruken";
			power = 1;
			ammo = 20;
			total_ammo = 200;
			ammo_cap = 20;
			total_ammo_cap = 200;
			firetime = 100;
			reloadtime = 2000;
			bulletSize=0.08;
			mass=10;
			spf = 1;
			break;
		}
		case WeaponState::Weapon3:
		{
			name = "Fire Ninpo";
			power = 3;
			ammo = 10;
			total_ammo = 100;
			ammo_cap = 10;
			total_ammo_cap = 100;
			firetime = 800;
			reloadtime = 3500;
			bulletSize=0.3;
			mass=20;
			spf = 1;
			lifetime = 200;
			bulletSpeed = 40.0;
			mat="firestar";
			break;
		}
		case WeaponState::Weapon4:
		{
			name = "Ice Ninpo";
			power = 1;
			ammo = 15;
			total_ammo = 90;
			ammo_cap = 15;
			total_ammo_cap = 90;
			firetime = 600;
			reloadtime = 3000;
			bulletSize=0.15;
			mass=10;
			mat="icestar";
			spf = 3;
			break;
		}
	}
	temp_firetime = firetime;
	temp_switchtime = switchtime;
	temp_reloadtime = reloadtime;
	reset_ammo = ammo;
	reset_total_ammo = total_ammo;

	col_shape=new btSphereShape(bulletSize);
}

void Weapon::setSMP(Ogre::SceneManager* smp){
	sceneMgr=smp;
}

Weapon::~Weapon()
{
	delete reloadTimer;
	delete fireTimer;
}

bool Weapon::next_level(int level)
{
	if (level % 5 == 0)
	{
		replenish_weapon();
		return true;
	}
	else
	{
		reset_ammo = ammo;
		reset_total_ammo = total_ammo;
		return false;
	}
}

void Weapon::replenish_weapon()
{
	ammo = ammo_cap;
	reset_ammo = ammo_cap;
	total_ammo = total_ammo_cap;
	reset_total_ammo = total_ammo_cap;
}

void Weapon::reset_level()
{
	ammo = reset_ammo;
	total_ammo = reset_total_ammo;
}

int Weapon::ammo_left()
{
	return ammo;
}

int Weapon::total_ammo_left()
{
	return total_ammo;
}


int Weapon::fire(void)
{
	if (fireTimer->getMilliseconds() >= temp_firetime && switchTimer->getMilliseconds() >= temp_switchtime)
	{
		temp_firetime = firetime;
		temp_switchtime = switchtime;
		if (ammo == 0)
		{
			return Weapon::reload();
		}
		else
		{
			// FIRE
			ammo = ammo - spf;
			if (name != "Lead Shiruken")
			{
				total_ammo = total_ammo - spf;
			}
			weaponsound->fire();
			fireTimer->reset();
			return 1;
			//spawnBullet();
		}
	}
	return 2;
}

void Weapon::pause()
{
	temp_firetime = temp_firetime - fireTimer->getMilliseconds();
	temp_switchtime = temp_switchtime - fireTimer->getMilliseconds();
	temp_reloadtime = temp_reloadtime - fireTimer->getMilliseconds();
	weaponsound->pause();
	// cout << "\n pausetime: " << temp_reloadtime << "\n";
}

void Weapon::unpause()
{
	reloadTimer->reset();
	fireTimer->reset();
	switchTimer->reset();
	weaponsound->unpause();
}

void Weapon::cancel_reload(void)
{
	reloadBool = false;
	weaponsound->switch_weapon();
}

Bullet* Weapon::spawnBullet(btVector3 playerPos, btVector3 dir, Simulator* simulator)
{
	Bullet * b = new Bullet(this,sceneMgr,simulator,playerPos, dir.rotate(btVector3(0, 1, 0), btScalar(-0.1)),std::to_string(count),mat);


	count++;
	if(count<0)
		count=0;
	if (name == "Ice Ninpo")
	{
		Bullet * b2 = new Bullet(this,sceneMgr,simulator,playerPos, dir,std::to_string(count),mat);

		count++;
		Bullet * b3 = new Bullet(this,sceneMgr,simulator,playerPos, dir.rotate(btVector3(0, 1, 0), btScalar(0.1)),std::to_string(count),mat);

		count++;
	}
	return b;
}

bool Weapon::reload(void)
{
	// cout << "\n can we reload? " << reloadTimer->getMilliseconds() << " vs " << temp_reloadtime;
	if (ammo == ammo_cap || total_ammo <= 0)
		return true;
	else if (reloadBool == false && switchTimer->getMilliseconds() >= temp_switchtime)
	{
		temp_switchtime = switchtime;
		weaponsound->reload();
		reloadTimer->reset();
		reloadBool = true;
	}
	else if (reloadTimer->getMilliseconds() >= temp_reloadtime)
	{
		temp_reloadtime = reloadtime;
		reloadBool = false;
		reloadTimer->reset();
		if (total_ammo >= ammo_cap)
			ammo = ammo_cap;
		else
			ammo = total_ammo;
		return true;
	}
	return false;
}

void Weapon::switch_weapon(void)
{
	switchTimer->reset();
}
