/*
 * Copyrights (c):
 *     2002 - 2008 , Werner Freytag.
 *     2009, Haiku
 * Distributed under the terms of the MIT License.
 *
 * Original Author:
 *              Werner Freytag <freytag@gmx.de>
 */

#include <Alert.h>
#include <Control.h>
#include <Entry.h>
#include <FindDirectory.h>
#include <Message.h>
#include <Messenger.h>
#include <Path.h>
#include <Window.h>
#include <Roster.h>

#ifdef DEBUG
#include <iostream>
#endif

#include "App.h"
#include "constants.h"
#include "Prefs.h"
#include "SettingsWindow.h"

App::App()
	:	BApplication( kAppSignature ),
		fSettingsWindow( 0 )
{

	fSettingsWindow = new SettingsWindow();

}

App::~App()
{
	prefs.SendToPort();
}

void App::MessageReceived(BMessage *message) {

	switch( message->what ) {
	
		// --- Allgemein -----------------------------------------------

		case B_SILENT_RELAUNCH: {
			fSettingsWindow->Lock();
			fSettingsWindow->SetWorkspaces( B_CURRENT_WORKSPACE );
			if (fSettingsWindow->IsHidden()) fSettingsWindow->Show();
			fSettingsWindow->Activate();
			fSettingsWindow->Unlock();
		} break;
		
		// --- Settings ------------------------------------------

		case 'Acti': {
			int32	value = 0;
			message->FindInt32( "be:value", &value );
			prefs.fActive = value;
		} break;

		case B_SOME_APP_ACTIVATED: {
			
		 	// Die Message wird gesendet, wenn das kein Programmfenster aktiviert ist

			bool active = message->FindBool("active");
			prefs.SendToPort(!active);
		
		} break;
		
		// --- Rest -----------------------------------------------

		default:
			BApplication::MessageReceived( message );
	}
}

void App::AboutRequested() {

 	// Die Message wird gesendet, wenn das kein Programmfenster aktiviert ist
	prefs.SendToPort(true);

	(new BAlert("About JoystickUtilizer", 
		"JoystickUtilizer 2.0.1\n\n"
		"Copyright ©2002 by Werner Freytag\n"
		"\n"
		"If you like this tool, please vote for it at\n"
		"- BeZip.de (http://bezip.de/app/1198/) and\n"
		"- BeBits (http://www.bebits.com/app/2807)\n"
		"\n"
		"Please have also a look at my other programs!\n"
		"\n"
		"e-mail:	werner@pecora.de\n"
		"homepage:	http://www.pecora.de",
		"Ok, thank you!"))->Go();

 	// Die Message wird gesendet, wenn das kein Programmfenster aktiviert ist
	prefs.SendToPort(false);

}

int main() {

	(new App)->Run();
	delete be_app;

	return 0;

}
