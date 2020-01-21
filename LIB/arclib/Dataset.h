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

// Dataset.h: CDataset �N���X�̃C���^�[�t�F�C�X
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_DATASET_H__DED66EC7_AB85_47C6_973B_BA54EFD4AEEF__INCLUDED_)
#define AFX_DATASET_H__DED66EC7_AB85_47C6_973B_BA54EFD4AEEF__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#pragma warning(disable : 4786)

#include <Workspaces.h>
#include <vector>
#include <set>

namespace sindy
{

/**
 *	@class	CSelectedDataset
 *	@brief	�I�����ꂽ�f�[�^�̊i�[�p�N���X
 */
class CSelectedDataset
{
	typedef	std::vector<CAdapt<_IRowPtr> >	ROWARRAY;
	typedef	std::map<CString, ROWARRAY>		ROWARRAYMAP;

	ROWARRAYMAP				m_RowArrayMap;

public:
	CSelectedDataset();
	virtual ~CSelectedDataset();

	/**
	 *	@brief	�f�[�^���i�[����
	 *	@param	lpcszLayerName		[in]	���C����
	 *	@param	ipRow				[in]	�f�[�^
	 */
	void put_Data(LPCTSTR lpcszLayerName, _IRowPtr ipRow);

	/**
	 *	@brief	�f�[�^�Z�b�g�����擾����
	 */
	long get_DatasetCount() { return (long)m_RowArrayMap.size(); }

	/**
	 *	@brief	�f�[�^�Z�b�g���̎擾
	 *	@param	cNames				[out]	���C����
	 */
	void get_DatasetName(std::vector<CString> *cNames);

	/**
	 *	@brief	���C���̃f�[�^�����J�E���g����
	 *	@param	lpcszLayerName		[in]	���C����
	 *	@return	�f�[�^��
	 */
	long get_RowCount(LPCTSTR lpcszLayerName);

	/**
	 *	@brief	�f�[�^���擾����
	 *	@param	lpcszLayerName		[in]	���C����
	 *	@param	nIndex				[in]	�C���f�b�N�X�ԍ�
	 *	@return	�f�[�^
	 */
	_IRowPtr get_Row(LPCTSTR lpcszLayerName, long nIndex);
};

/**
 *	@class	���[�N�X�y�[�X�A�e�[�u���̊Ǘ��N���X
 *	@brief	CWorkspaces�ƃe�[�u�����Ǘ����܂�
 */
class CDataset : public CWorkspaces  
{
	std::map<CString, ITablePtr> m_mapTable;
public:
	CDataset();
	virtual ~CDataset();

	/**
	 *	@brief	<b>�e�[�u���̃I�[�v��</b>\n
	 *	�K�v�ȃ��[�N�X�y�[�X���I�[�v������Ă��Ȃ���΁A���[�N�X�y�[�X���I�[�v������
	 *	@param	lpcszTableName		[in]	�e�[�u������
	 *	@param	lpcszConnectString	[in]	�ڑ��p�����[�^
	 *	@return	ITablePtr
	 */
	ITablePtr OpenTable(LPCTSTR lpcszTableName, LPCTSTR lpcszConnectString);

	/**
	 *	@brief	<b>�e�[�u���̃I�[�v��</b>\n
	 *	�K�v�ȃ��[�N�X�y�[�X���I�[�v������Ă��Ȃ���΁A���[�N�X�y�[�X���I�[�v������
	 *	@param	lpcszTableName		[in]	�e�[�u������
	 *	@param	lpcszUserName		[in]	���[�U��
	 *	@param	lpcszPassword		[in]	�p�X���[�h
	 *	@param	lpcszVersion		[in]	�o�[�W����
	 *	@param	lpcszInstance		[in]	�C���X�^���X
	 *	@param	lpcszServerName		[in]	�T�[�o��
	 *	@return	ITablePtr
	 */
	ITablePtr OpenTable(LPCTSTR lpcszTableName, LPCTSTR lpcszUserName, LPCTSTR lpcszPassword, LPCTSTR lpcszVersion, LPCTSTR lpcszInstance, LPCTSTR lpcszServerName);

	/**
	 *	@brief	���ɃI�[�v���ς݂̃e�[�u������������
	 *	@param	lpcszTableName		[in]	�e�[�u����
	 *	@return	ITablePtr �I�[�v���ς݂łȂ���΃��^�[���O
	 */
	ITablePtr FindTable(LPCTSTR lpcszTableName);

	/**
	 *	@brief	�N�G���ɂ��f�[�^�̎擾
	 *	@param	lpcszTableName		[in]	�e�[�u����
	 *	@param	ipQueryFilter		[in]	�t�B���^
	 *	@param	cDataset			[out]	�擾���ꂽ�f�[�^
	 *	@return	����������true
	 */
	bool Select(LPCTSTR lpcszTableName, IQueryFilterPtr ipQueryFilter, CSelectedDataset *cDataset);

	/**
	 *	@brief	�������ɂ��f�[�^�̎擾
	 *	@param	lpcszTableName		[in]	�e�[�u����
	 *	@param	lpcszWhereClause	[in]	������
	 *	@param	cDataset			[out]	�擾���ꂽ�f�[�^
	 *	@return	����������true
	 */
	bool SelectByWhereClause(LPCTSTR lpcszTableName, LPCTSTR lpcszWhereClause, CSelectedDataset *cDataset);

	/**
	 *	@brief	�w�肳�ꂽID�̃f�[�^���擾����
	 *	@param	lpcszTableName		[in]	�e�[�u����
	 *	@param	lpcszFieldName		[in]	ID���X�g�Ɠ������Ȃ�Ȃ���΂Ȃ�Ȃ��t�B�[���h��
	 *	@param	idSet				[in]	���j�[�N��ID���X�g
	 *	@param	cDataset			[out]	�擾���ꂽ�f�[�^
	 *	@return	����������true
	 */
	bool SelectByIDs(LPCTSTR lpcszTableName, LPCTSTR lpcszFieldName, std::set<long> &idSet, CSelectedDataset *cDataset);

};

}	// sindy

#endif // !defined(AFX_DATASET_H__DED66EC7_AB85_47C6_973B_BA54EFD4AEEF__INCLUDED_)
