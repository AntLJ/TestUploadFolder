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
class POIPointRecord
{
public:
	POIPointRecord() : m_nMeshCode(0), m_nX(0), m_nY(0), m_lGrpCode(0), m_lChainCode(0), m_nAddXYC(0), m_buildingID(0), m_deleteCode(0){}

	bool operator<( const POIPointRecord& ob ) const
	{
		if( m_nAddXYC == 0)
			return	m_strTel < ob.m_strTel;
		else
			return	m_strTel + m_strName < ob.m_strTel + ob.m_strName;
	}

	/**
	 * @brief	精度優先付け
	 * @param	strAccCode	[in]	精度コード
	 * @return	優先度(低いほど優先度高) 1S(0) > 3A(1) > 1A(2) > UK(3)
	 */
	int GetPriorValueFromAcc() const;

	long	m_lOID;			//!< OBJECTID
	CString	m_strAccCode;	//!< 精度コード

	int		m_nMeshCode;	//!< 2次メッシュコード
	int		m_nX;			//!< MeshX
	int		m_nY;			//!< MeshY
	long	m_lGrpCode;		//!< 分類コード
	long	m_lChainCode;	//!< 店舗コード
	CString	m_strSeq;		//!< データシーケンス
	CString	m_strName;		//!< 名称
	CString	m_strTel;		//!< 電話番号
	CString	m_strAddrCode1;	//!< 住所コード1
	CString	m_strAddrCode2;	//!< 住所コード2
	int		m_nAddXYC;		//!< マッチング種別
	long	m_buildingID;	//!< 建物OBJECTID
	long	m_deleteCode;	//!< 削除コード 0:生存 1:削除 2:削除保留
};

