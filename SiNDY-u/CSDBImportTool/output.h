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
#include <fstream>
#include <atlfile.h>
#include <map>

class COutput
{
public:
	COutput(void);
	~COutput(void);
	void InitFile( LPCTSTR lpszFileName );
	
	void ResetValue();
	void SetPOI( IFeature* ipPOIFeature, long Meshcode );
	void SetNewAnnoPoint( long lOID, LPCTSTR lpcszName, int nPlace, int nCrossingLinks );
	void SetOrgAnnoPoint( long lOID, int type );
	void SetOverlapAnno( LPCTSTR lpszNewName, long lOID, long lAnnoCode, LPCTSTR lpszName ){ m_strAnnoName = lpszNewName, m_lOverlapOID = lOID; m_lOverlapAnnoCode = lAnnoCode; m_strOverlapAnnoName = lpszName;}
	void SetExitAnno( long lOID, LPCTSTR strAnnoName ){ m_lExistOID = lOID, m_strExistAnnoName = strAnnoName;}
	void SetResult( int n );	// ×：－１, 既存：０, 新規：１, PEC：２
	void SetWornString( int n );	// 1:国道番号	2:ＳＡ施設

	void WriteLog();
	void VariantLog( CComVariant& cval);

private:
	std::map<CString, long> GetPOIFieldMap( IFeature* ipFeature );

private:
	std::ofstream m_fout;								// ファイル出力ストリーム

	long m_lOverlapOID;									// 重なった注記のOID
	long m_lOverlapAnnoCode;							// 重なった注記の注記種別コード
	CString m_strOverlapAnnoName;						// 重なった注記の表示文字列
	long m_lPOIObjectID;								// 元のPOIのオブジェクトID
	long m_lMeshCode;									// 元のPOIのあるメッシュコード
	CString m_strAccCode;								// POI精度
	CString m_strPOIName;								// POI店舗名
	long m_lNewAnnoOID;									// 新規真位置のオブジェクトID
	int m_nNewDispType;									// 新規真位置の表示種別 2:記号＋文字列 4:記号のみ
	int m_nPlaceType;									// どの場所に置けたか
	std::list< std::pair<long, int> > m_lstOrgAnno;     // 新規真位置に対応する元真位置の候補リスト（1個では無いかもしれない）
	std::map<int, CString> mapDispType;                 // 表示種別を定義するマップ
	std::map<long, CString> m_mapAnnoCodeDomain;		// 注記種別ドメインマップ
	std::map<CString, long> m_mapPOIField;				// POIフィールドマップ

	long m_lExistOID;									// 元々あった同一注記のOID
	CString m_strExistAnnoName;
	CString m_strResult;								// 既存 or 新規 or 不可

	int m_nCrossingLinks;								// 重なった道路リンクの数

	CString m_strAnnoName;
	CString m_strWorn;									// 警告ログ用
};
