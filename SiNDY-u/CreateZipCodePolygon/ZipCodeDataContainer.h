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

class ZipCodeDataContainer
{
public:
	ZipCodeDataContainer(void);
	~ZipCodeDataContainer(void);

	// �|���S��ID�A�X�֔ԍ��A�Z���R�[�h�A�ʐς̏���
	bool operator < (const ZipCodeDataContainer& dst)
	{
		return
			(m_polygonId != dst.m_polygonId) ? (m_polygonId < dst.m_polygonId) :
			(0 != dst.m_zipcode.Compare(m_zipcode)) ? (0 < dst.m_zipcode.Compare(m_zipcode)) :
			(0 != dst.m_prefCode.Compare(m_prefCode)) ? (0 < dst.m_prefCode.Compare(m_prefCode)) :
			(0 != dst.m_cityCode.Compare(m_cityCode)) ? (0 < dst.m_cityCode.Compare(m_cityCode)) :
			(0 != dst.m_oazaCode.Compare(m_oazaCode)) ? (0 < dst.m_oazaCode.Compare(m_oazaCode)) :
			(0 != dst.m_azaCode.Compare(m_azaCode)) ? (0 < dst.m_azaCode.Compare(m_azaCode)) :
			(CalcArea(m_geometry) < CalcArea(dst.m_geometry));
	}

	bool operator == (const ZipCodeDataContainer& dst)
	{
		CString addrCode =
			m_prefCode + m_cityCode + m_oazaCode + m_azaCode;
		CString dstAddrCode =
			dst.m_prefCode + dst.m_cityCode + dst.m_oazaCode + dst.m_azaCode;

		return (m_polygonId == dst.m_polygonId) &&
			(0 == m_zipcode.Compare(dst.m_zipcode)) &&
			(0 == addrCode.Compare(dstAddrCode));
	}

	/**
	 * @brief �X�֔ԍ��̔�r
	 * @param target [in] ��r�Ώ�
	 * @retval true �X�֔ԍ���������
	 * @retval false �X�֔ԍ����������Ȃ�
	 */
	bool CompareZipCode(const ZipCodeDataContainer& target);

	/**
	 * @brief 11���Z���R�[�h�̔�r
	 * @param target [in] ��r�Ώ�
	 * @retval true �Z���R�[�h��������
	 * @retval false �Z���R�[�h���������Ȃ�
	 */
	bool CompareAddrCode(const ZipCodeDataContainer& target);

	/**
	 * @brief �f�[�^�̊i�[
	 * @param adminCityCode [in] �s�s�n�}�s���E��CITYCODE
	 * @param adminAddrCode [in] �s�s�n�}�s���E��ADDRCODE�A�܂���ADDRCODE2
	 * @param adminZipCode [in] CTYCODE�AADDRCODE�ɕR�Â��X�֔ԍ�
	 * @param adminGeometry [in] �s�s�n�}�s���E�̃W�I���g��
	 */
	void SetData(
		const CString& adminCityCode,
		const CString& adminAddrCode,
		const CString& adminZipCode,
		const IGeometryPtr& adminGeometry);

	/**
	 * @brief m_geometry�̃}���`�|���S������
	 * @retval true �}���`�|���S���ł���
	 * @retval false �}���`�|���S���łȂ�
	 */
	bool IsMultiPolygon();

	/**
	 * @brief �|���S��ID���Z�b�g����
	 * @param id [in] ID
	 */
	void SetPolygonId(long id);
	
	/**
	 * @brief �|���S���C���f�b�N�X���Z�b�g����
	 * @param index [in] �C���f�b�N�X
	 */
	void SetGeometryIndex(long index){ m_geometryIndex = index; };

	/**
	 * @brief �W�I���g�����Z�b�g����
	 * @param geometry [in] �W�I���g��
	 */
	void SetGeometry(const IGeometryPtr& geometry);

	/**
	 * @brief �|���S��ID���擾����
	 * @retval �|���S��ID(������)
	 */
	long GetPolygonId() const { return m_polygonId; }

	/**
	 * @brief �X�֔ԍ����擾����
	 * @retval �X�֔ԍ�
	 */
	CString GetZipCode() const { return m_zipcode; }

	/**
	 * @brief �s���{���R�[�h���擾����
	 * @retval �s���{���R�[�h
	 */
	CString GetPrefCode() const { return m_prefCode; }

	/**
	 * @brief �s�撬���R�[�h���擾����
	 * @retval �s�撬���R�[�h
	 */
	CString GetCityCode() const { return m_cityCode; }

	/**
	 * @brief �厚�R�[�h���擾����
	 * @retval �厚�R�[�h
	 */
	CString GetOazaCode() const { return m_oazaCode; }

	/**
	 * @brief ���R�[�h���擾����
	 * @retval ���R�[�h
	 */
	CString GetAzaCode() const { return m_azaCode; }

	/**
	 * @brief �W�I���g�����擾����
	 * @retval �W�I���g��
	 */
	IGeometryPtr GetGeometry() const { return m_geometry; }

	/**
	 * @brief �|���S���C���f�b�N�X���擾����
	 * @retval �|���S���C���f�b�N�X
	 */
	long GetGeometryIndex() const { return m_geometryIndex; }

private:
	/**
	 * @brief �ʐς����߂�
	 * @param geometry [in] �W�I���g��
	 * @retval �W�I���g���̖ʐ�
	 */
	double CalcArea(const IGeometryPtr& geometry);

private:
	long m_polygonId; //!< �|���S��ID
	CString m_zipcode; //!< �X�֔ԍ�
	CString m_prefCode; //!< �s���{���R�[�h
	CString m_cityCode; //!< �s�撬���R�[�h
	CString m_oazaCode; //!< �厚�R�[�h
	CString m_azaCode; //!< ���R�[�h
	IGeometryPtr m_geometry; //!< �|���S���`��
	long m_geometryIndex; //!< �}���`�|���S�����������ۂɐU����|���S���C���f�b�N�X
};

