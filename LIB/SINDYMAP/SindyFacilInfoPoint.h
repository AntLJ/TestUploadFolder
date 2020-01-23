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

// SindyFacilInfoPoint.h: SindyFacilInfoPoint �N���X�̃C���^�[�t�F�C�X
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_SINDYFACILINFOPOINT_H__9377DB9C_73BF_46E2_B80B_F31D53F443B0__INCLUDED_)
#define AFX_SINDYFACILINFOPOINT_H__9377DB9C_73BF_46E2_B80B_F31D53F443B0__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "SindyHighwayPoint.h"

class SindyFacilInfoPoint : public SindyHighwayPoint  
{
public:
	SindyFacilInfoPoint(SindyDBConnection *cDBConnection);
	virtual ~SindyFacilInfoPoint();

	/**
	 * @brief �H���R�[�h�ƘH�����V�[�P���X�ԍ�����{�ݏ��|�C���g��_IRowPtr���擾
	 * @note	SindyHighwayPoint::SearchByFacilCode()�ł�GDBHWYFACILRANGE�������邪�A
	 *			����H���R�[�h�E�H�����V�[�P���X�ԍ������{�ݏ��|�C���g�͂P�����Ȃ��͂�(HC3002)�Ȃ̂ŁA
	 *			���̂����P�̎{�ݏ��|�C���g������Ԃ��֐����I�[�o�[���[�h�B
	 * @param[in]	cRoadCode	�H���R�[�h
	 * @param[in]	cRoadSeq	�H�����V�[�P���X�ԍ�
	 * @return	���������{�ݏ��|�C���g�i������Ȃ��ꍇ��NULL�j
	 */
	_IRow* SearchByFacilCode(long cRoadCode, long cRoadSeq);

	using SindyHighwayPoint::SearchByFacilCode;		// �I�[�o�[���[�h�ɂ�茩���Ȃ��Ȃ�̂�h��
};

#endif // !defined(AFX_SINDYFACILINFOPOINT_H__9377DB9C_73BF_46E2_B80B_F31D53F443B0__INCLUDED_)
