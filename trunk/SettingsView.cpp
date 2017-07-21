/*
 * Copyrights (c):
 *     2002 - 2008 , Werner Freytag.
 *     2009, Haiku
 * Distributed under the terms of the MIT License.
 *
 * Original Author:
 *              Werner Freytag <freytag@gmx.de>
 */

#include "SettingsView.h"

#include <Box.h>
#include <MenuField.h>
#include <MenuItem.h>
#include <TabView.h>
#include <Window.h>
#include <PopUpMenu.h>

#include <vector>

#include "constants.h"
#include "JoystickSettingView.h"

#ifdef DEBUG
#include <iostream>
#endif

#include "Prefs.h"

SettingsView::SettingsView( BRect bounds )
:	BBox( bounds, "settingsview", B_FOLLOW_ALL )
{
}

SettingsView::~SettingsView()
{
}

void SettingsView::AttachedToWindow() {
	
	if (!prefs.fCurrentSetting.CountJoysticks()) return;
	
	BTab *tab;
	
	BRect r( Bounds() );
	r.InsetBy(5,5);
	r.top += 22;
	
	fTabView = new BTabView(r, "tabview");
	
	fTabView->SetViewColor( 222, 222, 222 );
	fTabView->SetLowColor( 222, 222, 222 );
	
	r.bottom -= fTabView->TabHeight();
	r.OffsetTo(0, 0);
	
	float tab_text_width = (20 + Bounds().Width()) / prefs.fCurrentSetting.CountJoysticks() - 20;
	
	for (int i=0; i<prefs.fCurrentSetting.CountJoysticks(); ++i) {
		
		BString title;
		title.SetTo( prefs.fCurrentSetting.NameOfJoystick(i) );

		tab = new BTab();
		
		JoystickSettingView *view = new JoystickSettingView(r, title.String(), i);
		fJoystickSettingView.AddItem(view);
		
		fTabView->AddTab(view, tab);

		TruncateString( &title, B_TRUNCATE_MIDDLE, tab_text_width );

		tab->SetLabel(title.String());
		
	}
	
	AddChild( fTabView );
	
	fTabView->SetTabWidth( B_WIDTH_FROM_WIDEST );

	UpdateSettings();

}

void SettingsView::UpdateSettings( bool updateKeys )
{

	// Update label
	BMenuField		*menuField	= (BMenuField *)FindView("menufield");
	
	BPopUpMenu		*menu = new BPopUpMenu( "Please select" );

	BMenuItem		*menuItem = 0;
	
	if ( prefs.fPresets.IsEmpty() ) {
		menuItem = new BMenuItem( "No presets saved", 0 );
		menuItem->SetEnabled( false );
		menu->AddItem( menuItem );
	}
	else {
		Preset *preset;
		for (int i=0; (preset = (Preset *)prefs.fPresets.ItemAt( i )); ++i) {
			menuItem = new BMenuItem( preset->fName.String(), new BMessage('Pres') );
			menu->AddItem( menuItem );
			if (i==prefs.fSelectedPreset) menuItem->SetMarked( true );
		}
	}
	
	menu->AddItem( new BSeparatorItem() );

	menuItem = new BMenuItem( "New...", new BMessage('AddS') );
	menu->AddItem( menuItem );

	menuItem = new BMenuItem( "Delete Current", new BMessage('DelS') );
	if ( prefs.fSelectedPreset==-1) menuItem->SetEnabled( false );
	menu->AddItem( menuItem );
	
	BRect	rect( 0, 0, Bounds().Width() - 10, 0 );

	menuField = new BMenuField( rect, "menufield", "Preset:", menu );
	menuField->SetDivider( be_bold_font->StringWidth( "Preset:") + 8 );
	menuField->SetFont( be_bold_font );

	SetLabel( menuField );

	// Update TextFields
	if (!updateKeys) return;
	
	JoystickSettingView *view;
	for (int i=0; ( view = (JoystickSettingView *)fJoystickSettingView.ItemAt(i)); ++i )
		view->UpdateTextFields();

}

void SettingsView::MouseDown( BPoint where )
{
	Window()->Activate();
	MakeFocus();
	
	BView::MouseDown( where );
}

void SettingsView::Draw( BRect updateRect )
{
	
	BBox::Draw(updateRect);
	if (!prefs.fCurrentSetting.CountJoysticks())
		DrawString( kNoJoystick, BPoint( (Frame().Width() - StringWidth(kNoJoystick))/2, Frame().Height()/2 ) );

}

