#ifndef stars_h
#define stars_h

#define NUM_PRESETS 10

typedef enum {
	Sun,
	ProximaCentauri,
	TRAPPIST1,
	Pegasi51,
	K218,
	Wolf1069,
	BarnardsStar,
	LHS1140,
	GJ1061,
	Kepler22,
	UserDefStar
} Star;

static const char* starlabels[NUM_PRESETS+1] = {
	"Sun",
	"Proxima Cen",
	"TRAPPIST-1",
	"51 Pegasi",
	"K2-18",
	"Wolf 1069",
	"Barnard's",
	"LHS 1140",
	"GJ 1061",
	"Kepler 22",
	"User Defined"
};

static const float starTemps[] = {
	[Sun]             = 5780,
	[ProximaCentauri] = 2992,
	[TRAPPIST1]       = 2566,
	[Pegasi51]        = 5768,
	[K218]            = 3645,
	[Wolf1069]        = 3158,
	[BarnardsStar]    = 3195,
	[LHS1140]         = 3096,
	[GJ1061]          = 2977,
        [Kepler22]        = 5596
};

static const float starLums[] = {
	[Sun]             = 1.0,
	[ProximaCentauri] = 0.002,
	[TRAPPIST1]       = 0.001,
	[Pegasi51]        = 1.398,
	[K218]            = 0.023,
	[Wolf1069]        = 0.003,
	[BarnardsStar]    = 0.003,
	[LHS1140]         = 0.004,
	[GJ1061]          = 0.002,
        [Kepler22]        = 0.79
};

void changePresetStar( float* temp, float* lum, Star preset );
Star checkCurrentStar( float temp, float lum );

#endif
