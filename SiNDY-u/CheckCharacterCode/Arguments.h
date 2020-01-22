#pragma once

// �v���O��������
static const char* g_strFileListPath = "filelist";
static const char* g_strFileType = "filetype";
static const char* g_strCharCodeListPath = "charcodelist";
static const char* g_strExitMode = "exitmode";
static const char* g_strRunLogFilePath = "runlog";
static const char* g_strErrLogFilePath = "errlog";

static const CString g_strSHAPE = "shape";
static const CString g_strIMMEDIATE = "immediate";
static const CString g_strALL = "all";

/**
 * @brief �R�}���h���C�������N���X
 */
class Arguments
{
public:
	/**
	 * @brief �R���X�g���N�^
	 */
	Arguments(){};

	/**
	 * @brief �R�}���h���C�������̉��
	 * @param	argc [in]	�R�}���h���C�������̐�
	 * @param	argv [in]	�R�}���h���C�������̓��e
	 * @retval	true	����
	 * @retval	false	���s
	 */
	bool parse(int argc, TCHAR* argv[]);

	/**
	 * @brief �W���o�́A���s���O�ɏo�͂���I�v�V�����̈ꗗ���o�͂���
	 * @return ���͂��ꂽ�I�v�V�����̈ꗗ�̕�����
	 */
	CString Arguments::GetOption();

	/**
	 * @brief �I�v�V�����œ��͂��ꂽ�l�����Ȃ����`�F�b�N����
	 * @retval true  �I�v�V�����̒l�ɖ��Ȃ�
	 * @retval false �I�v�V�����̒l�ɖ�肠��
	 */
	bool Arguments::CheckOptionParameter();

private:
	/**
	 * @brief �W���o�́A���s���O�ɏo�͂���I�v�V�����Ƃ��̒l�̏��𐮌`����
	 * @param strParam [in] �I�v�V�����̃p�����[�^��
	 * @param strValue [in] �I�v�V�����ɑ΂��ē��͂��ꂽ�l
	 * @return �I�v�V�����Ƃ��̒l�𐮌`����������
	 */
	CString GetOptionLine(const CString& strParam, const CString& strValue);

public:
	uh::tstring	strFileListPath;		//!< ���̓t�@�C�����X�g�̃p�X
	uh::tstring strFileType;			//!< �`�F�b�N�Ώۂ̃t�@�C���^�C�v
	uh::tstring strCharCodeListPath;	//!< ���e�����R�[�h�͈͎w��t�@�C���̃p�X
	uh::tstring strExitMode;			//!< �s�������R�[�h���o���̏I�����[�h
	uh::tstring strRunLogFilePath;		//!< ���s���O�̃p�X
	uh::tstring strErrLogFilePath;		//!< �G���[���O�̃p�X
};
