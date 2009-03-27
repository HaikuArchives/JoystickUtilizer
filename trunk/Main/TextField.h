/*
 * Copyrights (c):
 *     2002 - 2008 , Werner Freytag.
 *     2009, Haiku
 * Distributed under the terms of the MIT License.
 *
 * Original Author:
 *              Werner Freytag <freytag@gmx.de>
 */

#ifndef _TEXT_FIELD_H
#define _TEXT_FIELD_H

#include <Control.h>
#include <String.h>

class TextField : public BControl
{

public:
					TextField( 	BRect frame,
								BMessage *message,
								alignment align = B_ALIGN_LEFT );
					
	void			MessageReceived( BMessage *msg );
	
	void			Draw( BRect bounds );
	
	void			MouseDown( BPoint where );

	void			KeyDown(const char *bytes, int32 numBytes);

	void			SetKeyFromMessage( BMessage *msg );

private:
	
	BString			fChar;
	alignment		fAlignment;
	
};

#endif
