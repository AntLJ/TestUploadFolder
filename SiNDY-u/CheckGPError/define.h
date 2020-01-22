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

#define PROGRAM_NAME	_T("CheckGPError")	// プログラム名

/**
 * @enum	ECheckType
 * @brief	チェックタイプ
 */
enum ECheckType
{
	kCTAddr16	= 0,	//!< 16桁検索
	kCTAddr11	= 1,	//!< 11桁検索
	kCTAddrEX	= 2,	//!< 11桁 + 拡張号番号=On 検索
	kCTAddrCM	= 3,	//!< 通称モード検索(Bug8166)
};

/**
 * @enum	EResultStat
 * @brief	チェック結果ステータス
 */
enum EResultStat
{
	kO_NOT_FOUND,			//!< CS or Gouから該当ポイントみつからない
	kO_MATCH_PRIOR_GP,		//!< Gouと優先フラグありポイントで一致(一時ステータス)
	kO_MATCH_GP,			//!< Gouと優先フラグなしポイントで一致(一時ステータス)
	kW_DUPLI_CS,			//!< 【要確認】CSポイントが複数ある
	kW_CS_SAME_ADMIN,		//!< 【要確認】CSポイントと同一16桁の行政界が存在します	[Bug6180]
	kO_CS_ADDED,			//!< 新規追加(CS)
	kO_CS_OK,				//!< 問題なし(CS)
	kW_CS_MOVED,			//!< 【要確認】CSポイント移動
	kE_CS_MOVED_FAIL,		//!<【エラー】CSポイント移動失敗
	kW_GP_SAME_ADMIN,		//!< 【要確認】同一コードの符号界が存在
	kW_GP_DUPLI_PRIOR,		//!< 【要確認】同一20桁コードで優先フラグ立っているものが複数あり
	kW_GP_NOT_PRIOR,		//!< 【要確認】優先フラグが立っていない
	kW_GP_ADDED_PRIOR_TPG,	//!< 新規追加／【要確認】優先フラグの立っているポイントがTPG
	kO_GP_ADDED_PRIOR,		//!< 新規追加
	kW_GP_PRIOR_TPG,		//!< 【要確認】優先フラグの立っているポイントがTPG
	kO_GP_OK,				//!< 問題なし
	kW_GP_MOVED,			//!<【要確認】号ポイント移動
	kW_GP_MOVED_NOT_PRIOR,	//!<【要確認】号ポイント移動(優先フラグON)
	kW_GP_MOVED_FAIL,		//!<【エラー】号ポイント移動失敗
	kW_GP_MOVED_DIFF_ADMIN,	//!<【要確認】移動前後で行政界の住所コードが異なる
	kW_CS_INSERT,			//!<【要確認】CSポイント追加
	kE_CS_INSERT_FAIL,		//!<【エラー】CSポイント追加失敗
	kW_GP_INSERT,			//!<【要確認】号ポイント追加
	kE_GP_INSERT_FAIL,		//!<【エラー】号ポイント追加失敗
	kW_GP_INSERT_EX,		//!<【要確認】拡張号ポイント追加
	kE_GP_INSERT_EX_FAIL,	//!<【要確認】拡張号ポイント追加失敗
	kE_INSERT_FAIL,			//!<【エラー】ポイント追加失敗(不正なMeshXY)
	kE_INSERT_ADMIN_FAIL,	//!<【エラー】ポイント追加失敗(行政界確認)
	KW_CS_INSERT_CANNOT_CHAR,	//!< 【要確認】文字付き住所は、CSで追加できない
	kE_PNTNAME_GET_FAIL,	//!< 【エラー】文字地番名称の取得に失敗
	kE_GOUNAME_GET_FAIL,	//!< 【エラー】文字号名称の取得に失敗
	kE_MOVED_REF_FAIL,	//!< 【エラー】参照DBのポイントと不一致のため移動に失敗
	kE_INSERT_REF_FAIL	//!< 【エラー】参照DBのポイントと不一致のため追加に失敗
};

static const _TCHAR* GetResultMsg( EResultStat emStatus )
{
	switch( emStatus )
	{
	case kO_NOT_FOUND:				return _T("【要確認】該当ポイントなし");
	case kW_DUPLI_CS:				return _T("【要確認】CSポイントが複数ある");
	case kW_CS_SAME_ADMIN:			return _T("【要確認】CSポイントと同一16桁の行政界が存在します");
	case kO_CS_ADDED:				return _T("新規追加(CS)");
	case kO_CS_OK:					return _T("問題なし(CS)");
	case kW_CS_MOVED:				return _T("【要確認】CSポイント移動");
	case kE_CS_MOVED_FAIL:			return _T("【Error】CSポイント移動失敗");
	case kW_GP_SAME_ADMIN:			return _T("【要確認】同一コードの符号界が存在");
	case kW_GP_DUPLI_PRIOR:			return _T("【要確認】同一20桁コードで優先フラグ立っているものが複数あり");
	case kW_GP_NOT_PRIOR:			return _T("【要確認】優先フラグが立っていない");
	case kW_GP_ADDED_PRIOR_TPG:		return _T("新規追加／【要確認】優先フラグの立っているポイントがTPG");
	case kO_GP_ADDED_PRIOR:			return _T("新規追加");
	case kW_GP_PRIOR_TPG:			return _T("【要確認】優先フラグの立っているポイントがTPG");
	case kO_GP_OK:					return _T("問題なし");
	case kW_GP_MOVED:				return _T("【要確認】号ポイント移動");
	case kW_GP_MOVED_NOT_PRIOR:		return _T("【要確認】号ポイント移動(優先フラグON)");
	case kW_GP_MOVED_FAIL:			return _T("【エラー】号ポイント移動失敗");
	case kW_GP_MOVED_DIFF_ADMIN:	return _T("【要確認】移動前後で行政界の住所コードが異なる");
	case kW_CS_INSERT:				return _T("【要確認】CSポイント追加");
	case kE_CS_INSERT_FAIL:			return _T("【要確認】CSポイント追加失敗");
	case kW_GP_INSERT:				return _T("【要確認】号ポイント追加");
	case kE_GP_INSERT_FAIL:			return _T("【エラー】号ポイント追加失敗");
	case kW_GP_INSERT_EX:			return _T("【要確認】拡張号ポイント追加");
	case kE_GP_INSERT_EX_FAIL:		return _T("【エラー】拡張号ポイント追加失敗");
	case kE_INSERT_FAIL:			return _T("【エラー】ポイント追加失敗(不正なMeshXY)");
	case kE_INSERT_ADMIN_FAIL:		return _T("【エラー】ポイント追加失敗(行政界確認)");
	case KW_CS_INSERT_CANNOT_CHAR:	return _T("【要確認】文字付き住所は、CSで追加できない");
	case kE_PNTNAME_GET_FAIL:		return _T("【エラー】文字地番名称の取得に失敗");
	case kE_GOUNAME_GET_FAIL:		return _T("【エラー】文字号名称の取得に失敗");
	case kE_MOVED_REF_FAIL:			return _T("【エラー】参照DBのポイントと不一致のため移動に失敗");
	case kE_INSERT_REF_FAIL:			return _T("【エラー】参照DBのポイントと不一致のため追加に失敗");
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
	CString			m_strAddrCode;		//!< 住所コード
	CString			m_strAddrCodeNew;	//!< 新住所コード
	CMeshXY			m_cMXY;				//!< メッシュXY
	long			m_lPurpose;			//!< 作業目的
	long			m_lExFlag;			//!< 拡張号番号フラグ
	CString			m_strSource;		//!< 情報ソース
	CMeshXY			m_cMXY_Ref;			//!< 移動先メッシュXY

	long			m_lEditPointID;		//!< 編集したポイントのOID

	CLRec() : m_lExFlag(0), m_lPurpose(0), m_lEditPointID(0){}

	/**
	 * @brief	文字付き地番?(住所コードから判断)
	 * @return	bool
	 */
	bool IsCharPntCode() const{ return m_strAddrCode.GetLength() == 20? !_istdigit(m_strAddrCode.GetAt(11)) : false; }

	/**
	 * @brief	文字付き号?(住所コードから判断)
	 * @return	bool
	 */
	bool IsCharGouCode() const{ return m_strAddrCode.GetLength() == 20? !_istdigit(m_strAddrCode.GetAt(16)) : false; }

	/**
	 * @brief	文字付き住所?(住所コードから判断)
	 * @return	bool
	 */
	bool IsCharAddrCode() const{ return IsCharPntCode() || IsCharGouCode(); }
};

typedef	std::list<CLRec>	CLList;
