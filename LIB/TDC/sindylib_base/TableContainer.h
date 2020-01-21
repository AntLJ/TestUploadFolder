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

#include "TableBase.h"
#include "IndexedContainer.h"
#include "TableFinderSupport.h"
#include "sindylib_base_classconstructorimpl.h"

class CModel;

namespace sindy {

class CRowContainer;
class CTableContainer;
typedef boost::shared_ptr<CRowContainer> CSPRowContainer;
typedef boost::shared_ptr<CTableContainer> CSPTableContainer;

/**
 * @class CTableContainer
 * 
 * ���̃N���X�͕ҏW�n���\�b�h�𒼐ڌĂԂ��Ƃ��u���b�N���邽�߂�
 * �N���X�ł��BCInfoMaster�o�R�ōs���Ă��������B
 */
class CTableContainer : 
	public CTableBase, 
	public CIndexedContainer, 
	public CTableFinderSupport
{
	SINDYBASE_TABLECLASS_CONSTRUCTOR_BASE( CTableContainer )
public:
	/// sort�p�v���f�B�P�[�g
	class LessSPRowContainerOID
	{
	public:
		bool operator()( CSPContainerBase& c1, CSPContainerBase& c2 ) const;
	};

	//@{ CIndexedContainer �I�[�o�[���C�h
	virtual void push_back( const CSPContainerBase& cContainer, bool bSecureUnique = true );
	virtual void push_front( const CSPContainerBase& cSPContainer, bool bSecureUnique = true );
	virtual iterator insert( iterator it, const CSPContainerBase& cContainer, bool bSecureUnique = true );
	virtual void Reset();
	//@}

	virtual CSPRowContainer Create();
	virtual CSPRowContainer CreateDummyRow();
	/// CreateDummyRow��push_back���Ȃ���
	virtual CSPRowContainer CreateDummyRowNoPush();
	/// row�̃L���b�V�����g�p���ă_�~�[Row���쐬����
	virtual CSPRowContainer CreateDummyRow( const CRowContainer& row );
	virtual errorcode::sindyErrCode Materialization( LPCTSTR lpcszParentName, long lParentOID, CErrorObjectsBase& cErrs );
	virtual errorcode::sindyErrCode CreateCache( bool bDetach = false, const CSPFieldMap& spFields = CSPFieldMap(), const CSPTableNameString& spNameString = CSPTableNameString() );
	virtual errorcode::sindyErrCode CheckLogic( const CLogicRule& cRule, CErrorObjectsBase& cErrObjects ) const;

	/// ��茟���i�P�Ɓj
	std::list<long>& SelectKey( LPCTSTR lpcszTableName, IQueryFilter* ipFilter, LPCTSTR lpcszSubFieldName, LPCTSTR lpcszWhere, std::list<long>& listKey = std::list<long>() ) const;
	std::list<long>& SelectKey( LPCTSTR lpcszTableName, IQueryFilter* ipFilter, LPCTSTR lpcszSubFieldName, LPCTSTR lpcszAddWhereKeyName, const std::list<long>& listAddWhereKey, std::list<long>& listKey = std::list<long>() ) const;
	/// ��茟���i�K�w�j
	std::list<long>& SelectKey( LPCTSTR lpcszTopLevelTableName, const std::map<CString,IQueryFilterPtr>& mapQuery, std::list<long>& listKey = std::list<long>() ) const;

	/**
	 * @brief ���茟���i�P�Ɓj
	 * 
	 * �������ʂ͎������g�̃R���e�i�Ɋi�[����܂��B
	 * @param listID       [in] �L�[ID
	 * @param lpcszKeyName [in] �L�[��
	 */
	void _Select( const std::list<long>& listID, LPCTSTR lpcszKeyName, bool bDetach, bool bUnique = false );
	void _Select( const std::list<long>& listID, LPCTSTR lpcszKeyName, LPCTSTR lpcszSubFields, bool bDetach, bool bUnique = false );
	void _Select( IQueryFilterPtr& ipFilter, bool bDetach, bool bUnique = false );
	void _Select( _ICursorPtr& ipCursor, bool bDetach, bool bUnique = false );

	/// �q������
	void _SelectChild( const std::list<long>& listID, LPCTSTR lpcszKey = _T("OBJECTID"), bool bDetach = false, bool bUnique = true );

	/**
	 * @brief ���茟���i�K�w�j
	 * 
	 * �������ʂ͎������g�̃R���e�i�Ɋi�[����܂��B\n
	 * ���f����q�������݂���ꍇ�AFindTable�����N���X�Ɍ�����
	 * �����p����܂��B\n
	 * �q���̌������I�������i�K�Őe�q�֌W���\�z���܂��B
	 * ���̂��߁A�������ʂ͊e�R���e�i���y��TableFinder���ێ�����
	 * �e�e�[�u���N���X���ɕێ�����܂��ishared_ptr�x�[�X�Ȃ̂�
	 * �������ʂ̎��̂͂��ꂼ���ł��j�B
	 * @param listID       [in] �L�[ID
	 * @param lpcszKeyName [in,optional] �L�[���i�f�t�H���g�FOBJECTID�j
	 * @param bDetach     [in,optional] Row�I�u�W�F�N�g�̃C���X�^���X��؂藣���ꍇ��true�i�f�t�H���g�Ffalse�j
	 * @param bUnique      [in,optional] �ǉ������̏ꍇ��true�i�f�t�H���g�Ftrue�j
	 */
	void Select( const std::list<long>& listID, LPCTSTR lpcszKey = _T("OBJECTID"), bool bDetach = false, bool bUnique = true );

	/// ���E���葍������
	void Select( const std::map<CString,IQueryFilterPtr>& mapQuery, bool bDetach = false, bool bUnique = true );

	bool exist( const CRowContainer& cRow ) const;
	bool exist( const CTableBase& cTable ) const;

	/**
	 * @brief ���g�����CSPTableContainer�����
	 * 
	 * �R���e�i�ňꎞ�I�ɊǗ����������A������CSPTableContainer�ɂ͉e�����y�ڂ������Ȃ��ꍇ��
	 * �g�p���Ă��������B
	 */
	CSPTableContainer CreateEmptyClone() const;

protected:
};

typedef boost::shared_ptr<CTableContainer> CSPTableContainer;

} // sindy
