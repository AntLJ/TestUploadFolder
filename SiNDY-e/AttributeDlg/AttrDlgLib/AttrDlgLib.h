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

// AttrDlgLib用ヘッダ
// LQ系の場合は__SINDY_ROADQUEUEMODE__が定義されていること．
// AttributeDlg/AttrDlgLib/lib にパスが通っていること．

#include "SizingFramework/include/WinMgr.h"

#include "ControlDef.h"
#include "ControlRel.h"
#include "FieldDef.h"
#include "RelationalTableDef.h"
#include "RowDef.h"
#include "ExportDlg.h"
#include "ExportDlg2.h"

#ifdef __SINDY_ROADQUEUEMODE__
#include "LQAttrBaseDlg.h"
#include "LQAttrCommonBaseDlg.h"
	#if _MSC_VER == 1700 // VC11
		#ifdef	_DEBUG
			#pragma comment( lib, "liblqattrdlg_vc11-ugd.lib")	
		#else
			#pragma comment( lib, "liblqattrdlg_vc11-u.lib")	
		#endif	//_DEBUG
	#else
		#ifdef	_DEBUG
			#pragma comment( lib, "liblqattrdlg-ugd.lib")
		#else
			#pragma comment( lib, "liblqattrdlg-u.lib")
		#endif
	#endif

// LQ以外
#else
#include "AttrBaseDlg.h"
#include "RowsControlRel.h"
	#if _MSC_VER == 1700 // VC11
		#ifdef	_DEBUG
			#pragma comment( lib, "libattrdlg_vc11-ugd.lib")	
		#else
			#pragma comment( lib, "libattrdlg_vc11-u.lib")	
		#endif	//_DEBUG
	#else
		#ifdef	_DEBUG
			#pragma comment( lib, "libattrdlg-ugd.lib")
		#else
			#pragma comment( lib, "libattrdlg-u.lib")
		#endif
	#endif
#endif








