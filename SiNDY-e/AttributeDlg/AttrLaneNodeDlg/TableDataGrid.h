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

#include <memory>
#include <WinLib/grid_util.h>
#include "GridField.h"

// �O���b�h�̐F
namespace grid_color {
	const COLORREF selectedString = RGB(255, 255, 255);		// �I���s�̕����F
	const COLORREF selectedBack = RGB(51, 153, 255);		// �I���s�̔w�i�F
	const COLORREF notSelectedString = RGB(0, 0, 0);		// ��I���s�̕����F
	const COLORREF notSelectedBack = RGB(255, 255, 255);	// ��I���s�̔w�i�F
}

/**
 *	@brief   �e�[�u������\������O���b�h�N���X
 */
class TableDataGrid
{
public:
	class Builder;

public:
	TableDataGrid() {}
	TableDataGrid(ITablePtr table, GridField grid_field)
		: m_table(table), m_gridField(grid_field) {};
	virtual ~TableDataGrid() {};

public:
	// �C�x���g�n���h���̏������Ϗ����邽�߂̊֐�
	bool onGridMouseDown(SHORT Button, SHORT Shift, FLOAT X, FLOAT Y, VARIANT_BOOL * Cancel);
	void onBeforeEdit(LONG Row, LONG Col, VARIANT_BOOL * Cancel);
	bool onAfterEdit(LONG Row, LONG Col);

public:
	/**
	*  @brief �O���b�h�̗�̏�����
	*/
	void init();

	/**
	*  @brief �O���b�h�ŕҏW�\�ȃt�B�[���h��ݒ肷��
	*  @param[in] field_names  �ҏW�\�Ƃ���t�B�[���h�����X�g
	*/
	void enableEditing(const std::set<CString>& field_names) { m_editableFields = field_names; }

	/**
	*  @brief �f�[�^��ǂݍ���ŃO���b�h�ɕ\������
	*  @param[in] where_clause ����������
	*/
	void select(const CString& where_clause);

	/**
	*  @brief �O���b�h�̕ێ�����f�[�^�ɕύX�������Ă��邩
	*/
	bool isChanged() const;

	/**
	*  @brief �f�[�^�̍X�V
	*/
	bool store();

	/**
	*  @brief �I�����Ă���O���b�h�s�ɑΉ����郌�R�[�h��Ԃ�
	*  @return ���R�[�h
	*/
	_IRowPtr getSelectedRow() const;

	/**
	*  @brief �ύX�̂������Z�����n�C���C�g����
	*/
	void highlightChangedCell() const;

private:
	/**
	 *  @brief �e�[�u������f�[�^��ǂݍ��݃R���e�i�Ɋi�[����
	 *  @param[in]  table         �ǂݍ��ރe�[�u��
	 *  @param[in]  where_clause  �f�[�^�ǂݍ��ݎ���Where��
	 *  @param[out] data          �ǂݍ��񂾃f�[�^�̊i�[��
	 *  @retval  true  �ǂݍ��ݐ���
	 *  @retval  false �ǂݍ��ݎ��s
	 */
	bool readData(const ITablePtr & table, const CString& where_clause, std::map<long, _IRowPtr>& row_map);

	/**
	 *  @brief �R�[�h�l�h���C�������t�B�[���h�̃O���b�h����R���{�{�b�N�X�ɂ���
	 *  @param[in]  table      �e�[�u��
	 *  @param[in]  grid       �R���{�{�b�N�X�ɂ���Ώۂ̃O���b�h
	 *  @param[in]  grid_field �R���{�{�b�N�X�ɂ���O���b�h�̃t�B�[���h���
	 */
	void initColSettings(const ITablePtr& table, const IVSFlexGridPtr& grid, const GridField& grid_field);

	/**
	 *  @brief �ǂݍ��񂾊֘A�e�[�u���̏����O���b�h�֊i�[����
	 *  @param[in]  grid       �i�[�Ώۂ̃O���b�h
	 *  @param[in]  grid_field �l���i�[����O���b�h�̃t�B�[���h���
	 *  @param[in]  row_map    �ǂݍ��񂾊֘A�e�[�u���̏�� (key: OID, value: �֘A�e�[�u���̃��R�[�h)
	 */
	void setDataToGrid(const IVSFlexGridPtr& grid, const GridField& grid_field, const std::map<long, _IRowPtr>& row_map);
	
	/**
	*  @brief  �e�t�B�[���h�̃w�b�_�[��"|"��؂�ŕԂ�
	*  @detail (e.g. OBJECTID|���H�\�����|�n�C�E�F�C�H���R�[�h)
	*  @return �w�b�_�[��"|"��؂�
	*/
	CString getHeader() const;

	/**
	*  @brief  �O���b�h�̎w��s�ɑΉ�����e�[�u���̃��R�[�h���擾����
	*  @param[in] grid_row  �O���b�h�̍s�ԍ�
	*  @return  �e�[�u����1���R�[�h(IRow)
	*/
	_IRowPtr getRow(long grid_row) const;

	/**
	*  @brief  �w��t�B�[���h�̒l���ύX����Ă��邩��Ԃ�
	*  @param[in] row         �ύX����Ă��邩�̊m�F�Ώۂ̃��R�[�h
	*  @param[in] field_name  �ύX����Ă��邩���m�F����t�B�[���h�̃C���f�b�N�X
	*  @retval true  �ύX����Ă���
	*  @retval false �ύX����Ă��Ȃ�
	*/
	bool isValueChanged(const _IRowPtr row, long field_index) const;

protected:
	IVSFlexGridPtr   m_grid;

	ITablePtr m_table;			//!< �֘A�e�[�u��

	// �O���b�h�ŕێ����Ă���f�[�^�Q (key: OID, value: ���R�[�h)
	std::map<long, _IRowPtr> m_data;	

	GridField m_gridField;					//!< �t�B�[���h���ƃO���b�h��̑Ή����
	std::set<CString> m_editableFields;		//!< �O���b�h�ŕҏW�\�ȃt�B�[���h�����X�g
};


/**
 *	@brief   TableDataGrid �I�u�W�F�N�g�̍쐬���s���N���X
 */
class TableDataGrid::Builder
{
public:
	Builder() {}

public:
	Builder& setGridHwnd(HWND hwnd) { m_hwnd = hwnd; return *this; }
	Builder& setTable(ITablePtr table) { m_table = table; return *this; }
	Builder& addField(const CString& fieldName) {
		m_gridField.add(fieldName, m_gridField.size());
		return *this;
	}

	virtual TableDataGrid build() const
	{
		TableDataGrid grid(m_table, m_gridField);
		CAxWindow wndGrid = m_hwnd;
		wndGrid.QueryControl(&(grid.m_grid));

		return grid;
	}

protected:
	HWND m_hwnd;				//!< �O���b�h�R���g���[���̃n���h��
	ITablePtr m_table;			//!< �e�[�u��
	GridField m_gridField;		//!< �O���b�h�̃t�B�[���h���
};
