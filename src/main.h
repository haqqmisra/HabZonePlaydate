#ifndef main_h
#define main_h

#include <stdio.h>
#include <stdlib.h>

#include "pd_api.h"
#include "habzone.h"
#include "stars.h"

#define CURSORINIT 30
#define CURSORDELTA 25
#define LPADDING 30

typedef enum { tempItem, lumItem } Menu;
const char* fontpath = "fonts/ibm-cga-condensed-8x16.pft";

static int update( void* userdata );
void reset( void* userdata );
void changeMass( void* userdata );
void selectStar( void* userdata );
void selectPlanet( void* userdata );

#endif
