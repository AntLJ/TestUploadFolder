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
#include <sindy/schema/rule.h>

namespace sindy {

namespace schema {

namespace annotation_class {

	const _TCHAR kTableName[]			= _T("ANNOTATION_CLASS");	//!< テーブル名
	const _TCHAR kClass[]				= _T("CLASS");				//!< SiNDY注記種別コード
	const _TCHAR kMSNo[]				= _T("MS_NO");				//!< 原稿番号
	const _TCHAR kInClass[]				= _T("IN_CLASS");			//!< IN注記種別コード
	const _TCHAR kClassName[]			= _T("CLASS_NAME");			//!< 種別名称
	const _TCHAR kTopFlag[]				= _T("TOP_FLAG");			//!< TOP_FLAG
	const _TCHAR kMiddleFlag[]			= _T("MIDDLE_FLAG");		//!< MIDDLE_FLAG
	const _TCHAR kBaseFlag[]			= _T("BASE_FLAG");			//!< BASE_FLAG
	const _TCHAR kCityFlag[]			= _T("CITY_FLAG");			//!< CITY_FLAG
	const _TCHAR kDispType[]			= _T("DISP_TYPE");			//!< 表示タイプ
	const _TCHAR kTS1DispRule[]			= _T("T_S1DISPRULE");		//!< トップマップS1表示ルール
	const _TCHAR kTS2DispRule[]			= _T("T_S2DISPRULE");		//!< トップマップS2表示ルール
	const _TCHAR kTS3DispRule[]			= _T("T_S3DISPRULE");		//!< トップマップS3表示ルール
	const _TCHAR kTS4DispRule[]			= _T("T_S4DISPRULE");		//!< トップマップS4表示ルール
	const _TCHAR kTTruePosition[]		= _T("T_TRUEPOSITION_F");	//!< トップマップ真位置フラグ
	const _TCHAR kTFontSize[]			= _T("T_FONTSIZE");			//!< トップマップフォントサイズ
	const _TCHAR kTExceptScale[]		= _T("T_EXCEPTSCALE_F");	//!< トップマップスケールの中抜けフラグ
	const _TCHAR kTDoubleAnno[]			= _T("T_DOUBLEANNO_F");		//!< トップマップ２行注記フラグ
	const _TCHAR kTCopyPaste[]			= _T("T_COPYPASTE_F");		//!< トップマップコピーペーストフラグ
	const _TCHAR kTIDenticalData[]		= _T("T_IDENTICALDATA_F");	//!< トップマップ同一データの存在フラグ
	const _TCHAR kMS1DispRule[]			= _T("M_S1DISPRULE");		//!< ミドルマップS1表示ルール
	const _TCHAR kMS2DispRule[]			= _T("M_S2DISPRULE");		//!< ミドルマップS2表示ルール
	const _TCHAR kMS3DispRule[]			= _T("M_S3DISPRULE");		//!< ミドルマップS3表示ルール
	const _TCHAR kMS4DispRule[]			= _T("M_S4DISPRULE");		//!< ミドルマップS4表示ルール
	const _TCHAR kMTruePosition[]		= _T("M_TRUEPOSITION_F");	//!< ミドルマップ真位置フラグ
	const _TCHAR kMFontSize[]			= _T("M_FONTSIZE");			//!< ミドルマップフォントサイズ
	const _TCHAR kMExceptScale[]		= _T("M_EXCEPTSCALE_F");	//!< ミドルマップスケールの中抜けフラグ
	const _TCHAR kMDoubleAnno[]			= _T("M_DOUBLEANNO_F");		//!< ミドルマップ２行注記フラグ
	const _TCHAR kMCopyPaste[]			= _T("M_COPYPASTE_F");		//!< ミドルマップコピーペーストフラグ
	const _TCHAR kMIDenticalData[]		= _T("M_IDENTICALDATA_F");	//!< ミドルマップ同一データの存在フラグ
	const _TCHAR kBS1DispRule[]			= _T("B_S1DISPRULE");		//!< ベースマップS1表示ルール
	const _TCHAR kBS2DispRule[]			= _T("B_S2DISPRULE");		//!< ベースマップS2表示ルール
	const _TCHAR kBS3DispRule[]			= _T("B_S3DISPRULE");		//!< ベースマップS3表示ルール
	const _TCHAR kBS4DispRule[]			= _T("B_S4DISPRULE");		//!< ベースマップS4表示ルール
	const _TCHAR kBTruePosition[]		= _T("B_TRUEPOSITION_F");	//!< ベースマップ真位置フラグ
	const _TCHAR kBFontSize[]			= _T("B_FONTSIZE");			//!< ベースマップフォントサイズ
	const _TCHAR kBExceptScale[]		= _T("B_EXCEPTSCALE_F");	//!< ベースマップスケールの中抜けフラグ
	const _TCHAR kBDoubleAnno[]			= _T("B_DOUBLEANNO_F");		//!< ベースマップ２行注記フラグ
	const _TCHAR kBCopyPaste[]			= _T("B_COPYPASTE_F");		//!< ベースマップコピーペーストフラグ
	const _TCHAR kBIDenticalData[]		= _T("B_IDENTICALDATA_F");	//!< ベースマップ同一データの存在フラグ
	const _TCHAR kCDispRule[]			= _T("C_SDISPRULE");		//!< 都市地図表示ルール
	const _TCHAR kCTruePosition[]		= _T("C_TRUEPOSITION_F");	//!< 都市地図真位置フラグ
	const _TCHAR kCFontSize[]			= _T("C_FONTSIZE");			//!< 都市地図フォントサイズ
	const _TCHAR kCExceptScale[]		= _T("C_EXCEPTSCALE_F");	//!< 都市地図スケールの中抜けフラグ
	const _TCHAR kCDoubleAnno[]			= _T("C_DOUBLEANNO_F");		//!< 都市地図２行注記フラグ
	const _TCHAR kCCopyPaste[]			= _T("C_COPYPASTE_F");		//!< 都市地図コピーペーストフラグ
	const _TCHAR kCIDenticalData[]		= _T("C_IDENTICALDATA_F");	//!< 都市地図同一データの存在フラグ
	const _TCHAR kRotationRule[]		= _T("ROT_RULE");			//!< 回転に関連するルール
} // annotation_class
namespace background_class {

	const _TCHAR kTableName[]			= _T("BACKGROUND_CLASS");	//!< テーブル名

	const _TCHAR kClass[]				= _T("CLASS");				//!< 種別コード
	const _TCHAR kClassName[]			= _T("CLASS_NAME");			//!< 種別名称
	const _TCHAR kMSNo[]				= _T("MS_NO");				//!< 原稿番号
	const _TCHAR kLayerID[]				= _T("LAYERID");			//!< 背景レイヤのID
	const _TCHAR kUnderGround[]			= _T("UNDERGROUND_F");		//!< 地下フラグ
	const _TCHAR kInClass[]				= _T("IN_CLASS");			//!< IN種別コード
} // background_class
namespace alley_mainte_list {

	const _TCHAR kTableName[]			= _T("ALLEY_MAINTE_LIST");	//!< テーブル名

	const _TCHAR kAlleyMainteList[]		= _T("ALLEY_MAINTE_LIST");	//!< 細街路作業情報コード
	const _TCHAR kWorkYear[]			= _T("WORK_YEAR");			//!< 作業年度
	const _TCHAR Regulation[]			= _T("REGULATION_F");		//!< 規制調査フラグ
} // allay_mainte_list
namespace connectuser {

	const _TCHAR kTableName[]			= _T("CONNECTUSER");		//!< テーブル名

	const _TCHAR kConnectUserID[]		= _T("CONNECTUSR_ID");		//!< 接続ユーザID
	const _TCHAR kName[]				= _T("NAME");				//!< 接続ユーザ名
} // connectuser
namespace creatablelayers {

	const _TCHAR kTableName[]			= _T("CREATABLELAYERS");	//!< テーブル名

	const _TCHAR kPurposeID[]			= _T("PURPOSE_ID");			//!< 作業目的ID
	const _TCHAR kLayerNameID[]			= _T("LAYERNAME_ID");		//!< レイヤ名ID
	const _TCHAR kAutoFlag[]			= _T("AUTO_F");				//!< 自動作成フラグ
	const _TCHAR kAutoValue[]			= _T("AUTO_VALUE");			//!< 自動作成属性値
} // creatablelayers
namespace deletablelayers {

	const _TCHAR kTableName[]			= _T("DELETABLELAYERS");	//!< テーブル名

	const _TCHAR kPurposeID[]			= _T("PURPOSE_ID");			//!< 作業目的ID
	const _TCHAR kLayerNameID[]			= _T("LAYERNAME_ID");		//!< レイヤ名ID
	const _TCHAR kRistrict1[]			= _T("RISTRICT1");			//!< 属性制限ルール１
	const _TCHAR kRistrict2[]			= _T("RISTRICT2");			//!< 属性制限ルール２
	const _TCHAR kRistrict3[]			= _T("RISTRICT3");			//!< 属性制限ルール３
	const _TCHAR kRistrict4[]			= _T("RISTRICT4");			//!< 属性制限ルール４
	const _TCHAR kRistrict5[]			= _T("RISTRICT5");			//!< 属性制限ルール５
	const _TCHAR kRistrict6[]			= _T("RISTRICT6");			//!< 属性制限ルール６
	const _TCHAR kRistrict7[]			= _T("RISTRICT7");			//!< 属性制限ルール７
	const _TCHAR kRistrict8[]			= _T("RISTRICT8");			//!< 属性制限ルール８
	const _TCHAR kRistrict9[]			= _T("RISTRICT9");			//!< 属性制限ルール９
	const _TCHAR kRistrict10[]			= _T("RISTRICT10");			//!< 属性制限ルール１０
} // deletablelayers
namespace dispnavirelation {

	const _TCHAR kTableName[]			= _T("DISPNAVIRELATION");	//!< テーブル名

	const _TCHAR kDispClassID[]			= _T("DISPCLASS_ID");		//!< 道路リンク表示種別ID
	const _TCHAR kNaviClassID[]			= _T("NAVICLASS_ID");		//!< 道路リンク経路種別ID
	const _TCHAR kRelValue[]			= _T("RELVALUE");			//!< ルール値
} // dispnavirelation
namespace dispotherrelation {

	const _TCHAR kTableName[]			= _T("DISPOTHERRELATION");	//!< テーブル名

	const _TCHAR kDispClassID[]			= _T("DISPCLASS_ID");		//!< 道路リンク表示種別ID
	const _TCHAR kOtherAttrID[]			= _T("OTHERATTR_ID");		//!< 道路リンク属性ID
	const _TCHAR kRelValue[]			= _T("RELVALUE");			//!< ルール値
} // dispotherrelation
namespace editattrlayers {

	const _TCHAR kTableName[]			= _T("EDITATTRLAYERS");		//!< テーブル名

	const _TCHAR kPurposeID[]			= _T("PURPOSE_ID");			//!< 作業目的ID
	const _TCHAR kLayerNameID[]			= _T("LAYERNAME_ID");		//!< レイヤ名ID
	const _TCHAR kRistrict1[]			= _T("RISTRICT1");			//!< 属性制限ルール１
	const _TCHAR kRistrict2[]			= _T("RISTRICT2");			//!< 属性制限ルール２
	const _TCHAR kRistrict3[]			= _T("RISTRICT3");			//!< 属性制限ルール３
	const _TCHAR kRistrict4[]			= _T("RISTRICT4");			//!< 属性制限ルール４
	const _TCHAR kRistrict5[]			= _T("RISTRICT5");			//!< 属性制限ルール５
	const _TCHAR kRistrict6[]			= _T("RISTRICT6");			//!< 属性制限ルール６
	const _TCHAR kRistrict7[]			= _T("RISTRICT7");			//!< 属性制限ルール７
	const _TCHAR kRistrict8[]			= _T("RISTRICT8");			//!< 属性制限ルール８
	const _TCHAR kRistrict9[]			= _T("RISTRICT9");			//!< 属性制限ルール９
	const _TCHAR kRistrict10[]			= _T("RISTRICT10");			//!< 属性制限ルール１０
} // editattrlayers
namespace edithistory {

	const _TCHAR kTableName[]			= _T("EDITHISTORY");		//!< テーブル名

	const _TCHAR kModifyDate[]			= _T("MODIFYDATE");			//!< 変更日時
	const _TCHAR kModifyTable[]			= _T("MODIFYTABLE");		//!< 変更したルールテーブル名
	const _TCHAR kModifyOID[]			= _T("MODIFYOID");			//!< 変更したOBJECTID番号
	const _TCHAR kUpdateType[]			= _T("UPDATETYPE_C");		//!< 変更方法
	const _TCHAR kOperator[]			= _T("OPERATOR");			//!< 変更者名
	const _TCHAR kMessage[]				= _T("MESSAGE");			//!< メッセージ
} // edithistory
namespace editmesh {

	const _TCHAR kTableName[]			= _T("EDITMESH");			//!< テーブル名

	const _TCHAR kMeshCode[]			= _T("MESHCODE");			//!< メッシュコード
	const _TCHAR kLayerNameID[]			= _T("LAYERNAME_ID");		//!< レイヤ名ID
	const _TCHAR kPurposeID[]			= _T("PURPOSE_ID");			//!< 作業目的ID
	const _TCHAR kOperatorID[]			= _T("OPERATOR_ID");		//!< オペレータID
} // editmesh
namespace layernames {

	const _TCHAR kTableName[]			= _T("LAYERNAMES");			//!< テーブル名

	const _TCHAR kLayerName[]			= _T("LAYERNAME");			//!< レイヤ名
	const _TCHAR kLayerNameID[]			= _T("LAYERNAME_ID");		//!< レイヤ名ID
	const _TCHAR kLayerAliasName[]		= _T("LAYERALIASNAME");		//!< レイヤエイリアス名
} // layernames
namespace lockarea {

	const _TCHAR kTableName[]			= _T("LOCKAREA");			//!< テーブル名

	const _TCHAR kLayerNameID[]			= _T("LAYERNAME_ID");		//!< レイヤ名ID
	const _TCHAR kConnectUserID[]		= _T("CONNECTUSR_ID");		//!< 接続ユーザ名ID
} // lockarea
namespace lockmesh {

	const _TCHAR kTableName[]			= _T("LOCKMESH");			//!< テーブル名

	const _TCHAR kMeshCode[]			= _T("MESHCODE");			//!< メッシュコード
	const _TCHAR kLayerNameID[]			= _T("LAYERNAME_ID");		//!< レイヤ名ID
	const _TCHAR kConnectUserID[]		= _T("CONNECTUSR_ID");		//!< 接続ユーザ名ID
} // lockmesh
namespace movablelayers {

	const _TCHAR kTableName[]			= _T("MOVABLELAYERS");		//!< テーブル名

	const _TCHAR kPurposeID[]			= _T("PURPOSE_ID");			//!< 作業目的ID
	const _TCHAR kLayerNameID[]			= _T("LAYERNAME_ID");		//!< レイヤ名ID
	const _TCHAR kRistrict1[]			= _T("RISTRICT1");			//!< 属性制限ルール１
	const _TCHAR kRistrict2[]			= _T("RISTRICT2");			//!< 属性制限ルール２
	const _TCHAR kRistrict3[]			= _T("RISTRICT3");			//!< 属性制限ルール３
	const _TCHAR kRistrict4[]			= _T("RISTRICT4");			//!< 属性制限ルール４
	const _TCHAR kRistrict5[]			= _T("RISTRICT5");			//!< 属性制限ルール５
	const _TCHAR kRistrict6[]			= _T("RISTRICT6");			//!< 属性制限ルール６
	const _TCHAR kRistrict7[]			= _T("RISTRICT7");			//!< 属性制限ルール７
	const _TCHAR kRistrict8[]			= _T("RISTRICT8");			//!< 属性制限ルール８
	const _TCHAR kRistrict9[]			= _T("RISTRICT9");			//!< 属性制限ルール９
	const _TCHAR kRistrict10[]			= _T("RISTRICT10");			//!< 属性制限ルール１０
} // movablelayers
namespace soperator {

	const _TCHAR kTableName[]			= _T("OPERATOR");			//!< テーブル名

	const _TCHAR kOperatorID[]			= _T("OPERATOR_ID");		//!< オペレータ名ID
	const _TCHAR kName[]				= _T("NAME");				//!< オペレータ名
	const _TCHAR kAliasName[]			= _T("ALIASNAME");			//!< オペレータ名エイリアス
} // operator
namespace roadattribute {

	const _TCHAR kTableName[]			= _T("ROADATTRIBUTE");		//!< テーブル名

	const _TCHAR kName[]				= _T("NAME");				//!< 道路リンク属性名
	const _TCHAR kAliasName[]			= _T("ALIASNAME");			//!< 道路リンク属性名エイリアス
	const _TCHAR kNameID[]				= _T("NAME_ID");			//!< 道路リンク属性名ID
} // roadattribute
namespace roaddispclass {

	const _TCHAR kTableName[]			= _T("ROADDISPCLASS");		//!< テーブル名

	const _TCHAR kName[]				= _T("NAME");				//!< 道路リンク表示種別名
	const _TCHAR kNameID[]				= _T("NAME_ID");			//!< 道路リンク表示種別名ID
} // roaddispclass
namespace roadnaviclass {

	const _TCHAR kTableName[]			= _T("ROADNAVICLASS");		//!< テーブル名

	const _TCHAR kName[]				= _T("NAME");				//!< 道路リンク経路種別名
	const _TCHAR kNameID[]				= _T("NAME_ID");			//!< 道路リンク経路種別名ID
} // roadnaviclass
namespace shareddeletelayers {

	const _TCHAR kTableName[]			= _T("SHAREDDELETELAYERS");	//!< テーブル名

	const _TCHAR kSetID[]				= _T("SET_ID");				//!< 共有削除レイヤセット番号
	const _TCHAR kLayerNameID[]			= _T("LAYERNAME_ID");		//!< レイヤ名ID
} // shareddeletelayers
namespace sharedmovelayers {

	const _TCHAR kTableName[]			= _T("SHAREDMOVELAYERS");	//!< テーブル名

	const _TCHAR kSetID[]				= _T("SET_ID");				//!< 共有移動レイヤセット番号
	const _TCHAR kLayerNameID[]			= _T("LAYERNAME_ID");		//!< レイヤ名ID
} // sharedmovelayers
namespace workpurpose {

	const _TCHAR kTableName[]			= _T("WORKPURPOSE");		//!< テーブル名

	const _TCHAR kPurpose[]				= _T("PURPOSE");			//!< 作業目的名
	const _TCHAR kMXD[]					= _T("MXD");				//!< MXDファイル名
	const _TCHAR kAutoAttrGiving[]		= _T("AUTOATTRGIVING");		//!< 自動属性付与値
	const _TCHAR kWorkKind[]			= _T("WORKKIND");			//!< 作業種類
	const _TCHAR kGencho[]				= _T("GENCHO_F");			//!< 現調反映作業用フラグ
	const _TCHAR kClaim[]				= _T("CLAIME_F");			//!< クレーム修正作業用フラグ
	const _TCHAR kChuki[]				= _T("CHUKI_F");			//!< 注記編集作業用フラグ
	const _TCHAR kPurposeID[]			= _T("PURPOSE_ID");			//!< 作業目的名ID
	const _TCHAR kDelete[]				= _T("DELETE_F");			//!< 削除済みフラグ
	const _TCHAR kTest[]				= _T("TEST_F");				//!< テスト用フラグ
	const _TCHAR kCategory[]			= _T("CATEGORY");			//!< 作業カテゴリ
	const _TCHAR kLayerFilePath[]		= _T("LAYERFILES_PATH");	//!< レイヤファイルパス
	const _TCHAR kCanSave[]				= _T("CANSAVE_F");			//!< 保存可不可フラグ
} // workpurpose
namespace history_table {

	const _TCHAR kTableName[]    = _T("EXPORT_INFO") ; //!< テーブル名

	const _TCHAR kObjectID[]     = _T("OBJECTID");     //!< オブジェクトID
	const _TCHAR kName[]         = _T("NAME");         //!< 名称
	const _TCHAR kFeatureClass[] = _T("FEATURECLASS"); //!< フィーチャクラス名
	const _TCHAR kConnectInfo[]  = _T("CONNECT_INFO"); //!< 接続情報
	const _TCHAR kUser[]         = _T("USER");         //!< ユーザ名
	const _TCHAR kMachine[]      = _T("MACHINE");      //!< マシン名
	const _TCHAR kDate[]         = _T("EXP_DATE");     //!< 日付
	const _TCHAR kOperation[]    = _T("OPERATION");    //!< 作業名
} // history_table

} // schema

} // sindy
