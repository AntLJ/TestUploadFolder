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

// SindyHighwayTrace.h: SindyHighwayTrace �N���X�̃C���^�[�t�F�C�X
//
//////////////////////////////////////////////////////////////////////
/**
 * @file HwyConvertLib�J���ɔ����ASindyHighwayCheck�̃\�[�X��SindyHighwayConvert�ł��g���Ă����̂��������邽�߁A
 *       ���̋��L������SindyHighwayCheck����HwyConvertLib�Ɉڊǂ����B
 */

#if !defined(AFX_SINDYHIGHWAYTRACE_H__F1C372A3_22A9_4424_88C0_1E86155D28E6__INCLUDED_)
#define AFX_SINDYHIGHWAYTRACE_H__F1C372A3_22A9_4424_88C0_1E86155D28E6__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "SindyHighwayTools.h"
#include "SindyRoadLinkTraceCondition.h"

class SindyHighwayTrace : public SindyRoadLinkTraceCondition
{
public:
	enum TraceTarget
	{
		trace_all		= 0,	// �S��
		trace_ramp		= 1,	// �{�������N�ȊO�̃����N�̂�
		trace_mainline	= 2		// �{�������N�̂�
	};

	SindyHighwayTrace(TraceTarget cTraceTarget = trace_ramp);		// �f�t�H���g�ł͖{���ȊO�̃����N�݂̂�ǐՑΏۂƂ���
	virtual ~SindyHighwayTrace();

	/**
	 * @brief �ǐՕ�����sindy::rnw::relation::code��fore,back,bothdir�̂����ꂩ�Ŏw�肷��
	 * @param[in]	cDirection	�ǐՕ���
	 */
	virtual void set_direction(long cDirection) { mDirection = cDirection; }

	/**
	 * @brief �ǐՑΏۃ����N���uUpDownClass_C��HWY�Ώۂ̂��̂����ɂ���v���ۂ���ݒ肷��
	 * @note	[Bug8480]�Ή����ɁAtrace()�̂��̕�����L���ɂ��邩��h���N���X���C�ӂɑI���ł���悤�ɂ��邽�߂ɍ쐬�B
	 *			�i�Ƃ������A��������trace()�ɂ����ǉ��������Ƃ��݌v�~�X�Ȃ̂ŁA���̃t�H���[�ł���B�j
	 *			����܂łɍ�����h���N���X���u�L���v�ɂȂ�悤�ɁAmUpDownClassEnabled�̃f�t�H���g�̓R���X�g���N�^�Łu�L���v�ɂ��Ă����B
	 * @param[in]	cIsEnabled	�ݒ�l
	 */
	void set_up_down_class_option( bool cIsEnabled ) { mUpDownClassEnabled = cIsEnabled; }

	virtual HRESULT trace( SindyRoadInfo *cRoadInfo, IFeaturePtr ipLink, IFeaturePtr ipNode, bool *cTrace );

	/**
	 * @brief �Ώۃm�[�h����Ώۃ����N�ցA�ǐՕ����ɉ����đJ�ډ\�ł��邩�ۂ��𔻒肷��i[Bug7176]�Œǉ��j
	 * @note	[Bug7176]�ŁA�h���N���X�ɂď�L�����𔻒肵�����ӏ����������̂ŁAtrace()�̈ꕔ���֐���
	 * @param[in]	cRoadInfo	���H���
	 * @param[in]	ipLink		�Ώۃ����N
	 * @param[in]	ipNode		�Ώۃm�[�h
	 * @param[out]	cTrace		���茋�ʂ�Ԃ��o�b�t�@
	 * @return	����
	 */
	virtual HRESULT Passable( SindyRoadInfo *cRoadInfo, IFeaturePtr ipLink, IFeaturePtr ipNode, bool *cTrace );

protected:
	long		mDirection;				// �ǐՕ���
	TraceTarget	m_TraceTarget;			// �ǐՑΏ�
	bool		mUpDownClassEnabled;	// trace()��UpDownClass_C�ɂ�锻����s�����ۂ��i[Bug8480]�Œǉ��j
};

#endif // !defined(AFX_SINDYHIGHWAYTRACE_H__F1C372A3_22A9_4424_88C0_1E86155D28E6__INCLUDED_)
