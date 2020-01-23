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
 * @file ModifyCheck.h
 */
#ifndef _MODIFYCHECK_H_
#define _MODIFYCHECK_H_

#include "resource.h"       // ���C�� �V���{��

#include "exportimport.h"
#include "sde_ondemand.h"
#include "time.h"

namespace sindy {
class CRow;

#ifdef _TEST_NEW_MODIFYCHECK
extern SINDYLIB_API TCHAR g_strModifyCheckProcedure[];	//!< ModifyCheck�v���V�[�W����
#endif // ifdef _TEST_NEW_MODIFYCHECK

class SINDYLIB_API CModifyCheck
{
/*	struct ADDSDATA
	{
		CString Operator;
		LONG PurposeID;
		CString Purpose;
		tm DateTime;
		BOOL UsefulDateTime;
		LONG UpdateTypeID;
		CString UpdateType;
		LONG StateID;
	};

	struct STRLONG
	{
		CString TABLE;
		LONG OID;
		BOOL DELETED;
	};*/
	public:
	/**
	 * @class CModifyInfo
	 * @brief <b>�I�u�W�F�N�g�̍X�V�������Ǘ�����N���X</b>\n
	 * DLL�O�ł̃���������������邽�߁A�R���X�g���N�^���͂��߂Ƃ���
	 * �������������͂��ׂ�*.cpp�ɋL�q���Ă���܂�
	 */
	class CModifyInfo
	{
	public:
		CModifyInfo();
		virtual ~CModifyInfo();

		CModifyInfo( const CModifyInfo& cInfo ) { *this = cInfo; }
		CModifyInfo& operator=( const CModifyInfo& cInfo );

		void SetOperator( LPCTSTR lpcszName );
		void SetTime( LPCTSTR lpcszTime );
		void SetTime( LPSYSTEMTIME lpsysTime );
		void SetTime( const tm& tmTime );
		void SetTableName( LPCTSTR lpcszName );
		void SetUpdateType( LPCTSTR lpcszType );
		void SetPurpose( LPCTSTR lpcszPurpose );
		LPCTSTR GetOperator() const { return m_strOperator; }
		LPCTSTR GetTime() const { return m_strTime; }
		LPCTSTR GetTableName() const { return m_strTableName; }
		LPCTSTR GetUpdateType() const { return m_strUpdateType; }
		LPCTSTR GetPurpose() const { return m_strPurpose; }
	public:
		bool m_bIsModified;	//!< �`�F�b�N�Ɉ������������ꍇ�͏�������E���Ȃ��Ɋւ�炸true�i�v���V�[�W���ŃG���[�������������ǂ����̊m�F�p�j

		long m_lOID;
		long m_lPurposeID;
		long m_lStateID;
		long m_lUpdateTypeID;
		bool m_bIsLocal;
		bool m_bDeleted;
		bool m_bUsefulTime;
//		tm   m_tmTime;
	private:
		CString m_strOperator;
		CString m_strTime;
		CString m_strTableName;
		CString m_strUpdateType;
		CString m_strPurpose;
	};

	CModifyCheck();
	virtual ~CModifyCheck();

	bool IsModified( const CRow& cRow, CModifyInfo& cInfo );
private:
	/**
	 * @brief
	 */
	BOOL CreateConnection( const CRow& cRow, CString& strVersion, CString& strDatasetName, CString& strConnStr );

	/**
	 * @brief �����̕ҏW�n�����J���}��؂�Ŏ擾����
	 *
	 * @param cSdeConnect	[in]	�Z�b�V����
	 * @param lpcszVer		[in]	�o�[�W�������i�ڑ����[�U���͂��Ȃ��j
	 *
	 * @retval CString
	 */
	CString GetEditStateChain( SE_CONNECTION conn, LPCTSTR lpcszVer );

	/** 
	 * @brief �X�e�[�g�������Ă��邩�ǂ���
	 *
	 * @param cSdeConnect	[in]	�Z�b�V����
	 * @param lpcszVer		[in]	�ҏW���̃o�[�W������
	 * @param lpcszIDs		[in]	�����Ώۂ̃X�e�[�g������i�J���}��؂�j
	 * @param nActiveType	[out]	-1:Active�ł͂Ȃ� 0:�ҏW�����̓|�X�g��̃X�e�[�g 1:���[�J���o�[�W�����݂̂ɕۑ�����Ă���X�e�[�g
	 *
	 * @retval -1 �X�e�[�g�͐����ĂȂ�
	 * @retval -1�ȊO �X�e�[�g�͐����Ă���i�X�e�[�g�ԍ��j
	 */
	LONG IsActiveStates( SE_CONNECTION conn, LPCTSTR lpcszVer, LPCTSTR lpcszIDs, INT& nActiveType );

	/**
	 * @brief A�e�[�u������t�B�[�`���̃X�e�[�g���擾����
	 *
	 * @param cSdeConnect		[in]			�Z�b�V����
	 * @param lpcszTableID		[in]			SDE�ɓo�^����Ă���e�[�u��ID
	 * @param lObjectID			[in]			�X�e�[�g���擾����t�B�[�`����OBJECTID
	 * @param listAddsData		[out]			�擾�����X�e�[�g���X�g
	 * @param lpcszExcludeIDs	[in,optional]	���O���X�g
	 *
	 * @return �擾�����X�e�[�g��ID�i�J���}��؂�j
	 */
	CString GetAStates( SE_CONNECTION conn, LPCTSTR lpcszTableID, LONG lObjectID, std::list<CModifyInfo>& listAdds, LPCTSTR lpcszExcludeIDs = NULL );

	/**
	 * @brief D�e�[�u������t�B�[�`���̃X�e�[�g���擾����
	 *
	 * @param cSdeConnect		[in]			�Z�b�V����
	 * @param lpcszTableID		[in]			SDE�ɓo�^����Ă���e�[�u��ID
	 * @param lObjectID			[in]			�X�e�[�g���擾����t�B�[�`����OBJECTID
	 * @param lpcszExcludeIDs	[in,optional]	���O���X�g
	 *
	 * @return �擾�����X�e�[�g��ID�i�J���}��؂�j
	 */
	CString GetDStates( SE_CONNECTION conn, LPCTSTR lpcszTableID, LONG lObjectID, LPCTSTR lpcszExcludeIDs = NULL );

	/**
	* @brief ���͂��ꂽ���[�U�[�� A �e�[�u���� D �e�[�u�����擾����
	*
	* @param cSdeConnect	[in] SdeConnect �I�u�W�F�N�g�iSDE �Ƃ̐ڑ��Z�b�V�����j
	* @param vaUser			[in] ���ݐڑ����Ă��郆�[�U�[���iIPropertySet ����擾�j
	*
	* @retval TRUE	�擾����
	* @retval FALSE �擾���s
	**/	
	BOOL GetTableList( SE_CONNECTION conn, const VARIANT& vaUser );
	
	/**
	* @brief �ύX�`�F�b�N�Ώۂ̃I�u�W�F�N�g��ID
	*
	* @param cSdeConnect	[in] SdeConnect �I�u�W�F�N�g�iSDE �Ƃ̐ڑ��Z�b�V�����j
	* @param strTable		[in] �����Ώۃe�[�u��
	* @param bVersion		[in] Version ������
	* @param lOID			[in] �ύX�`�F�b�N�Ώۂ̃I�u�W�F�N�g��ID
	* @param sMessage		[out] �ύX����Ă����ꍇ�̃��b�Z�[�W
	*
	* @retval TRUE	�ύX���ꂽ
	* @retval FALSE �ύX����Ă��Ȃ�
	**/
	bool IsModifiedByLocal( SE_CONNECTION conn, LPCTSTR lpcszTable, LPCTSTR lpcszVersion, long lOID, CModifyInfo& cInfo );
private:
#pragma warning(push)
#pragma warning(disable: 4251)
#ifdef _MODIFYCHECK_CREATE_NEW_CONNECTION
	static std::map<CString, SE_CONNECTION>	g_mapConnection;			//!< SdeConnect �I�u�W�F�N�g��ڑ������񂲂Ƃɕێ�
#endif // ifdef _MODIFYCHECK_CREATE_NEW_CONNECTION
	static std::map<CString, CString>		g_mapTabls;					//!< A �e�[�u���� D �e�[�u���̖��O��ێ�
	CString									m_strEditStateChain;		//!< �ҏW�n���ێ��p

	bool									m_bUseProcedure;
	bool									m_bUncheck;					//!< ModifyCheck���g�p���邩�ǂ���
	bool									m_bExistProcedure;
#pragma warning(pop)
};

} // sindy

#endif // ifndef _MODIFYCHECK_H_
