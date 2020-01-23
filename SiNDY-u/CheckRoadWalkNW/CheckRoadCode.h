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
#include "CheckRoadWalkNode.h"

#define D_WALK_ROAD_NAME	101

class CCheckRoadCode : public CCheckRoadWalkNode
{
public:
	CCheckRoadCode(void);
	~CCheckRoadCode(void);

	/**
	 * @brief	road_code�t�H�[�}�b�g�`�F�b�N
	 * @param	ipTable	[in]	�`�F�b�N�Ώۂ̃e�[�u��
	 * @param	fieldIndex	[in]	�`�F�b�N�Ώۂ̃t�B�[���h�C���f�b�N�X
	 * @param	dbCtrl	[in,out]	��r�Ώۂ�DB���
	 * @return	bool
	 */
	virtual bool checkFormat(const ITablePtr& ipTable, const FIELD_INDEX& fieldIndex, CDatabaseController& dbCtrl);

protected:
	/**
	 * @brief	�w�肳�ꂽ���R�[�h����l�̎擾
	 * @note	�擾�Ɏ��s�����ꍇ�A�G���[���b�Z�[�W��ǉ�����
	 * @param	roadcode	[in]	�H���R�[�h
	 * @param	ipRow	[in]	�擾�Ώۂ̃��R�[�h
	 * @param	fieldName	[in]	�擾�Ώۂ̃t�B�[���h��
	 * @param	fieldIndex	[in]	�擾�Ώۂ̃t�B�[���h�C���f�b�N�X
	 * @param	value	[out]	�擾�����l
	 * @return	bool
	 */
	virtual bool getValue(long roadcode, const _IRowPtr& ipRow, const CString& fieldName, const FIELD_INDEX& fieldIndex, CComVariant& value);

	/**
	 * @brief	DB��r�`�F�b�N
	 * @param	roadcode	[in]	�H���R�[�h
	 * @param	ipRow	[in]	�`�F�b�N�Ώۂ̃��R�[�h
	 * @param	fieldName	[in]	�`�F�b�N�Ώۂ̃t�B�[���h��
	 * @param	fieldIndex	[in]	�`�F�b�N�Ώۂ̃t�B�[���h�C���f�b�N�X
	 * @param	mapDbValue	[in]	��r�Ώۂ�DB���R�[�h�̒lmap
	 * @param	dbWalkfieldName	[in]	��r�Ώۂ̕��s�҃t�B�[���h��
	 * @param	dbRoadfieldName	[in]	��r�Ώۂ̓��H�t�B�[���h��
	 * @param	walkRecord	[in]	���s�҂��ۂ�
	 * @return	bool
	 */
	virtual bool checkDBValue(long roadcode, const _IRowPtr& ipRow, const CString& fieldName, const FIELD_INDEX& fieldIndex,
								  const MAP_VALUE& mapDbValue, const CString& dbWalkFieldName, const CString& dbRoadFieldName, bool walkRecord);

};

