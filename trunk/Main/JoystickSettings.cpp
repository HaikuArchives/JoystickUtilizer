/*
 * Copyrights (c):
 *     2002 - 2008 , Werner Freytag.
 *     2009, Haiku
 * Distributed under the terms of the MIT License.
 *
 * Original Author:
 *              Werner Freytag <freytag@gmx.de>
 */

#include "JoystickSettings.h"

#include <string.h>

#ifdef DEBUG
#include <iostream>
#include <Beep.h>
#include <File.h>
#endif

JoystickSettings::JoystickSettings( const char *name )
	:	fCountButtons( 0 ),
		fIsOpen( false )
{
	strcpy(fName, name);
}

JoystickSettings::JoystickSettings( JoystickSettings *js )
	:	fCountButtons( js->fCountButtons )
{
	strcpy(fName, js->Name());

	for (int i=0; i<20; ++i)
		fButtons[i] = js->fButtons[i];
}

status_t JoystickSettings::Open()
{
	if (!fName || !strlen(fName)) return B_ERROR;
	
	if (fIsOpen) return B_OK;
	
	status_t result = BJoystick::Open( fName );
	if (result!=B_ERROR) {
		fCountButtons = min_c( 16, BJoystick::CountButtons() );
		fIsOpen = true;
	}
	
	return result;
	
}

void JoystickSettings::Close()
{
	if (fIsOpen) BJoystick::Close();
	fIsOpen = false;
}
