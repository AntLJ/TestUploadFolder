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
 * @file AheGeometryConvert.h
 * @brief <b>�W�I���g���R���o�[�g�n��`�t�@�C��</b>\n
 * @author �R���e���c�{���Z�p�J�������암�J���O���[�v �Ð�M�G
 * ���̊֐��Q�́ASiNDYLib��GeometryConvert.cpp���Q�l�ɍ��܂����B
 * @version $Id$
 */
#pragma once

/**
 * @brief <b>�W�I���g������^����ꂽ�����̃W�I���g���𐶐�����</b>\n
 * ���L�ϊ��ȊO�Ŏ����������ꍇ�̓R�s�[�𐶐����܂��B\n
 * �T�|�[�g���Ă���ϊ��͎��̒ʂ�F\n
 * @li Ring     -> Polygon
 * @li Envelope -> Polygon
 * @li Polygon  -> Polyline
 * @li Ring     -> Polyline
 * @li Envelope -> Polyline
 * @li Path     -> Polyline
 * @li Polygon  -> Multipoint
 * @li Ring     -> Multipoint
 * @li Envelope -> Multipoint
 * @li Polyline -> Multipoint
 * @li Path     -> Multipoint
 * @li Point    -> Multipoint
 * @param ipGeom	[in]	�W�I���g��
 * @param emGeomDim	[in]	����
 * @retval NULL		�G���[
 * @retval NULL�ȊO	�ϊ���̃W�I���g��
 */
IGeometryPtr Convert( IGeometry* ipGeom, esriGeometryDimension emGeomDim );

/**
 * @brief <b>�W�I���g������}���`�|�C���g�𐶐�����</b>\n
 * �^����ꂽ�W�I���g�����}���`�|�C���g�̏ꍇ�̓R�s�[�𐶐����܂��B\n
 * �T�|�[�g���Ă���W�I���g���^�C�v�͈ȉ��̂Ƃ���F\n
 * @li Polygon
 * @li Ring
 * @li Envelope
 * @li Polyline
 * @li Path
 * @li Point
 * @param ipGeom	[in]	�W�I���g��
 * @retval NULL		�G���[
 * @retval NULL�ȊO	�ϊ���̃W�I���g��
 */
IGeometryPtr ToMultipoint( IGeometry* ipGeom );

/**
 * @brief <b>�W�I���g������|�����C���𐶐�����</b>\n
 * �^����ꂽ�W�I���g�����|�����C���̏ꍇ�̓R�s�[�𐶐����܂��B\n
 * �T�|�[�g���Ă���W�I���g���^�C�v�͈ȉ��̂Ƃ���F\n
 * @li Polygon
 * @li Ring
 * @li Envelope
 * @param ipGeom	[in]	�W�I���g��
 * @retval NULL		�G���[
 * @retval NULL�ȊO	�ϊ���̃W�I���g��
 */
IGeometryPtr ToPolyline( IGeometry* ipGeom );

/**
 * @brief <b>�W�I���g������p�X�𐶐�����</b>\n
 * �^����ꂽ�W�I���g�����p�X�̏ꍇ�̓R�s�[�𐶐����܂��B\n
 * �T�|�[�g���Ă���W�I���g���^�C�v�͈ȉ��̂Ƃ���F\n
 * @li Ring
 * @li Envelope
 * @param ipGeom	[in]	�W�I���g��
 * @retval NULL		�G���[
 * @retval NULL�ȊO	�ϊ���̃W�I���g��
 */
IGeometryPtr ToPath( IGeometry* ipGeom );

/**
 * @brief <b>�W�I���g������|���S���𐶐�����</b>\n
 * �^����ꂽ�W�I���g�����|���S���̏ꍇ�̓R�s�[�𐶐����܂��B\n
 * �T�|�[�g���Ă���W�I���g���^�C�v�͈ȉ��̂Ƃ���F\n
 * @li Ring
 * @li Envelope
 * @param ipGeom	[in]	�W�I���g��
 * @retval NULL		�G���[
 * @retval NULL�ȊO	�ϊ���̃W�I���g��
 */
IGeometryPtr ToPolygon( IGeometry* ipGeom );

/**
 * @brief <b>�W�I���g�����烊���O�𐶐�����</b>\n
 * �^����ꂽ�W�I���g���������O�̏ꍇ�̓R�s�[�𐶐����܂��B\n
 * �T�|�[�g���Ă���W�I���g���^�C�v�͈ȉ��̂Ƃ���F\n
 * @li Envelope
 * @param ipGeom	[in]	�W�I���g��
 * @retval NULL		�G���[
 * @retval NULL�ȊO	�ϊ���̃W�I���g��
 */
IGeometryPtr ToRing( IGeometry* ipGeom );
