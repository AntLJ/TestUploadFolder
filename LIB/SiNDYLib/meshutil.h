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
 * @file meshutil.h
 * @brief ���b�V������p�O���֐���`�t�@�C��\n
 * ���̃t�@�C����crd_cnv�̃��b�v�֐��Q�̒�`��������Ă��܂��B
 * @author �n�}�c�a���암�V�X�e���J���O���[�v �Ð�M�G
 * $Id$
 */
#ifndef _MESHUTIL_H_
#define _MESHUTIL_H_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "exportimport.h"

/**
 * @namespace sindy::mesh_type
 * @brief ���b�V���^�C�v�l�[���X�y�[�X
 */
namespace mesh_type { // TODO: sindyAnnotationGroup�ő�p�ł���̂ł���Ȃ��͂�
	/**
	 * @enum sindy::mesh_type::ECode
	 * @brief ���b�V���^�C�v
	 */
	enum ECode {
		kCity,			//!< �s�s�n�}���b�V��
		kBase,			//!< �x�[�X���b�V��
		kMiddle,		//!< �~�h�����b�V��
		kTop,			//!< �g�b�v���b�V��
	};
} // mesh_type

/**
 * @brief ���b�V���R�[�h�{���K��XY����ܓx�o�x�ɕϊ�����
 * @see crd_cnv::MeshToLL
 * @param lMeshCode	[in]	���b�V���R�[�h
 * @param x			[in]	���K�����WX
 * @param y			[in]	���K�����WY
 * @param lon		[out]	�o�x
 * @param lat		[out]	�ܓx
 */
void SINDYLIB_API MESH2LL( unsigned int lMeshCode, int x, int y, double& lon, double& lat );

/**
 * @brief ���b�V���R�[�h����ܓx�o�x�ɕϊ�����
 * @see MESH2LL
 * @param lMeshCode	[in]	���b�V���R�[�h
 * @param dMinLon	[out]	�ŏ��o�x
 * @param dMinLat	[out]	�ŏ��ܓx
 * @param dMaxLon	[out]	�ő�o�x
 * @param dMaxLat	[out]	�ő�ܓx
 */
void SINDYLIB_API MESH2ENV( unsigned int lMeshCode, double& dMinLon, double& dMinLat, double& dMaxLon, double& dMaxLat );

/**
 * @brief ���b�V���R�[�h�����`�ɕϊ�����
 * @see MESH2LL
 * @param lMeshCode	[in]	���b�V���R�[�h
 * @param ipSpRef	[in]	�o�͋�`�ɓK�p�����ԎQ��
 * @return IEnvelopePtr
 */
IEnvelopePtr SINDYLIB_API MESH2ENV( unsigned int lMeshCode, ISpatialReference* ipSpRef );

/**
 * @brief ���b�V���R�[�h����|���S���ɕϊ�����
 * @see MESH2LL
 * @param lMeshCode	[in]	���b�V���R�[�h
 * @param ipSpRef	[in]	�o�͋�`�ɓK�p�����ԎQ��
 * @return IGeometryPtr
 */
IGeometryPtr SINDYLIB_API MESH2POLY( unsigned int lMeshCode, ISpatialReference* ipSpRef );

/**
 * @brief �ܓx�o�x���烁�b�V���R�[�h�{���K��XY�ɕϊ�����
 * @see crd_cnv::LLtoMesh
 * @param dLon	[in]	�o�x
 * @param dLat	[in]	�ܓx
 * @param level	[in]	1:�ꎟ���b�V�� 2:�񎟃��b�V�� 64:�s�s�n�}���b�V��
 * @param x		[out]	���K�����WX
 * @param y		[out]	���K�����WY
 * @param no_mill	[in,optional]	���K�����W1000000�����e����ꍇ��0�i�f�t�H���g�F0�j
 * @return ���b�V���R�[�h
 */
unsigned int SINDYLIB_API LL2MESH( const double& dLon, const double& dLat, int level, int& x, int& y, int no_mill = 0 );

/**
 * @brief �ܓx�o�x����ꎟ���b�V���R�[�h�{���K��XY�ɕϊ�����
 * @see LL2MESH
 * @param dLon	[in]	�o�x
 * @param dLat	[in]	�ܓx
 * @param x		[out]	���K�����WX
 * @param y		[out]	���K�����WY
 * @param no_mill	[in,optional]	���K�����W1000000�����e����ꍇ��0�i�f�t�H���g�F0�j
 * @return ���b�V���R�[�h
 */
unsigned int SINDYLIB_API LL2MESH1st( const double& dLon, const double& dLat, int& x, int& y, int no_mill = 0 );

/**
 * @brief �ܓx�o�x����񎟃��b�V���R�[�h�{���K��XY�ɕϊ�����
 * @see LL2MESH
 * @param dLon	[in]	�o�x
 * @param dLat	[in]	�ܓx
 * @param x		[out]	���K�����WX
 * @param y		[out]	���K�����WY
 * @param no_mill	[in,optional]	���K�����W1000000�����e����ꍇ��0�i�f�t�H���g�F0�j
 * @return ���b�V���R�[�h
 */
unsigned int SINDYLIB_API LL2MESH2nd( const double& dLon, const double& dLat, int& x, int& y, int no_mill = 0 );

/**
 * @brief �ܓx�o�x����s�s�n�}���b�V���R�[�h�{���K��XY�ɕϊ�����
 * @see LL2MESH
 * @param dLon	[in]	�o�x
 * @param dLat	[in]	�ܓx
 * @param x		[out]	���K�����WX
 * @param y		[out]	���K�����WY
 * @param no_mill	[in,optional]	���K�����W1000000�����e����ꍇ��0�i�f�t�H���g�F0�j
 * @return ���b�V���R�[�h
 */
unsigned int SINDYLIB_API LL2MESHcity( const double& dLon, const double& dLat, int& x, int& y, int no_mill = 0 );

/**
 * @brief Point���烁�b�V���R�[�h�{���K��XY�ɕϊ�����
 * @see LL2MESH
 * @param Point	[in]	�|�C���g
 * @param x		[out]	���K�����WX
 * @param y		[out]	���K�����WY
 * @param no_mill	[in,optional]	���K�����W1000000�����e����ꍇ��0�i�f�t�H���g�F0�j
 * @return ���b�V���R�[�h
 */
unsigned int SINDYLIB_API Point2MESH( IPoint* ipPoint, int level, int& x, int& y, int no_mill = 0 );

/**
 * @brief <b>�n���ꂽ���b�V���R�[�h�̉E�ׂ̃��b�V���R�[�h��Ԃ�</b>\n
 * @param code	[in]	���b�V���R�[�h
 * @return �E�ׂ̃��b�V���R�[�h
 */
unsigned int SINDYLIB_API INCMESH_X( unsigned int code );

/**
 * @brief <b>�n���ꂽ���b�V���R�[�h�̏�ׂ̃��b�V���R�[�h��Ԃ�</b>\n
 * @param code	[in]	���b�V���R�[�h
 * @return ��ׂ̃��b�V���R�[�h
 */
unsigned int SINDYLIB_API INCMESH_Y( unsigned int code );

/**
 * @brief <b>�n���ꂽ���b�V���R�[�h�̍��ׂ̃��b�V���R�[�h��Ԃ�</b>\n
 * @param code	[in]	���b�V���R�[�h
 * @return ���ׂ̃��b�V���R�[�h
 */
unsigned int SINDYLIB_API DECMESH_X( unsigned int code );

/**
 * @brief <b>�n���ꂽ���b�V���R�[�h�̉��ׂ̃��b�V���R�[�h��Ԃ�</b>\n
 * @param code	[in]	���b�V���R�[�h
 * @return ���ׂ̃��b�V���R�[�h
 */
unsigned int SINDYLIB_API DECMESH_Y( unsigned int code );

/**
 * @brief <b>2�����b�V���A�s�s�n�}���b�V������1�����b�V���R�[�h��Ԃ�</b>\n
 * @param code	[in]	���b�V���R�[�h
 * @retval -1		�G���[
 * @retval -1�ȊO	1�����b�V���R�[�h
 */
unsigned int SINDYLIB_API MESH2MESH1ST( unsigned int code );

#endif // _MESHUTIL_H_
