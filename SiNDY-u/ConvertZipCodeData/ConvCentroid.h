#pragma once
#include "ConvBase.h"
#include "Define.h"

/**
 * @brief �d�S�|�C���g�ϊ��p�N���X
 */
class CConvCentroid : public CConvBase
{
public:
	CConvCentroid(const CString& strOutDir){
		m_strOutDir = strOutDir;
		m_bFeatureClass = true;
		m_strShapefileName = zip_pol::schema::zip_centroid::kTableName;
	}
	~CConvCentroid(){
		m_mapMergedZipGeom.clear();
		m_mapZipCentorid.clear();
	}

public:

	/**
	 * @brief �}�[�W��`��ݒ�
	 */
	void setMergedZipGeom(const std::map<CString,IGeometryPtr>& mapMergedZipGeom)
	{
		m_mapMergedZipGeom = mapMergedZipGeom;
	}
	void clearData(){
		m_mapMergedZipGeom.clear();
		m_mapZipCentorid.clear();
	}

private:
	/**
	 * @brief ����ϊ��i���z�֐��j
	 * @retval true ��������
	 * @retval false �������s
	 */
	bool convSpecial() override;

	/** 
	 * @brief �t�B�[���h�C���f�b�N�X�쐬
	 * @retval true ��������
	 * @retval false �������s
	 */
	bool createFieldIndex() override;

	/** 
	 * @brief �o�͐�Shapefile�̃t�B�[���h�ݒ�
	 * @param ipFields [out] �t�B�[���h�Q
	 * @retval true ��������
	 * @retval false �������s
	 */
	bool createFields(IFieldsPtr& ipFields) override;

	/**
	 * @brief �ϊ����e�[�u������f�[�^�ǂݍ���
	 * @retval true ��������
	 * @retval false �������s
	 */
	bool loadData() override;

	/**
	 * @brief Shapefile�փf�[�^�o��
	 * @retval true ��������
	 * @retval false �������s
	 */
	bool outputData() override;

private:
	/**
	 * @brief �d�S�|�C���g�쐬
	 * @param mapZipGeom [out] �ǂݍ��񂾃f�[�^��X�֔ԍ���OID+�`��Ń}�b�v��������
	 * @retval true ��������
	 * @retval false �������s
	 */
	bool createCentroid(const std::map<CString,IGeometryPtr>& mapMergedZipGeom);

private:
	std::map<CString,IGeometryPtr> m_mapMergedZipGeom;	//!< �X�֔ԍ��ƃ}�[�W���ꂽ�`��̃}�b�v
	std::map<CString,IPointPtr> m_mapZipCentorid;		//!< �X�֔ԍ��Əd�S�|�C���g�̃}�b�v
};
