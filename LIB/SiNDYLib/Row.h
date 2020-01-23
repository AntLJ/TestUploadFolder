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
 * @file Row.h
 * @brief <b>CRow�N���X��`�t�@�C��</b>\n
 * @author �n�}�c�a���암�J���O���[�v �Ð�M�G
 * $Id$
 */
#ifndef _ROW_H_
#define _ROW_H_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "../LIB/WinLib/ComWrapper.h"
#include "FieldMap.h"
#include "TableType.h"
#include "ClassUtil.h"
#include "NameString.h"
#include "Container.h"
#include "ListString.h"

#pragma warning(push)
#pragma warning(disable : 4275)
#pragma warning(disable : 4251)
namespace sindy {
class CErrorInfos;
class CErrorObject;
class CLogicRule;
class CCacheAttribute;
class CGeometry;
class CTable;

// ���Ԍv���p
extern SINDYLIB_API double g_dTotalCacheTime;	//!< CRow::CreateCache�ɂ����鎞�Ԃ̑��v

class CRow;
typedef boost::shared_ptr<CRow> CSPRow;

/**
 * @class CRow
 * @brief IRowPtr�̃��b�p�[�N���X
 *
 * �S�ẴA�C�e���N���X�͂��̃N���X�����ƂȂ�܂�
 */
class SINDYLIB_API CRow : public CComWrapper<_IRow>, public CContainer
{
public:
	CRow();
	virtual ~CRow()
	{
		DeleteCache();
	}

	CRow( const CRow& obj );
	CRow( _IRow* lp, sindyTableType::ECode emTableType = sindyTableType::unknown, bool bCreated = false, const CSPFieldMap& spFields = CSPFieldMap(), const CSPTableNameString& spNameString = CSPTableNameString() );

	CRow& operator=( const CRow& obj );
	CRow& operator=( _IRow* lp );

	operator _IRow*() const
	{
		return (_IRow*)p;
	}

	bool operator ==( const CRow& obj ) const
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
	bool operator <( const CRow& obj );

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
	 * @brief <b>�_�~�[CRow�����̉�������</b>
	 * CTable::CreateDummyRow�ō쐬�����_�~�[�ɑ΂��Ď��̂��Z�b�g���܂��B
	 * �I�u�W�F�N�g�̍쐬�^�C�~���O�����肬��܂Œx�����������ꍇ�ɗL���ł��B
	 * CRow::p���Z�b�g���邾���ł��̂ŃL���b�V�����̓N���A����܂���B
	 * �e�[�u���^�C�v���m�F���܂���̂Ń^�C�v�̈قȂ���̂��Z�b�g���Ȃ��ł��������B
	 */
	void Materialization( _IRow* lp );

	/**
	 * @brief <b>�_�~�[CRow�����̉�������</b>
	 * CTable::CreateDummyRow�ō쐬�����_�~�[�ɑ΂��Ď��̂��Z�b�g���܂��B
	 * �I�u�W�F�N�g�̍쐬�^�C�~���O�����肬��܂Œx�����������ꍇ�ɗL���ł��B
	 * CRow::p���Z�b�g���邾���ł��̂ŃL���b�V�����̓N���A����܂���B
	 * �e�[�u���^�C�v���m�F���܂���̂Ń^�C�v�̈قȂ���̂��Z�b�g���Ȃ��ł��������B
	 */
	errorcode::sindyErrCode Materialization( ITable* ipTable );

	/**
	 * @brief <b>�_�~�[CRow�����̉�������</b>
	 * CTable::CreateDummyRow�ō쐬�����_�~�[�ɑ΂��Ď��̂��Z�b�g���܂��B
	 * �I�u�W�F�N�g�̍쐬�^�C�~���O�����肬��܂Œx�����������ꍇ�ɗL���ł��B
	 * CRow::p���Z�b�g���邾���ł��̂ŃL���b�V�����̓N���A����܂���B
	 * �e�[�u���^�C�v���m�F���܂���̂Ń^�C�v�̈قȂ���̂��Z�b�g���Ȃ��ł��������B
	 */
	errorcode::sindyErrCode Materialization( ITable* ipTable, ITable* ipSubTable );

	/**
	 * @brief <b>�t�B�[�`���̃R�s�[���쐬����</b>\n
	 * �����e�[�u����ɓ������������I�u�W�F�N�g���쐬���܂��B\n
	 * �L���b�V���̓R�s�[����܂���B\n
	 * �t�B�[���h�Ή��\�A�e�[�u�����L���b�V���̓R�s�[����܂��ishared_ptr�j�B
	 * @warning �ҏW�J�n����Ă��Ȃ���΃G���[���O���o�܂��B
	 * @warning IRow���f�^�b�`����Ă���ꍇ�̓G���[���O���o�܂��B
	 * @retval CRow
	 */
	CRow Clone() const;

	/**
	 * @brief <b>�t�B�[�`���̕ύX�����ɖ߂�</b>\n
	 * �쐬�t���O�����Ă������̂�CTable::CreateDummyRow()�����̏�����
	 * �s���܂��B\n
	 * �폜�t���O��false�ɂȂ�܂��B
	 * @param bRecursive [in] CContainer�̒��g���߂������ꍇ��true(�f�t�H���g�Ffalse)
	 */
	virtual void Reset( bool bRecursive = false );

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
	 * ���f������ɂ�Store()����K�v������܂��B
	 * @param bRecursive [in,optional] CContainer�ɂ��K�p����ꍇ��true�i�f�t�H���g�Ffalse�j
	 */
	virtual void Delete( bool bRecursive = false );

	//@{ @name IRow�C���^�[�t�F�[�X
	IFieldsPtr GetFields() const;
	bool HasOID() const;
    long GetOID() const;
	virtual errorcode::sindyErrCode Store( CErrorObjects& cErrs );
	ITablePtr GetTable() const;
	errorcode::sindyErrCode GetValue( long lIndex, VARIANT& vaValue ) const;
	virtual errorcode::sindyErrCode SetValue( long lIndex, const VARIANT& vaValue );
#if defined(SINDYLIB_EXPORTS) || defined(USE_SINDYLIB_DLL)
	/**
	 * @brief <b>CComVariant��Ԃ�GetValue</b>\n
	 * ���̊֐��̓X�^�e�B�b�N�����N�ł݂̂̒񋟂ł�
	 * @param lIndex	[in]	�t�B�[���h�C���f�b�N�X�ԍ�
	 * @return CComVariant
	 */
	CComVariant GetValue( long lIndex ) const
	{
		CComVariant vaValue;
		GetValue( lIndex, vaValue );
		return vaValue;
	}
#endif // defined(SINDYLIB_EXPORTS) || defined(USE_SINDYLIB_DLL)

	//@}

	/**
	 * @brief <b>���[�N�X�y�[�X���擾����</b>\n
	 * @return IWorkspacePtr
	 */
	IWorkspacePtr GetWorkspace() const;

	/**
	 * @brief <b>�e�[�u���^�C�v���擾����</b>\n
	 * @return sindyTableType::ECode
	 */
	sindyTableType::ECode GetTableType() const;

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
	CSPFieldMap GetFieldMap() const;

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
	CSPTableNameString GetNameString() const;

	/**
	 * @brief <b>�e�[�u������Ԃ�</b>\n
	 * @see CTableNameString::GetTableName()
	 * @retval LPCTSTR
	 */
	LPCTSTR GetTableName() const;

	bool IsEqualObject(_IRow* ipRow ) const;
	bool IsEqualObject( const CRow& cRow ) const;

	/**
	 * @brief <b>�t�B�[���h���Ńt�B�[���h�l���擾����</b>\n
	 * �t�B�[���h�Ή��\��������΍쐬���Ă���擾���܂��B
	 * @param lpcszFieldName	[in]	�t�B�[���h��
	 * @param vaValue			[out]	�t�B�[���h�l
	 *
	 * @return sindyErrCode
	 */
	errorcode::sindyErrCode GetValueByFieldName( LPCTSTR lpcszFieldName, VARIANT& vaValue ) const;

#if defined(SINDYLIB_EXPORTS) || defined(USE_SINDYLIB_DLL)
	/**
	 * @brief <b>CComVariant��Ԃ�GetValueByFieldName</b>\n
	 * ���̊֐��̓X�^�e�B�b�N�����N�ł݂̂̒񋟂ł�
	 * @param lpcszFieldName	[in]	�t�B�[���h��
	 * @return CComVariant
	 */
	CComVariant getValueByFieldName( LPCTSTR lpcszFieldName ) const
	{
		CComVariant vaValue;
		GetValueByFieldName( lpcszFieldName, vaValue );
		return vaValue;
	}
#endif // defined(SINDYLIB_EXPORTS) || defined(USE_SINDYLIB_DLL)

	/**
	 * @brief <b>�t�B�[���h���Ńt�B�[���h�l���Z�b�g����</b>\n
	 * �t�B�[���h�Ή��\��������΍쐬���Ă���Z�b�g���܂��B
	 * @param lpcszFieldName	[in]	�t�B�[���h��
	 * @param vaValue			[out]	�t�B�[���h�l
	 *
	 * @return sindyErrCode
	 */
	errorcode::sindyErrCode SetValueByFieldName( LPCTSTR lpcszFieldName, const VARIANT& vaValue );

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

	/**
	 * @brief <b>���������`�F�b�N����</b>\n
	 * ���̊֐��ł͊�{�I�ȃ`�F�b�N�����s���܂���B
	 * �K�v�ȃ`�F�b�N�́A�p����̃N���X�Ŏ������Ă��������B
	 * @param cRule			[in]	�������`�F�b�N�p���[��
	 * @param cErrs			[out]	�G���[���i�[�p�R���e�i
	 * @return sindyErrCode
	 */
	virtual errorcode::sindyErrCode CheckLogic( const CLogicRule& cRule, CErrorObjects& cErrs ) const;

	/**
	 * @brief <b>�R���t���N�g����̂��߂̊��ҏW�`�F�b�N���s��</b>\n
	 * @param cErrs		[out]	�G���[���i�[�p�R���e�i
	 * @return sindyErrCode
	 */
	errorcode::sindyErrCode CheckModify( CErrorObjects& cErrs ) const;

	/**
	 * @brief <b>�ҏW�\���ǂ������`�F�b�N����</b>\n
	 * �ҏW���[���Ɋ�Â��ĕҏW�\���ǂ������`�F�b�N���܂��B\n
	 * �ҏW�ς݃`�F�b�N�͍s���܂���B
	 * @param emType		[in]	�ҏW�^�C�v
	 * @param cErrInfos		[out]	�G���[�i�[�p�R���e�i
	 * @return sindyErrCode
	 */
	errorcode::sindyErrCode CheckEditable( sindyeEditType emType, CErrorInfos& cErrInfos ) const;

	/**
	 * @brief <b>�C���f�b�N�X�ԍ���bool�l���擾����</b>\n
	 * �t�B�[���h�^�C�v��long�ł��A�g�p���@��bool�̏ꍇ�͂�����g�p���܂��B
	 * @param index				[in]			�t�B�[���h�C���f�b�N�X�ԍ�
	 * @param bDefaultValue		[in]			NULL�A�܂��̓G���[�̏ꍇ�ɕԂ��l
	 * @return bool
	 */
	bool GetBoolValue( long index, bool bDefaultValue ) const;

	/**
	 * @brief �C���f�b�N�X�ԍ���bool�l���Z�b�g����\n
	 * �t�B�[���h�^�C�v��long�ł��A�g�p���@��bool�̏ꍇ�͂�����g�p���܂��B
	 * @param index				[in]			�t�B�[���h�C���f�b�N�X�ԍ�
	 * @param bNewValue			[in]			�Z�b�g����l
	 * @return sindyErrCode
	 */
	errorcode::sindyErrCode SetBoolValue( long index, bool bNewValue );

	/**
	 * @brief <b>�C���f�b�N�X�ԍ���long�l���擾����</b>\n
	 * @param index				[in]			�t�B�[���h�C���f�b�N�X�ԍ�
	 * @param lDefaultValue		[in]			NULL�A���̓G���[�̏ꍇ�ɕԂ��l
	 * @return long
	 */
	long GetLongValue( long index, long lDefaultValue ) const;

	/**
	 * @brief �C���f�b�N�X�ԍ���long�l���Z�b�g����
	 * @param index				[in]			�t�B�[���h�C���f�b�N�X�ԍ�
	 * @param lNewValue			[in]			�Z�b�g����l
	 * @return sindyErrCode
	 */
	errorcode::sindyErrCode SetLongValue( long index, long lNewValue );

	/**
	 * @brief �C���f�b�N�X�ԍ���double�l���擾����
	 * @param index				[in]			�t�B�[���h�C���f�b�N�X�ԍ�
	 * @param dDefaultValue		[in]			NULL�A�܂��̓G���[�̏ꍇ�ɕԂ��l
	 * @return double
	 */
	double GetDoubleValue( long index, const double& dDefaultValue ) const;

	/**
	 * @brief �C���f�b�N�X�ԍ���double�l���Z�b�g����
	 * @param index				[in]			�t�B�[���h�C���f�b�N�X�ԍ�
	 * @param dNewValue			[in]			�Z�b�g����l
	 * @return sindyErrCode
	 */
	errorcode::sindyErrCode SetDoubleValue( long index, const double& dNewValue );
	/**
	 * @brief �C���f�b�N�X�ԍ���String�l���擾����\n
	 * �l��NULL�A���͎擾�Ɏ��s�����ꍇ��NULL���Ԃ�܂��B
	 * @param index				[in]			�t�B�[���h�C���f�b�N�X�ԍ�
	 * @param nLen				[in]			lpszValue�̎��[�\������
	 * @param lpszValue			[out]			�擾��������i�[����o�b�t�@
	 * @retval NULL �G���[�̏ꍇ
	 * @retval �󕶎��� �l��NULL�̏ꍇ
	 */
	LPCTSTR GetStringValue( long index, int nLen, LPTSTR lpszValue ) const;

	/**
	 * @brief �C���f�b�N�X�ԍ��ŕ�����l���Z�b�g����
	 * @param index				[in]			�t�B�[���h�C���f�b�N�X�ԍ�
	 * @param lpcszNewValue		[in]			�Z�b�g����l
	 * @return sindyErrCode
	 */
	errorcode::sindyErrCode SetStringValue( long index, LPCTSTR lpcszNewValue );

	/**
	 * @brief <b>�t�B�[���h����bool�l���擾����</b>\n
	 * �t�B�[���h�^�C�v��long�ł��A�g�p���@��bool�̏ꍇ�͂�����g�p���܂��B
	 * @param lpcszFieldName	[in]			�t�B�[���h��
	 * @param bDefaultValue		[in]			NULL�A�܂��̓G���[�̏ꍇ�ɕԂ��l
	 * @return bool
	 */
	bool GetBoolValueByFieldName( LPCTSTR lpcszFieldName, bool bDefaultValue ) const;

	/**
	 * @brief �t�B�[���h����bool�l���Z�b�g����\n
	 * �t�B�[���h�^�C�v��long�ł��A�g�p���@��bool�̏ꍇ�͂�����g�p���܂��B
	 * @param lpcszFieldName	[in]			�t�B�[���h��
	 * @param bNewValue			[in]			�Z�b�g����l
	 * @return sindyErrCode
	 */
	errorcode::sindyErrCode SetBoolValueByFieldName( LPCTSTR lpcszFieldName, bool bNewValue );

	/**
	 * @brief �t�B�[���h����long�l���擾����
	 * @param lpcszFieldName	[in]			�t�B�[���h��
	 * @param lDefaultValue		[in]			NULL�A�܂��̓G���[�̏ꍇ�ɕԂ��l
	 * @return long
	 */
	long GetLongValueByFieldName( LPCTSTR lpcszFieldName, long lDefaultValue ) const;

	/**
	 * @brief �t�B�[���h����long�l���Z�b�g����
	 * @param lpcszFieldName	[in]			�t�B�[���h��
	 * @param lNewValue			[in]			�Z�b�g����l
	 * @return sindyErrCode
	 */
	errorcode::sindyErrCode SetLongValueByFieldName( LPCTSTR lpcszFieldName, long lNewValue );

	/**
	 * @brief �t�B�[���h����double�l���擾����
	 * @param lpcszFieldName	[in]			�t�B�[���h��
	 * @param dDefaultValue		[in]			NULL�A�܂��̓G���[�̏ꍇ�ɕԂ��l
	 * @return double
	 */
	double GetDoubleValueByFieldName( LPCTSTR lpcszFieldName, const double& dDefaultValue ) const;

	/**
	 * @brief �t�B�[���h����double�l���Z�b�g����
	 * @param lpcszFieldName	[in]			�t�B�[���h��
	 * @param dNewValue			[in]			�Z�b�g����l
	 * @return sindyErrCode
	 */
	errorcode::sindyErrCode SetDoubleValueByFieldName( LPCTSTR lpcszFieldName, const double& dNewValue );
	/**
	 * @brief �t�B�[���h����String�l���擾����\n
	 * �l��NULL�A���͎擾�Ɏ��s�����ꍇ��NULL���Ԃ�܂��B\n
	 * @param lpcszFieldName	[in]			�t�B�[���h��
	 * @param nLen				[in]			lpszValue�̎��[�\������
	 * @param lpszValue			[out]			�擾��������i�[����o�b�t�@
	 * @retval NULL �G���[�̏ꍇ���͒l��NULL�̏ꍇ
	 * @retval NULL�ȊO �t�B�[���h�l
	 */
	LPCTSTR GetStringValueByFieldName( LPCTSTR lpcszFieldName, int nLen, LPTSTR lpszValue ) const;

	/**
	 * @brief <b>�t�B�[���h����String�l�̃T�C�Y���擾����</b>\n
	 * �i�[����Ă��镶�������擾�������ꍇ�́AGetStringValueNumByFieldName���g�p���Ă��������B\n
	 * �I�[��NULL�̓J�E���g�Ɋ܂߂܂���B
	 * @param lpcszFieldName	[in]			�t�B�[���h��
	 * @retval -1 �G���[
	 * @retval -1�ȊO �T�C�Y�i�o�b�t�@���j
	 */
	int GetStringValueLengthByFieldName( LPCTSTR lpcszFieldName ) const;

	/**
	 * @brief <b>�t�B�[���h����String�l�̕��������擾����</b>\n
	 * �o�b�t�@�����擾�������ꍇ�́AGetStringValueLengthByFieldName���g�p���Ă��������B\n
	 * @param lpcszFieldName	[in]			�t�B�[���h��
	 * @retval -1 �G���[
	 * @retval -1�ȊO ������
	 */
	int GetStringValueNumByFieldName( LPCTSTR lpcszFieldName ) const;

	/**
	 * @brief �t�B�[���h���ŕ�����l���Z�b�g����
	 * @param lpcszFieldName	[in]			�t�B�[���h��
	 * @param lpcszNewValue		[in]			�Z�b�g����l
	 * @return sindyErrCode
	 */
	errorcode::sindyErrCode SetStringValueByFieldName( LPCTSTR lpcszFieldName, LPCTSTR lpcszNewValue );

	/**
	 * @brief <b>�����E�`��Ȃǂ̃L���b�V�����쐬����</b>\n
	 * �t�B�[���h�Ή��\�A�e�[�u�����L���b�V���I�u�W�F�N�g��������΍쐬���Ă���쐬���܂����A
	 * ���̏ꍇ�ق��̃I�u�W�F�N�g�Ƌ��L�ł��Ȃ����߃��\�[�X�𑽂��K�v�Ƃ��܂��B
	 * ���\�[�X���ǂ��ł��悢�ꍇ�A�y�уI�u�W�F�N�g�����Ȃ��ꍇ�������ċɗ̓t�B�[���h�Ή��\
	 * �ƃe�[�u�����L���b�V���I�u�W�F�N�g��n���悤�ɂ��Ă��������B\n
	 * ���ɃL���b�V�������݂���ꍇ�̓L���b�V�����쐬���܂���B�܂��A
	 * bDetatch��true�ɂ���ƁA�L���b�V���쐬���
	 * _IRow*��؂藣���܂��B
	 * @param bDetach			[in,optional]	������_IRow*��؂藣���ꍇ��true
	 * @param ipFields			[in,optional]	�t�B�[���h�Ή��\�I�u�W�F�N�g(shared_ptr)
	 * @param spNameString		[in,optional]	�e�[�u�������Ǘ��I�u�W�F�N�g(shared_ptr)
	 * @return sindyErrCode;
	 */
	virtual errorcode::sindyErrCode CreateCache( bool bDetatch = false, const CSPFieldMap& spFields = CSPFieldMap(), const CSPTableNameString& spNameString = CSPTableNameString() );

	/**
	 * @brief <b>�L���b�V�����폜����</b>\n
	 * �f�X�g���N�^����Ă΂�܂��B
	 * �p����N���X�ŃI�[�o�[���C�h�����ꍇ�A
	 * C++�̋K��ɂ��Ⴆvirtual�ł��f�X�g���N�^��
	 * CRow::DeleteCache()���K���Ă΂�邱�Ƃɒ��ӂ��Ă��������B
	 */
	virtual void DeleteCache( bool bDeleteContainerCache = false );

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
	 * @retval CGeometry*
	 */
	virtual CGeometry* GetShape();
	/**
	 * @brief �`���Ԃ�
	 * 
	 * @warning ���̊֐���const�ł����A�L���b�V����������΃L���b�V���������쐬���܂��B
	 * @retval CGeometry*
	 */
	const CGeometry* GetShape() const;

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
	 * @brief <b>���݊i�[����Ă���t�B�[�`���̌`��̃R�s�[��Ԃ�</b>\n
	 * @param bLatest               	[in,optional] �ŐV�̌`�󂪗~�����ꍇ��true�i�f�t�H���g�Ftrue�j
	 * @param bOriginal             	[in,optional] �ύX�O�̌`�󂪗~�����ꍇ��true�i�f�t�H���g�Ffalse�j
	 * @param bFeatureChangedOriginal	[in,optional] IFeatureChanges::get_OriginalShape�̌`�󂪗~�����ꍇ�ɂ�true�i�f�t�H���g�Ffalse�j
	 * @param bDiff						[in,optional] ������Ԃ��w�肵�����ɁAUnion�����`��ł͂Ȃ������`��ɂ��邩�ǂ���
	 * @param lParam					[in,optional] �C�ӈ����iCRow::GetShapeCopy�ł͎g�p���܂��� �f�t�H���g�F-1�j\n�p����Ŏg�p���܂��B
	 * @note ���̊֐��́A�ύX�O�E�ύX����܂ޑS�Ă̌`��̃R�s�[���擾���邱�Ƃ��ł��܂��B
	 * ������Ԃ��w�肵���ꍇ��Union���ꂽ�`��ƂȂ�܂��B
	 * @note IGeometry::get_ShapeCopy�����̋@�\����������ɂ́A�����ɉ����w�肹���Ɏg�p���Ă��������B
	 */
	virtual IGeometryPtr GetShapeCopy( bool bLatest = true, bool bOriginal = false, bool bFeatureChangedOriginal = false, bool bDiff = false, long lParam = -1 ) const;

	/**
	 * @brief <b>�������Z�b�g����</b>\n
	 * �����t�B�[���h�����݂��Ȃ��Ă��G���[���o���܂���B\n
	 * �������f�p�̍�ƖړI�̏ꍇ�͌����t���O��t�^���܂��B\n
	 *
	 * @warn CRow::Store���ĂԂƂ��̊֐����ꏏ�ɌĂ΂�܂��B�]���āACRow::Store��
	 * �g�p����ꍇ�͂��̊֐��͌ĂԕK�v�͂���܂���B
	 *
	 * @return sindyErrCode
	 */
	errorcode::sindyErrCode SetHistory( schema::ipc_table::update_type::ECode emCode );

	/**
	 * @brief <b>�R�s�[����</b>\n
	 * dest�ɃL���b�V��������ꍇ�́A�폜����܂��B
	 * �L���b�V��������ꍇ�́A�L���b�V�����ꏏ�ɃR�s�[����܂��B
	 * @param dest				[in]	�y�[�X�g�Ώ�
	 * @param bIsCopyCache		[in]	�L���b�V�����R�s�[���邩�ǂ���
	 * @param bIsCopyRow		[in]	�I�u�W�F�N�g���R�s�[���邩�ǂ���
	 * @param bIsCopyFlag		[in]	�쐬�폜�t���O���R�s�[���邩�ǂ���
	 * @param bIsCopyContainer	[in]	�R���e�i���R�s�[���邩�ǂ���
	 * @return sindyErrCode
	 */
	virtual errorcode::sindyErrCode CopyTo( CRow& dest, bool bIsCopyCache=true, bool bIsCopyRow=true, bool bIsCopyFlag=true, bool bIsCopyContainer=true ) const;

	/**
	 * @brief <b>���g����������</b>\n
	 * @param dest				[in]	�y�[�X�g�Ώ�
	 * @param src				[in]	�R�s�[�Ώ�
	 * @param bIsCopyCache		[in]	�L���b�V�����R�s�[���邩�ǂ���
	 * @param bIsCopyRow		[in]	�I�u�W�F�N�g���R�s�[���邩�ǂ���
	 * @param bIsCopyFlag		[in]	�쐬�폜�t���O���R�s�[���邩�ǂ���
	 * @param bIsCopyContainer	[in]	�R���e�i���R�s�[���邩�ǂ���
	 * @return sindyErrCode
	 */
	void swap(CRow& src, bool bIsCopyCache=true, bool bIsCopyRow=true, bool bIsCopyFlag=true, bool bIsCopyContainer=true );

	/**
	 * @brief IRow*��V�������̂ɑ}���ւ���
	 * @param pTargetTable [in,optional] �w�肵�Ȃ���Γ����e�[�u���ɃN���[�����쐬�i�f�t�H���g�FNULL�j
	 * @param bRecursive [in,optional] �K�w�I�Ɏ��s����̂ł����true�i�f�t�H���g�Ffalse�j
	 * @warning ipTargetTable�͓����\���̃e�[�u���ł���K�v������܂��i�֐����̂͐���I�����܂��j�B
	 * @note Detach�ς݊���ipTargetTable=NULL�̏ꍇ�̓G���[�ƂȂ�܂��B
	 */
	errorcode::sindyErrCode ReplaceNewRow( const CTable* pTargetTable = NULL, bool bRecursive = false );

	/**
	 * @brief ����2�̃��R�[�h�̑����l���������ǂ���
	 *
	 * @param src			[in]			��r���郌�R�[�h�i_IRowPtr or IFeaturePtr�j
	 *
	 * @note SiNDY-e �I�ɕҏW�\�ȃt�B�[���h�̂݃`�F�b�N���܂�
	 * 
	 * @retval true  ���������l������
	 * @retval false �قȂ鑮���l������
	 */
	bool IsSameAttribute(const CRow& src, const CListString& cExclusions=CListString()) const;

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
protected:
	/**
	 * @brief <b>�����o�ϐ�������������</b>\n
	 * �R���X�g���N�^����Ă΂�܂��B
	 * �p����N���X�ŃI�[�o�[���C�h�����ꍇ�A
	 * C++�̋K��ɂ��Ⴆvirtual�ł��R���X�g���N�^��
	 * CRow::Init()���K���Ă΂�邱�Ƃɒ��ӂ��Ă��������B
	 */
	virtual void Init();

	/** 
	* @brief �L���b�V���𔽉f������
	*
	* CRow::Store()����̕����ł��B
	* �L���b�V����p�ɔ��f�����܂��B
	* p�����݂��Ȃ��ꍇ�͉������܂���B
	* ���f����L���b�V���͍폜���܂���B
	*
	* @param emUpdate [out] �X�V�^�C�v
	* @param cErrs [out] �G���[�I�u�W�F�N�g�R���e�i
	* @param pErr [out] �G���[�I�u�W�F�N�g
	*/
	void UpdateCache( schema::ipc_table::update_type::ECode& emUpdate, CErrorObjects& cErrs, CErrorObject* pErr );
protected:
	mutable sindyTableType::ECode	m_emTableType;	//!< �i�[����Ă���t�B�[�`���̃e�[�u�����ʎq
	mutable CSPTableNameString		m_spNameString;	//!< �e�[�u�����L���b�V���p�ishared ptr�j
	mutable CSPFieldMap				m_spFields;		//!< �t�B�[���h�Ή��\�ishared ptr�j

	CCacheAttribute*				m_pCache;		//!< �����L���b�V��
	CGeometry*						m_pGeom;		//!< �`��L���b�V��
	mutable long					m_lOID;			//!< OBJECTID�L���b�V��
	bool							m_bCreated;		//!< �쐬�t���O
	bool							m_bDeleted;		//!< �폜�t���O
};

bool operator==( int null, const CRow& obj );
bool operator!=( int null, const CRow& obj );
inline bool operator==( int null, const CRow& obj )
{
	return obj.operator ==( null );
}
inline bool operator!=( int null, const CRow& obj )
{
	return obj.operator !=( null );
}

} // namepsace sindy
#pragma warning(pop)

#endif // _ROW_H_
