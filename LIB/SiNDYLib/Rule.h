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
 * @file Rule.h
 * CRule�N���X��`�t�@�C��
 * @author �n�}�c�a���암�J���O���[�v �Ð�M�G
 * $Id$
 */
#ifndef _RULE_H_
#define _RULE_H_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "AnnotationClassTable.h"
#include "MovableLayersTable.h"
#include "DeletableLayersTable.h"
#include "EditAttrLayersTable.h"
#include "CreatableLayersTable.h"
#include "WorkPurposeTable.h"
#include "LayerNamesTable.h"
#include "SiNDYeSettings.h"
#include "BackGroundClassTable.h"
#include "ConnectUserTable.h"
#include "DispNaviRelationTable.h"
#include "DispOtherRelationTable.h"
#include "EditHistoryTable.h"
#include "OperatorTable.h"
#include "RoadAttributeTable.h"
#include "RoadDispClassTable.h"
#include "RoadNaviClassTable.h"
#include "SharedDeleteLayersTable.h"
#include "SharedMoveLayersTable.h"
#include "LockAreaTable.h"
#include "LockMeshTable.h"
#include "EditMeshTable.h"
#include "RuleCommunity.h"
#include "AutoValue.h"

class CListString;
class CListLong;
class CSiNDYRuleObj;
namespace sindy {
/**
 * @class CRule
 * @brief <b>���[���Ǘ��N���X</b>\n
 * ���̃N���X�́A���[�����L���b�V�����Ďg�p���邱�Ƃ��O���
 * �Ȃ��Ă��܂��̂ŁA�g�p����O�ɕK��CreateRuleCache���Ă�
 * �K�v������܂��B�������A�ȉ��̏ꍇ�͎����I�ɍ쐬����܂��B
 * @li ���O�C���_�C�A���O��\��������
 * @li sindy::CApplication��������������
 * @li sindy::CWorkspaces�Őڑ����Ǘ�����ꍇ�i�o�b�`�n�j
 */
#pragma warning(push)
#pragma warning(disable: 4251)
class SINDYLIB_API CRule
{
public:
	CRule();
	virtual ~CRule();

	friend class CSiNDYRuleObj;

	/**
	 * @brief <b>���[���̃L���b�V�����쐬����</b>\n
	 * �L���b�V�����쐬������́A�ڑ���؂�܂��B
	 * ���ɃL���b�V�������݂���ꍇ�͉������܂���B
	 * �L���b�V������蒼�������ꍇ��RecreateRuleCache��
	 * �Ă�ł��������B
	 * 
	 * @note ����̍�ƖړI�����L���b�V���������ꍇ��lPurposeID�ɍ�ƖړIID���Z�b�g���܂��B
	 * �f�t�H���g�̏�Ԃł͑S��ƖړI���L���b�V�����܂��B
	 *
	 * @param bAnnoRule	[in,optional] ���L�̃��[����ǂݍ��ޏꍇ��true�i�f�t�H���g�Ftrue�j
	 * @param bEditRule [in,optional] �ҏW���[����ǂݍ��ޏꍇ��true�i�f�t�H���g�Ftrue�j
	 * @param bBGClass [in,optional] �w�i��ʃ��[����ǂݍ��ޏꍇ��true�i�f�t�H���g�Ftrue�j
	 * @param lPurposeID [in,optional] ��ƖړI�����肵�ăL���b�V�����쐬����i�f�t�H���g�F-1�j
	 * @return sindyErrCode
	 */
	errorcode::sindyErrCode CreateRuleCache( bool bAnnoRule = true, bool bEditRule = true, bool bBGClass = true, long lPurposeID = -1 );

	/**
	 * @brief <b>���[���̃L���b�V�����쐬���Ȃ���</b>\n
	 * �L���b�V�����쐬������́A�ڑ���؂�܂��B
	 * ���ɃL���b�V�������݂���ꍇ�̓L���b�V�����폜���Ă���쐬���܂��B
	 * 
	 * @note ����̍�ƖړI�����L���b�V���������ꍇ��lPurposeID�ɍ�ƖړIID���Z�b�g���܂��B
	 * �f�t�H���g�̏�Ԃł͑S��ƖړI���L���b�V�����܂��B
	 *
	 * @param bAnnoRule	[in,optional] ���L�̃��[����ǂݍ��ޏꍇ��true�i�f�t�H���g�Ftrue�j
	 * @param bEditRule [in,optional] �ҏW���[����ǂݍ��ޏꍇ��true�i�f�t�H���g�Ftrue�j
	 * @param bBGClass [in,optional] �w�i��ʃ��[����ǂݍ��ޏꍇ��true�i�f�t�H���g�Ftrue�j
	 * @param lPurposeID [in,optional] ��ƖړI�����肵�ăL���b�V�����쐬����i�f�t�H���g�F-1�j
	 * @return sindyErrCode
	 */
	errorcode::sindyErrCode RecreateRuleCache( bool bAnnoRule = true, bool bEditRule = true, bool bBGClass = true, long lPurposeID = -1 );

	/**
	 * @brief ���[���L���b�V�����N���A����
	 *
	 * @param isFinal [in,optional] CFieldMap��CTableNameString�܂ŃN���A����ꍇ��true�i�f�t�H���g�Ffalse�j
	 * @note isFinal������C++/CLI�łǂ����Ă��K�v�Ȃ̂ł��܂����B
	 * C++�Ŏg�p���镪�ɂ͍l������K�v�͂���܂���B
	 */
	void ClearRuleCache( bool isFinal = false );

	/**
	 * @brief <b>���L���[���e�[�u����Ԃ�</b>\n <- �Ӗ��Ȃ��̂Ŕp�~
	 * @return CAnnotationClassTable&
	 */
/*	const annotation_class::CAnnotationClassTable& GetAnnotationClassTable() const {
		return m_cAnnotationClassTable;
	}*/
	/**
	 * @brief <b>���L���[�����R�[�h���擾����</b>\n
	 * @param emTableType	[in]	�e�[�u���^�C�v
	 * @param emCode		[in]	���L���
	 * @return �Y�����R�[�h���Ȃ��ꍇ��NULL��Ԃ�
	 */
	const annotation_class::CAnnotationClass* GetAnnoRule( sindyTableType::ECode emTableType, schema::annotation::annotation_code::ECode emCode ) const;

	/**
	 * @brief <b>���L���[���L���b�V���e�[�u�����擾����</b>\n
	 * @retval NULL						�G���[�܂��̓L���b�V�����쐬����Ă��Ȃ�
	 * @retval CAnnotationClassTable*	���L���[���L���b�V���e�[�u��
	 */
	const annotation_class::CAnnotationClassTable* GetAnnotationClassTable() const;

	/**
	 * @brief <b>�w�i���[���L���b�V���e�[�u�����擾����</b>\n
	 * @retval NULL						�G���[�܂��̓L���b�V�����쐬����Ă��Ȃ�
	 * @retval CBackGroundClassTable*	���L���[���L���b�V���e�[�u��
	 */
	const background_class::CBackGroundClassTable* GetBackGroundClassTable() const;

	/**
	 * @brief <b>�ҏW�\���b�V���e�[�u�����擾����</b>\n
	 * @return �ҏW�\���b�V���e�[�u��
	 */
	const editmesh::CEditMeshTable* GetEditMeshTable() const;

	//@{ @name SiNDY-e�ҏW���[���Ɋւ���֐�
	/**
	 * @brief <b>���݂̍�ƖړI�̍�ƖړI���R�[�h���擾����</b>\n
	 * @return �Y�����R�[�h���Ȃ��ꍇ��NULL��Ԃ�
	 */
	const workpurpose::CWorkPurpose* GetWorkPurpose() const;

	/**
	 * @brief <b>���݂̍�ƖړIID���擾����</b>\n
	 * @retval 0�ȏ� ��ƖړIID
	 * @retval -1 ���O�C�����Ă��Ȃ�
	 */
	long GetWorkPurposeID() const;

	/**
	 * @brief <b>���݂�MXD�t�@�C�������擾����</b>\n
	 * ���O�C�����Ă��邱�Ƃ��m�F������Ŏg�p���Ă��������B
	 * @param nLen		[in]	�o�b�t�@�i�[�\�������iworkpurpose::_MXD_MAX_LEN���g�p���邱�Ɓj
	 * @param lpszMXD	[out]	MXD�t�@�C�����i�[�o�b�t�@
	 * @retval NULL	���O�C�����Ă��Ȃ��A���̓G���[
	 * @retval LPCTSTR �擾����
	 */
	LPCTSTR GetMXDFileName( int nLen, LPTSTR lpszMXD ) const;

	/**
	 * @brief <b>���݂̎��������t�^�̒l���擾����</b>\n
	 * ���O�C�����Ă��邱�Ƃ��m�F������Ŏg�p���Ă��������B
	 * @note �Ȃ�ׂ�GetAutoValue���g�p���Ă��������B
	 * @param nLen		[in]	�o�b�t�@�i�[�\�������iworkpurpose::_MXD_MAX_LEN���g�p���邱�Ɓj
	 * @param lpszMXD	[out]	���݂̎��������t�^�̒l
	 * @retval NULL	���O�C�����Ă��Ȃ��A���̓G���[
	 * @retval LPCTSTR �擾����
	 */
	LPCTSTR GetAutoAttrGiving( int nLen, LPTSTR lpszAutoAttrGiving ) const;
	/**
	 * @brief <b>���݂̎��������t�^�̒l���擾����</b>\n
	 * ���O�C�����Ă��邱�Ƃ��m�F������Ŏg�p���Ă��������B
	 * GetAutoAttrGiving�Ɩ����͓����ł����A��͂������̂�Ԃ��܂��B
	 * @note SetLoginInfo���Ă΂ꂽ�i�K�ŏ���������܂��B
	 * @return CAutoValue
	 */
	const CAutoValue& GetAutoValue() const { return m_cAutoValue; }

	/**
	 * @brief <b>��ƖړI�e�[�u���̎Q�Ƃ��擾����</b>\n
	 * @warning �L���b�V���쐬��ɌĂ�ł��������B
	 * @return ��ƖړI�e�[�u���̎Q��
	 */
	const workpurpose::CWorkPurposeTable& GetWorkPurposeTable() const;

	/**
	 * @brief <b>���C�����e�[�u���̎Q�Ƃ��擾����</b>\n
	 * @warning �L���b�V���쐬��ɌĂ�ł��������B
	 * @return ���C�����e�[�u���̎Q��
	 */
	const layernames::CLayerNamesTable& GetLayerNamesTable() const;

	/**
	 * @brief <b>�쐬�\���C���L���b�V���e�[�u�����擾����</b>\n
	 * @retval NULL						�G���[�܂��̓L���b�V�����쐬����Ă��Ȃ�
	 * @retval CCreatableLayersTable&	�쐬�\���C���L���b�V���e�[�u��
	 */
	const creatablelayers::CCreatableLayersTable& GetCreatableLayersTable() const;

	/**
	 * @brief <b>�ҏW�\���ǂ������`�F�b�N����</b>\n
	 * @param cRow			[in]			�`�F�b�N�ΏۃI�u�W�F�N�g
	 * @param emEditType	[in]			�ҏW�^�C�v
	 * @param nLen			[in]			�o�b�t�@�i�[�\������
	 * @param lpszMessage	[out]			�G���[���b�Z�[�W�i�[�p�o�b�t�@
	 * @param check_modify	[in,optional]	ModifyCheck�����邩�ǂ����i�f�t�H���g�Ftrue�j
	 * @return sindyeEditableType
	 */
	sindyeEditableType IsEditable( const CRow& cRow, sindyeEditType emEditType, int nLen, LPTSTR lpszMessage, bool check_modify = true ) const;

	/**
	 * @brief <b>�ҏW�ς݂��ǂ������`�F�b�N����</b>\n
	 * ������ModifyCheck.dll���g�p���܂��B
	 * �J�X�^���|�X�g���[�h�̎��ɂ͓��삵�܂���B
	 * @param cRow			[in]	�`�F�b�N�ΏۃI�u�W�F�N�g
	 * @param nLen			[in]	�o�b�t�@�i�[�\������
	 * @param lpszMessage	[out]	�G���[���b�Z�[�W�i�[�p�o�b�t�@
	 * @retval true �ҏW�ς�
	 * @retval false �ҏW����Ă��Ȃ�
	 */
	bool IsModified( const CRow& cRow, int nLen, LPTSTR lpszMessage ) const;

	/**
	 * @brief <b>�ړ��\���ǂ������`�F�b�N����</b>\n
	 * @param cRow			[in]	�`�F�b�N�ΏۃI�u�W�F�N�g
	 * @param nLen			[in]	lpszMessage���[�\������
	 * @param lpszMessage	[out]	�`�F�b�N���ʃ��b�Z�[�W
	 * @return sindyeEditableType
	 */
	sindyeEditableType IsMovable( const CRow& cRow, int nLen, LPTSTR lpszMessage ) const;

	/**
	 * @brief <b>�폜�\���ǂ������`�F�b�N����</b>\n
	 * @param cRow			[in]	�`�F�b�N�ΏۃI�u�W�F�N�g
	 * @param nLen			[in]	lpszMessage���[�\������
	 * @param lpszMessage	[out]	�`�F�b�N���ʃ��b�Z�[�W
	 * @return sindyeEditableType
	 */
	sindyeEditableType IsDeletable( const CRow& cRow, int nLen, LPTSTR lpszMessage ) const;

	/**
	 * @brief <b>�쐬�\���ǂ������`�F�b�N����</b>\n
	 * �����ł̓e�[�u�����݂̂Ŕ��f����̂ŁAcRow�͋�̂܂ܓn�����A
	 * ���͓������C���̃I�u�W�F�N�g��n���Ă��������B
	 * @param cRow			[in]	�`�F�b�N�ΏۃI�u�W�F�N�g
	 * @param nLen			[in]	lpszMessage���[�\������
	 * @param lpszMessage	[out]	�`�F�b�N���ʃ��b�Z�[�W
	 * @return sindyeEditableType
	 */
	sindyeEditableType IsCreatable( const CRow& cRow, int nLen, LPTSTR lpszMessage ) const;

	/**
	 * @brief <b>�����ҏW�\���ǂ������`�F�b�N����</b>\n
	 * @param cRow			[in]	�`�F�b�N�ΏۃI�u�W�F�N�g
	 * @param nLen			[in]	lpszMessage���[�\������
	 * @param lpszMessage	[out]	�`�F�b�N���ʃ��b�Z�[�W
	 * @return sindyeEditableType
	 */
	sindyeEditableType IsAttributeEditable( const CRow& cRow, int nLen, LPTSTR lpszMessage ) const;

	/**
	 * @brief <b>�ҏW�\�̈悩�ǂ������`�F�b�N����</b>\n
	 * @param lpcszName		[in]	�`�F�b�N�Ώۃ��C����
	 * @param ipGeom		[in]	�`�F�b�N�ΏۃG���A
	 * @return sindyErrCode
	 */
	errorcode::sindyErrCode IsEditableArea( LPCTSTR lpcszName, IGeometry* ipGeom ) const;

	/**
	 * @brief <b>���b�N����Ă��郁�b�V�����ǂ������`�F�b�N����</b>\n
	 * @param lpcszConnUser	[in]	�`�F�b�N�Ώېڑ����[�U��
	 * @param lpcszName		[in]	�`�F�b�N�Ώۃ��C����
	 * @param lCode			[in]	�`�F�b�N�Ώۃ��b�V���R�[�h
	 * @return sindyErrCode
	 */
	errorcode::sindyErrCode IsLockMesh( LPCTSTR lpcszConnUser, LPCTSTR lpcszName, long lCode ) const;

	/**
	 * @brief <b>���b�N����Ă���G���A���ǂ������`�F�b�N����</b>\n
	 * @param lpcszConnUser	[in]	�`�F�b�N�Ώېڑ����[�U��
	 * @param lpcszName		[in]	�`�F�b�N�Ώۃ��C����
	 * @param ipGeom		[in]	�`�F�b�N�ΏۃG���A
	 * @return sindyErrCode
	 */
	errorcode::sindyErrCode IsLockArea( LPCTSTR lpcszConnUser, LPCTSTR lpcszName, IGeometry* ipGeom ) const;

	/**
	 * @brief <b>���O�C���ς݂ł��邩�ǂ������`�F�b�N����</b>\n
	 * ���O�C�������擾����O�ɁA�K�����̃��\�b�h�Ń��O�C���ς݂��ǂ���
	 * ���`�F�b�N���Ă��������B
	 * @retval true ���O�C���ς�
	 */
	bool IsLogined() const;

	/**
	 * @brief <b>���O�C������</b>\n
	 * @retval sindyErr_LoginCanceled ���O�C�����L�����Z�����ꂽ
	 * @retval sindyErr_LoginErr ���O�C���Ɏ��s����
	 * @retval sindyErr_NoErr ���O�C������
	 * @retval ��L�ȊO �G���[
	 */
	errorcode::sindyErrCode Login();

	/**
	 * @brief <b>���O�C��������������</b>\n
	 * ���O�C�����y�ю����t�^������ύX���܂��B���O�C�����ȊO�Ɏg�p����ƁA�ҏW���[���̐��������Ȃ��Ȃ�܂��B
	 */
	void SetLoginInfo( long lPurposeID, LPCTSTR lpcszUserName );

	/**
	 * @brief <b>�ۑ��\�ȍ�ƖړI���ǂ������`�F�b�N����</b>\n
	 * ���O�C�����Ă��Ȃ��ꍇ�A���͍�ƖړI���R�[�h��������Ȃ��ꍇ��
	 * �ۑ��\�Ɣ��f���܂��B
	 * @retval true �ۑ��\�i�f�t�H���g�j
	 * @retval false �ۑ��s�\
	 */
	bool CanSaveEdit() const;

	/**
	 * @brief <b>��ƖړIID�����ƖړI�����擾����</b>\n
	 * @param lID		[in]	��ƖړIID
	 * @param nLen		[in]	lpszName�̎��[�\������
	 * @param lpszName	[out]	�擾��������i�[����o�b�t�@
	 * @retval LPCTSTR ��ƖړI��
	 * @retval NULL �Ή������ƖړI�������݂��Ȃ��A���̓��O�C�����Ă��Ȃ�
	 */
	LPCTSTR GetOperatingPurposeFromID( long lID, int nLen, LPTSTR lpszName ) const;

	/**
	 * @brief <b>���݂̍�ƎҖ����擾����</b>\n
	 * @retval LPCTSTR ��ƎҖ�
	 * @retval NULL ���O�C�����Ă��Ȃ�
	 */
	LPCTSTR GetOperatorName() const;

	/**
	 * @brief <b>�ҏW�\���C���̈ꗗ���擾����</b>\n
	 * @param listLayers	[out]			�ړ��\���C�����X�g
	 * @return sindyErrCode
	 */
	errorcode::sindyErrCode GetMovableLayers( CListString& listLayers ) const;

	/**
	 * @brief <b>�폜�\���C���̈ꗗ���擾����</b>\n
	 * @param listLayers	[out]			�폜�\���C�����X�g
	 * @return sindyErrCode
	 */
	errorcode::sindyErrCode GetDeletableLayers( CListString& listLayers ) const;

	/**
	 * @brief <b>�쐬�\���C���̈ꗗ���擾����</b>\n
	 * @param listLayers	[out]			�쐬�\���C�����X�g
	 * @return sindyErrCode
	 */
	errorcode::sindyErrCode GetCreatableLayers( CListString& listLayers ) const;

	/**
	 * @brief <b>�����ҏW�\���C���̈ꗗ���擾����</b>\n
	 * @param lpszLayers	[out]			�����ҏW�\���C�����X�g
	 * @return sindyErrCode
	 */
	errorcode::sindyErrCode GetEditAttrLayers( CListString& listLayers ) const;

	/**
	 * @brief <b>�쐬�\���C�����ǂ����`�F�b�N����</b>\n
	 * @param lpctstrLayer	[in]	�`�F�b�N�Ώۂ̃��C����
	 * @retval true �쐬�\
	 * @retval false �쐬�s�\
	 */
	bool IsCreatableLayer( LPCTSTR lpcszLayer ) const;

	/**
	 * @brief <b>�쐬�\���C�����ǂ����`�F�b�N����</b>\n
	 * @warning ���̊֐��ł́A�����ɂ�鐧���̓`�F�b�N�ΏۊO�ł��B
	 * �ʏ��IsDeletable���g�p���Ă��������B
	 * @param lpctstrLayer	[in]	�`�F�b�N�Ώۂ̃��C����
	 * @retval true �폜�\
	 * @retval false �폜�s�\
	 */
	bool IsDeletableLayer( LPCTSTR lpcszLayer ) const;

	/**
	 * @brief <b>�ړ��\���C�����ǂ����`�F�b�N����</b>\n
	 * @warning ���̊֐��ł́A�����ɂ�鐧���̓`�F�b�N�ΏۊO�ł��B
	 * �ʏ��IsMovable���g�p���Ă��������B
	 * @param lpctstrLayer	[in]	�`�F�b�N�Ώۂ̃��C����
	 * @retval true �ړ��\
	 * @retval false �ړ��s�\
	 */
	bool IsMovableLayer( LPCTSTR lpcszLayer ) const;

	/**
	 * @brief <b>�����ҏW�\���C�����ǂ����`�F�b�N����</b>\n
	 * @warning ���̊֐��ł́A�����ɂ�鐧���̓`�F�b�N�ΏۊO�ł��B
	 * �ʏ��IsAttributeEditable���g�p���Ă��������B
	 * @param lpctstrLayer	[in]	�`�F�b�N�Ώۂ̃��C����
	 * @retval true �����ҏW�\
	 * @retval false �����ҏW�s�\
	 */
	bool IsEditAttrLayer( LPCTSTR lpcszLayer ) const;

	/**
	 * @brief <b>�n���ꂽ���C�����ɑΉ����鋤�L�ړ��Ώۃ��C���������X�g�ŕԂ�</b>\n
	 * @param lpcszLayerName	[in]	���C����
	 * @param listNames			[out]	���L�ړ��Ώۃ��C�������X�g
	 * @return ���L�ړ����C����
	 */
	long GetSharedMovableLayer( LPCTSTR lpcszLayerName, CListString& listNames ) const;

	/**
	 * @brief <b>�n���ꂽ���C�����ɑΉ����鋤�L�폜�Ώۃ��C���������X�g�ŕԂ�</b>\n
	 * @param lpcszLayerName	[in]	���C����
	 * @param listNames			[out]	���L�폜�Ώۃ��C�������X�g
	 * @return ���L�폜���C����
	 */
	long GetSharedDeletableLayer( LPCTSTR lpcszLayerName, CListString& listNames ) const;

	/**
	 * @brief <b>���H�����N�̓��H��ʂƌo�H��ʊԂ̃��[�����擾����</b>\n
	 * @param lRoadClassID	[in]	���H���
	 * @param lNaviClassID	[in]	�o�H���
	 * @return sindyeDispNaviRelation
	 */
	sindyeDispNaviRelation GetDispNaviRule(  long lRoadClassID, long lNaviClassID  ) const;

	/**
	 * @brief <b>���H�����N�̓��H��ʂƂ��̑��̑����Ԃ̃��[�����擾����</b>\n
	 * @param lRoadClassID		[in]	���H���
	 * @param lpcszFieldName	[in]	���̑��̑�����
	 * @return sindyeDispOtherRelation
	 */
	sindyeDispOtherRelation GetDispOtherRule( long lRoadClassID, LPCTSTR lpcszFieldName ) const;
	//@}
	//@{ @name ���W�X�g���E�ݒ�t�@�C��
	/**
	 * @brief <b>�ҏW臒l���擾����</b>\n
	 * @return �ҏW臒l�i�s�N�Z���P�ʁj
	 */
	int GetSelectTolerance() const;

	/**
	 * @brief <b>�ҏW臒l���Z�b�g����</b>\n
	 * @param nPixel [in] �ҏW臒l�i�s�N�Z���P�ʁj
	 * @retval true ����
	 * @retval false ���s
	 */
	bool SetSelectTolerance( int nPixel );

	/// �g���[�X���O�t�@�C�������擾����
	LPCTSTR GetTraceLogFileName( int nLen, LPTSTR strBuf ) const;

	/// �g���[�X���O�t�@�C������ݒ肷��
	bool SetTraceLogFileName( LPCTSTR lpcszFileName );

	/// �G���[���O�t�@�C�������擾����
	LPCTSTR GetErrorLogFileName( int nLen, LPTSTR strBuf ) const;

	/// �G���[���O�t�@�C������ݒ肷��
	bool SetErrorLogFileName( LPCTSTR lpcszFileName );

	/// �ĕ`��o�b�t�@�l���擾����
	int GetRefreshBuffer() const;

	/// �ĕ`��o�b�t�@�l��ݒ肷��
	bool SetRefreshBuffer( int nPixel );
	//@}
	/**
	 * @brief <b>�������`�F�b�N�p���[�����쐬����</b>\n
	 * �I�u�W�F�N�g���ƂɓK�؂Ȑ������`�F�b�N�p���[�����쐬���܂��B
	 * @param cRow		[in]	�`�F�b�N�ΏۃI�u�W�F�N�g
	 * @param cRule		[out]	�������`�F�b�N�p���[��
	 * @return cRule�̎Q��
	 */
//	CLogicRule& CreateLogicRule( const CRow& cRow, CLogicRule& cRule ) const;
private:
	/**
	 * @brief <b>���C����������쐬�p�⏕�֐�</b>\n
	 * ���̊֐��͕⏕�֐��ł��B
	 * GetSharedMovableLayer�y��GetSharedDeletablelayer����
	 * �Ă΂�܂��B
	 * @param listLayerNameIDs	[in]	���C����ID���i�[����Ă��郊�X�g
	 * @param listLayerNames	[out]	���C�����i�[���X�g
	 */
	void CRule::LayerNameID2LayerName( const CListLong& listLayerNameIDs, CListString& listLayerNames ) const;
private:
	HANDLE											m_hMutex;					//!< ���[���ǂݍ��ݎ��̔r�������p
	// ���L�n
	annotation_class::CAnnotationClassTable			m_cAnnotationClassTable;	//!< ���L���[���e�[�u��
	// �w�i�n
	background_class::CBackGroundClassTable			m_cBackGroundClassTable;	//!< �w�i���[���e�[�u��
	// �ҏW���[���n
	workpurpose::CWorkPurposeTable					m_cWorkPurposeTable;		//!< ��ƖړI�Ǘ��e�[�u��
	soperator::COperatorTable						m_cOperatorTable;			//!< ��ƎҖ��Ǘ��e�[�u��
	connectuser::CConnectUserTable					m_cConnectUserTable;		//!< �ڑ����[�U���Ǘ��e�[�u��
	deletablelayers::CDeletableLayersTable			m_cDeletableLayersTable;	//!< �폜�\���C���Ǘ��e�[�u��
	creatablelayers::CCreatableLayersTable			m_cCreatableLayersTable;	//!< �쐬�\���C���Ǘ��e�[�u��
	movablelayers::CMovableLayersTable				m_cMovableLayersTable;		//!< �ړ��\���C���Ǘ��e�[�u��
	editattrlayers::CEditAttrLayersTable			m_cEditAttrLayersTable;		//!< �����ҏW�\���C���Ǘ��e�[�u��
	roadattribute::CRoadAttributeTable				m_cRoadAttributeTable;		//!< ���H�������̊Ǘ��e�[�u��
	roaddispclass::CRoadDispClassTable				m_cRoadDispClassTable;		//!< ���H�����N�̓��H��ʊǗ��e�[�u��
	roadnaviclass::CRoadNaviClassTable				m_cRoadNaviClassTable;		//!< ���H�����N�̌o�H��ʊǗ��e�[�u��
	shareddeletelayers::CSharedDeleteLayersTable	m_cSharedDeleteLayersTable;	//!< ���L�폜���C�����Ǘ��e�[�u��
	sharedmovelayers::CSharedMoveLayersTable		m_cSharedMoveLayersTable;	//!< ���L�ړ����C�����Ǘ��e�[�u��
	dispnavirelation::CDispNaviRelationTable		m_cDispNaviRelationTable;	//!< ���H�����N�̓��H��ʂƌo�H��ʂ̊֘A�Ǘ��e�[�u��
	dispotherrelation::CDispOtherRelationTable		m_cDispOtherRelationTable;	//!< ���H�����N�̓��H��ʂƂ��̑��̑����̊֘A�Ǘ��e�[�u��
	edithistory::CEditHistoryTable					m_cEditHistoryTable;		//!< ���[���ҏW�����Ǘ��e�[�u��
	editmesh::CEditMeshTable						m_cEditMeshTable;			//!< �ҏW�\���b�V���Ǘ��e�[�u��
	lockarea::CLockAreaTable						m_cLockAreaTable;			//!< ���b�N�G���A�Ǘ��e�[�u��
	lockmesh::CLockMeshTable						m_cLockMeshTable;			//!< ���b�N���b�V���Ǘ��e�[�u��
	layernames::CLayerNamesTable					m_cLayerNamesTable;			//!< ���C�����Ǘ��e�[�u��
	// �����t�^����
	CAutoValue										m_cAutoValue;				//!< �����t�^����
	// ���O�C�����
	long											m_lWorkPurposeID;			//!< ��ƖړIID
	CString											m_strOperatorName;			//!< ��ƎҖ�
//	bool											m_bCanSaveEdit;				//!< ���݂̍�ƖړI���ҏW�\���ǂ����̃t���O�ێ��p
	// ���W�X�g���E�ݒ�t�@�C��
	CSiNDYeSettings									m_cSiNDYeSettings;			//!< ���W�X�g���E�ݒ�t�@�C���Ǘ��N���X

	bool											m_bAnnoRuleCached;			//!< ���L���[�����L���b�V���������ǂ����̃t���O
	bool											m_bEditRuleCached;			//!< �ҏW���[�����L���b�V���������ǂ����̃t���O
	bool											m_bBGClassCached;			//!< �w�i��ʃ��[�����L���b�V�������ǂ����̃t���O
};
#pragma warning(pop)
} // sindy

#endif // _RULE_H_
