#pragma once

// �J�[�\���t���b�V���p
#define	FLUSH_COUNT	10000

// �s�撬����\�̏Z���R�[�h����
#define	REP_ADDRCODE_COUNT	5

// �s�撬����\�̗X�֔ԍ����ʗp
#define	REP_ZIPCODE	_T("00")

// 11���Z���f�[�^�̃t�@�C����
#define ADDR11_FILENAME	L"addr[0-9][0-9].txt"
// 11���Z���f�[�^�̃��R�[�h�^�u��
#define ADDR11_TAB_COUNT	16
// 11���Z���f�[�^�̐��x�R�[�h�^�u�ʒu
#define ADDR11_POS_ACCCODE	4
// 11���Z���f�[�^��PrefCode�^�u�ʒu
#define ADDR11_POS_PREFCODE	5
// 11���Z���f�[�^��CityCode�^�u�ʒu
#define ADDR11_POS_CITYCODE	6
// 11���Z���f�[�^��AzaCode�^�u�ʒu
#define ADDR11_POS_AZACODE	7
// 11���Z���f�[�^��OazaCode�^�u�ʒu
#define ADDR11_POS_OAZACODE	8

// ���x�R�[�h2
#define ACCCODE_2	_T("2")

// �Z���R�[�h��
#define ADDR_DIGIT_JUKYO_LEVEL 20
#define ADDR_DIGIT_CHIBAN_LEVEL 16
#define ADDR_DIGIT_AZA_LEVEL 11
#define ADDR_DIGIT_OAZA_LEVEL 8
#define ADDR_DIGIT_CITY_LEVEL 5
#define ADDR_DIGIT_PREF_LEVEL 2

// ����������擾
inline CString getNowTime()
{
	CString strTime;
	
	SYSTEMTIME sysTime;
	GetLocalTime(&sysTime);
	strTime.Format(_T("%04d/%02d/%02d %02d:%02d:%02d"), sysTime.wYear, sysTime.wMonth, sysTime.wDay, sysTime.wHour, sysTime.wMinute, sysTime.wSecond);

	return strTime;
}

// OID�擾�p
inline CString getID(const _IRowPtr& ipRow)
{
	CString strOID;
	long lOID = 0;
	ipRow->get_OID(&lOID);
	strOID.Format(_T("%ld"), lOID);
	return strOID;
}

// EUC����SJIS�ϊ��p
inline CString convEUC2SJIS(const CStringA& str)
{
	char szRet[1024] = {0};
	wkfConvertKanjiCodeOfString(KC_EUC, CA2A(str), KC_SJIS, szRet, sizeof(szRet));
	CString strRet(szRet);
	return strRet;
}
