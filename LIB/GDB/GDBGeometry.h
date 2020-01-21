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
 *	GDBGeometry.h
 *	@author	Fusatoshi Abe
 *	@date	2003/09/26		�V�K�쐬
 */

#ifndef	___GDB_GEOMETRY_H___
#define	___GDB_GEOMETRY_H___

/**
 * @brief �Q�����b�V�������K�����W�̎擾
 * @param[in]		ipPoint		�Ώۃ|�C���g
 * @param[in,out]	cMesh		�Q�����b�V���R�[�h
 * 								0���w�肵���ꍇ��[out]�ŁAZMeshPos�ɂ��ϊ��œ���ꂽ���b�V���R�[�h�����̂܂ܕԂ�
 * 								��0���w�肵���ꍇ��[in]�ŁAcX, cY���w�肵�����b�V���R�[�h�ł̐��K�����W�ɏC������
 * @param[out]		cX			�Q�����b�V�������K��X���W
 * @param[out]		cY			�Q�����b�V�������K��Y���W
 * @param[in]		ipSpatRef	ipPoint�ɃZ�b�g�����ԎQ�Ɓi�w�肵�Ȃ��ꍇ�̓Z�b�g���Ȃ��j
 * @return	�G���[���
 */
HRESULT GDBGetBaseMeshNormalizeCoord(IPointPtr ipPoint, long *cMesh, long *cX, long *cY, ISpatialReferencePtr ipSpatRef = 0);

HRESULT GDBGetDistance(IPointPtr ipPoint1, IPointPtr ipPoint2, double *cDistance);	// �P�ʂ͂�

/**
 * @brief �W�I���g���ƃ|���S���̊֌W�`�F�b�N
 * @note	ipGeometry�̍\���_�S�Ă��A�uipPolygon�̍\���_��ɂ���(Contains)���A�܂��͕ӏ�ɂ���(Touches)�v���ۂ��𔻒肷��
 * @param[in]	ipGeometry	�ΏۃW�I���g���iPoint, Polyline, Polygon�̂����ꂩ�j
 * @param[in]	ipPolygon	�Ώۃ|���S��
 * @param[out]	cContains	���茋��
 * @return	�G���[���
 */
HRESULT GDBCheckRelation(IGeometryPtr ipGeometry, IPolygonPtr ipPolygon, bool *cContains);

HRESULT GDBGetPolygonEnvelope(IPolygonPtr ipPolygon, IEnvelope **ipEnvelope);

#endif	//___GDB_GEOMETRY_H___
