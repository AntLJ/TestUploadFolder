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
#include <boost/foreach.hpp>
#include "Model.h"

#define foreach BOOST_FOREACH

CModel::CModel()
{
}

CModel::~CModel()
{
}

// ���f���̃g�b�v���疖�[�܂ł��V���A���Ɏ擾����
std::list<CString> CModel::GetOrderdNameList( LPCTSTR lpcszTopLevelParentName ) const
{
	std::list<CString> ret; // �߂�l
	std::set<CString> for_find; // �����ςݐe�̌����p

	TYPEDEF_MODEL model( m_model );
	for( TYPEDEF_MODEL::iterator it = model.begin(); it != model.end(); )
	{
		if( ( it->second.ParentName.IsEmpty() && it->second.Name == lpcszTopLevelParentName ) ||// �e�����Ȃ����g�b�v���x��
			for_find.end() != for_find.find( it->second.ParentName ) ) // �e�����ɖ߂�l�Ƃ��ēo�^�ς�
		{
			CString strName( it->first ); // �������̂ŕێ�
			ret.push_back( strName );
			for_find.insert( strName );
			// ���f������폜����
			model.erase( it );
			it = model.begin(); // ������x�ŏ�����
		}
		else
			++it;
	}

	return ret;
}

// �e�ɓ����郂�f�������擾����
std::list<CString> CModel::GetParent( LPCTSTR lpcszName ) const
{
	std::list<CString> ret; // �߂�l

	foreach( const CModel::TYPEDEF_MODEL_PAIR& m, std::make_pair( m_model.find( lpcszName ), m_model.end() ) )
		ret.push_back( m.second.ParentName );

	_ASSERT_EXPR( ! ret.empty(), _T("���f�������o�^����Ă��Ȃ�") );

	return ret;
}

// �q�ɂ����郂�f�������擾����
std::list<CString> CModel::GetChild( LPCTSTR lpcszName ) const
{
	std::list<CString> ret; // �߂�l

	if( ! lpcszName )
		return ret;

	foreach( const CModel::TYPEDEF_MODEL_PAIR& m, m_model )
	{
		if( 0 != m.second.ParentName.CompareNoCase( lpcszName ) )
			continue;

		ret.push_back( m.first );
	}
	return ret;
}

// ���f����ǉ�����
CModel::TYPEDEF_MODEL::iterator CModel::AddModel( LPCTSTR lpcszChild, LPCTSTR lpcszChildKey, LPCTSTR lpcszParent, LPCTSTR lpcszParentKey )
{
	TYPEDEF_MODEL::iterator it = m_model.insert( std::make_pair(CString(lpcszChild),stModel()) );
	stModel& model = it->second;
	model.Name = lpcszChild;
	model.OuterKey = lpcszChildKey;
	model.ParentName = lpcszParent;
	model.ParentKey = lpcszParentKey;
	return it;
}

void CModel::Add( LPCTSTR lpcszChild, LPCTSTR lpcszChildKey, LPCTSTR lpcszParent, LPCTSTR lpcszParentKey, LPCTSTR lpcszParentStrictKey, const CComVariant& vaStrictValue )
{
	// �����t�B�[���h��2�̏ꍇ�p�iSiNDY-i �� FEATURE_REL��INFO_POINT,INFO_LINE,INFO_POLYGON�Ȃǁj
	TYPEDEF_MODEL::iterator it = AddModel( lpcszChild, lpcszChildKey, lpcszParent, lpcszParentKey );
	it->second.ParentStrictKey = lpcszParentStrictKey;
	it->second.ParentStrictValue = vaStrictValue;
}

// �q�̃��f��������L�[�����擾����
std::list<stModel> CModel::GetKeys( LPCTSTR lpcszName ) const
{
	std::list<stModel> listRet; // �߂�l

	foreach( const CModel::TYPEDEF_MODEL_PAIR& m, std::make_pair( m_model.find( lpcszName ), m_model.end() ) )
	{
		if( m.second.Name != lpcszName )
			break;
		listRet.push_back( m.second );
	}

	return listRet;
}

// �e�̃��f��������L�[�����擾����
std::list<stModel> CModel::GetKeysByParentName( LPCTSTR lpcszName ) const
{
	std::list<stModel> listRet; // �߂�l

	foreach( const CModel::TYPEDEF_MODEL_PAIR& m, m_model )
	{
		if( 0 != m.second.ParentName.CompareNoCase( lpcszName ) )
			continue;

		listRet.push_back( m.second );
	}

	return listRet;

}

// �e�q�̃��f��������L�[�����擾����
stModel CModel::GetKey( LPCTSTR lpcszName, LPCTSTR lpcszParent ) const
{
	foreach( const CModel::TYPEDEF_MODEL_PAIR& m, std::make_pair( m_model.find( lpcszName ), m_model.end() ) )
	{
		if( m.second.ParentName == lpcszParent )
			return m.second;
	}
	return stModel();
}
