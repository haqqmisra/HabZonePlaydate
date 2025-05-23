#include "habzone.h"

//-------------------------------------------------------
// This implementation of the Kopparapu et al. (2013) 
// Habitable Zone calculator is based on the Python code 
// developed by Eelt at:
// https://github.com/Eelt/HabitableZoneCalculator.git
//-------------------------------------------------------

float distance( float lum, float seff )
{
	return powf( lum / seff, 0.5 );
}

float getBoundary( float temp, Zone zone, Planet planet )
{
	float tempdiff, seffSun, a, b, c, d;

	tempdiff = temp - 5780;

	if ( zone == recentVenus ) {
		seffSun = 1.766;
		a = 2.136e-4;
		b = 2.533e-8;
		c = -1.332e-11;
		d = -3.097e-15;
	}
	if ( zone == runawayGreenhouse ) {
		if ( planet == superEarth ) {
			seffSun = 1.188;
			a = 1.433e-4;
			b = 1.707e-8;
			c = -8.968e-12;
			d = -2.084e-15;
		}
		else if ( planet == subEarth ) {
			seffSun = 0.99;
			a = 1.209e-4;
			b = 1.404e-8;
			c = -7.418e-12;
			d = -1.713e-15;
		}
		else { // values for Earth
			seffSun = 1.107;
			a = 1.332e-4;
			b = 1.580e-8;
			c = -8.308e-12;
			d = -1.931e-15;
		}
	}
	if ( zone == maximumGreenhouse ) {
		seffSun = 0.356;
		a = 6.171e-5;
		b = 1.689e-9;
		c = -3.198e-12;
		d = -5.575e-16;
	}
	if ( zone == earlyMars ) {
		seffSun = 0.320;
		a = 5.547e-5;
		b = 1.526e-9;
		c = -2.874e-12;
		d = -5.011e-16;
	}

	return calculateHZ( seffSun, a, b, c, d, tempdiff );
}

float calculateHZ( float seffSun, float a, float b, float c, float d, float tempdiff )
{
	return seffSun + ( a * tempdiff ) + ( b * powf( tempdiff, 2 ) ) + ( c *powf( tempdiff, 3 ) ) + ( d * powf( tempdiff, 4) );
}
