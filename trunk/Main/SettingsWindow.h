/*
 * Copyrights (c):
 *     2002 - 2008 , Werner Freytag.
 *     2009, Haiku
 * Distributed under the terms of the MIT License.
 *
 * Original Author:
 *              Werner Freytag <freytag@gmx.de>
 */

#ifndef _SETTINGS_WINDOW_H
#define _SETTINGS_WINDOW_H

#include <Message.h>
#include <Window.h>

class SettingsWindow : public BWindow {

public:
				SettingsWindow();
				~SettingsWindow();
				
	void		MessageReceived(BMessage *message);
	
};

#endif
