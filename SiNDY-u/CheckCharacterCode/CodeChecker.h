#pragma once

#include "Arguments.h"
#include "CharacterCode.h"
#include <sstream>

/**
 * @brief �����R�[�h�`�F�b�N���N���X
 */
class CodeChecker
{
public:
	/**
	 * @brief �R���X�g���N�^
	 */
	CodeChecker(void){};

	/**
	 * @brief �f�X�g���N�^
	 */
	virtual ~CodeChecker(void){};

	/**
	 * @brief ���̓t�@�C�����X�g��ǂݍ���
	 * @param strFileListPath [in] ���̓t�@�C�����X�g�̃p�X
	 * @retval true ��������
	 * @retval false �������s
	 */
	bool SetTargetFileList(const CString& strFileListPath);

	/**
	 * @brief ���e�����R�[�h�͈̓��X�g��ǂݍ���
	 * @param strCharCodeListPath [in] �����R�[�h���X�g�̃p�X
	 * @retval true ��������
	 * @retval false �������s
	 */
	bool SetCharacterCodeList(const CString& strCharCodeListPath);

	/**
	 * @brief �����R�[�h�`�F�b�N�Ώۃt�@�C����ǂݍ���ŕ����R�[�h�`�F�b�N���s��
	 * @param isImmediateExit [in] �s�����o�������I���t���O
	 * @retval true ��������
	 * @retval false �������s
	 */
	virtual bool CheckCharacterCode(bool isImmediateExit) = 0;

protected:
	/**
	 * @brief TSV���R�[�h��񂲂Ƃ̒l�ɕ������Ċi�[����
	 * @param strTSVRecord [in] �C�ӂ̒�����TSV���R�[�h1�s
	 * @return TSV���R�[�h��񂲂Ƃɕ������Ċi�[����Vector
	 */
	std::vector<std::string> GetTSVParameter(std::string strTSVRecord);

	/**
	 * @brief �Ώە����񂪋��e����镶���񂩂ǂ���
	 * @param chkStr [in] �t�@�C������ǂݍ��񂾕�����
	 * @retval true ���e����镶���񂾂���
	 * @retval false ���e����Ȃ������񂾂���
	 */
	bool IsParmitted(const std::string& chkStr);

	/**
	 * @brief �����R�[�h�̃o�C�g���16�i���ɕϊ�����i��F0xE5��"E5"�j
	 * @param org_str [in] �����R�[�h�̃o�C�g��
	 * @return �����R�[�h�̃o�C�g���16�i���̕�����Ƃ��ĕϊ�����������
	 */
	std::string ToHex(const std::string& org_str);

protected:
	std::vector<CString> m_vecTargetFileList;		//!< �����R�[�h�`�F�b�N�Ώۃt�@�C���̃��X�g
	std::vector<CharacterCode> m_vecCharCodeList;	//!< ���e���镶���R�[�h�͈͂̃��X�g
	static const int m_intCharCodeMin = 0;			//!< �����R�[�h�͈̓t�@�C����1���
	static const int m_intCharCodeMax = 1;			//!< �����R�[�h�͈̓t�@�C����2���
};
