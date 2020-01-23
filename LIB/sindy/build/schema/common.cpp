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

#include "stdafx.h"
#include <sindy/schema/common.h>

namespace sindy {

namespace schema {

namespace object {
	const _TCHAR kObjectID[] = _T("OBJECTID");
}

namespace feature {
	const _TCHAR kShape[] = _T("SHAPE");
}

namespace ipc_table {
	const _TCHAR kOperator[] = _T("OPERATOR");
	const _TCHAR kPurpose[] = _T("PURPOSE_C");
	const _TCHAR kModifyDate[] = _T("MODIFYDATE");
	const _TCHAR kUpdateType[] = _T("UPDATETYPE_C");
	const _TCHAR kProgModifyDate[] = _T("PROGMODIFYDATE");
	const _TCHAR kModifyProgName[] = _T("MODIFYPROGNAME");
	const _TCHAR kUserClaim[] = _T("USERCLAIM_F");
	const _TCHAR kSource[] = _T("SOURCE");
}

namespace tmp_info {
	const _TCHAR tmp_info::kTmpMeshCode[] = _T("TMP_MESHCODE");
	const _TCHAR tmp_info::kTmpPrimID[] = _T("TMP_PRIMID");
}

namespace fieldsurvey_table {
	const _TCHAR kFieldSurvey[] = _T("FIELDSURVEY_F");
	const _TCHAR kWorkYear[] = _T("WORK_YEAR");
}

namespace link_queue {
	const _TCHAR kInfID[] = _T("INF_ID");
	const _TCHAR kSequence[] = _T("SEQUENCE");
	const _TCHAR kLastLink[] = _T("LASTLINK_F");
}

/* ============================================================================== */
/// ADAM v2用
namespace adam_v2_global_ipc_feature {
	const _TCHAR kGlobalID[] = _T("GLOBALID");		///< GLOBALID
}

namespace adam_v2_adam_ipc_feature {
	const _TCHAR kSperculationC[]	= _T("SPECULATION_C");		///< 推測コード
	const _TCHAR kSourceID[]		= _T("SOURCEID");			///< 提供元ID
	const _TCHAR kSourceLot[]       = _T("SOURCE_LOT");	    	///< 提供元ロット
	const _TCHAR kSourceUpdateC[]   = _T( "SOURCE_UPDATE_C" );	///< 提供元更新コード
}

namespace adam_v2_table {
	const _TCHAR kSourceID[]		= _T("SOURCEID");			///< 提供元ID
	const _TCHAR kSourceLot[]       = _T( "SOURCE_LOT" );	   	///< 提供元ロット
	const _TCHAR kSourceUpdateC[]   = _T( "SOURCE_UPDATE_C" );	///< 提供元更新コード
}

namespace adam_v2_global_table {
	const _TCHAR kGlobalID[] = _T("GLOBALID");		///< GLOBALID
}

namespace adam_v2_node_queue {
	const _TCHAR kSequence[]  = _T("SEQUENCE");		///< 順番
	const _TCHAR kLastNodeF[] = _T("LASTNODE_F");		///< 終端ノードフラグ
}
/* ============================================================================== */

} // namespace schema

} // namespace sindy
