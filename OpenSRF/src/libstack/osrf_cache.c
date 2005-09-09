/*
Copyright (C) 2005  Georgia Public Library Service 
Bill Erickson <highfalutin@gmail.com>

This program is free software; you can redistribute it and/or
modify it under the terms of the GNU General Public License
as published by the Free Software Foundation; either version 2
of the License, or (at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.
*/

#include "osrf_cache.h"

struct memcache* __osrfCache = NULL;
time_t __osrfCacheMaxSeconds = -1;

int osrfCacheInit( char* serverStrings[], int size, time_t maxCacheSeconds ) {
	if( !(serverStrings && size > 0) ) return -1;

	int i;
	__osrfCache = mc_new();
	__osrfCacheMaxSeconds = maxCacheSeconds;

	for( i = 0; i < size && serverStrings[i]; i++ ) 
		mc_server_add4( __osrfCache, serverStrings[i] );

	return 0;
}

int osrfCachePutObject( char* key, const jsonObject* obj, time_t seconds ) {
	if( !(key && obj) ) return -1;
	char* s = jsonObjectToJSON( obj );
	if( seconds < 0 ) seconds = __osrfCacheMaxSeconds;
	mc_add(__osrfCache, key, strlen(key), s, strlen(s), seconds, 0);
	free(s);
	return 0;
}

int osrfCachePutString( char* key, const char* value, time_t seconds ) {
	if( !(key && value) ) return -1;
	if( seconds < 0 ) seconds = __osrfCacheMaxSeconds;
	mc_add(__osrfCache, key, strlen(key), value, strlen(value), seconds, 0 );
	return 0;
}

jsonObject* osrfCacheGetObject( char* key ) {
	jsonObject* obj = NULL;
	if( key ) {
		char* data = (char*) mc_aget( __osrfCache, key, strlen(key) );
		if( data ) {
			obj = jsonParseString( data );
			return obj;
		}
	}
	return NULL;
}

char* osrfCacheGetString( char* key ) {
	if( key ) return (char*) mc_aget(__osrfCache, key, strlen(key) );
	return NULL;
}


int osrfCacheRemove( char* key ) {
	if( key ) return mc_delete(__osrfCache, key, strlen(key), 0 );
	return -1;
}


