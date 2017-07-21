/*
 * Copyrights (c):
 *     2002 - 2008 , Werner Freytag.
 *     2009, Haiku
 * Distributed under the terms of the MIT License.
 *
 * Original Author:
 *              Werner Freytag <freytag@gmx.de>
 */

#ifndef _MAIN_VIEW_H
#define _MAIN_VIEW_H

#include <View.h>

class BBitmap;

class MainView : public BView {

public:

		MainView( BRect frame );
		
void	AttachedToWindow();

void	MouseDown( BPoint where );
void	Draw( BRect updateRect );
		
};

#endif
