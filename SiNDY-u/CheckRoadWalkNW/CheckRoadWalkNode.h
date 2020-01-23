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
#include "DatabaseController.h"
#include "GeomProc.h"

#include "Properties.h"
#include "Log.h"

using namespace std;

class CCheckRoadWalkNode
{
protected:
	std::vector<CString> m_errorMessageList;
	CGeomProc m_geomProcess;
 
public:
	CCheckRoadWalkNode(void);
	~CCheckRoadWalkNode(void);

	/**
	 * @brief	RWN�t�H�[�}�b�g�`�F�b�N
	 * @param	ipFeatureClass	[in]	�`�F�b�N�Ώۂ̃t�B�[�`���[�N���X
	 * @param	fieldIndex	[in]	�`�F�b�N�Ώۂ̃t�B�[���h�C���f�b�N�X
	 * @param	dbCtrl	[in]	��r�Ώۂ�DB���
	 * @return	bool
	 */
	virtual bool checkFormat(const IFeatureClassPtr ipFeatureClass, FIELD_INDEX fieldIndex, CDatabaseController dbCtrl);

	/**
	 * @brief	�G���[���b�Z�[�W���X�g�̎擾
	 * @return	std::vector<CString>	�G���[���b�Z�[�W���X�g
	 */
	virtual std::vector<CString> getMeessageList();
	
	virtual void clear(void);

protected:
	/**
	 * @brief	�w�肳�ꂽ�t�B�[�`���[����l�̎擾
	 * @note	�擾�Ɏ��s�����ꍇ�A�G���[���b�Z�[�W��ǉ�����
	 * @param	objectID	[in]	�I�u�W�F�N�gID
	 * @param	ipFeature	[in]	�擾�Ώۂ̃t�B�[�`���[
	 * @param	fieldName	[in]	�擾�Ώۂ̃t�B�[���h��
	 * @param	fieldIndex	[in]	�擾�Ώۂ̃t�B�[���h�C���f�b�N�X
	 * @param	value	[out]	��r�����l
	 * @return	bool
	 */
	virtual bool getFeatureValue(long objectID, const IFeaturePtr ipFeature,CString fieldName, FIELD_INDEX fieldIndex, CComVariant& value); 
	
	/**
	 * @brief	���H�FDB��r�A���s�ҁF�Œ�l��r�A�܂��͂��̋t�̃`�F�b�N
	 * @param	objectID	[in]	�I�u�W�F�N�gID
	 * @param	ipFeature	[in]	�`�F�b�N�Ώۂ̃t�B�[�`���[
	 * @param	fieldName	[in]	�`�F�b�N�Ώۂ̃t�B�[���h��
	 * @param	fieldIndex	[in]	�`�F�b�N�Ώۂ̃t�B�[���h�C���f�b�N�X
	 * @param	ipDbFeature	[in]	��r��̃t�B�[�`���[
	 * @param	dbFieldName	[in]	��r��̃t�B�[���h��
	 * @param	dbFieldIndex	[in]	��r��̃t�B�[���h�C���f�b�N�X
	 * @param	walkIsFixedValue	[in]	�Œ�l��r�̑Ώۂ����s�҂��ۂ�
	 * @param	fixedValue	[in]	��r����Œ�l
	 * @return	bool
	 */
	virtual bool checkDBValueFixedValue(long objectID, 
										const IFeaturePtr ipFeature, CString fieldName, FIELD_INDEX fieldIndex,
										const IFeaturePtr ipDbFeature, CString dbFieldName, FIELD_INDEX dbFieldIndex,
										bool walkIsFixedValue, long fixedValue);

	/**
	 * @brief	SHAPE�̒l��DB�̒l�̓��ꔻ��
	 * @note	DB�̒l��NULL�ł������ꍇ�ASHAPE�̌^����" "�܂���-1�Ɠ����ł��邩��r����
	 * @param	shapeValue	[in]	SHAPE�̒l
	 * @param	dbValue	[in]	DB�̒l
	 * @return	bool
	 */
	virtual bool compShapeDbValue(CComVariant shapeValue, CComVariant dbValue);

	/**
	 * @brief	�`���v�`�F�b�N�i�|�C���g�j
	 * @param	objectID	[in]	�I�u�W�F�N�gID
	 * @param	ipShapeFeature	[in]	SHAPE�̃t�B�[�`���[
	 * @param	ipDBFeature	[in]	DB�̃t�B�[�`���[
	 * @return	bool
	 */
	virtual bool checkGeomPoint(long objectID, const IFeaturePtr ipShapeFeature, const IFeaturePtr ipDBFeature);

	/**
	 * @brief	�`���v�`�F�b�N�i�����|�C���g�j
	 * @param	objectID	[in]	�I�u�W�F�N�gID
	 * @param	ipShapeFeature	[in]	SHAPE�̃t�B�[�`���[
	 * @param	ipDBFeature	[in]	DB�̃t�B�[�`���[
	 * @return	bool
	 */
	virtual bool checkGeomPoints(long objectID, const IFeaturePtr ipShapeFeature, const IFeaturePtr ipDBFeature);


	/**
	 * @brief	�G���[���b�Z�[�W�̍쐬�ƒǉ�
	 * @param	objectID	[in]	�I�u�W�F�N�gID
	 * @param	fieldName	[in]	�t�B�[���h��
	 * @param	message	[in]	�G���[���b�Z�[�W
	 * @param	value	[in]	�G���[�Ώۂ̒l
	 * @param	compValue	[in]	��r�Ώۂ̒l
	 */
	virtual void pushColumnErrorMessage(long objectID,CString filedName, CString message, CComVariant value=NULL, CComVariant compValue=NULL);

	/**
	 * @brief	�G���[���b�Z�[�W�̒ǉ�
	 * @param	message	[in]	�G���[���b�Z�[�W
	 */
	virtual void pushErrorMessage(CString message);
};
