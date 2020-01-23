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

#include "stdafx.h"
#include "CheckRoadWalkNode.h"

class CCheckRoadWalkLink : public CCheckRoadWalkNode
{
public:
	CCheckRoadWalkLink(void);
	~CCheckRoadWalkLink(void);


	/**
	 * @brief	RWL�t�H�[�}�b�g�`�F�b�N
	 * @param	ipFeatureClass	[in]	�`�F�b�N�Ώۂ̃t�B�[�`���[�N���X
	 * @param	fieldIndex	[in]	�`�F�b�N�Ώۂ̃t�B�[���h�C���f�b�N�X
	 * @param	dbCtrl	[in]	��r�Ώۂ�DB���
	 * @return	bool
	 */
	virtual bool checkFormat(const IFeatureClassPtr ipFeatureClass, FIELD_INDEX fieldIndex, CDatabaseController dbCtrl);	

protected:
	enum walkAttrType 
	{
		eAUTOWALK=1, 
		eSTAIR=2, 
		eSLOPE=4, 
		eESCALATOR=8, 
		eARCADE=16, 
		eTUNNEL=32,
		eOPENAREA=64
	};

	/**
	 * @brief	NodeID�p��r�`�F�b�N
	 * @param	objectID	[in]	�I�u�W�F�N�gID
	 * @param	ipFeature	[in]	�`�F�b�N�Ώۂ̃t�B�[�`���[
	 * @param	fieldName	[in]	�`�F�b�N�Ώۂ̃t�B�[���h��
	 * @param	fieldIndex	[in]	�`�F�b�N�Ώۂ̃t�B�[���h�C���f�b�N�X
	 * @param	ipDbFeature	[in]	��r�Ώۂ̃t�B�[�`���[
	 * @param	dbWalkfieldName	[in]	��r�Ώۂ̕��s�҃t�B�[���h��
	 * @param	dbRoadfieldName	[in]	��r�Ώۂ̓��H�t�B�[���h��
	 * @param	dbFieldIndex	[in]	��r�Ώۂ̃t�B�[���h�C���f�b�N�X
	 * @return	bool
	 */
	virtual bool checkNodeID(long objectID, const IFeaturePtr ipFeature,CString fieldName, FIELD_INDEX fieldIndex,const IFeaturePtr ipDbFeature, CString dbWalkFieldName, CString dbRoadFieldName, FIELD_INDEX dbFieldIndex);

	/**
	 * @brief	DB��r�`�F�b�N
	 * @param	objectID	[in]	�I�u�W�F�N�gID
	 * @param	ipFeature	[in]	�`�F�b�N�Ώۂ̃t�B�[�`���[
	 * @param	fieldName	[in]	�`�F�b�N�Ώۂ̃t�B�[���h��
	 * @param	fieldIndex	[in]	�`�F�b�N�Ώۂ̃t�B�[���h�C���f�b�N�X
	 * @param	ipDbFeature	[in]	��r�Ώۂ̃t�B�[�`���[
	 * @param	dbWalkfieldName	[in]	��r�Ώۂ̕��s�҃t�B�[���h��
	 * @param	dbRoadfieldName	[in]	��r�Ώۂ̓��H�t�B�[���h��
	 * @param	dbFieldIndex	[in]	��r�Ώۂ̃t�B�[���h�C���f�b�N�X
	 * @return	bool
	 */
	virtual bool checkDBValue(long objectID, const IFeaturePtr ipFeature,CString fieldName, FIELD_INDEX fieldIndex,const IFeaturePtr ipDbFeature, CString dbWalkFieldName, CString dbRoadFieldName,  FIELD_INDEX dbFieldIndex);
	
	/**
	 * @brief	���s�ґ����쐬
	 * @param	objectID	[in]	�I�u�W�F�N�gID
	 * @param	ipDbFeature	[in]	�쐬����DB�̃t�B�[�`���[
	 * @param	dbFieldName	[in]	�쐬����DB�̃t�B�[���h��
	 * @param	dbFieldIndex	[in]	�쐬����DB�̃t�B�[���h�C���f�b�N�X
	 * @param	walkAttrType	[in]	�쐬������s�ґ����̎��
	 * @param	walkAttrValue	[out]	�쐬�������s�ґ����̒l
	 * @return	bool
	 */
	virtual bool getWalkAttr(long objectID, const IFeaturePtr ipDbFeature, CString dbFieldName, FIELD_INDEX dbFieldIndex, walkAttrType walkAttrType, int& walkAttrValue);
}
;
