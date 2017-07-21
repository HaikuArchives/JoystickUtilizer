/*
 * Copyrights (c):
 *     2002 - 2008 , Werner Freytag.
 *     2009, Haiku
 * Distributed under the terms of the MIT License.
 *
 * Original Author:
 *              Werner Freytag <freytag@gmx.de>
 */

#include "NewPresetWindow.h"

#include <Beep.h>
#include <Button.h>
#include <Screen.h>
#include <String.h>
#include <TextControl.h>
#include <View.h>

#ifdef DEBUG
#include <iostream>
#endif

NewPresetWindow::NewPresetWindow( BLooper *target )
	:	BWindow( BRect( 0, 0, 0, 0), "Create new preset", B_MODAL_WINDOW, B_NOT_RESIZABLE|B_ASYNCHRONOUS_CONTROLS ),
		fTarget( target )
{
	// resize and move
	ResizeTo( 260, be_plain_font->Size() * 2 + 50 );
	BScreen	screen( this );
	
	MoveTo( (screen.Frame().Width() - Bounds().Width()) / 2,
			(screen.Frame().Height() - Bounds().Height()) / 2 );

	// main view
	BView *view = new BView( Bounds(), 0, B_FOLLOW_ALL, B_WILL_DRAW );
	view->SetViewColor( 222, 222, 222 );
	AddChild( view );
	
	// text control
	BRect rect( 8, 8, Frame().Width() - 16, 8 + be_plain_font->Size() + 4 );
	
	BTextControl	*textcontrol = new BTextControl( rect, "textcontrol",
		"Preset name:", "", 0 );
	textcontrol->SetDivider( be_plain_font->StringWidth( "Preset name:" ) + 8 );
	textcontrol->TextView()->SetMaxBytes(255);
	view->AddChild( textcontrol );
	
	// cancel button
	rect.left = textcontrol->Divider() + 8;
	rect.right = rect.left + (rect.Width() - 12) / 2;
	rect.top = rect.bottom + 16;
	rect.bottom = rect.top + be_plain_font->Size() + 8;
	
	BButton	*button = new BButton( rect, "cancelbutton", "Cancel",
		new BMessage('Canc') );
	view->AddChild( button );

	// add button
	rect.OffsetTo( rect.right + 12, rect.top );
	
	button = new BButton( rect, "addbutton", "Add", new BMessage('Add') );
	view->AddChild( button );
	button->MakeDefault( true );
	
}

void NewPresetWindow::MessageReceived(BMessage *message)
{
	switch (message->what) {
		case 'Canc':
			
			message->what = 'CncP';
			fTarget->PostMessage( message );
			Quit();

		break;

		case 'Add': {
			
			bool exit (true);
			
			BTextControl *textcontrol = (BTextControl *)FindView("textcontrol");
			if (textcontrol) {
				BString string = textcontrol->Text();
				if (string.Length()) {
					message->AddString( "newpreset_name", string );
					message->what = 'AddP';
					fTarget->PostMessage( message );
				}
				else {
					textcontrol->MakeFocus();
					exit = false;
				}
			}
			if (exit) Quit();

		} break;

		default:
			BWindow::MessageReceived( message );
	}
}
