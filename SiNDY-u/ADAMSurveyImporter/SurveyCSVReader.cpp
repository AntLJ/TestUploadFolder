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

#include <boost/filesystem/operations.hpp>
#include <boost/foreach.hpp>

#include <TDC/useful_headers/directory_util.h>

#include "LogUtil.h"
#include "SurveyCSVReader.h"

using namespace adam_v2;

SurveyCSVReader::SurveyCSVReader(void) : 
	_csv(nullptr),
	_skipDeleteRecord(false),
	_hasError(false)
{
	_csv = std::make_shared<SurveyCSV>();
}


SurveyCSVReader::~SurveyCSVReader(void)
{
}

bool SurveyCSVReader::Read(const uh::tstring& fileName)
{
	bool ret = true;
	bool readSkiped = false; // 読み込みスキップしたか

	LogUtil::printDoubleLine();
	LogUtil::print("入力ファイルの読み込みを開始します");

	// CSVファイルからデータを読み込む
	// 既に読み込み済みの場合はスキップし、ジオメトリチェックも行わない
	     if( fileName == SurveyCSV::SpatialRef::FILE_NAME )
		_csv->listSpatialRef.empty() ?			ret &=	ReadCSV( _path, fileName, &SurveyCSVReader::ReadSpatialRef )			:	readSkiped = true;	// 空間参照
	else if( fileName == SurveyCSV::Version::FILE_NAME )
		_csv->listVersion.empty() ?				ret &=	ReadCSV( _path, fileName, &SurveyCSVReader::ReadVersion )				:	readSkiped = true;	// CSVバージョン
	else if( fileName == SurveyCSV::LaneLink::FILE_NAME )
		_csv->listLaneLink.empty() ?			ret &=	ReadCSV( _path, fileName, &SurveyCSVReader::ReadLaneLink )				:	readSkiped = true;	// 車線リンク
	else if( fileName == SurveyCSV::LanePoint::FILE_NAME )
		_csv->listLanePoint.empty() ?			ret &=	ReadCSV( _path, fileName, &SurveyCSVReader::ReadLanePoint )				:	readSkiped = true;	// 車線ノード
	else if( fileName == SurveyCSV::RelLaneLink::FILE_NAME )
		_csv->listRelLaneLink.empty() ?					ReadCSV( _path, fileName, &SurveyCSVReader::ReadRelLaneLink )			:	readSkiped = true;	// 車線関連
	else if( fileName == SurveyCSV::CompartLink::FILE_NAME )
		_csv->listCompartLink.empty() ?			ret &=	ReadCSV( _path, fileName, &SurveyCSVReader::ReadCompartLink )			:	readSkiped = true;	// 区画線リンク
	else if( fileName == SurveyCSV::CompartPoint::FILE_NAME )
		_csv->listCompartPoint.empty() ?		ret &=	ReadCSV( _path, fileName, &SurveyCSVReader::ReadCompartPoint )			:	readSkiped = true;	// 区画線ノード
	else if( fileName == SurveyCSV::RelCompartLink::FILE_NAME )
		_csv->listRelCompartLink.empty() ?		ret &=	ReadCSV( _path, fileName, &SurveyCSVReader::ReadRelCompartLink )		:	readSkiped = true;	// 区画線関連
	else if( fileName == SurveyCSV::BorderLink::FILE_NAME )
		_csv->listBorderLink.empty() ?			ret &=	ReadCSV( _path, fileName, &SurveyCSVReader::ReadBorderLink )			:	readSkiped = true;	// 境界線リンク
	else if( fileName == SurveyCSV::BorderPoint::FILE_NAME )
		_csv->listBorderPoint.empty() ?			ret &=	ReadCSV( _path, fileName, &SurveyCSVReader::ReadBorderPoint )			:	readSkiped = true;	// 境界線ノード
	else if( fileName == SurveyCSV::RelBorderLink::FILE_NAME )
		_csv->listRelBorderLink.empty() ?		ret &=	ReadCSV( _path, fileName, &SurveyCSVReader::ReadRelBorderLink )			:	readSkiped = true;	// 境界線関連
	else if( fileName == SurveyCSV::SignalPoint::FILE_NAME )
		_csv->listSignalPoint.empty() ?			ret &=	ReadCSV( _path, fileName, &SurveyCSVReader::ReadSignalPoint )			:	readSkiped = true;	// 信号機
	else if( fileName == SurveyCSV::RelSignalPoint::FILE_NAME )
		_csv->listRelSignalPoint.empty() ?		ret &=	ReadCSV( _path, fileName, &SurveyCSVReader::ReadRelSignalPoint )		:	readSkiped = true;	// 信号機関連
	else if( fileName == SurveyCSV::RoadSignPoint::FILE_NAME )
		_csv->listRoadSignPoint.empty() ?		ret &=	ReadCSV( _path, fileName, &SurveyCSVReader::ReadRoadSignPoint )			:	readSkiped = true;	// 道路標識
	else if( fileName == SurveyCSV::RelRoadSignPoint::FILE_NAME )
		_csv->listRelRoadSignPoint.empty() ?	ret &=	ReadCSV( _path, fileName, &SurveyCSVReader::ReadRelRoadSignPoint )		:	readSkiped = true;	// 道路標識関連
	else if( fileName == SurveyCSV::CrossWalkLine::FILE_NAME )
		_csv->listCrossWalkLine.empty() ?				ReadCSV( _path, fileName, &SurveyCSVReader::ReadCrossWalkLine )			:	readSkiped = true;	// 横断歩道
	else if( fileName == SurveyCSV::RelCrossWalkLine::FILE_NAME )
		_csv->listRelCrossWalkLine.empty() ?			ReadCSV( _path, fileName, &SurveyCSVReader::ReadRelCrossWalkLine )		:	readSkiped = true;	// 横断歩道関連
	else if( fileName == SurveyCSV::VirtualCompartLink::FILE_NAME )
		_csv->listVirtualCompartLink.empty() ?			ReadCSV( _path, fileName, &SurveyCSVReader::ReadVirtualCompartLink )	:	readSkiped = true;	// 仮想線リンク
	else if( fileName == SurveyCSV::VirtualCompartPoint::FILE_NAME )
		_csv->listVirtualCompartPoint.empty() ?			ReadCSV( _path, fileName, &SurveyCSVReader::ReadVirtualCompartPoint )	:	readSkiped = true;	// 仮想線ノード
	else if( fileName == SurveyCSV::RelVirtualCompartLink::FILE_NAME )
		_csv->listRelVirtualCompartLink.empty() ?		ReadCSV( _path, fileName, &SurveyCSVReader::ReadRelVirtualCompartLink )	:	readSkiped = true;	// 仮想線関連
	else if( fileName == SurveyCSV::CoveringArea::FILE_NAME )
		_csv->listCoveringArea.empty() ?				ReadCSV( _path, fileName, &SurveyCSVReader::ReadCoveringArea )			:	readSkiped = true;	// 覆い物
	else
		_ASSERT( false );

	if(ret)
	{
		if(!readSkiped)
		{
			LogUtil::print("入力ファイルの読み込みに成功しました");
		}
		else
		{
			LogUtil::print("入力ファイルは読み込み済みです");
		}
	}
	else
	{
		LogUtil::printError("入力ファイルの読み込み処理が終了しました。エラーがあるためログを確認してください");
	}
	
	//ジオメトリチェック
	if(ret && !readSkiped)
	{
		LogUtil::printLine();
		LogUtil::print("入力ファイルのジオメトリチェックを実行します");

		ret &= CheckGeometryIntegrity();
		ret &= CheckSortByOID();

		if(ret)
		{
			LogUtil::print("ジオメトリチェックに成功しました");
		}
		else
		{
			LogUtil::printError("ジオメトリチェックに失敗しました。エラーがあるためログを確認してください");
		}
	}
	
	LogUtil::printDoubleLine();

	return ret;
}

void SurveyCSVReader::Clear()
{
	_csv->Clear();
}


bool SurveyCSVReader::ReadCSV(
	const uh::tstring& dirPath,
	const uh::tstring& fileName,
	bool (SurveyCSVReader::*func)(const boost::tokenizer<boost::char_separator<char>>&)
	)
{
	uh::tstring filePath = dirPath + _T( "\\" ) + fileName;

	LogUtil::print(uh::toStr( filePath ) + " の読み込みを開始します...");

	if(!_csv)
	{
		return false;
	}

	if( ! uh::isFile( filePath.c_str() ))
	{
		LogUtil::printError(50, "ファイルオープン失敗\t" + uh::toStr( filePath ));
		LogUtil::printError("読み込みに失敗しました");
		return false;
	}

	//ファイル読み込み
	_hasError = false;
	boost::filesystem::path const path(filePath);
	std::ifstream ifs(filePath.c_str());

	//BOM スキップ
	//http://qiita.com/yumetodo/items/3744afa94ab029f4e1d5
	{
		int dst[3];
		for (auto& i : dst)
		{
			i = ifs.get();
		}
		const int utf8[] = { 0xEF, 0xBB, 0xBF };
		if (!std::equal(std::begin(dst), std::end(dst), utf8))
		{
			ifs.seekg(0);
		}
	}

	std::string line;
	boost::char_separator<char> sep(uh::toStr(SurveyCSV::DELIMITER).c_str(), "", boost::keep_empty_tokens);

	long line_count = 0;
	long error_count = LogUtil::getErrCount();
	long normal_count = 0;
	long indexUpdateC = GetIndexUpdateC( fileName );
	bool isCheched = true;	//フォーマットチェック結果
	
	while (std::getline(ifs, line)) 
	{
		++line_count;
		LogUtil::flushLine((boost::format("%10ld reading...") % line_count).str());

		// 入力ファイルは BOMなしUTF-8(UTF-8N)固定
		// UTF-8 → Unicode変換
		std::string lineStr;
		{
			int buflen = MultiByteToWideChar(CP_UTF8, 0, line.c_str(), -1, NULL, 0);
			WCHAR* wbuf = new wchar_t[buflen];

			// 文字列をワイド文字列（Unicode）にマップ
			MultiByteToWideChar(
				CP_UTF8,			// コードページ：UTF-8 を使った変換
				0,					// 文字の種類を指定するフラグ
				line.c_str(),		// 変換元文字列
				-1,					// 変換元文字列サイズ(-1指定で自動計算)
				wbuf,				// 変換先バッファ
				buflen);			// 変換先バッファサイズ	

			lineStr = uh::toStr(wbuf);

			delete[] wbuf;
		}

		boost::tokenizer<boost::char_separator<char>> tokens(lineStr, sep);

		if(line_count == SurveyCSV::HEADER_COUNT)
		{
			// コメント行
			// 12	WAR	ヘッダ行有無チェック	FMT仕様通りか
			if(lineStr.size() > 0 && lineStr.compare(0, 1, SurveyCSV::HEADER_PREFIX) == 0)
				continue;
			LogUtil::printWarning(12, "ヘッダ行が定義されていません");
		}
		
		// フラグが立っていて、かつ削除レコードであればスキップ
		if( _skipDeleteRecord && IsDeleteRecord( fileName, tokens, indexUpdateC ) )
			continue;

		if(!((this->*func)(tokens)))
		{
			LogUtil::printError(std::to_string(line_count) + "行目にフォーマット違反があります。ログを確認してください");
			isCheched = false;
		}
		else
		{
			normal_count++;
		}
	}

	ifs.close();

	if(!isCheched)
	{
		LogUtil::printError(std::to_string(LogUtil::getErrCount() - error_count) + "件 入力データにフォーマット違反があります。ログを確認してください");
	}
	else
	{
		LogUtil::print("正常に読み込みました\t" + uh::toStr( filePath ));
	}

	LogUtil::print(std::to_string(normal_count) + " 行 読み込みました");
	LogUtil::print( uh::toStr( filePath ) + " の読み込みを終了します...");

	return isCheched;
}

bool SurveyCSVReader::IsDeleteRecord( const uh::tstring& fileName, const boost::tokenizer<boost::char_separator<char>>& tokens, long index )
{
	// 更新コード「削除」のレコード判別用
	static bool isDeleteRecord = false;

	// 以下は判定対象外
	if( fileName == SurveyCSV::SpatialRef::FILE_NAME ||
		fileName == SurveyCSV::Version::FILE_NAME ||
		fileName == SurveyCSV::RelLaneLink::FILE_NAME ||
		fileName == SurveyCSV::CrossWalkLine::FILE_NAME ||
		fileName == SurveyCSV::RelCrossWalkLine::FILE_NAME ||
		fileName == SurveyCSV::VirtualCompartLink::FILE_NAME ||
		fileName == SurveyCSV::VirtualCompartPoint::FILE_NAME ||
		fileName == SurveyCSV::RelVirtualCompartLink::FILE_NAME ||
		fileName == SurveyCSV::CoveringArea::FILE_NAME )
		return false;

	// ジオメトリのみか判定
	if( getTokenCount( tokens ) == SurveyCSV::GEOMETRY_ROW_COUNT )
	{
		// 削除フラグレコードを返す
		// 車線・区画線・境界線は、納入FMTの仕様上、最初の構成点のレコードに属性が存在し、
		// 以降の構成点のレコードはジオメトリのみとなっている
		// そのため、「更新コードのカラムに値が存在しない＝削除ではない」とならないよう、
		// 同一のオブジェクトを表す構成点群で、同じ判定となるよう管理している
		return isDeleteRecord;
	}

	// カラムをループ
	long column = 0;
	for( const auto& token : tokens )
	{
		++column;
		if( index != column )
			continue;

		// 削除レコードか判定
		if( token == std::to_string( update_code::kDelete ) )
		{
			// 車線・区画線・境界線か判定
			if( fileName == SurveyCSV::LaneLink::FILE_NAME ||
				fileName == SurveyCSV::CompartLink::FILE_NAME ||
				fileName == SurveyCSV::BorderLink::FILE_NAME )
			{
				// 削除レコードフラグを立てる
				isDeleteRecord = true;
			}

			return true;
		}
		// 削除レコードフラグを倒す
		isDeleteRecord = false;
	}

	return false;
}

long SurveyCSVReader::GetIndexUpdateC( const uh::tstring& fileName )
{
	long indexUpdateC = -1;

	if( fileName == SurveyCSV::LaneLink::FILE_NAME )
	{
		SurveyCSV::LaneLink csv;
		indexUpdateC = csv.update_c.GetIndex();
	}
	else if( fileName == SurveyCSV::LanePoint::FILE_NAME )
	{
		SurveyCSV::LanePoint csv;
		indexUpdateC = csv.update_c.GetIndex();
	}
	else if( fileName == SurveyCSV::CompartLink::FILE_NAME )
	{
		SurveyCSV::CompartLink csv;
		indexUpdateC = csv.update_c.GetIndex();
	}
	else if( fileName == SurveyCSV::CompartPoint::FILE_NAME )
	{
		SurveyCSV::CompartPoint csv;
		indexUpdateC = csv.update_c.GetIndex();
	}
	else if( fileName == SurveyCSV::RelCompartLink::FILE_NAME )
	{
		SurveyCSV::RelCompartLink csv;
		indexUpdateC = csv.update_c.GetIndex();
	}
	else if( fileName == SurveyCSV::BorderLink::FILE_NAME )
	{
		SurveyCSV::BorderLink csv;
		indexUpdateC = csv.update_c.GetIndex();
	}
	else if( fileName == SurveyCSV::BorderPoint::FILE_NAME )
	{
		SurveyCSV::BorderPoint csv;
		indexUpdateC = csv.update_c.GetIndex();
	}
	else if( fileName == SurveyCSV::RelBorderLink::FILE_NAME )
	{
		SurveyCSV::RelBorderLink csv;
		indexUpdateC = csv.update_c.GetIndex();
	}
	else if( fileName == SurveyCSV::SignalPoint::FILE_NAME )
	{
		SurveyCSV::SignalPoint csv;
		indexUpdateC = csv.update_c.GetIndex();
	}
	else if( fileName == SurveyCSV::RelSignalPoint::FILE_NAME )
	{
		SurveyCSV::RelSignalPoint csv;
		indexUpdateC = csv.update_c.GetIndex();
	}
	else if( fileName == SurveyCSV::RoadSignPoint::FILE_NAME )
	{
		SurveyCSV::RoadSignPoint csv;
		indexUpdateC = csv.update_c.GetIndex();
	}
	else if( fileName == SurveyCSV::RelRoadSignPoint::FILE_NAME )
	{
		SurveyCSV::RelRoadSignPoint csv;
		indexUpdateC = csv.update_c.GetIndex();
	}

	return indexUpdateC;
}

long SurveyCSVReader::getTokenCount(const boost::tokenizer<boost::char_separator<char>>& token)
{
	long count = 0;

	BOOST_FOREACH(std::string s, token)
	{
		count++;
	}

	return count;
}

bool SurveyCSVReader::CheckGeometryIntegrity()
{
	//ジオメトリフィーチャを対象にジオメトリ整合性チェック実施

	bool ret = true;

	//車線リンク
	for(auto& data : _csv->listLaneLink)
	{
		if(!data.shape.Check(true))
		{
			LogUtil::printError(uh::toStr(data.FILE_NAME)+ "\tジオメトリエラーがあります。ログを確認してください\tOID:" + data.objectid.GetText());
			ret &= false;
		}
	}

	//車線ポイントの整合性チェックはないのでSkip

	//区画線リンク
	for(auto& data : _csv->listCompartLink)
	{
		if(!data.shape.Check(true))
		{
			LogUtil::printError(uh::toStr(data.FILE_NAME)+ "\tジオメトリエラーがあります。ログを確認してください\tOID:" + data.objectid.GetText());
			ret &= false;
		}
	}

	//区画線ポイントの整合性チェックはないのでSkip

	//境界線リンク
	for(auto& data : _csv->listBorderLink)
	{
		if(!data.shape.Check(true))
		{
			LogUtil::printError(uh::toStr(data.FILE_NAME)+ "\tジオメトリエラーがあります。ログを確認してください\tOID:" + data.objectid.GetText());
			ret &= false;
		}
	}

	//境界線ポイントの整合性チェックはないのでSkip

	//信号機ポイントの整合性チェックはないのでSkip

	//標識ポイントの整合性チェックはないのでSkip

	//横断歩道ライン
	for(auto& data : _csv->listCrossWalkLine)
	{
		if(!data.shape.Check(true))
		{
			LogUtil::printError(uh::toStr(data.FILE_NAME)+ "\tジオメトリエラーがあります。ログを確認してください\tOID:" + data.objectid.GetText());
			ret &= false;
		}
	}

	//仮想線リンク
	for(auto& data : _csv->listVirtualCompartLink)
	{
		if(!data.shape.Check(true))
		{
			LogUtil::printError(uh::toStr(data.FILE_NAME)+ "\tジオメトリエラーがあります。ログを確認してください\tOID:" + data.objectid.GetText());
			ret &= false;
		}
	}

	//覆い物エリア
	for(auto& data : _csv->listCoveringArea)
	{
		if(!data.shape.Check(true))
		{
			LogUtil::printError(uh::toStr(data.FILE_NAME)+ "\tジオメトリエラーがあります。ログを確認してください\tOID:" + data.objectid.GetText());
			ret &= false;
		}
	}

	if(ret)
	{
		LogUtil::print("ジオメトリ整合性チェックに成功しました");
	}
	else
	{
		LogUtil::printError("ジオメトリ整合性チェックにエラーがあります。ログを確認してください");
	}

	return ret;
}

bool SurveyCSVReader::CheckSortByOID()
{
	//車線リンク
	{
		long long beforeOid = -1;

		for(const auto& csv : _csv->listLaneLink)
		{
			long long currentOid = csv.objectid.GetValue();
			if(beforeOid == -1)
			{
				beforeOid = currentOid;
				continue;
			}
			if(beforeOid >=  currentOid)
			{
				LogUtil::printWarning(13, "OID昇順ではありません\t車線リンクOID:" + std::to_string(beforeOid));
			}

			beforeOid = currentOid;
		}
	}

	//車線ポイント
	{
		long long beforeOid = -1;

		for(const auto& csv : _csv->listLanePoint)
		{
			long long currentOid = csv.objectid.GetValue();
			if(beforeOid == -1)
			{
				beforeOid = currentOid;
				continue;
			}
			if(beforeOid >=  currentOid)
			{
				LogUtil::printWarning(13, "OID昇順ではありません\t車線ポイントOID:" + std::to_string(beforeOid));
			}

			beforeOid = currentOid;
		}
	}

	//車線関連
	{
		long long beforeOid = -1;

		for(const auto& csv : _csv->listRelLaneLink)
		{
			long long currentOid = csv.objectid.GetValue();
			if(beforeOid == -1)
			{
				beforeOid = currentOid;
				continue;
			}
			if(beforeOid >=  currentOid)
			{
				LogUtil::printWarning(13, "OID昇順ではありません\t車線関連リンクOID:" + std::to_string(beforeOid));
			}

			beforeOid = currentOid;
		}
	}

	//区画線リンク
	{
		long long beforeOid = -1;

		for(const auto& csv : _csv->listCompartLink)
		{
			long long currentOid = csv.objectid.GetValue();
			if(beforeOid == -1)
			{
				beforeOid = currentOid;
				continue;
			}
			if(beforeOid >=  currentOid)
			{
				LogUtil::printWarning(13, "OID昇順ではありません\t区画線リンクOID:" + std::to_string(beforeOid));
			}

			beforeOid = currentOid;
		}
	}

	//区画線ポイント
	{
		long long beforeOid = -1;

		for(const auto& csv : _csv->listCompartPoint)
		{
			long long currentOid = csv.objectid.GetValue();
			if(beforeOid == -1)
			{
				beforeOid = currentOid;
				continue;
			}
			if(beforeOid >=  currentOid)
			{
				LogUtil::printWarning(13, "OID昇順ではありません\t区画線ポイントOID:" + std::to_string(beforeOid));
			}

			beforeOid = currentOid;
		}
	}

	//区画線関連
	{
		long long beforeOid = -1;

		for(const auto& csv : _csv->listRelCompartLink)
		{
			long long currentOid = csv.objectid.GetValue();
			if(beforeOid == -1)
			{
				beforeOid = currentOid;
				continue;
			}
			if(beforeOid >=  currentOid)
			{
				LogUtil::printWarning(13, "OID昇順ではありません\t区画線関連OID:" + std::to_string(beforeOid));
			}

			beforeOid = currentOid;
		}
	}

	//境界線リンク
	{
		long long beforeOid = -1;

		for(const auto& csv : _csv->listBorderLink)
		{
			long long currentOid = csv.objectid.GetValue();
			if(beforeOid == -1)
			{
				beforeOid = currentOid;
				continue;
			}
			if(beforeOid >=  currentOid)
			{
				LogUtil::printWarning(13, "OID昇順ではありません\t境界線リンクOID:" + std::to_string(beforeOid));
			}

			beforeOid = currentOid;
		}
	}

	//境界線ポイント
	{
		long long beforeOid = -1;

		for(const auto& csv : _csv->listBorderPoint)
		{
			long long currentOid = csv.objectid.GetValue();
			if(beforeOid == -1)
			{
				beforeOid = currentOid;
				continue;
			}
			if(beforeOid >=  currentOid)
			{
				LogUtil::printWarning(13, "OID昇順ではありません\t境界線ポイントOID:" + std::to_string(beforeOid));
			}

			beforeOid = currentOid;
		}
	}

	//境界線関連
	{
		long long beforeOid = -1;

		for(const auto& csv : _csv->listRelBorderLink)
		{
			long long currentOid = csv.objectid.GetValue();
			if(beforeOid == -1)
			{
				beforeOid = currentOid;
				continue;
			}
			if(beforeOid >=  currentOid)
			{
				LogUtil::printWarning(13, "OID昇順ではありません\t境界線関連OID:" + std::to_string(beforeOid));
			}

			beforeOid = currentOid;
		}
	}

	//信号機
	{
		long long beforeOid = -1;

		for(const auto& csv : _csv->listSignalPoint)
		{
			long long currentOid = csv.objectid.GetValue();
			if(beforeOid == -1)
			{
				beforeOid = currentOid;
				continue;
			}
			if(beforeOid >=  currentOid)
			{
				LogUtil::printWarning(13, "OID昇順ではありません\t信号機OID:" + std::to_string(beforeOid));
			}

			beforeOid = currentOid;
		}
	}

	//信号機関連
	{
		long long beforeOid = -1;

		for(const auto& csv : _csv->listRelSignalPoint)
		{
			long long currentOid = csv.objectid.GetValue();
			if(beforeOid == -1)
			{
				beforeOid = currentOid;
				continue;
			}
			if(beforeOid >=  currentOid)
			{
				LogUtil::printWarning(13, "OID昇順ではありません\t信号機関連OID:" + std::to_string(beforeOid));
			}

			beforeOid = currentOid;
		}
	}

	//道路標識
	{
		long long beforeOid = -1;

		for(const auto& csv : _csv->listRoadSignPoint)
		{
			long long currentOid = csv.objectid.GetValue();
			if(beforeOid == -1)
			{
				beforeOid = currentOid;
				continue;
			}
			if(beforeOid >=  currentOid)
			{
				LogUtil::printWarning(13, "OID昇順ではありません\t道路標識OID:" + std::to_string(beforeOid));
			}

			beforeOid = currentOid;
		}
	}

	//道路標識関連
	{
		long long beforeOid = -1;

		for(const auto& csv : _csv->listRelRoadSignPoint)
		{
			long long currentOid = csv.objectid.GetValue();
			if(beforeOid == -1)
			{
				beforeOid = currentOid;
				continue;
			}
			if(beforeOid >=  currentOid)
			{
				LogUtil::printWarning(13, "OID昇順ではありません\t道路標識関連OID:" + std::to_string(beforeOid));
			}

			beforeOid = currentOid;
		}
	}

	//横断歩道
	{
		long long beforeOid = -1;

		for(const auto& csv : _csv->listCrossWalkLine)
		{
			long long currentOid = csv.objectid.GetValue();
			if(beforeOid == -1)
			{
				beforeOid = currentOid;
				continue;
			}
			if(beforeOid >=  currentOid)
			{
				LogUtil::printWarning(13, "OID昇順ではありません\t横断歩道OID:" + std::to_string(beforeOid));
			}

			beforeOid = currentOid;
		}
	}

	//横断歩道関連
	{
		long long beforeOid = -1;

		for(const auto& csv : _csv->listRelCrossWalkLine)
		{
			long long currentOid = csv.objectid.GetValue();
			if(beforeOid == -1)
			{
				beforeOid = currentOid;
				continue;
			}
			if(beforeOid >=  currentOid)
			{
				LogUtil::printWarning(13, "OID昇順ではありません\t横断歩道関連OID:" + std::to_string(beforeOid));
			}

			beforeOid = currentOid;
		}
	}

	//すべてWARなので true
	return true;
}

bool SurveyCSVReader::CheckCsvVersion()
{
	if(!Read(SurveyCSV::Version::FILE_NAME))
		return false;

	if(_csv->listVersion.size() == 0)
	{
		LogUtil::printError("フォーマットファイルが読み込まれていません");
		return false;
	}

	uh::tstring version = _csv->listVersion.begin()->format_def.GetValue();

	//バージョン許容値チェック
	for(const auto& allow_ver: SurveyCSV::CONST_ALLOW_FMT_VERSION_LIST)
	{
		if(allow_ver == version)
		{
			LogUtil::printWarning(801, "論理チェック", "FMTバージョンが最新ではありませんが、継続可能なため処理を続行します", "\t入力Ver:" + uh::toStr(_csv->listVersion.begin()->format_def.GetValue()) + "\t利用可能Ver:" + uh::toStr(SurveyCSV::CONST_FMT_VERSION));
			return true;
		}
	}

	//バージョン一致チェック
	if(SurveyCSV::CONST_FMT_VERSION != version)
	{
		LogUtil::printError(800, "論理チェック", "FMTバージョンが異なります", "\t入力Ver:" + uh::toStr(_csv->listVersion.begin()->format_def.GetValue()) + "\t利用可能Ver:" + uh::toStr(SurveyCSV::CONST_FMT_VERSION));
		return false;
	}

	return true;
}

bool SurveyCSVReader::ReadLaneLink(const boost::tokenizer<boost::char_separator<char>>& tokens)
{
	if(!_csv)
	{
		return false;
	}

	long tokenCount = getTokenCount(tokens);
	
	//列数チェック
	if(tokenCount != SurveyCSV::GEOMETRY_ROW_COUNT && tokenCount != SurveyCSV::LaneLink::ROW_COUNT)
	{
		LogUtil::printError(6, "列数エラー","列数:" + std::to_string(tokenCount), "列数上限:" + std::to_string(SurveyCSV::LaneLink::ROW_COUNT));
		_hasError = true;
		return false;
	}
	
	SurveyCSV::LaneLink csv;
	long row_count = 1;
	bool isCheched = true;
	Polyline geometry;
	long long oid;

	std::string lon, lat, height;
	// 要素の格納
	BOOST_FOREACH(std::string s, tokens)
	{
		if(row_count <= SurveyCSV::GEOMETRY_ROW_COUNT)
		{
			if(row_count == Location::INDEX_LON)
				lon = s;
			else if(row_count == Location::INDEX_LAT)
				lat = s;
			else if(row_count == Location::INDEX_HEIGHT)
				height = s;

			if(row_count == SurveyCSV::GEOMETRY_ROW_COUNT)
			{
				if(tokenCount == SurveyCSV::GEOMETRY_ROW_COUNT)
				{
					if(!_hasError && _csv->listLaneLink.size() > 0)
					{
						//OID行が追加済み かつ 追加済みのオブジェクトに追記
						Polyline addedGeo = _csv->listLaneLink.back().shape.GetValue();
						isCheched &= addedGeo.AddPoint(lon, lat, height);
						_csv->listLaneLink.back().shape.SetValue(addedGeo);
					}
					else
					{
						LogUtil::printError(52,  "インポートエラー\t属性定義行がエラーのためインポートをSkipしました");
						return false;
					}
				}
				else if(tokenCount > SurveyCSV::GEOMETRY_ROW_COUNT)
				{
					isCheched &= geometry.AddPoint(lon, lat, height);
				}
			}
		}
		else if(row_count == csv.objectid.GetIndex())
		{
			isCheched &= csv.objectid.SetText(s);
			oid = csv.objectid.GetValue();
		}
		else if(row_count == csv.from_node_id.GetIndex())
			isCheched &= csv.from_node_id.SetText(s);
		else if(row_count == csv.to_node_id.GetIndex())
			isCheched &= csv.to_node_id.SetText(s);
		else if(row_count == csv.lane_group_id.GetIndex())
		{
			isCheched &= csv.lane_group_id.SetText(s);
		}
		else if(row_count == csv.lane_group_c.GetIndex())
			isCheched &= csv.lane_group_c.SetText(s);
		else if(row_count == csv.lane_no.GetIndex())
			isCheched &= csv.lane_no.SetText(s);
		else if(row_count == csv.lane_count.GetIndex())
			isCheched &= csv.lane_count.SetText(s);
		else if(row_count == csv.max_legal_speed_c.GetIndex())
			isCheched &= csv.max_legal_speed_c.SetText(s);
		else if(row_count == csv.min_legal_speed_c.GetIndex())
			isCheched &= csv.min_legal_speed_c.SetText(s);
		else if(row_count == csv.travel_direction_c.GetIndex())
			isCheched &= csv.travel_direction_c.SetText(s);
		else if(row_count == csv.road_structure_c.GetIndex())
			isCheched &= csv.road_structure_c.SetText(s);
		else if(row_count == csv.both_link_direction_f.GetIndex())
			isCheched &= csv.both_link_direction_f.SetText(s);
		else if(row_count == csv.left_change_f.GetIndex())
			isCheched &= csv.left_change_f.SetText(s);
		else if(row_count == csv.right_change_f.GetIndex())
			isCheched &= csv.right_change_f.SetText(s);
		else if(row_count == csv.left_protrusion_f.GetIndex())
			isCheched &= csv.left_protrusion_f.SetText(s);
		else if(row_count == csv.right_protrusion_f.GetIndex())
			isCheched &= csv.right_protrusion_f.SetText(s);
		else if(row_count == csv.rubbing_f.GetIndex())
			isCheched &= csv.rubbing_f.SetText(s);
		else if(row_count == csv.tunnel_f.GetIndex())
			isCheched &= csv.tunnel_f.SetText(s);
		else if(row_count == csv.update_c.GetIndex())
			isCheched &= csv.update_c.SetText(s);
		else
		{
			LogUtil::printError(6, "列数エラー","列数:" + std::to_string(row_count), "列数上限:" + std::to_string(csv.ROW_COUNT));
			isCheched = false;
			break;
		}

		row_count++;
	}

	// リストに追加
	if(tokenCount > SurveyCSV::GEOMETRY_ROW_COUNT)
	{
		if(isCheched)
		{
			_hasError = false;
			isCheched &= csv.shape.SetValue(geometry);
			_csv->listLaneLink.push_back(csv);
		}
		else
		{
			_hasError = true;
			LogUtil::printError(52,  "インポートエラー", "エラーのためインポートをSkipしました","OID:" + std::to_string(oid));
		}
	}

	return isCheched;
}

bool SurveyCSVReader::ReadLanePoint(const boost::tokenizer<boost::char_separator<char>>& tokens)
{
	if(!_csv)
	{
		return false;
	}

	long tokenCount = getTokenCount(tokens);
	
	//列数チェック
	if(tokenCount != SurveyCSV::LanePoint::ROW_COUNT)
	{
		LogUtil::printError(6, "列数エラー","列数:" + std::to_string(tokenCount), "列数上限:" + std::to_string(SurveyCSV::LanePoint::ROW_COUNT));
		return false;
	}
	
	SurveyCSV::LanePoint csv;
	long row_count = 1;
	bool isCheched = true;
	Point geometry;
	long long oid;

	std::string lon, lat, height;
	// 要素の格納
	BOOST_FOREACH(std::string s, tokens)
	{
		if(row_count <= SurveyCSV::GEOMETRY_ROW_COUNT)
		{
			if(row_count == Location::INDEX_LON)
				lon = s;
			else if(row_count == Location::INDEX_LAT)
				lat = s;
			else if(row_count == Location::INDEX_HEIGHT)
			{
				height = s;
				isCheched &= geometry.AddPoint(lon, lat, height);
				isCheched &= csv.shape.SetValue(geometry);
			}
		}
		else if(row_count == csv.objectid.GetIndex())
		{
			isCheched &= csv.objectid.SetText(s);
			oid = csv.objectid.GetValue();
		}
		else if(row_count == csv.node_id.GetIndex())
			isCheched &= csv.node_id.SetText(s);
		else if(row_count == csv.point_class_c.GetIndex())
			isCheched &= csv.point_class_c.SetText(s);
		else if(row_count == csv.lane_link_id.GetIndex())
			isCheched &= csv.lane_link_id.SetText(s);
		else if(row_count == csv.sequence.GetIndex())
			isCheched &= csv.sequence.SetText(s);
		else if(row_count == csv.lastnode_f.GetIndex())
			isCheched &= csv.lastnode_f.SetText(s);
		else if(row_count == csv.stop_point_f.GetIndex())
			isCheched &= csv.stop_point_f.SetText(s);
		else if(row_count == csv.width.GetIndex())
			isCheched &= csv.width.SetText(s);
		else if(row_count == csv.linear_element_c.GetIndex())
			isCheched &= csv.linear_element_c.SetText(s);
		else if(row_count == csv.curvature.GetIndex())
			isCheched &= csv.curvature.SetText(s);
		else if(row_count == csv.curvature_radius.GetIndex())
			isCheched &= csv.curvature_radius.SetText(s);
		else if(row_count == csv.curvature_change_rate.GetIndex())
			isCheched &= csv.curvature_change_rate.SetText(s);
		else if(row_count == csv.clothoid_parameter.GetIndex())
			isCheched &= csv.clothoid_parameter.SetText(s);
		else if(row_count == csv.longitudinal_slope.GetIndex())
			isCheched &= csv.longitudinal_slope.SetText(s);
		else if(row_count == csv.cross_slope.GetIndex())
			isCheched &= csv.cross_slope.SetText(s);
		else if(row_count == csv.cover_f.GetIndex())
			isCheched &= csv.cover_f.SetText(s);
		else if(row_count == csv.update_c.GetIndex())
			isCheched &= csv.update_c.SetText(s);
		else
		{
			LogUtil::printError(6, "列数エラー","列数:" + std::to_string(row_count), "列数上限:" + std::to_string(csv.ROW_COUNT));
			isCheched = false;
			break;
		}

		row_count++;
	}

	// リストに追加
	if(isCheched)
	{
		_csv->listLanePoint.push_back(csv);
	}
	else
	{
		LogUtil::printError(52,  "インポートエラー", "エラーのためインポートをSkipしました","OID:" + std::to_string(oid));
	}

	return isCheched;
}

bool SurveyCSVReader::ReadRelLaneLink(const boost::tokenizer<boost::char_separator<char>>& tokens)
{
	if(!_csv)
	{
		return false;
	}

	long tokenCount = getTokenCount(tokens);
	
	//列数チェック
	if(tokenCount != SurveyCSV::RelLaneLink::ROW_COUNT)
	{
		LogUtil::printError(6, "列数エラー","列数:" + std::to_string(tokenCount), "列数上限:" + std::to_string(SurveyCSV::RelLaneLink::ROW_COUNT));
		return false;
	}
	
	SurveyCSV::RelLaneLink csv;
	long row_count = 1;
	bool isCheched = true;
	long long oid;

	std::string lon, lat, height;
	// 要素の格納
	BOOST_FOREACH(std::string s, tokens)
	{
		if(row_count == csv.objectid.GetIndex())
		{
			isCheched &= csv.objectid.SetText(s);
			oid = csv.objectid.GetValue();
		}
		else if(row_count == csv.lane_link_id.GetIndex())
			isCheched &= csv.lane_link_id.SetText(s);
		else if(row_count == csv.road_link_id.GetIndex())
			isCheched &= csv.road_link_id.SetText(s);
		else if(row_count == csv.road_link_direction_c.GetIndex())
			isCheched &= csv.road_link_direction_c.SetText(s);
		else if(row_count == csv.update_c.GetIndex())
			isCheched &= csv.update_c.SetText(s);
		else
		{
			LogUtil::printError(6, "列数エラー","列数:" + std::to_string(row_count), "列数上限:" + std::to_string(csv.ROW_COUNT));
			isCheched = false;
			break;
		}

		row_count++;
	}

	// リストに追加
	if(isCheched)
	{
		_csv->listRelLaneLink.push_back(csv);
	}
	else
	{
		LogUtil::printError(52,  "インポートエラー", "エラーのためインポートをSkipしました","OID:" + std::to_string(oid));
	}

	return isCheched;
}

bool SurveyCSVReader::ReadCompartLink(const boost::tokenizer<boost::char_separator<char>>& tokens)
{
	if(!_csv)
	{
		return false;
	}

	long tokenCount = getTokenCount(tokens);
	
	//列数チェック
	if(tokenCount != SurveyCSV::GEOMETRY_ROW_COUNT && tokenCount != SurveyCSV::CompartLink::ROW_COUNT)
	{
		LogUtil::printError(6, "列数エラー","列数:" + std::to_string(tokenCount), "列数上限:" + std::to_string(SurveyCSV::CompartLink::ROW_COUNT));
		_hasError = true;
		return false;
	}
	
	SurveyCSV::CompartLink csv;
	long row_count = 1;
	bool isCheched = true;
	Polyline geometry;
	long long oid;

	std::string lon, lat, height;
	// 要素の格納
	BOOST_FOREACH(std::string s, tokens)
	{
		if(row_count <= SurveyCSV::GEOMETRY_ROW_COUNT)
		{
			if(row_count == Location::INDEX_LON)
				lon = s;
			else if(row_count == Location::INDEX_LAT)
				lat = s;
			else if(row_count == Location::INDEX_HEIGHT)
				height = s;

			if(row_count == SurveyCSV::GEOMETRY_ROW_COUNT)
			{
				if(tokenCount == SurveyCSV::GEOMETRY_ROW_COUNT)
				{
					if(!_hasError && _csv->listCompartLink.size() > 0)
					{
						//OID行が追加済み かつ 追加済みのオブジェクトに追記
						Polyline addedGeo = _csv->listCompartLink.back().shape.GetValue();
						isCheched &= addedGeo.AddPoint(lon, lat, height);
						_csv->listCompartLink.back().shape.SetValue(addedGeo);
					}
					else
					{
						LogUtil::printError(52,  "インポートエラー\t属性定義行がエラーのためインポートをSkipしました");
						return false;
					}
				}
				else if(tokenCount > SurveyCSV::GEOMETRY_ROW_COUNT)
				{
					isCheched &= geometry.AddPoint(lon, lat, height);
				}
			}
		}
		else if(row_count == csv.objectid.GetIndex())
		{
			isCheched &= csv.objectid.SetText(s);
			oid = csv.objectid.GetValue();
		}
		else if(row_count == csv.from_node_id.GetIndex())
			isCheched &= csv.from_node_id.SetText(s);
		else if(row_count == csv.to_node_id.GetIndex())
			isCheched &= csv.to_node_id.SetText(s);
		else if(row_count == csv.compart_line_c.GetIndex())
			isCheched &= csv.compart_line_c.SetText(s);
		else if(row_count == csv.compart_style_c.GetIndex())
			isCheched &= csv.compart_style_c.SetText(s);
		else if(row_count == csv.slowdown_label_c.GetIndex())
			isCheched &= csv.slowdown_label_c.SetText(s);
		else if(row_count == csv.installation_f.GetIndex())
			isCheched &= csv.installation_f.SetText(s);
		else if(row_count == csv.update_c.GetIndex())
			isCheched &= csv.update_c.SetText(s);
		else
		{
			LogUtil::printError(6, "列数エラー","列数:" + std::to_string(row_count), "列数上限:" + std::to_string(csv.ROW_COUNT));
			isCheched = false;
			break;
		}

		row_count++;
	}

	// リストに追加
	if(tokenCount > SurveyCSV::GEOMETRY_ROW_COUNT)
	{
		if(isCheched)
		{
			_hasError = false;
			isCheched &= csv.shape.SetValue(geometry);
			_csv->listCompartLink.push_back(csv);
		}
		else
		{
			_hasError = true;
			LogUtil::printError(52,  "インポートエラー", "エラーのためインポートをSkipしました","OID:" + std::to_string(oid));
		}
	}

	return isCheched;
}

bool SurveyCSVReader::ReadCompartPoint(const boost::tokenizer<boost::char_separator<char>>& tokens)
{
	if(!_csv)
	{
		return false;
	}

	long tokenCount = getTokenCount(tokens);
	
	//列数チェック
	if(tokenCount != SurveyCSV::CompartPoint::ROW_COUNT)
	{
		LogUtil::printError(6, "列数エラー","列数:" + std::to_string(tokenCount), "列数上限:" + std::to_string(SurveyCSV::CompartPoint::ROW_COUNT));
		return false;
	}
	
	SurveyCSV::CompartPoint csv;
	long row_count = 1;
	bool isCheched = true;
	Point geometry;
	long long oid;

	std::string lon, lat, height;
	// 要素の格納
	BOOST_FOREACH(std::string s, tokens)
	{
		if(row_count <= SurveyCSV::GEOMETRY_ROW_COUNT)
		{
			if(row_count == Location::INDEX_LON)
				lon = s;
			else if(row_count == Location::INDEX_LAT)
				lat = s;
			else if(row_count == Location::INDEX_HEIGHT)
			{
				height = s;
				isCheched &= geometry.AddPoint(lon, lat, height);
				isCheched &= csv.shape.SetValue(geometry);
			}
		}
		else if(row_count == csv.objectid.GetIndex())
		{
			isCheched &= csv.objectid.SetText(s);
			oid = csv.objectid.GetValue();
		}
		else if(row_count == csv.node_id.GetIndex())
			isCheched &= csv.node_id.SetText(s);
		else if(row_count == csv.point_class_c.GetIndex())
			isCheched &= csv.point_class_c.SetText(s);
		else if(row_count == csv.compart_link_id.GetIndex())
			isCheched &= csv.compart_link_id.SetText(s);
		else if(row_count == csv.sequence.GetIndex())
			isCheched &= csv.sequence.SetText(s);
		else if(row_count == csv.lastnode_f.GetIndex())
			isCheched &= csv.lastnode_f.SetText(s);
		else if(row_count == csv.width.GetIndex())
			isCheched &= csv.width.SetText(s);
		else if(row_count == csv.update_c.GetIndex())
			isCheched &= csv.update_c.SetText(s);
		else
		{
			LogUtil::printError(6, "列数エラー","列数:" + std::to_string(row_count), "列数上限:" + std::to_string(csv.ROW_COUNT));
			isCheched = false;
			break;
		}

		row_count++;
	}

	// リストに追加
	if(isCheched)
	{
		_csv->listCompartPoint.push_back(csv);
	}
	else
	{
		LogUtil::printError(52,  "インポートエラー", "エラーのためインポートをSkipしました","OID:" + std::to_string(oid));
	}

	return isCheched;
}

bool SurveyCSVReader::ReadRelCompartLink(const boost::tokenizer<boost::char_separator<char>>& tokens)
{
	if(!_csv)
	{
		return false;
	}

	long tokenCount = getTokenCount(tokens);
	
	//列数チェック
	if(tokenCount != SurveyCSV::RelCompartLink::ROW_COUNT)
	{
		LogUtil::printError(6, "列数エラー","列数:" + std::to_string(tokenCount), "列数上限:" + std::to_string(SurveyCSV::RelCompartLink::ROW_COUNT));
		return false;
	}
	
	SurveyCSV::RelCompartLink csv;
	long row_count = 1;
	bool isCheched = true;
	long long oid;

	std::string lon, lat, height;
	// 要素の格納
	BOOST_FOREACH(std::string s, tokens)
	{
		if(row_count == csv.objectid.GetIndex())
		{
			isCheched &= csv.objectid.SetText(s);
			oid = csv.objectid.GetValue();
		}
		else if(row_count == csv.lane_link_id.GetIndex())
			isCheched &= csv.lane_link_id.SetText(s);
		else if(row_count == csv.lr_f.GetIndex())
			isCheched &= csv.lr_f.SetText(s);
		else if(row_count == csv.compart_link_id.GetIndex())
			isCheched &= csv.compart_link_id.SetText(s);
		else if(row_count == csv.compart_link_direction_c.GetIndex())
			isCheched &= csv.compart_link_direction_c.SetText(s);
		else if(row_count == csv.sequence.GetIndex())
			isCheched &= csv.sequence.SetText(s);
		else if(row_count == csv.update_c.GetIndex())
			isCheched &= csv.update_c.SetText(s);
		else
		{
			LogUtil::printError(6, "列数エラー","列数:" + std::to_string(row_count), "列数上限:" + std::to_string(csv.ROW_COUNT));
			isCheched = false;
			break;
		}

		row_count++;
	}

	// リストに追加
	if(isCheched)
	{
		_csv->listRelCompartLink.push_back(csv);
	}
	else
	{
		LogUtil::printError(52,  "インポートエラー", "エラーのためインポートをSkipしました","OID:" + std::to_string(oid));
	}

	return isCheched;
}

bool SurveyCSVReader::ReadBorderLink(const boost::tokenizer<boost::char_separator<char>>& tokens)
{
	if(!_csv)
	{
		return false;
	}

	long tokenCount = getTokenCount(tokens);
	
	//列数チェック
	if(tokenCount != SurveyCSV::GEOMETRY_ROW_COUNT && tokenCount != SurveyCSV::BorderLink::ROW_COUNT)
	{
		LogUtil::printError(6, "列数エラー","列数:" + std::to_string(tokenCount), "列数上限:" + std::to_string(SurveyCSV::BorderLink::ROW_COUNT));
		_hasError = true;
		return false;
	}
	
	SurveyCSV::BorderLink csv;
	long row_count = 1;
	bool isCheched = true;
	Polyline geometry;
	long long oid;

	std::string lon, lat, height;
	// 要素の格納
	BOOST_FOREACH(std::string s, tokens)
	{
		if(row_count <= SurveyCSV::GEOMETRY_ROW_COUNT)
		{
			if(row_count == Location::INDEX_LON)
				lon = s;
			else if(row_count == Location::INDEX_LAT)
				lat = s;
			else if(row_count == Location::INDEX_HEIGHT)
				height = s;

			if(row_count == SurveyCSV::GEOMETRY_ROW_COUNT)
			{
				if(tokenCount == SurveyCSV::GEOMETRY_ROW_COUNT)
				{
					if(!_hasError && _csv->listBorderLink.size() > 0)
					{
						//OID行が追加済み かつ 追加済みのオブジェクトに追記
						Polyline addedGeo = _csv->listBorderLink.back().shape.GetValue();
						isCheched &= addedGeo.AddPoint(lon, lat, height);
						_csv->listBorderLink.back().shape.SetValue(addedGeo);
					}
					else
					{
						LogUtil::printError(52,  "インポートエラー\t属性定義行がエラーのためインポートをSkipしました");
						return false;
					}
				}
				else if(tokenCount > SurveyCSV::GEOMETRY_ROW_COUNT)
				{
					isCheched &= geometry.AddPoint(lon, lat, height);
				}
			}
		}
		else if(row_count == csv.objectid.GetIndex())
		{
			isCheched &= csv.objectid.SetText(s);
			oid = csv.objectid.GetValue();
		}
		else if(row_count == csv.from_node_id.GetIndex())
			isCheched &= csv.from_node_id.SetText(s);
		else if(row_count == csv.to_node_id.GetIndex())
			isCheched &= csv.to_node_id.SetText(s);
		else if(row_count == csv.imaginary_boder_f.GetIndex())
			isCheched &= csv.imaginary_boder_f.SetText(s);
		else if(row_count == csv.update_c.GetIndex())
			isCheched &= csv.update_c.SetText(s);
		else
		{
			LogUtil::printError(6, "列数エラー","列数:" + std::to_string(row_count), "列数上限:" + std::to_string(csv.ROW_COUNT));
			isCheched = false;
			break;
		}

		row_count++;
	}

	// リストに追加
	if(tokenCount > SurveyCSV::GEOMETRY_ROW_COUNT)
	{
		if(isCheched)
		{
			_hasError = false;
			isCheched &= csv.shape.SetValue(geometry);
			_csv->listBorderLink.push_back(csv);
		}
		else
		{
			_hasError = true;
			LogUtil::printError(52,  "インポートエラー", "エラーのためインポートをSkipしました","OID:" + std::to_string(oid));
		}
	}

	return isCheched;
}

bool SurveyCSVReader::ReadBorderPoint(const boost::tokenizer<boost::char_separator<char>>& tokens)
{
	if(!_csv)
	{
		return false;
	}

	long tokenCount = getTokenCount(tokens);
	
	//列数チェック
	if(tokenCount != SurveyCSV::BorderPoint::ROW_COUNT)
	{
		LogUtil::printError(6, "列数エラー","列数:" + std::to_string(tokenCount), "列数上限:" + std::to_string(SurveyCSV::BorderPoint::ROW_COUNT));
		return false;
	}
	
	SurveyCSV::BorderPoint csv;
	long row_count = 1;
	bool isCheched = true;
	Point geometry;
	long long oid;

	std::string lon, lat, height;
	// 要素の格納
	BOOST_FOREACH(std::string s, tokens)
	{
		if(row_count <= SurveyCSV::GEOMETRY_ROW_COUNT)
		{
			if(row_count == Location::INDEX_LON)
				lon = s;
			else if(row_count == Location::INDEX_LAT)
				lat = s;
			else if(row_count == Location::INDEX_HEIGHT)
			{
				height = s;
				isCheched &= geometry.AddPoint(lon, lat, height);
				isCheched &= csv.shape.SetValue(geometry);
			}
		}
		else if(row_count == csv.objectid.GetIndex())
		{
			isCheched &= csv.objectid.SetText(s);
			oid = csv.objectid.GetValue();
		}
		else if(row_count == csv.node_id.GetIndex())
			isCheched &= csv.node_id.SetText(s);
		else if(row_count == csv.point_class_c.GetIndex())
			isCheched &= csv.point_class_c.SetText(s);
		else if(row_count == csv.border_link_id.GetIndex())
			isCheched &= csv.border_link_id.SetText(s);
		else if(row_count == csv.sequence.GetIndex())
			isCheched &= csv.sequence.SetText(s);
		else if(row_count == csv.lastnode_f.GetIndex())
			isCheched &= csv.lastnode_f.SetText(s);
		else if(row_count == csv.update_c.GetIndex())
			isCheched &= csv.update_c.SetText(s);
		else
		{
			LogUtil::printError(6, "列数エラー","列数:" + std::to_string(row_count), "列数上限:" + std::to_string(csv.ROW_COUNT));
			isCheched = false;
			break;
		}

		row_count++;
	}

	// リストに追加
	if(isCheched)
	{
		_csv->listBorderPoint.push_back(csv);
	}
	else
	{
		LogUtil::printError(52,  "インポートエラー", "エラーのためインポートをSkipしました","OID:" + std::to_string(oid));
	}

	return isCheched;
}

bool SurveyCSVReader::ReadRelBorderLink(const boost::tokenizer<boost::char_separator<char>>& tokens)
{
	if(!_csv)
	{
		return false;
	}

	long tokenCount = getTokenCount(tokens);
	
	//列数チェック
	if(tokenCount != SurveyCSV::RelBorderLink::ROW_COUNT)
	{
		LogUtil::printError(6, "列数エラー","列数:" + std::to_string(tokenCount), "列数上限:" + std::to_string(SurveyCSV::RelBorderLink::ROW_COUNT));
		return false;
	}
	
	SurveyCSV::RelBorderLink csv;
	long row_count = 1;
	bool isCheched = true;
	long long oid;

	std::string lon, lat, height;
	// 要素の格納
	BOOST_FOREACH(std::string s, tokens)
	{
		if(row_count == csv.objectid.GetIndex())
		{
			isCheched &= csv.objectid.SetText(s);
			oid = csv.objectid.GetValue();
		}
		else if(row_count == csv.lane_link_id.GetIndex())
			isCheched &= csv.lane_link_id.SetText(s);
		else if(row_count == csv.lr_f.GetIndex())
			isCheched &= csv.lr_f.SetText(s);
		else if(row_count == csv.border_link_id.GetIndex())
			isCheched &= csv.border_link_id.SetText(s);
		else if(row_count == csv.border_link_direction_c.GetIndex())
			isCheched &= csv.border_link_direction_c.SetText(s);
		else if(row_count == csv.sequence.GetIndex())
			isCheched &= csv.sequence.SetText(s);
		else if(row_count == csv.update_c.GetIndex())
			isCheched &= csv.update_c.SetText(s);
		else
		{
			LogUtil::printError(6, "列数エラー","列数:" + std::to_string(row_count), "列数上限:" + std::to_string(csv.ROW_COUNT));
			isCheched = false;
			break;
		}

		row_count++;
	}

	// リストに追加
	if(isCheched)
	{
		_csv->listRelBorderLink.push_back(csv);
	}
	else
	{
		LogUtil::printError(52,  "インポートエラー", "エラーのためインポートをSkipしました","OID:" + std::to_string(oid));
	}

	return isCheched;
}

bool SurveyCSVReader::ReadSignalPoint(const boost::tokenizer<boost::char_separator<char>>& tokens)
{
	if(!_csv)
	{
		return false;
	}

	long tokenCount = getTokenCount(tokens);
	
	//列数チェック
	if(tokenCount != SurveyCSV::SignalPoint::ROW_COUNT)
	{
		LogUtil::printError(6, "列数エラー","列数:" + std::to_string(tokenCount), "列数上限:" + std::to_string(SurveyCSV::SignalPoint::ROW_COUNT));
		return false;
	}
	
	SurveyCSV::SignalPoint csv;
	long row_count = 1;
	bool isCheched = true;
	Point geometry;
	long long oid;

	std::string lon, lat, height;
	// 要素の格納
	BOOST_FOREACH(std::string s, tokens)
	{
		if(row_count <= SurveyCSV::GEOMETRY_ROW_COUNT)
		{
			if(row_count == Location::INDEX_LON)
				lon = s;
			else if(row_count == Location::INDEX_LAT)
				lat = s;
			else if(row_count == Location::INDEX_HEIGHT)
			{
				height = s;
				isCheched &= geometry.AddPoint(lon, lat, height);
				isCheched &= csv.shape.SetValue(geometry);
			}
		}
		else if(row_count == csv.objectid.GetIndex())
		{
			isCheched &= csv.objectid.SetText(s);
			oid = csv.objectid.GetValue();
		}
		else if(row_count == csv.horizontal_angle.GetIndex())
			isCheched &= csv.horizontal_angle.SetText(s);
		else if(row_count == csv.signal_mold_c.GetIndex())
			isCheched &= csv.signal_mold_c.SetText(s);
		else if(row_count == csv.lamp_count.GetIndex())
			isCheched &= csv.lamp_count.SetText(s);
		else if(row_count == csv.arrow_f.GetIndex())
			isCheched &= csv.arrow_f.SetText(s);
		else if(row_count == csv.warning_f.GetIndex())
			isCheched &= csv.warning_f.SetText(s);
		else if(row_count == csv.independent_f.GetIndex())
			isCheched &= csv.independent_f.SetText(s);
		else if(row_count == csv.update_c.GetIndex())
			isCheched &= csv.update_c.SetText(s);
		else
		{
			LogUtil::printError(6, "列数エラー","列数:" + std::to_string(row_count), "列数上限:" + std::to_string(csv.ROW_COUNT));
			isCheched = false;
			break;
		}

		row_count++;
	}

	// リストに追加
	if(isCheched)
	{
		_csv->listSignalPoint.push_back(csv);
	}
	else
	{
		LogUtil::printError(52,  "インポートエラー", "エラーのためインポートをSkipしました","OID:" + std::to_string(oid));
	}

	return isCheched;
}

bool SurveyCSVReader::ReadRelSignalPoint(const boost::tokenizer<boost::char_separator<char>>& tokens)
{
	if(!_csv)
	{
		return false;
	}

	long tokenCount = getTokenCount(tokens);
	
	//列数チェック
	if(tokenCount != SurveyCSV::RelSignalPoint::ROW_COUNT)
	{
		LogUtil::printError(6, "列数エラー","列数:" + std::to_string(tokenCount), "列数上限:" + std::to_string(SurveyCSV::RelSignalPoint::ROW_COUNT));
		return false;
	}
	
	SurveyCSV::RelSignalPoint csv;
	long row_count = 1;
	bool isCheched = true;
	long long oid;

	std::string lon, lat, height;
	// 要素の格納
	BOOST_FOREACH(std::string s, tokens)
	{
		if(row_count == csv.objectid.GetIndex())
		{
			isCheched &= csv.objectid.SetText(s);
			oid = csv.objectid.GetValue();
		}
		else if(row_count == csv.lane_node_id.GetIndex())
			isCheched &= csv.lane_node_id.SetText(s);
		else if(row_count == csv.signal_point_id.GetIndex())
			isCheched &= csv.signal_point_id.SetText(s);
		else if(row_count == csv.update_c.GetIndex())
			isCheched &= csv.update_c.SetText(s);
		else
		{
			LogUtil::printError(6, "列数エラー","列数:" + std::to_string(row_count), "列数上限:" + std::to_string(csv.ROW_COUNT));
			isCheched = false;
			break;
		}

		row_count++;
	}

	// リストに追加
	if(isCheched)
	{
		_csv->listRelSignalPoint.push_back(csv);
	}
	else
	{
		LogUtil::printError(52,  "インポートエラー", "エラーのためインポートをSkipしました","OID:" + std::to_string(oid));
	}

	return isCheched;
}

bool SurveyCSVReader::ReadRoadSignPoint(const boost::tokenizer<boost::char_separator<char>>& tokens)
{
	if(!_csv)
	{
		return false;
	}

	long tokenCount = getTokenCount(tokens);
	
	//列数チェック
	if(tokenCount != SurveyCSV::RoadSignPoint::ROW_COUNT)
	{
		LogUtil::printError(6, "列数エラー","列数:" + std::to_string(tokenCount), "列数上限:" + std::to_string(SurveyCSV::RoadSignPoint::ROW_COUNT));
		return false;
	}
	
	SurveyCSV::RoadSignPoint csv;
	long row_count = 1;
	bool isCheched = true;
	Point geometry;
	long long oid;

	std::string lon, lat, height;
	// 要素の格納
	BOOST_FOREACH(std::string s, tokens)
	{
		if(row_count <= SurveyCSV::GEOMETRY_ROW_COUNT)
		{
			if(row_count == Location::INDEX_LON)
				lon = s;
			else if(row_count == Location::INDEX_LAT)
				lat = s;
			else if(row_count == Location::INDEX_HEIGHT)
			{
				height = s;
				isCheched &= geometry.AddPoint(lon, lat, height);
				isCheched &= csv.shape.SetValue(geometry);
			}
		}
		else if(row_count == csv.objectid.GetIndex())
		{
			isCheched &= csv.objectid.SetText(s);
			oid = csv.objectid.GetValue();
		}
		else if(row_count == csv.horizontal_angle.GetIndex())
			isCheched &= csv.horizontal_angle.SetText(s);
		else if(row_count == csv.roadsign_c.GetIndex())
			isCheched &= csv.roadsign_c.SetText(s);
		else if(row_count == csv.variable_f.GetIndex())
			isCheched &= csv.variable_f.SetText(s);
		else if(row_count == csv.update_c.GetIndex())
			isCheched &= csv.update_c.SetText(s);
		else
		{
			LogUtil::printError(6, "列数エラー","列数:" + std::to_string(row_count), "列数上限:" + std::to_string(csv.ROW_COUNT));
			isCheched = false;
			break;
		}

		row_count++;
	}

	// リストに追加
	if(isCheched)
	{
		_csv->listRoadSignPoint.push_back(csv);
	}
	else
	{
		LogUtil::printError(52,  "インポートエラー", "エラーのためインポートをSkipしました","OID:" + std::to_string(oid));
	}

	return isCheched;
}

bool SurveyCSVReader::ReadRelRoadSignPoint(const boost::tokenizer<boost::char_separator<char>>& tokens)
{
	if(!_csv)
	{
		return false;
	}

	long tokenCount = getTokenCount(tokens);
	
	//列数チェック
	if(tokenCount != SurveyCSV::RelRoadSignPoint::ROW_COUNT)
	{
		LogUtil::printError(6, "列数エラー","列数:" + std::to_string(tokenCount), "列数上限:" + std::to_string(SurveyCSV::RelRoadSignPoint::ROW_COUNT));
		return false;
	}
	
	SurveyCSV::RelRoadSignPoint csv;
	long row_count = 1;
	bool isCheched = true;
	long long oid;

	std::string lon, lat, height;
	// 要素の格納
	BOOST_FOREACH(std::string s, tokens)
	{
		if(row_count == csv.objectid.GetIndex())
		{
			isCheched &= csv.objectid.SetText(s);
			oid = csv.objectid.GetValue();
		}
		else if(row_count == csv.lane_node_id.GetIndex())
			isCheched &= csv.lane_node_id.SetText(s);
		else if(row_count == csv.roadsign_point_id.GetIndex())
			isCheched &= csv.roadsign_point_id.SetText(s);
		else if(row_count == csv.update_c.GetIndex())
			isCheched &= csv.update_c.SetText(s);
		else
		{
			LogUtil::printError(6, "列数エラー","列数:" + std::to_string(row_count), "列数上限:" + std::to_string(csv.ROW_COUNT));
			isCheched = false;
			break;
		}

		row_count++;
	}

	// リストに追加
	if(isCheched)
	{
		_csv->listRelRoadSignPoint.push_back(csv);
	}
	else
	{
		LogUtil::printError(52,  "インポートエラー", "エラーのためインポートをSkipしました","OID:" + std::to_string(oid));
	}

	return isCheched;
}

bool SurveyCSVReader::ReadCrossWalkLine(const boost::tokenizer<boost::char_separator<char>>& tokens)
{
	if(!_csv)
	{
		return false;
	}

	long tokenCount = getTokenCount(tokens);
	
	//列数チェック
	if(tokenCount != SurveyCSV::GEOMETRY_ROW_COUNT && tokenCount != SurveyCSV::CrossWalkLine::ROW_COUNT)
	{
		LogUtil::printError(6, "列数エラー","列数:" + std::to_string(tokenCount), "列数上限:" + std::to_string(SurveyCSV::CrossWalkLine::ROW_COUNT));
		_hasError = true;
		return false;
	}
	
	SurveyCSV::CrossWalkLine csv;
	long row_count = 1;
	bool isCheched = true;
	Polyline geometry;
	long long oid;

	std::string lon, lat, height;
	// 要素の格納
	BOOST_FOREACH(std::string s, tokens)
	{
		if(row_count <= SurveyCSV::GEOMETRY_ROW_COUNT)
		{
			if(row_count == Location::INDEX_LON)
				lon = s;
			else if(row_count == Location::INDEX_LAT)
				lat = s;
			else if(row_count == Location::INDEX_HEIGHT)
				height = s;

			if(row_count == SurveyCSV::GEOMETRY_ROW_COUNT)
			{
				if(tokenCount == SurveyCSV::GEOMETRY_ROW_COUNT)
				{
					if(!_hasError && _csv->listCrossWalkLine.size() > 0)
					{
						//OID行が追加済み かつ 追加済みのオブジェクトに追記
						Polyline addedGeo = _csv->listCrossWalkLine.back().shape.GetValue();
						isCheched &= addedGeo.AddPoint(lon, lat, height);
						_csv->listCrossWalkLine.back().shape.SetValue(addedGeo);
					}
					else
					{
						LogUtil::printError(52,  "インポートエラー\t属性定義行がエラーのためインポートをSkipしました");
						return false;
					}
				}
				else if(tokenCount > SurveyCSV::GEOMETRY_ROW_COUNT)
				{
					isCheched &= geometry.AddPoint(lon, lat, height);
				}
			}
		}
		else if(row_count == csv.objectid.GetIndex())
		{
			isCheched &= csv.objectid.SetText(s);
			oid = csv.objectid.GetValue();
		}
		else if(row_count == csv.width.GetIndex())
			isCheched &= csv.width.SetText(s);
		else if(row_count == csv.update_c.GetIndex())
			isCheched &= csv.update_c.SetText(s);
		else
		{
			LogUtil::printError(6, "列数エラー","列数:" + std::to_string(row_count), "列数上限:" + std::to_string(csv.ROW_COUNT));
			isCheched = false;
			break;
		}

		row_count++;
	}

	// リストに追加
	if(tokenCount > SurveyCSV::GEOMETRY_ROW_COUNT)
	{
		if(isCheched)
		{
			_hasError = false;
			isCheched &= csv.shape.SetValue(geometry);
			_csv->listCrossWalkLine.push_back(csv);
		}
		else
		{
			_hasError = true;
			LogUtil::printError(52,  "インポートエラー", "エラーのためインポートをSkipしました","OID:" + std::to_string(oid));
		}
	}

	return isCheched;
}

bool SurveyCSVReader::ReadRelCrossWalkLine(const boost::tokenizer<boost::char_separator<char>>& tokens)
{
	if(!_csv)
	{
		return false;
	}

	long tokenCount = getTokenCount(tokens);
	
	//列数チェック
	if(tokenCount != SurveyCSV::RelCrossWalkLine::ROW_COUNT)
	{
		LogUtil::printError(6, "列数エラー","列数:" + std::to_string(tokenCount), "列数上限:" + std::to_string(SurveyCSV::RelCrossWalkLine::ROW_COUNT));
		return false;
	}
	
	SurveyCSV::RelCrossWalkLine csv;
	long row_count = 1;
	bool isCheched = true;
	long long oid;

	std::string lon, lat, height;
	// 要素の格納
	BOOST_FOREACH(std::string s, tokens)
	{
		if(row_count == csv.objectid.GetIndex())
		{
			isCheched &= csv.objectid.SetText(s);
			oid = csv.objectid.GetValue();
		}
		else if(row_count == csv.lane_node_id.GetIndex())
			isCheched &= csv.lane_node_id.SetText(s);
		else if(row_count == csv.crosswalk_line_id.GetIndex())
			isCheched &= csv.crosswalk_line_id.SetText(s);
		else if(row_count == csv.update_c.GetIndex())
			isCheched &= csv.update_c.SetText(s);
		else
		{
			LogUtil::printError(6, "列数エラー","列数:" + std::to_string(row_count), "列数上限:" + std::to_string(csv.ROW_COUNT));
			isCheched = false;
			break;
		}

		row_count++;
	}

	// リストに追加
	if(isCheched)
	{
		_csv->listRelCrossWalkLine.push_back(csv);
	}
	else
	{
		LogUtil::printError(52,  "インポートエラー", "エラーのためインポートをSkipしました","OID:" + std::to_string(oid));
	}

	return isCheched;
}

bool SurveyCSVReader::ReadVirtualCompartLink	(const boost::tokenizer<boost::char_separator<char>>& tokens)
{
	if(!_csv)
	{
		return false;
	}

	long tokenCount = getTokenCount(tokens);
	
	//列数チェック
	if(tokenCount != SurveyCSV::GEOMETRY_ROW_COUNT && tokenCount != SurveyCSV::VirtualCompartLink::ROW_COUNT)
	{
		LogUtil::printError(6, "列数エラー","列数:" + std::to_string(tokenCount), "列数上限:" + std::to_string(SurveyCSV::VirtualCompartLink::ROW_COUNT));
		_hasError = true;
		return false;
	}
	
	SurveyCSV::VirtualCompartLink csv;
	long row_count = 1;
	bool isCheched = true;
	Polyline geometry;
	long long oid;

	std::string lon, lat, height;
	// 要素の格納
	BOOST_FOREACH(std::string s, tokens)
	{
		if(row_count <= SurveyCSV::GEOMETRY_ROW_COUNT)
		{
			if(row_count == Location::INDEX_LON)
				lon = s;
			else if(row_count == Location::INDEX_LAT)
				lat = s;
			else if(row_count == Location::INDEX_HEIGHT)
				height = s;

			if(row_count == SurveyCSV::GEOMETRY_ROW_COUNT)
			{
				if(tokenCount == SurveyCSV::GEOMETRY_ROW_COUNT)
				{
					if(!_hasError && _csv->listVirtualCompartLink.size() > 0)
					{
						//OID行が追加済み かつ 追加済みのオブジェクトに追記
						Polyline addedGeo = _csv->listVirtualCompartLink.back().shape.GetValue();
						isCheched &= addedGeo.AddPoint(lon, lat, height);
						_csv->listVirtualCompartLink.back().shape.SetValue(addedGeo);
					}
					else
					{
						LogUtil::printError(52,  "インポートエラー\t属性定義行がエラーのためインポートをSkipしました");
						return false;
					}
				}
				else if(tokenCount > SurveyCSV::GEOMETRY_ROW_COUNT)
				{
					isCheched &= geometry.AddPoint(lon, lat, height);
				}
			}
		}
		else if(row_count == csv.objectid.GetIndex())
		{
			isCheched &= csv.objectid.SetText(s);
			oid = csv.objectid.GetValue();
		}
		else if(row_count == csv.from_node_id.GetIndex())
			isCheched &= csv.from_node_id.SetText(s);
		else if(row_count == csv.to_node_id.GetIndex())
			isCheched &= csv.to_node_id.SetText(s);
		else if(row_count == csv.compart_line_c.GetIndex())
			isCheched &= csv.compart_line_c.SetText(s);
		else if(row_count == csv.compart_style_c.GetIndex())
			isCheched &= csv.compart_style_c.SetText(s);
		else if(row_count == csv.slowdown_label_c.GetIndex())
			isCheched &= csv.slowdown_label_c.SetText(s);
		else if(row_count == csv.installation_f.GetIndex())
			isCheched &= csv.installation_f.SetText(s);
		else
		{
			LogUtil::printError(6, "列数エラー","列数:" + std::to_string(row_count), "列数上限:" + std::to_string(csv.ROW_COUNT));
			isCheched = false;
			break;
		}

		row_count++;
	}

	// リストに追加
	if(tokenCount > SurveyCSV::GEOMETRY_ROW_COUNT)
	{
		if(isCheched)
		{
			_hasError = false;
			isCheched &= csv.shape.SetValue(geometry);
			_csv->listVirtualCompartLink.push_back(csv);
		}
		else
		{
			_hasError = true;
			LogUtil::printError(52,  "インポートエラー", "エラーのためインポートをSkipしました","OID:" + std::to_string(oid));
		}
	}

	return isCheched;
}

bool SurveyCSVReader::ReadVirtualCompartPoint	(const boost::tokenizer<boost::char_separator<char>>& tokens)
{
	//使わないけど読み込んでおく
	if(!_csv)
	{
		return false;
	}

	long tokenCount = getTokenCount(tokens);
	
	//列数チェック
	if(tokenCount != SurveyCSV::VirtualCompartPoint::ROW_COUNT)
	{
		LogUtil::printError(6, "列数エラー","列数:" + std::to_string(tokenCount), "列数上限:" + std::to_string(SurveyCSV::VirtualCompartPoint::ROW_COUNT));
		return false;
	}
	
	SurveyCSV::VirtualCompartPoint csv;
	long row_count = 1;
	bool isCheched = true;
	Point geometry;
	long long oid;

	std::string lon, lat, height;
	// 要素の格納
	BOOST_FOREACH(std::string s, tokens)
	{
		if(row_count <= SurveyCSV::GEOMETRY_ROW_COUNT)
		{
			if(row_count == Location::INDEX_LON)
				lon = s;
			else if(row_count == Location::INDEX_LAT)
				lat = s;
			else if(row_count == Location::INDEX_HEIGHT)
			{
				height = s;
				isCheched &= geometry.AddPoint(lon, lat, height);
				isCheched &= csv.shape.SetValue(geometry);
			}
		}
		else if(row_count == csv.objectid.GetIndex())
		{
			isCheched &= csv.objectid.SetText(s);
			oid = csv.objectid.GetValue();
		}
		else if(row_count == csv.node_id.GetIndex())
			isCheched &= csv.node_id.SetText(s);
		else if(row_count == csv.point_class_c.GetIndex())
			isCheched &= csv.point_class_c.SetText(s);
		else if(row_count == csv.v_compart_link_id.GetIndex())
			isCheched &= csv.v_compart_link_id.SetText(s);
		else if(row_count == csv.sequence.GetIndex())
			isCheched &= csv.sequence.SetText(s);
		else if(row_count == csv.lastnode_f.GetIndex())
			isCheched &= csv.lastnode_f.SetText(s);
		else if(row_count == csv.width.GetIndex())
			isCheched &= csv.width.SetText(s);
		else
		{
			LogUtil::printError(6, "列数エラー","列数:" + std::to_string(row_count), "列数上限:" + std::to_string(csv.ROW_COUNT));
			isCheched = false;
			break;
		}

		row_count++;
	}

	// リストに追加
	if(isCheched)
	{
		_csv->listVirtualCompartPoint.push_back(csv);
	}
	else
	{
		LogUtil::printError(52,  "インポートエラー", "エラーのためインポートをSkipしました","OID:" + std::to_string(oid));
	}

	return isCheched;
}

bool SurveyCSVReader::ReadRelVirtualCompartLink	(const boost::tokenizer<boost::char_separator<char>>& tokens)
{
	//使わないけど読み込んでおく
	if(!_csv)
	{
		return false;
	}

	long tokenCount = getTokenCount(tokens);
	
	//列数チェック
	if(tokenCount != SurveyCSV::RelVirtualCompartLink::ROW_COUNT)
	{
		LogUtil::printError(6, "列数エラー","列数:" + std::to_string(tokenCount), "列数上限:" + std::to_string(SurveyCSV::RelVirtualCompartLink::ROW_COUNT));
		return false;
	}
	
	SurveyCSV::RelVirtualCompartLink csv;
	long row_count = 1;
	bool isCheched = true;
	long long oid;

	std::string lon, lat, height;
	// 要素の格納
	BOOST_FOREACH(std::string s, tokens)
	{
		if(row_count == csv.objectid.GetIndex())
		{
			isCheched &= csv.objectid.SetText(s);
			oid = csv.objectid.GetValue();
		}
		else if(row_count == csv.lane_link_id.GetIndex())
			isCheched &= csv.lane_link_id.SetText(s);
		else if(row_count == csv.lr_f.GetIndex())
			isCheched &= csv.lr_f.SetText(s);
		else if(row_count == csv.v_compart_link_id.GetIndex())
			isCheched &= csv.v_compart_link_id.SetText(s);
		else if(row_count == csv.v_compart_link_direction_c.GetIndex())
			isCheched &= csv.v_compart_link_direction_c.SetText(s);
		else if(row_count == csv.sequence.GetIndex())
			isCheched &= csv.sequence.SetText(s);
		else
		{
			LogUtil::printError(6, "列数エラー","列数:" + std::to_string(row_count), "列数上限:" + std::to_string(csv.ROW_COUNT));
			isCheched = false;
			break;
		}

		row_count++;
	}

	// リストに追加
	if(isCheched)
	{
		_csv->listRelVirtualCompartLink.push_back(csv);
	}
	else
	{
		LogUtil::printError(52,  "インポートエラー", "エラーのためインポートをSkipしました","OID:" + std::to_string(oid));
	}

	return isCheched;
}

bool SurveyCSVReader::ReadCoveringArea			(const boost::tokenizer<boost::char_separator<char>>& tokens)
{
	if(!_csv)
	{
		return false;
	}

	long tokenCount = getTokenCount(tokens);
	
	//列数チェック
	if(tokenCount != SurveyCSV::GEOMETRY_ROW_COUNT && tokenCount != SurveyCSV::CoveringArea::ROW_COUNT)
	{
		LogUtil::printError(6, "列数エラー","列数:" + std::to_string(tokenCount), "列数上限:" + std::to_string(SurveyCSV::CoveringArea::ROW_COUNT));
		_hasError = true;
		return false;
	}
	
	SurveyCSV::CoveringArea csv;
	long row_count = 1;
	bool isCheched = true;
	Polygon geometry;
	long long oid;

	std::string lon, lat, height;
	// 要素の格納
	BOOST_FOREACH(std::string s, tokens)
	{
		if(row_count <= SurveyCSV::GEOMETRY_ROW_COUNT)
		{
			if(row_count == Location::INDEX_LON)
				lon = s;
			else if(row_count == Location::INDEX_LAT)
				lat = s;
			else if(row_count == Location::INDEX_HEIGHT)
				height = s;

			if(row_count == SurveyCSV::GEOMETRY_ROW_COUNT)
			{
				if(tokenCount == SurveyCSV::GEOMETRY_ROW_COUNT)
				{
					if(!_hasError && _csv->listCoveringArea.size() > 0)
					{
						//OID行が追加済み かつ 追加済みのオブジェクトに追記
						Polygon addedGeo = _csv->listCoveringArea.back().shape.GetValue();
						isCheched &= addedGeo.AddPoint(lon, lat, height);
						_csv->listCoveringArea.back().shape.SetValue(addedGeo);
					}
					else
					{
						LogUtil::printError(52,  "インポートエラー\t属性定義行がエラーのためインポートをSkipしました");
						return false;
					}
				}
				else if(tokenCount > SurveyCSV::GEOMETRY_ROW_COUNT)
				{
					isCheched &= geometry.AddPoint(lon, lat, height);
				}
			}
		}
		else if(row_count == csv.objectid.GetIndex())
		{
			isCheched &= csv.objectid.SetText(s);
			oid = csv.objectid.GetValue();
		}
		else if(row_count == csv.update_c.GetIndex())
			isCheched &= csv.update_c.SetText(s);
		else
		{
			LogUtil::printError(6, "列数エラー","列数:" + std::to_string(row_count), "列数上限:" + std::to_string(csv.ROW_COUNT));
			isCheched = false;
			break;
		}

		row_count++;
	}

	// リストに追加
	if(tokenCount > SurveyCSV::GEOMETRY_ROW_COUNT)
	{
		if(isCheched)
		{
			_hasError = false;
			isCheched &= csv.shape.SetValue(geometry);
			_csv->listCoveringArea.push_back(csv);
		}
		else
		{
			_hasError = true;
			LogUtil::printError(52,  "インポートエラー", "エラーのためインポートをSkipしました","OID:" + std::to_string(oid));
		}
	}

	return isCheched;
}

bool SurveyCSVReader::ReadSpatialRef(const boost::tokenizer<boost::char_separator<char>>& tokens)
{
	if(!_csv)
	{
		return false;
	}

	long tokenCount = getTokenCount(tokens);
	
	//列数チェック
	if(tokenCount != SurveyCSV::SpatialRef::ROW_COUNT)
	{
		LogUtil::printError(6, "列数エラー","列数:" + std::to_string(tokenCount), "列数上限:" + std::to_string(SurveyCSV::SpatialRef::ROW_COUNT));
		return false;
	}
	
	SurveyCSV::SpatialRef csv;
	long row_count = 1;
	bool isCheched = true;

	std::string lon, lat, height;
	// 要素の格納
	BOOST_FOREACH(std::string s, tokens)
	{
		if(row_count == csv.srid.GetIndex())
			isCheched &= csv.srid.SetText(s);
		else if(row_count == csv.name.GetIndex())
			isCheched &= csv.name.SetText(s);
		else
		{
			LogUtil::printError(6, "列数エラー","列数:" + std::to_string(row_count), "列数上限:" + std::to_string(csv.ROW_COUNT));
			isCheched = false;
			break;
		}

		row_count++;
	}

	// リストに追加
	if(isCheched)
	{
		_csv->listSpatialRef.push_back(csv);
	}
	else
	{
		LogUtil::printError(52,  "インポートエラー\tエラーのためインポートをSkipしました");
	}

	return isCheched;
}

bool SurveyCSVReader::ReadVersion(const boost::tokenizer<boost::char_separator<char>>& tokens)
{
	if(!_csv)
	{
		return false;
	}

	long tokenCount = getTokenCount(tokens);
	
	//列数チェック
	if(tokenCount != SurveyCSV::Version::ROW_COUNT)
	{
		LogUtil::printError(6, "列数エラー","列数:" + std::to_string(tokenCount), "列数上限:" + std::to_string(SurveyCSV::Version::ROW_COUNT));
		return false;
	}
	
	SurveyCSV::Version csv;
	long row_count = 1;
	bool isCheched = true;

	std::string lon, lat, height;
	// 要素の格納
	BOOST_FOREACH(std::string s, tokens)
	{
		if(row_count == csv.data_def.GetIndex())
			isCheched &= csv.data_def.SetText(s);
		else if(row_count == csv.format_def.GetIndex())
			isCheched &= csv.format_def.SetText(s);
		else
		{
			LogUtil::printError(6, "列数エラー","列数:" + std::to_string(row_count), "列数上限:" + std::to_string(csv.ROW_COUNT));
			isCheched = false;
			break;
		}

		row_count++;
	}

	// リストに追加
	if(isCheched)
	{
		_csv->listVersion.push_back(csv);
	}
	else
	{
		LogUtil::printError(52,  "インポートエラー\tエラーのためインポートをSkipしました");
	}

	return isCheched;
}

SurveyCSVPtr SurveyCSVReader::GetSurveyCSVPtr()
{
	return _csv;
}