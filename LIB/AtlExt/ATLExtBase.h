/*
	Copyright(c) DevInstinct inc. 2002
	http://www.devinstinct.com

	Written by Martin Lapierre

	All rights reserved. No warrantees extended. Use at your own risk.
	You may use this code in your product in compiled form without royalty.
	You may not distribute it in source code form as part of a class library
	or product without the express written consent of DevInstinct inc.
*/

#pragma once

#ifndef __ATLCOM_H__
	#error The ATL Extension requires atlcom.h to be included first.
#endif

////////////////////////
// Master version number
////////////////////////
#define _ATLEXT_VER 0x0100 // ATL Extention version 1.0

///////////////////
// HRESULT checking
///////////////////
#define TestHR(hr) {HRESULT _hr(hr); if (FAILED(_hr) || _hr == S_FALSE) return _hr == S_FALSE ? E_FAIL : _hr;}
