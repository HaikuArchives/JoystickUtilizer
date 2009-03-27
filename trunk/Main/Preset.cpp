/*
 * Copyrights (c):
 *     2002 - 2008 , Werner Freytag.
 *     2009, Haiku
 * Distributed under the terms of the MIT License.
 *
 * Original Author:
 *              Werner Freytag <freytag@gmx.de>
 */

#include "Preset.h"

#include <String.h>

#include "Prefs.h"

Preset::Preset( const char *name = 0 )
{
	if (name) fName = name;
}

Preset::~Preset()
{
	Clear();
}

void Preset::SetTo( Preset &from )
{
	LoadFromMessage(from.SaveToMessage());
}

void Preset::Clear()
{
	JoystickSettings *js;
	for (int i=0; (js = (JoystickSettings *)fJoystickSettings.RemoveItem(0L)); ++i)
		delete js;
}

void Preset::AddJoystick( JoystickSettings *js )
{
	fJoystickSettings.AddItem( js );
}

void Preset::LoadFromMessage( BMessage &settingMsg )
{
	
	char *name;
	uint32 type;
	int32 count;
	
	for ( int i = 0; settingMsg.GetInfo(B_MESSAGE_TYPE, i, &name, &type, &count) == B_OK; i++ ) {
		
		bool foundit = false;
		JoystickSettings *js = 0;
		for (int i=0; (js = (JoystickSettings*)fJoystickSettings.ItemAt(i) ); ++i) {
			if ( strcmp(js->Name(), name)==0 ) {
				foundit = true;
				break;
			}
		}

		if (!foundit) {
			
			// existiert dieser Joystick überhaupt?
			for (int i=0; (js = (JoystickSettings*)prefs.fCurrentSetting.fJoystickSettings.ItemAt(i) ); ++i) {
				if ( strcmp(js->Name(), name)==0 ) {
					foundit = true;
					break;
				}
			}

			if (!foundit) return;
			
			js = new JoystickSettings( name );
			fJoystickSettings.AddItem( js );
		}
		
		for (int j=0; j<20; ++j) {
			if (settingMsg.FindMessage( name, j, &js->fButtons[j] )!=B_OK)
				break;
		}

	}
}

BMessage &Preset::SaveToMessage()
{
	BMessage *settingMsg = new BMessage();
	
	JoystickSettings *js;
	for (int i=0; (js = (JoystickSettings*)fJoystickSettings.ItemAt(i) ); ++i) {

		for (int j=0; j < 20; ++j) {
			settingMsg->AddMessage( js->Name(), &js->fButtons[j] );
		}
			
	}

	return *settingMsg;
}

uint8 Preset::CountJoysticks()
{
	return fJoystickSettings.CountItems();
}

const char *Preset::NameOfJoystick( uint8 index )
{
	return ((JoystickSettings *)fJoystickSettings.ItemAt(index))->Name();
}

void Preset::SetJoystickButton( uint8 joystick_index, uint8 button_index, BMessage &msg )
{
	JoystickSettings *js = (JoystickSettings*)fJoystickSettings.ItemAt(joystick_index);
	if (!js) return;
	
	js->fButtons[button_index] = msg;
}
