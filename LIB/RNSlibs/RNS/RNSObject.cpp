/*
 * Copyright (C) INCREMENT P CORP. All Rights Reserved.
 *
 * THIS SOFTWARE IS PROVIDED BY INCREMENT P CORP., WITHOUT WARRANTY OF
 * ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE
 * WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE
 * AND NONINFRINGEMENT.
 *
 * IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDER BE LIABLE FOR ANY
 * CLAIM, DAMAGES SUFFERED BY LICENSEE AS A RESULT OF USING, MODIFYING
 * OR DISTRIBUTING THIS SOFTWARE OR ITS DERIVATIVES.
*/

#include "RNS.h"

#include <set>


#if defined(_MT) && defined(_WIN32) // ----- マルチスレッド版 ----- //

#include <windows.h>

static const RNSObject* sObjects[500];
static int sCount = 0;

class RNSCriticalSection {
public:
	RNSCriticalSection()
	{
		InitializeCriticalSection(&mCs);
	}

	~RNSCriticalSection()
	{
		DeleteCriticalSection(&mCs);
	}

	operator CRITICAL_SECTION* ()
	{
		return &mCs;
	}

private:
	CRITICAL_SECTION mCs;
};

static RNSCriticalSection sCs;

bool __is_newed_rns_object__(const RNSObject* cObject) 
{
	bool result = false;

	if(sCount) {
		EnterCriticalSection(sCs);
		for(int i = 0; i < sCount; ++i) {
			if(sObjects[i] == cObject) {
				result = true;
				--sCount;
				for(int j = i; j < (sCount); ++j) {
					sObjects[j] = sObjects[j + 1];
				}
				break;
			}
		}
		LeaveCriticalSection(sCs);
	}

	return result;
}

void __newed_rns_object__(const RNSObject* cObject)
{
	EnterCriticalSection(sCs);
	sObjects[sCount++] = cObject;
	LeaveCriticalSection(sCs);
}

#else // ----- シングルスレッド版 ----- //

static const RNSObject* sObject = 0;

bool __is_newed_rns_object__(const RNSObject* cObject)
{
	return sObject == cObject;
}

void __newed_rns_object__(const RNSObject* cObject)
{
	sObject = cObject;
}

#endif // _MT
