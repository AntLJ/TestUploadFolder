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

// SindyAppConfig.cpp: SindyAppConfig �N���X�̃C���v�������e�[�V����
//
//////////////////////////////////////////////////////////////////////

#include "SindyAppConfig.h"
#include <io.h>

SindyAppConfig::SindyAppConfig() :
mConfiguration()
{

}

SindyAppConfig::~SindyAppConfig()
{

}

static bool is_skip_line(const char *cLine)
{
	if (*cLine == '\n' ||			// ���s
		*cLine == '#'  ||			// �V���[�v
		*cLine == '\0' ||			// NULL
		*cLine == ';'  ||			// �Z�~�R����
		*cLine == '\t' ||			// �^�u
		*cLine == ' ')				// �X�y�[�X
		return true;
	return false;
}

static bool is_skip_char(const char cChar)
{
	if (cChar == ' '  ||			// �X�y�[�X
		cChar == '\n' ||			// ���s
//		cChar == '='  ||			// �C�R�[��
		cChar == '\t' ||			// �^�u
		cChar == ','  ||			// �J���}
		cChar == '#')				// �V���[�v
		return true;
	return false;
}

HRESULT SindyAppConfig::Read(const char *cFileName)
{
	const char *func = "SindyAppConfig::Read";

	std::ifstream aFile(cFileName);
	if (_access(cFileName, 0) != 0) { std::cerr << cFileName << " not found." << std::endl; return E_ACCESSDENIED; }
	if (! aFile.is_open()) return GDBERROR2(E_ACCESSDENIED, func);

	while (aFile.good())
	{
		char buff[BUFSIZ*2];
		for (long p = 0; p < (BUFSIZ*2); p++) buff[p] = '\0';
		aFile.getline(buff, BUFSIZ*2);

		if (is_skip_line(buff)) continue;

		char word[BUFSIZ*2];
		long num = 0, count = 0;

		AppConfig aConf;
		for (long i = 0; buff[i] != '\n' && buff[i] != '\0'; i++)
		{
			if (is_skip_char(buff[i]))
			{
				if (! num) continue;
				if (! count)
				{
					word[num++] = '\0';
					aConf.eKeyword = word;
					count++;
					num = 0;
				}
				else
				{
					word[num++] = '\0';
					aConf.eConfigList.push_back(word);
					count++;
					num = 0;
				}
				continue;
			}
			word[num++] = buff[i];
		}

		if (num && ! count)
		{
			word[num] = '\0';
			aConf.eKeyword = word;
		}
		else if (num && count)
		{
			word[num] = '\0';
			aConf.eConfigList.push_back(word);
		}

		mConfiguration.insert(aConf);
	}

	return S_OK;
}

void SindyAppConfig::GetConfig(const std::string &cKey, std::vector<std::string> *cConfigList)
{
	cConfigList->clear();
	AppConfig aKey(cKey);
	std::set<AppConfig,lessAppConfig>::iterator aIter = mConfiguration.find(aKey);
	if (aIter == mConfiguration.end()) return;
	*cConfigList = aIter->eConfigList;
	return;
}

void SindyAppConfig::GetConfig(const std::string &cKey, std::string *cConfig)
{
	*cConfig = "";
	AppConfig aKey(cKey);
	std::set<AppConfig,lessAppConfig>::iterator aIter = mConfiguration.find(aKey);
	if (aIter == mConfiguration.end()) return;
	if (aIter->eConfigList.size()) *cConfig = aIter->eConfigList[0];
	return;
}
