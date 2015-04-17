ACLOCAL_AMFLAGS= -I m4
noinst_HEADERS= BaseApplication.h

bin_PROGRAMS= OgreApp
OgreApp_SOURCES= BaseApplication.cpp GameObject.cpp config.h GameObject.h Simluator.h Simulator.cpp Level.h Level.cpp Tile.h Tile.cpp

OgreApp_CPPFLAGS= -I$(top_srcdir)
OgreApp_CXXFLAGS=$(OGRE_CFLAGS) $(OIS_CFLAGS) $(bullet_CFLAGS) $(CEGUI_CFLAGS) $(CEGUI_OGRE_CFLAGS) $(sdl_CFLAGS)
OgreApp_LDADD= $(OGRE_LIBS) $(OIS_LIBS) $(bullet_LIBS) $(CEGUI_LIBS) $(CEGUI_OGRE_LIBS) $(sdl_LIBS)
OgreApp_LDFLAGS= -lOgreOverlay -lboost_system -lSDL_mixer

EXTRA_DIST= buildit makeit
AUTOMAKE_OPTIONS= foreign
