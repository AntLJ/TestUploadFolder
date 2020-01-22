#pragma once

#include "stdafx.h"
#include "TsvWriter.h"

/**
 * @brief ������|�󃊃X�g�t�@�C�����t�H�[�}�b�g�i�t�H���_���݁j
 */
const CString FILENAME_FORMAT = _T("%s\\%s_%.4d.txt");

/**
 * @brief ������|�󃊃X�g�����N���X�i�h���N���X�j
 */
class MultiTranslateListWriter : public TsvWriter
{
public:
	/**
	 * @brief �R���X�g���N�^
	 * @param[in] outputDir       �t�@�C���o�͐�t�H���_
	 * @param[in] maxRecNum       �ő发�����R�[�h��
	 * @param[in] grpcode         �f�[�^���ރR�[�h
	 * @param[in] addColumnList   �ǉ��J�������X�g
	 */
	MultiTranslateListWriter(
				const CString& outputDir,
				int maxRecNum,
				const CString& grpcode,
				const std::vector<CString>& addColumnList);

	/**
	 * @brief �f�X�g���N�^
	 */
	virtual ~MultiTranslateListWriter() {}

	/**
	 * @brief �t�@�C���I�[�v��
	 * @retval true ����
	 * @retval false ���s
	 */
	bool open();

	/**
	 * @brief �t�@�C���ւ̏�������
	 * @param[in] writeStrList �������ޕ�����̃��X�g
	 * @retval true ����
	 * @retval false ���s
	 */
	bool writeRecord( const std::vector<CString>& writeStrList );

private:

	CString m_outputDir;                   //!< �t�@�C���o�͐�t�H���_
	CString m_grpcode;                     //!< �f�[�^���ރR�[�h
	int m_maxRecNum;                       //!< �t�@�C�����ő发���݃��R�[�h��
	int m_writeRecCounter;                 //!< �����݌����J�E���^
	int m_fileSepCounter;                  //!< �t�@�C�������J�E���^
	std::vector<CString> m_headerList;     //!< ������|�󃊃X�g�w�b�_�ꗗ
};

