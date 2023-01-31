#ifndef ENGINE_H
#define ENGINE_H

#include <ft2build.h>
#include FT_FREETYPE_H

extern FT_Library ft;

// initializes libraries
int EngineInit();

int EngineInitGLAD();

int EngineClean();

#endif
