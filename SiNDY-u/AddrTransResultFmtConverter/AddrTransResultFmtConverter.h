/*
 * Copyright (C) INCREMENT P CORP. All Rights Reserved.
 *
 * THIS SOFTWARE IS PROVIDED BY INCREMENT P CORP., WITHOUT WARRANTY OF
 * ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE
 * WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE
 * AND NONINFRINGEMENT.
 *
 * IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDER BE LIABLE FOR ANY
 * CLAIM, DAMAGES SUFFERED BY LICENSEE AS A RESULT OF USING, MODIFYING
 * OR DISTRIBUTING THIS SOFTWARE OR ITS DERIVATIVES.
*/

#pragma once

#include "Argument.h"

/**
 * @brief   �������struct
 */
struct CombinePairs
{
	int firstColumn;
	int secondColumn;
	CombinePairs():firstColumn(0),secondColumn(0){}
	CombinePairs(int inputFstClm, int inputSndClm):firstColumn(inputFstClm),secondColumn(inputSndClm){}
};

/**
 * @brief   �t�@�C�����C����struct
 */
struct GroupLines
{
	map<int, wstring>   mapGroupLines;
	int                 lineNum;
	GroupLines():lineNum(0){}
	GroupLines(int inputLineNum):lineNum(inputLineNum){}
};

/**
 * @class  AddrTransResultFmtConverter
 * @brief  ���C�����[�`���N���X
 */
class AddrTransResultFmtConverter
{
public:
	/**
	* @brief   ������
	* @param   [in] argc �R�}���h���C�������̐�
	* @param   [in] argv �R�}���h���C�������̓��e
	* @retval  true:���� false:���s
	*/
	bool init( int argc, TCHAR** argv );

	/**
	* @brief   ���s
	* @retval  true:���� false:���s
	*/
	bool run();

	/**
	 * @brief   city.txt�t�@�C���̏���
	 */
	void convertCity();

	/**
	 * @brief   addr[**].txt�t�@�C���̏���
	 */
	void convertAddr();

	/**
	* @brief    aza[**].txt�t�@�C���̍쐬
	* @param    [in] pFilePath          ���̓t�@�C���p�X
	* @retval   true:����  false:���s
	*/
	bool createAzaFile(const wstring& pFilePath);

	/**
	* @brief    oaza[**].txt�t�@�C���̍쐬
	* @param    [in] pFilePath          ���̓t�@�C���p�X
	* @retval   true:����  false:���s
	*/
	bool createOazaFile(const wstring& pFilePath);

	/**
	* @brief    �t�@�C���s�̕���
	* @param    [in] str                �t�@�C���̍s
	* @param    [in] delim              ��������
	*/
	void split(const std::wstring& str, const std::wstring& delim);

	/**
	* @brief    ���ʃt�@�C���̏�������
	* @param    [in] ret                �o�͓��e
	*/
	void writeResultFile(map<int, wstring>& ret);

	/**
	* @brief    �t�@�C�����̂̎擾
	* @param    [in] path               �t�@�C���̃p�X
	*/
	void getFileNames(const wchar_t* path);

	/**
	* @brief    run.log�̍s�̐���
	* @param    [in] inFileName         ���̓t�@�C������
	* @param    [in] outFileName        �o�̓t�@�C������
	* @param    [in] inRecordNum        ���̓��R�[�h��
	* @param    [in] outRecordNum       �o�̓��R�[�h��
	* @param    [in] bNormal            �����̒��ŃG���[�����邩
	* @retval   �}�[�W����������
	*/
	wstring mergeRunLogLine(const wstring& inFileName, const wstring& outFileName, int inRecordNum, int outRecordNum, bool bNormal = true);

	/**
	* @brief    err.log�̍s�̐���
	* @param    [in] sFileName          ���t�@�C������
	* @param    [in] lineNum            �s��
	* @param    [in] errMsg             �G���[���b�Z�[�W
	* @param    [in] info1              ���1
	* @param    [in] info2              ���2
	* @param    [in] info3              ���3
	* @retval   �}�[�W����������
	*/
	wstring mergeErrLogLine(const wstring& sFileName, int lineNum, const wstring& errMsg, const wstring& info1, const wstring& info2, const wstring& info3 = L"");

	/**
	* @brief    �o�̓t�@�C���̓��v
	*/
	void outputFileStat();

	/**
	* @brief    ���s���O�ƃG���[���O�̃��b�Z�[�W
	*/
	void writeInformation();

	/**
	* @brief    ���ԋL�^
	* @param    [in] phaseName          �J�n���Ԃ܂��͊�������
	* @retval   ���ԏ��
	*/
	wstring timeMemo(const wstring& phaseName);

private:
	Argument                            m_argument;              //!< ����
	FileController                      m_errLogger;             //!< �G���[���O�t�@�C��
	FileController                      m_runLogger;             //!< ���s���O�t�@�C��
	FileController                      m_sourceFile;            //!< ���̓t�@�C��
	FileController                      m_resultFile;            //!< �o�̓t�@�C��

	map<wstring, int>                   m_mapFileHeadStr2Int;    //!< city.txt�t�@�C���̃w�b�_
	map<int, wstring>                   m_mapFileLine;           //!< �t�@�C���̃��C��<key,value>(���index,��̓��e)
	vector<wstring>                     m_vecNameYomi2;          //!< �u����2�v�܂��́u�ǂ�2�v���܂ޗ�

	vector<wstring>                     m_vecAddressFileName;    //!< addr[**].txt�t�@�C����(path�܂�)
	map<int, CombinePairs>              m_mapCombineColumnIdxs;  //!< ������̍���
	map<wstring, map<int, wstring>>     m_mapOazaFile;           //!< oaza[**].txt�̃R���e�i
	map<wstring, vector<GroupLines>>    m_mapErrCheck;           //!< addr[**].txt��oaza[**].txt�����̕���
	vector<wstring>                     m_vecRunInfo;            //!< ���s���b�Z�[�W�̃R���e�i
};