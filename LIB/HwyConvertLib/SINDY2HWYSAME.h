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

// SINDY2HWYSAME.h: SINDY2HWYSAME �N���X�̃C���^�[�t�F�C�X
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_SINDY2HWYSAME_H__5B9246F6_99EB_491C_84A2_D6B1A62B08E0__INCLUDED_)
#define AFX_SINDY2HWYSAME_H__5B9246F6_99EB_491C_84A2_D6B1A62B08E0__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "SINDY2INHWY.h"
#include "Customer.h"
#include "FacilNameChangeTable.h"

class SINDY2HWYSAME : public SINDY2INHWY  
{
public:
	enum EFormat
	{
		hwysame_old			= 1,	/// ���t�H�[�}�b�g(5.50�ȑO)�A�X�}�[�gIC���o�͂��Ȃ�
		hwysame_new			= 2		/// �ʏ�̃t�H�[�}�b�g(5.61�ȍ~)�A�X�}�[�gIC���o�͂���
	};

	SINDY2HWYSAME(SINDY2INHWYINFO *cHwyInfo);
	virtual ~SINDY2HWYSAME();

	virtual HRESULT Convert();
	virtual HRESULT Write(std::string cFileName, EFormat cFormat);

	/**
	 * @brief �{�ݖ��̕ϊ��i�d������w��j
	 * @note	�{�ݖ��̕ϊ����s�v�Ȏd�����悪�w�肳�ꂽ�ꍇ�͉������Ȃ��B
	 * @param	cCustomer	[in]	�d������
	 * @param	cpFacilNameChangeTable	[in]	�{�ݖ��̕ϊ��e�[�u��
	 */
	void changeFacilName(Customer cCustomer, FacilNameChangeTable* cpFacilNameChangeTable);

	/**
	 * @brief �{�ݖ��̕ϊ�
	 * @param	cpFacilNameChangeTable	[in]	�{�ݖ��̕ϊ��e�[�u��
	 */
	void changeFacilName( FacilNameChangeTable* cpFacilNameChangeTable );

	HRESULT createHwySameListWithoutSmartIC(void);

public:
	struct HwySame
	{
		long				eRoadCode;
		long				eSequence;
		long				eDirection;
		std::string			eName;
		std::vector<long>	eFacil;

		enum direction_c
		{
			fore			= 0,	// ������(����)
			back			= 1		// �t����(���)
		};

		HwySame() : eRoadCode(0), eSequence(0), eDirection(0), eName(""), eFacil() {}
		virtual ~HwySame() {}
	};

	struct lessHwySame : public std::binary_function<HwySame,HwySame,bool>
	{
		bool operator ()(const HwySame &cHwySame1, const HwySame &cHwySame2) const
		{
			return (cHwySame1.eRoadCode != cHwySame2.eRoadCode) ? cHwySame1.eRoadCode < cHwySame2.eRoadCode : (cHwySame1.eSequence != cHwySame2.eSequence) ? cHwySame1.eSequence < cHwySame2.eSequence : cHwySame1.eDirection < cHwySame2.eDirection;
		}
	};

protected:
	typedef std::vector<HwySame> Data;
	Data	mHwySameList;
	Data	mHwySameListWithoutSmartIC;		// �X�}�[�gIC���������f�[�^

public:
	typedef Data::const_iterator Iter;

	/**
	 * @brief �f�[�^��̐擪���w��const_iterator��Ԃ�
	 * @param[in]	withoutSmartIC	true�Ȃ�u�X�}�[�gIC���������f�[�^�v�Afalse�Ȃ�ʏ�̃f�[�^������
	 */
	Iter begin( bool withoutSmartIC = false ) const { return withoutSmartIC ? mHwySameListWithoutSmartIC.begin() : mHwySameList.begin(); }

	/**
	 * @brief �f�[�^��̖������w��const_iterator��Ԃ�
	 * @param[in]	withoutSmartIC	true�Ȃ�u�X�}�[�gIC���������f�[�^�v�Afalse�Ȃ�ʏ�̃f�[�^������
	 */
	Iter end( bool withoutSmartIC = false ) const { return withoutSmartIC ? mHwySameListWithoutSmartIC.end() : mHwySameList.end(); }

	/**
	 * @brief �f�[�^��̃T�C�Y��Ԃ�
	 * @param[in]	withoutSmartIC	true�Ȃ�u�X�}�[�gIC���������f�[�^�v�Afalse�Ȃ�ʏ�̃f�[�^������
	 */
	std::size_t size( bool withoutSmartIC = false ) const { return withoutSmartIC ? mHwySameListWithoutSmartIC.size() : mHwySameList.size(); }
};

#endif // !defined(AFX_SINDY2HWYSAME_H__5B9246F6_99EB_491C_84A2_D6B1A62B08E0__INCLUDED_)
