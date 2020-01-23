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

#include <codecvt>

class OutputFile
{
public:
	OutputFile(LPCTSTR outputDir)
	 :m_inited(false) 
	{
		m_roadNamePath.Format(_T("%s\\%s.txt"), outputDir, ROAD_NAME);
		m_relRoadNameLinkPath.Format(_T("%s\\%s.txt"), outputDir, REL_ROAD_NAME_LINK);
		m_spltRoadNamePath.Format(_T("%s\\%s.txt"), outputDir, SPLIT_ROAD_NAME);
	}
	~OutputFile() {}

public:
	/**
	 * @brief ������
	 * @retval true ����
	 * @retval false ���s
	 */
	bool init();

	/**
	 * @brief �t�@�C���o��
	 * @param rdNameRecs	[in] ���H���̃f�[�^���R�[�h�ϊ�����
	 * @param rlNameLnkRecs [in] ���H�����N�ɕR�Â����H���̃��R�[�h�ϊ�����
	 * @param spltRdNameRecs [in] ���H���̕����f�[�^���R�[�h�ϊ�����
	 * @retval true ����
	 * @retval false ���s
	 */
	bool write(const std::set<RoadNameRecord>& rdNameRecs, const std::set<RelNameLinkRecord>& rlNameLnkRecs, const std::set<SpltNameRecord>& spltRdNameRecs);

private:
	/**
	 * @brief �t�@�C���폜
	 * @note �t�@�C�������݂���ꍇ�̂ݍ폜
	 * @param filePath [in]
	 * @retval true ����
	 * @retval false ���s
	 */
	bool deleteFile(LPCTSTR filePath);

	/**
	 * @brief ���H���̃t�@�C���o��
	 * @param rdNameRecs [in] ���H���̃f�[�^���R�[�h
	 * @retval true ����
	 * @retval false ���s
	 */
	bool writeRoadName(const std::set<RoadNameRecord>& rdNameRecs);

	/**
	 * @brief ���H�����N�ɕR�Â����H�o��
	 * @param rlNameLnkRecs [in] ���H�����N�ɕR�Â����H���R�[�h
	 * @retval true ����
	 * @retval false ���s
	 */
	bool writeRelNameLink(const std::set<RelNameLinkRecord>& rlNameLnkRecs);

	/**
	 * @brief ���H���̕����f�[�^�o��
	 * @param spltRdNameRecs [in] ���H���̕����f�[�^���R�[�h
	 * @retval true ����
	 * @retval false ���s
	 */
	bool writeSplitRoadName(const std::set<SpltNameRecord>& spltRdNameRecs);

	/**
	 * @brief UTF-16��UTF-8�ϊ�
	 * @param utf16Str [in] UTF16������
	 * @return UTF8������
	 */
	inline CStringA UTF16toUTF8(const CStringW& utf16Str)
	{
		// ���C�h������ƃo�C�g������𑊌ݕϊ�����N���X��������
		std::wstring_convert<std::codecvt_utf8<wchar_t>, wchar_t> conv;

		// UTF-16��UTF-8�ϊ�
		std::string utf8Str = conv.to_bytes(utf16Str);

		return utf8Str.c_str();
	}

private:
	bool m_inited;

	CString m_roadNamePath;
	CString m_relRoadNameLinkPath;
	CString m_spltRoadNamePath;
	

};

