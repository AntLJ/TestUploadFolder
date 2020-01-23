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

// �R�}���h���C�������p�i�\���̂ł��ǂ������B�B�j
class CInputInfo
{
public:
	CInputInfo() : bTestMode(false), lPurpose(-1) {}
	~CInputInfo(){}

public:
	CString strBeforeDir;     //!< �ҏW�OPGDB�̃f�B���N�g��
	CString strAfterDir;      //!< �ҏW��PGDB�̃f�B���N�g��
	CString strSDEConnect;    //!< �C���|�[�g��DB�ւ̐ڑ�������
	CString strOperator;      //!< �C���|�[�g���̕ҏW�Җ�
	long lPurpose;            //!< �C���|�[�g���̍�ƖړI
	std::set<long> setMesh;   //!< �C���|�[�g�Ώۃ��b�V�����X�g
	CString strLogDir;        //!< ���O�o�͐�f�B���N�g��
	bool bTestMode;           //!< �e�X�g���[�h���ǂ���
	CString strExceptList;    //!< �C���|�[�g���O�t�B�[���h���X�g�t�@�C����(bug 10363)
};

class CDefaultInfo
{
public:
	CDefaultInfo(){}
	~CDefaultInfo(){}

public:
	void init();

public:

};
