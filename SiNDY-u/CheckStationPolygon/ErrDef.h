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

// STL
#include <map>

// useful_headers
#include <TDC/useful_headers/tstring.h>

// boost
#include <boost/assign.hpp>



/**
 *	@brief �G���[��`
 */
namespace err_def
{
	/**
	 *	@brief �G���[�R�[�h
	 */
	enum Code
	{
		kErrNotLinkedStation             = 101,		//!< [ERROR] �w��񂪕R�t�����Ă��Ȃ�
		kErrPolygonCOutOfValue           = 102,		//!< [ERROR] �|���S����ʃR�[�h���R�[�h�l�h���C���͈͊O
		kErrOutOfLowPlatformShape        = 103,		//!< [ERROR] �|���S�����v���b�g�t�H�[���S�̃|���S���̊O�Ɉʒu���Ă���
		kErrOutOfMidZoomStation          = 104,		//!< [ERROR] �|���S�����w�ȈՃ|���S���̊O�Ɉʒu���Ă���
		kErrLinkedNotExistPolygon        = 105,		//!< [ERROR] ���݂��Ȃ��w�|���S���ւ̉w���R�t�����R�[�h�����݂���
		kErrLinkedNotExistStation        = 106,		//!< [ERROR] ���݂��Ȃ��w�ւ̉w���R�t�����R�[�h�����݂���
		kErrOutOfSameIdLowPlatformShape  = 107,		//!< [ERROR] �|���S�����A�����wID�̃v���b�g�t�H�[���S�̃|���S���������Ȃ����A�O�Ɉʒu���Ă���
		kErrOutOfSameIdMidZoomStation    = 108,		//!< [ERROR] �|���S�����A�����wID�̉w�ȈՃ|���S���������Ȃ����A�O�Ɉʒu���Ă���
		kWarnOutOfHighZoomStation        = 201,		//!< [WARN]  �|���S�����w�ڍ׃|���S���̊O�Ɉʒu���Ă���
		kWarnOutOfMidZoomStation         = 202,		//!< [WARN]  �|���S�����w�ȈՃ|���S���̊O�Ɉʒu���Ă���
		kWarnOutOfSameIdHighZoomStation  = 203,		//!< [WARN]  �|���S�����A�����wID�̉w�ڍ׃|���S���������Ȃ����A�O�Ɉʒu���Ă���
		kWarnOutOfSameIdMidZoomStation   = 204		//!< [WARN]  �|���S�����A�����wID�̉w�ȈՃ|���S���������Ȃ����A�O�Ɉʒu���Ă���
	};


	/**
	 *	@brief �G���[��`
	 */
	const std::map<Code, uh::tstring> errDef = 
								boost::assign::map_list_of
										( kErrNotLinkedStation,             _T("�w��񂪕R�t�����Ă��Ȃ�") )
										( kErrPolygonCOutOfValue,           _T("�|���S����ʃR�[�h���R�[�h�l�h���C���͈͊O") )
										( kErrOutOfLowPlatformShape,        _T("�|���S�����v���b�g�t�H�[���S�̃|���S���̊O�Ɉʒu���Ă���") )
										( kErrOutOfMidZoomStation,          _T("�|���S�����w�ȈՃ|���S���̊O�Ɉʒu���Ă���") )
										( kErrLinkedNotExistPolygon,        _T("���݂��Ȃ��w�|���S���ւ̉w���R�t�����R�[�h�����݂���") )
										( kErrLinkedNotExistStation,        _T("���݂��Ȃ��w�ւ̉w���R�t�����R�[�h�����݂���") )
										( kErrOutOfSameIdLowPlatformShape,  _T("�|���S�����A�����wID�̃v���b�g�t�H�[���S�̃|���S���������Ȃ����A�O�Ɉʒu���Ă���") )
										( kErrOutOfSameIdMidZoomStation,    _T("�|���S�����A�����wID�̉w�ȈՃ|���S���������Ȃ����A�O�Ɉʒu���Ă���") )
										( kWarnOutOfHighZoomStation,        _T("�|���S�����w�ڍ׃|���S���̊O�Ɉʒu���Ă���") )
										( kWarnOutOfMidZoomStation,         _T("�|���S�����w�ȈՃ|���S���̊O�Ɉʒu���Ă���") )
										( kWarnOutOfSameIdHighZoomStation,  _T("�|���S�����A�����wID�̉w�ڍ׃|���S���������Ȃ����A�O�Ɉʒu���Ă���") )
										( kWarnOutOfSameIdMidZoomStation,   _T("�|���S�����A�����wID�̉w�ȈՃ|���S���������Ȃ����A�O�Ɉʒu���Ă���") )
										;

	/// �G���[�R�[�h�ƃG���[���b�Z�[�W�̃y�A
	typedef std::pair<int, uh::tstring> ErrPair;

	/**
 	 *	@brief �G���[����Ԃ�
	 *  @param code [in] �G���[�R�[�h
	 *  @return �G���[���( key: �G���[�R�[�h, value: �G���[���b�Z�[�W )
	 */
	inline ErrPair err( Code code )
	{
		_ASSERTE( errDef.count(code) != 0 );
		return *errDef.find(code);
	}

} // err_def
