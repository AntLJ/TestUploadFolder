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

#ifndef SINDY_SCHEMA_RULE_H_
#define SINDY_SCHEMA_RULE_H_

namespace sindy {

namespace schema {

/// 注記ルールテーブル
namespace annotation_class {

	extern const _TCHAR kTableName[];			//!< テーブル名

	extern const _TCHAR kClass[];				//!< SiNDY注記種別コード
	extern const _TCHAR kMSNo[];				//!< 原稿番号
	extern const _TCHAR kInClass[];				//!< IN注記種別コード
	extern const _TCHAR kClassName[];			//!< 種別名称
	extern const _TCHAR kTopFlag[];				//!< TOP_FLAG
	extern const _TCHAR kMiddleFlag[];			//!< MIDDLE_FLAG
	extern const _TCHAR kBaseFlag[];			//!< BASE_FLAG
	extern const _TCHAR kCityFlag[];			//!< CITY_FLAG
	extern const _TCHAR kDispType[];			//!< 表示タイプ
	extern const _TCHAR kTS1DispRule[];			//!< トップマップS1表示ルール
	extern const _TCHAR kTS2DispRule[];			//!< トップマップS2表示ルール
	extern const _TCHAR kTS3DispRule[];			//!< トップマップS3表示ルール
	extern const _TCHAR kTS4DispRule[];			//!< トップマップS4表示ルール
	extern const _TCHAR kTTruePosition[];		//!< トップマップ真位置フラグ
	extern const _TCHAR kTFontSize[];			//!< トップマップフォントサイズ
	extern const _TCHAR kTExceptScale[];		//!< トップマップスケールの中抜けフラグ
	extern const _TCHAR kTDoubleAnno[];			//!< トップマップ２行注記フラグ
	extern const _TCHAR kTCopyPaste[];			//!< トップマップコピーペーストフラグ
	extern const _TCHAR kTIDenticalData[];		//!< トップマップ同一データの存在フラグ
	extern const _TCHAR kMS1DispRule[];			//!< ミドルマップS1表示ルール
	extern const _TCHAR kMS2DispRule[];			//!< ミドルマップS2表示ルール
	extern const _TCHAR kMS3DispRule[];			//!< ミドルマップS3表示ルール
	extern const _TCHAR kMS4DispRule[];			//!< ミドルマップS4表示ルール
	extern const _TCHAR kMTruePosition[];		//!< ミドルマップ真位置フラグ
	extern const _TCHAR kMFontSize[];			//!< ミドルマップフォントサイズ
	extern const _TCHAR kMExceptScale[];		//!< ミドルマップスケールの中抜けフラグ
	extern const _TCHAR kMDoubleAnno[];			//!< ミドルマップ２行注記フラグ
	extern const _TCHAR kMCopyPaste[];			//!< ミドルマップコピーペーストフラグ
	extern const _TCHAR kMIDenticalData[];		//!< ミドルマップ同一データの存在フラグ
	extern const _TCHAR kBS1DispRule[];			//!< ベースマップS1表示ルール
	extern const _TCHAR kBS2DispRule[];			//!< ベースマップS2表示ルール
	extern const _TCHAR kBS3DispRule[];			//!< ベースマップS3表示ルール
	extern const _TCHAR kBS4DispRule[];			//!< ベースマップS4表示ルール
	extern const _TCHAR kBTruePosition[];		//!< ベースマップ真位置フラグ
	extern const _TCHAR kBFontSize[];			//!< ベースマップフォントサイズ
	extern const _TCHAR kBExceptScale[];		//!< ベースマップスケールの中抜けフラグ
	extern const _TCHAR kBDoubleAnno[];			//!< ベースマップ２行注記フラグ
	extern const _TCHAR kBCopyPaste[];			//!< ベースマップコピーペーストフラグ
	extern const _TCHAR kBIDenticalData[];		//!< ベースマップ同一データの存在フラグ
	extern const _TCHAR kCDispRule[];			//!< 都市地図S1表示ルール
	extern const _TCHAR kCTruePosition[];		//!< 都市地図真位置フラグ
	extern const _TCHAR kCFontSize[];			//!< 都市地図フォントサイズ
	extern const _TCHAR kCExceptScale[];		//!< 都市地図スケールの中抜けフラグ
	extern const _TCHAR kCDoubleAnno[];			//!< 都市地図２行注記フラグ
	extern const _TCHAR kCCopyPaste[];			//!< 都市地図コピーペーストフラグ
	extern const _TCHAR kCIDenticalData[];		//!< 都市地図同一データの存在フラグ
	extern const _TCHAR kRotationRule[];		//!< 回転に関連するルール

	/// スケール表示ルール
	namespace scaledisp_rule {
		enum ECode {
			kOOOOO						= 0,	//!< 記号+文字列 ... 指示点化可能						-> kAny (10)
			kOXOXO						= 1, //!< 記号+文字列(指示点不可) ... 指示点化・記号のみ不可	-> kNorDirPoint (50)
			kOOXXX						= 2, //!< 記号のみ												-> kMarkOnly (90)
			kOXOXX						= 3, //!< 文字列のみ											-> kStrOnly (100)
			kXXXXX						= 4, //!< 表示無し												-> kNone (999)
			kAny						= 10,	//!< 指+文|記+文|記のみ|文のみ|表示無	 … 何でも有り
			kAnyMust					= 11,	//!< 指+文|記+文|記のみ|文のみ|×		 … 何でも有り（表示必須）
			kMustMark					= 20,	//!< 指+文|記+文|記のみ|    ×|表示無	 … 記号必須
			kMustMarkMust				= 21,	//!< 指+文|記+文|記のみ|    ×|×		 … 記号必須（表示必須）
			kMustStr					= 30,	//!< 指+文|記+文|    ×|文のみ|表示無	 … 文字列必須
			kMustStrMust				= 31,	//!< 指+文|記+文|    ×|文のみ|×		 … 文字列必須（表示必須）
			kMustPair					= 40,	//!< 指+文|記+文|    ×|    ×|表示無	 … 記号と文字必ずペア
			kMustPairMust				= 41,	//!< 指+文|記+文|    ×|    ×|×		 … 記号と文字必ずペア（表示必須）
			kNotDirPoint				= 50,	//!< ×   |記+文|記のみ|文のみ|表示無	 … 指示点不可
			kNotDirPointMust			= 51,	//!< ×   |記+文|記のみ|文のみ|×		 … 指示点不可（表示必須）
			kNotDirPointMultMark		= 60,	//!< ×   |記+文|記のみ|    ×|表示無	 … 指示点不可記号必須
			kNotDirPointMultMarkMust	= 61,	//!< ×   |記+文|記のみ|    ×|×		 … 指示点不可記号必須（表示必須）
			kNotDirPointMustStr			= 70,	//!< ×   |記+文|    ×|文のみ|表示無	 … 指示点不可文字列必須
			kNotDirPointMustStrMust		= 71,	//!< ×   |記+文|    ×|文のみ|×		 … 指示点不可文字列必須（表示必須）
			kMarkStr					= 80,	//!< ×   |記+文|    ×|    ×|表示無	 … 記号（指示点不可）+文字列のみ
			kMarkStrMust				= 81,	//!< ×   |記+文|    ×|    ×|×		 … 記号（指示点不可）+文字列のみ（表示必須）
			kMarkOnly					= 90,	//!< ×   |   ×|記のみ|    ×|表示無	 … 記号（指示点不可）のみ
			kMarkOnlyMust				= 91,	//!< ×   |   ×|記のみ|    ×|×		 … 記号（指示点不可）のみ（表示必須）
			kStrOnly					= 100,	//!< ×   |   ×|    ×|文のみ|表示無	 … 文字列のみ
			kStrOnlyMust				= 101,	//!< ×   |   ×|    ×|文のみ|×		 … 文字列のみ（表示必須）
			kNone						= 999,	//!< ×   |   ×|    ×|    ×|表示無	 … 表示なし
		};
	} // scaledisp_rule

	/// 注記表示文字列表示タイプコード
	namespace disptype_rule {
		enum ECode {
			kCommon			= 0, //!< 一般
			kLinear			= 1, //!< 線状
			kFlag			= 2, //!< 旗揚げ
			kRouteNumber	= 3, //!< 道路番号
			kSpecialLinear	= 4, //!< 特殊線状
		};
	} // disp_type
} // annotation_class

/// 背景ルールテーブル
namespace background_class {

	extern const _TCHAR kTableName[];			//!< テーブル名

	extern const _TCHAR kClass[];				//!< 種別コード
	extern const _TCHAR kClassName[];			//!< 種別名称
	extern const _TCHAR kMSNo[];				//!< 原稿番号
	extern const _TCHAR kLayerID[];				//!< 背景レイヤのID
	extern const _TCHAR kUnderGround[];			//!< 地下フラグ
	extern const _TCHAR kInClass[];				//!< IN種別コード

	/// 背景レイヤコード
	namespace background_layer_id {
		enum ECode {
			kCommon				= 0,	//!< 共通
			kTopLine			= 1,	//!< トップライン
			kTopSite			= 4,	//!< トップ背景
			kTopContour			= 5,	//!< トップ段彩
			kMiddleLine			= 11,	//!< ミドルライン
			kMiddleRail			= 12,	//!< ミドル鉄道
			kMiddleSta			= 13,	//!< ミドル鉄道駅
			kMiddleSite			= 14,	//!< ミドル背景
			kMiddleContour		= 15,	//!< ミドル段彩
			kBaseLine			= 21,	//!< ベースライン
			kBaseRail			= 22,	//!< ベース鉄道
			kBaseSta			= 23,	//!< ベース鉄道駅
			kBaseSite			= 24,	//!< ベース背景
			kBaseContour		= 25,	//!< ベース段彩
			kBaseGround			= 26,	//!< ベース陸地
			kCityLine			= 31,	//!< 都市ライン
			kCityRail			= 32,	//!< 都市鉄道
			kCitySta			= 33,	//!< 都市鉄道駅
			kCitySite			= 34,	//!< 都市背景
			kCityBuilding		= 35,	//!< 都市建物
			kCityBuildingLine	= 36,	//!< 都市建物ライン
		};
	} // background_layer_id
} // background_class

/// 作業年度管理テーブル
namespace alley_mainte_list {

	extern const _TCHAR kTableName[];			//!< テーブル名

	extern const _TCHAR kAlleyMainteList[];		//!< 細街路作業情報コード
	extern const _TCHAR kWorkYear[];			//!< 作業年度
	extern const _TCHAR Regulation[];			//!< 規制調査フラグ
} // allay_mainte_list

/// 接続ユーザ名管理テーブル
namespace connectuser {

	extern const _TCHAR kTableName[];			//!< テーブル名

	extern const _TCHAR kConnectUserID[];		//!< 接続ユーザID
	extern const _TCHAR kName[];				//!< 接続ユーザ名
} // connectuser

/// 作成可能レイヤ管理テーブル
namespace creatablelayers {

	extern const _TCHAR kTableName[];			//!< テーブル名

	extern const _TCHAR kPurposeID[];			//!< 作業目的ID
	extern const _TCHAR kLayerNameID[];			//!< レイヤ名ID
	extern const _TCHAR kAutoFlag[];			//!< 自動作成フラグ
	extern const _TCHAR kAutoValue[];			//!< 自動作成属性値
} // creatablelayers

/// 削除可能レイヤ管理テーブル
namespace deletablelayers {

	extern const _TCHAR kTableName[];			//!< テーブル名

	extern const _TCHAR kPurposeID[];			//!< 作業目的ID
	extern const _TCHAR kLayerNameID[];			//!< レイヤ名ID
	extern const _TCHAR kRistrict1[];			//!< 属性制限ルール１
	extern const _TCHAR kRistrict2[];			//!< 属性制限ルール２
	extern const _TCHAR kRistrict3[];			//!< 属性制限ルール３
	extern const _TCHAR kRistrict4[];			//!< 属性制限ルール４
	extern const _TCHAR kRistrict5[];			//!< 属性制限ルール５
	extern const _TCHAR kRistrict6[];			//!< 属性制限ルール６
	extern const _TCHAR kRistrict7[];			//!< 属性制限ルール７
	extern const _TCHAR kRistrict8[];			//!< 属性制限ルール８
	extern const _TCHAR kRistrict9[];			//!< 属性制限ルール９
	extern const _TCHAR kRistrict10[];			//!< 属性制限ルール１０
} // deletablelayers

/// 道路リンクの表示種別と経路種別の関連定義用テーブル
namespace dispnavirelation {

	extern const _TCHAR kTableName[];			//!< テーブル名

	extern const _TCHAR kDispClassID[];			//!< 道路リンク表示種別ID
	extern const _TCHAR kNaviClassID[];			//!< 道路リンク経路種別ID
	extern const _TCHAR kRelValue[];			//!< ルール値
} // dispnavirelation

/// 路リンクの表示種別とそれ以外の属性の関連定義用テーブル
namespace dispotherrelation {

	extern const _TCHAR kTableName[];			//!< テーブル名

	extern const _TCHAR kDispClassID[];			//!< 道路リンク表示種別ID
	extern const _TCHAR kOtherAttrID[];			//!< 道路リンク属性ID
	extern const _TCHAR kRelValue[];			//!< ルール値
} // dispotherrelation

/// 属性編集可能レイヤ管理テーブル
namespace editattrlayers {

	extern const _TCHAR kTableName[];			//!< テーブル名

	extern const _TCHAR kPurposeID[];			//!< 作業目的ID
	extern const _TCHAR kLayerNameID[];			//!< レイヤ名ID
	extern const _TCHAR kRistrict1[];			//!< 属性制限ルール１
	extern const _TCHAR kRistrict2[];			//!< 属性制限ルール２
	extern const _TCHAR kRistrict3[];			//!< 属性制限ルール３
	extern const _TCHAR kRistrict4[];			//!< 属性制限ルール４
	extern const _TCHAR kRistrict5[];			//!< 属性制限ルール５
	extern const _TCHAR kRistrict6[];			//!< 属性制限ルール６
	extern const _TCHAR kRistrict7[];			//!< 属性制限ルール７
	extern const _TCHAR kRistrict8[];			//!< 属性制限ルール８
	extern const _TCHAR kRistrict9[];			//!< 属性制限ルール９
	extern const _TCHAR kRistrict10[];			//!< 属性制限ルール１０
} // editattrlayers

/// ルール編集履歴テーブル
namespace edithistory {

	extern const _TCHAR kTableName[];			//!< テーブル名

	extern const _TCHAR kModifyDate[];			//!< 変更日時
	extern const _TCHAR kModifyTable[];			//!< 変更したルールテーブル名
	extern const _TCHAR kModifyOID[];			//!< 変更したOBJECTID番号
	extern const _TCHAR kUpdateType[];			//!< 変更方法
	extern const _TCHAR kOperator[];			//!< 変更者名
	extern const _TCHAR kMessage[];				//!< メッセージ
} // edithistory

/// 編集可能メッシュ管理テーブル
namespace editmesh {

	extern const _TCHAR kTableName[];			//!< テーブル名

	extern const _TCHAR kMeshCode[];			//!< メッシュコード
	extern const _TCHAR kLayerNameID[];			//!< レイヤ名ID
	extern const _TCHAR kPurposeID[];			//!< 作業目的ID
	extern const _TCHAR kOperatorID[];			//!< オペレータID
} // editmesh

/// レイヤ名管理テーブル
namespace layernames {

	extern const _TCHAR kTableName[];			//!< テーブル名

	extern const _TCHAR kLayerName[];			//!< レイヤ名
	extern const _TCHAR kLayerNameID[];			//!< レイヤ名ID
	extern const _TCHAR kLayerAliasName[];		//!< レイヤエイリアス名
} // layernames

/// ロックエリア管理テーブル
namespace lockarea {

	extern const _TCHAR kTableName[];			//!< テーブル名

	extern const _TCHAR kLayerNameID[];			//!< レイヤ名ID
	extern const _TCHAR kConnectUserID[];		//!< 接続ユーザ名ID
} // lockarea

/// ロックメッシュ管理テーブル
namespace lockmesh {

	extern const _TCHAR kTableName[];			//!< テーブル名

	extern const _TCHAR kMeshCode[];			//!< メッシュコード
	extern const _TCHAR kLayerNameID[];			//!< レイヤ名ID
	extern const _TCHAR kConnectUserID[];		//!< 接続ユーザ名ID
} // lockmesh

/// 移動可能レイヤ管理テーブル
namespace movablelayers {

	extern const _TCHAR kTableName[];			//!< テーブル名

	extern const _TCHAR kPurposeID[];			//!< 作業目的ID
	extern const _TCHAR kLayerNameID[];			//!< レイヤ名ID
	extern const _TCHAR kRistrict1[];			//!< 属性制限ルール１
	extern const _TCHAR kRistrict2[];			//!< 属性制限ルール２
	extern const _TCHAR kRistrict3[];			//!< 属性制限ルール３
	extern const _TCHAR kRistrict4[];			//!< 属性制限ルール４
	extern const _TCHAR kRistrict5[];			//!< 属性制限ルール５
	extern const _TCHAR kRistrict6[];			//!< 属性制限ルール６
	extern const _TCHAR kRistrict7[];			//!< 属性制限ルール７
	extern const _TCHAR kRistrict8[];			//!< 属性制限ルール８
	extern const _TCHAR kRistrict9[];			//!< 属性制限ルール９
	extern const _TCHAR kRistrict10[];			//!< 属性制限ルール１０
} // movablelayers

/// 作業者管理テーブル
namespace soperator {

	extern const _TCHAR kTableName[];			//!< テーブル名

	extern const _TCHAR kOperatorID[];			//!< オペレータ名ID
	extern const _TCHAR kName[];				//!< オペレータ名
	extern const _TCHAR kAliasName[];			//!< オペレータ名エイリアス
} // operator

/// 道路属性名称管理テーブル
namespace roadattribute {

	extern const _TCHAR kTableName[];			//!< テーブル名

	extern const _TCHAR kName[];				//!< 道路リンク属性名
	extern const _TCHAR kAliasName[];			//!< 道路リンク属性名エイリアス
	extern const _TCHAR kNameID[];				//!< 道路リンク属性名ID
} // roadattribute

/// 道路リンク表示種別名管理テーブル
namespace roaddispclass {

	extern const _TCHAR kTableName[];			//!< テーブル名

	extern const _TCHAR kName[];				//!< 道路リンク表示種別名
	extern const _TCHAR kNameID[];				//!< 道路リンク表示種別名ID
} // roaddispclass

/// 道路リンク経路種別名管理テーブル
namespace roadnaviclass {

	extern const _TCHAR kTableName[];			//!< テーブル名

	extern const _TCHAR kName[];				//!< 道路リンク経路種別名
	extern const _TCHAR kNameID[];				//!< 道路リンク経路種別名ID
} // roadnaviclass

/// 共有削除定義管理テーブル
namespace shareddeletelayers {

	extern const _TCHAR kTableName[];			//!< テーブル名

	extern const _TCHAR kSetID[];				//!< 共有削除レイヤセット番号
	extern const _TCHAR kLayerNameID[];			//!< レイヤ名ID
} // shareddeletelayers

/// 共有移動定義管理テーブル
namespace sharedmovelayers {

	extern const _TCHAR kTableName[];			//!< テーブル名

	extern const _TCHAR kSetID[];				//!< 共有移動レイヤセット番号
	extern const _TCHAR kLayerNameID[];			//!< レイヤ名ID
} // sharedmovelayers

/// 作業目的管理テーブル
namespace workpurpose {

	extern const _TCHAR kTableName[];			//!< テーブル名

	extern const _TCHAR kPurpose[];				//!< 作業目的名
	extern const _TCHAR kMXD[];					//!< MXDファイル名
	extern const _TCHAR kAutoAttrGiving[];		//!< 自動属性付与値
	extern const _TCHAR kWorkKind[];			//!< 作業種類
	extern const _TCHAR kGencho[];				//!< 現調反映作業用フラグ
	extern const _TCHAR kClaim[];				//!< クレーム修正作業用フラグ
	extern const _TCHAR kChuki[];				//!< 注記編集作業用フラグ
	extern const _TCHAR kPurposeID[];			//!< 作業目的名ID
	extern const _TCHAR kDelete[];				//!< 削除済みフラグ
	extern const _TCHAR kTest[];				//!< テスト用フラグ
	extern const _TCHAR kCategory[];			//!< 作業カテゴリ
	extern const _TCHAR kLayerFilePath[];		//!< レイヤファイルパス
	extern const _TCHAR kCanSave[];				//!< 保存可不可フラグ
} // workpurpose

// PGDB出力情報テーブル
namespace history_table {

	extern const _TCHAR kTableName[];    //!< テーブル名

	extern const _TCHAR kObjectID[];     //!< オブジェクトID
	extern const _TCHAR kName[];         //!< 名称
	extern const _TCHAR kFeatureClass[]; //!< フィーチャクラス名
	extern const _TCHAR kConnectInfo[];  //!< 接続情報
	extern const _TCHAR kUser[];         //!< ユーザ名
	extern const _TCHAR kMachine[];      //!< マシン名
	extern const _TCHAR kDate[];         //!< 日付
	extern const _TCHAR kOperation[];    //!< 作業名
} // history_table

} // schema

} // sindy

#endif // ifndef SINDY_SCHEMA_RULE_H_
