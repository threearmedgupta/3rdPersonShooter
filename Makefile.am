ACLOCAL_AMFLAGS= -I m4
noinst_HEADERS= BaseApplication.h GameObject.h Simulator.h Level.h Tile.h Weapon.h Monster.h BGMusic.h WeaponSound.h Player.h BulletSound.h MonsterSound.h

bin_PROGRAMS= OgreApp
OgreApp_SOURCES= BaseApplication.cpp GameObject.cpp Simulator.cpp Level.cpp Tile.cpp Weapon.cpp Monster.cpp BGMusic.cpp WeaponSound.cpp Player.cpp BulletSound.cpp MonsterSound.cpp

OgreApp_CPPFLAGS= -I$(top_srcdir)
OgreApp_CXXFLAGS=$(OGRE_CFLAGS) $(OIS_CFLAGS) $(bullet_CFLAGS) $(CEGUI_CFLAGS) $(CEGUI_OGRE_CFLAGS) $(sdl_CFLAGS)
OgreApp_LDADD= $(OGRE_LIBS) $(OIS_LIBS) $(bullet_LIBS) $(CEGUI_LIBS) $(CEGUI_OGRE_LIBS) $(sdl_LIBS)
OgreApp_LDFLAGS= -lOgreOverlay -lboost_system -lSDL_mixer

EXTRA_DIST= buildit makeit
AUTOMAKE_OPTIONS= foreign
