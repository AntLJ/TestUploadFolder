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
 * @file Progressor.h
 * @brief CProgressor��`�t�@�C��
 * @author �n�}�c�a���암�V�X�e���J���O���[�v �Ð�M�G
 * @version $Id$
 */
#ifndef _PROGRESSOR_H_
#define _PROGRESSOR_H_

#include "exportimport.h"

/**
 * @class CProgressor
 * @brief <b>ICancelTracker�y��IProgressor���܂Ƃ߂ĊǗ�����N���X</b>\n
 * �v���O���X�_�C�A���O�A�v���O���X�o�[�A�L�����Z���g���b�J�[���܂Ƃ߂ĊǗ����܂��B\n
 * �v���O���b�T�[�̏��������W��0-100�A�X�e�b�v��10�ł��B
 */
class SINDYLIB_API CProgressor
{
public:
	CProgressor();
	virtual ~CProgressor();

	CProgressor( ITrackCancel* lp );

	/**
	 * @brief <b>�v���O���X�_�C�A���O��\�����邽�߂̐e�E�B���h�E�n���h�����Z�b�g����</b>\n
	 * �v���O���X�_�C�A���O�g�p���ɂ͕K���Z�b�g����K�v������܂��B
	 * @param hWnd	[in]	�e�E�B���h�E�n���h��
	 */
	void SetHWND( HWND hWnd );

	/**
	 * @brief <b>�L�����Z���g���b�J�[���擾����</b>\n
	 * ������Ύ����I�ɍ쐬����܂��B
	 */
	ITrackCancelPtr GetTrackCancel() const;
	IProgressorPtr GetProgressor() const;
	void ShowProgressBar( bool bShow = true ) const;
	void ShowProgressDialog( bool bShow = true );
	bool Continue() const;
private:
	HWND			m_hWnd;
	ITrackCancel*	m_ipTrackCancel;
};

#endif // ifndef _PROGRESSOR_H_
