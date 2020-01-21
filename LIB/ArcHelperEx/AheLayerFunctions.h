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
 * @file LayerFunctions.h
 * @brief <b>ArcObjects�O���[�o���w���p�[�֐���`�t�@�C��</b>\n
 * ���̃t�@�C���ɂ́A��Ƀ��C���n�̃O���[�o���֐�����`����Ă��܂��B
 * @author �n�}�c�a���암�J���O���[�v �Ð�M�G
 * $Id$
 */
#ifndef _LAYERFUNCTIONS_H_
#define _LAYERFUNCTIONS_H_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

//#include "exportimport.h"

//namespace sindy {

/**
* @brief ���̔�r�Ώ�
*/
enum FINDTYPE
{
	_FIND_TABLE = 0,	//!< �e�[�u����
	_FIND_ALIAS = 1,	//!< �G�C���A�X��
	_FIND_MXDNAME = 2	//!< MXD�t�@�C����`���C������
};

/**
 * @brief <b>���C������e�[�u�����擾����</b>\n
 * @param ipLayer	[in]	���C��
 * @return �e�[�u���n�̃��C���ł͂Ȃ��ꍇ��NULL
 */
ITablePtr /*SINDYLIB_API*/ AheLayer2Table( ILayer* ipLayer );

/**
 * @brief <b>�ҏW�\�ȃe�[�u�����ǂ������`�F�b�N����</b>\n
 * SDE�̔�o�[�W�����Ή����C���͕ҏW�s�ɂȂ�܂��B
 * ����ȊO�̃e�[�u���́A�ҏW�J�n����Ă���ꍇ�͕ҏW�ɂȂ�܂��B
 * SiNDY���L�̃��[���͍l�����܂���̂Œ��ӂ��ĉ������B
 * @return �ҏW�Ȃ�true
 */
BOOL /*SINDYLIB_API*/ AheIsEditableTable( ITable* ipTable );

/// ���C�����f�[�^�x�[�X�I�ɕҏW�\���ǂ������`�F�b�N����
BOOL AheIsEditableLayer( ILayer* ipLayer );

/**
 * @brief <b>�e�[�u�������������ǂ������`�F�b�N����</b>\n
 * �e�[�u���̃I�[�i�[���ƃe�[�u�����݂̂ł̔�r�ɂȂ�܂��B
 * @return �����Ȃ�true
 */
bool /*SINDYLIB_API*/ AheIsEqualTable( ITable* ipTable1, ITable* ipTable2 );

/**
 * @brief <b>���C������I�[�i�[��.�e�[�u�������擾����</b>\n
 * ���̊֐���CString��Ԃ����߁ADLL�łł͔���J�ł��B
 * @param ipLayer	[in]	���C��
 * @return �e�[�u���n�̃I�u�W�F�N�g�ł͂Ȃ��ꍇ�͋󕶎����Ԃ�
 */
inline CString AheGetOwnerTableName( ITable* ipTable )
{
	CString strName;
	IDatasetPtr ipDataset( ipTable );
	if( ipDataset != NULL )
	{
		BSTR bstrName = 0;
		ipDataset->get_Name( &bstrName );
		strName = bstrName;
		SysFreeString(bstrName);
	}
	return strName;
}

//////////////////////////////////////////////////////////////////////
// ���C�����E�e�[�u�����擾�Ɋւ���֐�
//////////////////////////////////////////////////////////////////////
/**
 * @brief �ڑ����[�U�����擾����
 * @param ipUnk [in] Row,Feature,Table,FeatureClass,FeatureLayer
 * return CString
 */
CString AheGetConnectUserName( IUnknownPtr ipUnk );

/**
 * Owner.Class�����擾����
 *
 * @param ipUnk			[in]			IFeature�AIFeatureClass�AILayer�C���^�[�t�F�[�X�����I�u�W�F�N�g
 *
 * @return CString
 */
CString AheGetOwnerDotClassName( IUnknownPtr ipUnk );

/**
 * �t�B�[�`���N���X�����擾����
 *
 * @param ipUnk			[in]			IFeature�AIFeatureClass�AILayer�C���^�[�t�F�[�X�����I�u�W�F�N�g
 *
 * @note AheGetLayerTableName() �ł� ***.*** �̌`�ŕԂ邱�Ƃ�����܂����A
 * @note ���̊֐��͕K���t�B�[�`���N���X���̂ݕԂ��܂�
 *
 * @return �t�B�[�`���N���X��
 */
CString AheGetFeatureClassName( IUnknownPtr ipUnk );

/**
 * �t�B�[�`���N���X�̃I�[�i�[�����擾����
 *
 * @param ipUnk			[in]			IFeature�AIFeatureClass�AILayer�C���^�[�t�F�[�X�����I�u�W�F�N�g
 *
 * @return �t�B�[�`���N���X�̃I�[�i�[��
 */
CString AheGetFeatureClassOwnerName( IUnknownPtr ipUnk );

/**
 * ���C���̃e�[�u�������擾����
 *
 * @param ipUnk			[in]			IFeature�AIFeatureClass�AILayer�C���^�[�t�F�[�X�����I�u�W�F�N�g
 * @param bstrLayerName	[out]			���C���̃e�[�u����
 *
 * @retval TURE �e�[�u�������擾�ł���
 * @retval FALSE �e�[�u�������擾�ł��Ȃ�����
 */
BOOL AheGetLayerTableName( IMapPtr piMap, IUnknownPtr ipUnk, BSTR* bstrLayerName );	

/**
 * ���C���̃G�C���A�X�����擾����
 *
 * @param ipUnk				[in]			IFeature�AIFeatureClass�AILayer�I�u�W�F�N�g�̂����̂ǂꂩ
 * @param bstrLayerName		[out]			���C���̃e�[�u����
 *
 * @retval TURE �G�C���A�X�����擾�ł���
 * @retval FALSE �G�C���A�X�����擾�ł��Ȃ�����
 */
BOOL AheGetLayerAliasName( IMapPtr piMap, IUnknownPtr ipUnk, BSTR* bstrLayerName );

/**
 * ���C���̃G�C���A�X�����擾����
 *
 * @param ipUnk				[in]			IFeature�AIFeatureClass�AILayer�I�u�W�F�N�g�̂����̂ǂꂩ
 *
 * @return ���C���G�C���A�X��
 */
CString AheGetLayerAliasName( IUnknownPtr ipUnk );

/**
 * *.mxd�t�@�C���ɕۑ�����Ă��郌�C�������擾����
 *
 * @param ipUnk				[in]			IFeature�AIFeatureClass�AILayer�I�u�W�F�N�g�̂����̂ǂꂩ
 * @param bstrLayerName		[out]			���C���̃e�[�u����
 *
 * @retval TURE ���C�������擾�ł���
 * @retval FALSE ���C�������擾�ł��Ȃ�����
 */
BOOL AheGetLayerMXDName( IMapPtr piMap, IUnknownPtr ipUnk, BSTR* bstrLayerName );
CString AheGetLayerMXDName( IMapPtr piMap, IUnknownPtr ipUnk);

/**
 * @brief IUnknown ���烌�C�������擾
 *
 * @note IFeatureClass���擾�ł��Ȃ��ꍇ��*.mxd�t�@�C���ɕۑ�����Ă��閼�O���g�p
 *
 * @param piMap			[in]			IMap �̃C���X�^���X
 * @param ipUnk			[in]			IUnknown �̃C���X�^���X
 * @param bstrName		[out]			�擾���ꂽ�G�C���A�X��
 * @param type			[in]			�擾�������G�C���A�X�̃^�C�v
 *
 * @retval TURE �K��������W���擾�ł���
 * @retval FALSE �K��������W���擾�ł��Ȃ�����
 */
BOOL AheGetLayerName( IMapPtr piMap, IUnknownPtr ipUnk, BSTR* bstrName, FINDTYPE type = _FIND_TABLE );

/**
 * @brief IUnknown ���烌�C�������擾
 *
 * @note IFeatureClass���擾�ł��Ȃ��ꍇ��*.mxd�t�@�C���ɕۑ�����Ă��閼�O���g�p
 *
 * @param piMap			[in]			IMap �̃C���X�^���X
 * @param ipUnk			[in]			IUnknown �̃C���X�^���X
 * @param bstrName		[out]			�擾���ꂽ�G�C���A�X��
 * @param type			[in]			�擾�������G�C���A�X�̃^�C�v
 *
 * @retval TURE �擾�ł���
 * @retval FALSE �擾�ł��Ȃ�����
 */
BOOL AheGetLayerName( IMapPtr piMap, IUnknownPtr ipUnk, LPCTSTR& lpszName, FINDTYPE type = _FIND_TABLE );

/**
 * @brief IUnknown ���烌�C�������擾
 *
 * @note IFeatureClass���擾�ł��Ȃ��ꍇ��*.mxd�t�@�C���ɕۑ�����Ă��閼�O���g�p
 *
 * @param piMap			[in]			IMap �̃C���X�^���X
 * @param ipUnk			[in]			IUnknown �̃C���X�^���X
 * @param bstrName		[out]			�擾���ꂽ�G�C���A�X��
 * @param type			[in]			�擾�������G�C���A�X�̃^�C�v
 *
 * @retval �擾�������C�����i�擾�ł��Ȃ������ꍇ�͋󕶎���j
 */
CString AheGetLayerName( IMapPtr piMap, IUnknownPtr ipUnk, FINDTYPE type = _FIND_TABLE );
//////////////////////////////////////////////////////////////////////
// ���C���E�e�[�u���E�t�B�[�`���N���X�擾�Ɋւ���֐�
//////////////////////////////////////////////////////////////////////
/**
 * ���C���̃e�[�u�������� ILayer ���擾����itype = 0�j
 *
 * @param ipUnk				[in]			IFeature�AIFeatureClass�AILayer�I�u�W�F�N�g�̂����̂ǂꂩ
 * @param ipFeatureClass	[out]			�擾���ꂽ IFeatureClass �I�u�W�F�N�g
 * @param bEditalbeOnly		[in,optional]	�ҏW�\�Ȃ��݂̂̂�ΏۂƂ���ꍇ��TRUE�i�f�t�H���g�FFALSE�j
 *
 * @retval TURE IFeatureClass ���擾�ł���
 * @retval FALSE IFeatureClass ���擾�ł��Ȃ�����
 */
BOOL AheGetLayerByTableName( IMapPtr piMap, LPCTSTR lpszLayerName, ILayer** ipRetLayer, BOOL bEditableOnly = FALSE );

/**
 * ���C���̃e�[�u�������� ILayer ���擾����itype = 0�j
 *
 * @param ipMap				[in]			IMapPtr
 * @param lpcszLayerName	[in]			�擾���������C���̃e�[�u����
 * @param bEditableOnly		[in,optional]	�ҏW�\�Ȃ��݂̂̂�ΏۂƂ���ꍇ��TRUE�i�f�t�H���g�FFALSE�j
 *
 * @return ILayerPtr
 */
ILayerPtr AheGetLayerByTableName( IMapPtr ipMap, LPCTSTR lpcszLayerName, BOOL bEditableOnly = FALSE );

/**
 * ���C���̃G�C���A�X������ ILayer ���擾����itype = 1�j
 *
 * @param ipUnk				[in]			IFeature�AIFeatureClass�AILayer�I�u�W�F�N�g�̂����̂ǂꂩ
 * @param ipFeatureClass	[out]			�擾���ꂽ IFeatureClass �I�u�W�F�N�g
 *
 * @retval TURE IFeatureClass ���擾�ł���
 * @retval FALSE IFeatureClass ���擾�ł��Ȃ�����
 */
BOOL AheGetLayerByAliasName( IMapPtr piMap, LPCTSTR lpszLayerName, ILayer** ipRetLayer );

/**
 * ���C���̃G�C���A�X������ ILayer ���擾����itype = 1�j
 *
 * @param ipMap				[in]			IMapPtr
 * @param lpcszLayerName	[in]			�擾���������C���̃G�C���A�X��
 *
 * @return ILayerPtr
 */
ILayerPtr AheGetLayerByAliasName( IMapPtr ipMap, LPCTSTR lpszLayerName );

/**
 * *.mxd�t�@�C���ɕۑ�����Ă��郌�C�������� ILayer ���擾����itype = 2�j
 *
 * @param ipUnk				[in]			IFeature�AIFeatureClass�AILayer�I�u�W�F�N�g�̂����̂ǂꂩ
 * @param ipFeatureClass	[out]			�擾���ꂽ IFeatureClass �I�u�W�F�N�g
 *
 * @retval TURE IFeatureClass ���擾�ł���
 * @retval FALSE IFeatureClass ���擾�ł��Ȃ�����
 */
BOOL AheGetLayerByMXDName( IMapPtr piMap, LPCTSTR lpszLayerName, ILayer** ipRetLayer );

/**
 * *.mxd�t�@�C���ɕۑ�����Ă��郌�C�������� ILayer ���擾����itype = 2�j
 *
 * @param ipMap				[in]			IMapPtr
 * @param lpcszLayerName	[in]			�擾���������C����MXD���C����
 *
 * @return ILayerPtr
 */
ILayerPtr AheGetLayerByMXDName( IMapPtr ipMap, LPCTSTR lpszLayerName );

/**
 * ���C�������� ILayer ���擾����
 *
 *  type = 0 : ���C���̃e�[�u�����Ō���
 *  type = 1 : ���C���̃G�C���A�X���Ō���
 *  type = 2 : MXD�ɕۑ�����Ă��郌�C�����Ō���
 *
 * @param lpszLayerName		[in]			�擾���������C���̃��C����
 * @param ipRetLayer		[out]			�擾���ꂽ ILayer �I�u�W�F�N�g
 * @param type				[in,optional]	0 �` 2 �̒l�����B�f�t�H���g�� 0
 * @param bEditableOnly		[in,optional]	�ҏW�\�Ȃ��݂̂̂�ΏۂƂ���ꍇ��TRUE�i�f�t�H���g�FFALSE�j
 * @param bFullName			[in,optional]	�e�[�u�����Ō������AOwner�����܂߂�ꍇ��TRUE�i�f�t�H���g�FFALSE�j
 *
 * @retval TURE ILayer ���擾�ł���
 * @retval FALSE ILayer ���擾�ł��Ȃ�����
 */
BOOL AheGetLayerByName( IMapPtr piMap, LPCTSTR lpszLayerName, ILayer** ipRetLayer, FINDTYPE type = _FIND_TABLE, BOOL bEditableOnly = FALSE, BOOL bFullName = FALSE );

/**
 * ���C�������� ILayer ���擾����
 *
 *  type = 0 : ���C���̃e�[�u�����Ō���
 *  type = 1 : ���C���̃G�C���A�X���Ō���
 *  type = 2 : MXD�ɕۑ�����Ă��郌�C�����Ō���
 *
 * @param lpszLayerName		[in]			�擾���������C���̃��C����
 * @param type				[in]			0 �` 2 �̒l�����B�f�t�H���g�� 0
 * @param bEditableOnly		[in,optional]	�ҏW�\�Ȃ��݂̂̂�ΏۂƂ���ꍇ��TRUE�i�f�t�H���g�FFALSE�j
 * @param bFullName			[in,optional]	�e�[�u�����Ō������AOwner�����܂߂�ꍇ��TRUE�i�f�t�H���g�FFALSE�j
 *
 * @return ILayerPtr
 */
ILayerPtr AheGetLayerByName( IMapPtr ipMap, LPCTSTR lpszLayerName, FINDTYPE type = _FIND_TABLE, BOOL bEditableOnly = FALSE, BOOL bFullName = FALSE );

/**
 * @brief TOC�Ō��ݑI������Ă��郌�C���擾
 *
 * @note ��{�I��TOC�Ō��ݑI�����Ă��郌�C����Ԃ��B
 *		�O���[�v���C���I������Ă���ꍇ�ɂ́A���̂Ȃ��̕ҏW�\����Ă��郌�C�����擾���Ă���
 *		���̃O���[�v���C���̒��ɕ����̕ҏW�\���C��������ꍇ�ɂ́A�I�����Ă���O���[�v���C����Ԃ��܂��B
 *
 * @param piMxDoc		[in]	IMxDocument �̃C���X�^���X
 * @param ipRetLayer	[out]	�擾���ꂽ ILayer �I�u�W�F�N�g
 * @param bIsEditable	[in]�@	�ҏW�J�n����Ă��郌�C�����O���[�v���C���̉��̃��C������E���Ă��邩�H(�f�t�H���g�Ftrue)
 *
 * @retval TURE ILayer ���擾�ł���
 * @retval FALSE ILayer ���擾�ł��Ȃ�����
 */
BOOL AheGetSelectedLayer( IMxDocumentPtr piMxDoc, ILayer** ipRetLayer, bool bIsEditable = true);
 
/**
 * @brief �e�[�u��������ITable���擾����
 *
 * @param piMap				[in]			IMap �̃C���X�^���X
 * @param lpszTableName		[in]			�擾������ ITable ��
 * @param ipRetTable		[out]			�擾���ꂽ ITable �̃C���X�^���X
 * @param bReference		[in,optional]	REFERENCE. ���擾�������ꍇ�� TRUE�i�f�t�H���g�FFALSE�j
 * @param bEditableOnly		[in,optional]	�ҏW�J�n����Ă���e�[�u���̂ݎ擾����ꍇ��TRUE�i�f�t�H���g�FFALSE�j
 *
 * @see AheOpenTableByTableName
 *
 * @retval TURE ITable ���擾�ł���
 * @retval FALSE ITable ���擾�ł��Ȃ�����
 */
BOOL AheGetTableByTableName( IMapPtr piMap, LPCTSTR lpszTableName, ITable** ipTable, BOOL bReference = FALSE, BOOL bEditableOnly = FALSE );

/**
 * @brief �e�[�u��������ITable���擾����
 *
 * @param ipMap				[in]			IMapPtr
 * @param lpszTableName		[in]			�擾�������e�[�u����
 * @param bReference		[in,optional]	REFERENCE. ���擾�������ꍇ�� TRUE�i�f�t�H���g�FFALSE�j
 * @param bEditableOnly		[in,optional]	�ҏW�J�n����Ă���e�[�u���̂ݎ擾����ꍇ��TRUE�i�f�t�H���g�FFALSE�j
 *
 * @see AheOpenTableByTableName
 *
 * @retval NULL �e�[�u���擾���s
 * @retval ITablePtr �e�[�u��
 */
ITablePtr AheGetTableByTableName( IMapPtr ipMap, LPCTSTR lpszTableName, BOOL bReference = FALSE, BOOL bEditableOnly = FALSE  );

/**
 * @brief <b>�f�[�^�t���[���ɓǂݍ��܂�Ă���e�[�u���E�t�B�[�`���N���X���擾����</b>\n
 * nFindCount�̒l��1�ȏ�̏ꍇ�́A��舵���ɒ��ӂ��ĉ������B
 * @param ipMap					[in]			IMapPtr
 * @param lpcszOwnerTableName	[in]			�I�[�i�[.�e�[�u����
 * @param nFindCount			[out]			�������������i�����e�[�u���̏ꍇ�̓J�E���g���Ȃ��j
 * @param bEditableOnly			[in,optional]	�ҏW�\�Ȃ��̂̂ݎ擾�������ꍇ��true
 * @return �e�[�u���E�t�B�[�`���N���X���擾�ł��Ȃ��ꍇ��NULL
 */
ITablePtr /*SINDYLIB_API*/ AheGetTableByTableName2( IMap* ipMap, LPCTSTR lpcszOwnerTableName, int& nFindCount, bool bEditableOnly = true );

/**
 * @brief �e�[�u��������ITable���擾����
 *
 * @param ipWorkspace		[in]			IWorkspacePtr
 * @param lpszTableName		[in]			�擾�������e�[�u����
 * @param bReference		[in,optional]	REFERENCE. ���擾�������ꍇ�� TRUE�i�f�t�H���g�FFALSE�j
 *
 * @retval NULL �e�[�u���擾���s
 * @retval ITablePtr �e�[�u��
 */
ITablePtr AheOpenTableByTableName( IWorkspacePtr ipWorkspace, LPCTSTR lpszTableName, BOOL bReference = FALSE );

/**
 * @brief �����I�[�i�[�̃e�[�u�����I�[�v������
 * 
 * @param ipTable			[in]			�I�[�v���������e�[�u���Ɠ����I�[�i�[�̃e�[�u��
 * @param lpcszTableName	[in]			�I�[�v���������e�[�u����
 *
 * return ITablePtr
 */
ITablePtr AheOpenSameOwnerTable( IWorkspace* ipWork, ITable* ipTable, LPCTSTR lpcszTableName );
ITablePtr AheOpenSameOwnerTable( ITable* ipTable, LPCTSTR lpcszTableName );

/**
 * @brief �Q�Ɨp�e�[�u�����擾����
 *
 * @param ipMap				[in]			IMapPtr
 * @param lpcszTableName	[in]			�擾�������e�[�u����
 *
 * @see AheGetTableByTableName
 *
 * @retval NULL �e�[�u���擾���s
 * @retval ITablePtr �e�[�u��
 */
ITablePtr AheGetReferenceTable( IMapPtr ipMap, LPCTSTR lpcszTableName );

/**
 * �t�B�[�`���N���X���擾����
 *
 * @param ipUnk				[in]			IFeature�AIFeatureClass�AILayer�I�u�W�F�N�g�̂����̂ǂꂩ
 * @param ipFeatureClass	[out]			�擾���ꂽ IFeatureClass �I�u�W�F�N�g
 *
 * @retval TURE IFeatureClass ���擾�ł���
 * @retval FALSE IFeatureClass ���擾�ł��Ȃ�����
 */
BOOL AheGetFeatureClass( IMapPtr piMap, IUnknownPtr ipUnk, IFeatureClass** ipFeatureClass );			//!< �t�B�[�`���N���X���擾

/**
 * �t�B�[�`���N���X���擾����
 * @param ipUnk				[in]	Row,Feature,Table,FeatureClass,FeatureLayer�I�u�W�F�N�g�̂����̂ǂꂩ
 * @retval ITablePtr
 */
ITablePtr AheGetTable( IUnknownPtr ipUnk );

/**
 * �t�B�[�`���N���X���擾����
 * @param ipUnk				[in]	Row,Feature,Table,FeatureClass,FeatureLayer�I�u�W�F�N�g�̂����̂ǂꂩ
 * @retval IFeatureClassPtr
 */
IFeatureClassPtr AheGetFeatureClass( IUnknownPtr ipUnk );
//} // sindy

#endif // _LAYERFUNCTIONS_H_

inline BOOL AheIsEditableLayer( ILayer* ipLayer ) {
	return AheIsEditableTable( AheGetTable( ipLayer ) );
}

inline IFeatureClassPtr AheGetFeatureClass( IUnknownPtr ipUnk ) {
	return AheGetTable( ipUnk );
}
