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

#include <tuple>
#include "sindylib_base_global.h"
#include "RowBase.h"
#include "ContainerBase.h"
#include "TableFinderSupport.h"
#include "sindylib_base_classconstructorimpl.h"

namespace sindy {

class CContainerBase;
class CRowContainer;
class CTableBase;
typedef boost::shared_ptr<CContainerBase> CSPContainerBase;
typedef boost::shared_ptr<CRowContainer> CSPRowContainer;

/**
 * @class CRowContainer
 * 
 * ���̃N���X�̓��f���T�|�[�g�p���R�[�h�N���X�̃x�[�X�ƂȂ�
 * �N���X�ł��B
 * ���f���T�|�[�g�ɓ������������͈ȉ��̒ʂ�F
 * @li �q���ƂȂ郌�R�[�h�Q���e�[�u���P�ʂ�CContainerBase�ňꊇ�Ǘ�\
 *     ���܂ł͊e���R�[�h�N���X�̃����o�Ƃ��ēƎ��ɕێ����Ă��܂������A
 *     ���ꂾ�ƃI�[�o�[���C�h�~�X���ɂ�肤�܂��`�B����Ȃ����Ƃ����X
 *     ����܂����B�����ŁA���̃N���X�ł͎q�����R�[�h�֊K�w�I�ɏ��������
 *     ���\�b�h�͑S�ăI�[�o�[���C�h���ē`�B�~�X���Ȃ��Ȃ�悤�ɂ��Ă��܂��B
 *     �e�q�e�[�u���R���e�i�̎擾��GetItemTable()�֐��ōs���܂��B
 */
class CRowContainer : 
	public CRowBase,
	public CContainerBase,
	public CTableFinderSupport
{
public:
	SINDYBASE_ROWCLASS_CONSTRUCTOR_BASE( CRowContainer, unknown )
protected:
	void InitRelationalTables();
public:
	//@{ @name CRowBase���\�b�h�I�[�o�[���C�h
	virtual bool Changed( schema::ipc_table::update_type::ECode* emUpdateType = NULL ) const;
	virtual errorcode::sindyErrCode CreateCache( bool bDetach = false, const CSPFieldMap& spFields = CSPFieldMap(), const CSPTableNameString& spNameString = CSPTableNameString() );
	virtual errorcode::sindyErrCode CheckLogic( const CLogicRule& cRule, CErrorObjectsBase& cErrs ) const;
	virtual errorcode::sindyErrCode Store( CErrorObjectsBase& cErrs );
	virtual void Delete();
	virtual void UnDelete();
	//@}
	//@{ @name CContainerBase���\�b�h�I�[�o�[���C�h
	virtual CContainerBase& Query( sindyCompareOperator2 emOperator, LPCTSTR lpcszTableName, CContainerBase& cContainer, bool bSecureUnique = true ) const;
	virtual void Reset();
	//@}
	/// �^����ꂽ���̂��z���ɑ��݂��邩�m�F����
	bool exist( const CRowContainer& cRow ) const;
	bool exist( const CTableBase& cTable ) const;

	/// �R���e�i���X�g����R���e�i���擾����
	virtual CSPTableContainer GetItemTable( LPCTSTR lpcszTableName );
	virtual CSPTableContainer GetItemTable( LPCTSTR lpcszTableName ) const;
};

#define CAST_ROWC boost::static_pointer_cast<CRowContainer>

} // sindy
