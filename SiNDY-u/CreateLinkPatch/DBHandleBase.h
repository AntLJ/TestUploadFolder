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
 *	@file DBHandleBase.h
 *	@brief DB�����A�v���P�[�V�������N���X ��`��
 *	@author	F.Adachi
 *	@date	2005/08/02		�V�K�쐬
 *	@note	Template Method
 *	$ID$
 */

#ifndef ___DBHANDLEBASE___
#define ___DBHANDLEBASE___

#include <ostream>
#include <memory>
#include <boost/utility.hpp>
#include "DBInfo.h"
#include "DBAccess.h"

/// <B>�A�v���P�[�V�������N���X</B>
class DBHandleBase : boost::noncopyable
{
public:
	DBHandleBase(void);
	virtual ~DBHandleBase();

	virtual void	run(void);										///< ���C���������s
	virtual void	process(void) = 0;								///< �����i�����̓T�u�N���X�Ɉˑ��j

	const DBInfo*		getDBInfo(void) const { return m_pDBInfo.get(); }
	void				setDBInfo( std::auto_ptr<const DBInfo> cpDBInfo ) { m_pDBInfo = cpDBInfo; }
	const IWorkspacePtr	getWorkspace(void) const { return m_ipWorkspace; }
	CString				getProgramName(void) { return m_ProgramName; }

private:
	std::auto_ptr<const DBInfo>		m_pDBInfo;				///< DB���N���X�ւ̊֘A
	IWorkspacePtr	m_ipWorkspace;							///< �Ώۃ��[�N�X�y�[�X�ւ̃|�C���^
	static const CString m_ProgramName;						///< �v���O������

	DBAccess*	m_pDBAccess;								///< DB�ڑ��Ǘ��N���X�ւ̊֘A
};

#endif
