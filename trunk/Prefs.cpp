/*
 * Copyrights (c):
 *     2002 - 2008 , Werner Freytag.
 *     2009, Haiku
 * Distributed under the terms of the MIT License.
 *
 * Original Author:
 *              Werner Freytag <freytag@gmx.de>
 */

#include "Prefs.h"

#include <Directory.h>
#include <Entry.h>
#include <File.h>
#include <FindDirectory.h>
#include <OS.h>
#include <Path.h>

#ifdef DEBUG
#include <iostream>
#include <Beep.h>
#endif

#include "App.h"
#include "constants.h"

#include <stdlib.h>

Prefs prefs;

Prefs::Prefs()
	:	fActive( true ),
		fSelectedPreset( -1 )
{

	SendToPort(false);
	
	// Pfade initialisieren
	find_directory(B_USER_SETTINGS_DIRECTORY, &fSavePath);
	fSavePath.SetTo( fSavePath.Path(), kPrefsFileName );
	
	// Joystickdaten einlesen
	BJoystick	joystick;
	char name[B_OS_NAME_LENGTH];
	
	if (joystick.CountDevices()) {
	
		for (int i=0; i<joystick.CountDevices(); ++i) {
			
			joystick.GetDeviceName( i, name );
			JoystickSettings *js = new JoystickSettings(name);
			if (js->Open()!=B_ERROR) {
				fCurrentSetting.AddJoystick(js);
				js->Close();
			}
			else
				delete js;
			
		}
	}
	
	LoadFromFile();
	
}

Prefs::~Prefs() {
	
	SaveSettingsToPreset();
	
	SaveToFile();

	Preset *preset;
	while ( (preset = (Preset *)fPresets.RemoveItem((int32)0)) )
		delete preset;

}

// A preset was selected and is loaded into memory
void Prefs::LoadSettingsFromPreset()
{

	Lock();
		
	Preset *preset;
	if ( (preset = (Preset *)fPresets.ItemAt( fSelectedPreset ) ) )
		fCurrentSetting.SetTo( *preset );
	
	Unlock();
	
}

void Prefs::SaveSettingsToPreset()
{

	Lock();

	Preset *preset;
	if ( (preset = (Preset *)fPresets.ItemAt( fSelectedPreset ) ) )
		preset->SetTo(fCurrentSetting);
	
	Unlock();

}

void Prefs::LoadFromFile()
{
	BMessage	prefsMessage;
	BFile		file( fSavePath.Path(), B_READ_ONLY );
	
	if (file.InitCheck()!=B_OK || prefsMessage.Unflatten( &file )!=B_OK)
		return;

	// -- Presets
	BMessage msg;
	for (int i=0; prefsMessage.FindMessage("preset", i, &msg )==B_OK; ++i) {

		BString name;
		if (msg.FindString("name", &name)!=B_OK) continue;
		
		Preset *preset = new Preset( name.String() );
		preset->LoadFromMessage( msg );
		
		fPresets.AddItem( preset );
	}
	
	prefsMessage.FindBool("active", &fActive );

	prefsMessage.FindInt32("selectedpreset", &fSelectedPreset );
	if (fSelectedPreset>fPresets.CountItems())
		fSelectedPreset = -1;
	
	if (prefsMessage.FindMessage("currentsetting", &msg )==B_OK)
		fCurrentSetting.LoadFromMessage( msg );
}

void Prefs::SaveToFile()
{
	Lock();
	
	BMessage	prefsMessage;
	
	prefsMessage.AddBool("active", fActive );
	prefsMessage.AddInt32("selectedpreset", fSelectedPreset );
	
	Preset	*preset;
	for (int i=0; (preset = (Preset *)fPresets.ItemAt(i)); ++i) {
		BMessage msg = preset->SaveToMessage();
		msg.AddString("name", preset->fName);
		prefsMessage.AddMessage("preset", &msg );
	}
	
	prefsMessage.AddMessage("currentsetting", &fCurrentSetting.SaveToMessage() );
	
	BFile		file( fSavePath.Path(), B_WRITE_ONLY|B_CREATE_FILE );
	
	file.SetSize( 0 );
	prefsMessage.Flatten( &file );
	
	Unlock();
	
}

void Prefs::SendToPort(bool active)
{

	Lock();
	
	port_id		pid = find_port(kPortName);
	if (pid!=B_NAME_NOT_FOUND) {
	
		if (active && fActive) {
			
			JoystickSettings *js;
			for (int i=0; ( js = (JoystickSettings *)fCurrentSetting.fJoystickSettings.ItemAt(i) ); ++i)
				js->Close();

			BMessage	message;
			
			message.AddBool("active", fActive );
		
			BMessage settingMsg = fCurrentSetting.SaveToMessage();
			message.AddMessage("currentsetting", &settingMsg );
			
			ssize_t	size = message.FlattenedSize();
			
			char *buffer = (char*)malloc(size);
			if (buffer) {
			
				if ( message.Flatten(buffer, size)==B_OK )
					write_port( pid, B_MESSAGE_TYPE, buffer, size );
			
				free(buffer);
			
			}
			
		}
		else {

			write_port( pid, 'Stop', 0, 0 );
			
			// Dies ist sehr wichtig, da das BeOS abstürzt, wenn zwei Apps gleichzeitig
			// einen Joystick öffnen!
			
			snooze(100000);
			JoystickSettings *js;
			for (int i=0; ( js = (JoystickSettings *)fCurrentSetting.fJoystickSettings.ItemAt(i) ); ++i)
				js->Open();
		
		}
	}
	
	Unlock();
	
}
