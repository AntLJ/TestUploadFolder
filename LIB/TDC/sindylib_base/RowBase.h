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
 * @file RowBase.h
 * @brief <b>CRowBase�N���X��`�t�@�C��</b>\n
 * @author �n�}�c�a���암�J���O���[�v �Ð�M�G
 * $Id$
 */
#ifndef _ROWBASE_H_
#define _ROWBASE_H_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <atlsimpcoll.h>
#include "WinLib/ComWrapper.h"
#include "FieldMap.h"
#include "TableType.h"
#include "ClassUtil.h"
#include "EditDef.h"
#include "sindylib_base_global.h"
#include "SchemaSupport.h"

#pragma warning(push)
#pragma warning(disable : 4275)
#pragma warning(disable : 4251)
namespace sindy {
class CErrorInfosBase;
class CErrorObjectsBase;
class CLogicRule;
class CCacheAttribute;
class CGeometryBase;
class CTable;
class CRowBase;
class CTableNameString;

typedef boost::shared_ptr<CRowBase> CSPRowBase;

/**
 * @class CRowBase
 * @brief IRowBasePtr�̃��b�p�[�N���X
 *
 * �S�ẴA�C�e���N���X�͂��̃N���X�����ƂȂ�܂�
 */
class CRowBase : public CComWrapper<_IRow>, public CSchemaSupport
{
public:
	CRowBase();
	virtual ~CRowBase()
	{
		DeleteCache();
	}

	CRowBase( const CRowBase& obj );
	CRowBase( _IRow* lp, sindyTableType::ECode emTableType = sindyTableType::unknown, bool bCreated = false, const CSPFieldMap& spFields = CSPFieldMap(), const CSPTableNameString& spNameString = CSPTableNameString() );

	CRowBase& operator=( const CRowBase& obj );
	CRowBase& operator=( _IRow* lp );

	operator _IRow*() const
	{
		return (_IRow*)p;
	}
	operator IGeometry*() const;

	bool operator ==( const CRowBase& obj ) const
	{
		return IsEqualObject( obj );
	}
	bool operator ==( _IRow* obj ) const
	{
		return IsEqualObject( obj );
	}
	bool operator ==( int null ) const
	{
        if (null != 0) {
            _com_issue_error(E_POINTER);
        }

        return p == NULL;
	}
	bool operator !=( int null ) const
	{
        if (null != 0) {
            _com_issue_error(E_POINTER);
        }

        return p != NULL;
	}
    operator bool() const throw()
    { 
        return p != NULL; 
    }
	bool operator <( const CRowBase& obj );

	_IRow** Release()
	{
		IUnknown* pTemp = p;
		if (pTemp)
		{
			p = NULL;
			pTemp->Release();
		}
		// �L���b�V�����폜����
		DeleteCache();
		// �����o������
		Init();

		return &p;
	}

	/**
	 * @brief <b>�t�B�[�`���E���R�[�h���Z�b�g����</b>\n
	 * �����Ńe�[�u�������擾���A�e�[�u���^�C�v��ݒ肵�܂��B\n
	 * ��O�p�����[�^�́A�V�K�ɍ쐬�����ꍇ�ɂ̂�true������Ă��������B���̃t���O
	 * ��Store���̗���t�^�̂��߂ɕK�v�ł��B\n
	 * spFields�AspNames�͂Ȃ�ׂ��Z�b�g����悤�ɂ��Ă��������B
	 * �Z�b�g���Ȃ������ꍇ�A�Ⴆ��GetTableName()���Ă񂾂Ƃ��ɂ��̏��
	 * �L���b�V�����쐬���邽�߃R�X�g��������܂��B
	 * @param lp			[in]			�t�B�[�`��
	 * @param emTableType	[in,optional]	�e�[�u���^�C�v�i�f�t�H���g�FsindyTableType::unknown�j
	 * @param bCreated		[in,optional]	�V�K�ɍ쐬���ꂽ�t�B�[�`�����Z�b�g����ꍇ�͕K��true��n�����Ɓi�f�t�H���g�Ffalse�j
	 * @param spFields		[in,optional]	�t�B�[���h�}�b�v�L���b�V���i�f�t�H���g�FNULL�j
	 * @param spNames		[in,optional]	�e�햼�O���L���b�V���i�f�t�H���g�FNULL�j
	 */
	void SetObject( _IRow* lp, sindyTableType::ECode emTableType = sindyTableType::unknown, bool bCreated = false, const CSPFieldMap& spFields = CSPFieldMap(), const CSPTableNameString& spNames = CSPTableNameString() );

	/**
	 * @brief <b>�_�~�[CRowBase�����̉�������</b>
	 * CTable::CreateDummyRowBase�ō쐬�����_�~�[�ɑ΂��Ď��̂��Z�b�g���܂��B
	 * �I�u�W�F�N�g�̍쐬�^�C�~���O�����肬��܂Œx�����������ꍇ�ɗL���ł��B
	 * CRowBase::p���Z�b�g���邾���ł��̂ŃL���b�V�����̓N���A����܂���B
	 * �e�[�u���^�C�v���m�F���܂���̂Ń^�C�v�̈قȂ���̂��Z�b�g���Ȃ��ł��������B
	 */
	void Materialization( _IRow* lp );

	/**
	 * @brief <b>�t�B�[�`���̃R�s�[���쐬����</b>\n
	 * �����e�[�u����ɓ������������I�u�W�F�N�g���쐬���܂��B\n
	 * �L���b�V���̓R�s�[����܂���B\n
	 * �t�B�[���h�Ή��\�A�e�[�u�����L���b�V���̓R�s�[����܂��ishared_ptr�j�B
	 * @warning �ҏW�J�n����Ă��Ȃ���΃G���[���O���o�܂��B
	 * @warning IRowBase���f�^�b�`����Ă���ꍇ�̓G���[���O���o�܂��B
	 * @retval CSPRowBase
	 */
	CSPRowBase Clone() const;

	/**
	 * @brief <b>�t�B�[�`���̕ύX�����ɖ߂�</b>\n
	 * �쐬�t���O�����Ă������̂�CTable::CreateDummyRowBase()�����̏�����
	 * �s���܂��B\n
	 * �폜�t���O��false�ɂȂ�܂��B
	 * @param bRecursive [in] CContainer�̒��g���߂������ꍇ��true(�f�t�H���g�Ffalse)
	 */
	virtual void Reset();

	/**
	 * @brief <b>�f�t�H���g�l���Z�b�g����</b>\n
	 * @warning p�����݂��邩�A����FieldMap���Z�b�g����Ă���K�v������܂��B
	 */
	void SetDefaultValue();

	/**
	 * @brief <b>�V�K�쐬���̎����t�^������t�^����<\b>\n
	 * @note ���O�C�����Ă��鎞�݂̂Ɍ��肳��܂�
	 */
	void SetCreateAutoValue();

	/**
	 * @brief <b>�폜����</b>\n
	 * �폜�t���O�𗧂Ă܂��B
	 * ��x���Ă��폜�t���O�����ɖ߂��ɂ�UnDelete���g�p���܂��B
	 * ���f������ɂ�Store()����K�v������܂��B
	 * @param bRecursive [in,optional] CContainer�ɂ��K�p����ꍇ��true�i�f�t�H���g�Ffalse�j
	 */
	virtual void Delete(){ m_bDeleted = true; }
	virtual void UnDelete(){ m_bDeleted = false; }

	//@{ @name IRowBase�C���^�[�t�F�[�X
	IFieldsPtr GetFields() const;
	bool HasOID() const;
    long GetOID() const;
	virtual errorcode::sindyErrCode Store( CErrorObjectsBase& cErrs );
	ITablePtr GetTable() const;
	errorcode::sindyErrCode GetValue( long lIndex, VARIANT& vaValue ) const;
	virtual errorcode::sindyErrCode SetValue( long lIndex, const VARIANT& vaValue );
	/**
	 * @brief <b>CComVariant��Ԃ�GetValue</b>\n
	 * @param lIndex	[in]	�t�B�[���h�C���f�b�N�X�ԍ�
	 * @return CComVariant
	 */
	CComVariant GetValue( long lIndex ) const;

	//@}

	/**
	 * @brief <b>���[�N�X�y�[�X���擾����</b>\n
	 * @return IWorkspacePtr
	 */
	IWorkspacePtr GetWorkspace() const;

	//@{ @name CSchemaSupport�I�[�o�[���C�h
	/**
	 * @brief <b>�t�B�[���h�Ή��\�̎Q�Ƃ�Ԃ�</b>\n
	 * m_spFields�̎��̂̎Q�Ƃ�Ԃ��܂��B\n
	 * m_spNameString�̎Q�Ƃ�Ԃ��܂��B\n
	 * ��L�����o�ϐ��́ACreateCache�֐��Ŏ����I�ɍ쐬����܂����A
	 * �L���b�V�������݂��Ȃ��ꍇ�͂��̊֐����Ă΂ꂽ���_�ŐV�K�ɍ쐬����܂��B
	 * @warning ���̊֐��Ŏ��̂��������������ƁA���̕����\�[�X�������܂��B
	 * ������ނ̃I�u�W�F�N�g���������݂���ꍇ�͂ł��邾��CreateCache��shared_ptr
	 * ��n���ċ��L�ł���悤�ɐS�����Ă��������B
	 * @return boost::shared_ptr<CFieldMap>
	 */
	virtual CSPFieldMap GetFieldMap() const;

	/**
	 * @brief <b>�e�[�u�����Ȃǂ��Ǘ����郁���o�̎Q�Ƃ�Ԃ�</b>\n
	 * m_spNameString�̎��̂̎Q�Ƃ�Ԃ��܂��B\n
	 * ��L�����o�ϐ��́ACreateCache�֐��Ŏ����I�ɍ쐬����܂����A
	 * �L���b�V�������݂��Ȃ��ꍇ�͂��̊֐����Ă΂ꂽ���_�ŐV�K�ɍ쐬����܂��B
	 * @warning ���̊֐��Ŏ��̂��������������ƁA���̕����\�[�X�������܂��B
	 * ������ނ̃I�u�W�F�N�g���������݂���ꍇ�͂ł��邾��CreateCache��shared_ptr
	 * ��n���ċ��L�ł���悤�ɐS�����Ă��������B
	 * @return boost::shared_ptr<CTableNameString>
	 */
	virtual CSPTableNameString GetNameString() const;
	//@}

	bool IsEqualObject(_IRow* ipRowBase ) const;
	bool IsEqualObject( const CRowBase& cRowBase ) const;

	/**
	 * @brief <b>�t�B�[���h���Ńt�B�[���h�l���擾����</b>\n
	 * �t�B�[���h�Ή��\��������΍쐬���Ă���擾���܂��B
	 * @param lpcszFieldName	[in]	�t�B�[���h��
	 * @param vaValue			[out]	�t�B�[���h�l
	 *
	 * @return sindyErrCode
	 */
	errorcode::sindyErrCode GetValue( LPCTSTR lpcszFieldName, VARIANT& vaValue ) const { return GetValue( FindField(lpcszFieldName), vaValue ); }

	/**
	 * @brief <b>CComVariant��Ԃ�GetValue</b>\n
	 * @param lpcszFieldName	[in]	�t�B�[���h��
	 * @return CComVariant
	 */
	CComVariant GetValue( LPCTSTR lpcszFieldName ) const { return GetValue( FindField(lpcszFieldName) ); }

	/**
	 * @brief <b>�t�B�[���h���Ńt�B�[���h�l���Z�b�g����</b>\n
	 * �t�B�[���h�Ή��\��������΍쐬���Ă���Z�b�g���܂��B
	 * @param lpcszFieldName	[in]	�t�B�[���h��
	 * @param vaValue			[out]	�t�B�[���h�l
	 *
	 * @return sindyErrCode
	 */
	errorcode::sindyErrCode SetValue( LPCTSTR lpcszFieldName, const VARIANT& vaValue ){ return SetValue( FindField(lpcszFieldName), vaValue ); }

	/**
	 * @brief <b>�����ɕύX�����������ǂ������`�F�b�N����</b>\n
	 * �L���b�V�����Ă���ꍇ�Ɍ���L���ł��B�L���b�V�����Ă��Ȃ���΁A���
	 * false���Ԃ�܂��B
	 * ���z�֐��Ȃ̂́A�����N���CLQ�ɕύX������������CInfo���X�V���Ȃ���΂����Ȃ�����
	 * @param emUpdateType	[out,optional]	�ύX�^�C�v������i�f�t�H���g�FNULL�j
	 * @retval true �ύX����
	 * @retval false �ύX�Ȃ��i�L���b�V�����Ă��Ȃ��ꍇ�͏��false�j
	 */
	virtual bool Changed( schema::ipc_table::update_type::ECode* emUpdateType = NULL ) const;

	/**
	 * @brief <b>�����ɕύX�����������ǂ������`�F�b�N����</b>\n
	 * �L���b�V�����Ă���ꍇ�Ɍ���L���ł��B�L���b�V�����Ă��Ȃ���΁A���
	 * false���Ԃ�܂��B
	 * @param lpcszFieldName	[in]	�t�B�[���h��
	 * @retval true �ύX����
	 * @retval false �ύX�Ȃ��i�L���b�V�����Ă��Ȃ��ꍇ�͏��false�j
	 */
	bool Changed( LPCTSTR lpcszFieldName ) const;

	/// �`��ɕύX�����������ǂ����`�F�b�N����
	bool ShapeChanged() const;

	/**
	 * @brief <b>���������`�F�b�N����</b>\n
	 * ���̊֐��ł͊�{�I�ȃ`�F�b�N�����s���܂���B
	 * �K�v�ȃ`�F�b�N�́A�p����̃N���X�Ŏ������Ă��������B
	 * @param cRule			[in]	�������`�F�b�N�p���[��
	 * @param cErrs			[out]	�G���[���i�[�p�R���e�i
	 * @return sindyErrCode
	 */
	virtual errorcode::sindyErrCode CheckLogic( const CLogicRule& cRule, CErrorObjectsBase& cErrs ) const;
	void CheckLogic( const CLogicRule& cRule, CErrorInfosBase& infos ) const;

	/**
	 *  @brief �t�B�[���h�P�ʂ̃`�F�b�N������
	 * 
	 * @warning �ҏW�\�t�B�[���h���������Ă��܂���
	 * 
	 * @param cRule			[in]	�������`�F�b�N�p���[��
	 * @param cErrs			[out]	�G���[���i�[�p�R���e�i
	 * @param lIndex        [in]    �t�B�[���h�C���f�b�N�X
	 * @param cFieldAttr    [in]    �t�B�[���h���
	 * @param vaValue       [in]    ���݂̑����l
	 * @param vaOrg         [in]    ���Ƃ��Ƃ̑����l
	 * @param bChanged      [in]    �����ɕύX�����邩�ǂ���
	 * @param bIsSDE        [in]    SDE���ǂ����i��SJIS��UNICODE���ǂ����j
	 */
	virtual void CheckLogic_Field( const CLogicRule& cRule, CErrorInfosBase& cErrs, long lIndex, const CFieldMap::_FIELD& cFieldAttr, const CComVariant& vaValue, const CComVariant& vaOrg, bool bChanged, bool bIsSDE ) const;

	/**
	 * @brief <b>�R���t���N�g����̂��߂̊��ҏW�`�F�b�N���s��</b>\n
	 * @param cErrs		[out]	�G���[���i�[�p�R���e�i
	 * @return sindyErrCode
	 */
	errorcode::sindyErrCode CheckModify( CErrorObjectsBase& cErrs ) const;

	/**
	 * @brief <b>�ҏW�\���ǂ������`�F�b�N����</b>\n
	 * �ҏW���[���Ɋ�Â��ĕҏW�\���ǂ������`�F�b�N���܂��B\n
	 * �ҏW�ς݃`�F�b�N�͍s���܂���B
	 * @param emType          [in]          �ҏW�^�C�v
	 * @param cErrInfos       [out]         �G���[�i�[�p�R���e�i
	 * @param check_modify    [in,optional] ModifyCheck�����邩�ǂ����i�f�t�H���g�Ftrue�j
	 * @param err2warn_modify [in,optional] ModifyCheck���x�������ɂ��邩�ǂ����i�f�t�H���g�Ffalse�j
	 * @return sindyErrCode
	 */
	errorcode::sindyErrCode CheckEditable( sindyeEditType emType, CErrorInfosBase& cErrInfos, bool check_modify = true, bool err2warn_modify = false ) const;

	long GetValue( long index, long lDefaultValue ) const;
	double GetValue( long index, const double& dDefaultValue ) const;
	bool GetValue( long index, bool bDefaultValue ) const;
	CString GetValue( long index, LPCTSTR lpcszDefaultValue ) const;

	long GetValue( LPCTSTR lpcszFieldName, long lDefaultValue ) const { return GetValue( FindField(lpcszFieldName), lDefaultValue ); }
	double GetValue( LPCTSTR lpcszFieldName, const double& dDefaultValue ) const { return GetValue( FindField(lpcszFieldName), dDefaultValue ); }
	bool GetValue( LPCTSTR lpcszFieldName, bool bDefaultValue ) const { return GetValue( FindField(lpcszFieldName), bDefaultValue ); }
	CString GetValue( LPCTSTR lpcszFieldName, LPCTSTR lpcszDefaultValue ) const { return GetValue( FindField(lpcszFieldName), lpcszDefaultValue ); }

	/**
	 * @brief �C���f�b�N�X�ԍ���long�l���Z�b�g����
	 * @param index				[in]			�t�B�[���h�C���f�b�N�X�ԍ�
	 * @param lNewValue			[in]			�Z�b�g����l
	 * @return sindyErrCode
	 */
	errorcode::sindyErrCode SetValue( long index, long lNewValue );
	errorcode::sindyErrCode SetValue( long index, int iNewValue ){ return SetValue( index, (long)iNewValue ); }
	errorcode::sindyErrCode SetValue( long index, const double& dNewValue );
	errorcode::sindyErrCode SetValue( long index, bool bNewValue );
	errorcode::sindyErrCode SetValue( long index, LPCTSTR lpcszNewValue );

	errorcode::sindyErrCode SetValue( LPCTSTR lpcszFieldName, long lNewValue ) { return SetValue( FindField(lpcszFieldName), lNewValue ); }
	errorcode::sindyErrCode SetValue( LPCTSTR lpcszFieldName, const double& dNewValue ) { return SetValue( FindField(lpcszFieldName), dNewValue ); }
	errorcode::sindyErrCode SetValue( LPCTSTR lpcszFieldName, bool bNewValue ) { return SetValue( FindField(lpcszFieldName), bNewValue ); }
	errorcode::sindyErrCode SetValue( LPCTSTR lpcszFieldName, LPCTSTR lpcszNewValue ) { return SetValue( FindField(lpcszFieldName), lpcszNewValue ); }

	/**
	 * @brief �l��NULL���ǂ����m�F����
	 * 
	 * @param index [in] �t�B�[���h�C���f�b�N�X�ԍ�
	 * @retval true �l��NULL
	 * @retval false �l��NULL�ł͂Ȃ�
	 */
	bool IsNull( long index ) const;
	bool IsNull( LPCTSTR lpcszFieldName ) const { return IsNull( FindField(lpcszFieldName) ); }
	/**
	 * @brief �w�肳�ꂽ�t�B�[���h���f�t�H���g�l���ǂ������`�F�b�N����
	 *
	 * @param lpcszFieldName [in] �t�B�[���h��
	 * @retval true �f�t�H���g�l�ł���
	 * @retval false �f�t�H���g�l�ł͂Ȃ��i�擾�G���[�̏ꍇ���܂ށj
	 */
	bool IsDefaultValue( LPCTSTR lpcszFieldName ) const;

	/**
	 * @brief �ύX�O�̒l���擾����
	 */
	CComVariant GetOriginalValue( LPCTSTR lpcszFieldName ) const;
	CComVariant GetOriginalValue( long lFieldIndex ) const;

	/**
	 * @brief �t�B�[���h�����瑊������R�[�h�l�h���C���̕�������擾����
	 * @param lpszFieldName	[in]	�Ώۃt�B�[���h��
	 * @return �R�[�h�l�h���C���̕�������擾�i�R�[�h�l�h���C���̕�������擾�ł��Ȃ������ꍇ�͒l�����̂܂ܕԂ��j
	 */
	CString GetDomainName( LPCTSTR lpszFiedlName ) const;

	/**
	 * @brief <b>�����E�`��Ȃǂ̃L���b�V�����쐬����</b>\n
	 * �t�B�[���h�Ή��\�A�e�[�u�����L���b�V���I�u�W�F�N�g��������΍쐬���Ă���쐬���܂����A
	 * ���̏ꍇ�ق��̃I�u�W�F�N�g�Ƌ��L�ł��Ȃ����߃��\�[�X�𑽂��K�v�Ƃ��܂��B
	 * ���\�[�X���ǂ��ł��悢�ꍇ�A�y�уI�u�W�F�N�g�����Ȃ��ꍇ�������ċɗ̓t�B�[���h�Ή��\
	 * �ƃe�[�u�����L���b�V���I�u�W�F�N�g��n���悤�ɂ��Ă��������B\n
	 * ���ɃL���b�V�������݂���ꍇ�̓L���b�V�����쐬���܂���B�܂��A
	 * bDetach��true�ɂ���ƁA�L���b�V���쐬���
	 * _IRow*��؂藣���܂��B
	 * @param bDetach			[in,optional]	������_IRow*��؂藣���ꍇ��true
	 * @param ipFields			[in,optional]	�t�B�[���h�Ή��\�I�u�W�F�N�g(shared_ptr)
	 * @param spNameString		[in,optional]	�e�[�u�������Ǘ��I�u�W�F�N�g(shared_ptr)
	 * @return sindyErrCode;
	 */
	virtual errorcode::sindyErrCode CreateCache( bool bDetach = false, const CSPFieldMap& spFields = CSPFieldMap(), const CSPTableNameString& spNameString = CSPTableNameString() );

	/**
	 * @brief <b>�L���b�V�����폜����</b>\n
	 * �f�X�g���N�^����Ă΂�܂��B
	 * �p����N���X�ŃI�[�o�[���C�h�����ꍇ�A
	 * C++�̋K��ɂ��Ⴆvirtual�ł��f�X�g���N�^��
	 * CRowBase::DeleteCache()���K���Ă΂�邱�Ƃɒ��ӂ��Ă��������B
	 */
	virtual void DeleteCache();

	/**
	 * @brief <b>�L���b�V������Ă��邩�ǂ������`�F�b�N����</b>\n
	 * @retval true		�S�ăL���b�V������Ă���
	 * @retval false	��ł��L���b�V������Ă��Ȃ����̂�����
	 */
	virtual bool IsCached() const;

	/**
	 * @brief �`���Ԃ�
	 * 
	 * @warning ���̊֐��́A�L���b�V����������΃L���b�V���������쐬���܂��B
	 * @retval CGeometryBase*
	 */
	virtual CGeometryBase* GetShape();
	/**
	 * @brief �`���Ԃ�
	 * 
	 * @warning ���̊֐���const�ł����A�L���b�V����������΃L���b�V���������쐬���܂��B
	 * @retval CGeometryBase*
	 */
	const CGeometryBase* GetShape() const;

	/**
	 * @brief �`����Z�b�g����
	 * 
	 * INF_*���̌`��������Ȃ��I�u�W�F�N�g�ł����z�I�Ȍ`���
	 * �����Ƃ��ł��܂��B
	 * @param Shape [in] �Z�b�g����`��i�����ŃR�s�[���쐬���܂��j
	 * @return sindyErrCode
	 */
	virtual errorcode::sindyErrCode SetShape( IGeometry* Shape );

	/**
	 * @brief �`��̃R�s�[���擾����
	 *
	 * @note ���̊֐��́A�ύX�O�E�ύX����܂ޑS�Ă̌`��̃R�s�[���擾���邱�Ƃ��ł��܂��B
	 * ������Ԃ��w�肵���ꍇ��Union���ꂽ�`��ƂȂ�܂��B
	 * @note IGeometry::get_ShapeCopy�����̋@�\����������ɂ́A�����ɉ����w�肹���Ɏg�p���Ă��������B
	 * 
	 * @param emMode       [in,optional] �R�s�[���擾����ۂ̃��[�h�iShapeMode���Q�� �f�t�H���g�FemLatest�j
	 * @return IGeometryPtr
	 */
	virtual IGeometryPtr GetShapeCopy( ShapeMode emMode = enumLatest ) const;

	/**
	 * @brief ���݊i�[����Ă���t�B�[�`���̑��݋�`��Ԃ�
	 *
	 * @note ���̊֐��́A�Ⴆ�ΕҏW��̍ĕ`�������ꍇ�ɕύX�O�E�ύX��̌`����܂񂾋�`���~�����ꍇ�ȂǂɎg�p���܂��B
	 * @warning Store()���Ă��܂��ƃL���b�V���͑S�Ĕj������܂��̂ŁA�ύX�O�̌`�󂪎擾�ł��Ȃ��Ȃ�܂��B
	 * �ύX�O�̌`����擾����K�v������ꍇ��Store()�O�ɂ��̊֐����ĂԕK�v������܂��B
	 *
	 * @param emMode       [in,optional] �R�s�[���擾����ۂ̃��[�h�iShapeMode���Q�� �f�t�H���g�FemLatest�j
	 * @return IEnvelopePtr
	 */
	IEnvelopePtr GetEnvelope( ShapeMode emMode = enumLatest ) const;

	/**
	 * @brief <b>�������Z�b�g����</b>\n
	 * �����t�B�[���h�����݂��Ȃ��Ă��G���[���o���܂���B\n
	 * �������f�p�̍�ƖړI�̏ꍇ�͌����t���O��t�^���܂��B\n
	 *
	 * @warn CRowBase::Store���ĂԂƂ��̊֐����ꏏ�ɌĂ΂�܂��B�]���āACRowBase::Store��
	 * �g�p����ꍇ�͂��̊֐��͌ĂԕK�v�͂���܂���B
	 * ������SetAutoValue���Ăт܂��B
	 *
	 * @note ������emCode��UPDATE_C�ɕϊ����Ă��܂����AemCode�͑����X�V���`��X�V�Ƃ�����Ԃ������Ȃ����߁A
	 * ���̏ꍇ�ɂ͌`��X�V�ƂȂ�܂��B�����AUPDATE_C�X�V���ɂ͑����X�V���`��X�V�̏�Ԃɂ���
	 * �K�v���L�邽�߁AbAttrUpdated��UPDATE_C���X�V����ۂ�emCode���`��ύX�������ꍇ�ɂ̂ݗL���ɓ����܂��B
	 *
	 * @param emCode [in] �X�V�R�[�h
	 * @param bAttrUpdated [in,optional] �����X�V���ꂽ���ǂ����i�f�t�H���g�Ffalse�j
	 *
	 * @return sindyErrCode
	 */
	errorcode::sindyErrCode SetHistory( schema::ipc_table::update_type::ECode emCode, bool bAttrUpdated = false );

	/**
	 * @breif �����t�^������t�^����
	 *
	 * SiNDY���[���Ƀ��O�C�����Ă��Ȃ��ꍇ�͉���������sindyErr_NoErr��Ԃ��܂��B
	 * �T�|�[�g���Ă���^�͕�����y�ѐ����^�݂̂ł��B
	 *
	 * @retval sindyErr_NoErr ����I��
	 * @retval sindyErr_AlgorithmFailed �T�|�[�g����Ă��Ȃ��^�ɑ΂��ĕt�^���悤�Ƃ���
	 */
	virtual errorcode::sindyErrCode SetAutoValue();

	/**
	 * @brief <b>�R�s�[����</b>\n
	 * dest�ɃL���b�V��������ꍇ�́A�폜����܂��B
	 * �L���b�V��������ꍇ�́A�L���b�V�����ꏏ�ɃR�s�[����܂��B
	 * @param dest              [in]�@�y�[�X�g�Ώ�
	 * @param bIsCopyCache      [in] �L���b�V�����R�s�[���邩�ǂ���
	 * @param bIsCopyRowBase    [in] �I�u�W�F�N�g���R�s�[���邩�ǂ���
	 * @param bIsCopyFlag       [in] �쐬�폜�t���O���R�s�[���邩�ǂ���
	 * @param bIsCopyHistory    [in] �ҏW�������R�s�[���邩�ǂ���
	 * @param bIsCopySchema     [in] �X�L�[�}�֌W���R�s�[���邩�ǂ���
	 * @return sindyErrCode
	 */
	virtual errorcode::sindyErrCode CopyTo( CRowBase& dest, bool bIsCopyCache=true, bool bIsCopyRowBase=true, bool bIsCopyFlag=true, bool bIsCopyHistory = true, bool bIsCopySchema=true ) const;

	/**
	 * @brief <b>���g����������</b>\n
	 * @param dest				[in]	�y�[�X�g�Ώ�
	 * @param src				[in]	�R�s�[�Ώ�
	 * @param bIsCopyCache		[in]	�L���b�V�����R�s�[���邩�ǂ���
	 * @param bIsCopyRowBase		[in]	�I�u�W�F�N�g���R�s�[���邩�ǂ���
	 * @param bIsCopyFlag		[in]	�쐬�폜�t���O���R�s�[���邩�ǂ���
	 * @param bIsCopyContainer	[in]	�R���e�i���R�s�[���邩�ǂ���
	 * @return sindyErrCode
	 */
	void swap(CRowBase& src, bool bIsCopyCache=true, bool bIsCopyRowBase=true, bool bIsCopyFlag=true/*, bool bIsCopyContainer=true*/ );

	/**
	 * @brief <b>�^����ꂽ�`��͈̔͂ŕ��s�ړ��������s��</b>\n
	 * �ҏW���[�����l�����Ĉړ��������s���܂��B\n
	 * �ҏW�ς݃`�F�b�N�͍s���܂���B
	 * @param cGeom			[in]	�N�G���`��
	 * @param pairMove		[in]	�ړ�����
	 * @param emMode		[in]	�ړ����[�h
	 * @param cErrInfos		[out]	�G���[���i�[�R���e�i
	 * @return sindyErrCode
	 */
	virtual errorcode::sindyErrCode Move( const CGeometryBase& cGeom, const std::pair<double,double>& pairMove, sindyeMoveMode emMode, CErrorObjectsBase& cErrObjects );

	/**
	 * @brief ����2�̃��R�[�h�̑����l���������ǂ���
	 *
	 * @param src			[in]			��r���郌�R�[�h�i_IRowPtr or IFeaturePtr�j
	 * @param cExclusions   [in,optional]   ���O�ΏۃJ������
	 *
	 * @note SiNDY-e �I�ɕҏW�\�ȃt�B�[���h�̂݃`�F�b�N���܂�
	 * 
	 * @retval true  ���������l������
	 * @retval false �قȂ鑮���l������
	 */
	bool IsSameAttribute(const CRowBase& src, const CSimpleArray<CString>& cExclusions) const;

	/**
	 * @brief �쐬�t���O�������Ă��邩
	 *
	 * @retval true  �쐬�t���O�������Ă���
	 * @retval false �쐬�t���O�������Ă��Ȃ�
	 */
	bool IsCreated() const { return m_bCreated; }
	/**
	 * @brief �폜�t���O�������Ă��邩
	 *
	 * @retval true  �폜�t���O�������Ă���
	 * @retval false �폜�t���O�������Ă��Ȃ�
	 */
	bool IsDeleted() const{ return m_bDeleted; }

	void Trace( bool bAttribute = true, bool bGeometry = true ) const;

	const VARIANT* GetCacheValue( long index ) const;
	VARIANT* GetCacheValue( long index );
	const VARIANT* GetValuePointer( long index, CComVariant& va ) const;
	VARIANT* GetValuePointer( long index, CComVariant& va );
protected:
	/**
	 * @brief <b>�����o�ϐ�������������</b>\n
	 * �R���X�g���N�^����Ă΂�܂��B
	 * �p����N���X�ŃI�[�o�[���C�h�����ꍇ�A
	 * C++�̋K��ɂ��Ⴆvirtual�ł��R���X�g���N�^��
	 * CRowBase::Init()���K���Ă΂�邱�Ƃɒ��ӂ��Ă��������B
	 */
	virtual void Init();
protected:
	CCacheAttribute*				m_pCache;		//!< �����L���b�V��
	CGeometryBase*					m_pGeom;		//!< �`��L���b�V��
	mutable long					m_lOID;			//!< OBJECTID�L���b�V��
	bool							m_bCreated;		//!< �쐬�t���O
	bool							m_bDeleted;		//!< �폜�t���O
};

bool operator==( int null, const CRowBase& obj );
bool operator!=( int null, const CRowBase& obj );
bool operator==( _IRow* lp, const CRowBase& obj );
inline bool operator==( int null, const CRowBase& obj )
{
	return obj.operator ==( null );
}
inline bool operator!=( int null, const CRowBase& obj )
{
	return obj.operator !=( null );
}
inline bool operator==( _IRow* lp, const CRowBase& obj )
{
	return obj.operator ==( lp );
}

#define CAST_ROWB boost::static_pointer_cast<CRowBase>

} // namepsace sindy
#pragma warning(pop)

#endif // _ROWBASE_H_
