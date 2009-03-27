/*
 * Copyrights (c):
 *     2002 - 2008 , Werner Freytag.
 *     2009, Haiku
 * Distributed under the terms of the MIT License.
 *
 * Original Author:
 *              Werner Freytag <freytag@gmx.de>
 */

#ifndef _JOYSTICK_SETTINGS_H
#define _JOYSTICK_SETTINGS_H

#include <Joystick.h>
#include <Message.h>

class JoystickSettings : public BJoystick {

friend class Preset;

public:
					JoystickSettings( const char *name );
					JoystickSettings( JoystickSettings *js );

status_t			Open();
void				Close();

inline int32		CountButtons() { return fCountButtons; }
inline const char	*Name() { return fName; }
inline BMessage		*Button( int32 button_nr ) { return &fButtons[button_nr]; }
inline bool			IsOpen() { return fIsOpen; }

private:

char				fName[B_OS_NAME_LENGTH];
BMessage			fButtons[20];
int32				fCountButtons;
bool				fIsOpen;
};

#endif
