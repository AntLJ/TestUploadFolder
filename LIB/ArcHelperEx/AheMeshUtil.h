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
#ifndef _AHE_MESHUTIL_H_
#define _AHE_MESHUTIL_H_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef AHE_UNUSE_ARCOBJECTS
	#include "AheMeshUtilArcObjects.h"
#endif

#if 0
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
#endif // if 0

/**
 * @brief ���E���b�V���ҏW���[�h���ǂ����𒲂ׂ�
 * @return ���E���b�V���ҏW���[�h�Ȃ�true
 */
bool AheIsWorldMeshMode();

/**
 * @brief ���b�V���R�[�h�{���K��XY����ܓx�o�x�ɕϊ�����
 * @see crd_cnv::MeshToLL
 * @param lMeshCode	[in]	���b�V���R�[�h
 * @param x			[in]	���K�����WX
 * @param y			[in]	���K�����WY
 * @param lon		[out]	�o�x
 * @param lat		[out]	�ܓx
 */
void AheMESH2LL( unsigned int lMeshCode, int x, int y, double& lon, double& lat );

/**
 * @brief ���b�V���R�[�h����ܓx�o�x�ɕϊ�����
 * @see MESH2LL
 * @param lMeshCode	[in]	���b�V���R�[�h
 * @param dMinLon	[out]	�ŏ��o�x
 * @param dMinLat	[out]	�ŏ��ܓx
 * @param dMaxLon	[out]	�ő�o�x
 * @param dMaxLat	[out]	�ő�ܓx
 */
void AheMESH2ENV( unsigned int lMeshCode, double& dMinLon, double& dMinLat, double& dMaxLon, double& dMaxLat );


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
unsigned int AheLL2MESH( const double& dLon, const double& dLat, int level, int& x, int& y, int no_mill = 0 );

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
unsigned int AheLL2MESH1st( const double& dLon, const double& dLat, int& x, int& y, int no_mill = 0 );

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
unsigned int AheLL2MESH2nd( const double& dLon, const double& dLat, int& x, int& y, int no_mill = 0 );

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
unsigned int AheLL2MESHcity( const double& dLon, const double& dLat, int& x, int& y, int no_mill = 0 );


/**
 * @brief �n���ꂽ���b�V���R�[�h����ix,y�j���ꂽ���b�V���R�[�h�����߂�
 * @param code  [in] ���b�V���R�[�h
 * @param xdist [in] X�����։����b�V������Ă��邩
 * @param ydist [in] Y�����։����b�V������Ă��邩
 * @retval -1 ���s
 * @retval -1�ȊO ���b�V���R�[�h
 */
unsigned int AheGETMESHCODE( unsigned int code, int xdist, int ydist );

/**
 * @brief <b>�n���ꂽ���b�V���R�[�h�̉E�ׂ̃��b�V���R�[�h��Ԃ�</b>\n
 * @param code	[in]	���b�V���R�[�h
 * @return �E�ׂ̃��b�V���R�[�h
 */
unsigned int AheINCMESH_X( unsigned int code );

/**
 * @brief <b>�n���ꂽ���b�V���R�[�h�̏�ׂ̃��b�V���R�[�h��Ԃ�</b>\n
 * @param code	[in]	���b�V���R�[�h
 * @return ��ׂ̃��b�V���R�[�h
 */
unsigned int AheINCMESH_Y( unsigned int code );

/**
 * @brief <b>�n���ꂽ���b�V���R�[�h�̍��ׂ̃��b�V���R�[�h��Ԃ�</b>\n
 * @param code	[in]	���b�V���R�[�h
 * @return ���ׂ̃��b�V���R�[�h
 */
unsigned int AheDECMESH_X( unsigned int code );

/**
 * @brief <b>�n���ꂽ���b�V���R�[�h�̉��ׂ̃��b�V���R�[�h��Ԃ�</b>\n
 * @param code	[in]	���b�V���R�[�h
 * @return ���ׂ̃��b�V���R�[�h
 */
unsigned int AheDECMESH_Y( unsigned int code );

/**
 * @brief <b>2�����b�V���A�s�s�n�}���b�V������1�����b�V���R�[�h��Ԃ�</b>\n
 * @param code	[in]	���b�V���R�[�h
 * @retval -1		�G���[
 * @retval -1�ȊO	1�����b�V���R�[�h
 */
unsigned int AheMESH2MESH1ST( unsigned int code );

/**
 * @brief 2�_�ԁi�ܓx�o�x�j�̋����im�j�����߂�
 * @param lon1 [in] �n�_1 X
 * @param lat1 [in] �n�_1 Y
 * @param lon2 [in] �n�_2 X
 * @param lat2 [in] �n�_2 Y
 * @retval �����im�j
 */
double AheGETDIST( const double& lon1, const double& lat1, const double& lon2, const double& lat2 );

/**
 * @brief 2�n�_�̌o�ܓx����A2�_�Ԃ̋��� + ����(X,Y)m�����߂�
 * @param lon1  [in]  �n�_1 X
 * @param lat1  [in]  �n�_1 Y
 * @param lon2  [in]  �n�_2 X
 * @param lat2  [in]  �n�_2 Y
 * @param distX [out] ���� X
 * @param distY [out] ���� Y
 * @retval �����im�j
 */
double AheGETDISTXY( const double& lon1, const double& lat1, const double& lon2, const double& lat2, double& distX, double& distY);

/**
 * @brief �o�ܓx�Ƃ�������o�x������X(m),�ܓx������Y(m)���ꂽ�Ƃ���̌o�ܓx�����߂�
 * @param lon1  [in]  �n�_1 X
 * @param lat1  [in]  �n�_1 Y
 * @param distX [in]  ���� X
 * @param distY [in]  ���� Y
 * @param lon2  [out] �n�_2 X
 * @param lat2  [out] �n�_2 Y
 * @retval �����܂ł̉�
 */
int AheGETLLPT( const double& lon1, const double& lat1, const double& distX, const double& distY, double& lon2, double& lat2);

/**
 * @brief 2�_�ԁi���b�V��XY�j�̍��������߂�
 * @param mesh1 [in] ���b�V���R�[�h�P
 * @param x1    [in] ���K��X���W1
 * @param y1    [in] ���K��Y���W1
 * @param mesh2 [in] ���b�V���R�[�h�Q
 * @param x2    [in] ���K��X���W2
 * @param y2    [in] ���K��Y���W2
 * @param dx    [out] X����
 * @param dy    [out] Y����
 * @retval  0 ����
 * @retval -1 ���s
 */
int AheGETDIFF( unsigned int mesh1, int x1, int y1, unsigned int mesh2, int x2, int y2, double& dx, double& dy );

struct coord19;

/**
 * @brief �o�ܓx����ł��߂��P�X���W�n�̌n�Ƃw�x��Ԃ�
 * @param lon [in] X
 * @param lat [in] Y
 * @param sys_xy [out] �n�{XY
 */
void AheLL2XY19( double lon, double lat, coord19 *sys_xy );

/**
 * @brief �o�ܓx����UTM���W�n�̃]�[������Ԃ�
 * @param lon [in] X
 * @param lat [in] Y
 * @param zone [out] XY
 */
int AheLL2UTM( double lon, double lat, double *dX, double *dY ); 

/**
 * @brief �ܓx�o�x�n����UTM���W�n�̍��W��Ԃ�
 * @param nZone [in] UTM�]�[��
 * @param lon [in] X
 * @param lat [in] Y
 * @param sys_xy [out] �n�{XY
 */
void AheLLtoUTM_WGS84( int nZone,  double lon, double lat, double *dX, double *dY );

struct d_lonlat;

/**
 * @brief �P�X���W�n�̂w�x����o�ܓx�֕ϊ�
 * @param axis [in] �Y��������W�n�̔ԍ�
 * @param x [in] 19���W�n��X(�ܓx����)
 * @param y [in] 19���W�n��Y(�o�x����)
 * param d_lonlat [out] 10�i�ŕ\�����o�x�ܓx(ex:130.23432342....)
 */
void AheXY192LL( int axis, double x, double y, d_lonlat *lonlat );
/**
 * @brief UTM���W�n����A�ܓx�o�x��Ԃ�
 * @param nZone [in] UTM�]�[��
 * @param lon [in] X
 * @param lat [in] Y
 * @param sys_xy [out] �n�{XY
 */
void AheUTM2LL_WGS84( int nZone, double x, double y, double *dX, double *dY );

#endif // _AHE_MESHUTIL_H_
