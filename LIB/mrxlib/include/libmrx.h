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
		#pragma comment( lib, "mrxlib_vc11-gd.lib")
	#else // _DEBUG
		#pragma comment( lib, "mrxlib_vc11.lib")
	#endif
#elif _MSC_VER == 1500
	#ifdef _DEBUG
		#pragma comment( lib, "mrxlib_vc90-gd.lib")
	#else // _DEBUG
		#pragma comment( lib, "mrxlib_vc90.lib")
	#endif
#elif _MSC_VER == 1400
	#ifdef _DEBUG
		#pragma comment( lib, "mrxlib_vc80-gd.lib")
	#else // _DEBUG
		#pragma comment( lib, "mrxlib_vc80.lib")
	#endif
#endif // if _MSC_VER == 1400
