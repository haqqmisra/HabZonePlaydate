#ifndef habzone_h
#define habzone_h

#include <math.h>

#define SOLTEMP 5780
#define SOLLUM 1.0
#define NUM_PLANETS 3

typedef enum {
	recentVenus,
	runawayGreenhouse,
	maximumGreenhouse,
	earlyMars
} Zone;

typedef enum {
	Earth,
	superEarth,
	subEarth
} Planet;

static const char* planetlabels[NUM_PLANETS] = {
	"Earth",
	"sup-Earth",
	"sub-Earth"
};

static const char* planetlonglabels[NUM_PLANETS] = {
	"1 Earth mass",
	"5 Earth mass",
	"0.1 Earth mass"
};

float distance( float lum, float seff );
float getBoundary( float temp, Zone zone, Planet planet );
float calculateHZ( float seffSun, float a, float b, float c, float d, float tempdiff );

#endif
