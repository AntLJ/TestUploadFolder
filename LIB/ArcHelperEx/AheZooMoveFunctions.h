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
 * @file AheZooMoveFunctions.h
 * @brief ArcObjects�O���[�o���w���p�[�֐���`
 *
 * ���̃t�@�C���ɂ͊g��k���A�X�N���[���ȂǂɊւ���
 * �O���[�o���֐�����`����Ă��܂��B
 * 
 * @author �n�}�c�a���암�V�X�e���J���O���[�v �Ð�M�G
 * @date 2007/01/26
 * $Id$
 */
#ifndef SINDYLIB_AHE_ZOOMOVEFUNCTIONS_H_
#define SINDYLIB_AHE_ZOOMOVEFUNCTIONS_H_

/**
* @brief ZooMoveTool�̃L�[�ړ��g��k������
*
* @note ZooMoveTool�Ŏ��������L�[�{�[�h�ł̏��������𒊏o
*
* @param ipActView [in] �r���[
* @param lKeyCode [in] ���z�L�[�R�[�h
* @param lShift [in] �C���L�[�H�X�e�[�^�X
*
* @retval true handled
* @retval false not handled
*/
bool AheKeyZooMove( IActiveView* ipActView, long lKeyCode, long lShift );

/**
* @brief �J�[�\���ʒu�ɉ�ʂ̒��S���ړ�����
* @param ipActiveView [in] �r���[
*/
bool AheMoveCenterToMousePointer( IActiveView* ipActiveView, bool bSuppressEvents = true );

/**
* @brief �J�[�\�����r���[�̃Z���^�[�Ɉړ�
* @param ipActiveView [in] �r���[
*/
bool AheMoveCursorToCenter( IActiveView* ipActiveView );

/**
* @brief �w��X�e�b�v�����̈ړ�
*
* @param piActiveView [in] �}�b�v�̃r���[
* @param nXStep [in] �ړ��P��
* @param nYStep [in] �ړ��P��
*/
bool AheScrollMap( IActiveView* ipActView, int nXStep, int nYStep );

/// MapFAN��ZM�l�ɑ������g�嗦�ł̊g��E�k������
bool AheZoom( IActiveView* ipActiveView, bool bUpScale );

/// MapFAN��ZM�l�ɑ������g�嗦�ł̊g�又��
bool AheZoomIn( IActiveView* ipActiveView );

/// MapFAN��ZM�l�ɑ������g�嗦�ł̏k������
bool AheZoomOut( IActiveView* ipActiveView );

// �ĕ`�惂�[�h���擾
bool AheIsRefreshMode();

/**
* @brief ������}���ĕ`��
*
* @param piActiveView [in] �}�b�v�̃r���[
* @param ipEnv [in] �ĕ`���`
*/
void AheRefresh(IActiveView* ipActiveView, IEnvelope* ipEnv=NULL);

#endif // ifndef SINDYLIB_AHE_ZOOMOVEFUNCTIONS_H_
