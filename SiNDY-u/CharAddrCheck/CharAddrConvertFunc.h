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

#include "ADOBase.h"

#include "../../LIB/jis/x0208/spirit/shift_jis.h"
#include <boost/assign/std/vector.hpp>

using namespace boost::spirit;
using namespace jis::x0208::spirit;

enum GouOrCityAddrLayer
{
	None_Layer			= 0,
	CityAdmin_Layer		= 1,
	GouPoint_Layer		= 2,
};

enum OutputType
{
	SplitCharAddr		= 0,
	GetYomiFromCharAddr	= 1,
};

enum YomiType
{
	NoneType			= 0,
	NumberType			= 1,
	AlphabetType		= 2,
	HiraKataType		= 3,
	KanjiYomiTableType	= 4,
};

enum CharAddrType
{
	NoneCharType	= 0,
	ObjectNameType	= 1,
	ObjectYomiType	= 2,
	KanjiNameType	= 3,
	KanjiYomiType	= 4,
};

typedef std::pair<GouOrCityAddrLayer,long>		ObjectLayerPair;
typedef std::pair<std::string,std::string>		KanjiYomiPair;

/**
* @brief	サブテーブルクラス
* @note		アルファベットなどの読み用テーブル
*/
class CSubConvertTable
{
public:
	CSubConvertTable(){};

	~CSubConvertTable(){};

public:
	std::map<std::wstring,std::wstring> GetSubtable() { return m_mapSubTable; }
	
	sjis_ranges getAlphabet() { return m_sjis_alphabet_p; }
	sjis_ranges getNumber() { return m_sjis_num_p; }
	sjis_ranges getKanjiKana() { return m_sjis_KanjiKana_p; } 
	sjis_ranges getKana() { return m_sjis_kana_p2; } 
	sjis_ranges getObjectName() { return m_sjis_ObjectName_p; }
	sjis_ranges getObjectYomi() { return m_sjis_ObjectYomi_p; }
	sjis_ranges getKanjiName() { return m_sjis_KanjiName_p; }
	sjis_ranges getKanjiYomi() { return m_sjis_KanjiYomi_p; }
	sjis_ranges getHyphen() { return m_sjis_hyphen_p; }
	void Init()
	{

		// 半全角英字
		m_sjis_alphabet_p = sjis_ranges().add(sjis_range(0x41,0x5A)).add(sjis_range(0x61,0x7A)).add(sjis_alpha_p);
		// 半全角数字
		m_sjis_num_p = sjis_ranges().add(sjis_range(0x30,0x39)).add(sjis_digit_p);
		// 全角カナ（ヰヱを除く）
		m_sjis_katakana_p2 = sjis_ranges().add(sjis_range(0x8340,0x838F)).add(sjis_range(0x8392,0x8396));
		// 全角かなカナ＋[ー]
		m_sjis_kana_p2 = sjis_ranges().add(sjis_kana_p).add(sjis_range(0x815B,0x815B));
		// sjis第一、第二水準漢字＋全角かなカナ＋[ー々]
		m_sjis_KanjiKana_p = sjis_ranges().add(sjis_kanji_p).add(sjis_kana_p).add(sjis_range(0x815B,0x815B)).add(sjis_range(0x8158,0x8158));
		// [-]
		m_sjis_hyphen_p = sjis_ranges().add(sjis_range(0x2d,0x2d));

		// sjis第一、第二水準漢字＋全角かなカナ＋全角英字＋半角数字＋[ー-々]
		m_sjis_ObjectName_p = sjis_ranges().add(sjis_kanji_p).add(sjis_kana_p).add(sjis_alpha_p).add(sjis_range(0x30,0x39)).add(sjis_range(0x815B,0x815B)).add(sjis_range(0x2d,0x2d)).add(sjis_range(0x8158,0x8158));
		// 全角数字カナ（ヰヱ以外）＋[ー－]
		m_sjis_ObjectYomi_p = sjis_ranges().add(m_sjis_katakana_p2).add(sjis_digit_p).add(sjis_range(0x815B,0x815B)).add(sjis_range(0x817C,0x817C));
		// sjis第一、第二水準漢字＋全角英字＋全角かなカナ＋[ー々]
		m_sjis_KanjiName_p = sjis_ranges().add(sjis_kanji_p).add(sjis_alpha_p).add(sjis_kana_p).add(sjis_range(0x815B,0x815B)).add(sjis_range(0x8158,0x8158));
		// 全角カナ（ヰヱ以外）＋[ー]
		m_sjis_KanjiYomi_p = sjis_ranges().add(m_sjis_katakana_p2).add(sjis_range(0x815B,0x815B));

		// insertで記述したほうが処理が早いんだけど100件程度だからいいか…
		m_mapSubTable[L"a"] = L"エー";			m_mapSubTable[L"A"] = L"エー";			m_mapSubTable[L"ａ"] = L"エー";			m_mapSubTable[L"Ａ"] = L"エー";
		m_mapSubTable[L"b"] = L"ビー";			m_mapSubTable[L"B"] = L"ビー";			m_mapSubTable[L"ｂ"] = L"ビー";			m_mapSubTable[L"Ｂ"] = L"ビー";
		m_mapSubTable[L"c"] = L"シー";			m_mapSubTable[L"C"] = L"シー";			m_mapSubTable[L"ｃ"] = L"シー";			m_mapSubTable[L"Ｃ"] = L"シー";
		m_mapSubTable[L"d"] = L"ディー";		m_mapSubTable[L"D"] = L"ディー";		m_mapSubTable[L"ｄ"] = L"ディー";			m_mapSubTable[L"Ｄ"] = L"ディー";
		m_mapSubTable[L"e"] = L"イー";			m_mapSubTable[L"E"] = L"イー";			m_mapSubTable[L"ｅ"] = L"イー";			m_mapSubTable[L"Ｅ"] = L"イー";
		m_mapSubTable[L"f"] = L"エフ";			m_mapSubTable[L"F"] = L"エフ";			m_mapSubTable[L"ｆ"] = L"エフ";			m_mapSubTable[L"Ｆ"] = L"エフ";
		m_mapSubTable[L"g"] = L"ジー";			m_mapSubTable[L"G"] = L"ジー";			m_mapSubTable[L"ｇ"] = L"ジー";			m_mapSubTable[L"Ｇ"] = L"ジー";
		m_mapSubTable[L"h"] = L"エイチ";		m_mapSubTable[L"H"] = L"エイチ";		m_mapSubTable[L"ｈ"] = L"エイチ";			m_mapSubTable[L"Ｈ"] = L"エイチ";
		m_mapSubTable[L"i"] = L"アイ";			m_mapSubTable[L"I"] = L"アイ";			m_mapSubTable[L"ｉ"] = L"アイ";			m_mapSubTable[L"Ｉ"] = L"アイ";
		m_mapSubTable[L"j"] = L"ジェイ";		m_mapSubTable[L"J"] = L"ジェイ";		m_mapSubTable[L"ｊ"] = L"ジェイ";			m_mapSubTable[L"Ｊ"] = L"ジェイ";
		m_mapSubTable[L"k"] = L"ケー";			m_mapSubTable[L"K"] = L"ケー";			m_mapSubTable[L"ｋ"] = L"ケー";			m_mapSubTable[L"Ｋ"] = L"ケー";
		m_mapSubTable[L"l"] = L"エル";			m_mapSubTable[L"L"] = L"エル";			m_mapSubTable[L"ｌ"] = L"エル";			m_mapSubTable[L"Ｌ"] = L"エル";
		m_mapSubTable[L"m"] = L"エム";			m_mapSubTable[L"M"] = L"エム";			m_mapSubTable[L"ｍ"] = L"エム";			m_mapSubTable[L"Ｍ"] = L"エム";
		m_mapSubTable[L"n"] = L"エヌ";			m_mapSubTable[L"N"] = L"エヌ";			m_mapSubTable[L"ｎ"] = L"エヌ";			m_mapSubTable[L"Ｎ"] = L"エヌ";
		m_mapSubTable[L"o"] = L"オー";			m_mapSubTable[L"O"] = L"オー";			m_mapSubTable[L"ｏ"] = L"オー";			m_mapSubTable[L"Ｏ"] = L"オー";
		m_mapSubTable[L"p"] = L"ピー";			m_mapSubTable[L"P"] = L"ピー";			m_mapSubTable[L"ｐ"] = L"ピー";			m_mapSubTable[L"Ｐ"] = L"ピー";
		m_mapSubTable[L"q"] = L"キュー";		m_mapSubTable[L"Q"] = L"キュー";		m_mapSubTable[L"ｑ"] = L"キュー";			m_mapSubTable[L"Ｑ"] = L"キュー";
		m_mapSubTable[L"r"] = L"アール";		m_mapSubTable[L"R"] = L"アール";		m_mapSubTable[L"ｒ"] = L"アール";			m_mapSubTable[L"Ｒ"] = L"アール";
		m_mapSubTable[L"s"] = L"エス";			m_mapSubTable[L"S"] = L"エス";			m_mapSubTable[L"ｓ"] = L"エス";			m_mapSubTable[L"Ｓ"] = L"エス";
		m_mapSubTable[L"t"] = L"ティー";		m_mapSubTable[L"T"] = L"ティー";		m_mapSubTable[L"ｔ"] = L"ティー";			m_mapSubTable[L"Ｔ"] = L"ティー";
		m_mapSubTable[L"u"] = L"ユー";			m_mapSubTable[L"U"] = L"ユー";			m_mapSubTable[L"ｕ"] = L"ユー";			m_mapSubTable[L"Ｕ"] = L"ユー";
		m_mapSubTable[L"v"] = L"ブイ";			m_mapSubTable[L"V"] = L"ブイ";			m_mapSubTable[L"ｖ"] = L"ブイ";			m_mapSubTable[L"Ｖ"] = L"ブイ";
		m_mapSubTable[L"w"] = L"ダブリュー";	m_mapSubTable[L"W"] = L"ダブリュー";	m_mapSubTable[L"ｗ"] = L"ダブリュー";		m_mapSubTable[L"Ｗ"] = L"ダブリュー";
		m_mapSubTable[L"x"] = L"エックス";		m_mapSubTable[L"X"] = L"エックス";		m_mapSubTable[L"ｘ"] = L"エックス";		m_mapSubTable[L"Ｘ"] = L"エックス";
		m_mapSubTable[L"y"] = L"ワイ";			m_mapSubTable[L"Y"] = L"ワイ";			m_mapSubTable[L"ｙ"] = L"ワイ";			m_mapSubTable[L"Ｙ"] = L"ワイ";
		m_mapSubTable[L"z"] = L"ゼット";		m_mapSubTable[L"Z"] = L"ゼット";		m_mapSubTable[L"ｚ"] = L"ゼット";			m_mapSubTable[L"Ｚ"] = L"ゼット";
	}

private:
	std::map<std::wstring,std::wstring> m_mapSubTable;

	sjis_ranges m_sjis_ObjectName_p;	//!< sjis第一、第二漢字＋全角英数字かなカナ＋[ー-々]＋全角英字＋半角数字
	sjis_ranges m_sjis_ObjectYomi_p;	//!< 全角数字カナ（ヰヱを除く）＋[ー－]
	sjis_ranges m_sjis_KanjiName_p;		//!< sjis第一、第二漢字＋全角アルファベット＋全角かなカナ＋[ー々]
	sjis_ranges m_sjis_KanjiYomi_p;		//!< 全角カナ（ヰヱを除く）＋[ー]

	sjis_ranges m_sjis_alphabet_p;	//!< 半全角英字
	sjis_ranges m_sjis_num_p;		//!< 半全角数字
	sjis_ranges m_sjis_katakana_p2;	//!< 全角カナ（ヰヱを除く）
	sjis_ranges m_sjis_kana_p2;		//!< 全角かなカナ＋[ー]
	sjis_ranges m_sjis_KanjiKana_p;	//!< sjis第一、第二漢字＋全角かなカナ＋[ー々]
	sjis_ranges m_sjis_hyphen_p;	//!< [-]
};


/**
* @brief	文字付き住所の読み変換クラス
*/
class CCharAddrConvert
{
public:
	CCharAddrConvert(){};
	~CCharAddrConvert(){};

public:
	/**
	* @brief	漢字読みテーブル、サブ読みテーブル初期化
	* @param	lpcszCovnertTablePath[in]	読み変換テーブルのPGDBパス
	* @return	漢字読みテーブル変換エラーの出力（出力サイズが0ならOK）
	*/
	std::multimap<long,CString> Init( LPCTSTR lpcszCovnertTablePath );

	/**
	* @brief	文字付き住所をオブジェクトID読み変換テーブルで変換して出力
	* @param	LayerType[in]				レイヤタイプ（GouOrCityAddrLayerを参照　CityAdmin_Layer(1):都市地図行政界,GouPoint_Layer(2):号ポイント）
	* @param	lOID[in]					オブジェクトID
	* @param	lpcszCharAddr[in]			文字付き住所名称
	* @param	bRet[in,out]				読み変換 true:成功,false:失敗
	* @return	変換した後の文字列を出力（失敗したらNULL）
	*/
	std::string GetYomiFromCharAddr_Object( GouOrCityAddrLayer LayerType, long lOID, LPCTSTR lpcszCharAddr, bool& bRet );

	/**
	* @brief	文字付き住所を漢字読み変換テーブルで変換して出力
	*
	* @note		例）　12-A1高橋C21-45山田あ89-はひふ34
	*			成功例）１２－Ａ１タカハシＣ２１－４５ヤマダア８９－ハヒフ３４
	*			失敗例）１２－Ａ１タカハシＣ２１－４５山田あ８９－ハヒフ３４
	*
	* @param	lpcszCharAddr[in]			文字付き住所名称
	* @param	bRet[in,out]				読み変換 true:成功,false:失敗
	* @return	変換した後の文字列を出力（失敗しても読みの出力は返す）
	*/
	std::string GetYomiFromCharAddr_Kanji( LPCTSTR lpcszCharAddr, bool& bRet );

	/**
	* @brief	文字付き住所の文字列を単語毎に分割したリストを取得
	* @note		1は漢字かなカナ、英字、数字、「-」でセパレート
	* @note		2は漢字かなカナ英字、数字、「-」でセパレート（こちらを使用するのは[bug7333]対応のため）
	*
	* @param	lpszCharAddr[in]			文字付き住所
	* @param	sjisSeparate[in]			「-」か「－」を指定
	* @return	文字付き住所リスト
	*/
	std::list<std::pair<YomiType,std::string>> GetCharAddrList( LPCTSTR lpcszCharAddr, const sjis_ranges& sjisSeparate = sjis_ranges().add(sjis_range(0x2d,0x2d)) );
	std::list<std::pair<YomiType,std::string>> GetCharAddrList2( LPCTSTR lpcszCharAddr, const sjis_ranges& sjisSeparate = sjis_ranges().add(sjis_range(0x2d,0x2d)));

	/**
	* @brief	オブジェクトID読みテーブルのレコードリストを取得する
	*/
	std::map<ObjectLayerPair,KanjiYomiPair> getObjectYomiItems(){ return m_mapObjectYomiTable; }

	/**
	* @brief	漢字読みテーブルのレコードリストを取得する
	*/
	std::map<std::string,std::string> getKanjiYomiItems(){ return m_mapKanjiYomiTable; }

private:
	/**
	* @brief	オブジェクト読み変換テーブルのデータリストを取得
	* @param	ipTable[in]					オブジェクトID読みテーブル
	* @param	mapError[in,out]			テーブルのレコードにエラーがあった場合出力する
	* @return	オブジェクトID読みテーブルマップ
	*/
	std::map<ObjectLayerPair,KanjiYomiPair> GetObjectYomiTableItems( ITable* ipTable, std::multimap<long,CString>& mapError );

	/**
	* @brief	漢字読み変換テーブルのデータリストを取得
	* @param	cKanjiYomiCursor[in]		漢字読みテーブル
	* @param	mapError[in,out]			テーブルのレコードにエラーがあった場合出力する
	* @return	漢字読みテーブルマップ
	*/
	std::map<std::string,std::string> GetKanjiYomiTableItems( CADOCursor& cKanjiYomiCursor, std::multimap<long,CString>& mapError );

	/**
	* @brief	漢字読みテーブルから読みを取得
	* @param	strCharAddr[in]				漢字読みテーブルと突き合わせるための文字列
	* @param	bRet[in,out]				テーブルにある文字列だったかどうか（true:あった,false:ない）
	* @return	読み
	*/
	std::string GetYomiFromKanjiYomiTable( const std::string& strCharAddr, bool& bRet );

	/**
	* @brief	サブテーブルから読みを取得
	* @param	strCharAddr[in]				サブテーブルと突き合わせるための文字列
	* @param	bRet[in,out]				テーブルにある文字列だったかどうか（true:あった,false:ない）
	* @return	読み
	*/
	std::string GetYomiFromSubTable( const std::wstring& strCharAddr, bool& bRet );

	/**
	* @brief	文字付き住所を漢字読み変換テーブルで変換して出力
	* @note		GetYomiFromCharAddr()で呼ばれる関数
	*			GetCharAddrList()で取得したリストを使用する
	* @param	listCharAddr[in]			文字付き住所の文字列を単語毎に分割したリスト
	* @param	bRet[in,out]				テーブルにある文字列だったかどうか（true:あった,false:ない）
	* @return	読み
	*/
	std::string GetYomi( const std::list<std::pair<YomiType,std::string>>& listCharAddr, bool& bRet );

	/**
	* @brief	変換テーブルの使用文字チェック
	* @param	strYomi[in]					チェック文字列
	* @param	nType[in]					変換テーブルタイプ
	* @retval	true	[使用文字OK]
	* @retval	false	[使用文字NG]
	*/
	bool IsCheckYomi( const std::string& strYomi, CharAddrType nType );

	/**
	* @brief	漢字と読みの整合性が取れているかチェック
	* @param	lpcszKanji[in]				漢字
	* @param	lpcszYomi[in]				読み
	* @param	lOID[in]					オブジェクトID
	* @param	mapError[in,out]			エラー情報（処理中にエラーがあればこれに追記する）
	* @retval	true	整合性が取れている
	* @retval	false	整合性が取れていない
	*/
	bool IsCheckKanjiYomi(LPCTSTR lpcszKanji, LPCTSTR lpcszYomi, long lOID, std::multimap<long,CString>& mapError);

	/**
	* @brief	文字変換
	* @note		全角平仮名->全角カタカナ
	* @note		半角数字->全角数字
	*/
	std::string ConvertHiraToKata(std::string strHira);
	std::string ConvertHanNumToZenNum(std::string strHanNum);

private:
	CSubConvertTable m_cSubConvert;									//!< サブ読み変換テーブル
	std::map<std::string,std::string> m_mapKanjiYomiTable;			//!< 漢字読み変換テーブルのマップ
	std::map<ObjectLayerPair,KanjiYomiPair> m_mapObjectYomiTable;	//!< オブジェクトID読み変換テーブル
	std::map<std::wstring,std::wstring> m_mapSubTable;				//!< サブ読み変換テーブルのマップ
};
