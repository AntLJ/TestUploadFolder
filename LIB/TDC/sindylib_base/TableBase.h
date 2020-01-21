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
 * @file TableBase.h
 * @brief CTableBase�N���X��`
 * @author �n�}�c�a���암�J���O���[�v �Ð�M�G
 * $Id$
 */
#ifndef _TABLEBASE_H_
#define _TABLEBASE_H_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "WinLib/ComWrapper.h"
#include "ErrorCode.h"
#include "SchemaSupport.h"

namespace sindy {
class CRowBase;

/**
 * @class CTableBase
 * @brief ITablePtr�̃��b�p�[�N���X
 *
 * �S�Ẵe�[�u���N���X�͂��̃N���X�����ƂȂ�܂�
 */
#pragma warning(push)
#pragma warning(disable : 4275)
#pragma warning(disable : 4251)
class CTableBase : public CComWrapper<ITable>, public CSchemaSupport
{
public:
	CTableBase()
	{
	}
	virtual ~CTableBase(){}

	CTableBase( ITable* lp ){ SetObject( sindyTableType::unknown, lp ); }
	CTableBase( IFeatureClass* lp ){ SetObject( sindyTableType::unknown, lp ); }
	CTableBase( sindyTableType::ECode emTableType, ITable *lp ){ SetObject( emTableType, lp ); }
	CTableBase( sindyTableType::ECode emTableType, IFeatureClass *lp ){	SetObject( emTableType, lp ); }

	CTableBase& operator =( ITable* lp ){ SetObject( sindyTableType::unknown, lp ); return *this; }
	CTableBase& operator =( IFeatureClass* lp ){ SetObject( sindyTableType::unknown, lp ); return *this; }
	CTableBase& operator =( const CTableBase& obj );

	/**
	 * @brief <b>�e�[�u�����Z�b�g����</b>\n
	 * �e�[�u�����Z�b�g���܂��B�ʏ�͂�������g�p���Ă��������B
	 * @param lp	[in]	�e�[�u��
	 */
	void SetObject( ITable* lp, const CSPFieldMap& cFields = CSPFieldMap(), const CSPTableNameString& cNames = CSPTableNameString() );
	void SetObject( IFeatureClass* lp, const CSPFieldMap& cFields = CSPFieldMap(), const CSPTableNameString& cNames = CSPTableNameString() ){ SetObject( (ITablePtr)lp, cFields, cNames ); }

	/**
	 * @brief <b>�e�[�u�����Z�b�g����</b>\n
	 * �e�[�u���^�C�v�𒼐ڎw�肷��łł��B
	 * �e�[�u�������Ⴄ���t�B�[���h�͈ꏏ�Ƃ����ꍇ�ɓ���������̃^�C�v�ɍ��킹�邽�߂�
	 * �g�p���܂��B\n
	 * �e�[�u���^�C�v��sindyTableType::unknown�̏ꍇ�ɂ́AGetTableType���Ă΂ꂽ�Ƃ���
	 * ��x�����e�[�u���^�C�v�̒����̂��߂̏���������܂��B���ڎw�肷�邱�ƂŁA���̃R�X
	 * �g���팸���邱�Ƃ��ł���Ƃ��������b�g������܂��B
	 * @warning �ȑO�̃o�[�W�����܂ł̓e�[�u���^�C�v��K���w�肷��K�v������܂������A
	 * ���݂̓e�[�u���^�C�v��GetTableType���Ă΂ꂽ�Ƃ��ɕK�v�ɉ����ăZ�b�g�����̂�
	 * �K�{�ł͂Ȃ��Ȃ�܂����B
	 */
	void SetObject( sindyTableType::ECode emTableType, ITable *lp);
	void SetObject( sindyTableType::ECode emTableType, IFeatureClass *lp){ SetObject( emTableType, (ITablePtr)lp ); }

	operator ITable*() const
	{
		return (ITable*)p;
	}

	//@{ @name IClass�C���^�[�t�F�[�X
	IUIDPtr GetCLSID() const;
	long FindField( LPCTSTR Name ) const;
	IFieldsPtr GetFields() const;
	bool HasOID() const;
	LPCTSTR GetOIDFieldName( int nLen, LPTSTR lpszName ) const;
	//@}
	//@{ @name ITable�C���^�[�t�F�[�X
	/**
	 * @brief ���R�[�h���쐬����
	 * @note �V�K�쐬���̎��������t�^�ɑΉ����Ă��܂��B
	 */
	virtual CRowBase& CreateRow( CRowBase& cRow );
	/**
	 * @brief �_�~�[���R�[�h���쐬����
	 * ���̉�������ꍇ�́ACRowBase::Materialization���g�p���Ă��������B
	 * @note �V�K�쐬���̎��������t�^�ɑΉ����Ă��܂��B
	 */
	virtual CRowBase& CreateDummyRow( CRowBase& cRow );
	errorcode::sindyErrCode DeleteSearchedRows( IQueryFilter* QueryFilter );
	_IRowPtr GetRow( long OID ) const;
	_IRowPtr GetRow( IQueryFilter* QueryFilter ) const;
	_ICursorPtr GetRows( const VARIANT& oids, bool Recycling ) const;
	long RowCount( IQueryFilter* QueryFilter ) const;
	_ICursorPtr Search( IQueryFilter* QueryFilter, bool Recycling ) const;
	//@}
	/**
	 * @brief <b>�e�[�u�����Ȃǂ��Ǘ����郁���o�̋��L�R�s�[��Ԃ�</b>\n
	 * m_spNameString�̎��̂�shared_ptr��Ԃ��܂��B���̍ہA�쐬����Ă��Ȃ����
	 * �����I�ɍ쐬����܂��B
	 * @return boost::shared_ptr<CTableNameString>
	 */
	CSPTableNameString GetNameString() const;

	/**
	 * @brief <b>�t�B�[���h�Ή��\�̋��L�R�s�[��Ԃ�</b>\n
	 * m_spFieldMap�̎��̂�shared_ptr��Ԃ��܂��B
	 * ���̍ہA�쐬����ĂȂ���Ύ����I�ɍ쐬���܂��B
	 * @return boost::shared_ptr<CFieldMap>
	 */
	CSPFieldMap GetFieldMap() const;

	/**
	 * @brief <b>�������[�N�X�y�[�X�ɑ��݂���e�[�u�����I�[�v������</b>\n
	 * IFeatureWorkspace::OpenTable�Ɠ�����������܂��B\n
	 * �I�[�i�[�����l������K�v������ꍇ�́A�I�[�i�[���t���̃e�[�u������n���K�v������܂��B\n
	 * ���̃I�u�W�F�N�g�Ɠ����I�[�i�[���̃e�[�u�����I�[�v������ꍇ��OpenSameOwnerTable�֐���
	 * �g�p���Ă��������B
	 * @param lpcszTableName	[in]	�e�[�u����
	 * @return ITablePtr
	 */
	ITablePtr OpenTable( LPCTSTR lpcszTableName ) const;

	/**
	 * @brief <b>�������[�N�X�y�[�X�ɑ��݂���e�[�u�����I�[�v������</b>\n
	 * @param lpcszOwnerName	[in]	�I�[�i�[��
	 * @param emTableType		[in]	�e�[�u���^�C�v
	 * @return ITablePtr
	 */
	ITablePtr OpenTable( LPCTSTR lpcszOwnerName, sindyTableType::ECode emTableType ) const;

	/**
	 * @brief <b>�������[�N�X�y�[�X�ɑ��݂��铯���I�[�i�[���̃e�[�u�����I�[�v������</b>\n
	 * lpcszTableName�ɃI�[�i�[�����܂܂�Ă���ꍇ�͂��̃I�[�i�[�������̃I�u�W�F�N�g�̃I�[�i�[��
	 * �ɂ��ăe�[�u�����I�[�v�����܂��B
	 * @param lpcszTableName	[in]	�e�[�u����
	 * @return ITablePtr
	 */
	ITablePtr OpenSameOwnerTable( LPCTSTR lpcszTableName ) const;

	/**
	 * @brief <b>�������[�N�X�y�[�X�ɑ��݂��铯���I�[�i�[���̃e�[�u�����I�[�v������</b>\n
	 * @param emTableType		[in]	�e�[�u���^�C�v
	 * @return ITablePtr
	 */
	ITablePtr OpenSameOwnerTable( sindyTableType::ECode emTableType ) const;

	/**
	 * @brief <b>���[�N�X�y�[�X���擾����</b>\n
	 * @return IWorkspacePtr
	 */
	IWorkspacePtr GetWorkspace() const;

	/**
	 * @brief <b>ID�񂩂�IN�Ŏg�p�ł��镶������쐬����</b>\n
	 * IN�Ŏg�p�ł���ID�̐��̏����1000�Ȃ̂ŁA1000�𒴂���ꍇ��
	 * �����̕����񂪕Ԃ�܂��B
	 * @param listIDs	[in]	ID��
	 * @param strIDs	[out]	1000��؂��ID��ie.x. "100,110,115,200"�j
	 */
	static void IDs2Str( const std::list<long>& listIDs, std::list<CString>& strIDs, LPCTSTR str_sep = NULL )
	{
		if( ! listIDs.empty() )
		{
			strIDs.push_back( CString(_T("")) );
	
			long lCount = 1;
			std::list<CString>::reverse_iterator itStr = strIDs.rbegin();

			for( std::list<long>::const_iterator it = listIDs.begin(); it != listIDs.end(); ++it, ++lCount )
			{
				CString item;
				if( str_sep )
					item.Format(_T("%s%ld%s"), str_sep, *it, str_sep );
				else
					item.Format(_T("%ld"), *it );

				if( itStr->IsEmpty() ) 
					*itStr = item;
				else {
					*itStr += _T(",");
					*itStr += item;
				}

				if( 1000 == lCount )
				{
					strIDs.push_back( CString(_T("")) );
					itStr = strIDs.rbegin();
					lCount = 0;
				}
			}
		}
		// 1000�Ŋ���؂��ꍇ���Ƌ󂪗]�v�ɓ���̂ŏ���
		if( ( ! strIDs.empty() ) && strIDs.rbegin()->IsEmpty() )
			strIDs.pop_back();
	}

	/**
	 * @brief <b>ID�񂩂�IN�Ŏg�p�ł��镶������쐬����</b>\n
	 * IN�Ŏg�p�ł���ID�̐��̏����1000�Ȃ̂ŁA1000�𒴂���ꍇ��
	 * �����̕����񂪕Ԃ�܂��B
	 * @param listIDs	[in]	ID��
	 * @param strIDs	[out]	1000��؂��ID��ie.x. "'100','110','115','200'"�j
	 */
	static void IDs2Str( const std::list<CString>& listIDs, std::list<CString>& strIDs, LPCTSTR str_sep = NULL )
	{
		if( ! listIDs.empty() )
		{
			strIDs.push_back( CString(_T("")) );
	
			long lCount = 1;
			std::list<CString>::reverse_iterator itStr = strIDs.rbegin();

			for( std::list<CString>::const_iterator it = listIDs.begin(); it != listIDs.end(); ++it, ++lCount )
			{
				CString item;
				if( str_sep )
					item.Format(_T("%s%s%s"), str_sep, *it, str_sep );
				else
					item = *it;

				if( itStr->IsEmpty() ) 
					*itStr = item;
				else {
					*itStr += _T(",");
					*itStr += item;
				}

				if( 1000 == lCount )
				{
					strIDs.push_back( CString(_T("")) );
					itStr = strIDs.rbegin();
					lCount = 0;
				}
			}
		}
		// 1000�Ŋ���؂��ꍇ���Ƌ󂪗]�v�ɓ���̂ŏ���
		if( ( ! strIDs.empty() ) && strIDs.rbegin()->IsEmpty() )
			strIDs.pop_back();
	}

	/**
	 * @brief <b>OID���X�g�Ō�������</b>\n
	 * @param fids		[in]	OID���X�g
	 * @param Recycling	[in]	NextRow�ł�IRow�����T�C�N������ꍇ��true
	 * @return ICursorPtr
	 */
	
	template <typename _Container> _ICursorPtr GetRows( const _Container& fids, bool Recycling ) const
	{
		SAFEARRAYBOUND bound = { (ULONG)fids.size(), 0 };
		SAFEARRAY* pSa = SafeArrayCreate( VT_I4, 1, &bound );
		LONG* pArray;
		SafeArrayAccessData( pSa, (void**)&pArray );

		long i = 0;
		for( _Container::const_iterator it = fids.begin(); it != fids.end(); ++it, ++i )
			pArray[i] = *it;

		SafeArrayUnaccessData( pSa );
		VARIANT vaArray;
		VariantInit( &vaArray );
		vaArray.vt = VT_ARRAY | VT_I4;
		vaArray.parray = pSa;

		_ICursorPtr ipCursor = GetRows( vaArray, Recycling );
		VariantClear( &vaArray );

		return ipCursor;
	}

	/**
	 * @brief <b>OID���X�g�Ō�������</b>\n
	 * @note ������̂��̂ƈقȂ�AWHERE���lpcszRelIDFieldName in ( fids[1], fids[2], ... )�Ƃ����w�肪����܂�
	 * @warning fids��1000�𒴂���ꍇ�͈�x�Ɍ����ł��Ȃ�����NULL��Ԃ��܂��i@see IDs2Str�j
	 * @param fids					[in]	OID���X�g
	 * @param Recycling				[in]	NextRow�ł�IRow�����T�C�N������ꍇ��true
	 * @param lpcszRelIDFieldName	[in]	fids���K�p�����t�B�[���h��
	 * @return ICursorPtr
	 */
	_ICursorPtr GetRows( const std::list<long>& fids, bool Recycling, LPCTSTR lpcszRelIDFieldName ) const;
};
#pragma warning(pop)

typedef boost::shared_ptr<CTableBase> CSPTableBase;

} // sindy

#endif // _TABLE_H_
