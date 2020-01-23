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
 * @file RelRoad.h
 * @brief <b>CRelRoad�N���X��`�w�b�_�t�@�C��</b>
 * @author 
 * @vertion $Id$
 */
#ifndef _RELROAD_H__
#define _RELROAD_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "RoadLink.h"

namespace sindy {
class CErrorObjects;
/**
 * @class CRelRoad
 * @brief ���H�����N�Ǘ��N���X
 * ���̃N���X�ł́A���H�Ɋ֌W�̂���I�u�W�F�N�g��ێ�
 * ���܂�
 */
class SINDYLIB_API CRelRoad : public road_link::CRoadLink
{
public:
	CRelRoad()
	{
	}
	virtual ~CRelRoad()
	{
	}
	CRelRoad( const CRelRoad& obj ) : CRoadLink( obj )
	{
	}
	CRelRoad( _IRow* lp, sindyTableType::ECode emTableType = sindyTableType::unknown, bool bCreated = false, const CSPFieldMap& spFields = CSPFieldMap(), const CSPTableNameString& spNameString = CSPTableNameString() )
		: CRoadLink( lp, emTableType, bCreated, spFields, spNameString )
	{
	}
	CRelRoad& operator=( const CRelRoad& obj )
	{
		CRoadLink::operator==( obj );
		return *this;
	}
	CRelRoad& operator=( _IRow* lp )
	{
		CRoadLink::operator==( lp );
		return *this;
	}
	/**
	 * @brief <b>�R���t���N�g����̂��߂̊��ҏW�`�F�b�N���s��</b>\n
	 * �ύX�����������̂ɑ΂��Ă̂݃`�F�b�N���s���܂��B\n
	 * �L���b�V�����쐬���Ă��Ȃ��ƕύX�̃`�F�b�N�Ɉ���������Ȃ��̂Œ��ӂ��Ă��������B\n
	 * @param cErrObjects	[out]	�G���[�I�u�W�F�N�g�i�[�p�R���e�i
	 * @return sindyErrCode
	 */
	errorcode::sindyErrCode CheckModify( CErrorObjects& cErrObjects ) const;
};

} // sindy

#endif // _RELROAD_H__
