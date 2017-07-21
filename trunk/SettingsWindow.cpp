/*
 * Copyrights (c):
 *     2002 - 2008 , Werner Freytag.
 *     2009, Haiku
 * Distributed under the terms of the MIT License.
 *
 * Original Author:
 *              Werner Freytag <freytag@gmx.de>
 */

#include "SettingsWindow.h"

#include <Control.h>
#include <MenuBar.h>
#include <Menu.h>
#include <MenuField.h>
#include <MenuItem.h>
#include <Message.h>
#include <Messenger.h>
#include <Screen.h>
#include <TabView.h>
#include <View.h>

#ifdef DEBUG
#include <iostream>
#endif

#include "App.h"
#include "Preset.h"
#include "NewPresetWindow.h"
#include "MainView.h"
#include "Prefs.h"
#include "SettingsView.h"

SettingsWindow::SettingsWindow()
:	BWindow( BRect( 0, 0, 346, 280 ), "JoystickUtilizer Preferences", B_TITLED_WINDOW,
		B_ASYNCHRONOUS_CONTROLS|B_NOT_RESIZABLE|B_NOT_ZOOMABLE|B_QUIT_ON_WINDOW_CLOSE )
{
	BScreen		screen;
	
	MoveTo( (screen.Frame().Width() - Frame().Width()) / 2,  (screen.Frame().Height() - Frame().Height()) / 2  );

	// Menü
	BMenuBar	*menuBar;
	BMenu		*menu;
	BMenuItem	*menuItem;
	
	menuBar	= new BMenuBar( BRect(0, 0, 10000, 10), "menubar" );

	// --- File ---
	menu	= new BMenu( "File" );

	menuItem = new BMenuItem( "Quit", new BMessage( B_QUIT_REQUESTED ), 'Q' );
	menu->AddItem( menuItem );
	
	menu->SetTargetForItems(be_app);

	menuBar->AddItem( menu );
	
	// --- Help ---
	menu	= new BMenu( "?" );
	menuItem = new BMenuItem( "About...", new BMessage( B_ABOUT_REQUESTED ), '?' );
	menu->AddItem( menuItem );
	
	menu->SetTargetForItems(be_app);

	menuBar->AddItem( menu );

	AddChild( menuBar );
	
	// mainview
	BRect	rect(  Bounds() );
	rect.top += menuBar->Bounds().Height() + 1;
	
	MainView	*mainview = new MainView( rect );
	AddChild( mainview );

	SetPulseRate(100000);
	
	Show();
}

SettingsWindow::~SettingsWindow()
{
	WindowActivated(false);
}

void SettingsWindow::MessageReceived(BMessage *message) {

//	message->PrintToStream();
	
	if ( message->what >= 'TxtF' && message->what < 'TxtF' + 20 ) {	// Einer der Keys wird geändert
		
		prefs.fCurrentSetting.SetJoystickButton(
			((BTabView *)FindView("tabview"))->Selection(),
			message->what - 'TxtF',
			*message );
			
		return;		
	}	

	switch (message->what) {

		case 'AddS': {
			NewPresetWindow	*win = new NewPresetWindow( this );
			win->Show();
		} break;
		
		case 'DelS': {
			if (prefs.fSelectedPreset == -1) break;

			Preset *preset = (Preset *)prefs.fPresets.RemoveItem( prefs.fSelectedPreset );
			delete preset;

			prefs.fSelectedPreset = -1;
			
			((SettingsView *)FindView("settingsview"))->UpdateSettings( false );
			((BMenuField *)FindView("menufield"))->Menu()->ItemAt(0)->SetMarked( false );
		} break;
		
		// Add new preset (selected ok in "new preset" window)
		case 'AddP': {
			
			// save current settings first
			prefs.SaveSettingsToPreset();
			
			// fetch name from message
			const char *name;
			if ( message->FindString("newpreset_name", &name ) != B_OK ) break;
			
			// create new preset
			Preset *preset = new Preset(name);
			preset->SetTo( prefs.fCurrentSetting );
			
			prefs.fPresets.AddItem( preset );
			prefs.fSelectedPreset = prefs.fPresets.CountItems() - 1;

			((SettingsView *)FindView("settingsview"))->UpdateSettings( false );
			
		} break;
		
		// cancel "new preset" window
		case 'CncP': {

			BMenuField *menufield = (BMenuField *)FindView("menufield");
			if (prefs.fSelectedPreset>-1) {
				menufield->Menu()->ItemAt(prefs.fSelectedPreset)->SetMarked( true );
			}
			else {
				// this is necessary to "unmark" the "New..." menu item
				menufield->Menu()->ItemAt(0)->SetMarked( true );
				menufield->Menu()->ItemAt(0)->SetMarked( false );
			}

		} break;
		
		// A preset was selected in the menufield
		case 'Pres': {
			
			prefs.SaveSettingsToPreset();
			
			prefs.fSelectedPreset = message->FindInt32("index");
			
			prefs.LoadSettingsFromPreset();
			
			((SettingsView *)FindView("settingsview"))->UpdateSettings( true );

		} break;
		
		default:
			BWindow::MessageReceived( message );
	}
	
}
