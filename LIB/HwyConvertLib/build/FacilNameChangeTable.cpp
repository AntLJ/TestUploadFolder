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

#include "FacilNameChangeTable.h"
#include <fstream>
#include <iostream>
#include <boost/lexical_cast.hpp>
#include <boost/tokenizer.hpp>

FacilNameChangeTable::FacilNameChangeTable(void)
{
}

FacilNameChangeTable::FacilNameChangeTable(std::string cFilePath, bool cEnable) :
m_EnabledToChange(cEnable)
{
	read(cFilePath);
}


FacilNameChangeTable::~FacilNameChangeTable(void)
{
}

/**
 * @brief �{�ݖ��̕ϊ��e�[�u���t�@�C���̓ǂݍ���
 * @param	cFilePath	[in]	���̓t�@�C���p�X
 **/
void FacilNameChangeTable::read(std::string cFilePath)
{
	using namespace std;
	using namespace boost;

	if (! m_EnabledToChange) {
		return;
	}

	ifstream aFin(cFilePath.c_str());
	if (!aFin)
	{
		throw runtime_error(string("Error") + "\t" + "���̓t�@�C�� " + lexical_cast<string>(cFilePath) + "���J���܂���B");
	}

	string aLineBuff;
	while (getline(aFin, aLineBuff))
	{
		tokenizer<char_separator<char> > aToken(aLineBuff, char_separator<char>("\t"));
		vector<string> aRecord;
		copy(aToken.begin(), aToken.end(), back_inserter(aRecord));
		if (aRecord.size() != 5)
		{
			throw runtime_error(string("Error") + "\t" + "�{�ݖ��̕ϊ��e�[�u���t�@�C���̃t�B�[���h�����s��");
		}

		m_Data.insert(
			make_pair(
				FacilNameChangeCondition(
					atoi(aRecord[0].c_str()),
					atoi(aRecord[1].c_str()),
					aRecord[2]
				),
				ChangedFacilName(aRecord[3], aRecord[4])
			)
		);
	}
}

/**
 * @brief �^����ꂽ�����i�ϊ��O��RoadCode, RoadSeq, �{�ݖ��́j�ɑΉ�����A�ϊ���̎{�ݖ��̂�Ԃ�
 * @param	cRoadCode	[in]	�ϊ��O�̘H���R�[�h
 * @param	cRoadSeq	[in]	�ϊ��O�̘H�����V�[�P���X�ԍ�
 * @param	cFacilName	[in]	�ϊ��O�̎{�ݖ���
 * @return	�ϊ���̎{�ݖ���
 * @retval	NULL	�ϊ��Ώۂł͂Ȃ��ꍇ��NULL��Ԃ�
 **/
std::auto_ptr<ChangedFacilName> FacilNameChangeTable::getChangedFacilName(long cRoadCode, long cRoadSeq, std::string cFacilName) const
{
	if (! m_EnabledToChange) {
		return std::auto_ptr<ChangedFacilName>();
	}

	Data::const_iterator aRecordIt = m_Data.find(FacilNameChangeCondition(cRoadCode, cRoadSeq, cFacilName));
	if (aRecordIt != m_Data.end())
	{
		return std::auto_ptr<ChangedFacilName>(
			new ChangedFacilName(
				aRecordIt->second.m_Kanji,
				aRecordIt->second.m_Yomi
			)
		);
	}

	return std::auto_ptr<ChangedFacilName>();
}


