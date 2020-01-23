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
//#include <sindy/schema/category.h>

namespace sindy {

namespace schema {

namespace sindym {

/// 調査ポイント
namespace spot_survey_point {
	extern const _TCHAR kTableName[]; //!< テーブル名

	extern const _TCHAR kObjectidPostgres[];  //!< ポスグレOID
	extern const _TCHAR kCompanyName[];  //!< 調査会社名
	extern const _TCHAR kAccountName[];  //!< アカウント名
	extern const _TCHAR kPermissionLevel[];  //!< 権限レベル
	extern const _TCHAR kModifyDatetime[];  //!< 最終更新日
	extern const _TCHAR kCitymesh[];  //!< 都市地図メッシュ
	extern const _TCHAR kCreatedBySurveyorF[];  //!< 調査員作成フラグ
	extern const _TCHAR kManagementId[];  //!< 管理ID
	extern const _TCHAR kTargetType[];  //!< 対象種別
	extern const _TCHAR kSeason[];  //!< 時期データ
	extern const _TCHAR kSurveyType[];  //!< 現調種別
	extern const _TCHAR kAllDisplayF[];  //!< 表示制限解除フラグ
	extern const _TCHAR kSurveyRank[];  //!< 調査ランク
	extern const _TCHAR kName[];  //!< 名称
	extern const _TCHAR kNameOrg[];  //!< 修正前名称
	extern const _TCHAR kAddress11[];  //!< 住所文字列11桁
	extern const _TCHAR kSurveyAttribute[];  //!< 調査対象属性
	extern const _TCHAR kSurveyRequestType[];  //!< 依頼内容
	extern const _TCHAR kCompanyComment[];  //!< 現調会社コメント
	extern const _TCHAR kChangedCode[];  //!< 変更内容コード
	extern const _TCHAR kSurveyComment[];  //!< 調査コメント
	extern const _TCHAR kPictureImpossibleF[];  //!< 撮影不可フラグ
	extern const _TCHAR kCreatedByIPCF[];  //!< iPC作成フラグ
	extern const _TCHAR kSurveyDate[];  //!< 調査日
	extern const _TCHAR kRemarks1[];  //!< 備考1
	extern const _TCHAR kRemarks2[];  //!< 備考2
	extern const _TCHAR kFirstCheckCompleteDate[];  //!< 一次検査完了日
	extern const _TCHAR kNgAttribute[];  //!< NG種別
	extern const _TCHAR kNgComment[];  //!< NGコメント
}

/// survey_pointのOIDを持つテーブル
namespace spot_survey_point_relational_table {
	extern const _TCHAR kSurveyPointId[];  //!< 調査ポイントID
}

/// 修正前調査ポイント
namespace spot_survey_point_org {
	extern const _TCHAR kTableName[]; //!< テーブル名

	using namespace spot_survey_point_relational_table;
	extern const _TCHAR kCitymeshOrg[];  //!< 修正前都市地図メッシュ
	extern const _TCHAR kMeshOid[];  //!< メッシュOID
}

/// 画像ポイント
namespace picture_point {
	extern const _TCHAR kTableName[]; //!< テーブル名

	using namespace spot_survey_point_relational_table;
	extern const _TCHAR kPictureSeq[];  //!< 画像格納順
	extern const _TCHAR kDirection[];  //!< 撮影向き
}

/// イラストポイント
namespace illustration_point {
	extern const _TCHAR kTableName[]; //!< テーブル名

	using namespace spot_survey_point_relational_table;
}

/// イラストライン
namespace illustration_line {
	extern const _TCHAR kTableName[]; //!< テーブル名

	using namespace spot_survey_point_relational_table;
}

/// イラストポリゴン
namespace illustration_polygon {
	extern const _TCHAR kTableName[]; //!< テーブル名

	using namespace spot_survey_point_relational_table;
}

} // sindym

} // schema

} // sindy

