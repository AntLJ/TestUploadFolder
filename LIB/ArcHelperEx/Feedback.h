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

/**
* @file Feedback.h
*
* @brief CFeedback �N���X�̃C���^�[�t�F�C�X��`
*/

#if !defined(AFX_FEEDBACK_H__A62BEE45_C308_4A59_978D_0637FD1CF6F0__INCLUDED_)
#define AFX_FEEDBACK_H__A62BEE45_C308_4A59_978D_0637FD1CF6F0__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/**
* @brief �~�`�t�B�[�h�o�b�N�Ǘ��N���X
*
* @note �I���G���A�\���Ȃǂ̂��߁A�~�}�`�̃t�B�[�h�o�b�N���Ǘ�����
*
*/
class CFeedback  
{
public:
	CFeedback();
	virtual ~CFeedback();

	/**
	* @brief �t�B�[�h�o�b�N�̕`��
	*
	* @note �p�����[�^�̓_�𒆐S�Ƃ��āA�I��̈��`��B
	* @note StartDrawSearchArea��́A�}�E�X�ړ����Ƃɂ��̃��\�b�h���Ăяo���ė��p����B
	*
	* @param piPoint [in] �t�B�[�h�o�b�N��\��������ׂ��_
	*/
	BOOL DrawSearchArea( IPointPtr piPoint);

	/**
	* @brief �t�B�[�h�o�b�N�̍ĕ`��
	*
	* @param hDC		[in,optional]	�f�o�C�X�R���e�L�X�g
	*
	* @note �Ō�̏�Ԃ����ɍĕ`������s����B
	*/
	void ResetSearchArea( OLE_HANDLE hDC = NULL );

//	void StartDrawSearchArea( IPoint* ipPoint, IEnvelope* ipEnv, IScreenDisplay* ipScreen);

	/**
	* @brief �t�B�[�h�o�b�N���J�n
	*
	* @note unTolerance�͉�ʏ��Pixel�l
	*
	* @param piScreen [in] �t�B�[�h�o�b�N��\��������X�N���[���C���^�[�t�F�C�X
	* @param piPoint [in] �t�B�[�h�o�b�N���J�n����_
	* @param unTolerance [in] �I��̈�Ƃ��ĕ\��������~�̔��a
	*/
	BOOL StartDrawSearchArea( IScreenDisplayPtr piScreen, IPointPtr piPoint, UINT unTolerance);

	/**
	* @brief �t�B�[�h�o�b�N���~
	*/
	void StopDrawSearchArea();

	/**
	 * @brief �����G���A���擾����
	 *
	 * ���̊֐��́AArcObjects�̃o�O��������邽�߂ɗ^����ꂽ臒l�{10�s�N�Z����
	 * �o�b�t�@���쐬���ĕԂ��܂��B���݂͂قځH�o�O���C������Ă��܂��̂ŗ]�v��
	 * �o�b�t�@�����O���s�v�ȏꍇ�͈�����2�̂��̂��g�p���Ă��������B
	 * @param ipPoint		[in]			�G���A���S�|�C���g
	 * @param dSearchRadius	[out]			�G���A���a
	 * @param ipSpRef		[in,optional]	ISpatialReferencePtr
	 *
	 * @return IGeometryPtr
	 */
	IGeometryPtr GetSearchArea( IPointPtr ipPoint, DOUBLE* dSearchRadius, ISpatialReferencePtr ipSpRef = NULL ) const;
	IGeometryPtr GetSearchArea( IPointPtr ipPoint, ISpatialReferencePtr ipSpRef ) const;

	void SetScreenDisplay( IScreenDisplay* ipDisp, int* pTolerance = NULL ) const {
		m_ipScreenDisplay = ipDisp;
		// StartDrawSearchArea��m_unTolerance�����������ꂸ�ɗ���P�[�X�����݂���̂�
		// �����ł��������ł���悤�Ɂi�ꓖ����I���Ȃ��c�j
		if( pTolerance )
			m_nTolerance = *pTolerance;
	}
	IScreenDisplayPtr GetScreenDisplay() const { return m_ipScreenDisplay; }
private:
	IMovePointFeedbackPtr m_piFeedback; //!< �t�B�[�h�o�b�N�C���^�[�t�F�C�X
	mutable IScreenDisplayPtr m_ipScreenDisplay;
	mutable int m_nTolerance;
};

#endif // !defined(AFX_FEEDBACK_H__A62BEE45_C308_4A59_978D_0637FD1CF6F0__INCLUDED_)
