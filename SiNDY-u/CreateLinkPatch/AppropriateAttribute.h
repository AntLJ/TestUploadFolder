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
 *	@file AppropriateAttribute.h
 *	@brief �H���R�[�h�w��t�@�C���N���X ��`��
 *	@author	F.Adachi
 *	@date	2005/08/10		�V�K�쐬
 *	$ID$
 */

#ifndef ___APPROPRIATEATTRIBUTE___
#define ___APPROPRIATEATTRIBUTE___

#include <map>
#include <boost/utility.hpp>

/// <b>�H���R�[�h�w��t�@�C���N���X</b>
class AppropriateAttribute : boost::noncopyable
{
public:
	AppropriateAttribute(void);
	~AppropriateAttribute();

	void	read( const char* cpFilePath );						///< �H���R�[�h�w��t�@�C���̓ǂݍ���
	void	disp( std::ostream& crStream ) const;				///< �f�[�^�S�̂��X�g���[���o��
	
	const std::map<long, long>* getTable(void) const { return &m_Table; }

	long	getRoadClass( long cRoadCode ) const;
protected:

private:
	std::map<long, long>	m_Table;		///< �H���R�[�h�Ɠ��H�W����ʂ̑Ή��e�[�u��

};

#endif
