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

#include <AddrLib/AddrCodeList.h>

using namespace addr;

/**
 * @enum ETarget
 * @brief �J�E���g�Ώێw����@
 */
enum ETarget
{
	kTgtAll,	///< �S���w��
	kTgtRange,	///< �͈͎w��
	kTgtList,	///< ���X�g�w��
	kTgtSingle,	///< �P�̎w��
};

typedef set<CString> AddrSet;

/**
 * @class AddrCount
 * @brief �Z���f�[�^�����J�E���g�N���X
 */
class AddrCount : public map<CString, int>
{

public:

	AddrCount(void) : mCharF(false), mTarget(kTgtAll) {}
	~AddrCount(void) {}

	/**
	 * @brief	������
	 * @param	cCharF			[in]	�����t���Z�����[�h���ǂ���
	 * @param	cTarget			[in]	�J�E���g�Ώێw����@
	 * @param	cTgtOpt			[in]	�J�E���g�Ώێw��I�v�V����������
	 * @param	cGouPoint		[in]	���|�C���g�E�t�B�[�`���N���X
	 * @param	cCityAdmin		[in]	�s�s�n�}�s���E�E�t�B�[�`���N���X
	 * @param	cCSAddrPoint	[in]	CS�|�C���g�E�t�B�[�`���N���X
	 * @return	�����������ۂ�
	 */
	bool Init(bool cCharF, ETarget cTarget, const CString& cTgtOpt, IFeatureClassPtr cGouPoint, IFeatureClassPtr cCityAdmin, IFeatureClassPtr cCSAddrPoint);

	/**
	 * @brief	�J�E���g�̎��s
	 * @return	�����������ۂ�
	 */
	bool Count();

private:

	bool				mCharF;			///< �J�E���g�̑Ώۂ𕶎��t���Z���Ƃ��邩�ǂ���
	ETarget				mTarget;		///< �J�E���g�Ώێw����@
	CString				mTgtOpt;		///< �͈͎w�蕶����i�͈� or ���X�g�t�@�C�� or 5���P�́j

	CAddrCodeList		mList;			///< �J�E���g�ΏۏZ���R�[�h���X�g

	IFeatureClassPtr	mGouPoint;		///< ���|�C���g�E�t�B�[�`���N���X
	IFeatureClassPtr	mCityAdmin;		///< �s�s�n�}�s���E�E�t�B�[�`���N���X
//	IFeatureClassPtr	mCSAddrPoint;	///< CS�|�C���g�E�t�B�[�`���N���X
	CFeatureClass		mCSAddrPoint;	///< CS�|�C���g�E�t�B�[�`���N���X

	CSPFieldMap			mFMapCSPoint;	///< CS�|�C���g�E�t�B�[���h�Ή��\
	CSPTableNameString	mNameCSPoint;	///< CS�|�C���g�E�e�[�u����

	/**
	 * @brief	�J�E���g�ΏۏZ���R�[�h���X�g�̍쐬
	 * @return	�����������ۂ�
	 */
	bool fnCreateAddrList();

	/**
	 * @brief	���ԍ���������u-�v�Ńg�[�N���ɕ���
	 * @param	cGouNo		[in]		���ԍ�
	 * @param	cTokens		[in,out]	�g�[�N�����X�g
	 */
	void fnTokenizeGouNo(const CString& cGouNo, vector<CString>& cTokens);
};
