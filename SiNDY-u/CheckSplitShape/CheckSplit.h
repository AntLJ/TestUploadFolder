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
#include "Arguments.h"

class CCheckSplit {
public:
	CCheckSplit() : m_CheckMode(check_mode::kNone), m_lMeshDigit(0), m_bJGD2000(false){}
	~CCheckSplit(){}

public:
	bool Init(const Arguments& argument);
	bool run(bool &bCheckErr);

private:
	/**
	 * @brief ���b�V�����X�g�擾
	 * @param strMeshlistPath  [in] ���b�V�����X�g�ւ̃p�X
	 * @retval true  ��������
	 * @retval true  �������s
	 */
	bool LoadMeshlist(const CString& strMeshlistPath);

	/**
	 * @brief �V�F�[�v�t�@�C���ւ̃p�X�擾
	 * @param strBasePath  [in] �x�[�X�p�X
	 * @param strMeshCode  [in] ���b�V���R�[�h
	 * @return �V�F�[�v�t�@�C���̃p�X
	 */
	CString GetWorkShpPath(const CString& strBasePath, const CString& strMeshCode);

public:
	CString m_strBeforeShpPath;              //!< �ϊ��O�V�F�[�v�t�@�C���i�[��f�B���N�g��
	CString m_strBeforeDBPath;               //!< �ϊ��ODB�ڑ���
	CString m_strAfterShpPath;               //!< �ϊ���V�F�[�v�t�@�C���i�[��f�B���N�g��
	CString m_strMeshDBPath;                 //!< ���b�V���`��pDB�p�X
	CString m_strMeshSchema;                 //!< ���b�V���`��p�X�L�[�}��
	check_mode::ECode m_CheckMode;           //!< �`�F�b�N���[�h
	std::set<CString> m_setMeshList;         //!< ���b�V�����X�g
	bool m_bJGD2000;                         //!< JGD2000�����邩�ǂ����i�����m�[�h�p�j
	double m_dMinSegment;                    //!< �����Z�O�����g��

	IFeatureClassPtr m_ipMeshClass;          //!< ���b�V���`��擾�p�t�B�[�`���N���X
	IFeatureClassPtr m_ipBeforeHNodeClass;   //!< �ϊ��O���΍����t�B�[�`���N���X
	ITablePtr m_ipAfterHNodeTable;           //!< �ϊ��㑊�΍����e�[�u��

	long m_lMeshDigit;                       //!< 
};

