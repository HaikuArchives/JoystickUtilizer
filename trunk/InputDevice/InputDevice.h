/*
 * Copyrights (c):
 *     2002 - 2008 , Werner Freytag.
 *     2009, Haiku
 * Distributed under the terms of the MIT License.
 *
 * Original Author:
 *              Werner Freytag <freytag@gmx.de>
 */

#ifndef _INPUT_DEVICE_H
#define _INPUT_DEVICE_H

#include <InputServerDevice.h>
#include <Joystick.h>
#include <List.h>
#include <OS.h>
#include <SupportDefs.h>
#include <Locker.h>

// export this for the input_server
extern "C" _EXPORT BInputServerDevice* instantiate_input_device();

class JoystickSettings; 

class InputDevice : public BInputServerDevice, public BLocker {
public:
							InputDevice();
	virtual					~InputDevice();
	
	virtual status_t		InitCheck();
	virtual status_t		SystemShuttingDown();
	
	virtual status_t		Start(const char *device, void *cookie);
	virtual	status_t		Stop(const char *device, void *cookie);

	virtual status_t		Control(const char	*device,
									void		*cookie,
									uint32		code, 
									BMessage	*message);
	
private:

	void					LoadSetting( BMessage *message );
	void					CloseJoysticks();
	
	void					HandleKey( JoystickSettings *js, int js_counter, int nr, bool condition );
	
	bool					fIsActive;
	BList					fJoystickSettings;
	
	thread_id				fThread;
	
	static int32			listener(void *arg);
};


#endif
