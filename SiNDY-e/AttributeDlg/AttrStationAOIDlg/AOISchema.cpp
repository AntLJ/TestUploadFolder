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
#include "AOISchema.h"

namespace sindy {

namespace schema {

namespace sj{

// TODO: �wAOI�̐����p�������܂����Ȃ�A���L��sj.cpp��sindy::schma::sj���Ɉڍs����

/// �wAOI�|���S��
namespace station_aoi {
	const _TCHAR kTableName[]         = _T("STATION_AOI"); //!< �e�[�u����
}// namespace station_aoi

/// �w�v���b�g�t�H�[���|���S��
namespace station_platform {
	const _TCHAR kTableName[]         = _T("STATION_PLATFORM"); //!< �e�[�u����

	const _TCHAR kAOI_ID[]            = _T("AOI_ID");           //!< AOI�|���S��ID
	const _TCHAR kZLevel[]            = _T("ZLEVEL");           //!< Z���x��
}// namespace station_station_poi

namespace aoi_rel_station_poi {
	const _TCHAR kTableName[]    = _T("REL_STATION_POI");   //!< �e�[�u����

	const _TCHAR kAOI_ID[]       = _T("AOI_ID");            //!< AOI�|���S��ID
	const _TCHAR kJoinLineInfo[] = _T("JOIN_LINE_INFO_ID"); //!< ������H����OID
} // namespace aoi_rel_station_poi

// TODO:�ڍs����̂͂��̏�̍s�܂�

}

}

}
