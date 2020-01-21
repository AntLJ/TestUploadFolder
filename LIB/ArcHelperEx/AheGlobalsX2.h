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
* @file AheGlobalsX2.h
*
* @brief Arc Helper �O���[�o��2�֐���`�t�@�C��
*/
#if	!defined( __AHEGLOBALSX2_H__)
#define __AHEGLOBALSX2_H__	//!< �d���C���N���[�h�h�~

/**
 * @brief *.lyr �t�@�C������ ILayer ���擾����
 *
 * @param lpszLayerFile		[in]			*.lyr �t�@�C���̃p�X
 * @param ipRetLayer		[out]			�擾���ꂽ ILayer �I�u�W�F�N�g
 *
 * @retval TURE ILayer ���擾�ł���
 * @retval FALSE ILayer ���擾�ł��Ȃ�����
 */
BOOL AheGetLayerFromFile( LPCTSTR lpszLayerFile, ILayer** ipRetLayer );

/**
 * @brief ���C���t�@�C�����烌�C�����擾����
 *
 *   �ǂݍ��񂾃��C���́A�ڑ�����Ă��Ȃ���������Ȃ��̂Œ��ӂ��邱�ƁB
 *
 * @param lpcszFilePath				[in]			���C���t�@�C���̃p�X
 *
 * @retval ILayer
 */
ILayerPtr AheGetLayerFromLyrFile( LPCTSTR lpcszFilePath );

#endif // __AHEGLOBALSX2_H__
