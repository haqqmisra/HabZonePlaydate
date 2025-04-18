#ifndef habzone_h
#define habzone_h

#include <math.h>

#define VERSION 1.0
#define SOLTEMP 5780
#define SOLLUM 1.0

typedef enum { recentVenus, runawayGreenhouse, maximumGreenhouse, earlyMars } Zone;

float distance( float lum, float seff );
float getBoundary( float temp, Zone whichzone );
float calculateHZ( float seffSun, float a, float b, float c, float d, float tempdiff );

#endif
