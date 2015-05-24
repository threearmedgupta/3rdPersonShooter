
#ifndef __gameenums_h__
#define __gameenums_h__

enum GameState{
	Main = 0,
	Store,
	Play,
	Pause,
	Win,
	Lose
};

enum WeaponState{
	Weapon1 = 0,
	Weapon2,
	Weapon3,
	Weapon4
};

enum PlayerState{
	NoFire = 0,
	Fire,
	Reload
};
/*
  tiles:
    0. no tile  1. no wall  2. north  3. south 4. east  5. west
    6. north-e  7. north-w  8. south-e  9. south-w
    10. nse 11. nsw 12. new 13. sew
*/
enum Tiles{
	NOTILE=0,
	NOWALL,
	N,S,E,W,
	NE,NW,SE,SW,
	NSE,NSW,NEW,SEW
};


#endif
