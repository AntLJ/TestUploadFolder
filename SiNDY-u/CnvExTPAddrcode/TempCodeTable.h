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

// TempCodeTable.h: CTempCodeTable クラスのインターフェイス
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_TEMPCODETABLE_H__AAFCFA8F_BDA3_41E9_8E67_6222C77C4491__INCLUDED_)
#define AFX_TEMPCODETABLE_H__AAFCFA8F_BDA3_41E9_8E67_6222C77C4491__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/**
 * @class	CTempCodeTable
 * @brief	仮コード対応表クラス
 */
class CTempCodeTable  
{
public:
	CTempCodeTable() : m_bIsSet(false){};
	virtual ~CTempCodeTable(){};

	/**
	 * @brief	仮コード対応表ファイルロード
	 * @param	lpcszFile	[in]	ファイル名
	 * @return	bool
	 */
	bool load( LPCTSTR lpcszFile );

	/**
	 * @brief	初期設定既にされてる？
	 * @return	bool
	 */
	bool isSetting() const { return m_bIsSet; }

	/**
	 * @brief	指定コードから置換できる新コードある？
	 * @param	strAddrCode		[in]	指定コード
	 * @param	strNewAddrCode	[out]	置換すべきコード
	 * @retval	true  : 置換コードあった
	 * @retval	false : なし
	 */
	bool getTaiouCode( const CString& strAddrCode, CString& strNewAddrCode )
	{
		std::multimap<CString, CString>::const_iterator itr=m_mapTmpTable.find( strAddrCode );
		if( itr != m_mapTmpTable.end() )
		{
			strNewAddrCode = itr->second;
			return true;
		}
		else
			return false;
	}



///////////////////////////////
//	メンバ変数
///////////////////////////////

private:

	bool	m_bIsSet;								//!< セットされている？

	std::multimap<CString, CString>	m_mapTmpTable;	//!< 仮コード対応表

};

#endif // !defined(AFX_TEMPCODETABLE_H__AAFCFA8F_BDA3_41E9_8E67_6222C77C4491__INCLUDED_)
