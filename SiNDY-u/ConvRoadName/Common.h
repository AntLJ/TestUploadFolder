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

#pragma once

/**
 * @brief 出力ファイル名定義
 */
const LPCTSTR ROAD_NAME = _T("road_name");
const LPCTSTR REL_ROAD_NAME_LINK = _T("rel_road_name_link");
const LPCTSTR SPLIT_ROAD_NAME = _T("split_road_name");


// 渡り線の場合に、元のROAD_CODEにプラスされる値
const long CROSS_OVER_PLUS_VALUE = 200000;
// 渡り線のROAD_CODE(CROSS_OVER_PLUS_VALUEを足した値)の範囲
const long CROSS_OVER_CODE_LOW = 400000;
const long CROSS_OVER_CODE_HIG = 599999;
// 高速無料区間の場合に、元のROAD_CODEにプラスされる値
const long FREE_HWY_PLUS_VALUE = 400000;
// 高速無料区間のROAD_CODEの範囲
const long FREE_HWY_CODE_LOW = 600000;
const long FREE_HWY_CODE_HIG = 799999;

/**
 * @brief 道路名称データレコード格納用
 */
class RoadNameRecord {
public:
	RoadNameRecord()
		:m_nameID(-1), m_nameCls(-1) {}
	RoadNameRecord(const long& nameID, const long& nameCls, const CString& name, const CString& yomi)
		:m_nameID(nameID), m_nameCls(nameCls), m_name(name), m_yomi(yomi) {}
	~RoadNameRecord() {}
public:
	bool operator < (const RoadNameRecord& roadNameRecord) const
	{
		return this->m_nameID < roadNameRecord.m_nameID;
	}
	bool operator == (const RoadNameRecord& roadNameRecord) const
	{
		return this->m_nameID == roadNameRecord.m_nameID;
	}
public:
	long m_nameID;
	long m_nameCls;
	CString m_name;
	CString m_yomi;
};

/**
 * @brief 道路リンクに紐付く道路名称レコード格納用
 */
class RelNameLinkRecord {
public:
	RelNameLinkRecord()
		:m_roadOID(-1), m_nameID(-1), m_nameCls(-1) {}
	RelNameLinkRecord(const long& roadOID, const long& nameID, const long& nameCls)
		:m_roadOID(roadOID), m_nameID(nameID), m_nameCls(nameCls) {}
	~RelNameLinkRecord() {}
public:
	bool operator < (const RelNameLinkRecord& relNameLinkRecord) const
	{
		return this->m_roadOID != relNameLinkRecord.m_roadOID ? this->m_roadOID < relNameLinkRecord.m_roadOID :
			this->m_nameID < relNameLinkRecord.m_nameID;
	}
	bool operator == (const RelNameLinkRecord& relNameLinkRecord) const
	{
		return this->m_roadOID == relNameLinkRecord.m_roadOID &&
			this->m_nameID == relNameLinkRecord.m_nameID;
	}
public:
	long m_roadOID;
	long m_nameID;
	long m_nameCls;
};

/**
 * @brief 道路名称分割データレコード格納用
 */
class SpltNameRecord {
public:
	SpltNameRecord()
		:m_nameID(-1), m_spltCls(-1), m_spltSeq(-1) {}
	SpltNameRecord(const long& nameID, const long& spltCls, const long& spltSeq,
		const CString& spltName, const CString& spltYomi)
		:m_nameID(nameID), m_spltCls(spltCls), m_spltSeq(spltSeq), m_spltName(spltName), m_spltYomi(spltYomi) {}
	~SpltNameRecord() {}
public:
	bool operator < (const SpltNameRecord& spltNameRecord) const
	{
		return this->m_nameID != spltNameRecord.m_nameID ? this->m_nameID < spltNameRecord.m_nameID :
			this->m_spltSeq < spltNameRecord.m_spltSeq;
	}
	bool operator == (const SpltNameRecord& spltNameRecord) const
	{
		return this->m_nameID == spltNameRecord.m_nameID &&
			this->m_spltSeq == spltNameRecord.m_spltSeq;
	}
public:
	long m_nameID;
	long m_spltCls;
	long m_spltSeq;
	CString m_spltName;
	CString m_spltYomi;
};

/**
 * @brief 日時文字列取得
 * @return 日時文字列
 */
inline CString getNowTime()
{
	CString strTime;
	
	SYSTEMTIME sysTime;
	GetLocalTime(&sysTime);
	strTime.Format(_T("%04d/%02d/%02d %02d:%02d:%02d"), sysTime.wYear, sysTime.wMonth, sysTime.wDay, sysTime.wHour, sysTime.wMinute, sysTime.wSecond);

	return strTime;
}

/**
 * @brief OID文字列取得
 * @param ipFeature [in] フィーチャ
 * @return OID文字列
 */
inline CString getID(const IFeaturePtr& ipFeature)
{
	CString strOID;
	long lOID = 0;
	if(S_OK != ipFeature->get_OID(&lOID)){
		// エラーなら例外で、、、
		throw std::runtime_error("OID取得失敗");
	}
	strOID.Format(_T("%ld"), lOID);

	return strOID;
}

/**
 * @brief 半角数字を全角数字文字列に変換
 * @param lNumber [in] 半角数字
 * @return 全角数字文字列
 */
inline CString num2ZenStr(long lNumber)
{
	// 桁数取得
	long digit = log10(lNumber) + 1;

	CString strZenNumber;
	for(long n = digit; n > 0; --n){
		long perNum = pow(10,(n-1));
		long digNum = long(lNumber / perNum);
		lNumber = lNumber % perNum;
		CString strDigNum;

		switch(digNum){
		case 0: strDigNum = _T("０");	break;
		case 1: strDigNum = _T("１");	break;
		case 2: strDigNum = _T("２");	break;
		case 3: strDigNum = _T("３");	break;
		case 4: strDigNum = _T("４");	break;
		case 5:	strDigNum = _T("５");	break;
		case 6:	strDigNum = _T("６");	break;
		case 7:	strDigNum = _T("７");	break;
		case 8:	strDigNum = _T("８");	break;
		case 9:	strDigNum = _T("９");	break;
		default:
			// ここにはこないはず、、
			throw std::runtime_error("数字NG");
			break;
		}
		strZenNumber += strDigNum;
	}
	return strZenNumber;
}

/**
 * @brief 半角数字をカナ読みに変換（5桁まで）
 * @param lNumber [in] 半角数字
 * @return 数字のカナ読み
 */
inline CString num2YomiStr(long lNumber)
{
	// 桁数取得
	long digit = log10(lNumber) + 1;

	CString strYomi;
	for(long n = digit; n > 0; --n){
		long perNum = pow(10,(n-1));
		long digNum = long(lNumber / perNum);
		lNumber = lNumber % perNum;
		CString strDigYomi;

		// 指定桁の数値の読み設定
		switch(digNum){
		case 1:
			// 特殊処理
			// 一、万の位の場合は読みに「イチ」が付与されるが、十、百、千の位の場合は読み不要
			// 例）11111→「イチマンセンヒャクジュウイチ」
			switch(n){
			case 1:
			case 5:
				strDigYomi = _T("イチ");
				break;
			default:
				break;
			}
			break;
		case 2:
			strDigYomi = _T("ニ");
			break;
		case 3:
			strDigYomi = _T("サン");
			break;
		case 4:
			strDigYomi = _T("ヨン");
			break;
		case 5:
			strDigYomi = _T("ゴ");
			break;
		case 6:
			// 特殊処理
			// 百の位の場合は読みが変わる
			// 例）66666→「ロクマンロクセンロッピャクロクジュウロク」
			if(3 == n){
				strDigYomi = _T("ロッ");
			} else {
				strDigYomi = _T("ロク");
			}
			break;
		case 7:
			strDigYomi = _T("ナナ");
			break;
		case 8:
			// 特殊処理
			// 百、千の位の場合は読みが変わる
			// 例）88888→「ハチマンハッセンハッピャクハチジュウハチ」
			switch(n){
			case 3:
			case 4:
				strDigYomi = _T("ハッ");
				break;
			default:
				strDigYomi = _T("ハチ");
				break;
			}
			break;
		case 9:
			strDigYomi = _T("キュウ");
			break;
		default:
			break;
		}
		// 一～万の位の読み設定
		if(0 != digNum){
			switch(n){
			case 0:
				break;
			case 1:
				break;
			case 2:
				strDigYomi += _T("ジュウ");
				break;
			case 3:
				// 百の位の特殊処理
				// 3,6,8の場合は読みが特殊
				switch(digNum){
				case 3:
					strDigYomi += _T("ビャク");
					break;
				case 6:
				case 8:
					strDigYomi += _T("ピャク");
					break;
				default:
					strDigYomi += _T("ヒャク");
					break;
				}
				break;
			case 4:
				// 千の位の特殊処理
				// 3の場合は読みが特殊
				switch(digNum){
				case 3:
					strDigYomi += _T("ゼン");
					break;
				default:
					strDigYomi += _T("セン");
					break;
				}
				break;
			case 5:
				strDigYomi += _T("マン");
				break;
			default:
				// ここにはこないはず、、
				throw std::runtime_error("数字NG");
				break;
			}
		}
		strYomi += strDigYomi;
	}
	return strYomi;
}

/**
 * @brief 県コードから県名取得
 * @param lPrefCode [in] 県コード
 * @return 県名称
 */
inline CString prefCode2PrefName(long lPrefCode)
{
	CString strPrefName;
	switch(lPrefCode){
	case 1:		strPrefName = _T("北海道");		break;
	case 2:		strPrefName = _T("青森県");		break;
	case 3:		strPrefName = _T("岩手県");		break;
	case 4:		strPrefName = _T("宮城県");		break;
	case 5:		strPrefName = _T("秋田県");		break;
	case 6:		strPrefName = _T("山形県");		break;
	case 7:		strPrefName = _T("福島県");		break;
	case 8:		strPrefName = _T("茨城県");		break;
	case 9:		strPrefName = _T("栃木県");		break;
	case 10:	strPrefName = _T("群馬県");		break;
	case 11:	strPrefName = _T("埼玉県");		break;
	case 12:	strPrefName = _T("千葉県");		break;
	case 13:	strPrefName = _T("東京都");		break;
	case 14:	strPrefName = _T("神奈川県");	break;
	case 15:	strPrefName = _T("新潟県");		break;
	case 16:	strPrefName = _T("富山県");		break;
	case 17:	strPrefName = _T("石川県");		break;
	case 18:	strPrefName = _T("福井県");		break;
	case 19:	strPrefName = _T("山梨県");		break;
	case 20:	strPrefName = _T("長野県");		break;
	case 21:	strPrefName = _T("岐阜県");		break;
	case 22:	strPrefName = _T("静岡県");		break;
	case 23:	strPrefName = _T("愛知県");		break;
	case 24:	strPrefName = _T("三重県");		break;
	case 25:	strPrefName = _T("滋賀県");		break;
	case 26:	strPrefName = _T("京都府");		break;
	case 27:	strPrefName = _T("大阪府");		break;
	case 28:	strPrefName = _T("兵庫県");		break;
	case 29:	strPrefName = _T("奈良県");		break;
	case 30:	strPrefName = _T("和歌山県");	break;
	case 31:	strPrefName = _T("鳥取県");		break;
	case 32:	strPrefName = _T("島根県");		break;
	case 33:	strPrefName = _T("岡山県");		break;
	case 34:	strPrefName = _T("広島県");		break;
	case 35:	strPrefName = _T("山口県");		break;
	case 36:	strPrefName = _T("徳島県");		break;
	case 37:	strPrefName = _T("香川県");		break;
	case 38:	strPrefName = _T("愛媛県");		break;
	case 39:	strPrefName = _T("高知県");		break;
	case 40:	strPrefName = _T("福岡県");		break;
	case 41:	strPrefName = _T("佐賀県");		break;
	case 42:	strPrefName = _T("長崎県");		break;
	case 43:	strPrefName = _T("熊本県");		break;
	case 44:	strPrefName = _T("大分県");		break;
	case 45:	strPrefName = _T("宮崎県");		break;
	case 46:	strPrefName = _T("鹿児島県");	break;
	case 47:	strPrefName = _T("沖縄県");		break;
	default:
		// ここにはこないはず、、
		throw std::runtime_error("県コードNG");
		break;
	}
	return strPrefName;
}

/**
 * @brief 県コードから県読み取得
 * @param lPrefCode [in] 県コード
 * @return 県読み
 */
inline CString prefCode2PrefYomi(long lPrefCode)
{
	CString strPrefYomi;
	switch(lPrefCode){
	case 1:		strPrefYomi = _T("ホッカイドウ");	break;
	case 2:		strPrefYomi = _T("アオモリケン");	break;
	case 3:		strPrefYomi = _T("イワテケン");		break;
	case 4:		strPrefYomi = _T("ミヤギケン");		break;
	case 5:		strPrefYomi = _T("アキタケン");		break;
	case 6:		strPrefYomi = _T("ヤマガタケン");	break;
	case 7:		strPrefYomi = _T("フクシマケン");	break;
	case 8:		strPrefYomi = _T("イバラキケン");	break;
	case 9:		strPrefYomi = _T("トチギケン");		break;
	case 10:	strPrefYomi = _T("グンマケン");		break;
	case 11:	strPrefYomi = _T("サイタマケン");	break;
	case 12:	strPrefYomi = _T("チバケン");		break;
	case 13:	strPrefYomi = _T("トウキョウト");	break;
	case 14:	strPrefYomi = _T("カナガワケン");	break;
	case 15:	strPrefYomi = _T("ニイガタケン");	break;
	case 16:	strPrefYomi = _T("トヤマケン");		break;
	case 17:	strPrefYomi = _T("イシカワケン");	break;
	case 18:	strPrefYomi = _T("フクイケン");		break;
	case 19:	strPrefYomi = _T("ヤマナシケン");	break;
	case 20:	strPrefYomi = _T("ナガノケン");		break;
	case 21:	strPrefYomi = _T("ギフケン");		break;
	case 22:	strPrefYomi = _T("シズオカケン");	break;
	case 23:	strPrefYomi = _T("アイチケン");		break;
	case 24:	strPrefYomi = _T("ミエケン");		break;
	case 25:	strPrefYomi = _T("シガケン");		break;
	case 26:	strPrefYomi = _T("キョウトフ");		break;
	case 27:	strPrefYomi = _T("オオサカフ");		break;
	case 28:	strPrefYomi = _T("ヒョウゴケン");	break;
	case 29:	strPrefYomi = _T("ナラケン");		break;
	case 30:	strPrefYomi = _T("ワカヤマケン");	break;
	case 31:	strPrefYomi = _T("トットリケン");	break;
	case 32:	strPrefYomi = _T("シマネケン");		break;
	case 33:	strPrefYomi = _T("オカヤマケン");	break;
	case 34:	strPrefYomi = _T("ヒロシマケン");	break;
	case 35:	strPrefYomi = _T("ヤマグチケン");	break;
	case 36:	strPrefYomi = _T("トクシマケン");	break;
	case 37:	strPrefYomi = _T("カガワケン");		break;
	case 38:	strPrefYomi = _T("エヒメケン");		break;
	case 39:	strPrefYomi = _T("コウチケン");		break;
	case 40:	strPrefYomi = _T("フクオカケン");	break;
	case 41:	strPrefYomi = _T("サガケン");		break;
	case 42:	strPrefYomi = _T("ナガサキケン");	break;
	case 43:	strPrefYomi = _T("クマモトケン");	break;
	case 44:	strPrefYomi = _T("オオイタケン");	break;
	case 45:	strPrefYomi = _T("ミヤザキケン");	break;
	case 46:	strPrefYomi = _T("カゴシマケン");	break;
	case 47:	strPrefYomi = _T("オキナワケン");	break;
	default:
		// ここにはこないはず、、
		throw std::runtime_error("県コードNG");
		break;
	}
	return strPrefYomi;
}

/**
 * @brief 県コードから接尾辞（都道府県）名取得
 * @param lPrefCode [in] 県コード
 * @return 接尾辞名称
 */
inline CString prefCode2PrefSuffixName(long lPrefCode)
{
	CString strPrefSuffixName;
	switch(lPrefCode){
	case 1:
		strPrefSuffixName = _T("道");
		break;
	case 13:
		strPrefSuffixName = _T("都");
		break;
	case 26:
	case 27:
		strPrefSuffixName = _T("府");
		break;
		break;
	default:
		strPrefSuffixName = _T("県");
		break;
	}
	return strPrefSuffixName;
}

/**
 * @brief 県コードから接尾辞（都道府県）読み取得
 * @param lPrefCode [in] 県コード
 * @return 接尾辞読み
 */
inline CString prefCode2PrefSuffixYomi(long lPrefCode)
{
	CString strPrefSuffixYomi;
	switch(lPrefCode){
	case 1:
		strPrefSuffixYomi = _T("ドウ");
		break;
	case 13:
		strPrefSuffixYomi = _T("ト");
		break;
	case 26:
	case 27:
		strPrefSuffixYomi = _T("フ");
		break;
		break;
	default:
		strPrefSuffixYomi = _T("ケン");
		break;
	}
	return strPrefSuffixYomi;
}
