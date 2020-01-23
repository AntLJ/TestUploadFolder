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
#include "stdafx.h"
#include "Log.h"
#include <utf8/usjis.h>

class CPoiFile
{
public:
	CPoiFile(void);
	~CPoiFile(void);

	/**
	 * @brief	POIファイルリスト読み込み
	 * @param	sPoiListPath	[in]	POIリストファイルパス
	 * @param	setContCode		[in]	対象コンテンツコード
	 * @param	vecPoiData		[out]	POIデータコンテナ
	 * @return							処理成否
	 */
	bool readPoiList(const std::string& sPoiListPath,
					 std::set<std::string>& setContCode,
					 MAP_POI_DATA& vecPoiData);

protected:
	/**
	 * @brief	POIデータ取得
	 * @param	file			[in]	ファイルパス
	 * @param	setContCode		[in]	対象コンテンツコード
	 * @param	vecPoiData		[out]	POIデータコンテナ
	 * @return							処理成否
	 */
	bool getPoiData(const std::string& file,
					std::set<std::string>& setContCode,
					MAP_POI_DATA& mapPoiData);
};

