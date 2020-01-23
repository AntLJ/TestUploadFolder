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

/**
 * @brief �e�[�u�����̎擾�N���X
 */
class TableNamesSearcher
{
public:
	TableNamesSearcher();
	~TableNamesSearcher();

	/**
	 * @brief  ���[�N�X�y�[�X���ɂ���e�[�u�����̂����ׂĎ擾����
	 * @param  ipDatasetNames  [in]  ���Ƃ���Dataset�̖���
	 * @param  cTableNames    [out] �e�[�u�����̈ꗗ
	 * @retval
	 */
	void getWorkspaceTableNames( IEnumDatasetName* ipDatasetNames, std::vector<uh::tstring>& cTableNames );
private:
	/**
	 * @brief  �f�[�^�Z�b�g�ł��邩����
	 * @param  type  [out]  esriDatasetType
	 * @retval true:�f�[�^�Z�b�g�ł���
	 */
	bool isDataset(esriDatasetType type);

};
