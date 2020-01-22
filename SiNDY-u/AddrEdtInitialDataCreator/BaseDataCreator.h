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

#include "Arguments.h"
#include "LayerInfoManager.h"

/**
 * @brief �G���[���O�w�b�_���X�g
 */
const std::vector<CString> ERRLOG_HEADER_LIST = boost::assign::list_of
		( _T("FLAG") )
		( _T("LAYER") )
		( _T("OBJECTID") )
		( _T("X") )
		( _T("Y") )
		( _T("�G���[���x��") )
		( _T("�G���[�R�[�h") )
		( _T("�G���[���b�Z�[�W") )
		( _T("���1") )
		( _T("���2") )
		( _T("���3") )
		;

/**
 * @brief Conversion between VARIANT_BOOL and boolean
 */
struct VariantBool
{
	VariantBool() : vb(VARIANT_FALSE) {}
	explicit VariantBool(bool b) : vb(b ? VARIANT_TRUE : VARIANT_FALSE) {}
	explicit VariantBool(VARIANT_BOOL b) : vb(b) {}
	operator VARIANT_BOOL() const { return vb; }
	operator bool() const { return (vb == VARIANT_FALSE) ? false : true; }
	VARIANT_BOOL* operator &() {return &vb;}
	VARIANT_BOOL vb;
};

/**
 * @brief   �f�[�^�����N���X�i���N���X�j
 */
class BaseDataCreator
{
public:

	/** 
	 * @ brief States of session
	 */
	enum State : char
	{
		S_ERROR_MIXED = -1,	///< Mixed with versioned and non versioned
		S_ERROR = 0,		///< Open failed
		S_VERSIONED,		///< Versioned
		S_NON_VERSIONED,	///< Non versioned
	};

	/**
	 * @brief   �R���X�g���N�^
	 * @param   arg [in]  ����
	 */
	BaseDataCreator(const Arguments& arg);

	/**
	 * @brief   �f�X�g���N�^
	 */
	virtual ~BaseDataCreator() {}

	/**
	 * @brief   �������i���ʁj
	 */
	bool initBase();

	/**
	 * @brief   �������i�h���N���X�Ŏ����j
	 */
	virtual bool init() = 0;

	/**
	 * @brief   �f�[�^�����i�h���N���X�Ŏ����j
	 */
	virtual bool create() = 0;

protected:

	/**
	 * @brief   ��������
	 * @param   layerManager [in]  ���C�����Ǘ�
	 * @param   whereClause [in]  ��������
	 * @return  FeatureCursor
	 */
	IFeatureCursorPtr searchByWhereClause(LayerInfoManager& layerManager, const CString& whereClause);

	/**
	 * @brief   �t�B�[���h�l�ݒ�
	 * @param   featureBuf [in/out]  �Ώ�FeatureBuffer
	 * @param   fieldIdx [in]  �t�B�[���h�C���f�b�N�X
	 * @param   value [in]  �ݒ�l
	 * @retval  true  ����
	 * @retval  false ���s
	 */
	bool putValue(IFeatureBufferPtr& featureBuf, long fieldIdx, const CString& value);

	/**
	 * @brief   �t�B�[���h�l�ݒ�
	 * @param   featureBuf [in/out]  �Ώ�FeatureBuffer
	 * @param   fieldIdx [in]  �t�B�[���h�C���f�b�N�X
	 * @param   value [in]  �ݒ�l
	 * @param   isNull [in]  �l��null�ɂ���ꍇ��true��ݒ�i�f�t�H���g��false�j
	 * @retval  true  ����
	 * @retval  false ���s
	 */
	bool putValue(IFeatureBufferPtr& featureBuf, long fieldIdx, int value, bool isNull = false);

	/**
	 * @brief   �t�B�[���h�l�ݒ�
	 * @param   featureBuf [in/out]  �Ώ�FeatureBuffer
	 * @param   fieldIdx [in]  �t�B�[���h�C���f�b�N�X
	 * @param   value [in]  �ݒ�l
	 * @retval  true  ����
	 * @retval  false ���s
	 */
	bool putValue(IFeatureBufferPtr& featureBuf, long fieldIdx, CComVariant& value);

	/**
	 * @brief   �t�B�[���h�l�擾
	 * @param   featureBuf [in/out]  �Ώ�Feature
	 * @param   fieldIdx [in]  �t�B�[���h�C���f�b�N�X
	 * @param   value [out]  �擾�l
	 * @retval  true  ����
	 * @retval  false ���s
	 */
	bool getValue(const IFeaturePtr& feature, long fieldIdx, CComVariant& value);

	/**
	 * @brief   �t�B�[���h�l�擾
	 * @param   row [in/out]  �Ώ�Row
	 * @param   fieldIdx [in]  �t�B�[���h�C���f�b�N�X
	 * @param   value [out]  �擾�l
	 * @retval  true  ����
	 * @retval  false ���s
	 */
	bool getValue(const _IRowPtr& row, long fieldIdx, CComVariant& value);

	/**
	 * @brief   �ҏW�J�n����
	 * @retval  true  ����
	 * @retval  false ���s
	 */
	bool startEdit();

	/**
	 * @brief   �ҏW�I������
	 * @retval  true  ����
	 * @retval  false ���s
	 */
	bool stopEdit(bool isSave);

	/**
	 * @brief   �ҏW�j������
	 * @retval  true  ����
	 * @retval  false ���s
	 */
	bool abort();

	/**
	 * @brief �e�[�u���̓��v���X�V
	 * @param layerName[in] ���C����
	 * @retval  true  ����
	 * @retval  false ���s
	 */
	bool tableAnalyze(const CString& layerName);

private:
	/**
	 * @brief �ҏW�J�n�iStartEditing�j
	 * @param bRedoUndo[in] RedoUndo���s�����itrue�F�s���Afalse�F�s��Ȃ��j
	 * @return ��������(State)
	 */
	State StartEditing(bool bRedoUndo);

	/**
	 * @brief �ҏW�I���iStopEditing�j
	 * @param bSave[in] �ҏW���ʂ�ۑ�����itrue�F�ۑ�����Afalse�F�ۑ����Ȃ��j
	 * @retval  true  ����
	 * @retval  false ���s
	 */
	bool StopEditing(bool bSave);

	/**
	 * @brief �ҏW�J�n�iStartEditOperation�j
	 * @retval  true  ����
	 * @retval  false ���s
	 */
	bool StartEditOperation();

	/**
	 * @brief �ҏW�I���iStopEditOperation�j
	 * @retval  true  ����
	 * @retval  false ���s
	 */
	bool StopEditOperation();

	/**
	 * @brief �ҏW�j��(AbortEditOperation)
	 * @note �o�[�W�����Ή��ɑ΂���ҏW�J�n���̂ݎ��{
	 * @retval  true  ����
	 * @retval  false ���s
	 */
	bool AbortEditOperation();

protected:
	Arguments m_args;                                      //!< ����
	IWorkspaceEditPtr m_edtWorkspace;                      //!< ���[�N�X�y�[�X
	std::map<CString, LayerInfoManager> m_baseLayerInfos;  //!< �Q�ƌ����C�����Ǘ��N���X�Q
	std::map<CString, LayerInfoManager> m_edtLayerInfos;   //!< �ҏW���C�����Ǘ��N���X�Q
	std::map<CString, CComVariant> m_commonAttrMap;        //!< �t�B�[�`�����ʑ����X�V�l�Ǘ�MAP
	bool m_isVersioned;
};

