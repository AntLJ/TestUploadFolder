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
// TimeReg.h: CTimeReg�N���X�̃C���^�[�t�F�C�X
// RoadCheck.SnwRCCheckTimeReg.h ���x�[�X
//////////////////////////////////////////////////////////////////////
#include "stdafx.h"

/**
 * @brief ���ԋK���N���X
 */
class CTimeReg
{
public:

	/// �R���X�g���N�^
	CTimeReg(
		long startMonth,
		long startDay,
		long startHour,
		long startMin,
		long endMonth,
		long endDay,
		long endHour,
		long endMin,
		long weekCode
	);

	/// �f�X�g���N�^
	~CTimeReg();

	/// ��r���Z�q
	bool operator == (const CTimeReg& cCond) const;

	/// �_���`�F�b�N
	bool check() const;

	/// �K���������^����ꂽ�K���������܂�ł���`�F�b�N
	bool contain(const CTimeReg& cCond) const;

	/// �K���������^����ꂽ�K�������ƌ�������`�F�b�N
	bool cross(const CTimeReg& cCond) const;

	/// �ʔN�`�F�b�N
	bool checkAllDay() const;

private:
	/// ���K���I�����Ԃ��Z�b�g
	void setNormalizedEndTime();

	/// ���t���܂�ł���
	bool containDay(const CTimeReg& cCond) const;
	/// ���Ԃ��܂�ł���
	bool containTime(const CTimeReg& cCond) const;
	/// �j�����܂�ł���
	bool containWeek(const CTimeReg& cCond) const;

	/// ���t���������Ă���
	bool crossDay(const CTimeReg& cCond) const;
	/// ���Ԃ��������Ă���
	bool crossTime(const CTimeReg& cCond) const;
	/// �j�����������Ă���
	bool crossWeek(const CTimeReg& cCond) const;

	/// �j���t���O�̎擾
	int getWeekFlag() const;

	/// �e���̍ő�����̎擾
	static int GetLastDay(long cMonth);

	long m_weekCode;   ///< �j���R�[�h
	long m_startMonth; ///< �J�n��
	long m_startDay;   ///< �J�n��
	long m_startHour;  ///< �J�n����
	long m_startMin;   ///< �J�n��
	long m_endMonth;   ///< �I����
	long m_endDay;     ///< �I����
	long m_endHour;    ///< �I������
	long m_endMin;     ///< �I����
	long eEndTime2;    ///< ���K���I������
	long eEndMonth2;   ///< ���K���I����

};
