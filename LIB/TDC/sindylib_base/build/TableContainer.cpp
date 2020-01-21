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

#include "StdAfx.h"
#include <boost/foreach.hpp>
#include "GlobalFunctions.h"
#include "TableContainer.h"
#include "RowContainer.h"
#include "ErrorObject2.h"
#include "Model.h"
#include "FieldMap.h"
#include "ClassFactory.h"
#include "TimeTrace.h"

#define foreach BOOST_FOREACH

using namespace sindy;
using namespace sindy::errorcode;

#ifndef SINDY_FOR_ENGLISH
const static _TCHAR FAIL_ITABLE_CREATEROW[]     = _T("ITable::CreateRow�Ɏ��s�FHRESULT[%X] MSG[%s]");
const static _TCHAR FAIL_CREATE_RECORD[]        = _T("[TABLE:%s] ���R�[�h�쐬�Ɏ��s���܂���:%s");
const static _TCHAR NONEXIST_ASSOCIATE_PARENT[] = _T("CTableContainer::Select() : �֘A�t����e�I�u�W�F�N�g�����݂��Ȃ� CHILD TABLE[%s] CHILD OID[%ld] PARENT TABLE[%s] PARENT OID[%ld]\n");
#else
const static _TCHAR FAIL_ITABLE_CREATEROW[]     = _T("ITable::Failed to CreateRow:HRESULT[%X] MSG[%s]");
const static _TCHAR FAIL_CREATE_RECORD[]        = _T("[TABLE:%s] Fail to create a record:%s");
const static _TCHAR NONEXIST_ASSOCIATE_PARENT[] = _T("CTableContainer::Select() : The parent object to associate does not exist. CHILD TABLE[%s] CHILD OID[%ld] PARENT TABLE[%s] PARENT OID[%ld]\n");
#endif	// SINDY_FOR_ENGLISH


bool CTableContainer::LessSPRowContainerOID::operator()( CSPContainerBase& c1, CSPContainerBase& c2 ) const
{
	if( ! ( c1 && c2 ) )
		return false;

	CRowContainer& row1( *boost::static_pointer_cast<CRowContainer>(c1) );
	CRowContainer& row2( *boost::static_pointer_cast<CRowContainer>(c2) );

	return row1.GetOID() < row2.GetOID();
}

void CTableContainer::Reset()
{
	iterator it = begin();
	while( it != end() )
	{
		(*it)->Reset();
		// �폜�t���O���c���Ă���Ȃ�R���e�i���珜�O
		if( boost::static_pointer_cast<CRowContainer>(*it)->IsDeleted() )
			it = erase(it);
		else
			++it;
	}
}

void CTableContainer::push_back( const CSPContainerBase& cContainer, bool bSecureUnique/* = true*/ )
{
	CFuncTraceWrapper ft(g_trace,_T("CTableContainer::push_back()"));
	CRowContainer& row( *boost::static_pointer_cast<CRowContainer>(cContainer) );
	// OnDemand�����^�C�v�̂��͈̂ꎞ�I��find_by_key�ł̌�������߂����Ȃ��Ɩ������[�v�ɂȂ�
	sindyContainerAutoCacheType emCache = m_emCacheType;
	m_emCacheType = sindyContainerAutoCacheNone;

	iterator itend = end();
	iterator it = bSecureUnique ? find_by_key( _T("OBJECTID"), row.GetOID() ) : itend;
	m_emCacheType = emCache;
	if( itend == it )
	{
/*#ifdef _DEBUG // ����̃I�u�W�F�N�g�������Ă��Ȃ����ǂ����̃`�F�b�N�p
		foreach( CSPContainerBase& con, GetContainer() )
		{
			_ASSERTE( &*cContainer != &*con );
		}
#endif // ifdef _DEBUG*/
		GetContainer().push_back( cContainer );
		it = --rbegin().base();
	}

	add_key( it );
}

void CTableContainer::push_front( const CSPContainerBase& cContainer, bool bSecureUnique/* = true*/ )
{
	CFuncTraceWrapper ft(g_trace,_T("CTableContainer::push_front()"));
	CRowContainer& row( *boost::static_pointer_cast<CRowContainer>(cContainer) );
	// OnDemand�����^�C�v�̂��͈̂ꎞ�I��find_by_key�ł̌�������߂����Ȃ��Ɩ������[�v�ɂȂ�
	sindyContainerAutoCacheType emCache = m_emCacheType;
	m_emCacheType = sindyContainerAutoCacheNone;

	iterator itend = end();
	iterator it = bSecureUnique ? find_by_key( _T("OBJECTID"), row.GetOID() ) : itend;
	m_emCacheType = emCache;
	if( itend == it )
	{
		GetContainer().push_front( cContainer );
		it = begin();
	}

	add_key( it );
}

CTableContainer::iterator CTableContainer::insert( iterator it, const CSPContainerBase& cContainer, bool bSecureUnique/* = true*/ )
{
	CFuncTraceWrapper ft(g_trace,_T("CTableContainer::insert()"));
	CRowContainer& row( *boost::static_pointer_cast<CRowContainer>(cContainer) );
	// OnDemand�����^�C�v�̂��͈̂ꎞ�I��find_by_key�ł̌�������߂����Ȃ��Ɩ������[�v�ɂȂ�
	sindyContainerAutoCacheType emCache = m_emCacheType;
	m_emCacheType = sindyContainerAutoCacheNone;

	iterator itend = end();
	iterator ret = bSecureUnique ? find_by_key( _T("OBJECTID"), row.GetOID() ) : itend;
	m_emCacheType = emCache;
	if( itend == ret )
		ret = GetContainer().insert( it, cContainer );

	add_key( ret );

	return ret;
}

CSPRowContainer CTableContainer::Create()
{
	CSPRowContainer cRow;
	if( ! p )
		return cRow;

	cRow = RowClassFactory( *this, _IRowPtr() ); // ��������NULL�ɂ��Ȃ���IsCreated�ɂȂ�Ȃ��I�I
	_IRowPtr ipRow;
	LOGASSERTEM_IF( SUCCEEDED( p->CreateRow( &ipRow ) ), sindyErr_AOCreateRowFailed, FAIL_CREATE_RECORD, GetTableName(), GETCOMERRORSTRING() )
	{
		cRow->SetObject( ipRow, GetTableType(), true ); // �����ő�O�����𖾎��I��false�ɂ��Ȃ��Ƃ���ς�IsCreated�ɂȂ�Ȃ��I�I
		push_back( boost::static_pointer_cast<CContainerBase>(cRow), false );
	}
	return cRow;
}

CSPRowContainer CTableContainer::CreateDummyRow()
{
	CFuncTraceWrapper ft(g_trace,_T("CTableContainer::CreateDummyRow()"));
	CSPRowContainer cRow( RowClassFactory( *this, _IRowPtr() ) );
	cRow->CreateCache( false, GetFieldMap(), GetNameString() ); // ���̂��Ȃ��̂ŃL���b�V����������Ă���
	cRow->SetDefaultValue(); // �f�t�H���g�l�t�^
	push_back( boost::static_pointer_cast<CContainerBase>(cRow), false ); // �R���e�i�ɒǉ�

	return cRow;
}

// CreateDummyRow��push_back���Ȃ���
CSPRowContainer CTableContainer::CreateDummyRowNoPush()
{
	CFuncTraceWrapper ft(g_trace,_T("CTableContainer::CreateDummyRow()"));
	CSPRowContainer cRow( RowClassFactory( *this, _IRowPtr() ) );
	cRow->CreateCache( false, GetFieldMap(), GetNameString() ); // ���̂��Ȃ��̂ŃL���b�V����������Ă���
	cRow->SetDefaultValue(); // �f�t�H���g�l�t�^

	return cRow;
}

CSPRowContainer CTableContainer::CreateDummyRow( const CRowContainer& row )
{
	CFuncTraceWrapper ft(g_trace,_T("CTableContainer::CreateDummyRow()"));
	CSPRowContainer cRow( RowClassFactory( *this, _IRowPtr() ) );
	row.CopyTo( *cRow, true, false, false );
	push_back( boost::static_pointer_cast<CContainerBase>(cRow), false ); // �R���e�i�ɒǉ�

	return cRow;
}

sindyErrCode CTableContainer::Materialization( LPCTSTR lpcszParentName, long lParentOID, CErrorObjectsBase& cErrs )
{
	if( ! p )
		return sindyErr_NoErr;

	// ���f������֘A���擾���A�ǂ̃t�B�[���h��
	// �eID����ꂽ��悢�̂����f����
	long lFieldIndex = -1;
	if( 0 < lParentOID )
	{
		// �e��ID��t�^����t�B�[���h�C���f�b�N�X���擾
		// ���Α��̏ꍇ�ł��Ή��ł���i���ꂼ��̐e����Materialization���Ăԁj
		const CModel& model = GetFinder().GetModel();
		std::list<stModel> listModel = model.GetKeys( GetTableName() );
		foreach( const stModel& model, listModel )
		{
			if( model.ParentName == lpcszParentName )
				lFieldIndex = GetFieldMap()->FindField( model.OuterKey );
		}
	}

	foreach( CSPContainerBase cCon, GetContainer() )
	{
		CSPRowContainer spRow = boost::static_pointer_cast<CRowContainer>(cCon);
		CRowContainer& cRow = *spRow;

		if( ( ! (_IRow*)cRow ) && cRow.IsCreated() && ( ! cRow.IsDeleted() ) )
		{
			_IRowPtr ipRow;
			HRESULT hr = S_OK;
			if( FAILED( hr = p->CreateRow( &ipRow ) ) )
			{
				cErrs.push_back( CSPErrorObjectBase( new CErrorObjectBase( cRow, CSPErrorInfoBase( new CErrorInfoBase( sindyErr_AOCreateRowFailed, sindyErrLevel_ERR, NULL, 0, -1, FAIL_ITABLE_CREATEROW, hr, GETCOMERRORSTRING() ) ) ) ) );
				return sindyErr_AOCreateRowFailed;
			}
#ifdef _DEBUG
			else
				ATLTRACE(_T("CreateRow : [TABLE]%s [OID]%d\n"), GetTableName(), AheGetOID(ipRow) );
#endif // ifdef _DEBUG

			// �����ւ�
			cRow.Materialization( ipRow );
		}
		// �eID�t�^
		if( 0 < lFieldIndex )
			cRow.SetValue( lFieldIndex, lParentOID );

		// CRowContainer��������R���e�i�ɂ��Ă������œK�p����
		long lOID = cRow.GetOID();
		LPCTSTR lpcszTableName = cRow.GetTableName();
		foreach( CSPContainerBase cConTable, std::make_pair( cRow.begin(), cRow.end() ) )
		{
			CSPTableContainer spTable = boost::static_pointer_cast<CTableContainer>(cConTable);
			if( sindyErr_NoErr != spTable->Materialization( lpcszTableName, lOID, cErrs ) )
				return cErrs.GetDefaultErrCode();
			// �e�q�֌W�擾
			stModel m = GetFinder().GetModel().GetKey( spTable->GetTableName(), GetTableName() );
			// �����q���̃L�[����OBJECTID�̏ꍇ�͎q������ID���擾���Đe���ɃZ�b�g����K�v������iFEATURE_REL��INFO_POINT�̏ꍇ�Ȃǁj
			if( m.OuterKey == _T("OBJECTID") )
			{
				CTableContainer::iterator it = spTable->begin();
				if( it != spTable->end() )
					cRow.SetValue( m.ParentKey, boost::static_pointer_cast<CRowContainer>(*it)->GetOID() );
			}
		}
	}

	return cErrs.GetDefaultErrCode();
}

sindyErrCode CTableContainer::CreateCache( bool bDetach/* = false*/, const CSPFieldMap& spFields/* = CSPFieldMap()*/, const CSPTableNameString& spNameString/* = CSPTableNameString()*/ )
{
	return CContainerBase::CreateCache( bDetach, spFields, spNameString );
}

sindyErrCode CTableContainer::CheckLogic( const CLogicRule& cRule, CErrorObjectsBase& cErrObjects ) const
{
	foreach( const CSPContainerBase& con, GetContainer() )
	{
		CRowContainer& row( *boost::static_pointer_cast<CRowContainer>(con) );
		row.CheckLogic( cRule, cErrObjects );
	}
	return cErrObjects.GetDefaultErrCode();
}

std::list<long>& CTableContainer::SelectKey( LPCTSTR lpcszTableName, IQueryFilter* ipFilter, LPCTSTR lpcszSubFieldName, LPCTSTR lpcszWhere, std::list<long>& listKey/* = std::list<long>()*/ ) const
{
	CFuncTraceWrapper ft(g_trace,_T("CTableContainer::SelectKey(3)"));
	if( !( ipFilter || lpcszWhere ) )
		return listKey;

	IQueryFilterPtr ipFilterTmp;

	if( ! ipFilter )
		ipFilterTmp = AheInitQueryFilter( NULL, lpcszSubFieldName );
	else
		ipFilterTmp = ipFilter;

	if( lpcszWhere && 0 < _tcslen( lpcszWhere ) )
	{
		CComBSTR bstrWhere;
		if( SUCCEEDED( ipFilterTmp->get_WhereClause( &bstrWhere ) ) )
		{
			CString strWhere;
			if( 0 < bstrWhere.Length() )
				strWhere.Format(_T("%s AND %s"), CString(bstrWhere), lpcszWhere );
			else
				strWhere = lpcszWhere;

			listKey.merge( Cursor2IDs( FindTable(lpcszTableName)->Search( AheInitQueryFilter( ipFilterTmp, lpcszSubFieldName, _T("%s"), strWhere ), true ), lpcszSubFieldName ) );
		}
		else
			_ASSERTE(false);
	}
	else
		listKey.merge( Cursor2IDs( FindTable(lpcszTableName)->Search( ipFilterTmp, true ), lpcszSubFieldName ) );

	listKey.sort();
	listKey.unique();

	return listKey;
}
std::list<long>& CTableContainer::SelectKey( LPCTSTR lpcszTableName, IQueryFilter* ipFilter, LPCTSTR lpcszSubFieldName, LPCTSTR lpcszAddWhereKeyName, const std::list<long>& listAddWhereKey, std::list<long>& listKey/* = std::list<long>()*/ ) const
{
	CFuncTraceWrapper ft(g_trace,_T("CTableContainer::SelectKey(2)"));
	std::list<long> listAddWhereKey2;
	if( ( ! ipFilter ) && listAddWhereKey.empty() )
		return listKey;

	IQueryFilterPtr ipFilterTmp;

	if( ! ipFilter )
		ipFilterTmp = AheInitQueryFilter( NULL, lpcszSubFieldName );
	else
		ipFilterTmp = ipFilter;

	if( listAddWhereKey.empty() )
		listKey.merge( Cursor2IDs( FindTable(lpcszTableName)->Search( ipFilterTmp, true ), lpcszSubFieldName ) );
	else {
		// lpcszAddWhereKeyName��OBJECTID�̏ꍇ�́AQueryFilter�ł̌����̂ݍs��
		// listAddWhereKey�Ƃ̐ς��Ƃ�
		if( 0 == _tcsicmp( lpcszAddWhereKeyName, _T("OBJECTID") ) )
		{
			// ���̍ہAQueryFilter�ł̌������S���ɂȂ�ꍇ��listAddWhereKey�݂̂��̗p����
			CComBSTR bstrWhere;
			ipFilterTmp->get_WhereClause( &bstrWhere );
			if( 0 < bstrWhere.Length() )
				listAddWhereKey2 = set_intersection( SelectKey( lpcszTableName, ipFilterTmp, lpcszSubFieldName, NULL ), listAddWhereKey );
			else
				listAddWhereKey2 = listAddWhereKey;
		}
		else
			listAddWhereKey2 = listAddWhereKey;

		std::list<CString> listIDs;
		CTableBase::IDs2Str( listAddWhereKey2, listIDs );
		for( std::list<CString>::const_iterator it = listIDs.begin(); it != listIDs.end(); ++it )
		{
			CString strWhere;
			strWhere.Format(_T("%s IN (%s)"), lpcszAddWhereKeyName, *it );
			listKey.merge( SelectKey( lpcszTableName, ipFilterTmp, lpcszSubFieldName, strWhere ) );
		}
	}

	listKey.sort();
	listKey.unique();
	return listKey;
}

std::list<long>& CTableContainer::SelectKey( LPCTSTR lpcszTopLevelTableName, const std::map<CString,IQueryFilterPtr>& mapQuery, std::list<long>& listKey/* = std::list<long>()*/ ) const
{
	CFuncTraceWrapper ft(g_trace,_T("CTableContainer::SelectKey()"));
	// ���[���珇�Ԃɕ��ׂ�
	const CModel& model = GetFinder().GetModel();
	std::list<CString> listTableName( model.GetOrderdNameList(lpcszTopLevelTableName) );
	listTableName.reverse();
	// �e�����p��set�ɂ�����Ă���
	std::set<CString> setTableName( listTableName.begin(), listTableName.end() );

	std::map<CString,std::list<long>> mapIDs; // �������ʎ擾�ł���ID���S�ē���
	foreach( const CString& name, listTableName )
	{
		// �N�G���t�B���^�擾
		IQueryFilterPtr ipFilter;
		std::map<CString,IQueryFilterPtr>::const_iterator itQuery = mapQuery.find( name );
		if( itQuery != mapQuery.end() )
			ipFilter = itQuery->second;

		// ���Ō������ꂽID�擾
		const std::list<long>* plistID = NULL;
		std::map<CString,std::list<long>>::const_iterator itIDs = mapIDs.find( name );
		if( itIDs != mapIDs.end() )
			plistID = &itIDs->second;

		if( lpcszTopLevelTableName == name )
		{
			// �g�b�v���x���̏ꍇ
			if( ipFilter )
				SelectKey( name, ipFilter, _T("OBJECTID"), NULL, std::list<long>(), mapIDs[name] );
		}
		else {
			// �e��ID������
			std::list<stModel> listParent( model.GetKeys( name ) );
			foreach( const stModel& m, listParent )
			{
				if( setTableName.end() == setTableName.find(m.ParentName) )
					continue;

				if( ipFilter || ( plistID && ( ! (*plistID).empty() ) ) )
					SelectKey( name, ipFilter, m.OuterKey, m.ParentKey, plistID ? *plistID : std::list<long>(), mapIDs[m.ParentName] );
			}
		}
	}
	listKey = mapIDs[lpcszTopLevelTableName];

	// ���j�[�N����
	listKey.sort();
	listKey.unique();

	return listKey;
}

// ���茟��
void CTableContainer::_Select( const std::list<long>& listID, LPCTSTR lpcszKeyName, bool bDetach, bool bUnique/* = false*/ )
{
	_Select( listID, lpcszKeyName, _T("*"), bDetach, bUnique );
}

void CTableContainer::_Select( const std::list<long>& listID, LPCTSTR lpcszKeyName, LPCTSTR lpcszSubFields, bool bDetach, bool bUnique/* = false*/ )
{
	CFuncTraceWrapper ft(g_trace,_T("CTableContainer::_Select(listID)"));
	if( listID.empty() )
		return;

	std::list<CString> listIDs;
//	if( 0 == lstrcmpi( lpcszKeyName, _T("OBJECTID") ) && 1000 < listID.size() )
//		_Select( GetRows( listID, bDetach ), bDetach, bUnique );
//	else {
		IDs2Str( listID, listIDs );
		foreach( const CString& ids, listIDs )
			_Select( Search( AheInitQueryFilter( NULL, lpcszSubFields, _T("%s in ( %s )"), lpcszKeyName, ids ), bDetach ), bDetach, bUnique );
//	} <- GetRows�͒x������̂ŕ���iPOI_POINT 1000�� �� GetRows �c 2�� IN �c 8�b �܂��A�󋵂ɂ��̂�������Ȃ����ǁB�j
}

void CTableContainer::_Select( IQueryFilterPtr& ipFilter, bool bDetach, bool bUnique/* = false*/ )
{
	CFuncTraceWrapper ft(g_trace,_T("CTableContainer::_Select(ipFilter)"));
	_Select( Search( ipFilter, bDetach ), bDetach, bUnique );
}

void CTableContainer::_Select( _ICursorPtr& ipCursor, bool bDetach, bool bUnique/* = false*/ )
{
	CFuncTraceWrapper ft(g_trace,_T("CTableContainer::_Select(ipCursor)"));
	if( ! ipCursor )
	{
		_ASSERT_EXPR( ! ipCursor, _T("�����Ɏ��s") );
		return;
	}
	_IRowPtr ipRow;
	while( S_OK == ipCursor->NextRow( &ipRow ) )
	{
		if( ! ipRow )
			break;

		CSPRowContainer spRow( RowClassFactory( *this, ipRow ) );
		if( bDetach )
			spRow->CreateCache( true ); // �L���b�V���쐬
		push_back( spRow, bUnique );
	}
}

// �q������
void CTableContainer::_SelectChild( const std::list<long>& listID, LPCTSTR lpcszKey/* = _T("OBJECTID")*/, bool bDetach/* = false*/, bool bUnique/* = true*/ )
{
	const CModel& model = GetFinder().GetModel();

	// �q��������
	std::list<stModel> listChildModel = model.GetKeysByParentName( GetTableName() );
	foreach( const stModel& m, listChildModel )
	{
		// ID�Ǝ��̂̑Ή��\�쐬�i�֘A�t���ׁ̈j
		// �g�b�v���x���̃R���e�i�͎g�p�����A�ꎞ�R���e�i���쐬����
		CSPTableContainer spChildTable( FindTable( m.Name )->CreateEmptyClone() );
		if( ! (ITable*)*spChildTable )
			continue; // �e�[�u�������݂��Ȃ��ꍇ�͖����ibug 9402 #15�j

		// �e���̃L�[���擾����
		std::list<long> listKey;
		if( lpcszKey == m.ParentKey )
			listKey = listID;
		else {
			foreach( CSPContainerBase& con, GetContainer() )
				listKey.push_back( boost::static_pointer_cast<CRowContainer>(con)->GetValue( m.ParentKey, -1L ) );
		}
		// �q���̌���
		spChildTable->Select( listKey, m.OuterKey, bDetach, bUnique );
		// �e�q�̊֘A�t��
		CTableContainer::iterator itEnd = end();
		foreach( CSPContainerBase spCon, std::make_pair( spChildTable->begin(), spChildTable->end() ) )
		{
			CSPRowContainer spRow = boost::static_pointer_cast<CRowContainer>(spCon);
			if( spRow->IsDeleted() )
				continue;
			// CIndexedContainer�̃C���f�b�N�X���g�p���ĕR�t��
			CTableContainer::IteratorContainer its = finds_by_key( m.ParentKey, spRow->GetValue( m.OuterKey, -1L ) );
			if (! its.empty())
				foreach( CTableContainer::iterator it, its )
				{
					boost::static_pointer_cast<CRowContainer>(*it)->GetItemTable( m.Name )->push_back( spRow, bUnique );
				}
			else {
				_ASSERT_EXPR( ! its.empty(), _T("ERROR: �֘A�t����e�I�u�W�F�N�g�����݂��Ȃ�") );
				TRACEMESSAGE( NONEXIST_ASSOCIATE_PARENT,
					spRow->GetTableName(), spRow->GetOID(), m.ParentKey, spRow->GetValue( m.OuterKey, -1L ) );
			}
		}
	}
}

// ���茟��
void CTableContainer::Select( const std::list<long>& listID, LPCTSTR lpcszKey/* = _T("OBJECTID")*/, bool bDetach/* = false*/, bool bUnique/* = true*/ )
{
//	CFuncTraceWrapper ft(g_trace,_T("CTableContainer::Select(listID)"));
	CTimeTrace trace;
	trace.Start(_T("CTableContainer::Select()") + CString(GetTableName()) );
	// �S�Ă̌������ʂ�TableFinder�̒��ɂ��Z�b�g�����
	// ���̕ӂ̎d�l��SiNDY-i�Ƃ͈قȂ�
	// �������A�֘A�t�����I�������Ȃ�Finder���ɂ���
	// ���R�[�h�̓N���A���Ă����Ȃ��B
	// �i�g�b�v���x���e�[�u����Finder���ɂȂ��Ȃ�A�̘b�j
	const CModel& model = GetFinder().GetModel();

	trace.Start(GetTableName());
	_Select( listID, lpcszKey, bDetach, bUnique );
	trace.End(GetTableName());

	_SelectChild( listID, lpcszKey, bDetach, bUnique );

	trace.End(_T("CTableContainer::Select()") + CString(GetTableName()) );
	trace.Trace();
}

// ���E���葍������
void CTableContainer::Select( const std::map<CString,IQueryFilterPtr>& mapQuery, bool bDetach/* = false*/, bool bUnique/* = true*/ )
{
	CFuncTraceWrapper ft(g_trace,_T("CTableContainer::Select(mapQuery)"));
	if( mapQuery.size() == 1 && mapQuery.begin()->first == GetTableName() )
	{
		_Select( Search( mapQuery.begin()->second, bDetach ), bDetach, bUnique );
		std::list<long> listKey;
		foreach( CSPContainerBase& con, GetContainer() )
			listKey.push_back( boost::static_pointer_cast<CRowContainer>(con)->GetOID() );
		_SelectChild( listKey, _T("OBJECTID"), bDetach, bUnique );
	}
	else
		Select( SelectKey( GetTableName(), mapQuery ), _T("OBJECTID"), bDetach, bUnique );
}

bool CTableContainer::exist( const CRowContainer& cRow ) const
{
	foreach( const CSPContainerBase& con, GetContainer() )
	{
		if( static_cast<const CRowContainer&>(*con).exist( cRow ) )
			return true;
	}
	return false;
}
bool CTableContainer::exist( const CTableBase& cTable ) const
{
	if( this == &cTable )
		return true;

	foreach( const CSPContainerBase& con, GetContainer() )
	{
		if( static_cast<const CRowContainer&>(*con).exist( cTable ) )
			return true;
	}
	return false;
}

// ���g����̃R���e�i�����
CSPTableContainer CTableContainer::CreateEmptyClone() const
{
	return boost::static_pointer_cast<CTableContainer>(TableClassFactory( GetFinder(), ITablePtr(p) ) );
}
