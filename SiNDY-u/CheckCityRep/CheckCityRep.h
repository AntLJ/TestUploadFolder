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

// CheckCityRep.h: CheckCityRep �N���X�̃C���^�[�t�F�C�X
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_CHECKCITYREP_H__6AED5E0E_5B38_49A4_B531_8EFBCDE2B54B__INCLUDED_)
#define AFX_CHECKCITYREP_H__6AED5E0E_5B38_49A4_B531_8EFBCDE2B54B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <set>
#include <string>

/// �`�F�b�N���[�h
enum	ECheckMode {
	kCheckNone		= 0,
	kCheckAll		= 1,
	kCheckByList	= 2,
	kCheckByRange	= 3
};

typedef	set<CString>	AddrCodeList;	//!< �Z���R�[�h�pset

/**
 * @class CheckCityRep
 * @brief �n�ԃ`�F�b�N�p���C���N���X
 */
class CheckCityRep
{
public:
	/**
	 * @brief �R���X�g���N�^
	 */
	CheckCityRep() : m_eCheckMode(kCheckNone), m_dSearchRange(0.002),
					 mFID_AnnoF(-1), mFID_PointClassC(-1), mFID_AddrCode(-1), mFID_RepTypeC(-1), mFID_ChibanNo(-1), mFID_GouType(-1), mFID_BgClassC(-1){}

	/**
	 * @brief �f�X�g���N�^
	 */
	~CheckCityRep(){}

	/**
	 * @brief	�g�p�@
	 */
	static void Usage();

	/**
	 * @brief  ������
	 * @param  argc	[in]	�����̐�
	 * @param  argv	[in]	����
	 * @retval true  ����
	 * @retval false ���s
	 */
	bool Init( int argc, char* argv[] );

	/**
	 * @brief  ���s
	 * @retval true  ����I��
	 * @retval false �ُ�I��
	 */
	bool Execute();

private:

	/**
	 * @brief  �s���E�R�[�h���X�g�쐬
	 * @retval true  ����
	 * @retval false ���s
	 */
	bool fnMakeACList();

	/**
	 * @brief  ����11���R�[�h�̍s���E���ЂƂ܂Ƃ߂ɂ���
	 * @param  strAddrCode  [in] 11���Z���R�[�h
	 * @retval IGeometryPtr ����11���s���E�����������W�I���g��
	 */
	IGeometryPtr fnUniteCityAdmin( const CString& strAddrCode );

private:

	ECheckMode m_eCheckMode;                   //!< �`�F�b�N���[�h
	ofstream   m_ofs;                          //!< ���O�o�̓X�g���[��

	CString    m_strAddrList;                  //!< �s�撬���R�[�h���X�g�t�@�C����
	CString    m_strRangeMin, m_strRangeMax;   //!< �R�[�h�͈̓��[�h����min max
	double     m_dSearchRange;                 //!< �n�Ԓ��L�����͈́i�x�P�ʁj

	IWorkspacePtr        m_ipWorkspaceAddress; //!< �Z���p���[�N�X�y�[�X
	IWorkspacePtr        m_ipWorkspaceMap;     //!< �n�}�p���[�N�X�y�[�X

	ISpatialReferencePtr m_ipSpRef;            //!< ��ԎQ��

	IFeatureClassPtr     m_ipFC_CityRepPoint;  //!< �n�ԑ�\�_�t�B�[�`���N���X
	IFeatureClassPtr     m_ipFC_CityAdmin;     //!< �s�s�s���E�t�B�[�`���N���X
	IFeatureClassPtr     m_ipFC_CitySite;      //!< �s�s�w�i�t�B�[�`���N���X
	IFeatureClassPtr     m_ipFC_GouPoint;      //!< ���|�C���g�t�B�[�`���N���X

	AddrCodeList         mACList;              //!< �����p�Z���R�[�h���X�g

	long       mFID_AnnoF, mFID_PointClassC, mFID_AddrCode, mFID_RepTypeC, mFID_ChibanNo, mFID_GouType, mFID_BgClassC; //!< �t�B�[���hID
};

#endif // !defined(AFX_CHECKCITYREP_H__6AED5E0E_5B38_49A4_B531_8EFBCDE2B54B__INCLUDED_)
