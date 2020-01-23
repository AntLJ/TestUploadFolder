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
#include "LogManager.h"
#include "CommonAttribute.h"
#include <TDC/sindylib_core/FieldMap.h>

namespace
{
	// key:11���Z���R�[�h val:�X�֔ԍ�
	typedef std::map<CString,CString> AddrZipCodeMap;
}

/**
 * @brief   ���C���N���X
 */
class CCreateZipCodePolygon
{

public:

	/**
	 * @brief   �R���X�g���N�^
	 * @param   args [in]  ����
	 */
	CCreateZipCodePolygon( const Arguments& args ) :
		m_idxCityCode(-1), m_idxAddrCode(-1), m_idxAddrCode2(-1)
	{
		m_args = args;
	}

	/**
	 * @brief   �f�X�g���N�^
	 */
	~CCreateZipCodePolygon(){}

	/**
	 * @brief   ���s
	 * @retval  true  ����
	 * @retval  false ���s
	 */
	bool execute();

	/**
	* @brief   �������s(Shp�o��)
	* @retval  true  ����
	* @retval  false ���s
	*/
	bool executeShp();

	/**
	* @brief   �������s(SDE�o��)
	* @retval  true  ����
	* @retval  false ���s
	*/
	bool executeSde();

private:

	/**
	 * @brief   ������
	 * @retval  true  ����
	 * @retval  false ���s
	 */
	bool init();

	/**
	 * @brief �Z��DB�֌W�̏����ݒ�
	 * @note �Z���}�X�^�ɐڑ����āA�X�֔ԍ����X�g�����
	 * @retval true ����
	 * @retval false ���s
	 */
	bool JusyoInit();

	/**
	 * @brief �s�s�n�}�s���E�֌W�̏����ݒ�
	 * @param ipCityAdminFc [out] �s�s�n�}�s���E�t�B�[�`���N���X
	 * @retval true ����
	 * @retval false ���s
	 */
	bool CityAdminInit(IFeatureClassPtr& ipCityAdminFc);

	/**
	 * @brief �o�͐�Shape�t�@�C���̏����ݒ�
	 * @param ipZipCodePolygonFc [out] �X�֔ԍ��|���S���f�[�^�o�͐�t�B�[�`���N���X
	 * @param ipZipCodeTable [out] �X�֔ԍ��e�[�u���f�[�^�o�͐�e�[�u��
	 * @retval true ����
	 * @retval false ���s
	 */
	bool ShapeInit(IFeatureClassPtr& ipZipCodePolygonFc, ITablePtr& ipZipCodeTable);

	/**
	 * @brief �X�֔ԍ��擾
	 * @param cityCode [in] �s�s�n�}�s���E�|���S����CITYCODE
	 * @param addrCode [in] �s�s�n�}�s���E�|���S����ADDRCODE�A�܂���ADDRCODE2
	 * @param oid [in] �s�s�n�}�s���E�|���S����OBJECTID
	 * @param zipCode [out] �X�֔ԍ�
	 * @retval true ��������
	 * @retval false �������s
	 */
	bool GetZipCode(const CString& cityCode, const CString& addrCode, long oid, CString& zipCode);

	/**
	 * @brief 11���Z���R�[�h����X�֔ԍ����擾
	 * @param elevenCode [in] 11���Z���R�[�h
	 * @retval �X�֔ԍ�
	 */
	CString TransAddrcode2ZipCode(const CString& elevenCode);

	/**
	 * @brief �X�֔ԍ��|���S���f�[�^�o��
	 * @param zipCodePolygon [in] �X�֔ԍ��|���S���o�͐�t�B�[�`���N���X
	 * @param polygonId [in] �X�֔ԍ��|���S��ID
	 * @param ipGeometry [in] �X�֔ԍ��|���S���W�I���g��
	 * @param zipCode [in] �X�֔ԍ�
	 */
	void OutputFeatureClass(
		IFeatureClassPtr& zipCodePolygon,
		long polygonId,
		const IGeometryPtr& ipGeometry,
		const CString& zipCode);

	/**
	 * @note �X�֔ԍ��|���S���f�[�^�o��(SDE)
	 * @param featureBuffer [in] �t�B�[�`���o�b�t�@
	 * @param featureCursor [in] �C���T�[�g�p�J�[�\��
	 * @param fieldMap [in] �X�֔ԍ��|���S���̃t�B�[���h�Ή��\
	 * @param ipGeometry [in] �|���S���W�I���g���`��
	 * @param zipCodePair [in] �����Ƃ��ăZ�b�g����X�֔ԍ�
	 */
	void OutputFeatureClass(
		IFeatureBufferPtr& featureBuffer,
		IFeatureCursorPtr& featureCursor,
		const sindy::CFieldMap& fieldMap,
		const IGeometryPtr& ipGeometry,
		const std::pair<CString, CString>& zipCodePair);

	/**
	 * @brief �X�֔ԍ��e�[�u���f�[�^�o��
	 * @param zipCodeTable [in] �X�֔ԍ��e�[�u���o�͐�e�[�u��
	 * @param polygonId [in] �X�֔ԍ��|���S��ID
	 * @param addrCodes [in] �X�֔ԍ��|���S���ɑΉ�����Z���R�[�h
	 */
	void OutputTable(
		ITablePtr& zipCodeTable,
		long polygonId,
		const std::vector<CString>& addrCodes);

public:
	LogManager m_logManager; //!< ���O�Ǘ��N���X

private:
	Arguments m_args; //!< ����
	CommonAttribute m_commonAttr; //!< ���ʂ̑����l(�ŏI�X�V������)�i�[�N���X
	AddrZipCodeMap m_addrZipCodeMap; //!< �Z���R�[�h�ƗX�֔ԍ��̑Ή��\
	ISpatialReferencePtr m_cityAdminSpatialReference; //!< �s�s�n�}�s���E�̋�ԎQ��
	long m_idxCityCode, m_idxAddrCode, m_idxAddrCode2; //!< CITY_ADMIN�̃t�B�[���h�C���f�b�N�X
};

