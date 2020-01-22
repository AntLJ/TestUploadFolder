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

#include "stdafx.h"
#include "TableFinder.h"

TableFinder::TableFinder(void)
{
}

TableFinder::~TableFinder(void)
{
}

// ���[�N�X�y�[�X�擾
IWorkspacePtr TableFinder::getWorkspace( const CString& lpcszKey )
{
	auto itr = m_mapWorkspace.find( lpcszKey );
	if( itr != m_mapWorkspace.end() )
	{
		// PGDB�̂Ƃ��Ƀ��b�V�����ς�����Ƃ��ɂ����[�N�X�y�[�X�擾���Ȃ���
		if( !itr->second.m_T )
		{
			itr->second.m_T = gf::getWorkspace( lpcszKey );
		}
		return itr->second.m_T;
	}
	else
		return nullptr;
}

// �e�[�u������
ITablePtr TableFinder::findTable( const CString& lpcszWorkspaceKey, const CString& lpcszTable )
{
	CString strKey;
	strKey.Format( _T("%s.%s"), lpcszWorkspaceKey, lpcszTable );	// �L�[�́A[���[�N�X�y�[�X�L�[.�e�[�u����]
	strKey.MakeLower();

	auto itr = m_mapTable.find( strKey );
	if( itr != m_mapTable.end() )
	{
		if( !itr->second.m_T )
		{
			itr->second.m_T = openTable( lpcszWorkspaceKey, lpcszTable );	// �e�[�u���܂��擾���Ă��Ȃ�
		}
		return itr->second.m_T;
	}
	else
	{
		// �L�[���̂��Ȃ��Ƃ��́A�ǉ����ăe�[�u���擾
		ITablePtr ipTable = openTable( lpcszWorkspaceKey, lpcszTable );
		if( ipTable )
		{
			m_mapTable.insert( std::pair< CString, CAdapt<ITablePtr> >(strKey, ipTable) );
		}
		return ipTable;
	}
	return nullptr;
}

//////////////////////////////////////////
//	����J�����o�֐�
//////////////////////////////////////////

// �e�[�u���擾
ITablePtr TableFinder::openTable( const CString& lpcszWorkspaceKey, const CString& lpcszTable )
{
	// �L�[���̂��Ȃ��Ƃ��́A�ǉ����ăe�[�u���擾
	const IWorkspacePtr& ipWorkspace = getWorkspace( lpcszWorkspaceKey );
	if( ipWorkspace )
	{	
		ITablePtr ipTable;
		IFeatureWorkspacePtr(ipWorkspace)->OpenTable( CComBSTR(lpcszTable), &ipTable );
		return ipTable;
	}
	return nullptr;
}

// ���[�N�X�y�[�X�ǉ�
bool TableFinder::addWorkspace( const CString& lpcszKey, const CString& lpcszWorkspaceKey )
{
	CAdapt<IWorkspacePtr> obj(nullptr);
	IWorkspacePtr ipWorkspace = gf::getWorkspace( lpcszWorkspaceKey );
	if( !ipWorkspace ) return false;
	m_mapWorkspace.insert( std::pair< CString, CAdapt<IWorkspacePtr> >(lpcszKey, ipWorkspace) );

	return true;
}