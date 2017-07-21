/*
 * Copyrights (c):
 *     2002 - 2008 , Werner Freytag.
 *     2009, Haiku
 * Distributed under the terms of the MIT License.
 *
 * Original Author:
 *              Werner Freytag <freytag@gmx.de>
 */

#ifndef _PREFS_H
#define _PREFS_H

#include <List.h>
#include <Locker.h>
#include <Message.h>
#include <Path.h>
#include <String.h>

#include <string>

#include "JoystickSettings.h"
#include "Preset.h"

class Prefs : BLocker {

public:
				Prefs();
				~Prefs();

	void		LoadSettingsFromPreset();
	void		SaveSettingsToPreset();
	
	void		LoadFromFile();
	void		SaveToFile();
				
	void		SendToPort(bool active=true);
				
	bool		fActive;
	BList		fPresets;
	int32		fSelectedPreset;

	Preset		fCurrentSetting;
	
private:
	
	BPath		fSavePath;

	void		LoadPresetFromMessage( BList &preset, BMessage &settingMsg );
	BMessage	&SavePresetToMessage( BList &preset );

	void		ClearPreset( BList &preset );
	
};

extern Prefs prefs;
	
#endif
