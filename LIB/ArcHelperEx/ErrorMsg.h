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
 * @file ErrorMsg.h
 * @brief <b>�G���[�E�x�����b�Z�[�W�Ǘ��t�@�C��</b><br>
 * ArcHelperEx���Ŏg�p�����G���[�x�����b�Z�[�W���Ǘ����܂�<br>
 * �v���v���Z�b�T SINDY_FOR_ENGLISH ����`����Ă���ꍇ�A���b�Z�[�W�̎��͉̂p��ɂȂ�܂�<br>
 * �v���v���Z�b�T SINDY_FOR_ENGLISH ����`����Ă��Ȃ��ꍇ�A���b�Z�[�W�̎��͓̂��{��ɂȂ�܂�<br>
 * @warning 2015/03/03�̒i�K�ł́ATargetFeatures���̃��b�Z�[�W�̂݊Ǘ����Ă��܂�<br>
 * @author s_kato (INCREMENT P CORP.) <s_kato@mr.ipc.pioneer.co.jp>
 * $Id$
 */

namespace libErrMsg
{

/**
 * @brief TargetFeatures���̃��b�Z�[�W�Q
 */
extern LPCTSTR ERROR_ROAD_LINK_IS_FOLDED_MESH_BOUNDARY;
extern LPCTSTR GETUNION_LAYER_OID;
extern LPCTSTR ERROR_FROM_OR_TO_MISSING;
extern LPCTSTR CONFIRM_CHECK_POLYGON_WITH_OVER_10000_POINT;
extern LPCTSTR GEOMETRY_CHECK;
extern LPCTSTR ERROR_DUPLICATE_OF_OID;
extern LPCTSTR ERROR_LINK_MUST_HAVE_FROM_OR_TO_ID;
extern LPCTSTR ERROR_LINK_LOOPES;
extern LPCTSTR ERROR_SEGMENT_OVERLAP;
extern LPCTSTR ERROR_SAME_TYPE_SEGMENT_OVERLAP;
extern LPCTSTR ERROR_ANGLE_IS_135_DEGREE_OR_LESS;
extern LPCTSTR ERROR_POINTS_ARE_TOUCHING;
extern LPCTSTR ERROR_ANGLE_IS_20_DEGREE_OR_LESS;
extern LPCTSTR ERROR_SEGMENT_CONNECT_SAME_NODE_IS_CLOSE;

} // namespace libErrMsg