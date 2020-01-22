#pragma once

#include "stdafx.h"

/**
 * @brief ���s���[�h
 */
enum Encoding
{
	Euc,          // EUC
	UTF8,         // UTF8�iBOM�Ȃ��j
	UTF8WithBom   // UTF8�iBOM�t���j
};

/**
 * @brief ���s�R�[�h
 */
enum LineFeed
{
	None,   // �Ȃ�
	CrLf,   // CR+LF
	Lf      // LF
};

/**
 * @brief ���s�R�[�h��`
 */
const CString LINE_FEED_CODE_CRLF = _T("\r\n");
const CString LINE_FEED_CODE_LF = _T("\n");

/**
 * @brief UTF-8��BOM�l��`
 */
const unsigned char BOM[3] = { 0xEF, 0xBB, 0xBF };

/**
 * @brief TSV�t�@�C�������N���X
 */
class TsvWriter
{
public:

	/**
	 * @brief �R���X�g���N�^
	 */
	TsvWriter() {}

	/**
	 * @brief �f�X�g���N�^
	 */
	virtual ~TsvWriter() {}

	/**
	 * @brief �t�@�C���I�[�v��
	 * @param file     [in] �t�@�C���p�X
	 * @param encoding [in] �����G���R�[�f�B���O
	 * @param lineFeed [in] ���s�R�[�h
	 * @retval true ����
	 * @retval false ���s
	 */
	virtual bool open( const CString& file, Encoding encoding, LineFeed lineFeed );

	/**
	 * @brief �t�@�C���N���[�Y
	 * @retval true ����
	 * @retval false ���s
	 */
	void close() { if( m_ofs.is_open() ) m_ofs.close(); };


	/**
	 * @brief �t�@�C���ւ̏�������
	 * @param writeStrList [in] �������ޕ�����̃��X�g
	 * @retval true ����
	 * @retval false ���s
	 */
	virtual bool writeRecord( const std::vector<CString>& writeStrList );

	/**
	 * @brief �t�@�C���ւ̏�������
	 * @param writeStr [in] �������ޕ�����
	 * @retval true ����
	 * @retval false ���s
	 */
	virtual bool writeRecord( const CString& writeStr );

protected:

	std::ofstream m_ofs; //!< �o�̓t�@�C���X�g���[��
	Encoding m_encoding; //!< �G���R�[�f�B���O
	LineFeed m_lineFeed; //!< ���s�R�[�h
};

