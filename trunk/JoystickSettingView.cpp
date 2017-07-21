/*
 * Copyrights (c):
 *     2002 - 2008 , Werner Freytag.
 *     2009, Haiku
 * Distributed under the terms of the MIT License.
 *
 * Original Author:
 *              Werner Freytag <freytag@gmx.de>
 */

#include "JoystickSettingView.h"

#include <Bitmap.h>
#include <Roster.h>
#include <TabView.h>
#include <Window.h>

#ifdef DEBUG
#include <iostream>
#include <Beep.h>
#endif

#include "get_resource_bitmap.h"
#include "Prefs.h"
#include "TextField.h"

JoystickSettingView::JoystickSettingView( BRect bounds, const char *name, uint8 joystick_nr )
:	BView( bounds, name, B_FOLLOW_ALL, B_WILL_DRAW | B_FRAME_EVENTS | B_NAVIGABLE_JUMP | B_PULSE_NEEDED ),
	fJoystickBmp( 0 ),
	fButtonBmp( 0 ),
	fButtonActiveBmp( 0 ),
	fJoystickNr( joystick_nr )
{
	fJoystickSettings = (JoystickSettings *)prefs.fCurrentSetting.fJoystickSettings.ItemAt(fJoystickNr);

	fNumButtons = fJoystickSettings->CountButtons();
	
	// add elements ----------
	
  	if (!prefs.fCurrentSetting.CountJoysticks()) return;

	// Icon
	fJoystickBmp		= get_resource_bitmap("joystick + arrows");
	fButtonBmp			= get_resource_bitmap("button");
	fButtonActiveBmp	= get_resource_bitmap("button_active");

	SetViewColor( 222, 222, 222 );
	SetLowColor( 222, 222, 222 );
	SetDrawingMode( B_OP_OVER );
	
	// TextFields
	BRect rect(0, 0, 58, 0);
	
	rect.OffsetTo( 66, 34 );
	fTextField[0] = new TextField( rect, new BMessage( 'TxtF' + 0 ), B_ALIGN_CENTER );
	AddChild(fTextField[0]);
	
	rect.OffsetBy( 0, 90 );
	fTextField[1] = new TextField( rect, new BMessage( 'TxtF' + 1 ), B_ALIGN_CENTER );
	AddChild(fTextField[1]);

	rect.OffsetBy( -66, -44 );
	fTextField[2] = new TextField( rect, new BMessage( 'TxtF' + 2 ), B_ALIGN_RIGHT );
	AddChild(fTextField[2]);

	rect.OffsetBy( 133, 0 );
	fTextField[3] = new TextField( rect, new BMessage( 'TxtF' + 3 ), B_ALIGN_LEFT );
	AddChild(fTextField[3]);
	
	BFont font;
	GetFont( &font );
	
	int buttons_per_col = fNumButtons > 8 ? fNumButtons >> 1 : fNumButtons;
	float height = (Frame().Height()-22) / buttons_per_col;

	rect = BRect(0, 0, fNumButtons>8 ? 47 : 60, 0);
	for (int i=0; i<fNumButtons; ++i) {
 		rect.OffsetTo( (fNumButtons>8 ? 192 : 210) + 19 + (i/buttons_per_col) * 67, 3 + height / 2 + (i % buttons_per_col) * height );
		fTextField[ 4 + i ] = new TextField( rect, new BMessage( 'TxtF' + 4 + i ) );
		AddChild(fTextField[ 4 + i ]);
	}
	
	UpdateTextFields();
}

JoystickSettingView::~JoystickSettingView()
{
	delete fJoystickBmp;
	delete fButtonBmp;
	delete fButtonActiveBmp;
}

void JoystickSettingView::UpdateTextFields()
{
	
	for (int l=0; l < fNumButtons + 4; ++l)
		fTextField[l]->SetKeyFromMessage( fJoystickSettings->Button(l) );

}

void JoystickSettingView::MouseDown( BPoint where )
{
	Window()->Activate();
	MakeFocus();
	
	BView::MouseDown( where );
}

void JoystickSettingView::Draw( BRect updateRect ) {
	
	BView::Draw( updateRect );
	
	DrawBitmap( fJoystickBmp, BPoint( 60, 52 ) );

	BFont font;
	GetFont( &font );
	
	int buttons_per_col = fNumButtons > 8 ? fNumButtons / 2 : fNumButtons;
	float height = (Frame().Height()-22) / buttons_per_col;

	// Wenn zwei Programme gleichzeitig auf den Joystick zugreifen stürzt das BeOS ab :-(
	uint32 buttonValues = 0;
	if (fJoystickSettings->IsOpen() && Window()->IsActive() && fJoystickSettings->Update()==B_OK)
		buttonValues = fJoystickSettings->ButtonValues();
		
	for (int i=0; i<fNumButtons; ++i)
		DrawBitmap( ((buttonValues>>i) & 1) ? fButtonActiveBmp : fButtonBmp, BPoint( (fNumButtons>8 ? 192 : 210) + (i/buttons_per_col) * 67, 3 + height / 2 + (i % buttons_per_col) * height ) );

}

void JoystickSettingView::Pulse()
{
	Draw( Bounds() );
}

