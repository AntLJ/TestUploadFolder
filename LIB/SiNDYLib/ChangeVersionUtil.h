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
 * @file ChangeVersionUtil.h
 * @brief <b>�o�[�W�����؂�ւ����[�e�B���e�B�֐���`�t�@�C��</b>\n
 * @author �n�}�c�a���암�J���O���[�v �Ð�M�G
 * $Id$
 */
#ifndef _CHANGEVERSIONUTIL_H_
#define _CHANGEVERSIONUTIL_H_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "ErrorCode.h"

namespace sindy {

/**
 * @brief <b>�o�[�W������ύX����</b>\n
 * @param ipMxDoc				[in]	IMxDocument
 * @param ipOldWorkspace		[in]	�o�[�W�����ύX�O�̃��[�N�X�y�[�X
 * @param lpcszNewVersionName	[in]	�V�����o�[�W������
 * @retval sindyErr_NoErr	�o�[�W�����ύX����
 * @retval ��L�ȊO			�o�[�W�����ύX���s
 */
errorcode::sindyErrCode SINDYLIB_API AheChangeVersionByName( IMxDocument* ipMxDoc, IFeatureWorkspace* ipOldWorkspace, LPCTSTR lpcszNewVersionName );

/**
 * @brief <b>�o�[�W������ύX����</b>\n
 * @param ipMxDoc				[in]	IMxDocument
 * @param ipOldWorkspace		[in]	�o�[�W�����ύX�O�̃��[�N�X�y�[�X
 * @param ipNewWorkspace		[in]	�o�[�W�����ύX��̃��[�N�X�y�[�X
 * @retval sindyErr_NoErr	�o�[�W�����ύX����
 * @retval ��L�ȊO			�o�[�W�����ύX���s
 */
errorcode::sindyErrCode SINDYLIB_API AheChangeVersion( IMxDocument* ipMxDoc, IFeatureWorkspace* ipOldWorkspace, IFeatureWorkspace* ipNewWorkspace );

/**
 * @brief <b>�n���ꂽ�o�[�W�����̐e�o�[�W�������擾����</b>\n
 * @param ipVersion			[in]	�o�[�W����
 * @retval �e�o�[�W����
 */
IVersionPtr SINDYLIB_API AheGetParentVersion( IVersion* ipVersion );

} // sindy

#endif // ifndef _CHANGEVERSIONUTIL_H_
