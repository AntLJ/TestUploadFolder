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
 *	@file	DBAccess.h
 *	@brief	DB�ڑ��Ǘ��N���X ��`��
 *	@author	F.Adachi
 *	@date	2005/08/05		�V�K�쐬
 *	$ID$
 */

#ifndef ___DBACCESS___
#define ___DBACCESS___

#include "DBInfo.h"
#include <boost/utility.hpp>

/// <b>DB�ڑ��Ǘ��N���X</b>
class DBAccess : boost::noncopyable
{
public:
	DBAccess(void);
	~DBAccess();

	IWorkspacePtr	open( const DBInfo* cpDBInfo ) const;	///< �Ώ�DB�ɐڑ�

private:
	const DBInfo*	m_pDBInfo;			///< DB���N���X�ւ̊֘A

};

#endif
