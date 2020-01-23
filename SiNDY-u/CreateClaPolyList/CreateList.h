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

// CreateList.h: CCreateList クラスのインターフェイス
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_CREATELIST_H__17BD2DE6_7C62_45EF_A27D_B864FECBE3CE__INCLUDED_)
#define AFX_CREATELIST_H__17BD2DE6_7C62_45EF_A27D_B864FECBE3CE__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

using namespace std;

/**
 * @class	CCreateList
 */
class CCreateList  
{
public:
	CCreateList();
	~CCreateList();

	/**
	 * @brief	使用法
	 */
	static void printUsage();

	/**
	 * @brief	初期化
	 * @param	コマンドライン引数そのまま
	 * @return	true: 成功	false: 失敗
	 */
	bool init( int argc, char* argv[] );

	/**
	 * @brief	実行
	 */
	bool execute();

private:
	/**
	 * @breif	クレポリリスト作成
	 * @param	ipFeature	[in]	クレームポリフィーチャ
	 */
	void makeClaimPolyList( IFeature* ipFeature );

	/**
	 * @brief	クレポリ情報出力
	 * @param	lOID		[in]	オブジェクトID
	 * @param	lpcszClaimNo	[in]	クレーム番号
	 * @param	nUselessF	[in]	不要フラグ
	 */
	void printInfo( long lOID, const _TCHAR* lpcszClaimNo, long lUselessF )
	{
		m_fout << lOID << "," << lpcszClaimNo << "," << lUselessF << endl;
	}

/////////////////////////////
//	メンバ変数
/////////////////////////////
private:

	std::ofstream		m_fout;					//!< 出力ストリーム

	IFeatureClassPtr	m_ipClaimPolyClass;		//!< クレポリフィーチャクラス	

	long				m_lClaimNumberFID;		//!<! クレーム番号フィールドインデックス
	long				m_lUselessF_FID;		//!<! 不要フラグフィールドインデックス
};

#endif // !defined(AFX_CREATELIST_H__17BD2DE6_7C62_45EF_A27D_B864FECBE3CE__INCLUDED_)
