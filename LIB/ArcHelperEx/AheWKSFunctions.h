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
 * @file WKSFunctions.h
 * @brief <b>WKSPoint���g�p�������Z�֐���`�t�@�C��</b>\n
 * ���̃t�@�C���́AWKSPoint���g�p�������Z�̂��߂̊֐�����`����Ă��܂��B
 * IGeometry���n�߂Ƃ���COM�C���^�[�t�F�[�X�ł̉��Z�ɔ�ׁA�����ɓ���
 * ���鎖���ł��܂��B
 * WKSPoint���擾����ɂ́AIPointCollection::QueryWKSPoint���g�p���܂��B
 * @author �n�}�c�a���암�J���O���[�v �Ð�M�G
 * $Id$
 */
#ifndef _WKSFUNCTIONS_H_
#define _WKSFUNCTIONS_H_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/**
 * @brief <b>��_�̋��������߂�</b>
 * @return ����
 */
double WKS2PLength( const WKSPoint& p1, const WKSPoint& p2 );

/**
 * @brief <b>�O�_�̓��ς����߂�</b>
 * @return ����
 */
double WKS3PInnerproduct( const WKSPoint& p1, const WKSPoint& p2, const WKSPoint& p3 );

/**
 * @brief <b>�O�_�̂Ȃ��p�����߂�</b>
 * @param p1		[in]			�|�C���g1
 * @param p2		[in]			�|�C���g2
 * @param p3		[in]			�|�C���g3
 * @return �p�x�i���W�A���j
 */
double WKS3PAngle( const WKSPoint& p1, const WKSPoint& p2, const WKSPoint& p3 );

/**
 * @brief <b>�x�N�g�����m�̌���������s��</b>\n
 * ���������Ɍ�_�����߂����ꍇ�́AdV1t�AdV2t���g�p���Ă��������B\n
 * �Ȃ��A���s�̏ꍇ�ɂ�dV1t�AdV2t�͕s��ƂȂ�̂ŁA�g�p����ۂ�
 * �K��bParallel���`�F�b�N���Ă��������B\n
 * �[�_���m���ڂ��Ă���ꍇ�́A�����Ɣ��肵�܂����AdV1t�AdV2t�̒l��
 * �ڐG���������𔻒f���邱�Ƃ��ł��܂��B�����Ɣ��f�������Ȃ��ꍇ�́A
 * bTouchIsCross��false���Z�b�g���Ă��������B
 * @param pV1From		[in]			�x�N�g��1��FROM�|�C���g
 * @param pV1To			[in]			�x�N�g��1��TO�|�C���g
 * @param pV2From		[in]			�x�N�g��2��FROM�|�C���g
 * @param pV2To			[in]			�x�N�g��2��TO�|�C���g
 * @param bTouchIsCross	[in,optional]	�ڂ��Ă���ꍇ��true�ŕԂ��ꍇ��true�i�f�t�H���g�Ftrue�j
 * @param bParallel		[out,optional]	���s�̏ꍇ��true��������i�f�t�H���g�FNULL�j
 * @param dV1t			[out,optional]	�x�N�g��1�̕�����i�f�t�H���g�FNULL�j
 * @param dV2t			[out,optional]	�x�N�g��2�̕�����i�f�t�H���g�FNULL�j
 * @retval true �������Ă���
 * @retval false �������Ȃ�
 */
bool WKSIsCross( const WKSPoint& pV1From, const WKSPoint& pV1To, const WKSPoint& pV2From, const WKSPoint& pV2To, bool bTouchIsCross = true, bool* bParallel = NULL, double* dV1t = NULL, double* dV2t = NULL );

/**
 * @brief <b>��`���m�̌���������s��</b>\n
 * �Е����Е��ɓ���E�O����ꍇ��A�ڂ��Ă���ꍇ�������Ɣ��f���܂��B
 * @param rect1			[in]			�����`�P
 * @param rect2			[in]			�����`�Q
 * @retval true �������Ă���
 * @retval false �������Ă��Ȃ�
 */
bool WKSIsCross( const WKSEnvelope& rect1, const WKSEnvelope& rect2 );

#endif // _WKSFUNCTIONS_H_
