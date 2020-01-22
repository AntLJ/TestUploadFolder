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

#include <boost/tokenizer.hpp>

#include "DataAccessor.h"
#include "SurveyCSV.h"

namespace adam_v2
{
/**
 * @brief	納入データ読み込みクラス
 */
class SurveyCSVReader : public DataAccessor
{
public:
	SurveyCSVReader(void);
	~SurveyCSVReader(void);

	/**
	 * @brief	データ読み込み
	 * @param[in]	fileName	CSVファイル名
	 * @retval	true:	成功
	 * @retval	false:	失敗
	 */
	bool Read(const uh::tstring& fileName);
	
	/**
	 * @brief	データ書き込み
	 * @note	未使用
	 * @param[in]	tableName	ADAMテーブル名
	 * @retval	true:	成功
	 * @retval	false:	失敗
	 */
	bool Write(const uh::tstring& tableName)
	{
		return false;
	}

	/**
	 * @brief	データクリア
	 */
	void Clear();

	/**
	 * @brief	読み込み済みデータの取得
	 * @note	書式エラーの入力はSKIPされている
	 * @return	読み込み済みデータ
	 */
	SurveyCSVPtr GetSurveyCSVPtr();

	/**
	 * @brief	削除レコードスキップフラグの設定
	 * @param[in]   isSkip スキップするか否か
	 */
	void SetSkipDeleteRecord( bool isSkip ) { _skipDeleteRecord = isSkip; };

	/**
	 * @brief	バージョンチェック
	 * @retval	true:	成功
	 * @retval	false:	失敗
	 */
	bool CheckCsvVersion();

private:
	/**
	 * @brief	データ読み込み
	 * @param[in]	dirPath		ディレクトリパス
	 * @param[in]	fileName	ファイル名
	 * @param[in]	func		関数ポインタ
	 * @retval	true:	成功
	 * @retval	false:	失敗
	 */
	bool ReadCSV(
		const uh::tstring& dirPath,
		const uh::tstring& fileName,
		bool (SurveyCSVReader::*func)(const boost::tokenizer<boost::char_separator<char>>&)
		);

	/**
	 * @brief	トークン数の取得
	 * @param[in]	tokens	デリミタで分割した要素
	 * @return	トークン数
	 */
	long getTokenCount(const boost::tokenizer<boost::char_separator<char>>& token);

	/**
	 * @brief	ジオメトリの整合性チェック
	 * @note	チェックNGのオブジェクトは削除する
	 * @retval	true:	異常なし
	 * @retval	false:	異常あり
	 */
	bool CheckGeometryIntegrity();

	/**
	 * @brief	OID並び順チェック
	 * @retval	true:	異常なし
	 * @retval	false:	異常あり
	 */
	bool CheckSortByOID();

	/**
	 * @brief	削除レコードか判定
	 *
	 * @param[in]	fileName	ファイル名
	 * @param[in]	tokens		レコード
	 * @param[in]	index		更新コードのインデックス
	 * @retval		true		削除レコードである
	 * @retval		false		削除レコードでない
	 */
	bool IsDeleteRecord( const uh::tstring& fileName, const boost::tokenizer<boost::char_separator<char>>& tokens, long index );

	/**
	 * @brief	更新コードのインデックス取得
	 *
	 * @param[in]	fileName	ファイル名
	 * @return	インデックス
	 */
	long GetIndexUpdateC( const uh::tstring& fileName );

	/**
	 * @brief	ファイルごとの読み込み処理
	 *
	 * @param[in]	tokens	デリミタで分割した要素
	 * @retval	true:	成功
	 * @retval	false:	失敗
	 */
	bool ReadLaneLink				(const boost::tokenizer<boost::char_separator<char>>& tokens);
	bool ReadLanePoint				(const boost::tokenizer<boost::char_separator<char>>& tokens);
	bool ReadRelLaneLink			(const boost::tokenizer<boost::char_separator<char>>& tokens);
	bool ReadCompartLink			(const boost::tokenizer<boost::char_separator<char>>& tokens);
	bool ReadCompartPoint			(const boost::tokenizer<boost::char_separator<char>>& tokens);
	bool ReadRelCompartLink			(const boost::tokenizer<boost::char_separator<char>>& tokens);
	bool ReadBorderLink				(const boost::tokenizer<boost::char_separator<char>>& tokens);
	bool ReadBorderPoint			(const boost::tokenizer<boost::char_separator<char>>& tokens);
	bool ReadRelBorderLink			(const boost::tokenizer<boost::char_separator<char>>& tokens);
	bool ReadSignalPoint			(const boost::tokenizer<boost::char_separator<char>>& tokens);
	bool ReadRelSignalPoint			(const boost::tokenizer<boost::char_separator<char>>& tokens);
	bool ReadRoadSignPoint			(const boost::tokenizer<boost::char_separator<char>>& tokens);
	bool ReadRelRoadSignPoint		(const boost::tokenizer<boost::char_separator<char>>& tokens);
	bool ReadCrossWalkLine			(const boost::tokenizer<boost::char_separator<char>>& tokens);
	bool ReadRelCrossWalkLine		(const boost::tokenizer<boost::char_separator<char>>& tokens);
	bool ReadVirtualCompartLink		(const boost::tokenizer<boost::char_separator<char>>& tokens);
	bool ReadVirtualCompartPoint	(const boost::tokenizer<boost::char_separator<char>>& tokens);
	bool ReadRelVirtualCompartLink	(const boost::tokenizer<boost::char_separator<char>>& tokens);
	bool ReadCoveringArea			(const boost::tokenizer<boost::char_separator<char>>& tokens);
	bool ReadSpatialRef				(const boost::tokenizer<boost::char_separator<char>>& tokens);
	bool ReadVersion				(const boost::tokenizer<boost::char_separator<char>>& tokens);
private:
	SurveyCSVPtr	_csv;				//!< データ実体
	bool			_skipDeleteRecord;	//!< 更新コード「削除」のレコードをスキップするか
	bool _hasError;						//!< エラー有無フラグ(インポート処理のチェック用)
};
typedef std::shared_ptr<SurveyCSVReader> SurveyCSVReaderPtr;

}; //adam_v2
