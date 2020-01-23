#pragma once
#include "ConvBase.h"
#include "Define.h"

/**
 * @brief �X�֔ԍ��|���S���ϊ��N���X
 */
class CConvZipPol : public CConvBase
{
public:
	CConvZipPol(const CString& strOutDir, const IFeatureClassPtr ipZipAreaClass){
		m_strOutDir = strOutDir;
		m_ipZipAreaClass = ipZipAreaClass;
		m_bFeatureClass = true;
		m_strShapefileName = zip_pol::schema::zip_pol::kTableName;
	};
	~CConvZipPol(){
	};

public:

	/**
	 * @brief �}�[�W��`��擾
	 * @return �}�[�W��`��
	 */
	std::map<CString,IGeometryPtr>& getMergedZipGeom()
	{
		return m_mapMergedZipGeom;
	}
	void clearData(){
		m_mapZipGeom.clear();
		m_mapMergedZipGeom.clear();
		m_mapZipPriority.clear();
	}

private:
	/**
	 * @brief ����ϊ�
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
	 * @brief �����`��ō\�������X�֔ԍ��̃}�[�W����
	 * @param mapZipGeom [in] �X�֔ԍ���OID+�`��̃}�b�v
	 * @param mapMergedZipGeom [out] �X�֔ԍ��ƃ}�[�W���ꂽ�`��̃}�b�v
	 * @retval true ��������
	 * @retval false �������s
	 */
	bool mergeZipGeom(std::map<CString,std::map<long,IGeometryPtr>>& mapZipGeom, std::map<CString,IGeometryPtr>& mapMergedZipGeom);

	/**
	 * @brief �X�֔ԍ��ɑ΂���D��x�ݒ肳�ꂽ���X�g�擾
	 * @param mapZipPriority [out] �X�֔ԍ��ƗD��x�̃}�b�v
	 * @retval true  ��������
	 * @retval false �������s
	 */
	bool createZipCodePriority(std::map<CString,long>& mapZipPriority);

	/**
	 * @brief �D��x�ݒ�p�̃��X�g�擾
	 * @param mapZipPriority [out] �X�֔ԍ��ƗD��x�̃}�b�v
	 * @param mapOverlapZip [out] �d�����Ă���X�֔ԍ��̃}�b�v�i�L�[���D��x���A�v�f���D��x��̗X�֔ԍ��j
	 * @retval true  ��������
	 * @retval false �������s
	 */
	bool getZipCodeList(std::map<CString,long>& mapZipPriority, std::map<CString,std::set<CString>>& mapOverlapZip);

	/**
	 * @brief �X�֔ԍ��ɑ΂���D��x�̐ݒ�
	 * @param mapZipPriority [in,out] �X�֔ԍ��ƗD��x�̃}�b�v
	 * @param mapOverlapZip [in] �d�����Ă���X�֔ԍ��̃}�b�v�i�L�[���D��x���A�v�f���D��x��̗X�֔ԍ��j
	 * @retval true  ��������
	 * @retval false �������s
	 */
	bool updatePriority(std::map<CString,long>& mapZipPriority, const std::map<CString,std::set<CString>>& mapOverlapZip);

	/**
	 * @brief �X�֔ԍ��ɑ΂���D��x�̐ݒ�i�ċA�j
	 * @param mapZipPriority [in,out] �X�֔ԍ��ƗD��x�̃}�b�v
	 * @param mapOverlapZip [in] �d�����Ă���X�֔ԍ��̃}�b�v�i�L�[���D��x���A�v�f���D��x��̗X�֔ԍ��j
	 * @param strZip [in] �`�F�b�N�Ώۂ̗X�֔ԍ�
	 * @param setZip [in] strZip���L�[�Ƃ���mapOverlapZip�̗v�f
	 * @param setCheckedZip [in,out] ���Ƀ`�F�b�N�ς݂̗X�֔ԍ��i�z�Ď��p�j
	 * @retval true  ��������
	 * @retval false �������s
	 */
	bool setPriority(std::map<CString,long>& mapZipPriority, const std::map<CString,std::set<CString>>& mapOverlapZip, const CString &strZip, const std::set<CString>& setZip, std::set<CString> setCheckedZip);

private:
	IFeatureClassPtr m_ipZipAreaClass;				//!< �ϊ����̗X�֔ԍ��|���S��
	std::map<CString,long> m_mapFieldIndexZipArea;	//!< �ϊ����iSDE,���v���J�j�e�[�u���̃t�B�[���h�C���f�b�N�X

	std::map<CString,std::map<long,IGeometryPtr>> m_mapZipGeom;	//!< �X�֔ԍ���OID�{�`��̃}�b�v
	std::map<CString,IGeometryPtr> m_mapMergedZipGeom;			//!< �X�֔ԍ��ƃ}�[�W���ꂽ�`��̃}�b�v
	std::map<CString,long> m_mapZipPriority;		//!< �X�֔ԍ��ƗD��x�̃}�b�v
};
