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

#include "AddrCount.h"
#include <AddrLib/AreaCodeHelper.h>

using namespace addr;

/**
 * @class	CompareAddrCount
 * @brief	�Z���f�[�^������r�N���X
 */
class CompareAddrCount
{

public:

	CompareAddrCount(void) :
	  mCharF(false), mTarget(kTgtAll) {}
	~CompareAddrCount(void) {}

	/**
	 * @brief	�g�p���@�̕\��
	 */
	void PrintUsage();

	/**
	 * @brief	������
	 * @param	cArgc	[in]	�����̐�
	 * @param	cArgv	[in]	�������X�g
	 * @return	�����������ۂ�
	 */
	bool Init(int cArgc, _TCHAR* cArgv[]);

	/**
	 * @brief	�����̎��s
	 * @return	�����������ۂ�
	 */
	bool Execute();

private:

	bool		mCharF;			///< �J�E���g�̑Ώۂ𕶎��t���Z���Ƃ��邩�ǂ���
	ETarget		mTarget;		///< �J�E���g�Ώێw����@
	ofstream	mLog;			///< ���O�t�@�C���X�g���[��
	CString		mLogFilename;	///< ���O�t�@�C����
	CString		mTgtOpt;		///< �͈͎w�蕶����i�͈� or ���X�g�t�@�C�� or 5���P�́j

	CString		mDBOld;			///< DB�ڑ�������(��)
	CString		mDBNew;			///< DB�ڑ�������(�V)

	CString		mFCGouPoint;	///< ���|�C���g�t�B�[�`���N���X��
	CString		mFCCityAdmin;	///< �s�s�n�}�s���E�t�B�[�`���N���X��
	CString		mFCCSAddrPoint;	///< CS�|�C���g�t�B�[�`���N���X��

	CString		mJDBOld;		///< �Z��DB�ڑ�������(��)
	CString		mJDBNew;		///< �Z��DB�ڑ�������(�V)

	CAreaCodeHelper	mACHelperOld;	///< �Z���}�X�^�A�N�Z�X�N���X(��)
	CAreaCodeHelper	mACHelperNew;	///< �Z���}�X�^�A�N�Z�X�N���X(�V)

	AddrCount	mAddrCountOld;	///< �Z���f�[�^�J�E���^(��)
	AddrCount	mAddrCountNew;	///< �Z���f�[�^�J�E���^(�V)

	/**
	 * @brief	�����̉��
	 * @param	cArgc	[in]	�����̐�
	 * @param	cArgv	[in]	�������X�g
	 * @return	�����������ۂ�
	 */
	bool fnAnalyzeArg(int cArgc, _TCHAR *cArgv[]);

	/**
	 * @brief	���ϐ��̎擾
	 * @return	�����������ۂ�
	 */
	bool fnGetEnvVar();

	/**
	 * @brief	�e�t�B�[�`���N���X�̎擾
	 * @param	cDB				[in]	DB�ڑ�������
	 * @param	cGouPoint		[out]	���|�C���g�E�t�B�[�`���N���X
	 * @param	cCityAdmin		[out]	�s�s�n�}�s���E�E�t�B�[�`���N���X
	 * @param	cCSAddrPoint	[out]	CS�|�C���g�E�t�B�[�`���N���X
	 * @return	�����������ۂ�
	 */
	bool fnGetFeatureClasses(const CString& cDB, IFeatureClassPtr& cGouPoint, IFeatureClassPtr& cCityAdmin, IFeatureClassPtr& cCSAddrPoint);

	/**
	 * @brief	�Z���}�X�^����̎{�s�N���E�p�~�N���̎擾
	 * @param	cAddrCode		[in]	�Z���R�[�h
	 * @param	cShikoDate		[out]	�{�s�N��
	 * @param	cHaishiDate		[out]	�p�~�N��
	 * @return	�����������ۂ�
	 */
	bool fnGetShikoHaishiDate(const CString& cAddrCode, CString& cShikoDate, CString& cHaishiDate); 
};
