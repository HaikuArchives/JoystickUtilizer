/*
 * Copyrights (c):
 *     2002 - 2008 , Werner Freytag.
 *     2009, Haiku
 * Distributed under the terms of the MIT License.
 *
 * Original Author:
 *              Werner Freytag <freytag@gmx.de>
 */

#ifndef _SETTINGS_VIEW_H
#define _SETTINGS_VIEW_H

#include <Box.h>
#include <List.h>

class BBitmap;
class BTabView;
class TextField;

class SettingsView : public BBox {

public:
	
				SettingsView( BRect frame );
				~SettingsView();
	
	void		AttachedToWindow();
	
	void		UpdateSettings( bool updateKeys = true );
			
	void		MouseDown( BPoint where );
	
	void		Draw( BRect updateRect );

private:
	
	BList		fJoystickSettingView;
	BTabView	*fTabView;
	
	
};

#endif
