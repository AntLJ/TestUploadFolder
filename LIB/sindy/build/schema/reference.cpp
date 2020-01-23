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
#include <sindy/schema/reference.h>

namespace sindy {

namespace schema {

namespace citymesh {
	const TCHAR kTableName[]			= _T("CITYMESH");				//!< テーブル名

	const TCHAR kCityMeshCode[]			= _T("MESHCODE");				//!< メッシュコード(ライブラリ使用している他ツールで整合取るため、残しておく 2007/10/16 s_suzuki)
	const TCHAR kCityName[]				= _T("CITYNAME");				//!< 市区町村名
	const TCHAR kCreateYear[]			= _T("CREATE_YEAR");			//!< 作成年度
	const TCHAR kUpdateYear[]			= _T("UPDATE_YEAR");			//!< 作業年度
	const TCHAR kComplete[]				= _T("COMPLETE_F");				//!< 完全化フラグ
	const TCHAR kSea[]					= _T("SEA_F");					//!< 海フラグ
	const TCHAR kZuka[]					= _T("ZUKA_F");					//!< 図化フラグ
	const TCHAR kRaster[]				= _T("RASTER");					//!< ラスター
	const TCHAR kCreateRasterYear[]		= _T("CREATE_RASTER_YEAR");		//!< ラスター作成年度
	const TCHAR kCurrentAdjYear[]		= _T("CURRENT_ADJ_YEAR");		//!< 現況修正年度
	const TCHAR kPhotoShotDate[]		= _T("PHOTO_SHOT_DATE");		//!< 使用写真撮影年月日
	const TCHAR kFieldResearchYear[]	= _T("FIELD_RESEARCH_YEAR");	//!< 現地調査実施年度（地図）
	const TCHAR kTrSurveyYear[]			= _T("TR_SURVEY_YEAR");			//!< 現地調査実施年度（規制）
	const TCHAR kShaBgworkDate[]		= _T("SHA_BGWORK_DATE");		//!< 上海作業年月（平面）
	const TCHAR kShaAnnoworkDate[]		= _T("SHA_ANNOWORK_DATE");		//!< 上海作業年月（注記）
	const TCHAR kCreateAnnoMsDate[]		= _T("CREATE_ANNO_MS_DATE");	//!< 注記原稿作成年月
	const TCHAR kAddrMainteYear[]		= _T("ADDR_MAINTE_YEAR");		//!< 住所整備見直し年度
	const TCHAR kJmapPublicationYear[]	= _T("JMAP_PUBLICATION_YEAR");	//!< 参照j地図発行年度
	const TCHAR kMapcubeWorkYear[]		= _T("MAPCUBE_WORK_YEAR");		//!< mapcube作業年度
	const TCHAR kPdmUseYear[]			= _T("PDM_USE_YEAR");			//!< pdm使用年度
}

const _TCHAR basemesh::kTableName[] = _T("BASEMESH");

namespace middlemesh {
	const TCHAR kTableName[]			= _T("MIDDLEMESH");				//!< テーブル名
	const TCHAR kMiddleMeshCode[]		= _T("MESHCODE");				//!< メッシュコード(ライブラリ使用している他ツールで整合取るため、残しておく 2007/10/16 s_suzuki)
}

const _TCHAR topmesh::kTableName[] = _T("TOPMESH");
const _TCHAR org_vics::kTableName[] = _T("ORG_VICS");
const _TCHAR drive_map::kTableName[] = _T("DriveMap_Link");
const _TCHAR ivics::kTableName[] = _T("IVICS");

namespace townpage {
	const TCHAR kTableName[]			= _T("TOWNPAGE");				//!< テーブル名

	const TCHAR kIpCode[]				= _T("IPCODE");					//!< IPコード
	const TCHAR kDataKubun[]			= _T("DATA_KUBUN");				//!< データ取得区分
	const TCHAR kDataSeq[]				= _T("DATA_SEQ");				//!< データ取得シーケンス
	const TCHAR kPstIpCode[]			= _T("PST_IPCODE");				//!< 座標提供会社コード
	const TCHAR kAccCode[]				= _T("ACCCODE");				//!< 精度コード
	const TCHAR kPinPointFlag[]			= _T("PINPOINTFLAG");			//!< ピンポイントフラグ
	const TCHAR kPrefCode[]				= _T("PREFCODE");				//!< 都道府県コード
	const TCHAR kCityCode[]				= _T("CITYCODE");				//!< 市区町村コード
	const TCHAR kAdrCode1[]				= _T("ADRCODE1");				//!< 大字・通称コード
	const TCHAR kAdrCode2[]				= _T("ADRCODE2");				//!< 字・丁目コード
	const TCHAR kChibanCode[]			= _T("CHIBANCODE");				//!< 地番コード
	const TCHAR kJukyoCode[]			= _T("JUKYOCODE");				//!< 住居番号コード
	const TCHAR kNttGrpCode[]			= _T("NTTGRPCODE");				//!< NTT分類コード
	const TCHAR kNttCode[]				= _T("NTTCODE");				//!< NTT業種コード
	const TCHAR kChainCode[]			= _T("CHAINCODE");				//!< 店舗コード
	const TCHAR kName[]					= _T("NAME");					//!< 表示用名称
	const TCHAR kYomi[]					= _T("YOMI");					//!< 表示用名称読み
	const TCHAR kTelC[]					= _T("TEL_C");					//!< 市街局番
	const TCHAR kTel[]					= _T("TEL");					//!< 電話番号
	const TCHAR kNaCode[]				= _T("NACODE");					//!< 使用禁止理由コード
}






} // namespace schema

} // namespace sindy
