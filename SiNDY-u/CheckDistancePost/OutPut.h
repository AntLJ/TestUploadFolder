#pragma once

/**
* @brief   �o�͏��擾�N���X [bug 12017]
*/
class Output
{
public:
	/**
	* @brief �R���X�g���N�^
	*/
	Output() {}

	/**
	* @brief �f�X�g���N�^
	*/
	virtual ~Output() {}

	/**
	* @brief ���ݎ����擾
	* @param lpSystemTime [in] �V�X�e���^�C��
	* @return CString ���ݎ���������
	*/
	CString GetNow(LPSYSTEMTIME lpSystemTime = nullptr) const;

	/**
	* @brief �o�[�W�������擾
	* @return pair<CString,CString> �o�[�W�������i�v���_�N�g�A�t�@�C���j
	*/
	std::pair<CString, CString> GetVersion();

	/**
	* @brief ���s�t�@�C�����擾
	* @return CString �t�@�C����
	*/
	CString GetModuleName() const;

	/**
	* @brief ���O�w�b�_�쐬
	* @param lpcszOpParam [in] �I�v�V���������ꗗ
	* @return CString ���O�w�b�_
	*/
	CString CreateLogHeader(LPCTSTR lpcszOpParam);
};