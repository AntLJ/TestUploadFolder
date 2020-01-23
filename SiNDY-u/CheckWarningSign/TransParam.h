#pragma once
#include "common.h"
#include <TDC/useful_headers/boost_program_options.h>
#include <TDC/useful_headers/tstring.h>


using boost::program_options::option_description;
using boost::program_options::value;

namespace po = boost::program_options;

/**
* @brief �I�v�V�����Ǘ��N���X
*/
class TransParam
{
public:
	/**
	 * @brief �R���X�g���N�^
	 */
	TransParam(void);

	/**
	 * @brief �f�X�g���N�^
	 */
	~TransParam(void);

	/**
	 * @brief �I�v�V�����̊i�[
	 * @param argc [in] �����̐�
	 * @param argv [in] �������X�g
	 */
	bool parse(int argc, _TCHAR* argv[]);

	CString getRoadDB() const { return CString(m_roadDB.c_str()); };
	CString getRoadOwner() const { return CString(m_roadOwner.c_str()); };
	CString getRoadWhere() const { return CString(m_roadWhere.c_str()); };
	CString getWarningSignDB() const { return CString(m_warningSignDB.c_str()); };
	CString getWaringSignOwner() const { return CString(m_warningSignOwner.c_str()); };
	double  getBuffer() const { return m_buffer; };
	CString getOutputLog() const { return CString(m_outputLog.c_str()); };
	CString getRunLog() const { return CString(m_runLog.c_str()); };
	CString getErrLog() const { return CString(m_errLog.c_str()); };


	std::string getOptions();

private:
	/**
	 * @brief �I�v�V�����̊i�[
	 * @param vm [in] �I�v�V�����Q
	 * @param option [in] �I�v�V������
	 */
	bool optionExistChk(const po::variables_map& vm, LPCSTR option); 

	/**
	 * @brief �I�v�V�����̊i�[
	 * @param option [in] �I�v�V������
	 * @param content [in] ����
	 */
	std::string getOptionLine(LPCSTR option, uh::tstring content); 

private:
	uh::tstring m_roadDB;				// ���HDB
	uh::tstring m_roadOwner;			// ���HDB���[�U
	uh::tstring m_roadWhere;			// ���H�����N�̌�������
	uh::tstring m_warningSignDB;		// �x���W��DB
	uh::tstring m_warningSignOwner;		// �x���W��DB���[�U
	double m_buffer;					// �x���W���|�C���g�ɂ�����o�b�t�@��
	uh::tstring m_outputLog;			// �o�̓��O
	uh::tstring m_runLog;				// ���s���O
	uh::tstring m_errLog;				// �G���[���O

};

