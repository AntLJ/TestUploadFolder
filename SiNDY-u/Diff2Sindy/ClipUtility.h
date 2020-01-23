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

#include "Workspace.h"

namespace sindy {
	class CRowBase;
	class CTableContainer;
} // sindy

ISpatialReferencePtr AheGetSpatialReference( IUnknownPtr ipUnk ); // AheGlobals.h

/**
 * @brief �W�I���g���̃��x���|�C���g���擾����
 * @param geom [in] IGeometryPtr
 * @return ���x���|�C���g
 */
IPointPtr GetLabelPoint( IGeometryPtr geom );

/**
 * @brief �W�I���g��������
 * 
 * 2�̃W�I���g����GeometryBag�Ƃ��ĕԂ��܂��B
 * �^����ꂽ���̂�����GeometryBag�̏ꍇ�́A�΂炵�ē��꒼���܂��B
 * @param geom1 [in] �ǉ��ΏۃW�I���g��1
 * @param geom2 [in] �ǉ��ΏۃW�I���g��2
 * @retval GeometryBag
 */
IGeometryPtr AddGeometry( IGeometryPtr geom1, IGeometryPtr geom2 );

/**
 * @brief �W�I���g�����󂩂ǂ������肷��
 * @param geom [in] ����ΏۃW�I���g��
 * @retval true NULL�A��ł���
 * @retval false ��ł͂Ȃ�
 */
bool IsEmpty( IGeometryPtr geom );

/**
 * @brief �}���`�p�[�g�`���ʃt�B�[�`���Ƃ��ĕ�������
 * @param row [in] �}���`�p�[�g�`���ێ����Ă��邩������Ȃ��t�B�[�`��
 * @param table [in] �V�K�ɍ쐬����ꍇ�̑ΏۃR���e�i
 */
void MultiGeometry2Geometries( sindy::CRowBase& row, sindy::CTableContainer& table );

/**
 * @brief �N���b�v����������
 *
 * �N���b�v�`����ׂ��t�B�[�`���𕪊����܂��B\n
 * �������ꂽ�`��͐V�K�t�B�[�`���Ƃ��č쐬����܂��B
 * 
 * @param con [in,out] �N���b�v�����Ώۃt�B�[�`�����i�[����Ă���R���e�i
 * @param row [in,out] �N���b�v�����Ώۃt�B�[�`��
 * @param ipClip [in] �N���b�v�`��
 */
void Clip( sindy::CTableContainer& con, sindy::CRowBase& row, IGeometryPtr ipClip );
