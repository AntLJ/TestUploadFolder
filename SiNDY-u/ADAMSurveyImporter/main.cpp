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

#include "stdafx.h"

#include <time.h>

#include <arctl/coinitializer.h>
#include <atl2/exception.h>
#include <TDC/useful_headers/japanese_locale.h>
#include <WinLib/VersionInfo.h>

#include "LogUtil.h"
#include "Arguments.h"
#include "ADAMSurveyImporter.h"

using namespace std;

int _tmain(int argc, _TCHAR* argv[])
{
	try
	{
		clock_t start = clock(); 
		setlocale(LC_ALL,"japanese");

		const arctl::coinitializer aCoInitializer;

		// ���P�[���֘A�̕s��h�~
		uh::setJapaneseCharacterTypeFacet();

		LogUtil::printLine();
		LogUtil::print("   ADAM v2 �C���|�[�g�c�[��");
		CVersion cVer;
		LogUtil::print((uh::tformat(_T("%s FILEVERSION:%s  PRODUCTVERSION:%s")) % cVer.GetOriginalFilename().GetString() % cVer.GetFileVersion().GetString() % cVer.GetProductVersion().GetString()).str());

		try
		{
			// �R�}���h���C����������
			Arguments args;
			if(! args.parse(argc, argv))
			{
				LogUtil::printError("�R�}���h���C�������ɖ�肪����܂�");
				return 1;
			}

			// ������
			int ret = 1;
			ADAMSurveyImporter app( args );
			if( app.execute() )
			{
				// ����I��
				LogUtil::printLine();
				LogUtil::print("����I��");
				ret = 0;
			}
			else
			{
				// �ُ�I��
				LogUtil::printLine();
				LogUtil::printError("�ُ�I��\t���O���m�F���Ă�������");
				ret = 1;
			}
			clock_t end = clock();
			std::string msg = "[��������] " + std::to_string((double)(end - start) / CLOCKS_PER_SEC / 60) + "min.\n";
			LogUtil::print(msg);
			LogUtil::write();
			return ret;
		}
		catch(const _com_error& e)
		{
			throw std::runtime_error(uh::toStr(atl2::stringize(e)));
		}
	}
	catch(const std::exception& e)
	{
		LogUtil::printLine();
		LogUtil::printError("�ُ�I�����܂���");
		LogUtil::printError("��O���o\t" +  uh::toStr(e.what()));
		LogUtil::write();
		return 2;
	}
}
