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

#include "StdAfx.h"
#include "Check.h"
#ifdef USE_BOOST_REGEX
#include <boost/regex.hpp>
#else
#include <regex>
#endif
#include "Defines.h"
#define foreach BOOST_FOREACH
namespace{
	const long BUF_SIZE = 256;
	typedef boost::char_separator<char> char_separator;
	typedef boost::tokenizer<char_separator> tokenizer;
	LPCTSTR GS_WEEK[] = {
		_T("日"),
		_T("月"),
		_T("火"),
		_T("水"),
		_T("木"),
		_T("金"),
		_T("土"),
		_T("祝"),
		NULL
	};
	LPCTSTR NOT_ORIG[] = {
		MAINTE_FIELD_NOT_CHECK1, 
		MAINTE_FIELD_NOT_CHECK2,
		MAINTE_FIELD_NOT_CHECK3,
		MAINTE_FIELD_NOT_CHECK4,
		MAINTE_FIELD_NOT_CHECK5,
		MAINTE_FIELD_NOT_CHECK6,
		NULL
	};

	LPCTSTR NOT_NULL[] = {
		MAINTE_FIELD_VARID_DATE,	//更新情報有効日付
		MAINTE_FIELD_PREFNAME,		//都道府県名称
		MAINTE_FIELD_CITY_NAME,		//市区町村名称
		MAINTE_FIELD_LONG256,		//1/256経度
		MAINTE_FIELD_LAT256,		//1/256緯度
		MAINTE_FIELD_LONG_DEG,		//度分秒経度
		MAINTE_FIELD_LAT_DEG,		//度分秒緯度
		MAINTE_FIELD_LONG,			//経度
		MAINTE_FIELD_LAT,			//緯度
		MAINTE_FIELD_CONTENTS_NAME,	//コンテンツ名称
		MAINTE_FIELD_NAME,			//名称
		MAINTE_FIELD_YOMI,			//読み
		MAINTE_FIELD_PRECISION,		//精度コード
		MAINTE_FIELD_COMPANY,		//座標値提供元コード
		MAINTE_FIELD_LANDMARK,		//ランドマーク
//		MAINTE_FIELD_TEL,			//電話番号
		MAINTE_FIELD_LIQUOR_FLAG,	//お酒販売
		MAINTE_FIELD_CIGAR_FLAG,	//ﾀﾊﾞｺ販売
		MAINTE_FIELD_TOOLROAD_FLAG,	//有料道路フラグ
		NULL
	};
}

CCheck::CCheck(void)
: m_OldDir(_T(""))
, m_NewDir(_T(""))
, m_OldFile()
, m_NewFile()
, m_LineNoIndex(-1)
, m_SeqIndex(0)
, m_FuncSeqIndex(0)
, m_NameIndexOrg(-1)
, m_NameIndexNew(-1)
, m_pFile(NULL)
, m_LineNo(_T(""))
, m_Seq(_T(""))
, m_Name(_T(""))
, m_HaveAddr(false)
, m_ParkMode(false)
, m_Mode(0)
{
}

CCheck::~CCheck(void)
{
	m_Head.clear();
	m_OrgData.clear();
	m_NewData.clear();
	m_IDSet.clear();
	if(m_pFile && m_pFile != stderr){
		fclose(m_pFile);
	}
}

bool CCheck::init(LPCTSTR cOldDir, LPCTSTR cNewDir, LPCTSTR cLogFile, int cMode, const CString &cMMaster)
{
	if(cLogFile){
		if((m_pFile = _tfopen(cLogFile, _T("wt"))) == NULL){
			_ftprintf(stderr, _T("ERROR\tログファイルが開けません。標準エラー出力を使用します。\t%s\n"), cLogFile);
			m_pFile = stderr;
		}
	}else{
		m_pFile = stderr;
	}
	if(_taccess(cOldDir, 0x04) == 0){
		m_OldDir = cOldDir;
	}else{
		_ftprintf(m_pFile, _T("ERROR\t編集前ディレクトリの設定が正しくありません。\t%s\n"), cOldDir);
		return false;
	}
	if(_taccess(cNewDir, 0x04) == 0){
		m_NewDir = cNewDir;
	}else{
		_ftprintf(m_pFile, _T("ERROR\t編集後ディレクトリの設定が正しくありません。\t%s\n"), cNewDir);
		return false;
	}
	if(cMMaster != _T("")){
		if(!fnLoadMikakuMaster(cMMaster)){
			return false;
		}
	}
	m_CTypeMap[MAINTE_FIELD_NAME] = (int)CheckerType::Name;	//名称
	m_CTypeMap[MAINTE_FIELD_YOMI] = (int)CheckerType::Yomi;	//読み
	m_CTypeMap[MAINTE_FIELD_VARID_DATE] = (int)CheckerType::Date;	//更新情報有効日付
	m_CTypeMap[MAINTE_FIELD_COMPANY] = (int)CheckerType::Ipcode;	//座標値提供元
	m_CTypeMap[MAINTE_FIELD_LONG256] = (int)CheckerType::Lon256;	//1/256経度
	m_CTypeMap[MAINTE_FIELD_LAT256] = (int)CheckerType::Lat256;	//1/256緯度
	m_CTypeMap[MAINTE_FIELD_LONG_DEG] = (int)CheckerType::DmsE;	//度分秒経度
	m_CTypeMap[MAINTE_FIELD_LAT_DEG] = (int)CheckerType::DmsN;	//度分秒緯度
	m_CTypeMap[MAINTE_FIELD_PRECISION] = (int)CheckerType::Acccode;	//精度
	m_CTypeMap[MAINTE_FIELD_CLOSED_CODE] = (int)CheckerType::Closecode;	//閉鎖理由コード
	m_CTypeMap[MAINTE_FIELD_ADDR] = (int)CheckerType::Address;	//住所
	m_CTypeMap[MAINTE_FIELD_ADMIN_MEMO] = (int)CheckerType::MntMemoAdmin;	//管理者メモ
	m_CTypeMap[MAINTE_FIELD_WORK_MEMO] = (int)CheckerType::MntMemoWork;	//作業者メモ
	m_CTypeMap[MAINTE_FIELD_BG_INFO1] = (int)CheckerType::BgInfo1;	//参考資料1
	m_CTypeMap[MAINTE_FIELD_BG_INFO2] = (int)CheckerType::BgInfo2;	//参考資料2
	m_CTypeMap[MAINTE_FIELD_URL] = (int)CheckerType::Url;	//URL
	m_CTypeMap[MAINTE_FIELD_USE_ID] = (int)CheckerType::IpUseId;	//情報提供元利用ID
	m_CTypeMap[MAINTE_FIELD_TEL] = (int)CheckerType::Tel;	//電話番号
	m_CTypeMap[MAINTE_FIELD_CLOSED_MEMO] = (int)CheckerType::CloseMemo;	//閉鎖理由メモ
//	m_CTypeMap[MAINTE_FIELD_XML] = CheckerType::MntInfo;	//整備作業情報（XML）.
	//	NameYomiPair
	m_Mode = cMode;
	return true;
}

bool CCheck::execCheck(LPCTSTR cFileName)
{
	m_IDSet.clear();
	m_Head.clear();
	m_OrgData.clear();
	m_NewData.clear();
	m_OldFile.close();
	m_NewFile.close();
	m_OldFile.clear();	//再利用には必須らしい
	m_NewFile.clear();	//再利用には必須らしい
	_tprintf(_T("Now %s\n"), cFileName);
	if(CString(cFileName).Find(_T("【PARK】")) == 0 || CString(cFileName).Find(_T("【ROUTE】")) == 0){
		m_ParkMode = true;
	}else{
		m_ParkMode = false;
	}
	//編集前データオープン
	CString aFile;
	aFile.Format(_T("%s\\%s"), static_cast<LPCTSTR>(m_OldDir), cFileName);
	m_OldFile.open(aFile, std::ios::in);
	if( ! m_OldFile.is_open() ){
		_ftprintf(m_pFile, _T("#ERROR\tファイルが開けません。\t%s\n"), static_cast<LPCTSTR>(cFileName));
		return false;
	}
	//編集後データオープン
	CString aNewFile;
	aFile.Format(_T("%s\\%s"), static_cast<LPCTSTR>(m_NewDir), cFileName);
	m_NewFile.open(aFile, std::ios::in);
	if( ! m_NewFile.is_open() ){
		m_NewFile.clear();	//再利用には必須らしい
		CString aSearch;
		CString aTmpFile = cFileName;
		int aIndex = aTmpFile.ReverseFind(_T('.'));
		if(aIndex > 0){
			aSearch.Format(_T("%s\\%s*%s"), static_cast<LPCTSTR>(m_NewDir), aTmpFile.Left(aIndex), aTmpFile.Mid(aIndex));
			WIN32_FIND_DATA fd;
			HANDLE hFind = ::FindFirstFile(aSearch, &fd);
			if(hFind == INVALID_HANDLE_VALUE){//検索失敗
				_ftprintf(m_pFile, _T("ERROR\t編集後ファイルが開けません。\t%s\n"), static_cast<LPCTSTR>(aFile));
				return false;
			}else{
				aFile.Format(_T("%s\\%s"), static_cast<LPCTSTR>(m_NewDir), fd.cFileName);
				aNewFile = aFile;
				m_NewFile.open(aFile, std::ios::in);
				if( ! m_NewFile.is_open() ){
					_ftprintf(m_pFile, _T("ERROR\t編集後ファイルが開けません。\t%s\n"), static_cast<LPCTSTR>(aFile));
					return false;
				}
			}
		}else{
			_ftprintf(m_pFile, _T("ERROR\t編集後ファイルが開けません。\t%s\n"), static_cast<LPCTSTR>(aFile));
			return false;
		}
	}
	//ヘッダの並び比較
	if(fnHeadComp()){
		//チェックに必要なフィールドの有無確認
		fnExistFieldCheck();
		//データの比較
		fnDataComp();
		//余りデータ・・・新規のはず？
		fnNewDataCheck();
	}
	return true;
}

bool CCheck::fnHeadComp(void)
{
	m_SeqIndex = m_FuncSeqIndex = -1;
	m_FieldMap.clear();
	std::string aLine;
	if( getline( m_OldFile, aLine ) ){
		if(fnAnalyzeHeadrer(aLine, m_Head)){
			if( getline( m_NewFile, aLine ) ){
				HEADER aTgtHead;
				if(fnAnalyzeHeadrer(aLine, aTgtHead)){
					if(m_Head == aTgtHead){
						if(m_SeqIndex >= 0){
							return true;
						}else{
							_ftprintf(m_pFile, _T("ERROR\t「シーケンス」がないため比較できません。\n"));
							return false;
						}
					}
				}
			}else{
				_ftprintf(m_pFile, _T("ERROR\t読み込みエラー\n"));
				return false;
			}
		}
	}
	_ftprintf(m_pFile, _T("ERROR\tヘッダが違うか比較できません。\n"));
	return false;
}

bool CCheck::fnAnalyzeHeadrer(const std::string cData, HEADER& cHead)
{
	char_separator a_csSep("\t", "", boost::keep_empty_tokens); // 分割設定
	tokenizer	a_tTokens(cData, a_csSep);	// 分割
	int i = 0;
	foreach( const std::string& aStr, std::make_pair( a_tTokens.begin(), a_tTokens.end()) ){
		CString aTmp(aStr.c_str());
		m_Seq = _T("ヘッダ");
		aTmp = fnCutDoubleQuotation(aTmp, _T("ヘッダ"));
		m_FieldMap[aTmp] = i;
		if(m_Mode == 0){
			if(aTmp == MAINTE_FIELD_SEQUENCE){
				m_SeqIndex = i;
			}else if(aTmp == MAINTE_FIELD_FUNC_SEQ){
				m_FuncSeqIndex = i;
			}else if(aTmp == MAINTE_FIELD_BANK_SEQUENCE){
				m_SeqIndex = i;
			}
		}else if(m_Mode == 1){
			//GS営業時間
			if(aTmp == MAINTE_FIELD_GS_SEQUENCE){
				m_SeqIndex = i;
			}
		}else{
			_ftprintf(m_pFile, _T("ERROR\t設定されていないモードです。\n"));
		}
		if(aTmp == MAINTE_FIELD_LINE_NO)	m_LineNoIndex = i;
		if(aTmp == MAINTE_FIELD_NAME)		m_NameIndexOrg = i;
		if(aTmp == CString(MAINTE_FIELD_NAME) + CString(MAINTE_FIELD_MODIFY))	m_NameIndexNew = i;
		cHead.push_back(aTmp);
		i++;
	}
	return true;
}

bool CCheck::fnDataComp(void)
{
	std::string aLine;
	while( getline( m_OldFile, aLine ) ){
		fnLoadOldData(aLine);
	}
	while( getline( m_NewFile, aLine ) ){
		fnLoadNewData(aLine);
	}
	return fnCompare();
}

bool CCheck::fnLoadOldData(std::string cData)
{
	char_separator a_csSep("\t", "", boost::keep_empty_tokens); // 分割設定
	tokenizer	a_tTokens(cData, a_csSep);	// 分割
	RECORD aRec;
	foreach( const std::string& aStr, std::make_pair( a_tTokens.begin(), a_tTokens.end()) ){
		CString aTmp(aStr.c_str());
		aRec.push_back(aTmp);
	}
	if(m_Head.size() != aRec.size()){
		if(m_FuncSeqIndex < 0){
			m_Seq = aRec[m_SeqIndex];
		}else{
			m_Seq = aRec[m_SeqIndex] + _T("_") + aRec[m_FuncSeqIndex];
		}
		if(m_LineNoIndex  >= 0) m_LineNo	= aRec[m_LineNoIndex];
		if(m_NameIndexOrg >= 0) m_Name		= aRec[m_NameIndexOrg];
		fnOutputErr(kError, _T("編集前データのフィールドが足りません。"));
		while(m_Head.size() > aRec.size()){
			//不足分はNULLで補う
			aRec.push_back(_T(""));
		}
	}
	m_OrgData.push_back(aRec);
	return true;
}

bool CCheck::fnLoadNewData(std::string cData)
{
	char_separator a_csSep("\t", "", boost::keep_empty_tokens); // 分割設定
	tokenizer	a_tTokens(cData, a_csSep);	// 分割
	RECORD aRec;
	foreach( const std::string& aStr, std::make_pair( a_tTokens.begin(), a_tTokens.end()) ){
		CString aTmp(aStr.c_str());
		aRec.push_back(aTmp);
	}
	if(m_FuncSeqIndex < 0){
		m_Seq = aRec[m_SeqIndex];
	}else{
		m_Seq = aRec[m_SeqIndex] + _T("_") + aRec[m_FuncSeqIndex];
	}
	if(m_LineNoIndex  >= 0) m_LineNo	= aRec[m_LineNoIndex];
	if(m_NameIndexOrg >= 0) m_Name		= aRec[m_NameIndexOrg];
	if(m_Head.size() != aRec.size()){
		fnOutputErr(kWarning, _T("編集後データのフィールドが足りません。"));
		while(m_Head.size() > aRec.size()){
			//不足分はNULLで補う
			aRec.push_back(_T(""));
		}
	}
	m_NewData.insert(COMP_LIST::value_type(m_Seq, aRec));
	return true;
}

bool CCheck::fnCompare(void)
{
	for(DATA::iterator aOrg = m_OrgData.begin(); aOrg != m_OrgData.end(); aOrg++){
		if(m_FuncSeqIndex < 0){
			m_Seq = (*aOrg)[m_SeqIndex];
		}else{
			m_Seq = (*aOrg)[m_SeqIndex] + _T("_") + (*aOrg)[m_FuncSeqIndex];
		}
		if(m_LineNoIndex  >= 0) m_LineNo	= (*aOrg)[m_LineNoIndex];
		if(m_NameIndexOrg >= 0) m_Name		= (*aOrg)[m_NameIndexOrg];
		COMP_LIST::iterator aNew = m_NewData.find(m_Seq);
		if(aNew != m_NewData.end()){
			if(m_Mode == 0){
				fnCompareRecord(*aOrg, aNew->second);
			}else if(m_Mode == 1){
				fnCompareRecordGS(*aOrg, aNew->second);
			}else{
				_ftprintf(m_pFile, _T("ERROR\t設定されていないチェックモードです。\t%d\n"), m_Mode);
				return false;
			}
			m_NewData.erase(aNew);
		}else{
			fnOutputErr(kError, _T("データが削除されました。"));
		}
	}
	return true;
}

bool CCheck::fnCompareRecord(const RECORD& cOrgData, const RECORD& cNewData)
{
/*	int aSize;
	if(cOrgData.size() > cNewData.size()){
		aSize = cNewData.size();
	}else{
		aSize = cOrgData.size();
	}*/
	/*注）修正値より元値が先に来ることが前提の作りになっている*/
	bool aChgCity = false;
	bool aChgXY = false;		//座標変更の有無
	bool aHavePrecision = false;	//「精度」フィールドの有無
	bool aHaveCompany = false;		//「座標値提供元」フィールドの有無
	bool aChgPrecision = false;	//「精度」フィールド変更の有無
	bool aChgCompany = false;	//「座標値提供元」フィールド変更の有無
	bool aQCCode = false;		//品証保証データ（座標値提供元=62）
	CString aPrecision = _T(""), aCompany = _T("");
	CString aAdd(_T(""));

	//銀行ATM用に追加
	bool aCheckBank = false;
	DELETE_INFO aDelInfo[2];
	long aOrgForm = -1;
	long aNewForm = -1;
	CString aParking = _T("");
	CString aBisTime = _T("");
	CString aBisHoliday = _T("");
	//銀行ATM用に追加　ここまで

	int i = 0;
	for(HEADER::iterator aHead = m_Head.begin(); /*i < aSize && */aHead != m_Head.end(); aHead++, i++){
		if(aHead->Find(MAINTE_FIELD_MODIFY) < 0){
			if(!fnIsNotOrig(*aHead)){
				//元値
				if(cOrgData[i] != cNewData[i] && cOrgData[i] != fnCompRegulate(*aHead, cNewData[i])){
					aAdd.Format(_T("%s\t%s\t%s"), static_cast<LPCTSTR>(*aHead), static_cast<LPCTSTR>(cOrgData[i]), static_cast<LPCTSTR>(cNewData[i]));
					fnOutputErr(kError, _T("元値が修正されています。"));
				}
				if(*aHead == MAINTE_FIELD_INFOFILE_ID){
					fnInfoFileCheck(cNewData[i]);
				}
				if(cOrgData[i] != _T("")){
					fnFieldCheck(*aHead, cOrgData[i], cOrgData);
				}
				//整合チェックの準備
				if((*aHead) == MAINTE_FIELD_PRECISION){
					aPrecision = cOrgData[i];
				}else if((*aHead) == MAINTE_FIELD_COMPANY){
					aCompany = cOrgData[i];
					if(aCompany == _T("62")){
						aQCCode = true;
					}
				}else if((*aHead) == MAINTE_FIELD_FORM){
					//店舗形態種別
					aOrgForm = _ttol(cOrgData[i]);
					aCheckBank = true;
				}
				//整合チェックの準備　ここまで
			}
		}else{
			//修正値
			if(cNewData[i] != _T("")){
				fnFieldCheck(*aHead, cNewData[i], cNewData);
				//整合チェックの準備
				if((*aHead).Find(MAINTE_FIELD_PREFNAME) == 0 || (*aHead).Find(MAINTE_FIELD_CITY_NAME) == 0){
					aChgCity = true;
				}else if((*aHead).Find(MAINTE_FIELD_LONG) >= 0 || (*aHead).Find(MAINTE_FIELD_LAT) >= 0){
					//「緯度」又は「経度」が含まれていればいいことに
					//同値が入っていることもあるから
					if(cOrgData[i] != cNewData[i])
						aChgXY = true;
				}
				//整合チェックの準備　ここまで
			}
			//整合チェックの準備
			if((*aHead).Find(MAINTE_FIELD_PRECISION) == 0){
				aHavePrecision = true;
				if(cNewData[i] != _T("")){
					aPrecision = cNewData[i];
					aChgPrecision = true;
				}
			}else if((*aHead).Find(MAINTE_FIELD_COMPANY) == 0){
				aHaveCompany = true;
				if(cNewData[i] != _T("")){
					aCompany = cNewData[i];
					aChgCompany = true;
					if(aQCCode && cNewData[i] != _T("62")){
						aAdd.Format(_T("%s\t%s\t%s"), static_cast<LPCTSTR>(*aHead), static_cast<LPCTSTR>(cOrgData[i]), static_cast<LPCTSTR>(cNewData[i]));
						fnOutputErr(kError, _T("品証データに対する座標値提供元コード修正です。"), aAdd);
					}
				}
			}else if((*aHead).Find(MAINTE_FIELD_FORM) == 0){
				//店舗形態種別
				aNewForm = _ttol(cNewData[i]);
			}else if((*aHead).Find(MAINTE_FIELD_WINDOW) == 0){
				//窓口_
				fnSetDeleteInfo(aDelInfo[0], (*aHead), cNewData[i]);
			}else if((*aHead).Find(MAINTE_FIELD_ATM) == 0){
				//ATM_
				fnSetDeleteInfo(aDelInfo[1], (*aHead), cNewData[i]);
			}else if((*aHead).Find(MAINTE_FIELD_BANK_PARKING) == 0){
				//駐車場
				aParking = cNewData[i];
			}else if((*aHead).Find(MAINTE_FIELD_BANK_BIS_TIME) == 0){
				//営業時間
				aBisTime = cNewData[i];
			}else if((*aHead).Find(MAINTE_FIELD_BANK_BIS_HOLIDAY) == 0){
				//休業日
				aBisHoliday = cNewData[i];
			}
			//整合チェックの準備　ここまで
		}
	}

	//各種整合チェック
	if(aChgXY)
	{
		if(aHavePrecision && !aChgPrecision){
			fnOutputErr(kError, _T("座標値修正値に入力がありますが、後述のフィールドに入力がありません。"), MAINTE_FIELD_PRECISION);
		}
		if(aHaveCompany && !aChgCompany){
			fnOutputErr(kError, _T("座標値修正値に入力がありますが、後述のフィールドに入力がありません。"), MAINTE_FIELD_COMPANY);
		}
		if(aQCCode){
			fnOutputErr(kError, _T("品証データに対する座標修正です。"));
		}
	}else{
		if(aChgCity){
			fnOutputErr(kError, _T("都道府県/市区町村が修正されていますが、座標値に修正値がありません。"));
		}
	}
	if(aHavePrecision && aHaveCompany){
		if((aCompany == _T("62") && aPrecision != _T("UK"))
			|| (aCompany == _T("00") && aPrecision == _T("UK"))
			|| (aCompany == _T("04") && aPrecision == _T("UK"))){
			aAdd.Format(_T("%s\t%s"), static_cast<LPCTSTR>(aCompany), static_cast<LPCTSTR>(aPrecision));
			fnOutputErr(kError, _T("精度との組み合わせが不正です。"), aAdd);
		}
	}
	//銀行ATM特別
	if(aCheckBank){
		if(aNewForm == 1){
			//窓口のみ
			if(aOrgForm == 3 || aOrgForm == 2){
				//ATM削除
				if(aDelInfo[1].m_DeleteCode != 1){
					aAdd.Format(_T("%d->%d\t%d"), aOrgForm, aNewForm, aDelInfo[1].m_DeleteCode);
					fnOutputErr(kError, _T("店舗形態種別と削除フラグの組み合わせが不正です。"), aAdd);
				}
				if(aDelInfo[1].m_ClosedCode == _T("")){
					aAdd.Format(_T("%d->%d\t%s"), aOrgForm, aNewForm, static_cast<LPCTSTR>(aDelInfo[1].m_ClosedCode));
					fnOutputErr(kError, _T("店舗形態種別と閉鎖理由コードの組み合わせが不正です。"), aAdd);
				}
				if(aDelInfo[1].m_ClosedMemo == _T("")){
					aAdd.Format(_T("%d->%d\t%s"), aOrgForm, aNewForm, static_cast<LPCTSTR>(aDelInfo[1].m_ClosedMemo));
					fnOutputErr(kError, _T("店舗形態種別と閉鎖日付・理由の組み合わせが不正です。"), aAdd);
				}
			}
			if(aParking != _T("あり") && aParking != _T("なし") && aParking != _T("○")){
				aAdd.Format(_T("%d\t%s"), aNewForm, static_cast<LPCTSTR>(aParking));
				fnOutputErr(kError, _T("店舗形態種別と駐車場の組み合わせが不正です。"), aAdd);
			}
			if(aBisTime != _T("") && aBisTime != _T("○")){
				aAdd.Format(_T("%d\t%s"), aNewForm, static_cast<LPCTSTR>(aBisTime));
				fnOutputErr(kError, _T("店舗形態種別と営業時間の組み合わせが不正です。"), aAdd);
			}
			if(aBisHoliday != _T("") && aBisHoliday != _T("○")){
				aAdd.Format(_T("%d\t%s"), aNewForm, static_cast<LPCTSTR>(aBisHoliday));
				fnOutputErr(kError, _T("店舗形態種別と休業日の組み合わせが不正です。"), aAdd);
			}
		}else if(aNewForm == 2){
			//ATMのみ
			if(aOrgForm == 3 || aOrgForm == 1){
				//窓口削除
				if(aDelInfo[0].m_DeleteCode != 1){
					aAdd.Format(_T("%d->%d\t%d"), aOrgForm, aNewForm, aDelInfo[0].m_DeleteCode);
					fnOutputErr(kError, _T("店舗形態種別と削除フラグの組み合わせが不正です。"), aAdd);
				}
				if(aDelInfo[0].m_ClosedCode == _T("")){
					aAdd.Format(_T("%d->%d\t%s"), aOrgForm, aNewForm, static_cast<LPCTSTR>(aDelInfo[0].m_ClosedCode));
					fnOutputErr(kError, _T("店舗形態種別と閉鎖理由コードの組み合わせが不正です。"), aAdd);
				}
				if(aDelInfo[0].m_ClosedMemo == _T("")){
					aAdd.Format(_T("%d->%d\t%s"), aOrgForm, aNewForm, static_cast<LPCTSTR>(aDelInfo[0].m_ClosedMemo));
					fnOutputErr(kError, _T("店舗形態種別と閉鎖日付・理由の組み合わせが不正です。"), aAdd);
				}
			}
			if(aParking != _T("") && aParking != _T("○")){
				aAdd.Format(_T("%d\t%s"), aNewForm, static_cast<LPCTSTR>(aParking));
				fnOutputErr(kError, _T("店舗形態種別と駐車場の組み合わせが不正です。"), aAdd);
			}
			if(aBisTime == _T("")){
				aAdd.Format(_T("%d\t%s"), aNewForm, static_cast<LPCTSTR>(aBisTime));
				fnOutputErr(kError, _T("店舗形態種別と営業時間の組み合わせが不正です。"), aAdd);
			}
			if(aBisHoliday == _T("")){
				aAdd.Format(_T("%d\t%s"), aNewForm, static_cast<LPCTSTR>(aBisHoliday));
				fnOutputErr(kError, _T("店舗形態種別と休業日の組み合わせが不正です。"), aAdd);
			}
		}else if(aNewForm == 3){
			//両方あり
			if(aParking == _T("")){
				aAdd.Format(_T("%d\t%s"), aNewForm, static_cast<LPCTSTR>(aParking));
				fnOutputErr(kError, _T("店舗形態種別と駐車場の組み合わせが不正です。"), aAdd);
			}
			if(aBisTime == _T("")){
				aAdd.Format(_T("%d\t%s"), aNewForm, static_cast<LPCTSTR>(aBisTime));
				fnOutputErr(kError, _T("店舗形態種別と営業時間の組み合わせが不正です。"), aAdd);
			}
			if(aBisHoliday == _T("")){
				aAdd.Format(_T("%d\t%s"), aNewForm, static_cast<LPCTSTR>(aBisHoliday));
				fnOutputErr(kError, _T("店舗形態種別と休業日の組み合わせが不正です。"), aAdd);
			}
		}
	}
	return true;
}

bool CCheck::fnFieldCheck(LPCTSTR cField, CString cData, const RECORD& cNewData)
{
	CString aTmpField = cField;
	aTmpField.Replace(MAINTE_FIELD_MODIFY, _T(""));
	CString aData = fnRegulate(aTmpField, cData);
	CString aAdd(_T(""));

	CHECK_TYPE_MAP::iterator aCType =m_CTypeMap.find(aTmpField);
	if(aCType != m_CTypeMap.end()){
		IChecker<CheckContainer^>^ iChecker = CheckerFactory::GetSingleUnitChecker((CheckerType)aCType->second);  // 文字列検査クラス
		if( iChecker )
		{
			CheckContainer^ cc;
			bool aDoYomiName = false;
			switch( aCType->second ){
				case CheckerType::Address:
				{
					if(m_HaveAddr){
						// 都道府県市区町村名取得
						CString strPrefName;
						if(cNewData[m_FieldMap[CString(MAINTE_FIELD_PREFNAME)+CString(MAINTE_FIELD_MODIFY)]] != _T("")){
							strPrefName = cNewData[m_FieldMap[CString(MAINTE_FIELD_PREFNAME)+CString(MAINTE_FIELD_MODIFY)]];
						}else{
							strPrefName = cNewData[m_FieldMap[MAINTE_FIELD_PREFNAME]];
						}
						CString strCityName;
						if(cNewData[m_FieldMap[CString(MAINTE_FIELD_CITY_NAME)+CString(MAINTE_FIELD_MODIFY)]] != _T("")){
							strCityName = cNewData[m_FieldMap[CString(MAINTE_FIELD_CITY_NAME)+CString(MAINTE_FIELD_MODIFY)]];
						}else{
							strCityName = cNewData[m_FieldMap[MAINTE_FIELD_CITY_NAME]];
						}
						// [CHECK] 都道府県コードと市区町村コードはセットである必要がある
						if( strPrefName.IsEmpty() || strCityName.IsEmpty() ){
//							cErrs.push_back( CSPSkErrorInfo(  new CSkErrorInfo( _T(""), _T(""), _T("整合性"), sindyErr_CheckReturnErr, sindyErrLevel_ERR, _T("%s"), _T("都道府県コードと市区町村コードはセットである必要があります") ) ) );
							aAdd.Format(_T("%s\t%s"), cField, static_cast<LPCTSTR>(cData));
							fnOutputErr(kError, _T("都道府県又は市区町村がNULL"), aAdd);
						}else {
							cc = gcnew AddressCheckContainer();
							safe_cast<AddressCheckContainer^>(cc)->Address = gcnew System::String(aData);
							safe_cast<AddressCheckContainer^>(cc)->RefPrefname = gcnew System::String(strPrefName);
							safe_cast<AddressCheckContainer^>(cc)->RefCityname = gcnew System::String(strCityName);
						}
					}
					break;
				}
/*				case CheckerType::NameYomiPair:
				{
					cc = gcnew NameYomiCheckContainer();
					safe_cast<NameYomiCheckContainer^>(cc)->Name = strCLIValue;
					// 読み部分は直接取得
					CString strYomi( GetValue( poi_point::kYomi, _T("") ) );
					strYomi.Remove(_T('｜'));
					safe_cast<NameYomiCheckContainer^>(cc)->Yomi = gcnew System::String(strYomi);
					break;
				}*/
				case CheckerType::Name:	//名称
				case CheckerType::Yomi:	//読み
					aDoYomiName = true;
					//個々のチェックもするから、breakしない。
				default:
					cc = gcnew StringCheckContainer();
					safe_cast<StringCheckContainer^>(cc)->Value = gcnew System::String(aData);
					break;
			}
			if( cc )
			{
				ErrorInfoList^ errList = iChecker->Check(cc); // チェック
				if( 0 < errList->Count )
				{
					for each( ErrorInfo^ info in errList )
					{
						pin_ptr<const wchar_t> strType = PtrToStringChars( info->ErrorType.ToString() );
						pin_ptr<const wchar_t> strErrID = PtrToStringChars( info->ErrorId );
						pin_ptr<const wchar_t> strErrMsg = PtrToStringChars( info->Message );
//						pin_ptr<const wchar_t> strCheckType = PtrToStringChars( aCType->second.ToString() );
						bool bIsWarning = ( 0 <= CString(strType).Find(_T("WAR")) );
						aAdd.Format(_T("%s\t%s"), cField, static_cast<LPCTSTR>(cData));
						fnOutputErr(bIsWarning ? kWarning : kError, CString(strErrMsg), aAdd);
//						cErrs.push_back( CSPSkErrorInfo(  new CSkErrorInfo( CString(strType), CString(strErrID), CString(strCheckType), bIsWarning ? sindyErr_CheckReturnWarning : sindyErr_CheckReturnErr, bIsWarning ? sindyErrLevel_WARN : sindyErrLevel_ERR, _T("%s（属性：%s）"), CString(strErrMsg), cFieldAttr.alias ) ) );
					}
				}
			}
			if(aDoYomiName){
				iChecker = CheckerFactory::GetSingleUnitChecker(CheckerType::NameYomiPair);	//必要？
				if( iChecker ){
					cc = gcnew NameYomiCheckContainer();
					CString strName;
					if(cNewData[m_FieldMap[CString(MAINTE_FIELD_NAME)+CString(MAINTE_FIELD_MODIFY)]] != _T("")){
						strName = cNewData[m_FieldMap[CString(MAINTE_FIELD_NAME)+CString(MAINTE_FIELD_MODIFY)]];
					}else{
						strName = cNewData[m_FieldMap[MAINTE_FIELD_NAME]];
					}
					safe_cast<NameYomiCheckContainer^>(cc)->Name = gcnew System::String(strName);
					CString strYomi;
					if(cNewData[m_FieldMap[CString(MAINTE_FIELD_YOMI)+CString(MAINTE_FIELD_MODIFY)]] != _T("")){
						strYomi = cNewData[m_FieldMap[CString(MAINTE_FIELD_YOMI)+CString(MAINTE_FIELD_MODIFY)]];
					}else{
						strYomi = cNewData[m_FieldMap[MAINTE_FIELD_YOMI]];
					}
					safe_cast<NameYomiCheckContainer^>(cc)->Yomi = gcnew System::String(strYomi);
					if( cc )
					{
						ErrorInfoList^ errList = iChecker->Check(cc); // チェック
						if( 0 < errList->Count )
						{
							for each( ErrorInfo^ info in errList )
							{
								pin_ptr<const wchar_t> strType = PtrToStringChars( info->ErrorType.ToString() );
								pin_ptr<const wchar_t> strErrID = PtrToStringChars( info->ErrorId );
								pin_ptr<const wchar_t> strErrMsg = PtrToStringChars( info->Message );
//								pin_ptr<const wchar_t> strCheckType = PtrToStringChars( aCType->second.ToString() );
								bool bIsWarning = ( 0 <= CString(strType).Find(_T("WAR")) );
								aAdd.Format(_T("%s\t%s"), cField, static_cast<LPCTSTR>(cData));
								fnOutputErr(bIsWarning ? kWarning : kError, CString(strErrMsg), aAdd);
		//						cErrs.push_back( CSPSkErrorInfo(  new CSkErrorInfo( CString(strType), CString(strErrID), CString(strCheckType), bIsWarning ? sindyErr_CheckReturnWarning : sindyErr_CheckReturnErr, bIsWarning ? sindyErrLevel_WARN : sindyErrLevel_ERR, _T("%s（属性：%s）"), CString(strErrMsg), cFieldAttr.alias ) ) );
							}
						}
					}
				}
			}
		}

	}else{
		if(aTmpField == MAINTE_FIELD_DELETE_CODE || aTmpField == MAINTE_FIELD_ROOT_DELCODE){
			if(aData != _T("") && aData != _T("1") && aData != _T("2") && aData != _T("×")){
				aAdd.Format(_T("%s\t%s"), cField, static_cast<LPCTSTR>(cData));
				fnOutputErr(kError, _T("削除コードが不正です。"), aAdd);
			}
		}else if(aTmpField == MAINTE_FIELD_PIC && m_Mikaku.size() > 0){
			if(aData != _T("○") && aData != _T("×")){
				aData = fnZen2Han(aData);
				aData = fnCutMultiSpace(aData);
				int aStart = 0;
				int aIndex = aData.Find(_T("､"));	//半角に変換されるから
				CString aCmp;
				while(aIndex > 0){
					aCmp = aData.Mid(aStart, aIndex - aStart);
					if(m_Mikaku.find(aCmp) == m_Mikaku.end()){
						aAdd.Format(_T("%s\t%s"), cField, static_cast<LPCTSTR>(aCmp));
						fnOutputErr(kError, _T("リストにない味覚狩りデータです。"), aAdd);
					}
					aStart = aIndex+1;
					aIndex = aData.Find(_T("､"), aStart);	//半角に変換されるから
				}
				aCmp = aData.Mid(aStart);
				if(m_Mikaku.find(aCmp) == m_Mikaku.end()){
					aAdd.Format(_T("%s\t%s"), cField, static_cast<LPCTSTR>(aCmp));
					fnOutputErr(kError, _T("リストにない味覚狩りデータです。"), aAdd);
				}
			}
		}else if(aTmpField.Find(MAINTE_FIELD_LONG) >= 0 || aTmpField.Find(MAINTE_FIELD_LAT) >= 0 ){
			// [Bug8763]座標書式チェック追加
			if( !fnCoordCheck(aTmpField, aData) ) {
				aAdd.Format(_T("%s\t%s"), cField, static_cast<LPCTSTR>(cData));
				fnOutputErr(kError, _T("座標フォーマットが不正です。"), aAdd);
			}
		}
	}
	if(aData.Find(_T("\"")) >= 0){
		aAdd.Format(_T("%s\t%s"), cField, static_cast<LPCTSTR>(cData));
		fnOutputErr(kError, _T("「\"」が含まれています。"), aAdd);
	}
	return true;
}

bool CCheck::fnInfoFileCheck(CString cData)
{
	//・ディレクトリ名の桁数は2～3桁であること
	//・ファイル名の桁数はディレクトリ名の桁数と同一であること
	//・ファイル名は同一フォルダ内でユニークであること（コンテンツ内ではないことに注意）
	//・数値と「f」のみで構成されていること（fは一つだけ）
/*
	・ディレクトリ名の桁数は2～3桁であること
	・ファイル名（拡張子なし）の桁数は、「7 - ディレクトリ名の桁数」であること
	・新規ファイル名（拡張子なし）の桁数は、「7 - ディレクトリ名の桁数」と一致していること
	・「7 - ディレクトリ名の桁数」に、ファイル名（拡張子なし）の桁数が満たないは0詰めされていること
	・ディレクトリ名とファイル名（拡張子なし）の間に「f」が存在すること
	・同一POIに紐付く情報ファイルIDは1つであること
*/
	if(cData != _T("")){
		if(cData.GetLength() == 8){
			std::string aData = CT2CA(cData).m_psz;
#ifdef USE_BOOST_REGEX
			if(!boost::regex_match(aData, boost::regex("[0-9]{2}f[0-9]{5}")) && !boost::regex_match(aData, boost::regex("[0-9]{3}f[0-9]{4}"))){
#else
			if(!std::tr1::regex_match(aData, std::tr1::regex("[0-9]{2}f[0-9]{5}")) && !std::tr1::regex_match(aData, std::tr1::regex("[0-9]{3}f[0-9]{4}"))){
#endif
				fnOutputErr(kError, _T("情報ファイルIDのフォーマットが不正です。"), cData);
			}
			std::pair<ID_SET::iterator, bool> aRet = m_IDSet.insert(cData);
			if(!aRet.second){
				fnOutputErr(kError, _T("情報ファイルIDがユニークではありません。"), cData);
			}
		}else{
			fnOutputErr(kError, _T("情報ファイルIDの桁が8桁ではありません。"), cData);
		}
	}
	return true;
}

CString CCheck::fnRegulate(const CString& cField, const CString& cData)
{

	CString aResult = fnCutDoubleQuotation(cData, cField);
	aResult = fnCutSpace(aResult);
	if(cField == MAINTE_FIELD_NAME || cField == MAINTE_FIELD_YOMI || cField == MAINTE_FIELD_ADDR)
	{
		// [Bug8762]正規化前に名称・町丁目以下住所について半角二重引用符チェックを実施(読みもやって問題ないので実施)
		if(aResult.Find(_T("\"")) >= 0){
			CString aAdd;
			aAdd.Format(_T("%s\t%s"), cField, static_cast<LPCTSTR>(cData));
			fnOutputErr(kError, _T("「\"」が含まれています。"), aAdd);
		}

		//名称・読み
		aResult = fnHan2Zen(aResult);
		aResult = fnCutMultiSpace(aResult);
		if(cField == MAINTE_FIELD_YOMI){
			aResult = fnConvYomiSP(aResult);
		}
	}else if(cField == MAINTE_FIELD_TEL || cField == MAINTE_FIELD_LONG256 || cField == MAINTE_FIELD_LAT256 || cField == MAINTE_FIELD_LONG_DEG || cField == MAINTE_FIELD_LAT_DEG
		|| cField == MAINTE_FIELD_ROOT_LONG256 || cField == MAINTE_FIELD_ROOT_LAT256){
		//電話番号・1/256経度・1/256緯度・度分秒経度・度分秒緯度, ルート探索用ポイント.1/256経度, ルート探索用ポイント.1/256緯度
		aResult = fnZen2Han(aResult);
	}else if(cField == MAINTE_FIELD_CLOSED_MEMO || cField == MAINTE_FIELD_WORK_MEMO || cField == MAINTE_FIELD_ADMIN_MEMO || cField == MAINTE_FIELD_BG_INFO1 || cField == MAINTE_FIELD_BG_INFO2 || cField == MAINTE_FIELD_URL){
		//閉鎖理由メモ・作業者メモ・管理者メモ・参考資料1・参考資料2・URL
		aResult = fnCutMultiSpace(aResult);
	}else if(cField.Find(MAINTE_FIELD_INTRO) == 0){
		//紹介文.xx
//		aResult = fnZen2Han(aResult);
//		aResult = fnCutMultiSpace(aResult);
	}else if(cField.Find(MAINTE_FIELD_XML) == 0){
		//整備作業情報（XML）.xx
	}
	return aResult;
}

CString CCheck::fnCutSpace(CString cData)
{
//	cData = _T(" 　  　　 テスト  　　　 ");
//	cData = _T(" 　  　　 ○  　　　 ");
	std::wstring aResult = cData;
#ifdef USE_BOOST_REGEX
	//先頭の空白削除
	aResult = boost::regex_replace(aResult, boost::wregex(L"^[ 　]+"), L"");
	//末尾のの空白削除
	aResult = boost::regex_replace(aResult, boost::wregex(L"[ 　]+$"), L"");
#else
	//先頭の空白削除
	aResult = std::tr1::regex_replace(aResult, std::tr1::wregex(L"^[ 　]+"), std::wstring(L""));
	//末尾のの空白削除
	aResult = std::tr1::regex_replace(aResult, std::tr1::wregex(L"[ 　]+$"), std::wstring(L""));
#endif
	return CString(aResult.c_str());
}

CString CCheck::fnZen2Han(CString cData)
{
	CString aResult = _T("");
		aResult = Ipc::PoiMaintenance::Utility::StringConvert::ToNarrow( gcnew System::String(cData) );
	return aResult;
}

CString CCheck::fnHan2Zen(CString cData)
{
	CString aResult = _T("");
	aResult = Ipc::PoiMaintenance::Utility::StringConvert::ToWide( gcnew System::String(cData) );
	return aResult;
}

CString CCheck::fnCutMultiSpace(CString cData)
{
//	cData = _T("　　テ    ス　　　ト");
	std::wstring aResult = cData;
#ifdef USE_BOOST_REGEX
	//2個以上連続するの空白を1個に(半角)
	aResult = boost::regex_replace(aResult, boost::wregex(L"  +"), L" ");
	//2個以上連続するの空白を1個に(全角)
	aResult = boost::regex_replace(aResult, boost::wregex(L"　　+"), L"　");
#else
	//2個以上連続するの空白を1個に(半角)
	aResult = std::tr1::regex_replace(aResult, std::tr1::wregex(L"  +"), std::wstring(L" "));
	//2個以上連続するの空白を1個に(全角)
	aResult = std::tr1::regex_replace(aResult, std::tr1::wregex(L"　　+"), std::wstring(L"　"));
#endif
	return CString(aResult.c_str());
}

CString CCheck::fnCutDoubleQuotation(CString cData, LPCTSTR cField)
{
	std::string aResult = CT2CA(cData).m_psz;
#ifdef USE_BOOST_REGEX
	if(boost::regex_match(aResult, boost::regex("^\".*\"$"))){	//両端に「"」があったら
		//先頭の「"」削除
		aResult = boost::regex_replace(aResult, boost::regex("^\""), "");
		//末尾の「"」削除
		aResult = boost::regex_replace(aResult, boost::regex("\"$"), "");
		//「""」を「"」に
//		aResult = boost::regex_replace(aResult, boost::regex("\"\""), "\"");
		_ftprintf(m_pFile, _T("ERROR,両端に「\"」があります。,%s,%s,%s\n"), static_cast<LPCTSTR>(m_Seq), cField, static_cast<LPCTSTR>(cData));
	}
#else
	if(std::tr1::regex_match(aResult, std::tr1::regex("^\".*\"$"))){	//両端に「"」があったら
		//先頭の「"」削除
		aResult = std::tr1::regex_replace(aResult, std::tr1::regex("^\""), std::string(""));
		//末尾の「"」削除
		aResult = std::tr1::regex_replace(aResult, std::tr1::regex("\"$"), std::string(""));
		//「""」を「"」に
//		aResult = boost::regex_replace(aResult, boost::regex("\"\""), "\"");
//		_ftprintf(m_pFile, _T("ERROR,両端に「\"」があります。,%s,%s,%s\n"), static_cast<LPCTSTR>(m_Seq), cField, static_cast<LPCTSTR>(cData));
	}
#endif
	return CString(aResult.c_str());
}

bool CCheck::fnNewDataCheck(void)
{
	CString aAdd(_T(""));

	for(COMP_LIST::iterator aNew = m_NewData.begin(); aNew != m_NewData.end(); aNew++)
	{
		// シーケンスセット
		m_Seq = aNew->first;

		int i = 0;
		RECORD aRec = aNew->second;
		if(m_LineNoIndex  >= 0) m_LineNo	= aRec[m_LineNoIndex];
		if(m_NameIndexNew >= 0) m_Name		= aRec[m_NameIndexNew];
		for(HEADER::iterator aHead = m_Head.begin(); aHead != m_Head.end(); aHead++, i++){
			if(aHead->Find(MAINTE_FIELD_MODIFY) < 0){
				if(!fnIsNotOrig(*aHead)){
					//元値
					if(aRec[i] != _T("")){
						if(!m_ParkMode){
							aAdd.Format(_T("%s\t%s"), static_cast<LPCTSTR>(*aHead), static_cast<LPCTSTR>(aRec[i]));
							fnOutputErr(kError, _T("新規なのに元値が入力されています。"), aAdd);
						}else{
							//機能内シーケンス、ルート探索用ポイント.*、整備作業情報（XML）.*は、駐車場でもNULLが仕様
							if(*aHead == MAINTE_FIELD_FUNC_SEQ || aHead->Find(MAINTE_FIELD_ROOT_POINT) == 0|| aHead->Find(MAINTE_FIELD_XML) == 0 || *aHead == MAINTE_FIELD_ROOT_PRI || *aHead == MAINTE_FIELD_ROOT_PRI_ATTR || *aHead == MAINTE_FIELD_ROOT_ATTR){
								aAdd.Format(_T("%s\t%s"), static_cast<LPCTSTR>(*aHead), static_cast<LPCTSTR>(aRec[i]));
								fnOutputErr(kError, _T("新規なのに元値が入力されています。"), aAdd);
							}
						}
					}else{
						if(m_ParkMode){
							//Not NULLチェック
							fnNotNullCheck(*aHead);
						}
					}
					if(*aHead == MAINTE_FIELD_INFOFILE_ID){
						fnInfoFileCheck(aRec[i]);
					}
				}
			}else{
				//修正値
				if(aRec[i] != _T("")){
					fnFieldCheck(*aHead, aRec[i], aRec);
				}else{
					if(!m_ParkMode){
						//Not NULLチェック
						fnNotNullCheck(*aHead);
					}
				}
			}
		}
	}
	return true;
}

CString CCheck::fnConvYomiSP(CString cData)
{
#if 0
	/*
	【読み】ハイフンなど→長音記号の変換
　　「―」(全角ダッシュ)
　　「‐」(全角ハイフン)
　　「－」(全角マイナス)
　　「─」（罫線）
	*/
	LPCTSTR  aHyphen[] = {
		_T("―"),
		_T("‐"),
		_T("－"),
		_T("─"),
		NULL
	};
	for(int i = 0; aHyphen[i] != NULL; i++){
		cData.Replace(aHyphen[i], _T("ー"));
	}

	/*
	【読み】特定記号の削除
	．　(全角ピリオド)
　　・　(全角中点)
　　「　(全角かぎカッコ(始))
　　」　(全角かぎカッコ(終))
　　（　(全角小カッコ(始))
　　）　(全角小カッコ(終))
　　／　(全角斜線)
　　，　(全角コンマ)
　　、　(全角読点)
　　。　(全角句点)
　　『　(二重かぎカッコ(始))
　　』　(二重かぎカッコ(終))
　　［　(全角大カッコ(始))
　　］　(全角大カッコ(終))
　　｛　(全角中カッコ(始))
　　｝　(全角中カッコ(終))
　　【　(すみつきカッコ(始))
　　】　(すみつきカッコ(終))
　　〔　(きっこうカッコ(始))
　　〕　(きっこうカッコ(終))
　　〈　(山カッコ(始))
　　〉　(山カッコ(終))
　　《　(二重山カッコ(始))
　　》　(二重山カッコ(終))
　　＜　(より小さい)
　　＞　(より大きい)
　　≪　(より非常に小さい)
　　≫　(より非常に大きい)
	*/
	LPCTSTR  aDelete[] = {
		_T("．"),//　(全角ピリオド)
		_T("・"),//　(全角中点)
		_T("「"),//　(全角かぎカッコ(始))
		_T("」"),//　(全角かぎカッコ(終))
		_T("（"),//　(全角小カッコ(始))
		_T("）"),//　(全角小カッコ(終))
		_T("／"),//　(全角斜線)
		_T("，"),//　(全角コンマ)
		_T("、"),//　(全角読点)
		_T("。"),//　(全角句点)
		_T("『"),//　(二重かぎカッコ(始))
		_T("』"),//　(二重かぎカッコ(終))
		_T("［"),//　(全角大カッコ(始))
		_T("］"),//　(全角大カッコ(終))
		_T("｛"),//　(全角中カッコ(始))
		_T("｝"),//　(全角中カッコ(終))
		_T("【"),//　(すみつきカッコ(始))
		_T("】"),//　(すみつきカッコ(終))
		_T("〔"),//　(きっこうカッコ(始))
		_T("〕"),//　(きっこうカッコ(終))
		_T("〈"),//　(山カッコ(始))
		_T("〉"),//　(山カッコ(終))
		_T("《"),//　(二重山カッコ(始))
		_T("》"),//　(二重山カッコ(終))
		_T("＜"),//　(より小さい)
		_T("＞"),//　(より大きい)
		_T("≪"),//　(より非常に小さい)
		_T("≫"),//　(より非常に大きい)
		NULL
	};
	for(int i = 0; aDelete[i] != NULL; i++){
		cData.Replace(aDelete[i], _T(""));
	}
#else
//	cData = _T("．≪―‐－─");
	cData = Ipc::PoiMaintenance::Utility::StringConvert::ToMacron( gcnew System::String(cData) );
	cData = Ipc::PoiMaintenance::Utility::StringConvert::DeleteYomiSymbols( gcnew System::String(cData) );
#endif
	return cData;
}

bool CCheck::fnNotNullCheck(CString cField)
{
	cField.Replace(MAINTE_FIELD_MODIFY, _T(""));
	for(int i = 0; NOT_NULL[i] != NULL; i++){
		if(cField == NOT_NULL[i]){
			fnOutputErr(kError, _T("新規に必要な値が入力されていません。"), cField);
			return false;
		}
	}
	return true;
}

bool CCheck::fnExistFieldCheck(void)
{
	if(m_FieldMap.find(MAINTE_FIELD_PREFNAME) != m_FieldMap.end() 
		&& m_FieldMap.find(CString(MAINTE_FIELD_PREFNAME)+CString(MAINTE_FIELD_MODIFY)) != m_FieldMap.end()
		&& m_FieldMap.find(MAINTE_FIELD_CITY_NAME) != m_FieldMap.end() 
		&& m_FieldMap.find(CString(MAINTE_FIELD_CITY_NAME)+CString(MAINTE_FIELD_MODIFY)) != m_FieldMap.end()){
		m_HaveAddr = true;
	}else{
		m_HaveAddr = false;
	}
	return true;
}

using namespace Ipc::PoiMaintenance::Sirius::Utility::SiriusSvTime::Controller;
using namespace System::Collections::Generic; 
using namespace Ipc::PoiMaintenance::Sirius::Utility::SiriusSvTime::Items;
using namespace Ipc::PoiMaintenance::Sirius::Utility::SiriusSvTime::Items::MultiSvElements;
using namespace Ipc::PoiMaintenance::Sirius::Utility::SiriusSvTime;

bool CCheck::fnCompareRecordGS(const RECORD & cOrgData, const RECORD & cNewData)
{
	bool aResult = true;
	int i = 0;
	List<GsRecord^> ^gsWeekList = gcnew List<GsRecord^>;
	List<GsRecord^> ^gsPeriodList = gcnew List<GsRecord^>;
	CString aAdd(_T(""));
	for(HEADER::iterator aHead = m_Head.begin(); aHead != m_Head.end(); aHead++, i++){
		if(fnIsWeekData(*aHead)){
			//日～土＋祝
			if(cNewData[i] == _T("")){
				fnOutputErr(kError, _T("フィールドがNULLです。"), *aHead);
				aResult = false;
			}else{
				GsWeekRecord ^aWeekRec = gcnew GsWeekRecord;
				System::String^ aCap = gcnew System::String(*aHead);
				aWeekRec->Caption = aCap;
				System::String^ aVal = gcnew System::String(cNewData[i]);
				aWeekRec->Value = aVal;
				gsWeekList->Add(aWeekRec);
			}
		}else if(aHead->Find(MAINTE_FIELD_PERIOD) == 0){
			//指定期間x
			if(cNewData[i] == _T("")){
				fnOutputErr(kError, _T("フィールドがNULLです。"), *aHead);
				aResult = false;
			}else{
				GsPeriodRecord ^aPeriodRec = gcnew GsPeriodRecord;
				System::String^ aCap = gcnew System::String(*aHead);
				aPeriodRec->Caption = aCap;
				System::String^ aVal = gcnew System::String(cNewData[i]);
				aPeriodRec->Value = aVal;
				gsPeriodList->Add(aPeriodRec);
			}
		}else{
			if(!fnIsNotOrig(*aHead)){
				if(cOrgData[i] != cNewData[i]){
					aAdd.Format(_T("%s\t%s\t%s"), static_cast<LPCTSTR>(*aHead), static_cast<LPCTSTR>(cOrgData[i]), static_cast<LPCTSTR>(cNewData[i]));
					fnOutputErr(kError, _T("元値が修正されています。"), aAdd);
				}
				if(cOrgData[i] != _T("")){
					fnFieldCheck(*aHead, cOrgData[i], cOrgData);
				}
			}
		}
	}
	if(aResult){
		GsStringAnalyzer^ analyzer = gcnew GsStringAnalyzer;
		MultiSvAnalyzeResult^ result = analyzer->Execute( gsWeekList, gsPeriodList );
		if( result->ResultStatus == SvResult::Error )
		{
	//		SvErr2stSiRiUSErr( result->ErrorList, listErrs );
			fnOutputErr(kError, _T("営業時間の解析に失敗しました。"));
			return false;
		}
		// 複数営業時間に変換
		MultiSvDataAnalyzer^ analyzer2 = MultiSvDataAnalyzer::CreateInstance();
		analyzer2->Execute( result );
		if( result->ResultStatus == SvResult::Error )
		{
	//		SvErr2stSiRiUSErr( result->ErrorList, listErrs );
			fnOutputErr(kError, _T("複数営業時間の変換に失敗しました。"));
			return false;
		}
	}
	return aResult;
}

bool CCheck::fnIsWeekData(const CString& cData)
{
	for(int i= 0; GS_WEEK[i] != NULL; i++){
		if(cData == GS_WEEK[i]){
			return true;
		}
	}
	return false;
}

bool CCheck::fnIsNotOrig(const CString& cData)
{
	for(int i= 0; NOT_ORIG[i] != NULL; i++){
		if(cData == NOT_ORIG[i]){
			return true;
		}
	}
	return false;
}

bool CCheck::fnLoadMikakuMaster(LPCTSTR cMMaster)
{
	// ファイル読み込み
	std::ifstream ifs( cMMaster );
	if( ! ifs.is_open() ){
		_ftprintf(m_pFile, _T("#ERROR\tファイルが開けません。\t%s\n"), cMMaster);
		return false;
	}

	std::string line;
	if( getline( ifs, line ) ){	//ヘッダ読み飛ばし
		while( getline( ifs, line ) ){
			RECORD aRecord;
			char_separator a_csSep("\t", "", boost::keep_empty_tokens); // 分割設定
			tokenizer	a_tTokens(line, a_csSep);	// 分割
			foreach( const std::string& str, std::make_pair( a_tTokens.begin(), a_tTokens.end()) ){
				aRecord.push_back(CString(str.c_str()));
			}
			m_Mikaku.insert(aRecord[1]);
		}
	}
	return true;
}

bool CCheck::fnSetDeleteInfo(DELETE_INFO& cDelInfo, CString cField, const CString cData)
{
	cField.Replace(MAINTE_FIELD_WINDOW, _T(""));
	cField.Replace(MAINTE_FIELD_ATM, _T(""));
	if(cField.Find(MAINTE_FIELD_BANK_DELETE_FLAG) == 0){
		//削除フラグ
		cDelInfo.m_DeleteCode = _ttol(cData);
	}else if(cField.Find(MAINTE_FIELD_BABK_CLOSED_CODE) == 0){
		//閉鎖理由コード
		cDelInfo.m_ClosedCode = cData;
	}else if(cField.Find(MAINTE_FIELD_BANK_CLOSED_MEMO) == 0){
		//閉鎖日付・理由
		cDelInfo.m_ClosedMemo = cData;
	}
	return true;
}

CString CCheck::fnCompRegulate(const CString& cField, CString cData)
{
	if(cData == _T("")){
		return cData;
	}
	CString aData = _T("");
	if(cField.Find(_T("日付")) > 0){
		std::string aDate = CT2CA(cData).m_psz;
		char_separator a_csSep("/", "", boost::keep_empty_tokens); // 分割設定
		tokenizer	a_tTokens(aDate, a_csSep);	// 分割
		RECORD aRec;
		std::vector<CString> aTmpList;
		foreach( const std::string& aStr, std::make_pair( a_tTokens.begin(), a_tTokens.end()) ){
			CString aTmp(aStr.c_str());
			aTmpList.push_back(aTmp);
		}
		int aYYYY= _ttol(aTmpList[0]);
		int aMM= _ttol(aTmpList[1]);
		int aDD= _ttol(aTmpList[2]);
		aData.Format(_T("%4d/%02d/%02d"), aYYYY, aMM, aDD);
	}else{
		aData = fnRegulate(cField, cData);
	}
	return aData;
}

// 座標書式チェック
bool CCheck::fnCoordCheck( const CString& strField, const CString& strData )
{
	// リストの座標フォーマットが1/256か度分秒かは判別できないので、
	// 大勢を占める 1/256 チェックを実施し、エラーがあれば度分秒を実施
	// 共にNGならエラーとする
	// (1/256なのに度分秒があるケースはエラーとならないが、運用上ありえないということで問題なし)
	std::string strTmpData( CT2CA(strData).m_psz );

	// 経度
	bool b256Error = false;
	if( strField.Find(MAINTE_FIELD_LONG) >= 0 )
	{
		// まず1/256チェック
		if( std::tr1::regex_match(strTmpData, std::tr1::regex("[0-9]{9}")) )
			b256Error = _ttol(strData) < 112435200 || _ttol(strData) > 143358965;
		else
			b256Error = true;

		// 1/256でNGなら、度分秒チェック
		if( b256Error )
		{
			// 秒で小数点以下ありバージョンとなしバージョンどちらにも対応
			return std::tr1::regex_match(strTmpData, std::tr1::regex("E1[2-5]{1}[0-9]{1}[.][0-5]{0,1}[0-9]{1}[.][0-5]{0,1}[0-9]{1}[.][0-9]{1,2}"))
				|| std::tr1::regex_match(strTmpData, std::tr1::regex("E1[2-5]{1}[0-9]{1}[.][0-5]{0,1}[0-9]{1}[.][0-5]{0,1}[0-9]{1}"));
		}
		else
			return true;

	}
	// 緯度
	else
	{
		// まず1/256チェック
		if( std::tr1::regex_match(strTmpData, std::tr1::regex("[0-9]{8}")) )
			b256Error = _ttol(strData) < 17510400 || _ttol(strData) > 48434165;
		else
			b256Error = true;

		// 1/256でNGなら、度分秒チェック
		if( b256Error )
		{
			// 秒で小数点以下ありバージョンとなしバージョンどちらにも対応
			return std::tr1::regex_match(strTmpData, std::tr1::regex("N[1-5]{1}[0-9]{1}[.][0-5]{0,1}[0-9]{1}[.][0-5]{0,1}[0-9]{1}[.][0-9]{1,2}"))
				   || std::tr1::regex_match(strTmpData, std::tr1::regex("N[1-5]{1}[0-9]{1}[.][0-5]{0,1}[0-9]{1}[.][0-5]{0,1}[0-9]{1}"));
		}
		else
			return true;
	}
}

// エラーログの出力
void CCheck::fnOutputErr(EErrLv cErrLv, const CString &cMsg, const CString &cAdd)
{
	CString aErrLv(_T("ERROR"));
	if(cErrLv == kWarning) aErrLv = _T("WARNING");

	_ftprintf(m_pFile, _T("%s\t%s\t%s\t%s\t%s\t%s\n"), static_cast<LPCTSTR>(aErrLv), static_cast<LPCTSTR>(cMsg),
		static_cast<LPCTSTR>(m_LineNo), static_cast<LPCTSTR>(m_Seq), static_cast<LPCTSTR>(m_Name), static_cast<LPCTSTR>(cAdd));
}
