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

#include "Parameter.h"
#include <crd_cnv/coord_converter.h>

/**
 * @class	CWorkTxtImple
 * @brief	中間データ出力基底クラス
 * @note	このクラスを継承して、各ポイントごとの実装を行う
 */
class CWorkTxtImpl
{
public:
	CWorkTxtImpl( const CParameter& rParam, std::ofstream& ofs ) : m_rParam(rParam), m_ofsLog(ofs){}
	virtual ~CWorkTxtImpl(void){}

	/**
	 * @brief	初期化
	 * @note	継承先で必ず実装
	 * @return	bool
	 */
	virtual bool Init() = 0;

	/**
	 * @brief	データ出力
	 * @note	継承先で必ず実装
	 * @return	bool
	 */
	virtual bool WriteData() = 0;

protected:

	/**
	 * @brief	緯度・経度からiPC都市地図メッシュコード取得
	 * @param	dLon	[in]	経度
	 * @param	dLat	[in]	緯度
	 * @return	メッシュコード(失敗したら-1)
	 */
	int GetMeshCode( double dLon, double dLat )
	{
		static crd_cnv g_cCrdCnv;
		int nMeshcode = 0, nX = 0, nY = 0;
		return g_cCrdCnv.LLtoMesh(dLon, dLat, 64, &nMeshcode, &nX, &nY)  == 0 ? nMeshcode : -1;
	}

	/**
	 * @brief	エラーログ出力
	 * @param	pRow		[in]	CRow
	 * @param	lpcszLevel	[in]	エラーレベル
	 * @param	lpcszMsg	[in]	エラーメッセージ
	 * @param	lpcszInfo	[in]	追加情報
	 */
	void PrintError( const CRow* pRow, const _TCHAR* lpcszLevel, const _TCHAR* lpcszMsg, const _TCHAR* lpcszInfo = NULL )
	{
		CString strLog;
		strLog.Format( _T("0\t%s\t%d\t\t\t%s\t\t%s\t%s"),
					   pRow->GetTableName(), pRow->GetOID(), lpcszLevel, lpcszMsg, lpcszInfo? lpcszInfo : _T("") );
		m_ofsLog << CT2CA(strLog) << std::endl;
	}

protected:

	const CParameter&	m_rParam;			//!< 実行情報

private:

	std::ofstream&		m_ofsLog;			//!< ログストリーム
};
