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

#include "ADAMSurveyImporter.h"

#include "LogUtil.h"

#include "SurveyCSVReader.h"
#include "ADAMSchemaWriter.h"


using namespace std;

// 実行
bool ADAMSurveyImporter::execute()
{
	using namespace adam_v2;

	LogUtil::printLine();
	LogUtil::print("対応ADAMスキーマ:\t" + uh::toStr(AdamSchema::AdamVersion::TableName));
	LogUtil::print("対応納入FMT:\t" + uh::toStr(SurveyCSV::CONST_FMT_VERSION));
	LogUtil::printLine();

	//ログファイル設定
	LogUtil::setLogDirPath(_args.log_dir);

	// インポート先のテーブル、CSVファイル、データ保持フラグを、"インポート用対応情報"として予め用意
	const vector<ImportInfo> vecImportInfo = boost::assign::list_of
		( ImportInfo( AdamSchema::LaneLinkGroup::TableName,				SurveyCSV::LaneLink::FILE_NAME,				true	) )
		( ImportInfo( AdamSchema::LaneNode::TableName,					SurveyCSV::LanePoint::FILE_NAME,			true	) )
		( ImportInfo( AdamSchema::LaneLink::TableName,					SurveyCSV::LaneLink::FILE_NAME,				true	) )
		( ImportInfo( AdamSchema::NQLaneNode::TableName,				SurveyCSV::LanePoint::FILE_NAME,			false	) )
		( ImportInfo( AdamSchema::CompartNode::TableName,				SurveyCSV::CompartPoint::FILE_NAME,			true	) )
		( ImportInfo( AdamSchema::CompartLink::TableName,				SurveyCSV::CompartLink::FILE_NAME,			true	) )
		( ImportInfo( AdamSchema::NQCompartNode::TableName,				SurveyCSV::CompartPoint::FILE_NAME,			true	) )
		( ImportInfo( AdamSchema::RelCompartLinkLaneLink::TableName,	SurveyCSV::RelCompartLink::FILE_NAME,		false	) )
		( ImportInfo( AdamSchema::BorderNode::TableName,				SurveyCSV::BorderPoint::FILE_NAME,			true	) )
		( ImportInfo( AdamSchema::BorderLink::TableName,				SurveyCSV::BorderLink::FILE_NAME,			true	) )
		( ImportInfo( AdamSchema::NQBorderNode::TableName,				SurveyCSV::BorderPoint::FILE_NAME,			true	) )
		( ImportInfo( AdamSchema::RelBorderLinkLaneLink::TableName,		SurveyCSV::RelBorderLink::FILE_NAME,		false	) )
		( ImportInfo( AdamSchema::SignalPoint::TableName,				SurveyCSV::SignalPoint::FILE_NAME,			false	) )
		( ImportInfo( AdamSchema::RelLaneNodeSignalPoint::TableName,	SurveyCSV::RelSignalPoint::FILE_NAME,		false	) )
		( ImportInfo( AdamSchema::CrossWalkLine::TableName,				SurveyCSV::CrossWalkLine::FILE_NAME,		false	) )
		( ImportInfo( AdamSchema::RelLaneNodeCrossWalkLine::TableName,	SurveyCSV::RelCrossWalkLine::FILE_NAME,		false	) )
		( ImportInfo( AdamSchema::RoadSignPoint::TableName,				SurveyCSV::RoadSignPoint::FILE_NAME,		false	) )
		( ImportInfo( AdamSchema::RelLaneNodeRoadSignPoint::TableName,	SurveyCSV::RelRoadSignPoint::FILE_NAME,		false	) )
		( ImportInfo( AdamSchema::VirtualCompartLine::TableName,		SurveyCSV::VirtualCompartLink::FILE_NAME,	false	) )
		( ImportInfo( AdamSchema::CoveringArea::TableName,				SurveyCSV::CoveringArea::FILE_NAME,			false	) )
		;

	// リーダー生成
	SurveyCSVReaderPtr reader = make_shared<SurveyCSVReader>();
	reader->SetParam(_args.survey_dir);
	reader->SetSkipDeleteRecord(_args.skip_delete_record);

	// CSVバージョンチェック
	if(!reader->CheckCsvVersion())
		return false;

	// ライター生成
	ADAMSchemaWriterPtr writer = make_shared<ADAMSchemaWriter>();
	writer->SetParam(_args.import_db);
	writer->SetUnuseVersion(_args.unuse_version);
	writer->SetSource(_args.company, _args.date, _args.lot);

	// 座標変換用パラメータファイルを読み込んでセット
	if(!writer->ReadCrdParam(_args.crd_param))
		return false;

	// SRIDを読み込んでセット
	if(!reader->Read(SurveyCSV::SpatialRef::FILE_NAME))
		return false;
	writer->SetCSV(reader->GetSurveyCSVPtr());
	if(!writer->SetSpatialReferenceId())
		return false;

	// ADAMバージョンチェック
	if(!writer->CheckAdamVersion())
		return false;

	// DB接続
	if(!writer->ConnectDB())
		return false;

	// 編集開始
	if(!writer->StartEdit())
		return false;

	// インポート用対応情報でループ
	for(const auto& info : vecImportInfo)
	{
		// CSVファイル読み込み
		if(!reader->Read(info.fileName))
		{
			writer->AbortEdit();
			return false;
		}

		// ADAMへインポート
		writer->SetCSV(reader->GetSurveyCSVPtr());
		if(!writer->Write(info.tableName))
		{
			writer->AbortEdit();
			return false;
		}

		// 保持フラグが折れていたらCSVデータ破棄（メモリ節約のため）
		if(!info.bKeepData)
			reader->Clear();
	}

	// 編集終了
	return writer->StopEdit();
}