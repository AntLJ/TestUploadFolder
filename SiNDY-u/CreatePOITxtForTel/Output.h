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
#include "POIPointRecord.h"
#include "POISubPointRecord.h"

typedef std::multiset<POIPointRecord> POIPointRecordSet;
typedef std::multiset<POIPointRecord>::const_iterator POIPointRecordSetItr;
typedef std::multiset<POISubPointRecord> POISubPointRecordSet;

// データ出力用クラス
class Output
{
public:
	~Output();

	/*
	 * @brief ファイルを開く
	 * @param [in] outputFilePath ファイルパス
	 */
	void Open( const uh::tstring& outputFilePath );

	/*
	 * @brief ファイルが開いているかどうかを調べる
	 * @retval true 開いている
	 * @retval false 開いてない
	 */
	bool IsOpen();

	/*
	 * @brief POI_POINTデータを書き込む
	 * @param [in] recordSet POI_POINTデータ
	 */
	void POIPointOutput( const POIPointRecordSet& recordSet );

	/*
	 * @brief POI_SUB_POINTデータを書き込む
	 * @param [in] recordSet POI_SUB_POINTデータ
	 */
	void POISubPointOutput( const POISubPointRecordSet& recordSet );

private:
	std::ofstream m_ofs; //!< ファイル出力ストリーム
};

