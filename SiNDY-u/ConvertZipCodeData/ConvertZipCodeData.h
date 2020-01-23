#pragma once
#include "Arguments.h"

/**
 * @brief �X�֔ԍ��f�[�^�̕ϊ����N���X
 */
class CConvertZipCodeData
{
public:
	CConvertZipCodeData(){}
	~CConvertZipCodeData(){
	}

public:
	/**
	 * @brief ������
	 * @param args [in] �R�}���h���C������
	 * @retval true  ��������
	 * @retval false �������s
	 */
	bool init(const Arguments& args);

	/**
	 * @brief �������s
	 * @retval true  ��������
	 * @retval false �������s
	 */
	bool run();

private:
	CString m_strOutDir;				//!< �o�͐�f�B���N�g��
	IFeatureClassPtr m_ipZipAreaClass;	//!< �ϊ����̗X�֔ԍ��|���S���N���X
	ITablePtr m_ipRelAddr;				//!< �ϊ����̗X�֔ԍ��ƏZ���R�t���e�[�u��
	CString m_strAddrDB;				//!< �Z��DB�ڑ����
	CString m_strAddr11Dir;				//!< 11�Z���f�[�^�i�[��
	CADOBase m_cADOBase;				//!< �Z��DB�R�l�N�V����
};

