/*
 * Copyrights (c):
 *     2002 - 2008 , Werner Freytag.
 *     2009, Haiku
 * Distributed under the terms of the MIT License.
 *
 * Original Author:
 *              Werner Freytag <freytag@gmx.de>
 */

#include "InputDevice.h"

#ifdef DEBUGIT

#include <Beep.h>
#include <String.h>
#include "BeDC.h"
BeDC bedc;

#endif

#include <File.h>
#include <FindDirectory.h>
#include <InterfaceDefs.h>
#include <Path.h>
#include <stdlib.h>

#include "JoystickSettings.h"

#include "constants.h"

BInputServerDevice* instantiate_input_device() {

	return new InputDevice();
	
}

InputDevice::InputDevice()
	:	BInputServerDevice()
{

	input_device_ref	device			= { "Joystick Utilizer",
											B_KEYBOARD_DEVICE,
											NULL };
	input_device_ref	*deviceList[2]	= {&device, NULL};

	RegisterDevices( deviceList );

	fIsActive = true;
	
	fThread = B_ERROR;

}

InputDevice::~InputDevice()
{
	// cleanup
}

status_t InputDevice::InitCheck()
{
	// do any init code that could fail here
	// you will be unloaded if you return false

	return BInputServerDevice::InitCheck();
}

status_t InputDevice::SystemShuttingDown() {
	// do any cleanup (ie. saving a settings file) when the
	// system is about to shut down

	return BInputServerDevice::SystemShuttingDown();
}


status_t InputDevice::Start( const char *device, void *cookie)
{

	// start generating events
	// this is a hook function, it is called for you
	// (you should not call it yourself)

	fThread = spawn_thread(listener, device, B_LOW_PRIORITY, this);
	resume_thread(fThread);

	return B_NO_ERROR;
}

status_t InputDevice::Stop( const char *device, void *cookie) {

	// stop generating events
	// this is a hook function, it is called for you
	// (you should not call it yourself)

	status_t err = B_OK;

	wait_for_thread(fThread, &err);
	fThread = B_ERROR;

	return B_NO_ERROR;
}

status_t InputDevice::Control( const char	*device, void *cookie,
	uint32 code, BMessage *message) {

	return B_NO_ERROR;
}

// -- Test buttons and send message if neccessary --
// js: joystick settings for current joystick
// js_counter: current joystick is the n-th joystick (js_counter=n)
// nr: test the n-th button (nr=n)
// condition: true = button is down, false = button is up
void InputDevice::HandleKey( JoystickSettings *js, int js_counter, int nr, bool condition )
{

	Lock();
	
	static BList key_list;
	static uint8 states[16];
	
	while (js_counter>key_list.CountItems()-1)
		key_list.AddItem(new bool[20]);
	
	bool *keys = (bool*)(key_list.ItemAt(js_counter));
	
	if ( condition==keys[nr] ) { Unlock(); return; }
	
	keys[nr] = condition;
	
	BMessage *event = new BMessage( *js->Button(nr) );

	event->what = condition ? B_KEY_DOWN : B_KEY_UP;
	
	// states
	const uint8 *byte = 0;
	ssize_t size = 0;
	if (event->FindData("states", B_UINT8_TYPE, (const void **)&byte, &size)!=B_OK)
		{ Unlock(); return; }
	
	for (int i=0; i<min_c(16, size); ++i)
		if (condition) states[i] |= byte[i];
		else states[i] &= ~byte[i];
	
	event->RemoveName("states");

	event->AddData("states", B_UINT8_TYPE, (const void *)states, 16);
	
	event->RemoveName("when");
	event->AddInt64("when", real_time_clock() );
	
	EnqueueMessage(event);
	
	Unlock();
}

void InputDevice::LoadSetting( BMessage *prefsMessage )
{
	
	Lock();
	
	JoystickSettings *js;
	while ( ( js = (JoystickSettings *)fJoystickSettings.RemoveItem( 0L ) ) )
		delete js;

	// -- Presets
	BMessage settingMessage;
	if (prefsMessage->FindMessage("currentsetting", &settingMessage)==B_OK) {
	
		char *name;
		uint32 type;
		int32 count;
		
		for ( int i = 0; settingMessage.GetInfo(B_MESSAGE_TYPE, i, &name, &type, &count) == B_OK; i++ ) {
			
			js = new JoystickSettings( name );
			
			if (js->Open()==B_ERROR) {
				delete js;
				continue;
			}
			
			for (int j=0; j<20; ++j) {
				if (settingMessage.FindMessage( name, j, js->Button(j) )!=B_OK) break;
			}
			
			fJoystickSettings.AddItem( js );

		}

		prefsMessage->FindBool("active", &fIsActive );
		
		if (!fIsActive) CloseJoysticks();
	}
	
	Unlock();
	
}

void InputDevice::CloseJoysticks()
{
	// Es ist sehr wichtig, die Joysticks zu schließen, wenn Sie nicht benötigt werden,
	// da das BeOS abstürzt, wenn zwei Apps gleichzeitig einen Joystick öffnen!

	Lock();
	
	JoystickSettings *js;
	for (int i=0; ( js = (JoystickSettings *)fJoystickSettings.ItemAt( i ) ); ++i )
		js->Close();
	
	Unlock();
	
}

int32 InputDevice::listener( void *arg ) {
	
	// -- General veriables ----
	port_id			pid = create_port(20, kPortName);
	InputDevice		*device = (InputDevice *)arg;
	
	// -- Load preferences ----
	BPath path;
	find_directory(B_USER_SETTINGS_DIRECTORY, &path);
	path.SetTo( path.Path(), kPrefsFileName );
	
	BMessage	message;
	BFile		file( path.Path(), B_READ_ONLY );
	
	if (message.Unflatten( &file )==B_OK)
		device->LoadSetting( &message );
	
	// -- Message loop ----
	while (true) {
		
		ssize_t	size = 0;
		
		bool is_active = device->fIsActive;
		
		size = is_active ? port_buffer_size_etc(pid, B_TIMEOUT, 10000 ) : port_buffer_size(pid);
		
		if (size==B_TIMED_OUT) { // joystick abfragen
			
			JoystickSettings *js;
			for (int i=0; ( js = (JoystickSettings *)device->fJoystickSettings.ItemAt( i ) ); ++i ) {
				
				if ( js->Update()!=B_OK ) continue;

				device->HandleKey( js, i, 0, js->vertical > 2048 + 1024);
				device->HandleKey( js, i, 1, js->vertical < 2048 - 1024);
				device->HandleKey( js, i, 2, js->horizontal > 2048 + 1024);
				device->HandleKey( js, i, 3, js->horizontal < 2048 - 1024);
			
				uint32 buttonValues = js->ButtonValues();
				
				for (int i=0; i<js->CountButtons(); ++i)
					device->HandleKey( js, i, 4 + i, buttonValues & (1 << i) );
				
			}
		}
		else if (size>=0) { // buffer>=0 : we received a message!
			
			char *buf = 0;
			
			if (size>0) {
				buf = (char*)malloc(size);
				if (!buf) break;
			}
			
			int32	code;
			
			status_t result = read_port(pid, &code, (void *)buf, size);

#ifdef DEBUGIT
	beep();
#endif			
			if ( result < B_OK)
				break;
			
			switch (code) {
				
				case 'Stop':
					device->fIsActive = false;
					device->CloseJoysticks();
				break;
				
				case B_MESSAGE_TYPE: {
					
					BMessage message;
					message.Unflatten( buf );
					
					device->LoadSetting( &message );
					
				} break;
				
				default:
				;
			}
			
			free(buf);

		}
		else { // error => quit
			break;
		}
	}

	delete_port( pid );
	
	return B_OK;
}
