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

#include "stdafx.h"
#include "OutputLog.h"
#include <jis/x0208/spirit/shift_jis.h>

using namespace jis::x0208::spirit;

/**
 * @brief   名称チェッククラス
 */
class cNameChk
{
public:

	
	/**
	 * @brief   コンストラクタ
	 */
	cNameChk()
	{
		SetString();
	};

	/**
	 * @brief   デストラクタ
	 */
	~cNameChk(){};

	/**
	 * @brief   サイズが偶数か
	 * @param   str [in]  文字列
	 * @retval  true  偶数
	 * @retval  false 奇数
	 */
	bool IsEvenSize( const std::string& str ){
		if( (str.size() % 2) == 0 )
			return true;
		
		return false;
	};

	/**
	 * @brief   文字検索
	 * @note    2バイト単位で検索する
	 * @param   str      [in]  文字列
	 * @param   find     [in]  検索する文字列
	 * @param   startPos [in]  開始位置
	 * @retval  string::size_type 見つけた位置
	 */
	std::string::size_type string_find( const std::string& str, const std::string& find, int startPos = 0 ){
		long size = str.size();
		for( int i = startPos ; i < size ; ){
			std::string::size_type pos = str.find( find, i );
			if( pos == std::string::npos )
				return std::string::npos;
			if( ( pos % 2 ) != 0 ){
				i = pos + 1;
				continue;
			}
			return pos;
		}
		return std::string::npos;
	};

	/**
	 * @brief   空文字列か？
	 * @param   str [in]  文字列
	 * @retval  true  空
	 * @retval  false 空じゃない
	 */
	bool IsNullString( const std::string& str ){
		if( str.empty() || 0 == str.compare( _T(" ") ) )
			return true;
		return false;
	};

	/**
	 * @brief   全て仕様範囲の文字か
	 * @param   str [in]  文字列
	 * @retval  true  OK
	 * @retval  false NG
	 */
	bool IsOkString( const std::string& str ){
		
		if( ! parse( str.c_str(), *m_okStrs ).full )
			return false;

		return true;
	};

	/**
	 * @brief   「｜」が存在するか
	 * @param    str [in]  文字列
	 * @retval   true  存在する
	 * @retval   false 存在しない
	 */
	bool IsFindPipe( const std::string& str ){
		if( string_find( str, "｜" ) != std::string::npos )
			return true;
		return false;
	};

	/**
	 * @brief   スペースが正常か(先頭、最後、連続がNG)
	 * @param   str [in]  文字列
	 * @retval  true  OK
	 * @retval  false NG
	 */
	bool IsOkSpace( const std::string& str ){
		long size = str.size();
		for( int i = 0 ; i < size - 2; i += 2 ){
			auto subStr1 = str.substr( i, 2 );
			auto subStr2 = str.substr( i + 2, 2 );
			if( i == 0 ){ // 最初
				if( IsSpace( subStr1 ) )
					return false;
			}
			if( i == size - 4 ){ // 最後
				if( IsSpace( subStr2 ) )
					return false;
			}
			if( IsSpace( subStr1) && IsSpace( subStr2 ) )
				return false;
		}
		return true;
	};

	/**
	 * @brief   文字が全角スペースか
	 * @param   word [in]  文字
	 * @retval  true  全角スペース
	 * @retval  false それ以外
	 */
	bool IsSpace( const std::string& word ){ 
		return ( 0 == word.compare( _T("　") ) ); 
	};

	/**
	 * @brief   ハイフン関連の文字が正常か
	 * @param   str [in]  文字列
	 * @retval  err_def::ECode エラーコード(OKも含む)
	 */
	err_def::ECode IsOkHiphen( const std::string& str ){
		using namespace err_def;
		long size = str.size();
		for( int i = 0 ; i < size ; i += 2 ){
			auto subStr = str.substr( i, 2 );
			if( i == 0 ){
				if( IsHiphen( subStr ) )
					return eTopZenHyphen; // 先頭NG
				continue;
			}
			auto preStr = str.substr( i - 2, 2 );
			if( IsHiphenPart1( subStr ) ){ // 「―」or「－」or「‐」
				// 全角英数字以外の後に「―」or「－」or「‐」があるのはNG
				if( ! parse( preStr.c_str(), *m_alnum ).full )
					return wNgHyphenWord; // NG
			}
			else if( IsHiphenPart2( subStr ) ){ // 「ー」
				// ひらがな、カナカナ以外の後に、「ー」があるのはNG（「ん」と「ン」もダメ）
				if( ! parse( preStr.c_str(), *m_hirakata ).full || 0 == preStr.compare( _T("ん") ) || 0 == preStr.compare( _T("ン") ) )
					return wNgHyphenWord; // NG
			}
		}
		return Ok; // OK
	};

	bool IsHiphen( const std::string& word ){ return ( IsHiphenPart1( word ) || IsHiphenPart2( word ) ); };
	bool IsHiphenPart1( const std::string& word ){ return ( 0 == word.compare( _T("―") ) || 0 == word.compare( _T("－") ) || 0 == word.compare( _T("‐") ) ); };
	bool IsHiphenPart2( const std::string& word ){ return ( 0 == word.compare( _T("ー") ) ); };

	// 小文字関連が正常か
	err_def::ECode IsOkLowStr( const std::string& str ){
		using namespace err_def;
		long size = str.size();
		for( int i = 0 ; i < size ; i += 2 ){
			auto subStr = str.substr( i, 2 );
			if( i == 0 ){
				if( IsHiraLowYaYuYo( subStr ) )
					return eTopHiraLowY; // 先頭「ゃゅょ」NG
				if( IsKataLowYaYuYo( subStr ) )
					return eTopKataLowY; // 先頭「ャュョ」NG
				if( IsHiraLowTsu( subStr ) )
					return eTopHiraLowTsu; // 先頭「っ」NG
				if( IsKataLowTsu( subStr ) )
					return eTopKataLowTsu; // 先頭「ッ」NG
				if (IsHiraLowAiueoN(subStr))
					return eTopHiraLowAN; // 先頭「ぁぃぅぇぉ」NG
				if (IsKataLowAiueoN(subStr))
					return eTopKataLowAN; // 先頭「ァィゥェォ」NG
				continue;
			}
			if( i == (size - 2) ){
				if( IsHiraLowTsu( subStr ) )
					return eNgHiraLowTsu; // 最後「っ」NG
				if( IsKataLowTsu( subStr ) )
					return eNgKataLowTsu; // 最後「ッ」NG
			}

			auto preStr = str.substr( i - 2, 2 );
			if( IsHiraLowYaYuYo( subStr ) ){
				// 「き」「し」「ち」「に」「ひ」「み」「り」「ぎ」「じ」「ぢ」「び」「ぴ」	以外の後に「ゃゅょ」がある
				if( ! HaveHiraLowYaYuYo( preStr ) )
					return err_def::eNgHiraLowY; // NG
			}
			else if( IsKataLowYaYuYo( subStr ) ){
				// 「キ」「シ」「チ」「ニ」「ヒ」「ミ」「リ」「ギ」「ジ」「ヂ」「ビ」「ピ」以外の後に「ャュョ」がある
				if( ! HaveKataLowYaYuYo( preStr ) )
					return err_def::eNgKataLowY; // NG
			}
			else if( IsHiraLowTsu( subStr ) ){
				// 「ん」「っ」「ー」の後に「っ」がある
				if( NotHaveHiraLowTsu( preStr ) )
					return eNgHiraLowTsu; // NG
				// ひらがな、カタカナ以外の後に「っ」がある
				if( ! parse( preStr.c_str(), *m_hirakata ).full )
					return wNgHiraLowTsu; // NG
			}
			else if( IsKataLowTsu( subStr ) ){
				// 「ン」「ッ」「ー」の後に「ッ」がある
				if( NotHaveKataLowTsu( preStr ) )
					return eNgKataLowTsu; // NG
				// ひらがな、カタカナ、漢数字以外の後に「ッ」がある
				if( ! parse( preStr.c_str(), *m_hirakata ).full && ! IsKanNum( preStr ) )
					return wNgKataLowTsu; // NG
			}
		}
		return err_def::Ok; // OK
	};

	bool IsHiraLowYaYuYo( const std::string& word ){ return ( 0 == word.compare( _T("ゃ") ) || 0 == word.compare( _T("ゅ") ) || 0 == word.compare( _T("ょ") ) ); };
	bool IsHiraLowAiueoN(const std::string& word) { return (0 == word.compare(_T("ぁ")) || 0 == word.compare(_T("ぃ")) || 0 == word.compare(_T("ぅ")) || 0 == word.compare(_T("ぇ")) || 0 == word.compare(_T("ぉ")) || 0 == word.compare(_T("ん"))); };
	bool HaveHiraLowYaYuYo( const std::string& word ){
		return ( 0 == word.compare( _T("き") ) || 0 == word.compare( _T("し") ) || 0 == word.compare( _T("ち") )
			|| 0 == word.compare( _T("に") ) || 0 == word.compare( _T("ひ") ) || 0 == word.compare( _T("み") )
			|| 0 == word.compare( _T("り") ) || 0 == word.compare( _T("ぎ") ) || 0 == word.compare( _T("じ") )
			|| 0 == word.compare( _T("ぢ") ) || 0 == word.compare( _T("び") ) || 0 == word.compare( _T("ぴ") )
			);
	};
	bool IsKataLowYaYuYo( const std::string& word ){ return ( 0 == word.compare( _T("ャ") ) || 0 == word.compare( _T("ュ") ) || 0 == word.compare( _T("ョ") ) ); };
	bool IsKataLowAiueoN(const std::string& word) { return (0 == word.compare(_T("ァ")) || 0 == word.compare(_T("ィ")) || 0 == word.compare(_T("ゥ")) || 0 == word.compare(_T("ェ")) || 0 == word.compare(_T("ォ")) || 0 == word.compare(_T("ン"))); };
	bool HaveKataLowYaYuYo( const std::string& word ){
		return ( 0 == word.compare( _T("キ") ) || 0 == word.compare( _T("シ") ) || 0 == word.compare( _T("チ") )
			|| 0 == word.compare( _T("ニ") ) || 0 == word.compare( _T("ヒ") ) || 0 == word.compare( _T("ミ") )
			|| 0 == word.compare( _T("リ") ) || 0 == word.compare( _T("ギ") ) || 0 == word.compare( _T("ジ") )
			|| 0 == word.compare( _T("ヂ") ) || 0 == word.compare( _T("ビ") ) || 0 == word.compare( _T("ピ") )
			);
	};
	bool IsHiraLowTsu( const std::string& word ){ return ( 0 == word.compare( _T("っ") ) ); };
	bool NotHaveHiraLowTsu( const std::string& word ){ return ( 0 == word.compare( _T("ん") ) || 0 == word.compare( _T("っ") ) || 0 == word.compare( _T("ー") ) ); };
	bool IsKataLowTsu( const std::string& word ){ return ( 0 == word.compare( _T("ッ") ) ); };
	bool NotHaveKataLowTsu( const std::string& word ){ return ( 0 == word.compare( _T("ン") ) || 0 == word.compare( _T("ッ") ) || 0 == word.compare( _T("ー") ) ); };
	bool IsKanNum( const std::string& word ){
		return ( 0 == word.compare( _T("一") ) || 0 == word.compare( _T("二") ) || 0 == word.compare( _T("三") )
			|| 0 == word.compare( _T("四") ) || 0 == word.compare( _T("五") ) || 0 == word.compare( _T("六") )
			|| 0 == word.compare( _T("七") ) || 0 == word.compare( _T("八") ) || 0 == word.compare( _T("九") )
			|| 0 == word.compare( _T("十") ) );
	};

	err_def::ECode IsOkKakko( const std::string& str ){
		using namespace err_def;
		if( parse( str.c_str(), *m_kakko ).full )
			return Ok; // 無ければOK
		long size = str.size();
		long bKakko[2][11] = { 
			-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
			-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1 };
		for( int i = 0 ; i < size ; i += 2 ){
			auto subStr = str.substr( i, 2 );
			if( ! IsKakko( subStr ) )
				continue;
			long leftKakko  = IsLeftKakko( subStr );
			if( leftKakko != -1 )
				bKakko[0][leftKakko] = i;

			long rightKakko = IsRightKakko( subStr );
			if( rightKakko != -1 )
				bKakko[1][rightKakko] = i;
		}
		for( int i = 0 ; i < 11 ; ++i ){
			if( ( bKakko[0][i] != -1 ) ^ ( bKakko[1][i] != -1 ) ){
				if( bKakko[0][i] != -1 )
					return eLeftParentOnly;
				return eRightParentOnly;
			}
			if( bKakko[0][i] > bKakko[1][i] )
				return eNgParentheses;
		}
		//std::string::size_type leftPos = str.find_first_of( _T("") );
		//std::string::size_type rightPos = str.find_first_of( _T("") );
		return Ok;
	};
	bool IsKakko( const std::string& word ) { return ( IsLeftKakko(word) || IsRightKakko(word) ); };
	long IsLeftKakko( const std::string& word ) { 
		if( IsLeftKakko0(word) ) return 0; if( IsLeftKakko1(word) ) return 1; if( IsLeftKakko2(word) ) return 2; if( IsLeftKakko3(word) ) return 3;
		if( IsLeftKakko4(word) ) return 4; if( IsLeftKakko5(word) ) return 5; if( IsLeftKakko6(word) ) return 6; if( IsLeftKakko7(word) ) return 7;
		if( IsLeftKakko8(word) ) return 8; if( IsLeftKakko9(word) ) return 9; if( IsLeftKakko10(word) ) return 10;
		return -1;
	};
	long IsRightKakko( const std::string& word ) { 
		if( IsRightKakko0(word) ) return 0; if( IsRightKakko1(word) ) return 1; if( IsRightKakko2(word) ) return 2; if( IsRightKakko3(word) ) return 3;
		if( IsRightKakko4(word) ) return 4; if( IsRightKakko5(word) ) return 5; if( IsRightKakko6(word) ) return 6; if( IsRightKakko7(word) ) return 7;
		if( IsRightKakko8(word) ) return 8; if( IsRightKakko9(word) ) return 9; if( IsRightKakko10(word) ) return 10;
		return -1;
	};

	bool IsLeftKakko0( const std::string& word ) { return ( 0 == word.compare( _T("（") ) ); };
	bool IsLeftKakko1( const std::string& word ) { return ( 0 == word.compare( _T("＜") ) ); };
	bool IsLeftKakko2( const std::string& word ) { return ( 0 == word.compare( _T("「") ) ); };
	bool IsLeftKakko3( const std::string& word ) { return ( 0 == word.compare( _T("『") ) ); };
	bool IsLeftKakko4( const std::string& word ) { return ( 0 == word.compare( _T("【") ) ); };
	bool IsLeftKakko5( const std::string& word ) { return ( 0 == word.compare( _T("［") ) ); };
	bool IsLeftKakko6( const std::string& word ) { return ( 0 == word.compare( _T("｛") ) ); };
	bool IsLeftKakko7( const std::string& word ) { return ( 0 == word.compare( _T("〔") ) ); };
	bool IsLeftKakko8( const std::string& word ) { return ( 0 == word.compare( _T("《") ) ); };
	bool IsLeftKakko9( const std::string& word ) { return ( 0 == word.compare( _T("〈") ) ); };
	bool IsLeftKakko10( const std::string& word ) { return ( 0 == word.compare( _T("≪") ) ); };
	bool IsRightKakko0( const std::string& word ) { return ( 0 == word.compare( _T("）") ) ); };
	bool IsRightKakko1( const std::string& word ) { return ( 0 == word.compare( _T("＞") ) ); };
	bool IsRightKakko2( const std::string& word ) { return ( 0 == word.compare( _T("」") ) ); };
	bool IsRightKakko3( const std::string& word ) { return ( 0 == word.compare( _T("』") ) ); };
	bool IsRightKakko4( const std::string& word ) { return ( 0 == word.compare( _T("】") ) ); };
	bool IsRightKakko5( const std::string& word ) { return ( 0 == word.compare( _T("］") ) ); };
	bool IsRightKakko6( const std::string& word ) { return ( 0 == word.compare( _T("｝") ) ); };
	bool IsRightKakko7( const std::string& word ) { return ( 0 == word.compare( _T("〕") ) ); };
	bool IsRightKakko8( const std::string& word ) { return ( 0 == word.compare( _T("》") ) ); };
	bool IsRightKakko9( const std::string& word ) { return ( 0 == word.compare( _T("〉") ) ); };
	bool IsRightKakko10( const std::string& word ) { return ( 0 == word.compare( _T("≫") ) ); };

private:

	void SetString(){
		
		m_okStrs = sjis_ranges()
			.add(sjis_kanji_p) // 第一、第二水準漢字
			.add(sjis_kana_p)  // 全角ひらがな、カタカナ
			.add(sjis_alnum_p) // 全角英数字
			.add(sjis_greek_p) // ギリシャ文字
			.add(sjis_range(0x8140, 0x81fc)); // 全角記号（全角スペース～◯）

		m_hirakata = sjis_ranges().add(sjis_kana_p);
		m_alnum    = sjis_ranges().add(sjis_alnum_p);
		m_kakko    = sjis_ranges()
			.add(sjis_range(0x8169,0x817a))  // （	）	〔	〕	［	］	｛	｝	〈	〉	《	》	「	」	『	』	【	】
			.add(sjis_range(0x8183,0x8184))  // ＜　＞
			.add(sjis_range(0x81e1,0x81e2)); // ≪　≫
	};


private:

	sjis_ranges m_okStrs;   //!< 許容文字
	sjis_ranges m_hirakata; //!< 全角ひらがな・カタカナ
	sjis_ranges m_alnum;    //!< 全角アルファベット
	sjis_ranges m_kakko;    //!< 全角カッコ

};
