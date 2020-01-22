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

#include <WinLib/grid_util.h>

#include "RelTableData.h"
#include "RelationTableHelper.h"


/// �H�����X�g�O���b�h���`
namespace lineinfo_grid {
	enum grid_cols{
		JoinLineInfoOid,	// JOIN_LINE_INFO_OID
		StationName,		// �w�� 
		RailwayName,		// �H���� 
		StationNumber,		// �w�ԍ� 
		col_num				// �J������
	};
}

/// ���Ӊw���X�g�O���b�h���`
namespace around_sta_grid {
	enum grid_cols{
		Check,				// �`�F�b�N�{�b�N�X
		JoinLineInfoOid,	// JOIN_LINE_INFO_OID
		StationName,		// �w�� 
		RailwayName,		// �H���� 
		col_num				// �J������
	};
}


/**
 * @brief �S���m�[�h�_�C�A���O��2�̃O���b�h�𐧌䂷��N���X
 * @note  �{�_�C�A���O�̃O���b�h����ɓ���
 * @ntoe  �֘A�e�[�u���ւ̐�����{�N���X����s��
 */
class CLineInfoController
{
public:
	// �wPOI�̌����͈�(���[�g��)
	static const int SEARCH_METER = 100;


public:
	CLineInfoController(void): m_bIsChange(false) {};
	~CLineInfoController(void) {};
	
	/**
	* @brief ������
	* @param hwnd1      [in] �H�����X�g�O���b�h�R���g���[���̃n���h��
	* @param hwnd2      [in] ���Ӊw���X�g�O���b�h�R���g���[���̃n���h��
	* @param ipRow      [in] �S���m�[�h�e�[�u���̃��R�[�h
	* @param cArcHelper [in] ArcHelper
	* @retval true   ����������
	* @retval false  ���������s
	*/
	bool init( HWND hwnd1, HWND hwnd2, _IRowPtr& ipRow, CArcHelper cArcHelper);

	/**
	* @brief �H�����X�g�O���b�h�̕\�����X�V����
	*/
	void updateLineInfoGrid();

	/**
	* @brief ���Ӊw���X�g�O���b�h�̕\�����X�V����
	*/
	void updateAroundStaGrid();

	/**
	* @brief ���Ӊw���X�g�O���b�h�̃`�F�b�N�̓��������R�[�h��H�����X�g�֒ǉ� 
	*/
	void addCheckedRecord();
	
	/**
	 * @brief	�w�����������O���b�h�ɕ\�� 
	 */
	void searchStationInfo(IFeaturePtr ipFeature, ILayerPtr ipLayer);

	/**
	 * @brief	�O���b�h�̕ύX�_���֘A�e�[�u���ɔ��f 
	 */
	bool updateTable();

	/**
	 * @brief	�O���b�g�̕ύX�ӏ���F�t������ 
	 */
	void drawColor();

	/**
	 * @brief	�O���b�h��ҏW�s�ɂ��� 
	 */
	void disable();

	/**
	 * @brief	�H�����X�g�O���b�h�N���b�N���̏���
	 * @param   �q�_�C�A���O�̃C�x���g�������Ϗ�����
	 */
	void OnLineInfoGridMouseUp(SHORT Button, SHORT Shift, FLOAT X, FLOAT Y, HWND hwnd, UINT menuId);

	/**
	 * @brief	�H�����X�g�O���b�h�ҏW�O�̏���
	 * @param   �q�_�C�A���O�̃C�x���g�������Ϗ�����
	 */
	void OnLineInfoGridBeforeEdit(long Row, long Col, VARIANT_BOOL* Cancel);

	/**
	 * @brief	�H�����X�g�O���b�h�ҏW��̏���
	 * @param   �q�_�C�A���O�̃C�x���g�������Ϗ�����
	 */
	void OnLineInfoGridAfterEdit(long Row, long Col);

	/**
	 * @brief	���Ӊw���X�g�O���b�h�ҏW�O�̏���
	 * @param   �q�_�C�A���O�̃C�x���g�������Ϗ�����
	 */
	void OnAroundStaBeforeEdit(long Row, long Col, VARIANT_BOOL* Cancel);

	/**
	 * @brief	�I�𒆂̍s���폜����
	 * @note    ���ݑI������Ă���s�ɕR�t��CRow���폜���܂�
	 */
	void deleteSelectedRow();

	/**
	 *  @brief	 �H�����X�g�O���b�h�ɕύX�����������ۂ�
	 *  @retval  true   �ύX���� 
	 *  @retval  false  �ύX�Ȃ�
	 */
	bool isChange() const { return m_bIsChange; }

	/**
	 *  @brief	�H�����X�g�f�[�^���󂩂ǂ��� 
	 *  @retval true   ��ł���
	 *  @retval false  ��ł͂Ȃ�
	 */
	bool isEnptyRelData() const { return m_LineInfoData.empty(); };


private:

	/**
	 *  @brief	�u���Ӊw���X�g�v���X�g�r���[����`�F�b�N�̓����Ă��郌�R�[�h��ID�Q���擾
	 *  @return  �`�F�b�N�������Ă��郌�R�[�h��ID�Q
	 */
	std::vector<long> getCheckedId();

	/**
	 *  @brief	 ���Ӊw�O���b�h�̎w��s�̃`�F�b�N�L�����擾
	 *  @param	 �擾�Ώۍs�̃C���f�b�N�X 
	 *  @return  �`�F�b�N�������Ă��邩�ۂ�
	 */
	bool isCheckedAroundStaGrid(int index) const;

	/**
	 *  @brief	 ���Ӊw�O���b�h�̎w��s��JOIN_INFO_ID���擾
	 *  @param	 �擾�Ώۍs�̃C���f�b�N�X 
	 *  @return  JOIN_INFO_ID
	 */
	long getIdFromAroundStaGrid(int index) const;

	/**
	 *  @brief	 �H�����X�g�O���b�h�̎w��s��JOIN_INFO_ID���擾
	 *  @param	 �擾�Ώۍs�̃C���f�b�N�X 
	 *  @return  JOIN_INFO_ID
	 */
	long getIdFromLineInfoGrid(int index) const;

	/**
	 *  @brief	 �H�����X�g�O���b�h�̎w��s�̉w�ԍ����擾
	 *  @param	 �擾�Ώۍs�̃C���f�b�N�X 
	 *  @return  �w�ԍ�
	 */
	CString getStaNumFromLineInfoGrid(int index) const;

	/**
	 *  @brief	�H�����X�g�O���b�h��ύX�������ۂ��̃t���O���X�V	
	 */
	void updateChangeStatus();


private:

	IVSFlexGridPtr m_spLineInfoGrid;	//!< �H�����X�g�O���b�h�R���g���[�� 
	IVSFlexGridPtr m_spAroundStaGrid;	//!< ���Ӊw�O���b�h�R���g���[�� 

	CRelTableData m_LineInfoData;		//!< �H�����X�g�f�[�^
	CRelTableData m_AroundStaData;		//!< ���Ӊw���X�g�f�[�^
	CRelTableData m_LineInfoData_Org;	//!< �ύX�O�̘H�����X�g�f�[�^

	CRelationTableHelper m_RelTableMgr;	//!< �֘A�e�[�u���̑���N���X

	CApplication  m_cArcHelper;

	bool m_bIsChange;					//!< �l�̕ύX���s�������ۂ�

};

