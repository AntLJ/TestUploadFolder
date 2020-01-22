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
namespace railwayline_grid {
	enum grid_cols{
		RailwayLineOid,		// RAILWAY_LINE_OID
		RailwayName,		// �H���� 
		CorpName,			// ��Ж� 
		col_num				// �J������
	};
}

/// �H�������O���b�h���`
namespace search_railway_grid {
	enum grid_cols{
		Check,				// �`�F�b�N�{�b�N�X
		RailwayLineOid,		// RAILWAY_LINE_OID
		RailwayName,		// �H���� 
		CorpName,			// ��Ж� 
		col_num				// �J������
	};

	enum search_mode{
		kAroundSearch,		// ���Ӊw�̘H���Ō���
		kStarionNameSearch	// �w���Ō���
	};
}


/**
 * @brief �֘A�e�[�u���ƓS���m�[�h�_�C�A���O��2�̃O���b�h�𐧌䂷��N���X
 * @note  �ėp�I�ȍ��ł͂Ȃ��A�{�_�C�A���O�̃O���b�h����ɓ���
 * @ntoe  �֘A�e�[�u���ւ̐�����{�N���X����s��
 */
class CRailwayLineController
{
public:
	// �wPOI�̌����͈�(���[�g��)
	static const int SEARCH_METER = 100;

public:
	CRailwayLineController(void): 
				m_bIsChange(false), 
				m_SearchMode(search_railway_grid::kAroundSearch) 
			{};
	~CRailwayLineController(void) {};
	
	/**
	* @brief ������
	* @param hwnd1      [in] �H�����X�g�O���b�h�R���g���[���̃n���h��
	* @param hwnd2      [in] ���Ӊw���X�g�O���b�h�R���g���[���̃n���h��
	* @param ipRow      [in] �S�������N�e�[�u���̃��R�[�h
	* @param cArcHelper [in] ArcHelper
	* @retval true  ����������
	* @retval false ���������s
	*/
	bool init( HWND hwnd1, HWND hwnd2, _IRowPtr& ipRow, CArcHelper cArcHelper);

	/**
	* @brief �H�����X�g�O���b�h�̕\�����X�V����
	*/
	void updateRailwayLineGrid();

	/**
	* @brief �H���������X�g�O���b�h�̕\�����X�V����
	*/
	void updateSearchRailwayGrid();

	/**
	* @brief ���Ӊw���X�g�O���b�h�̃`�F�b�N�̓��������R�[�h��H�����X�g�֒ǉ� 
	*/
	void addCheckedData();
	
	/**
	 * @brief  �w��t�B�[�`��������͈͂̉w�|�C���g����ǂݍ���
	 * @param  ipFeature [in] �w�|�C���g�������������ƂȂ�t�B�[�`��
	 * @param  ipLayer   [in] �w�|�C���g���C��
	 */
	void searchStationInfo(IFeaturePtr ipFeature, ILayerPtr ipLayer);

	/**
	 * @brief	�w�肵���w�𑖂��Ă���H�����X�g��ǂݍ���
	 * @note    �w���͒��Ԉ�v�Ō���
	 * @param   �H�������Ώۂ̉w��
	 */
	void searchStaName(CString strStaName);

	/**
	 * @brief	�O���b�h�̕ύX�_���֘A�e�[�u���ɔ��f 
	 * @retval true   �e�[�u���̍X�V����
	 * @retval false  �e�[�u���̍X�V���s
	 */
	bool updateTable();

	/**
	 * @brief	�O���b�g�̕ύX�ӏ���F�t������ 
	 */
	void drawColor();

	/**
	 * @brief	�H�����X�g�O���b�h�N���b�N���̏���
	 * @note    �����_�C�A���O�N���X�̃C�x���g�������Ϗ�����
	 * @param   bEnable   �`�F�b�N�{�b�N�X�̕ҏW���\�ɂ��邩�ǂ���
	 */
	void OnLineInfoGridMouseUp(SHORT Button, SHORT Shift, FLOAT X, FLOAT Y, HWND hwnd, UINT menuId, bool bEnable);

	/**
	 * @brief	�H�����X�g�O���b�h�ҏW�O�̏���
	 * @note    �����_�C�A���O�N���X�̃C�x���g�������Ϗ�����
	 * @note    �N���b�N���ꂽ�s�̘H�����̃t�B�[�`�����n�C���C�g�����Ɋi�[���Ă���
	 */
	void OnRailwayLineGridBeforeEdit(long Row, long Col, VARIANT_BOOL* Cancel);

	/**
	 * @brief	�H�������O���b�h�ҏW�O�̏���
	 * @note    �����_�C�A���O�N���X�̃C�x���g�������Ϗ�����
	 * @note    �N���b�N���ꂽ�s�̘H�����̃t�B�[�`�����n�C���C�g�����Ɋi�[���Ă���
	 * @param   bCheckEnable   �`�F�b�N�{�b�N�X�̕ҏW���\�ɂ��邩�ǂ���
	 */
	void OnSearchRailwayBeforeEdit(long Row, long Col, VARIANT_BOOL* Cancel, bool bCheckEnable);

	/**
	 * @brief	�I�𒆂̍s���폜����
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
	bool isEmptyRelData() const { return m_RailwayLineData.empty(); };

	/**
	 *  @brief	�������[�h�̃Z�b�g
	 *	@param	mode [in] �������[�h(���Ӊw���� or �w������)
	 */
	void setSearchMode(search_railway_grid::search_mode mode){ m_SearchMode = mode; };

	/**
	 *  @brief	�t�B�[�`���[���n�C���C�g������
	 */
	void highlightFeatures();
	
	/**
	 *  @brief  �H�����X�g�f�[�^�𓯊�����
	 *  @param  cRailwayLineCtlr   [in]  ������̃I�u�W�F�N�g
	 */
	void syncRailwayLineData( CRailwayLineController& cRailwayLineCtlr );

private:

	/**
	 *  @brief	 �H���������X�g�O���b�h����`�F�b�N�̓����Ă��郌�R�[�h��ID�Q���擾
	 *  @return  �`�F�b�N�������Ă��郌�R�[�h��ID�Q
	 */
	std::vector<long> getCheckedId();

	/**
	 *  @brief	 �H�������O���b�h�̎w��s�̃`�F�b�N�L�����擾
	 *  @param	  index  [in]  �擾�Ώۍs�̃C���f�b�N�X 
	 *  @return  �`�F�b�N�������Ă��邩�ۂ�
	 */
	bool isCheckedSearchRailwayGrid(int index) const;

	/**
	 *  @brief	 �H�������O���b�h�̎w��s��RAILWAY_LINE_ID���擾
	 *  @param	 index  [in]  �擾�Ώۍs�̃C���f�b�N�X 
	 *  @return  RAILWAY_LINE_ID
	 */
	long getIdFromSearchRailwayGrid(int index) const;

	/**
	 *  @brief	 �H�����X�g�O���b�h�̎w��s��RAILWAY_LINE_ID���擾
	 *  @param	 index  [in]  �擾�Ώۍs�̃C���f�b�N�X 
	 *  @return  RAILWAY_LINE_ID
	 */
	long getIdFromRailwayLineGrid(int index) const;

	/**
	 *  @brief	�H�����X�g�O���b�h��ύX�������ۂ��̃t���O���X�V	
	 */
	void updateChangeStatus();

	/**
	 *  @brief  �H�������O���b�h�֒l���i�[����
	 *  @param  cData [in]  �l�i�[���̘H���f�[�^�Q
	 */
	void putIntoSearchGrid(const CRelTableData& cData);

	/**
	 *  @brief  �w��R���e�i�̎w��S���H��ID�̃��R�[�h��H�����X�g�f�[�^�R���e�i�֊i�[����
	 *  @param  lId   [in]  �S���H��ID
	 *  @param  cData [in]  �i�[���̃R���e�i
	 */
	void addCheckedRecord(long lId, CRelTableData& cData);


private:

	IVSFlexGridPtr m_spRailwayLineGrid;				//!< �H�����X�g�O���b�h�R���g���[�� 
	IVSFlexGridPtr m_spSearchRailwayGrid;			//!< �H�������O���b�h�R���g���[�� 

	CRelTableData m_RailwayLineData;				//!< �H�����X�g�f�[�^
	CRelTableData m_SearchFromAroundStaData;		//!< �H���������X�g�f�[�^
	CRelTableData m_RailwayLineData_Org;			//!< �ύX�O�̘H�����X�g�f�[�^
	CRelTableData m_SearchFromStationNameData;		//!< �w���������X�g�f�[�^

	CRelationTableHelper m_RelTableMgr;				//!< �֘A�e�[�u���̑���N���X

	CApplication  m_cArcHelper;

	search_railway_grid::search_mode m_SearchMode;	//!< �������[�h
	bool m_bIsChange;								//!< �l�̕ύX���s�������ۂ�

	std::vector<CAdapt<IFeaturePtr>> m_HighlightFeatures;	//!< �n�C���C�g������t�B�[�`���[�Q
};
