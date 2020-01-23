#include "stdafx.h"
#include <arctl/coinitializer.h>
#include <atl2/exception.h>
#include <TDC/useful_headers/japanese_locale.h>
#include "Log.h"
#include "Define.h"
#include "Common.h"
#include "Arguments.h"
#include "ConvertZipCodeData.h"
using namespace std;

int _tmain(int argc, _TCHAR* argv[])
{
	int retval = 0;

	try{
		const arctl::coinitializer aCoInitializer;

		// ���P�[���֘A�̕s��h�~
		uh::setJapaneseCharacterTypeFacet();

		// �R�}���h���C����������
		Arguments args;
		if(! args.parse(argc, argv)){
			retval = 1;
		} else {
			// �ݒ�t�@�C���Ȃǂ̓ǂݍ��݁A������
			CConvertZipCodeData cConvertZipCodeData;
			if(!cConvertZipCodeData.init(args)){
				retval = 1;
			} else {
				// �������s
				if(!cConvertZipCodeData.run()){
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
