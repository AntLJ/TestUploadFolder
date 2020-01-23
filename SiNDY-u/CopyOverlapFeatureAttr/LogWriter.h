#pragma once

#include "stdafx.h"
#include <boost/program_options.hpp>
#include "Arguments.h"

// �G���[�R�[�h
namespace err_code {
	enum ECode {
		errOverBuffer                    = 1001,     //!< ��r��̃I�u�W�F�N�g����r���̌����͈͂��͂ݏo�������߁A�X�L�b�v
		errSrcNotExist                   = 1002,     //!< �d�Ȃ��Ă����r�������݂��Ȃ����߁A�X�L�b�v
		errOverlaySrcNotExist            = 1003,     //!< �d������臒l�𖞂�����r�������݂��Ȃ����߁A�X�L�b�v
		errScaleSrcNotExist              = 1004,     //!< �g�k����臒l�𖞂�����r�������݂��Ȃ����߁A�X�L�b�v
		errBothSrcNotExist               = 1005,     //!< �d�����Ɗg�k���𗼕��Ƃ���������r�������݂��Ȃ����߁A�X�L�b�v
		errMulitSrcSkipMode              = 1006,     //!< �ł��d�����������t�B�[�`����2�ȏ゠��A�X�L�b�v���[�h�̂��߁A�X�L�b�v
		errMulitSrcAttrNoMatch           = 1007,     //!< �ł��d�����������t�B�[�`����2�ȏ゠��A��v���胂�[�h�Ŕ�r�����m�̑�������v���Ă��Ȃ����߁A�X�L�b�v
		errAttrNotInAttrList             = 1008,     //!< �t�B�[�`���[�̑����l���ϊ��\�ɑ��݂��Ă��Ȃ����߁A�X�L�b�v
		errNotSameField                  = 1009,     //!< ��r��Ɣ�r���̃t�B�[���h�̌^����v���Ă��Ȃ����߁A�X�L�b�v
		errPutValueFailed                = 1010,     //!< ��r��̃t�B�[���h�l�ւ̏������݂����s�������߁A�X�L�b�v
		errSuccessUpdate                 = 1011,     //!< ����X�V
		errUpdateFailed                  = 1012,     //!< �����̍X�V�Ɏ��s
		errFailureOverlay                = 1013,     //!< �d������臒l�𖞂����Ȃ����߁A�X�L�b�v
		errFailureScale                  = 1014,     //!< �g�k����臒l�𖞂����Ȃ����߁A�X�L�b�v
		errNotSameSpRef                  = 1015,     //!< ��r��Ɣ�r���̑��n�n����v���Ă��Ȃ�
		errNone                          = 0         //!< �Ȃ�
	};
}; // err_code

// �G���[���x��
namespace err_level {
	enum ECode {
		levelError                      = 1001,     //!< ERROR LEVEL
		levelWarning                    = 1002,     //!< WARNING LEVEL
		levelInfo                       = 1003     //!< INFO LEVEL
	};
}; // err_code

// �G���[�o�͏��
struct ErrItem
{
	long          nFlag;                  //!< �Ή��󋵁F�f�t�H���g0
	CString       strLayer;               //!< ��r��̃��C����
	long          nOID;                   //!< ��r���OID
	double        dLon;                   //!< �o�x�i�󕶎���j
	double        dLat;                   //!< �ܓx�i�󕶎���j
	CString       strErrLevel;            //!< �G���[���x��
	long          nErrCode;               //!< �G���[�R�[�h
	CString       strErrMsg;              //!< �G���[���b�Z�[�W
	unsigned long nMeshCode;              //!< ���b�V���R�[�h
	long          nSrcOID;                //!< �d�Ȃ�����r����OID�i���݂��Ȃ��ꍇ��-1�A�������݂̏ꍇ�̓J���}��؂�ŏo�́j
	CString       strNote;                //!< ���l

	// �R���X�g���N�^
	ErrItem() :
		nFlag(0),
		nOID(-1),
		dLon(0.0),
		dLat(0.0),
		nErrCode(0),
		nMeshCode(0),
		nSrcOID(-1)
	{}

	virtual ~ErrItem()
	{
	}
};

/**
 * @brief ���O�������݂̂��߂̊��N���X
 */
class LogWriter
{
public:
	/**
	 * @brief  �R���X�g���N�^
	 */
	LogWriter();

	/**
	 * @brief  �f�X�g���N�^
	 */
	virtual ~LogWriter(void);

public:
	/**
	 * @brief  ���O�t�@�C���X�g���[�����m�ۂ���
	 * @param  runLogFileName [in] ���s���O�t�@�C����
	 * @param  errLogFileName [in] �G���[���O�t�@�C����
	 * @param  variable_map [in] �v���O�����I�v�V�����i�[�}�b�v
	 * @retval true �X�g���[���m�ې���
	 * @retval false �X�g���[���m�ێ��s
	 */
	bool openLogFiles(const CString& runLogFileName, const CString& errLogFileName, const Arguments& variable_map);

protected:
	/**
	 * @brief  ���s���O�t�@�C���Ƀ��O����������
	 * @note   �W���o�͂���K�v�̂Ȃ����O�̏ꍇ�ɂ́A��2������false�ɂ��邱��
	 * @param  message  [in] �������ރ��b�Z�[�W
	 * @param  putTime  [in] ���b�Z�[�W�Ɏ��Ԃ�ǋL���邩(�f�t�H���g:true)
	 * @param  disp   [in] �W���o�͂����邩�ǂ����i�f�t�H���g:true�j
	 */
	void writeRunLog(LPCTSTR message, bool putTime = true, bool disp = true);

	/**
	 * @brief  �G���[���O�t�@�C���Ƀ��O����������
	 * @note   �W���G���[�o�͂���K�v�̂Ȃ����O�̏ꍇ�ɂ́A��2������false�ɂ��邱��
	 * @param  message  [in] �������ރ��b�Z�[�W
	 * @param  disp   [in] �W���o�͂����邩�ǂ����i�f�t�H���g:true�j
	 */
	void writeErrLog(LPCTSTR message, bool disp = true);

	/**
	 * @brief  �G���[���O�t�@�C���Ƀ��O����������
	 * @note   �W���G���[�o�͂���K�v�̂Ȃ����O�̏ꍇ�ɂ́A��2������false�ɂ��邱��
	 * @param  cErrItem  [in] �������ރ��b�Z�[�W�̍\����
	 * @param  disp   [in] �W���o�͂����邩�ǂ����i�f�t�H���g:true�j
	 */
	void writeErrLog(ErrItem cErrItem, bool disp = true);

	/**
	 * @brief  ���O�t�@�C�����N���[�Y����
	 * @note ���s���O/�G���[���O�̑o�����N���[�Y����
	 */
	void closeLogFiles();

	/**
	 * @brief  ���O�t�@�C���Ƀv���O�����J�n�Ƃ��ăc�[�����A�I�v�V�����������o�͂���
	 * @param  args [in] �I�v�V��������
	 */
	void startLogFile(const Arguments& args );

	/**
	 * @brief  ���s���O�t�@�C���Ƀv���O�����I�������Ə������ʂ��o�͂���
	 */
	void endLogFile();

	/**
	 * @brief  ���s���O�t�@�C���Ƀc�[�����ƃo�[�W�������o�͂���
	 */
	void writeFileVersion();

	/**
	 * @brief  ���ݎ������擾����
	 * @return ���ݎ���������������
	 */
	uh::tstring getNowTime() const;

	/**
	 * @brief �G���[���b�Z�[�W������擾
	 * @param errCode [in] �G���[�R�[�h
	 * @return �G���[�R�[�h�ɑΉ�����G���[���b�Z�[�W������
	 */
	static CString getErrCodeMsg(err_code::ECode errCode);

	/**
	 * @brief �G���[���x��������擾
	 * @param errLevel [in] ���x���R�[�h
	 * @return ���x���R�[�h�ɑΉ�����G���[���x��������
	 */
	static CString getErrLevelMsg(err_level::ECode errLevel);

private:
	time_t m_startTime;              //!< �ϊ��X�^�[�g����

	std::wofstream m_runLogStream;   //!< ���s���O�o�͗p�t�@�C���X�g���[��
	std::wofstream m_errLogStream;   //!< �G���[���O�o�͗p�t�@�C���X�g���[��
};
