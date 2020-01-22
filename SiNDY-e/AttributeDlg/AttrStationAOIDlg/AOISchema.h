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

#include <tchar.h>
#include <sindy/schema/common.h>

namespace sindy {

namespace schema {

namespace sj{

// TODO: �wAOI�̐����p�������܂����Ȃ�A���L��sj.h��sindy::schma::sj���Ɉڍs����

/// �wAOI�|���S��
namespace station_aoi {
	using namespace ::sindy::schema::ipcfeature;

	extern const _TCHAR kTableName[]; //!< �e�[�u����
}

/// �w�v���b�g�t�H�[���|���S��
namespace station_platform {
	using namespace ::sindy::schema::ipcfeature;

	extern const _TCHAR kTableName[]; //!< �e�[�u����
	extern const _TCHAR kAOI_ID[];    //!< AOI�|���S��ID
	extern const _TCHAR kZLevel[];    //!< Z���x��
}

/// �wPOI�R�t���e�[�u����
namespace aoi_rel_station_poi {
	extern const _TCHAR kTableName[];    //!< �e�[�u����
	extern const _TCHAR kAOI_ID[];       //!< AOI�|���S��ID
	extern const _TCHAR kJoinLineInfo[]; //!< ������H����OID
} // namespace aoi_rel_station_poi

/// �wPOI�R�t���e�[�u��
namespace rel_station_poi {
	using namespace ::sindy::schema::ipc_table;
	using namespace ::sindy::schema::sj::aoi_rel_station_poi;
} // namespace rel_station_poi

// TODO:�ڍs����̂͂��̏�̍s�܂�

} // namespace sj

} // namespace schema

} // namespace sindy
