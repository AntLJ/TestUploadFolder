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

/**
 * @file	define.h
 * @brief	各種定義
 */

using namespace std;

#define PROGRAM_NAME	_T("CheckTelTPError")	// プログラム名

/**
 * @enum	ECheckType
 * @brief	チェックタイプ
 */
enum ECheckType
{
	kCTAddr16	= 0,	//!< 16桁検索
	kCTAddr11	= 1,	//!< 11桁検索
	kCTAddrEX	= 2,	//!< 11桁 + 拡張号番号=On 検索
	kCTAddrCM	= 3,	//!< 8桁＋「000」＆［住所変換モード：大字も生成］検索
};

/**
 * @enum	EResultStat
 * @brief	チェック結果ステータス
 */
enum EResultStat
{
	kO_TELTP_ADDED,			//!< 新規追加
	kO_TELTP_OK,			//!< 問題なし
	kW_TELTP_NOT_FOUND,		//!< 【要確認】対象ポイントが存在しない
	kW_TELTP_FAIL_PSTAT,	//!< 【要確認】「該当施設あり」でない
	kW_TELTP_FAIL_ADDXY,	//!< 【要確認】座標付与種別が違う
	kW_TELTP_MOVED,			//!< 【要確認】対象ポイント移動
	kW_DUPLI_TELTP,			//!< 【要確認】TelTPポイントが複数ある
	kE_FAIL_GET_INFO,		//!< 【エラー】TelTPポイントの情報取得失敗
	kE_FAIL_GET_ADDXY,		//!< 【エラー】ADDXY_C取得失敗
};

static const _TCHAR* GetResultMsg( EResultStat emStatus )
{
	switch( emStatus )
	{
	case kO_TELTP_ADDED:			return _T("新規追加");
	case kO_TELTP_OK:				return _T("問題なし");
	case kW_TELTP_NOT_FOUND:		return _T("【要確認】対象ポイントが存在しない");
	case kW_TELTP_FAIL_PSTAT:		return _T("【要確認】「該当施設あり」でない");
	case kW_TELTP_FAIL_ADDXY:		return _T("【要確認】座標付与種別が違う");
	case kW_TELTP_MOVED:			return _T("【要確認】対象ポイント移動");
	case kW_DUPLI_TELTP:			return _T("【要確認】同一シーケンス＆座標付与種別のTelTPポイントが複数ある");
	case kE_FAIL_GET_INFO:			return _T("【エラー】TelTPポイントの情報取得失敗");
	case kE_FAIL_GET_ADDXY:			return _T("【エラー】ADDXY_C取得失敗");
	default:
		return _T("不明");
	}
}

/**
 * @struct	CMeshXY
 * @brief	メッシュXY
 */
struct CMeshXY
{
	int			m_nMesh;	//!< メッシュコード
	int			m_nX;		//!< メッシュX
	int			m_nY;		//!< メッシュY

	CMeshXY() : m_nMesh(0), m_nX(0), m_nY(0){}

	/**
	 * @brief	== のオーバーロード
	 * @note	ここでは、メッシュコードが等しい　かつ 正規化XYが100未満の差であれば等しいとする
	 * @return	bool
	 */
	bool operator==( const CMeshXY& ob )
	{
		return (m_nMesh == ob.m_nMesh) && (abs(m_nX - ob.m_nX) < 100) && (abs(m_nY - ob.m_nY) < 100);
	}

};

/**
 * @struct	CLRec
 * @brief	リストレコード
 */
struct CLRec
{
	CString			m_strClaimNo;		//!< クレームNo
	CString			m_strObjectID;		//!< ObjectID
	CString			m_strSequence;		//!< シーケンス
	CMeshXY			m_cMXY;				//!< メッシュXY
	long			m_lAddXY;			//!< 座標付与種別(AddXY_C)
	long			m_lUserClaimF;		//!< ユーザクレームフラグ
	CString			m_strTelNum;		//!< 電話番号
	CString			m_strAddrName;		//!< 掲載住所名称
	CString			m_strAddrCode;		//!< 住所コード(11桁)
	CString			m_strKatagaki;		//!< 方書名称
	CString			m_strKeisaiName;	//!< 掲載名称
	long			m_lPointStat;		//!< 調査結果コード
	CString			m_strSource;		//!< 情報ソース
	CMeshXY			m_cMXY_Ref;			//!< 移動先メッシュXY
	CString			m_strAddComment;	//!< 追加コメント

	CLRec() : m_lAddXY(1), m_lPointStat(0), m_lUserClaimF(0){}

};

typedef	std::list<CLRec>	CLList;
