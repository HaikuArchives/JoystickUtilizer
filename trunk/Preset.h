/*
 * Copyrights (c):
 *     2002 - 2008 , Werner Freytag.
 *     2009, Haiku
 * Distributed under the terms of the MIT License.
 *
 * Original Author:
 *              Werner Freytag <freytag@gmx.de>
 */

#ifndef _PRESET_H
#define _PRESET_H

#include <List.h>
#include <Message.h>
#include <String.h>

#include "JoystickSettings.h"

class Preset {

public:

					Preset( const char *name = 0 );
					~Preset();
	
	void			SetTo( Preset &from );
	void			Clear();
	
	void			AddJoystick( JoystickSettings *js );
	
	uint8			CountJoysticks();

	const char		*NameOfJoystick( uint8 index );
	void			SetJoystickButton( uint8 joystick_index, uint8 button_index, BMessage &msg );
	
	void			LoadFromMessage( BMessage &settingMsg );
	BMessage		&SaveToMessage();

	BString			fName;
	BList			fJoystickSettings;
	
};

#endif
