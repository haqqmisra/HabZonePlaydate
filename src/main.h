#ifndef main_h
#define main_h

#include <stdio.h>
#include <stdlib.h>

#include "pd_api.h"
#include "habzone.h"

#define SCREEN_WIDTH 400
#define SCREEN_HEIGHT 240
#define CURSORINIT 30
#define CURSORDELTA 25
#define LPADDING 30

typedef enum { tempItem, lumItem } Menu;

const char* fontpath = "fonts/ibm-cga-condensed-8x16.pft";

static int update( void* userdata );

#endif
