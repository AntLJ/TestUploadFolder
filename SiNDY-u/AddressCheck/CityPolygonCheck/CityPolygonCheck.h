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
#include "..\layercheckbase.h"
#include "..\TextAddressMaster.h"

/**
* @brief	�s�撬���R�[�h�̃|���S��
*/
struct CityPolygonValue 
{
	long oID;            //!< �I�u�W�F�N�gID
	IGeometryPtr geo;    //!< �W�I���g��
	/**
	 * @brief  �R���X�g���N�^
	 */
	CityPolygonValue():oID(INVALID_VALUE){}

	/**
	 * @brief  �R���X�g���N�^
	 * @param	[in] objectID  //!< �I�u�W�F�N�gID
	 * @param	[in] cityGeo   //!< �W�I���g��
	 */
	CityPolygonValue(long objectID, const IGeometryPtr& cityGeo):oID(objectID), geo(cityGeo){}
};
/**

 * @class	CityPolygonCheck
 * @brief	�Z���n�`�F�b�N
 */
class CityPolygonCheck :	public LayerCheckBase
{
public:
	/**
	 * @brief  �R���X�g���N�^
	 */
	CityPolygonCheck(void){}

	 /**
	 * @brief  �f�X�g���N�^
	 */
	virtual ~CityPolygonCheck(void){}

	/**
	 * @brief	���C���`�F�b�N���擾
	 * @return	���C���`�F�b�N��
	 */
	LPCTSTR getName(){ return city_polygon::kTableName; }

	/**
	 * @brief	������
	 * @param	[in] settingFile		�ݒ�t�@�C��
	 * @param	[in] tableFinder		�e�[�u���Ǘ�
	 * @param	[in] errorLog           �G���[�t�@�C���Ǘ�
	 * @param	[in] textAddressMaster  �Z���}�X�^�f�[�^�p�[�X
	 * @retval	true:����
	 * @retval	false:���s
	 */
	bool initialize( const SettingFile& settingFile, TableFinder& tableFinder, ErrLogger& errorLog, const CString& textAddressMaster );

	/**
	 * @brief	�`�F�b�N
	 * @param	[in] ulMeshCode		    ���b�V���R�[�h
	 * @param	[in] ipMeshGeometry	    ���b�V���W�I���g��
	 * @param	[in] ulMeshID	    �@�@���b�V���I�u�W�F�N�gID
	 */
	void check( unsigned long ulMeshCode, const IGeometryPtr& ipMeshGeometry, long meshID );

	/**
	 * @brief	�����s�撬���R�[�h�̃`�F�b�N
	 * @param	[in] oID		        �I�u�W�F�N�gID
	 * @param	[in] geo        	    �W�I���g��
	 * @param	[in] cityCodeStr        �s�撬���R�[�h
	 * @param	[out]sameCityPolygonMap	�s�撬���R�[�h�ƃW�I���g���̃R���e�i
	 */
	void checkSameCityPolygon( long oID, const IGeometryPtr& geo, const CString& cityCodeStr, map<CString, map<long, IGeometryPtr>>& sameCityPolygonMap );

	/**
	 * @brief	�}�X�^�[�ƃ`�F�b�N
	 * @param	[in] oID		        �I�u�W�F�N�gID
	 * @param	[in] cityCodeStr        �s�撬���R�[�h
	 */
	void checkWithMasterInfo( long oID, const CString& cityCodeStr );

private:
	IFeatureClassPtr   m_ipCityPolygon;     //!< �s�s�|���S���t�B�[�`���N���X
	FIELDSMAP          m_cityPolygonFieldsMap;  //!< �s�s�|���S���t�B�[�`���N���X�̃t�B�[���h�}�b�v

	TextAddressMaster  m_textAddressMaster; //!< �e�L�X�g�Z���}�X�^

	RelationCheck      m_relationCheck;     //!< ���ʂ̊֌W�̃`�G�b�N
};

