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

#ifndef ARCHELPEREX_STRINGCONVERT_H__
#define ARCHELPEREX_STRINGCONVERT_H__
/**
 * @file StringConvert.h
 * @brief �����񐳋K���H�֐��Q��`�t�@�C��
 */

/**
* @brief �����������_���̂ɏC������
*
* @note [���[��]
* @note 1. �������̂ɑS�p�����ŁA�����A�������A�Љ����A�����A�p���A�u�A�v�u�Y�v�u�[�v�u�X�v�u�|�v�u�i�v�u�j�v�ȊO�̕������܂܂�Ă͂����Ȃ�
* @note 2. �������̂Ő擪�������L���ł͂����Ȃ�
* @note 3. �������̂�'�['���Љ����ƂЂ炪�ȈȊO�̌�ɂ����Ă͂����Ȃ�
* @note 4. �������̂�'�|'�������ȊO�̌�ɂ����Ă͂����Ȃ�
* @note 5. �J�i���̂ɔ��p�J�^�J�i�ȊO�̕������܂܂�Ă��Ă͂����Ȃ�		���u�S�p�J�^�J�i�ȊO�v�ł́H
*
* @param lpcszText			[in]	�����i�J�i�j������
* @param bIsKanji			[in]	�����̏ꍇ�� TRUE�A�J�i�̏ꍇ�� FALSE
*
* @retval CString �ϊ���̕�����
*/
CString AheConvertToIntersectionName( LPCTSTR lpcszText, BOOL bIsKanji );

/**
* @brief ���������ʈē����̂ɏC������
*
* @note [���[��]
* @note 1. �������̂ɑS�p�����ŁA�����A�������A�Љ����A�����A�p���A�u�A�v�u�Y�v�u�[�v�u�X�v�u�i�v�u�j�v�u�D�v�u�`�v�u�E�v�ȊO�̕������܂܂�Ă͂����Ȃ�
* @note 4. �J�i���̂ɑS�p�����ŁA�Љ����A�����A�p���A�u�E�v�u�A�v�u�[�v�u�i�v�u�j�v�ȊO�̕������܂܂�Ă͂����Ȃ�
* @note 2. �������́A�J�i���̂Ő擪�������L���ł͂����Ȃ�
* @note 3. �������́A�J�i���̂�'�['���Љ����ȊO�̌�ɂ����Ă͂����Ȃ�
*
* @param lpcszText			[in]	�����i�J�i�j������
* @param bIsKanji			[in]	�����̏ꍇ�� TRUE�A�J�i�̏ꍇ�� FALSE
*
* @retval CString �ϊ���̕�����
*/
CString AheConvertToDirguideName( LPCTSTR lpcszText, BOOL bIsKanji );

/**
* @brief �������A�N�Z�X�|�C���g�̕��ʈē��ɏC������
*
* @note [���[��]
* @note 1. �������̂ɑS�p�����ŁA�����A�������A�Љ����A�����A�p���A�u�A�v�u�Y�v�u�[�v�u�X�v�u�i�v�u�j�v�u�D�v�u�`�v�u�E�v�ȊO�̕������܂܂�Ă͂����Ȃ�
* @note 2. �������̂Ő擪�������L���ł͂����Ȃ�
* @note 3. �������̂�'�['���Љ����ȊO�̌�ɂ����Ă͂����Ȃ�
* @note 4. �������̂�'�|'�������ȊO�̌�ɂ����Ă͂����Ȃ�
* @note 5. �J�i���̂ɔ��p�J�^�J�i�ƃi�J�O���u�E�v�̈ȊO�̕������܂܂�Ă��Ă͂����Ȃ�
*
* @param lpcszText			[in]	�����i�J�i�j������
* @param bIsKanji			[in]	�����̏ꍇ�� TRUE�A�J�i�̏ꍇ�� FALSE
*
* @retval CString �ϊ���̕�����
*/
CString AheConvertToAccessPointName( LPCTSTR lpcszText, BOOL bIsKanji );

/**
* @brief �������{�ݏ�񖼏̂ɏC������
*
* @note [���[��]
* @note 1. �������̂ɑS�p�����ŁA�����A�������A�Љ����A�����A�p���A�u�A�v�u�Y�v�u�[�v�u�X�v�u�|�v�u�i�v�u�j�v�ȊO�̕������܂܂�Ă͂����Ȃ�
* @note 2. �������̂Ő擪�������L���ł͂����Ȃ�
* @note 3. �������̂�'�['���Љ����ȊO�̌�ɂ����Ă͂����Ȃ�
* @note 4. �������̂�'�|'�������ȊO�̌�ɂ����Ă͂����Ȃ�
* @note 5. �J�i���̂ɔ��p�J�^�J�i�ȊO�̕������܂܂�Ă��Ă͂����Ȃ�
*
* @param lpcszText			[in]	�����i�J�i�j������
* @param bIsKanji			[in]	�����̏ꍇ�� TRUE�A�J�i�̏ꍇ�� FALSE
*
* @retval CString �ϊ���̕�����
*/
CString AheConvertToFacilInfoName( LPCTSTR lpcszText, BOOL bIsKanji );

/**
 * @brief ���L�Ɏg���镶�����K���ȕ�����ɏC��
 *
 * @param lpcszText         [in]			���̕�����
 * @param bIsFull           [in]			������̌`���iTRUE:�S�p�@FALSE:���p�j
 * @param bEnabelAlphabet	[in]			�p�����g�p���邩�ǂ��� (TRUE:�g�p����, FALSE:�g�p���Ȃ�)
 * @param bUseMinusPeriod   [in,optional]	�}�C�i�X�L���A�����_���g�p���邩�ǂ���
 * @param pstrErr			[out,optional]	�G���[�ƂȂ��������񂪓���i�擪�̏ꍇ�ɒe�������̂͏����j
 *
 * @retval �C�����ꂽ������
 */
CString AheConvertToAnnoJustStr( LPCTSTR lpcszText, BOOL bIsFull, BOOL bUseAlphabet, BOOL bUseMinusPeriod = FALSE, CString* pstrErr = NULL );

/**
 * @brief ���e�ԍ��Ɏg���镶�����K���ȕ�����ɏC��
 *
 * @param lpcszText         [in]			���̕�����
 *
 * @retval �C�����ꂽ������
 */
CString AheConvertToGenkouNoStr( LPCTSTR lpcszText );

/**
 * @brief ���t�Ɏg���镶�����K���ȕ�����ɏC��
 *
 * @param lpcszText         [in]			���̕�����
 *
 * @retval �C�����ꂽ������
 */
CString AheConvertToDateStr( LPCTSTR lpcszText );

#endif // ifndef ARCHELPEREX_STRINGCONVERT_H__
