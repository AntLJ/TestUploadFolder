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


class RelLinkGridController
{
public:
	RelLinkGridController(void) {};
	virtual ~RelLinkGridController(void) {};

	/**
 	 * @brief ������
	 * @param[in]  hwnd  �֘A�����N�O���b�h�̃n���h��
	 */
	void init( HWND hwnd);

	/**
	 * @brief OID�Q���O���b�h�փZ�b�g
	 * @param[in]  oids  OID�Q
	 */
	void setData( const std::vector<long>& oids);

	/**
	 * @brief OID�Q��Ԃ�
	 * @return OID�Q
	 */
	std::vector<long> getData() const;

	/**
	 * @brief �O���b�h��ł̃N���b�N���쎞�̏���
	 */
	void OnMouseUpGrid(SHORT Button, SHORT Shift, FLOAT X, FLOAT Y, HWND hwnd, UINT menuId);

	/**
	 * @brief �s��ǉ�����
	 */
	void addRow();

	/**
	 * @brief �w�肳�ꂽOID�Ɉ�v����s���폜����
	 * @param[in]  oids  �폜�Ώۂ�OID�Q
	 */
	void deleteRow(const std::vector<long>& oids);

	/**
	 * @brief �I�𒆂̍s���폜����
	 */
	void deleteSelectedRow();

	/**
	 * @brief �I�𒆂̃��R�[�h��OID���擾����
	 */
	std::vector<long> getSelectedData() const;


private:
	IVSFlexGridPtr m_relLinkGrid;	//!< �֘A�����N�̃O���b�h�R���g���[��
	HWND m_gridWnd;					//!< �O���b�h�̃E�B���h�E�n���h��
};

