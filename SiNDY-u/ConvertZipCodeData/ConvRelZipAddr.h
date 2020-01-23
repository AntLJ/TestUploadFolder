#pragma once
#include "ConvBase.h"
#include "Define.h"

/**
 * @brief �X�֔ԍ��ƏZ���R�t���e�[�u���̃��R�[�h�p�N���X
 */
class CRelZipAddr {
public:
	bool operator==(const CRelZipAddr& cRelZipAddr) const {
		return strPrefCode == cRelZipAddr.strPrefCode &&
			strCityCode == cRelZipAddr.strCityCode &&
			strOazaCode == cRelZipAddr.strOazaCode &&
			strAzaCode == cRelZipAddr.strAzaCode &&
			strChibanCode == cRelZipAddr.strChibanCode &&
			strJukyoCode == cRelZipAddr.strJukyoCode;
	}
	bool operator<(const CRelZipAddr& cRelZipAddr) const {
		return strPrefCode != cRelZipAddr.strPrefCode ? strPrefCode < cRelZipAddr.strPrefCode :
			strCityCode != cRelZipAddr.strCityCode ? strCityCode < cRelZipAddr.strCityCode :
			strOazaCode != cRelZipAddr.strOazaCode ? strOazaCode < cRelZipAddr.strOazaCode :
			strAzaCode != cRelZipAddr.strAzaCode ? strAzaCode < cRelZipAddr.strAzaCode :
			strChibanCode != cRelZipAddr.strChibanCode ? strChibanCode < cRelZipAddr.strChibanCode :
			strJukyoCode < cRelZipAddr.strJukyoCode;
	}
public:
	CString strPrefCode;
	CString strCityCode;
	CString strOazaCode;
	CString strAzaCode;
	CString strChibanCode;
	CString strJukyoCode;
};

/**
 * @brief �X�֔ԍ��ƏZ���R�t���e�[�u���̕ϊ��N���X
 */
class CConvRelZipAddr : public CConvBase
{
public:
	CConvRelZipAddr(const CString& strOutDir, const ITablePtr& ipRelAddr, const CString& strAddr11Dir){
		m_strOutDir = strOutDir;
		m_ipRelAddr = ipRelAddr;
		m_strAddr11Dir = strAddr11Dir;
		m_bFeatureClass = false;
		m_strShapefileName = zip_pol::schema::rel_zip_addr::kTableName;
	}
	~CConvRelZipAddr(){
	}

public:
	
	std::map<CString,std::set<CRelZipAddr>>& getZipAddr()
	{
		return m_mapZipAddr;
	}
	void clearData(){
		m_mapZipAddr.clear();
		m_setAddr11Acc2.clear();
	}

private:
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
	 * @param mapZipAddr [in] �f�[�^�C���|�[�g�p�ɂɉ��H�����f�[�^
	 * @retval true ��������
	 * @retval false �������s
	 */
	bool outputData() override;

private:
	/**
	 * @brief 11���Z���f�[�^�iaddr??.txt�j���琸�x2��11���Z���R�[�h�擾
	 * @retval true ��������
	 * @retval false �������s
	 */
	bool loadAddr11Acc2();

private:
	ITablePtr m_ipRelAddr;				//!< �X�֔ԍ��ɕR�t���Z���R�[�h�i�ϊ����e�[�u���j
	CString m_strAddr11Dir;				//!< 11���Z���f�[�^�i�[�f�B���N�g��
	std::set<CString> m_setAddr11Acc2;	//!< ���x2��11���Z���R�[�h�iADR003�����j
	std::map<CString,long> m_mapFieldIndexRelAddr;	//!< �ϊ����iSDE,���v���J�j�e�[�u���̃t�B�[���h�C���f�b�N�X

	std::map<CString,std::set<CRelZipAddr>> m_mapZipAddr;	//!< �X�֔ԍ��Ɗ֘A����20���R�[�h�̃}�b�v
};

