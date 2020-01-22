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

// MeshList.h: MeshList �N���X�̃C���^�[�t�F�C�X
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MESHLIST_H__D38CA68D_9F8C_4C73_96DD_293BE42C74C4__INCLUDED_)
#define AFX_MESHLIST_H__D38CA68D_9F8C_4C73_96DD_293BE42C74C4__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <set>
#include <stdio.h>
#include <windows.h>

using namespace std;

/// ���b�V���R�[�h�̃R���e�i
struct MeshListCont
{

	char	eMeshCode[9];		///< ���b�V���R�[�h
	long	eObjectID;			///< ���b�V���|���S���̃I�u�W�F�N�gID

	/// �R���X�g���N�^
	MeshListCont() {
		memset(eMeshCode, '\0', 9);
		eObjectID = 0;
	}

	/// �R���X�g���N�^�i�����l�w��j
	MeshListCont(char *str, long cOID = 0) {
		memset(eMeshCode, '\0', 9);
		strcpy(eMeshCode, str);
		eObjectID = cOID;
	}

	/// �R���X�g���N�^�i�����l�w��C�����l�j
	MeshListCont(long cVal, long cOID = 0) {
		memset(eMeshCode, '\0', 9);
		sprintf(eMeshCode, "%ld", cVal);
		eObjectID = cOID;
	}

	/// ���
	bool operator=(const char *str) {
		memset(eMeshCode, '\0', 9);
		strcpy(eMeshCode, str);
		eObjectID = 0;
	}

	/// ��r
	bool operator<(const MeshListCont &mlc) const {
		if(strcmp(eMeshCode, mlc.eMeshCode) < 0) return 1;
		else return 0;
	}

};

typedef set<MeshListCont, less<MeshListCont> >				MeshListSet;
typedef set<MeshListCont, less<MeshListCont> >::iterator	MeshListSet_i;

/// ���b�V�����X�g�N���X
class MeshList : public MeshListSet
{

protected:

	FILE	*mMeshListFP;		///< ���b�V�����X�g�t�@�C���̃t�@�C���|�C���^

public:

	/// �R���X�g���N�^
	MeshList();

	/// �f�X�g���N�^
	~MeshList();

	/// ���b�V�����X�g�t�@�C���̃I�[�v��
	/**
	* @param cFileName [in] �t�@�C����
	*
	* @retval true ����
	* @retval false ���s
	*/
	bool	Open(char *cFileName);

	/// ���b�V�����X�g�t�@�C���̍쐬
	/**
	* @param cFileName [in] �t�@�C����
	*
	* @retval true ����
	* @retval false ���s
	*/
	bool	Create(char *cFileName);

	/// ���X�g�ւ̃��b�V���R�[�h�̒ǉ�
	/**
	* @param cMeshCode [in] ���b�V���R�[�h�i������^�j
	*
	* @retval true ����
	* @retval false ���s�i���͒l���͈͊O�j
	*/
	bool	Add(char *cMeshCode);

	/// ���X�g�ւ̃��b�V���R�[�h�̒ǉ�
	/**
	* @param cMeshCode [in] ���b�V���R�[�h�i�����^�j
	*
	* @retval true ����
	* @retval false ���s�i���͒l���͈͊O�j
	*/
	bool	Add(LONG cMeshCode);

};

#endif // !defined(AFX_MESHLIST_H__D38CA68D_9F8C_4C73_96DD_293BE42C74C4__INCLUDED_)
