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
#include "main.h"
#include "Arguments.h"
#include "ConvRoadName.h"

int _tmain(int argc, _TCHAR* argv[])
{
	int retVal = 0;
	CString strMsg;
	try {
		const arctl::coinitializer aCoInitializer;

		// ���P�[���֘A�̕s��h�~
		uh::setJapaneseCharacterTypeFacet();

		// �R�}���h���C����������
		Arguments args;
		if (!args.parse(argc, argv)) {
			retVal = 1;
		} else {
			// �{�������{
			strMsg.Format(_T("[%s] �����J�n"), getNowTime());
			Log::GetInstance().PrintLog(true, false, true, false, strMsg);
			ConvRoadName convRoadName;
			if (!convRoadName.run(args))
				retVal = 1;
		}
	} catch (const _com_error& e) {
		Log::GetInstance().PrintLog(true, true, true, false, CString(e.ErrorMessage()));
		retVal = 2;
	} catch (const std::exception& e) {
		Log::GetInstance().PrintLog(true, true, true, false, CString(e.what()));
		retVal = 2;
	} catch (...) {
		Log::GetInstance().PrintLog(true, true, true, false, _T("�\�����ʃG���[���������܂���"));
		retVal = 2;
	}
	if (0 == retVal) {
		strMsg.Format(_T("[%s] ����I��"), getNowTime());
		Log::GetInstance().PrintLog(true, false, true, false, strMsg);
	} else {
		strMsg.Format(_T("[%s] �ُ�I��"), getNowTime());
		Log::GetInstance().PrintLog(true, false, true, false, strMsg);
	}
	strMsg.Format(_T("[%s] ��������"), getNowTime());
	Log::GetInstance().PrintLog(true, false, true, false, strMsg);

    return retVal;
}

