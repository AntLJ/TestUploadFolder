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


/**
 * @brief ���b�V���R�[�h�����`�ɕϊ�����
 * @see MESH2LL
 * @param lMeshCode	[in]	���b�V���R�[�h
 * @param ipSpRef	[in]	�o�͋�`�ɓK�p�����ԎQ��
 * @return IEnvelopePtr
 */
IEnvelopePtr AheMESH2ENV( unsigned int lMeshCode, ISpatialReference* ipSpRef );

/**
 * @brief ���b�V���R�[�h����|���S���ɕϊ�����
 * @see MESH2LL
 * @param lMeshCode	[in]	���b�V���R�[�h
 * @param ipSpRef	[in]	�o�͋�`�ɓK�p�����ԎQ��
 * @return IGeometryPtr
 */
IGeometryPtr AheMESH2POLY( unsigned int lMeshCode, ISpatialReference* ipSpRef );

/**
 * @brief Point���烁�b�V���R�[�h�{���K��XY�ɕϊ�����
 * @see LL2MESH
 * @param Point	[in]	�|�C���g
 * @param x		[out]	���K�����WX
 * @param y		[out]	���K�����WY
 * @param no_mill	[in,optional]	���K�����W1000000�����e����ꍇ��0�i�f�t�H���g�F0�j
 * @return ���b�V���R�[�h
 */
unsigned int AhePoint2MESH( IPoint* ipPoint, int level, int& x, int& y, int no_mill = 0 );
