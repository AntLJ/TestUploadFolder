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
#include "resource.h"

enum EKiseiNum{
	KISEIJIKAN1,
	KISEIJIKAN2,
	KISEIJIKAN3,
	KISEIJIKAN4
};

typedef struct RegulationTime{
	CString strSMon;
	CString strEMon;
	CString strSDay;
	CString strEDay;
	CString strSHour;
	CString strEHour;
	CString strSMin;
	CString strEMin;
	int nYobiIndex;
	bool bIsEveryMonth;
}REGTIME;


/**
 * @class CRegulationTime
 * @brief �K�����ԂP�`�K�����ԂS�̊e�R���g���[���𑀍�A����A�`�F�b�N���邽�߂̃N���X
 *
 * �R���g���[��ID��n���ăC���X�^���X���쐬����ƁA�֘A����i�����K�����Ԃ́j�e�R���g���[����
 * �ӎ������Ɉ������Ƃ��ł��܂��B
 */
class CRegulationTime
{
public:
	CRegulationTime(void);
	~CRegulationTime(void);
	CRegulationTime( HWND hDlg, EKiseiNum Enum );

	/**
	 * @brief �R���g���[��ID����K�����Ԃ̔ԍ���Ԃ��ÓI�֐�
	 * 
	 * @param	nID	[in]	�R���g���[��ID
	 *
	 * @retval	�K�����Ԕԍ��iKISEIJIKAN1�`KISEIJIKAN4�j
	 */
	static EKiseiNum CtrlID2KiseiNum( const int nID ); 

	/**
	 * @brief �G�f�B�b�g�{�b�N�X�ɓ��͂���Ă��镶�����Ԃ�
	 * 
	 * @param	cEdit	[in]	�G�f�B�b�g�R���g���[��
	 *
	 * @retval	���͂���Ă��镶����
	 */
	CString GetEditStr( CEdit cEdit );

	/**
	 * @brief �J�n�A�I������"0"��������"00"��\������
	 * 
	 * @retval	�Ȃ�
	 */
	void DisplayZeroZero();

	/**
	 * @brief �u�����v�{�^����nBtnState�̏�Ԃɂ���
	 * @brief  nBtnState = 0: �g�O����Off�ɂ��āu���v��Disable�ɂ���
	 * @brief  nBtnState = 1: �g�O����On�ɂ��āu���v��Enable�ɂ���
	 * 
	 * @param	nBtnState	[in]	�g�O���{�^���̏��
	 * @retval	�Ȃ�
	 */
	void ToggleZeroGatu( const int nBtnState );

	/**
	 * @brief �u�����v�{�^���������ꂽ��Ԃɂ���
	 * @note �u�����v�{�^�����g�O��On�ɂ��āu���v�G�f�B�b�g��Enable�ɂ��܂�
	 * 
	 * @param	nBtnState	[in]	���ɂ��𓯎��ɃZ�b�g����0��1���`0��31���ɂ��邩�ǂ���
	 * @retval	�Ȃ�
	 */
	void SetZeroGatu( bool bIsSetDay = true );

	/**
	 * @brief �u�����v�{�^����������Ă��Ȃ���Ԃɂ���
	 * @note �u�����v�{�^�����g�O��OFF�ɂ��āu���v�G�f�B�b�g��Disable�ɂ��܂�
	 * 
	 * @retval	�Ȃ�
	 */
	void UnsetZeroGatu();

	/**
	 * @brief  �K�����Ԃ̃G�f�B�b�g�{�b�N�X�ɒl����͂���
	 * 
	 * @param	lSMon	[in]	�J�n��
	 * @param	lSDay	[in]	�J�n��
	 * @param	lEMon	[in]	�I����
	 * @param	lEDay	[in]	�I����
	 * @retval	�Ȃ�
	 */
	void SetRegDate( const long lSMon, const long lSDay, const long lEMon, const long lEDay ); 

	/**
	 * @brief  �K�����Ԃ̃G�f�B�b�g�{�b�N�X�ɒl����͂���
	 * 
	 * @param	lSHour	[in]	�J�n��
	 * @param	lSMin	[in]	�J�n��
	 * @param	lEHour	[in]	�I����
	 * @param	lEMin	[in]	�I����
	 * @retval	�Ȃ�
	 */
	void SetRegTime( const long lSHour, const long lSMin, const long lEHour, const long lEMin );

	/**
	 * @brief  �K�����Ԃ��N���A����
	 * @note  �e�G�f�B�b�g�{�b�N�X��_T("")�ŃN���A����A�R���{�{�b�N�X��<null>�ƂȂ�܂��B
	 * 
	 * @retval	�Ȃ�
	 */
	void Clear();

	/**
	 * @brief  �u0���v�̓��͂��Ď�����
	 * 
	 * @param	hWnd	[in]	�q�_�C�A���O�̃n���h��
	 * @param	strErr	[out]	�G���[���b�Z�[�W�B���ʂ�false�̎��͋�B
	 * @param	bFirst	[in]	true: ����o�^�̂��߁A0����͂�����B�@	false: ���ړ��͎���0����͂����Ȃ��B
	 * @retval	true: �J�n�܂��͏I����EditBox�Ɂu0�v�����͂��ꂽ
	 * @retval  false: ���͂́u0�v�ȊO 
	 */
	bool CheckZeroGatu( HWND hWnd, CString &strErr, bool bFinishInit );

	/**
	 * @brief  �u0���v�����͂���Ă��邩�ǂ���
	 * @note   IsEveryMonth()�Ƃ͈Ӗ����قȂ�܂��B
	 * 
	 * @retval	true: �J�n���A�I�������u0���v
	 */
	bool IsZeroGatu();

	/**
	 * @brief  �K�����Ԃ�1���`12����
	 * @note  IsZeroGatu()�Ƃ͈Ӗ����قȂ�܂��B
	 * 
	 * @retval	true: �J�n�� = 1 && �I���� = 12
	 */
	bool IsEveryMonth();

	/**
	 * @brief  �Ώۂ̋K���̓��ɂ����Ԃ��܂��Ă��邩
	 * 
	 * @param	tReg	[in]	��r�ΏۂƂȂ�K��
	 * @retval	true��: this: 1���`31�� �� tReg: 1���`7��
	 */
	bool IsContainDays(  REGTIME &tReg  );

	/**
	 * @brief  �Ώۂ̋K���̓��ɂ����Ԃ��܂��Ă��邩
	 * 
	 * @param	tReg	[in]	��r�ΏۂƂȂ�K��
	 * @retval	true��: this: 1���`31�� �� tReg: 1���`7��
	 */
	bool IsContainTime(  REGTIME &tReg  );

	/**
	 * @brief  �K�����ԍ\���̂�Ԃ�
	 * 
	 * @retval	���͂���Ă���K�����ԍ\����
	 */
	REGTIME GetRegTime();

	/**
	 * @brief  �K�����Ԃ��Z�b�g����
	 * 
	 * @retval	���͂���K�����ԍ\����
	 */
	void SetRegTime( REGTIME & tRegTime );

	
	//bool IsEmpty();
	//bool IsTunen();
	//bool IsSyujitu();
	//bool IsEverydayEveryTime();

private:
	CEdit m_cEditStartMonth, m_cEditStartDay, m_cEditEndMonth, m_cEditEndDay,
		  m_cEditStartHour, m_cEditStartMin, m_cEditEndHour, m_cEditEndMin;

	CButton m_cBtnZeroGatu;
	CComboBox m_cCmbYobi;
};

