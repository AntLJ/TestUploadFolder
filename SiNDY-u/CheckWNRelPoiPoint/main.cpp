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
#include <crd_cnv\coord_converter.h>
#include "Common.h"
#include "Param.h"
#include "Log.h"
#include "CheckWNRelPoiPoint.h"

int _tmain(int argc, _TCHAR* argv[]) {
	int retval = 0;
	try {
		arctl::coinitializer coinitializer;

		SYSTEMTIME sysTime;

		// �J�n���Ԏ擾
		auto st_time = std::chrono::system_clock::now();
		GetLocalTime( &sysTime );

		CParam cParam;
		// �p�����[�^�`�F�b�N
		if (!cParam.checkParam(argc, argv)) {
			retval = 1;
		}
		else {
			// ���O����
			if (!CLog::initLog(cParam)) {
				retval = 1;
			}
			else {
				// �J�n����
				CLog::WriteRunLogSTART(sysTime);

				// �f�[�^�`�F�b�N
				CCheckWNRelPoiPoint checker(cParam);
				if (!checker.execute()) {
					retval = 1;
				}
				else if (checker.isError()) {
					retval = 3;
				}

				// �I�����Ԏ擾
				auto end_time = std::chrono::system_clock::now();
				GetLocalTime( &sysTime );

				CString csTotal;
				csTotal.Format(_T("(TOTAL���� %.2f[sec])"),
							   (double)std::chrono::duration_cast<std::chrono::milliseconds>(end_time - st_time).count()/1000);

				// �I������
				CLog::WriteRunLogEND(sysTime, csTotal);
			}
		}
	} 
	catch (const std::exception &e)	{
		std::cerr << e.what() << std::endl;
		retval = 2;
	}
	catch (const _com_error &com_err) {
		std::cerr << CT2CA(com_err.ErrorMessage()) << std::endl;
		retval = 2;
	}

	switch (retval) {
		case 0:
			CLog::WriteRunLog( "����I��" );
			break;
		case 1:
			CLog::WriteRunLog( "�ُ�I��!" );
			break;
		case 2:
			CLog::WriteRunLog( "��O�I��!!" );
			break;
		case 3:
			CLog::WriteRunLog( "����I��(�`�F�b�N�G���[����!)" );
			break;
	}

	return retval;
}
