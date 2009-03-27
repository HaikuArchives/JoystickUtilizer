/*
 * Copyrights (c):
 *     2002 - 2008 , Werner Freytag.
 *     2009, Haiku
 * Distributed under the terms of the MIT License.
 *
 * Original Author:
 *              Werner Freytag <freytag@gmx.de>
 */

#ifndef _JOYSTICK_SETTING_VIEW_H
#define _JOYSTICK_SETTING_VIEW_H

#include <View.h>

class BBitmap;

class JoystickSettings;
class TextField;

class JoystickSettingView : public BView {

public:
	
						JoystickSettingView( BRect frame, const char *name, uint8 joystick_nr );
						~JoystickSettingView();
	
	void				UpdateTextFields();
			
	void				MouseDown( BPoint where );
	void				Draw( BRect updateRect );

	void				Pulse();
			
private:

	BBitmap				*fJoystickBmp, *fButtonBmp, *fButtonActiveBmp;
	TextField			*fTextField[20];

	uint8				fJoystickNr;
	JoystickSettings	*fJoystickSettings;

	int32				fNumButtons;
};

#endif
