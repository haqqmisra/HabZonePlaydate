#include "stars.h"

void changePresetStar( float* temp, float* lum, Star preset )
{
	*temp = starTemps[ preset ];
	*lum  = starLums[ preset ];

	return;
}

Star checkCurrentStar( float temp, float lum )
{
	int i, current;

	current = -1;
	for ( i = 0; i < NUM_PRESETS; i++ ) {
		if ( ( temp == starTemps[i] ) && ( lum == starLums[i] ) ) {
			current = i;
		}
	}
	return current;
}
