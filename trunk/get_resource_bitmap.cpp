/*
 * Copyrights (c):
 *     2002 - 2008 , Werner Freytag.
 *     2009, Haiku
 * Distributed under the terms of the MIT License.
 *
 * Original Author:
 *              Werner Freytag <freytag@gmx.de>
 */

#include "get_resource_bitmap.h"

#include <Bitmap.h>
#include <Entry.h>
#include <File.h>
#include <Message.h>
#include <Resources.h>
#include <Roster.h>

#ifdef DEBUG
#include <iostream>
#endif

#include "constants.h"

BBitmap *get_resource_bitmap(const char *name) {

	entry_ref	ref;
	if (be_roster->FindApp(kAppSignature, &ref)!=B_OK) return NULL;

	BFile		file( &ref, B_READ_ONLY );
	BResources	resources( &file );

	size_t		groesse;
	BMessage	msg;
	const char	*buf = (const char *)resources.LoadResource('BBMP', name, &groesse);
	
	if (buf) {
		msg.Unflatten(buf);
		return new BBitmap( &msg );
	}

#ifdef DEBUG
	cerr << "ERROR: Resource not found: " << name << endl;
#endif

	return NULL;
}
