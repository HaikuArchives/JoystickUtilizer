/*
 * Copyrights (c):
 *     2002 - 2008 , Werner Freytag.
 *     2009, Haiku
 * Distributed under the terms of the MIT License.
 *
 * Original Author:
 *              Werner Freytag <freytag@gmx.de>
 */

#ifndef _NEW_PRESET_WINDOW_H
#define _NEW_PRESET_WINDOW_H

#include <Window.h>

class NewPresetWindow : public BWindow
{

public:

				NewPresetWindow( BLooper *target );

void			MessageReceived(BMessage *message);


private:

BLooper			*fTarget;

};

#endif
