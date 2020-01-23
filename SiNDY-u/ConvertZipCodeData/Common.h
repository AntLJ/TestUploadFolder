#pragma once

// カーソルフラッシュ用
#define	FLUSH_COUNT	10000

// 市区町村代表の住所コード桁数
#define	REP_ADDRCODE_COUNT	5

// 市区町村代表の郵便番号判別用
#define	REP_ZIPCODE	_T("00")

// 11桁住所データのファイル名
#define ADDR11_FILENAME	L"addr[0-9][0-9].txt"
// 11桁住所データのレコードタブ数
#define ADDR11_TAB_COUNT	16
// 11桁住所データの精度コードタブ位置
#define ADDR11_POS_ACCCODE	4
// 11桁住所データのPrefCodeタブ位置
#define ADDR11_POS_PREFCODE	5
// 11桁住所データのCityCodeタブ位置
#define ADDR11_POS_CITYCODE	6
// 11桁住所データのAzaCodeタブ位置
#define ADDR11_POS_AZACODE	7
// 11桁住所データのOazaCodeタブ位置
#define ADDR11_POS_OAZACODE	8

// 精度コード2
#define ACCCODE_2	_T("2")

// 住所コード桁
#define ADDR_DIGIT_JUKYO_LEVEL 20
#define ADDR_DIGIT_CHIBAN_LEVEL 16
#define ADDR_DIGIT_AZA_LEVEL 11
#define ADDR_DIGIT_OAZA_LEVEL 8
#define ADDR_DIGIT_CITY_LEVEL 5
#define ADDR_DIGIT_PREF_LEVEL 2

// 日時文字列取得
inline CString getNowTime()
{
	CString strTime;
	
	SYSTEMTIME sysTime;
	GetLocalTime(&sysTime);
	strTime.Format(_T("%04d/%02d/%02d %02d:%02d:%02d"), sysTime.wYear, sysTime.wMonth, sysTime.wDay, sysTime.wHour, sysTime.wMinute, sysTime.wSecond);

	return strTime;
}

// OID取得用
inline CString getID(const _IRowPtr& ipRow)
{
	CString strOID;
	long lOID = 0;
	ipRow->get_OID(&lOID);
	strOID.Format(_T("%ld"), lOID);
	return strOID;
}

// EUCからSJIS変換用
inline CString convEUC2SJIS(const CStringA& str)
{
	char szRet[1024] = {0};
	wkfConvertKanjiCodeOfString(KC_EUC, CA2A(str), KC_SJIS, szRet, sizeof(szRet));
	CString strRet(szRet);
	return strRet;
}
