/*
 * Copyrights (c):
 *     2002 - 2008 , Werner Freytag.
 *     2009, Haiku
 * Distributed under the terms of the MIT License.
 *
 * Original Author:
 *              Werner Freytag <freytag@gmx.de>
 */

#include "MainView.h"

#include <Application.h>
#include <CheckBox.h>

#include "Prefs.h"
#include "SettingsView.h"

MainView::MainView( BRect bounds )
:	BView( bounds, NULL, B_FOLLOW_ALL, B_WILL_DRAW )
{
}

void MainView::AttachedToWindow() {

	SetViewColor( 222, 222, 222 );	
	SetLowColor( 222, 222, 222 );	
	
	// settings view
	AddChild( new SettingsView( BRect( 5, 5, Bounds().right - 5, Bounds().bottom - 28 ) ) );

	// check box
	BFont	font(be_plain_font);
	
	BRect rect = BRect( 0, 0, font.StringWidth("Enabled") + 24, 12 ).OffsetByCopy( 10,  Bounds().bottom - 24 );
	BCheckBox	*checkbox = new BCheckBox( rect, "active", "Enabled", new BMessage('Acti') );
	checkbox->SetValue( prefs.fActive );
	AddChild( checkbox );
	checkbox->SetTarget(be_app);
	
}

void MainView::MouseDown( BPoint where )
{
	MakeFocus();
	BView::MouseDown( where );
}

void MainView::Draw( BRect updateRect ) {

	SetHighColor( 255, 255, 255 );
	StrokeLine( Bounds().RightTop(), Bounds().LeftTop() );
	StrokeLine( Bounds().LeftBottom() );
	
	SetHighColor( 180, 180, 180 );
	StrokeLine( Bounds().RightBottom() );
	StrokeLine( Bounds().RightTop() );
}
