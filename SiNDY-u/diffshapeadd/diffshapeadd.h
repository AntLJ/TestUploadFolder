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
 * @brief	�ϊ��ɕK�v�ȃf�[�^�ۊǗp
 */
struct CInputData {
	long lDoType;						//!< ���s�^�C�v�i0:�����ǉ�, 1:�����f�[�^��SDE�ɒǉ��j

	CString strSShapeDir;				//!< S�Ж{�f�[�^Shape�ۑ��f�B���N�g���iDoType:0�j
	CString strDiffShapeDir;			//!< ����Shape�ۑ��f�B���N�g���iDoType:0,1�j
	CString strSDEConnect;				//!< SDE�ڑ���iDoType:1�j

	CString strDiffAddList;				//!< ����Shape�ǉ��p���X�g�iDoType:0�j

	long lMDivision;					//!< �ǉ������Ƃ���MDivision�l�i�w�肪������΁u3�v�j�iDoType:0�j

	CString strMeshlist;				//!< �����f�[�^�ǉ��p�̃��b�V�����X�g�iDoType:1�j
	std::set<CString> setMeshlist;		//!< �����f�[�^�ǉ��p�̃��b�V�����X�g�iDoType:1�j

	IWorkspacePtr ipSDEWork;			//!< SDE���[�N�X�y�[�X�iDoType:1�j
	IFeatureClassPtr ipLinkClass;		//!< �����N�N���X�iDoType:1�j
	IFeatureClassPtr ipNodeClass;		//!< �m�[�h�N���X�iDoType:1�j

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
