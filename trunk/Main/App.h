/*
 * Copyrights (c):
 *     2002 - 2008 , Werner Freytag.
 *     2009, Haiku
 * Distributed under the terms of the MIT License.
 *
 * Original Author:
 *              Werner Freytag <freytag@gmx.de>
 */

#ifndef _APP_H
#define _APP_H

#include <Application.h>
#include <Path.h>

class SettingsWindow;

class App : public BApplication {

public:
					App();
					~App();
					
	void			MessageReceived(BMessage *msg);
	
	void			AboutRequested();

private:

	SettingsWindow	*fSettingsWindow;
			
};

#endif
