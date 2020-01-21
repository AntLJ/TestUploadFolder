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
#include "IndexedContainer.h"
#include "RowContainer.h"
#include "TableContainer.h"
#include "GlobalFunctions.h"
#include <boost/assign.hpp>

VariantLess _vaLess;
sindy::CIndexedContainer::IteratorContainer _dummy;

using namespace sindy;

CIndexedContainer::CIndexedContainer(void) : m_emCacheType(sindyContainerAutoCacheNone)
{
}

CIndexedContainer::~CIndexedContainer(void)
{
}

CIndexedContainer& CIndexedContainer::operator=( const CIndexedContainer& con )
{
	CContainerBase::operator=( con );
	m_listIndex = con.m_listIndex;
	return *this;
}

void CIndexedContainer::push_back( const CSPContainerBase& cContainer, bool bSecureUnique/* = true*/ )
{
	if( ( ! bSecureUnique ) || end() == find( cContainer ) )
	{
		GetContainer().push_back( cContainer );
		add_key( --rbegin().base() );
	}
}

void CIndexedContainer::push_front( const CSPContainerBase& cContainer, bool bSecureUnique/* = true*/ )
{
	if( ( ! bSecureUnique ) || end() == find( cContainer ) )
	{
		GetContainer().push_front( cContainer );
		add_key( begin() );
	}
}

CIndexedContainer::iterator CIndexedContainer::insert( iterator it, const CSPContainerBase& cContainer, bool bSecureUnique/* = true*/ )
{
	iterator ret = ( bSecureUnique ? find( cContainer ) : end() );
	if( ret == end() )
	{
		ret = GetContainer().insert( it, cContainer );
		add_key( ret );
	}
	return ret;
}

void CIndexedContainer::clear()
{
	CContainerBase::clear();
	m_listIndex.clear();
}

CContainerBase::iterator CIndexedContainer::erase( iterator it )
{
	delete_key( it );
	return CContainerBase::erase( it );
}

CContainerBase::iterator CIndexedContainer::erase( iterator first, iterator last )
{
	for( iterator it = first; it != last; ++it )
		delete_key( it );
	return CContainerBase::erase( first, last );
}

CIndexedContainer::IteratorContainer& CIndexedContainer::finds_by_key( const KeyList& key, const ValueList& val )
{
	switch( m_emCacheType )
	{
	case sindyContainerAutoOnDemandCacheNoDetach:
	case sindyContainerAutoOnDemandCacheDetach:
		create_index( key, val );
		break;
	default:
		create_index( key );
	}

	// std::map<ValueList,IteratorContainer>::iterator it = m_listIndex[key].find( val );
	// VC9���Ƒ�3�����܂Ŋ܂߂��^�w�肵�Ȃ��ƃr���h���ʂ�Ȃ��B�B
	std::map<ValueList,IteratorContainer,ValueListLess>::iterator it = m_listIndex[key].find( val );
	return ( it == m_listIndex[key].end() ? _dummy : it->second );
}

const CIndexedContainer::IteratorContainer& CIndexedContainer::finds_by_key( const KeyList& key, const ValueList& val ) const
{
	CIndexedContainer* const_this = const_cast<CIndexedContainer*>(this);
	return const_this->finds_by_key( key, val );
}

CIndexedContainer::IteratorContainer& CIndexedContainer::finds_by_key( LPCTSTR lpcszTag, const CComVariant& key )
{
	ValueList vals = boost::assign::list_of(key); // �������͈ꎞ�ϐ����`���Ȃ��ƃR���p�C�����ʂ�Ȃ��c
	return finds_by_key( boost::assign::list_of(CString(lpcszTag)), vals );
}

const CIndexedContainer::IteratorContainer& CIndexedContainer::finds_by_key( LPCTSTR lpcszTag, const CComVariant& key ) const
{
	CIndexedContainer* const_this = const_cast<CIndexedContainer*>(this);
	return const_this->finds_by_key( lpcszTag, key );
}

CIndexedContainer::IteratorContainer& CIndexedContainer::finds_by_key( const std::pair<CString,CString>& tag, const std::pair<CComVariant,CComVariant>& key )
{
	ValueList vals = boost::assign::list_of(key.first)(key.second); // �������͈ꎞ�ϐ����`���Ȃ��ƃR���p�C�����ʂ�Ȃ��c
	return finds_by_key( boost::assign::list_of(CString(tag.first))(CString(tag.second)), vals );
}

const CIndexedContainer::IteratorContainer& CIndexedContainer::finds_by_key( const std::pair<CString,CString>& tag, const std::pair<CComVariant,CComVariant>& key ) const
{
	CIndexedContainer* const_this = const_cast<CIndexedContainer*>(this);
	return const_this->finds_by_key( tag, key );
}

void CIndexedContainer::delete_index( const KeyList& key ) const
{
	m_listIndex.erase(key);
}

void CIndexedContainer::delete_index( LPCTSTR lpcszTag ) const
{
	KeyList key = boost::assign::list_of(CString(lpcszTag));
	delete_index(key);
}

void CIndexedContainer::delete_index( const std::pair<CString,CString>& tag ) const
{
	KeyList key = boost::assign::list_of(CString(tag.first))(CString(tag.second));
	delete_index(key);
}

void CIndexedContainer::delete_index() const
{
	m_listIndex.clear();
}

void CIndexedContainer::create_index( const KeyList& keys ) const
{
	std::map<ValueList,IteratorContainer, ValueListLess>& index = m_listIndex[keys];
	if( ! index.empty() )
		return; // ���ɍ쐬�ς�

	// �����L���b�V��
	if( sindyContainerAutoCacheNone != m_emCacheType )
	{
		const CTableContainer* pTable = dynamic_cast<const CTableContainer*>(this);
		const_cast<CTableContainer*>(pTable)->_Select( AheInitQueryFilter(), ( sindyContainerAutoCacheDetach == m_emCacheType ? true : false ), true );
	}

	// index�쐬
	CString strTable;
	const CSchemaSupport* pSchema = dynamic_cast<const CSchemaSupport*>(this);
	if( pSchema )
		strTable = pSchema->GetTableName();
	foreach( const CString& key, keys )
	{
#ifdef _DEBUG
		ATLTRACE(_T("CIndexedContainer::create_index() : [TABLE]%s �C���f�b�N�X�쐬�J�n [TAG]%s\n"), (LPCTSTR)strTable, key );
#endif // ifdef _DEBUG
		CIndexedContainer* pCon = const_cast<CIndexedContainer*>(this);
		for( iterator it = pCon->begin(), itend = pCon->end(); it != itend; ++it )
			add_key( it );
#ifdef _DEBUG
		ATLTRACE(_T("CIndexedContainer::create_index() : [TABLE]%s �C���f�b�N�X�쐬�I�� [TAG]%s\n"), (LPCTSTR)strTable, key );
#endif // ifdef _DEBUG
	}
}

void CIndexedContainer::create_index( const KeyList& keys, const ValueList& vals ) const
{
	if( keys.size() != vals.size() )
	{
		_ASSERTE( keys.size() == vals.size() );
		return;
	}

	std::map<ValueList,IteratorContainer, ValueListLess>& index = m_listIndex[keys];
	bool has_tag = false; // �^�O�����ɓo�^����Ă��邩
	if( ! index.empty() )
	{
		has_tag = true;
		// ���ɍ쐬����Ă�����̂�vals�����݂��邩�ǂ�����
		// �擾�ς݂��ǂ����𒲂ׂ�
		if( index.find( vals ) != index.end() )
			return; // ���ɍ쐬�ς�
	}

#ifdef _DEBUG
	CString strTable;
	const CSchemaSupport* pSchema = dynamic_cast<const CSchemaSupport*>(this);
	if( pSchema )
		strTable = pSchema->GetTableName();
#endif // ifdef _DEBUG

	if( sindyContainerAutoCacheNone == m_emCacheType )
		return;

	// �����L���b�V��
	const CTableContainer* pTable = dynamic_cast<const CTableContainer*>(this);
	// �o���邾�����Ȃ��čςނ悤�ɃN�G��������t�^����
	CString strWhere;
	KeyList::const_iterator itKey = keys.begin();
	ValueList::const_iterator itVal = vals.begin();
	for( ; itKey != keys.end(); ++itKey,++itVal )
	{
		if( !strWhere.IsEmpty() )
			strWhere += _T(" AND ");

		switch( itVal->vt )
		{
		case VT_NULL:
			strWhere.AppendFormat(_T("%s IS NULL"), *itKey );
			break;
		case VT_BSTR:
			strWhere.AppendFormat(_T("%s='%s'"), *itKey, CString(itVal->bstrVal) );
			break;
		case VT_I2:
		case VT_I4:
			strWhere.AppendFormat(_T("%s=%ld"), *itKey, itVal->lVal );
			break;
		default:
			_ASSERT_EXPR( false, _T("CIndexedContainer�ł̃C���f�b�N�X�쐬�͕����񂩐����̂݃T�|�[�g���Ă��܂�") );
			break;
		}
	}
#ifdef _DEBUG
	ATLTRACE(_T("CIndexedContainer::create_index() : [TABLE]%s �ǉ��������܂�\n"), (LPCTSTR)strTable );
#endif // ifdef _DEBUG
	const_cast<CTableContainer*>(pTable)->_Select( AheInitQueryFilter( NULL, _T("*"), _T("%s"), strWhere ), ( sindyContainerAutoOnDemandCacheDetach == m_emCacheType ? true : false ), true );
	// �����̌��ʂ����݂��Ȃ������ꍇ�A�������N�G�X�g������Ƃ܂��������Ă��܂�
	// �Ȃ̂ŁA�����ς݂ł��邱�Ƃ��������߂ɁA�^�O��o�^���Ă���
	if( index.find( vals ) == index.end() )
		index[vals] = IteratorContainer();
}

void CIndexedContainer::create_index( LPCTSTR lpcszTag ) const
{
	KeyList keys = boost::assign::list_of(CString(lpcszTag));
	return create_index(keys);
}

void CIndexedContainer::create_index( LPCTSTR lpcszTag, const CComVariant& key ) const
{
	KeyList keys = boost::assign::list_of(CString(lpcszTag));
	ValueList vals = boost::assign::list_of(key);
	return create_index(keys,vals);
}

void CIndexedContainer::create_index( const std::pair<CString,CString>& tag ) const
{
	KeyList keys = boost::assign::list_of(CString(tag.first))(CString(tag.second));
	create_index(keys);
}

void CIndexedContainer::create_index( const std::pair<CString,CString>& tag, const std::pair<CComVariant,CComVariant>& key ) const
{
	KeyList keys = boost::assign::list_of(CString(tag.first))(CString(tag.second));
	ValueList vals = boost::assign::list_of(key.first)(key.second);
	create_index(keys,vals);
}

void CIndexedContainer::add_key( iterator& it ) const
{
	for( KeyCacheContainer::iterator itIndex = m_listIndex.begin(),
		itIndexEnd = m_listIndex.end(); itIndex != itIndexEnd; ++itIndex )
	{
		const CRowContainer& cRow = *boost::static_pointer_cast<CRowContainer>(*it);
		// KeyList�ɑΉ�����ValueList�쐬
		ValueList vals;
		foreach( const CString& key, itIndex->first )
			vals.push_back( ( _T("OBJECTID") == key ) ? cRow.GetOID() : cRow.GetValue( (LPCTSTR)key ) );
		itIndex->second[vals].insert(it);
	}
}

void CIndexedContainer::delete_key( iterator& it ) const
{
	typedef std::map<ValueList,IteratorContainer, ValueListLess> ValueCacheContainer;
	for( KeyCacheContainer::iterator itIndex = m_listIndex.begin(),
		itIndexEnd = m_listIndex.end(); itIndex != itIndexEnd; ++itIndex )
	{
		// �L���b�V������it��S�ď���
		for( ValueCacheContainer::iterator itValue = itIndex->second.begin();
			itValue != itIndex->second.end(); ++itValue )
			itValue->second.erase(it);
	}
}
