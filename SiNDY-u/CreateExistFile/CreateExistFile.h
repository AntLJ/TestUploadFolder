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

// CreateExistFile.h: CreateExistFile �N���X�̃C���^�[�t�F�C�X
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_CREATEEXISTFILE_H__012E22F1_38D2_426D_9F95_A7A3785DB30E__INCLUDED_)
#define AFX_CREATEEXISTFILE_H__012E22F1_38D2_426D_9F95_A7A3785DB30E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <direct.h>
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <iomanip>
#include <SDEAccess.h>
#include "MeshList.h"

// �Ƃ肠����VC11�Ƃ���ȊO��2�p�^�[���ɂ��Ă��� 2012.11.02
#ifdef _DEBUG
	#if _MSC_VER == 1700
		#pragma comment(lib,"SDEAccess_vc11-gd.lib")
	#else
		#pragma comment(lib,"SDEAccessD.lib")
	#endif
#else
	#if _MSC_VER == 1700
		#pragma comment(lib,"SDEAccess_vc11.lib")
	#else
		#pragma comment(lib,"SDEAccess.lib")
	#endif
#endif


#define ARG_MIN		1

#define DIV_MAX		64

#define FC_CITYMESH		"reference.CityMesh"
#define FC_BASEMESH		"reference.BaseMesh"
#define FC_CITYSITE		"City_Site"

#define FD_CITYMESHCODE	"MeshCode"
#define FD_BASEMESHCODE	"MeshCode"
#define FD_COMPLETE_F	"Complete_f"
#define FD_BGCLASS_C	"BgClass_c"

/// �������[�h
enum	ECreateMode {
	kCreateNone			= 0,	// ���w��
	kCreateAllMesh		= 1,	// �I�v�V���� -a �ɂ��S���b�V���w��
	kCreateMeshesInList	= 2,	// �I�v�V���� -l �ɂ�郁�b�V�����X�g�ł̃��b�V���w��
	kCreateSingleMesh	= 3		// �I�v�V���� -m �ɂ��P�ꃁ�b�V���w��
};

/// ���ݔ���t�@�C�������N���X
class CreateExistFile : public SDEAccess
{

private:

	int			mArgc;
	char		**mArgv;

	string		mOutDir;

	ECreateMode	mCreateMode;

	int			mDivNum;

	string		mFNMeshList;
	string		mMeshCode;

	MeshList	mMeshList;

	string		mDBMap;

	string		mFCCityMesh;
	string		mFCBaseMesh;
	string		mFCCitySite;

	IFeatureClassPtr	ipFC_CityMesh;
	IFeatureClassPtr	ipFC_BaseMesh;
	IFeatureClassPtr	ipFC_CitySite;

	long		mFID_CityMeshCode;
	long		mFID_BaseMeshCode;
	long		mFID_CompleteFlag;
	long		mFID_BGClass_C;

	bool	fnAnalyzeArg();

	void	fnPrintUsage();

	bool	fnMakeMeshList();

	bool	fnCreateCache(const string &cMeshCode);

	IPolygonPtr	fnCreateIncompletePoly(IEnvelopePtr ipE_CityMesh, long *cCount);

public:

	CreateExistFile();
	virtual ~CreateExistFile();

	/// ���ݔ���t�@�C���̐���
	bool	Create(int cArgc, char **cArgv);

private:
	bool ExistError;

};

#endif // !defined(AFX_CREATEEXISTFILE_H__012E22F1_38D2_426D_9F95_A7A3785DB30E__INCLUDED_)
