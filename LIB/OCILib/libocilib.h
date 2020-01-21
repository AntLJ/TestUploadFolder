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

#pragma once

#if _MSC_VER == 1700
	#ifdef _DEBUG
		#ifdef _UNICODE
			#pragma comment(lib, "OCILib_vc11-ugd.lib")
		#endif // ifdef _UNICODE
	#else
		#ifdef _UNICODE
			#pragma comment(lib, "OCILib_vc11-u.lib")
		#endif // ifdef _UNICODE
	#endif // ifdef _DEBUG
#else
	#ifdef _DEBUG
		#ifdef _UNICODE
			#pragma comment(lib, "OCILib-u.lib")
		#endif // ifdef _UNICODE
	#else
		#ifdef _UNICODE
			#pragma comment(lib, "sindyklib-ugd.lib")
		#endif // ifdef _UNICODE
	#endif // ifdef _DEBUG
#endif 


#include "oci.h"
#include "ocilibDate.h"
#include "ocilibConnection.h"
#include "ocilibColumn.h"
#include "ocilibRecordset.h"
#include "ocilibStatement.h"
#include "ocilibParameter.h"
