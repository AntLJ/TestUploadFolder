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
 * @file SymbolFunctions.cpp
 * @brief �F�E�V���{���n�O���[�o���w���p�[�֐�����
 * @author �n�}�c�a���암�J���O���[�v �Ð�M�G
 * $Id$
 */
#ifndef _SYMBOLFUNCTIONS_H_
#define _SYMBOLFUNCTIONS_H_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/**
 * @brief <b>�F���쐬����</b>\n
 * RGB����F���쐬�������ꍇ�́ARGB()�}�N��
 * ���g�p����COLORREF�ɕϊ����ēn���Ă��������B
 * @param color [in]	RGB
 * @return IColorPtr
 */
IColorPtr AheCreateColor( COLORREF color );

/**
 * @brief <b>�|�C���g�V���{�����쐬����</b>\n
 * @param ipColor	[in]			IColor*
 * @param size		[in,optional]	�|�C���g�V���{�����i�f�t�H���g�F1.0�j
 * @param style		[in,optional]	�V���{���X�^�C���i�f�t�H���g�FesriSMSCircle�j
 * @param bDrawXOR	[in,optional]	XOR�ŕ`�悷�邩�ǂ����i�f�t�H���g�Ffalse�j
 * @param bOutline	[in,optional]	�A�E�g���C����`�悷�邩�ǂ����i�f�t�H���g�Ffalse�j
 * @return ISymbolPtr
 */
ISymbolPtr AheCreatePointSymbol( IColor* ipColor, double size = 1.0, esriSimpleMarkerStyle style = esriSMSCircle, bool bDrawXOR = false, bool bOutline = false );

/**
 * @brief <b>�V���v�����C���V���{�����쐬����</b>\n
 * @param ipColor	[in]			IColor*
 * @param width		[in,optional]	�V���{�����i�f�t�H���g�F1.0�j
 * @param style		[in,optional]	�V���{���X�^�C���i�f�t�H���g�FesriSLSSolid�j
 * @param bDrawXOR	[in,optional]	XOR�ŕ`�悷�邩�ǂ����i�f�t�H���g�Ffalse�j
 * @return ISymbolPtr
 */
ISymbolPtr AheCreateLineSymbol( IColor* ipColor, double width = 1.0, esriSimpleLineStyle style = esriSLSSolid, bool bDrawXOR = false );

/**
 * @brief <b>�V���v���|���S���V���{�����쐬����</b>\n
 * @param ipColor	[in]			IColor*
 * @param style		[in,optional]	�V���{���X�^�C���i�f�t�H���g�FesriSLSSolid�j
 * @param bDrawXOR	[in,optional]	XOR�ŕ`�悷�邩�ǂ����i�f�t�H���g�Ffalse�j
 * @return ISymbolPtr
 */
ISymbolPtr AheCreatePolygonSymbol( IColor* ipColor, esriSimpleFillStyle style = esriSFSSolid, bool bDrawXOR = false );

#endif // _SYMBOLFUNCTIONS_H_
