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

#include "RowBase.h"
#include "FieldMap.h"
#include <boost/shared_ptr.hpp>

namespace sindy
{

/**
 * @class CFeatureClass
 * @brief �t�B�[�`���N���X�N���X
 * @file  CFeatureClass.h
 * @author �r�b�g�G�C�W�@�`
 * $Id$
*/
class CFeatureClass
{
public:
	typedef boost::shared_ptr<CRowBase> CSPRowBase;
	typedef std::map<long, CSPRowBase> Container;
	typedef Container::const_iterator const_iterator;

	/**
	 * @brief �R���X�g���N�^
	 */
	CFeatureClass(){};

	/**
	 * @brief �f�X�g���N�^
	 */
	~CFeatureClass()
	{
		m_ipFeatureClass = nullptr;
		m_mapContainer.clear();
	};

	/**
	 * @brief �R���X�g���N�^
	 */
	CFeatureClass(const IFeatureClassPtr& ipFeatureClass)
	{
		m_ipFeatureClass = ipFeatureClass;
	}

	/**
	 * @brief �R���X�g���N�^
	 */
	CFeatureClass( const CFeatureClass& obj )
	{
		m_ipFeatureClass = obj.m_ipFeatureClass;
	}

	/**
	 * @brief �R�s�[�R���X�g���N�^
	 */
	CFeatureClass& operator = (const IFeatureClassPtr& ipFeatureClass)
	{
		m_ipFeatureClass = ipFeatureClass;
		return *this;
	}

	operator IFeatureClassPtr() const { return m_ipFeatureClass; }

	/**
	 * @brief ��ԎQ�Ǝ擾
	 * @retval ISpatialReferencePtr
	*/
	ISpatialReferencePtr getSpRef();

	/**
	 * @brief �`��t�B�[���h���擾
	 * @retval CString �`��t�B�[���h��
	*/
	CString getShapeFieldName() const;

	/**
	 * @brief OBJECTID�t�B�[���h���擾
	 * @retval CString OBJECTID�t�B�[���h��
	*/
	CString getOIDFieldName() const;

	/**
	 * @brief �t�B�[�`������
	 * @param ipFilter [in] ��������
	 * @retval Container �����t�B�[�`���Q
	*/
	Container search(const IQueryFilterPtr& ipFilter);

	/**
	 * @brief �t�B�[�`������
	 * @param ipFilter [in] ��������
	 * @retval Container �����t�B�[�`���Q
	*/
	Container searchNoContain(const IQueryFilterPtr& ipFilter);

	/**
	 * @brief �t�B�[���h���쐬
	*/
	void CreateFieldMap();

	/**
	 * @brief �t�B�[���h���擾
	 * @retval CFieldMap �t�B�[���h���
	*/
	CSPFieldMap GetFieldMap() const;

	/**
	 * @brief �t�B�[�`���擾
	 * @param nOID [in] OBJECTID
	 * @retval IFeaturePtr �����t�B�[�`��
	*/
	CSPRowBase GetFeature(long nOID);

	/**
	 * @brief �t�B�[�`���쐬
	 * @retval IFeaturePtr �쐬�t�B�[�`��
	*/
	CSPRowBase Create();

	/**
	 * @brief �R���e�i�N���A
	*/
	void Clear();

	/**
	 * @brief �t�B�[�`������
	 * @retval Container �����t�B�[�`���Q
	*/
	const Container& getContainer() { return m_mapContainer; }

private:
	IFeatureClassPtr m_ipFeatureClass;	///< �t�B�[�`���N���X
	Container m_mapContainer;			///< �����t�B�[�`���Q
	CSPFieldMap m_spFieldMap;			///< �t�B�[���h���Q
};

}
