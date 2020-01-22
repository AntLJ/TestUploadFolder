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
#include "ADAMConverter.h"

int _tmain(int argc, _TCHAR* argv[])
{
	int retval = 0;

	try {
		const arctl::coinitializer aCoInitializer;

		// ���P�[���֘A�̕s��h�~
		uh::setJapaneseCharacterTypeFacet();

		// �R�}���h���C����������
		Arguments args;
		if(! args.parse(argc, argv)){
			retval = 1;
		} else {
			// �ݒ�t�@�C���Ȃǂ̓ǂݍ��݁A������
			CADAMConverter cADAMConverter;
			if(!cADAMConverter.init(args)){
				retval = 1;
			} else {
				// �������s
				if(!cADAMConverter.run()){
					retval = 1;
				}
			}
		}
	} catch(const _com_error& e) {
		CLog::GetInstance().PrintLog(true, true, true, false, CString(e.ErrorMessage()));
		retval = 2;
	} catch(const std::exception& e){
		CLog::GetInstance().PrintLog(true, true, true, false, CString(e.what()));
		retval = 2;
	} catch (...) {
		CLog::GetInstance().PrintLog(true, true, true, false, _T("�\�����ʃG���[���������܂���"));
		retval = 2;
	}

	CString strMsg;
	strMsg.Format(_T("End : %s\n"), getNowTime());
	CLog::GetInstance().PrintLog(true, false, true, false, strMsg);

	if(0 == retval){
		CLog::GetInstance().PrintLog(true, false, true, false, _T("����I��"));
	} else {
		CLog::GetInstance().PrintLog(true, false, true, false, _T("�ُ�I��"));
	}
	return retval;
}
