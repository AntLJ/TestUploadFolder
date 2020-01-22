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

#include "RelStationSiteData.h"
#include "RelationTableHelper.h"

// WinLib
#include <WinLib/grid_util.h>


// �w���O���b�h
namespace station_grid
{
	// ���`
	enum grid_cols{
		GroupId,			// GROUP_ID
		StationName,		// �w��
		col_num				// �J������
	};

	// �w�b�_�[
	static const _TCHAR* HEADER    = _T("GROUPID|�w��");
}

// �w��񌟍��O���b�h
namespace search_station_grid
{
	// ���`
	enum grid_cols{
		Check,				// �`�F�b�N�{�b�N�X
		GroupId,		 	// GROUP_ID
		StationName,		// �w��
		col_num				// �J������
	};

	// �w�b�_�[
	static const _TCHAR* HEADER = _T(" |GROUPID|�w��");
}

// �N���b�N
namespace clicked {
	enum click{
		kLeft = 1,
		kRight = 2
	};
}


/**
 * @brief �w��� / �w��񌟍� ��2�̃O���b�h���������Ǘ�����N���X
 * @note  �ėp�I�ȍ��ł͂Ȃ��A�{�_�C�A���O�̃O���b�h����ɓ���
 * @note  �{���̓R���g���[���ƃf�[�^�����͕�����ׂ��Ȃ̂�������Ȃ����A
 * @ntoe  �֘A�e�[�u���ւ̐�����{�N���X����s���Ă���
 */
class CStationInfoController
{
public:
	/**
	 *   @brief  �R���X�g���N�^
	 */
	CStationInfoController(void): m_bIsChange(false) {};

	/**
	 *   @brief  �f�X�g���N�^
	 */
	~CStationInfoController(void) {};
	
	/**
	* @brief ������
	* @param hwnd1      [in] �w���O���b�h�R���g���[���̃n���h��
	* @param hwnd2      [in] �w��񌟍��O���b�h�R���g���[���̃n���h��
	* @param ipRow      [in] �w�|���S���̃��R�[�h
	* @retval true  ����������
	* @retval false ���������s
	*/
	bool init( HWND hwnd1, HWND hwnd2, _IRowPtr& ipRow );

	/**
	* @brief �w��񃊃X�g�O���b�h�̕\�����X�V����
	*/
	void updateStationInfoGrid();

	/**
	* @brief �w��񌟍����X�g�O���b�h�̕\�����X�V����
	*/
	void updateSearchStationInfoGrid();

	/**
	 * @brief	�O���b�h�̕ύX�_���֘A�e�[�u���ɔ��f 
	 * @retval true   �e�[�u���̍X�V����
	 * @retval false  �e�[�u���̍X�V���s
	 */
	bool updateTable();

	/**
	* @brief �`�F�b�N�̓������w��񌟍��f�[�^���w���f�[�^�֒ǉ�
	*/
	void addCheckedData();
	
	/**
	 * @brief	�I�𒆂̍s���폜����
	 */
	void deleteSelectedRow();

	/**
	 *  @brief	 �w���O���b�h�ɕύX�����������ۂ�
	 *  @retval  true   �ύX���� 
	 *  @retval  false  �ύX�Ȃ�
	 */
	bool isChange() const { return m_bIsChange; }

	/**
	 * @brief	�w�肵���w���̃��R�[�h��ǂݍ���
	 * @note    �w���͒��Ԉ�v�Ō���
	 * @param   strStationName [in]  �����Ώۂ̉w��
	 */
	void searchStationName( const CString& strStationName );


	/// �O���b�h�̃C�x���g�����̈����󂯂邽�߂̊֐�

	/**
	 * @brief	�w���O���b�h�N���b�N���̏���
	 * @note    �����_�C�A���O�N���X�̃C�x���g�������Ϗ�����
	 * @param   bEnable   �`�F�b�N�{�b�N�X�̕ҏW���\�ɂ��邩�ǂ���
	 */
	void OnStationInfoGridMouseUp(SHORT Button, SHORT Shift, FLOAT X, FLOAT Y, HWND hwnd, UINT menuId, bool bEnable);

	/**
	 * @brief	�w���O���b�h�ҏW�O�̏���
	 * @note    �����_�C�A���O�N���X�̃C�x���g�������Ϗ�����
	 */
	void OnStationInfoGridBeforeEdit(long Row, long Col, VARIANT_BOOL* Cancel);

	/**
	 * @brief	�w��񌟍��O���b�h�ҏW�O�̏���
	 * @note    �����_�C�A���O�N���X�̃C�x���g�������Ϗ�����
	 * @param   bCheckEnable   �`�F�b�N�{�b�N�X�̕ҏW���\�ɂ��邩�ǂ���
	 */
	void OnSearchStationInfoBeforeEdit(long Row, long Col, VARIANT_BOOL* Cancel, bool bCheckEnable);


private:

	/**
	 *  @brief	 �w��񌟍����X�g�O���b�h����`�F�b�N�̓����Ă��郌�R�[�h��ID�Q���擾
	 *  @return  �`�F�b�N�������Ă��郌�R�[�h��ID�Q
	 */
	std::vector<long> getCheckedId();

	/**
	 *  @brief	 �w��񌟍��O���b�h�̎w��s�̃`�F�b�N�L�����擾
	 *  @param	  index  [in]  �擾�Ώۍs�̃C���f�b�N�X 
	 *  @return  �`�F�b�N�������Ă��邩�ۂ�
	 */
	bool isCheckedSearchStationGrid(int index) const;

	/**
	 *  @brief	 �w��񌟍��O���b�h�̎w��s�̃O���[�vID���擾
	 *  @param	 index  [in]  �擾�Ώۍs�̃C���f�b�N�X 
	 *  @return  �O���[�vID
	 */
	long getGroupIdFromSearchStationGrid(int index) const;

	/**
	 *  @brief	 �w���O���b�h�̎w��s�̃O���[�vID���擾
	 *  @param	 index  [in]  �擾�Ώۍs�̃C���f�b�N�X 
	 *  @return  �O���[�vID
	 */
	long getGroupIdFromStationGrid(int index) const;

	/**
	 *  @brief	�w���O���b�h��ύX�������ۂ��̃t���O���X�V	
	 */
	void updateChangeStatus();

	/**
	 * @brief	�O���b�g�̕ύX�ӏ���F�t������ 
	 */
	void drawColor();


private:

	IVSFlexGridPtr m_spStationGrid;					//!< �w���O���b�h�R���g���[�� 
	IVSFlexGridPtr m_spSearchStationGrid;			//!< �w��񌟍��O���b�h�R���g���[�� 

	CRelStationSiteData m_StationInfoData;			//!< �w���f�[�^
	CRelStationSiteData m_StationInfoData_Org;		//!< �ύX�O�̉w���f�[�^
	CRelStationSiteData m_SearchStationNameData;	//!< �w�������f�[�^

	CRelationTableHelper m_RelTableMgr;				//!< �֘A�e�[�u���̑���N���X

	bool m_bIsChange;								//!< �l�̕ύX���s�������ۂ�

};
