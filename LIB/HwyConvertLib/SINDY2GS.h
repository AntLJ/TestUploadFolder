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

// SINDY2GS.h: SINDY2GS �N���X�̃C���^�[�t�F�C�X
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_SINDY2GS_H__5A3385F9_C91D_4C4F_8511_8613F3CAE6DB__INCLUDED_)
#define AFX_SINDY2GS_H__5A3385F9_C91D_4C4F_8511_8613F3CAE6DB__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "SINDY2INHWY.h"

class SINDY2GS : public SINDY2INHWY  
{
public:
	enum EType
	{
		gs				= 0,	/// �K�\�����X�^���h���
		multiple_store	= 1		/// �`�F�[���X���
	};

	enum EFormat
	{
		gs_old	= 1,			/// ���t�H�[�}�b�g(5.50�ȑO)
		gs_new	= 2				/// �ʏ�̃t�H�[�}�b�g(5.61�ȍ~)
	};

	enum EDirection				/// �o�͂���㉺���t���O�̒�`
	{
		direction_down	= 0,	/// ����
		direction_up	= 1		/// ���
	};

	struct AccessStore
	{
		long		eStoreCode;		/// �`�F�[���X�}�X�^�[�R�[�h

		long		eStartHour;		/// �c�ƊJ�n����
		long		eStartMin;		/// �c�ƊJ�n��
		long		eEndHour;		/// �c�ƏI������
		long		eEndMin;		/// �c�ƏI����

		bool		eSunday;		/// �j���c�ƃt���O
		bool		eMonday;
		bool		eTuesday;
		bool		eWednesday;
		bool		eThursday;
		bool		eFriday;
		bool		eSaturday;

		bool		eBon;			/// ���ԉc�ƃt���O
		bool		eYearEnd;
		bool		eNewYear;
		bool		eGoldenWeek;

		bool		eHoliday;		/// �j�Փ��c�ƃt���O
		EType		eStoreType;		/// �X�܎�ʃR�[�h
	};

	SINDY2GS(SINDY2INHWYINFO *cHwyInfo);
	virtual ~SINDY2GS();

	virtual HRESULT Convert();
	virtual HRESULT Write(std::string cFileName, EType cType, EFormat cFormat);

	static HRESULT GetAccessStore(SINDY2INHWYINFO* cHwyInfo, IFeaturePtr cpAccessPoint, std::vector<AccessStore>* cpAccessStoreList);

public:
	struct GS
	{
		long		eRoadCode;	// �H���R�[�h
		long		eSequence;	// �H�����V�[�P���X�ԍ�
		std::string	eName;		// �{�ݖ���
		long		eLineDir;	// �㉺������
		long		eGsType;	// �f�r�̎��

		std::vector<AccessStore>	eAccessStoreInfo;	/// �K�\�����X�^���hor�`�F�[���X���̗�

		GS() : eRoadCode(0), eSequence(0), eName(""), eLineDir(0), eGsType(0) {}
		virtual ~GS() {}
	};

	typedef std::vector<GS> GsList;
private:
	GsList mGsList;

public:
	typedef GsList::const_iterator Iter;

	/// �f�[�^��̐擪���w��const_iterator��Ԃ�
	Iter begin() const { return mGsList.begin(); }

	/// �f�[�^��̖������w��const_iterator��Ԃ�
	Iter end() const { return mGsList.end(); }

	/// �f�[�^��̃T�C�Y��Ԃ�
	std::size_t size() const { return mGsList.size(); }
};

#endif // !defined(AFX_SINDY2GS_H__5A3385F9_C91D_4C4F_8511_8613F3CAE6DB__INCLUDED_)
