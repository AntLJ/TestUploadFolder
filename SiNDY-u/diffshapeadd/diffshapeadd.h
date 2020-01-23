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

const LPCTSTR g_sbaselink = _T("bas_link");
const LPCTSTR g_FID = _T("FID");
const LPCTSTR g_MID = _T("M_ID");
const LPCTSTR g_MDivision = _T("M_DIVISION");

const LPCTSTR g_UpdateOperator = _T("DiffShapeAdd");

/**
 * @brief	変換に必要なデータ保管用
 */
struct CInputData {
	long lDoType;						//!< 実行タイプ（0:差分追加, 1:差分データをSDEに追加）

	CString strSShapeDir;				//!< S社本データShape保存ディレクトリ（DoType:0）
	CString strDiffShapeDir;			//!< 差分Shape保存ディレクトリ（DoType:0,1）
	CString strSDEConnect;				//!< SDE接続先（DoType:1）

	CString strDiffAddList;				//!< 差分Shape追加用リスト（DoType:0）

	long lMDivision;					//!< 追加したときのMDivision値（指定が無ければ「3」）（DoType:0）

	CString strMeshlist;				//!< 差分データ追加用のメッシュリスト（DoType:1）
	std::set<CString> setMeshlist;		//!< 差分データ追加用のメッシュリスト（DoType:1）

	IWorkspacePtr ipSDEWork;			//!< SDEワークスペース（DoType:1）
	IFeatureClassPtr ipLinkClass;		//!< リンククラス（DoType:1）
	IFeatureClassPtr ipNodeClass;		//!< ノードクラス（DoType:1）

	CInputData() : lDoType(0), lMDivision(-1) {}

	void operator=(CInputData& cInputData) {
		this->lDoType = cInputData.lDoType;
		this->strSShapeDir = cInputData.strSShapeDir;
		this->strDiffShapeDir = cInputData.strDiffShapeDir;
		this->strSDEConnect = cInputData.strSDEConnect;
		this->strDiffAddList = cInputData.strDiffAddList;
		this->strMeshlist = cInputData.strMeshlist;
		this->setMeshlist = cInputData.setMeshlist;
		this->lMDivision = cInputData.lMDivision;

		this->ipSDEWork = cInputData.ipSDEWork;
		this->ipLinkClass = cInputData.ipLinkClass;
		this->ipNodeClass = cInputData.ipNodeClass;

	}
};


CString nowtime();
