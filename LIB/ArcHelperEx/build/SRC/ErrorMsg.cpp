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

#include "StdAfx.h"
#include "ErrorMsg.h"

namespace libErrMsg
{

#ifndef SINDY_FOR_ENGLISH
LPCTSTR ERROR_ROAD_LINK_IS_FOLDED_MESH_BOUNDARY     = _T("%d - %d �������b�V�����œ��H�����N���܂�Ԃ��Ă��܂��B");
LPCTSTR GETUNION_LAYER_OID                          = _T("CTargetItems::GetUnion() : ���C��=%s OID=%d");
LPCTSTR ERROR_FROM_OR_TO_MISSING                    = _T("FROM�ATO�ǂ��炩�������Ă��܂���B");
LPCTSTR CONFIRM_CHECK_POLYGON_WITH_OVER_10000_POINT = _T("���C���F%s OID=%d : �\���_����10000�ȏ�i%d�j�̃|���S���ɑ΂��ă`�F�b�N���s���Ǝ��Ԃ�������܂��B�`�F�b�N���܂����H");
LPCTSTR GEOMETRY_CHECK                              = _T("�W�I���g���`�F�b�N");
LPCTSTR ERROR_DUPLICATE_OF_OID                      = _T("OID=%d�̃|���S���Əd�����Ă��܂��B");
LPCTSTR ERROR_LINK_MUST_HAVE_FROM_OR_TO_ID          = _T("�l�b�g���[�N�����N��FROM����TO��ID�����Ă��܂���B");
LPCTSTR ERROR_LINK_LOOPES                           = _T("%d - %d ���H�����N�����[�v���Ă��܂��B");
LPCTSTR ERROR_SEGMENT_OVERLAP                       = _T("%d - %d �Z�O�����g���I�[�o�[���b�v���Ă��܂��B");
LPCTSTR ERROR_SAME_TYPE_SEGMENT_OVERLAP             = _T("%d - %d ����ʂŃZ�O�����g���I�[�o�[���b�v���Ă��܂��B");
LPCTSTR ERROR_ANGLE_IS_135_DEGREE_OR_LESS           = _T("%d - %d ���b�V�������͂��񂾐ڑ��p��135���ȉ��ł��i%d���j�B");
LPCTSTR ERROR_POINTS_ARE_TOUCHING                   = _T("%d - %d �\���_�ǂ������ڐG���Ă��܂�");
LPCTSTR ERROR_ANGLE_IS_20_DEGREE_OR_LESS            = _T("%d - %d 2���H�̐ڑ��p��20���ȉ��ł��i%d���j�B");
LPCTSTR ERROR_SEGMENT_CONNECT_SAME_NODE_IS_CLOSE    = _T("%ld - %ld ����m�[�h�ɐڑ�����������ߐڂ��Ă��܂��i���K�����W���� %.1lf�j");
#else
LPCTSTR ERROR_ROAD_LINK_IS_FOLDED_MESH_BOUNDARY     = _T("%d - %d�@Road link is folded at the mesh boundary within a same mesh.");
LPCTSTR GETUNION_LAYER_OID                          = _T("CTargetItems::GetUnion() : Layer=%s OID=%d");
LPCTSTR ERROR_FROM_OR_TO_MISSING                    = _T("Either FROM or TO is missing");
LPCTSTR CONFIRM_CHECK_POLYGON_WITH_OVER_10000_POINT = _T("Layer: %s OID=%d: A long time is required to check a polygon with 10,000 or more (%d) composition points. Do you want to perform the check?");
LPCTSTR GEOMETRY_CHECK                              = _T("Geometry check");
LPCTSTR ERROR_DUPLICATE_OF_OID                      = _T("Duplicate of OID=%d polygon.");
LPCTSTR ERROR_LINK_MUST_HAVE_FROM_OR_TO_ID          = _T("Link must has FROM or TO ID.");
LPCTSTR ERROR_LINK_LOOPES                           = _T("%d - %d Link Loopes.");
LPCTSTR ERROR_SEGMENT_OVERLAP                       = _T("%d - %d  Segments overlap.");
LPCTSTR ERROR_SAME_TYPE_SEGMENT_OVERLAP             = _T("%d - %d  Same type and segments overlap.");
LPCTSTR ERROR_ANGLE_IS_135_DEGREE_OR_LESS           = _T("%d - %d Connection angle across the mesh boundary is 135 degree or less.");
LPCTSTR ERROR_POINTS_ARE_TOUCHING                   = _T("%d - %d  A pair of composition points are touching.");
LPCTSTR ERROR_ANGLE_IS_20_DEGREE_OR_LESS            = _T("%d - %d  The angle of the fork in a road is 20 degrees or less (%d degrees).");
LPCTSTR ERROR_SEGMENT_CONNECT_SAME_NODE_IS_CLOSE    = _T("%d - %d Line segments connecting to a same node is close to each other  (Normarized coordinates distance:  %.1lf)");
#endif // SINDY_FOR_ENGLISH

} // namespace libErrMsg
