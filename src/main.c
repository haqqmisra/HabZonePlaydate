#include "main.h"

PlaydateAPI* pd = NULL;
LCDFont* font = NULL;
PDMenuItem *resetButton = NULL;
PDMenuItem *unitsButton = NULL;
PDMenuItem *dataButton  = NULL;

Menu currentItem;

float temperature, luminosity;
char * stringnum;
int cursorposition;

int eventHandler( PlaydateAPI* pd, PDSystemEvent event, uint32_t arg )
{
	(void)arg; // arg is currently only used for event = kEventKeyPressed

	if ( event == kEventInit ) {
		const char* err;
		font = pd->graphics->loadFont( fontpath, &err );

		if ( font == NULL ) {
			pd->system->error( "%s:%i Couldn't load font %s: %s", __FILE__, __LINE__, fontpath, err );
		}
		pd->system->setUpdateCallback( update, pd );

		temperature = SOLTEMP;
		luminosity = SOLLUM;
		cursorposition = CURSORINIT;
		currentItem = tempItem;
	}
	else if ( event == kEventResume ) {
		//
	}
        else if ( event == kEventTerminate ) {
		//
	}

	return 0;
}

static int update( void* userdata )
{
        pd = userdata;

        pd->graphics->clear( kColorWhite );
        pd->graphics->setFont( font );

        PDButtons pushed;
        pd->system->getButtonState( NULL, &pushed, NULL );

	// Use crank and Left/Right buttons to adjust temperature
        float crankchange = 0;
        if ( pd->system->isCrankDocked() == 0 ) {
                crankchange = pd->system->getCrankChange();
		if ( currentItem == tempItem ) {
			temperature += crankchange;
		}
		else {
			luminosity += crankchange / 1000;
		}
        }
	if ( pushed & kButtonRight ) {
		if ( currentItem == tempItem ) {
			temperature++;
		}
		else {
			luminosity+=0.001;
		}
	}
	if ( pushed & kButtonLeft ) {
		if ( currentItem == tempItem ) {
			temperature--;
		}
		else {
			luminosity-=0.001;
		}
	}
	if ( pushed & kButtonB ) {
		temperature = SOLTEMP;
		luminosity = SOLLUM;
	}

	// Use Up/Down buttons to move cursor
	if ( pushed & kButtonUp ) {
		cursorposition = CURSORINIT;
		currentItem = tempItem;
	}
	if ( pushed & kButtonDown ) {
		cursorposition = CURSORINIT + CURSORDELTA;
		currentItem = lumItem;
	}

	// Keep inputs within the minimum and maximum bounds
	if ( temperature < 2600 ) {
		temperature = 2600;
	}
	else if ( temperature > 7200 ) {
		temperature = 7200;
	}
	if ( luminosity < 0.0 ) {
		luminosity = 0.0;
	}
	else if ( luminosity > 9.0 ) {
		luminosity = 9.0;
	}

	// Display header text
	pd->system->formatString( &stringnum, "Habitable Zone Calculator, v%1.1f", VERSION );
	pd->graphics->drawText( stringnum, strlen( stringnum ), kASCIIEncoding, 3, 3 );
	pd->graphics->drawText( "__________________________________________________", 50, kASCIIEncoding, 0, 8 );

	// Input values
	pd->system->formatString( &stringnum, "Stellar Temperature:  %4.0f Kelvin", temperature );
	pd->graphics->drawText( stringnum, strlen( stringnum ), kASCIIEncoding, LPADDING, CURSORINIT );
	pd->system->formatString( &stringnum, "Luminosity:           %1.3f solar units", luminosity );
	pd->graphics->drawText( stringnum, strlen( stringnum ), kASCIIEncoding, LPADDING, CURSORINIT + CURSORDELTA );
	pd->graphics->drawText( ">", 1, kASCIIEncoding, LPADDING - 10, cursorposition );
	pd->graphics->drawText( "__________________________________________________", 50, kASCIIEncoding, 0, CURSORINIT + 2*CURSORDELTA - 16 );

	// Display output values
	pd->graphics->drawText( "Conservative Limits", 20, kASCIIEncoding, LPADDING, CURSORINIT + 3*CURSORDELTA - 16 );

	pd->system->formatString( &stringnum, "Inner HZ - Runaway Greenhouse:  %2.2f AU", distance( luminosity, getBoundary( temperature, runawayGreenhouse ) ) );
	pd->graphics->drawText( stringnum, strlen( stringnum ), kASCIIEncoding, LPADDING + 30, CURSORINIT + 4*CURSORDELTA - 16 );

	pd->system->formatString( &stringnum, "Outer HZ - Maximum Greenhouse:  %2.2f AU", distance( luminosity, getBoundary( temperature, maximumGreenhouse ) ) );
	pd->graphics->drawText( stringnum, strlen( stringnum ), kASCIIEncoding, LPADDING + 30, CURSORINIT + 5*CURSORDELTA - 16 );

	pd->graphics->drawText( "Optimistic Limits", 20, kASCIIEncoding, LPADDING, CURSORINIT + 6*CURSORDELTA - 16 );

	pd->system->formatString( &stringnum, "Inner HZ - Recent Venus:        %2.2f AU", distance( luminosity, getBoundary( temperature, recentVenus ) ) );
	pd->graphics->drawText( stringnum, strlen( stringnum ), kASCIIEncoding, LPADDING + 30, CURSORINIT + 7*CURSORDELTA - 16 );

	pd->system->formatString( &stringnum, "Outer HZ - Early Mars:          %2.2f AU", distance( luminosity, getBoundary( temperature, earlyMars ) ) );
	pd->graphics->drawText( stringnum, strlen( stringnum ), kASCIIEncoding, LPADDING + 30, CURSORINIT + 8*CURSORDELTA - 16 );

	return 1;
}
