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

class CArguments
{
public:
	CArguments(void);
	~CArguments(void);

	/**
	 * @brief コマンドライン引数の解析
	 * @param	argc [in]	コマンドライン引数の数
	 * @param	argv [in]	コマンドライン引数の内容
	 * @retval	true	成功
	 * @retval	false	失敗
	 */
	bool parse( int argc, _TCHAR* argv[] );

public:
	uh::tstring m_poiDB;     //!< POIデータSDE接続先
	uh::tstring m_addrDB;    //!< 住所データSDE接続先
	uh::tstring m_mapDB;     //!< 地図データSDE接続先
	uh::tstring m_poiPoint;  //!< POI_POINT
	uh::tstring m_poiSubPoint; //!< POI_SUB_POINT
	uh::tstring m_cityAdmin; //!< CITY_ADMIN
	uh::tstring m_building;  //!< BUILDING
	uh::tstring m_contents;  //!< POI_CONTENTS_MASTER
	uh::tstring m_listPath;  //!< 出力対象種別表リスト
	uh::tstring m_outFile;   //!< POI_POINT出力ファイル名
	uh::tstring m_outFileSub;   //!< POI_SUB_POINT出力ファイル名
	uh::tstring m_idnCode;   //!< 処理対象IDNCODE
};

