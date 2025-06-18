#include "main.h"

PlaydateAPI* pd = NULL;
LCDFont* font = NULL;
PDMenuItem *resetItem = NULL;
PDMenuItem *planetItem = NULL;
PDMenuItem *starItem  = NULL;

Menu currentSelection;
Star currentStar;
Planet currentPlanet;

float temperature, luminosity;
char * stringnum;
int cursorposition;
int tflag = 0;

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

		resetItem  = pd->system->addMenuItem( "Reset", reset, pd );
		planetItem = pd->system->addOptionsMenuItem( "Planet", planetlabels, NUM_PLANETS, selectPlanet, pd );
		starItem   = pd->system->addOptionsMenuItem( "Star", starlabels, NUM_PRESETS, selectStar, pd );

		reset( pd );
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
		if ( currentSelection == tempItem ) {
			temperature += crankchange;
		}
		else {
			luminosity += crankchange / 1000;
		}
			tflag = 1;
        }
	if ( pushed & kButtonRight ) {
		if ( currentSelection == tempItem ) {
			temperature++;
		}
		else {
			luminosity+=0.001;
		}
		tflag = 1;
	}
	if ( pushed & kButtonLeft ) {
		if ( currentSelection == tempItem ) {
			temperature--;
		}
		else {
			luminosity-=0.001;
		}
			tflag = 1;
	}
	if ( pushed & kButtonB ) {
		reset( pd );
	}

	// Update current star name
	if ( tflag == 1 ) {
		pd->system->removeMenuItem( starItem );
		currentStar = checkCurrentStar( temperature, luminosity );
		if ( currentStar == -1 ) {
			starItem = pd->system->addOptionsMenuItem( "Star", starlabels, NUM_PRESETS+1, selectStar, pd );
			pd->system->setMenuItemValue( starItem, UserDefStar );
		}
		else {
			starItem = pd->system->addOptionsMenuItem( "Star", starlabels, NUM_PRESETS, selectStar, pd );
			pd->system->setMenuItemValue( starItem, currentStar );
		}
		tflag = 0;
	}

	// Use Up/Down buttons to move cursor
	if ( pushed & kButtonUp ) {
		cursorposition = CURSORINIT;
		currentSelection = tempItem;
	}
	if ( pushed & kButtonDown ) {
		cursorposition = CURSORINIT + CURSORDELTA;
		currentSelection = lumItem;
	}

	// Keep inputs within the minimum and maximum bounds
	if ( temperature < 2500 ) {
		temperature = 2500;
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
	pd->graphics->drawText( "Habitable Zone Calculator", 25, kASCIIEncoding, 3, 3 );
	if ( currentPlanet == subEarth ) {
		pd->graphics->drawText( planetlonglabels[currentPlanet], strlen( planetlonglabels[currentPlanet] ), kASCIIEncoding, 285, 3 );
	}
	else {
		pd->graphics->drawText( planetlonglabels[currentPlanet], strlen( planetlonglabels[currentPlanet] ), kASCIIEncoding, 301, 3 );
	}
	pd->graphics->drawText( "__________________________________________________", 50, kASCIIEncoding, 0, 8 );

	// Input values
	pd->system->formatString( &stringnum, "Stellar Temperature:  %4d Kelvin", (int)temperature );
	//pd->system->formatString( &stringnum, "Stellar Temperature:  %4.0f Kelvin", temperature );
	pd->graphics->drawText( stringnum, strlen( stringnum ), kASCIIEncoding, LPADDING, CURSORINIT );
	pd->system->formatString( &stringnum, "Luminosity:           %1d.%003d solar units", (int)luminosity, (int)((luminosity-(int)luminosity)*1000) );
	//pd->system->formatString( &stringnum, "Luminosity:           %1.3f solar units", luminosity );
	pd->graphics->drawText( stringnum, strlen( stringnum ), kASCIIEncoding, LPADDING, CURSORINIT + CURSORDELTA );
	pd->graphics->drawText( ">", 1, kASCIIEncoding, LPADDING - 10, cursorposition );
	pd->graphics->drawText( "__________________________________________________", 50, kASCIIEncoding, 0, CURSORINIT + 2*CURSORDELTA - 16 );

	// Display output values
	pd->graphics->drawText( "Conservative Limits", 20, kASCIIEncoding, LPADDING, CURSORINIT + 3*CURSORDELTA - 16 );

	pd->system->formatString( &stringnum, "Inner HZ - Runaway Greenhouse:  %2.2f AU", distance( luminosity, getBoundary( temperature, runawayGreenhouse, currentPlanet ) ) );
	pd->graphics->drawText( stringnum, strlen( stringnum ), kASCIIEncoding, LPADDING + 30, CURSORINIT + 4*CURSORDELTA - 16 );

	pd->system->formatString( &stringnum, "Outer HZ - Maximum Greenhouse:  %2.2f AU", distance( luminosity, getBoundary( temperature, maximumGreenhouse, currentPlanet ) ) );
	pd->graphics->drawText( stringnum, strlen( stringnum ), kASCIIEncoding, LPADDING + 30, CURSORINIT + 5*CURSORDELTA - 16 );

	pd->graphics->drawText( "Optimistic Limits", 20, kASCIIEncoding, LPADDING, CURSORINIT + 6*CURSORDELTA - 16 );

	pd->system->formatString( &stringnum, "Inner HZ - Recent Venus:        %2.2f AU", distance( luminosity, getBoundary( temperature, recentVenus, currentPlanet ) ) );
	pd->graphics->drawText( stringnum, strlen( stringnum ), kASCIIEncoding, LPADDING + 30, CURSORINIT + 7*CURSORDELTA - 16 );

	pd->system->formatString( &stringnum, "Outer HZ - Early Mars:          %2.2f AU", distance( luminosity, getBoundary( temperature, earlyMars, currentPlanet ) ) );
	pd->graphics->drawText( stringnum, strlen( stringnum ), kASCIIEncoding, LPADDING + 30, CURSORINIT + 8*CURSORDELTA - 16 );

	return 1;
}

void reset( void* userdata )
{
	PlaydateAPI* pd = userdata;
	pd->system->setMenuItemValue( starItem, Sun );

	currentPlanet = Earth;

	changePresetStar( &temperature, &luminosity, Sun );
	cursorposition = CURSORINIT;
	currentSelection = tempItem;

	return;
}

void selectStar( void* userdata )
{
	PlaydateAPI* pd = userdata;
	currentStar = pd->system->getMenuItemValue( starItem );

	changePresetStar( &temperature, &luminosity, currentStar );

	return;
}

void selectPlanet( void* userdata )
{
	PlaydateAPI* pd = userdata;
	currentPlanet = pd->system->getMenuItemValue( planetItem );
}
