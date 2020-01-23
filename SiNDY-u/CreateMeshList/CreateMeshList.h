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

// CreateMeshList.h: CreateMeshList �N���X�̃C���^�[�t�F�C�X
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_CREATEMESHLIST_H__35D55BED_C2DE_4486_B7A4_1462AFDB1F56__INCLUDED_)
#define AFX_CREATEMESHLIST_H__35D55BED_C2DE_4486_B7A4_1462AFDB1F56__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <SDEAccess.h>
#include "MeshList.h"
#include <libsdeaccess.h>

#define FC_CITYMESH		"reference.CityMesh"
#define FC_BASEMESH		"reference.BaseMesh"
#define FC_CITYADMIN	"City_Admin"

#define FD_CITYMESHCODE	"MeshCode"		//2008/05/28 ���b�V���R�[�h�t�B�[���h�̕ύX�ɑΉ� �i���Y�j
#define FD_COMPLETE_F	"CREATE_YEAR"	//2008/05/28 �G���A���O����̕ύX�ɑΉ� �i���Y�j
#define FD_BASEMESHCODE	"MeshCode"

class CreateMeshList : public SDEAccess 
{

private:

	int		mArgc;
	char	**mArgv;

	char	*mOutFN;

	FILE	*mOutFP;

	bool	mAllAdmin;

	char	*mDBCon_Address;

	char	mFCCityMesh[50];
	char	mFCBaseMesh[50];
	char	mFCCityAdmin[50];

	char	mFDCityMeshCode[50];
	char	mFDCompleteF[50];

	IFeatureClassPtr	ipFC_CityMesh;
	IFeatureClassPtr	ipFC_BaseMesh;
	IFeatureClassPtr	ipFC_CityAdmin;

	MeshList	mMeshList;

	bool	fnAnalyzeArg(void);

	void	fnPrintUsage(void);

	/**
	 * @brief SpatialRelDescription�𗘗p����shape�ɂ�錟��
	 * @param ipGeometry [in] �����pshape�̃W�I���g���E�C���^�[�t�F�[�X
	 * @param cSpatialRelDesc [in] ��ԓI�֌W�������L�q
	 * @param ipFeatureClass [in] �����Ώۂ̃t�B�[�`���N���X�E�C���^�t�F�[�X
	 * @param cCount [out] �������ꂽ�t�B�[�`���̌���
	 *
	 * @return �����̌��ʂ��i�[�����t�B�[�`���J�[�\��
	 */
	IFeatureCursorPtr	SearchByShapeAndDesc(
		IGeometryPtr		ipGeometry,
		LPCTSTR				cSpatialRelDesc,
		IFeatureClassPtr	ipFeatureClass,
		LONG				*cCount
		);

public:

	CreateMeshList();
	virtual ~CreateMeshList();

	bool Create(int cArgc, char **cArgv);

};

#endif // !defined(AFX_CREATEMESHLIST_H__35D55BED_C2DE_4486_B7A4_1462AFDB1F56__INCLUDED_)
