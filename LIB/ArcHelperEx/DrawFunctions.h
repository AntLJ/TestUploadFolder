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
 * @file DrawFunctions.cpp
 * @brief �`��n�O���[�o���w���p�[�֐�����
 * @author �n�}�c�a���암�J���O���[�v �Ð�M�G
 * $Id$
 */
#ifndef _DRAWFUNCTIONS_H_
#define _DRAWFUNCTIONS_H_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/**
 * @brief <b>�|���S����`�悷��</b>\n
 * �ʏ��{�X�P�[�����ݒ肳��Ă���ꍇ�ɂ��̃X�P�[���ł̕\���ɑ΂��đ��ΓI�Ɋg��E�k������܂����A
 * bIgnoreRefScale��true�ɐݒ肷��ƁA��{�X�P�[�����ꎞ�I�ɖ��������ĕ`�悷�邽�߁A���̂܂܂�
 * �`�悪�Ȃ���܂��i�f�t�H���g��true�ł��j�B\n
 * �܂��A�`�������ۂɂ͕K��StartDrawing()�AFinishDrawing()�ŃV���{���ݒ�y�ѕ`����͂܂Ȃ����
 * �Ȃ�܂��񂪁AIActiveViewEvents::AfterDraw()���ł͎����I�Ɉ͂܂ꂽ���ɂȂ�܂��B���̏ꍇ�ɂ́A
 * bDoStartFinish��false�ɂ��ĉ������i�f�t�H���g�Ftrue�j�B\n
 * bDoRecording��true�ɂ���ƁA���b�Z�[�W�{�b�N�X�����������A�v���P�[�V������؂�ւ�����Ȃǂ�
 * �`�悪�����Ă��܂��̂�h�����Ƃ��ł��܂��B����ɂ��AIActiveViewEvents���C���v�������g���Ȃ��Ă�
 * ITool::Refresh()�ōĕ`�悷��΂悢�Ƃ������ƂɂȂ�܂��i�f�t�H���g�Ffalse�j�B
 * @param ipDisplay			[in]			IDisplay*
 * @param ipSymbol			[in]			ISymbol*
 * @param ipPolygon			[in]			�`�悷��|���S��
 * @param dRefScale			[in,optional]	��{�X�P�[���i�f�t�H���g�F0.0�j�BbIgnoreRefScale��false�̎��ɂ��̒l��0�ȏゾ�Ƃ��̒l����{�X�P�[���Ƃ��Ďg�p�����
 * @param bIgnoreRefScale	[in,optional]	��{�X�P�[���𖳎�����ꍇ��true�i�f�t�H���g�Ftrue�j
 * @param bDoStartFinish	[in,optional]	StartDrawing()�AFinishDrawing()�����̊֐����ł��ė~�����ꍇ��true�i�f�t�H���g�Ftrue�j
 * @param bDoRecording		[in,optional]	�`��L���b�V���ɋL�^����Ȃ�true�i�f�t�H���g�Ffalse�j
 */
void AheDrawPolygon( IDisplay* ipDisplay, ISymbol* ipSymbol, IPolygon* ipPolygon, double dRefScale = 0.0, bool bIgnoreRefScale = true, bool bDoStartFinish = true, bool bDoRecording = false );

/**
 * @brief <b>��`��`�悷��</b>\n
 * @param ipDisplay			[in]			IDisplay*
 * @param ipSymbol			[in]			ISymbol*
 * @param ipEnvelope		[in]			�`�悷���`
 * @param dRefScale			[in,optional]	��{�X�P�[���i�f�t�H���g�F0.0�j�BbIgnoreRefScale��false�̎��ɂ��̒l��0�ȏゾ�Ƃ��̒l����{�X�P�[���Ƃ��Ďg�p�����
 * @param bIgnoreRefScale	[in,optional]	��{�X�P�[���𖳎�����ꍇ��true�i�f�t�H���g�Ftrue�j
 * @param bDoStartFinish	[in,optional]	StartDrawing()�AFinishDrawing()�����̊֐����ł��ė~�����ꍇ��true�i�f�t�H���g�Ftrue�j
 * @param bDoRecording		[in,optional]	�`��L���b�V���ɋL�^����Ȃ�true�i�f�t�H���g�Ffalse�j
 */
void AheDrawRectangle( IDisplay* ipDisplay, ISymbol* ipSymbol, IEnvelope* ipEnvelope, double dRefScale = 0.0, bool bIgnoreRefScale = true, bool bDoStartFinish = true, bool bDoRecording = false );

/**
 * @brief <b>�|�����C����`�悷��</b>\n
 * @param ipDisplay			[in]			IDisplay*
 * @param ipSymbol			[in]			ISymbol*
 * @param ipPolyline		[in]			�`�悷�郉�C��
 * @param dRefScale			[in,optional]	��{�X�P�[���i�f�t�H���g�F0.0�j�BbIgnoreRefScale��false�̎��ɂ��̒l��0�ȏゾ�Ƃ��̒l����{�X�P�[���Ƃ��Ďg�p�����
 * @param bIgnoreRefScale	[in,optional]	��{�X�P�[���𖳎�����ꍇ��true�i�f�t�H���g�Ftrue�j
 * @param bDoStartFinish	[in,optional]	StartDrawing()�AFinishDrawing()�����̊֐����ł��ė~�����ꍇ��true�i�f�t�H���g�Ftrue�j
 * @param bDoRecording		[in,optional]	�`��L���b�V���ɋL�^����Ȃ�true�i�f�t�H���g�Ffalse�j
 */
void AheDrawPolyline( IDisplay* ipDisplay, ISymbol* ipSymbol, IPolyline* ipPolyline, double dRefScale = 0.0, bool bIgnoreRefScale = true, bool bDoStartFinish = true, bool bDoRecording = false );

/**
 * @brief <b>�|�C���g��`�悷��</b>\n
 * @param ipDisplay			[in]			IDisplay*
 * @param ipSymbol			[in]			ISymbol*
 * @param ipPoint			[in]			�`�悷��_
 * @param dRefScale			[in,optional]	��{�X�P�[���i�f�t�H���g�F0.0�j�BbIgnoreRefScale��false�̎��ɂ��̒l��0�ȏゾ�Ƃ��̒l����{�X�P�[���Ƃ��Ďg�p�����
 * @param bIgnoreRefScale	[in,optional]	��{�X�P�[���𖳎�����ꍇ��true�i�f�t�H���g�Ftrue�j
 * @param bDoStartFinish	[in,optional]	StartDrawing()�AFinishDrawing()�����̊֐����ł��ė~�����ꍇ��true�i�f�t�H���g�Ftrue�j
 * @param bDoRecording		[in,optional]	�`��L���b�V���ɋL�^����Ȃ�true�i�f�t�H���g�Ffalse�j
 */
void AheDrawPoint( IDisplay* ipDisplay, ISymbol* ipSymbol, IPoint* ipPoint, double dRefScale = 0.0, bool bIgnoreRefScale = true, bool bDoStartFinish = true, bool bDoRecording = false );

/**
 * @brief <b>�\���_��`�悷��</b>\n
 * @param ipDisplay			[in]			IDisplay*
 * @param ipSymbol			[in]			ISymbol*
 * @param ipGeometry		[in]			�`�悷��\���_�̃W�I���g��
 * @param dRefScale			[in,optional]	��{�X�P�[���i�f�t�H���g�F0.0�j�BbIgnoreRefScale��false�̎��ɂ��̒l��0�ȏゾ�Ƃ��̒l����{�X�P�[���Ƃ��Ďg�p�����
 * @param bIgnoreRefScale	[in,optional]	��{�X�P�[���𖳎�����ꍇ��true�i�f�t�H���g�Ftrue�j
 * @param bDoStartFinish	[in,optional]	StartDrawing()�AFinishDrawing()�����̊֐����ł��ė~�����ꍇ��true�i�f�t�H���g�Ftrue�j
 * @param bDoRecording		[in,optional]	�`��L���b�V���ɋL�^����Ȃ�true�i�f�t�H���g�Ffalse�j
 */
void AheDrawVertex( IDisplay* ipDisplay, ISymbol* ipSymbol, IGeometry* ipGeometry, double dRefScale = 0.0, bool bIgnoreRefScale = true, bool bDoStartFinish = true, bool bDoRecording = false );

/**
 * @brief <b>�}���`�|�C���g��`�悷��</b>\n
 * @param ipDisplay			[in]			IDisplay*
 * @param ipSymbol			[in]			ISymbol*
 * @param ipPoints			[in]			�`�悷��_
 * @param dRefScale			[in,optional]	��{�X�P�[���i�f�t�H���g�F0.0�j�BbIgnoreRefScale��false�̎��ɂ��̒l��0�ȏゾ�Ƃ��̒l����{�X�P�[���Ƃ��Ďg�p�����
 * @param bIgnoreRefScale	[in,optional]	��{�X�P�[���𖳎�����ꍇ��true�i�f�t�H���g�Ftrue�j
 * @param bDoStartFinish	[in,optional]	StartDrawing()�AFinishDrawing()�����̊֐����ł��ė~�����ꍇ��true�i�f�t�H���g�Ftrue�j
 * @param bDoRecording		[in,optional]	�`��L���b�V���ɋL�^����Ȃ�true�i�f�t�H���g�Ffalse�j
 */
void AheDrawMultipoint( IDisplay* ipDisplay, ISymbol* ipSymbol, IMultipoint* ipPoints, double dRefScale = 0.0, bool bIgnoreRefScale = true, bool bDoStartFinish = true, bool bDoRecording = false );

/**
 * @brief <b>������`�悷��</b>\n
 * @param ipDisplay			[in]			IDisplay*
 * @param ipSymbol			[in]			ISymbol*
 * @param ipGeom			[in]			�W�I���g��
 * @param lpcszText			[in]			�`�悷��e�L�X�g
 * @param dRefScale			[in,optional]	��{�X�P�[���i�f�t�H���g�F0.0�j�BbIgnoreRefScale��false�̎��ɂ��̒l��0�ȏゾ�Ƃ��̒l����{�X�P�[���Ƃ��Ďg�p�����
 * @param bIgnoreRefScale	[in,optional]	��{�X�P�[���𖳎�����ꍇ��true�i�f�t�H���g�Ftrue�j
 * @param bDoStartFinish	[in,optional]	StartDrawing()�AFinishDrawing()�����̊֐����ł��ė~�����ꍇ��true�i�f�t�H���g�Ftrue�j
 * @param bDoRecording		[in,optional]	�`��L���b�V���ɋL�^����Ȃ�true�i�f�t�H���g�Ffalse�j
 */
void AheDrawText( IDisplay* ipDisplay, ISymbol* ipSymbol, IGeometry* ipGeom, LPCTSTR lpcszText, double dRefScale = 0.0, bool bIgnoreRefScale = true, bool bDoStartFinish = true, bool bDoRecording = false );

#endif	//_DRAWFUNCTIONS_H_
