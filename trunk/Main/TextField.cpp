/*
 * Copyrights (c):
 *     2002 - 2008 , Werner Freytag.
 *     2009, Haiku
 * Distributed under the terms of the MIT License.
 *
 * Original Author:
 *              Werner Freytag <freytag@gmx.de>
 */

#include "TextField.h"

#include <String.h>
#include <Window.h>

#ifdef DEBUG
#include <iostream>
#endif

TextField::TextField( BRect frame, BMessage *message, alignment align )
	:	BControl( BRect( frame.left, frame.top, max_c( frame.right, frame.left + 16), max_c( frame.bottom, frame.top + 16 ) ), 0, 0, message, B_FOLLOW_LEFT, B_WILL_DRAW|B_NAVIGABLE ),
		fChar(""),
		fAlignment( align )
{
}

void TextField::MessageReceived(BMessage *msg)
{
//	msg->PrintToStream();
}

void TextField::Draw( BRect bounds )
{

	BControl::Draw( bounds );

	font_height	fontheight;
	GetFontHeight( &fontheight );
	
	BRect innerRect( 0, 0, max_c( 13, min_c( Frame().Width(), StringWidth( fChar.String() ) + 4 )), min_c( fontheight.ascent + fontheight.descent + 3, Frame().Width() ) );
	
	switch (fAlignment) {
		case B_ALIGN_CENTER:
			innerRect.OffsetTo( ( Frame().Width() - innerRect.Width() ) / 2,  ( Frame().Height() - innerRect.Height() ) / 2 );
		break;

		case B_ALIGN_RIGHT:
			innerRect.OffsetTo( Frame().Width() - innerRect.Width(),  ( Frame().Height() - innerRect.Height() ) / 2 );
		break;
		
		default: ;
	}

	if (IsFocus())	SetLowColor( 60, 70, 150 );
	else			SetLowColor( 245, 245, 245 );
	
	FillRoundRect( innerRect, 1.5, 1.5, B_SOLID_LOW );
	
	BRect rect = innerRect.InsetByCopy( 1.0, 1.0 );

	if (IsFocus())	SetHighColor( 70, 90, 200 );
	else			SetHighColor( 255, 255, 255 );

	StrokeLine( rect.LeftBottom(), rect.RightBottom() );
	StrokeLine( rect.RightTop() );
	
	if (IsFocus())	SetHighColor( 20, 30, 70 );
	else			SetHighColor( 200, 200, 200 );

	StrokeLine( rect.LeftTop() );
	StrokeLine( rect.LeftBottom() );

	// Text

	if (IsFocus())	SetHighColor( 255, 255, 255 );
	else			SetHighColor( 50, 50, 50 );

	DrawString( fChar.String(), BPoint( innerRect.left + 1 + ( innerRect.Width() - StringWidth(fChar.String()) ) / 2, fontheight.ascent + 2 ) );

	// Border
	SetHighColor( 80, 80, 80 );
	StrokeRoundRect( innerRect, 1.5, 1.5 );
}

void TextField::MouseDown( BPoint where )
{
	BControl::MouseDown(where);

	MakeFocus( !IsFocus() );
}

void TextField::KeyDown(const char *bytes, int32 numBytes)
{
	BMessage	*msg = Window()->CurrentMessage();
	msg->what = Message()->what;
	
	SetKeyFromMessage( msg );
	Invoke( msg );

	MakeFocus( false );
}

void TextField::SetKeyFromMessage( BMessage *msg )
{
	const char	*bytes;
	if (!msg ||	msg->FindString("bytes", (const char **)&bytes )!=B_OK) {
		fChar = "";
	}
	else {
		switch (bytes[0]) {
	
			case B_FUNCTION_KEY:
				fChar = "F";
				fChar << (uint)(msg->FindInt32("key") - 1);
			break;
			
			case 0:
				fChar = "";
			break;
			
			case B_BACKSPACE:
				fChar = "Backspace";
			break;
			
			case B_ENTER:
				fChar = "Enter";
			break;
	
			case B_SPACE:
				fChar = "Space";
			break;
			
			case B_TAB:
				fChar = "Tab";
			break;
	
			case B_ESCAPE:
				fChar = "Escape";
			break;
			
			case B_LEFT_ARROW:
				fChar = "Left";
			break;
			
			case B_RIGHT_ARROW:
				fChar = "Right";
			break;
			
			case B_UP_ARROW:
				fChar = "Up";
			break;
			
			case B_DOWN_ARROW:
				fChar = "Down";
			break;
			
			case B_INSERT:
				fChar = "Insert";
			break;
			
			case B_DELETE:
				fChar = "Delete";
			break;
			
			case B_HOME:
				fChar = "Home";
			break;
			
			case B_END:
				fChar = "End";
			break;
			
			case B_PAGE_UP:
				fChar = "Page up";
			break;
			
			case B_PAGE_DOWN:
				fChar = "Page down";
			break;
			
			default:
				msg->FindString("bytes", &fChar );
		}
	}
	
	TruncateString(&fChar, B_TRUNCATE_MIDDLE, Frame().Width() - 5);

	Invalidate();
}
