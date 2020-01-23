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

#ifndef __RNS_STRING__
#define	__RNS_STRING__

/* ************************************************

 	初稿 :
 	改訂 :  
 	追記 : 
 
 	制作 :	Fukuta.K
 	
************************************************* */

/* ///// インクルードファイル ///// */

#include <atlbase.h>

#include <string>
#include <vector>

#include "RNS.h"
#include "RNSPlant.h"
#include "RNSObject.h"


/* ////// クラス定義  ////// */


/**
 * 文字列クラス
 *
 * STLのstringに
 *
 * - 入力ストリーム
 * - 数値の文字列変換
 *
 * などの機能を追加したもの
 *
 */
class RNSString : public std::string, public RNSObject {
public:
	/* ----- 初期設定 ----- */
	/// デフォルトコンストラクタ
	RNSString(){}

	/**
	* コンストラクタ（文字列代入）
	*
	* @param	iStr	代入する文字列
	*/
	RNSString(const char *iStr);

	/**
	* コンストラクタ（文字列代入）
	*
	* @param	iStr	代入する文字列
	*/
	RNSString(const std::string &iStr);

	/**
	 * コンストラクタ（数値→文字列変換代入）
	 *
	 * @param	iValue	代入する数値
	 */
	RNSString(int iValue);

	/**
	 * コンストラクタ（符号なし数値→文字列変換代入）
	 *
	 * @param	iValue	代入する符号なし数値
	 */
	RNSString(unsigned int iValue);

	/// デストラクタ
	virtual ~RNSString(){}

	/* ----- 追加 ----- */
	/**
	* 文字列を追加する
	*
	* @param	iStr	追加する文字列
	*
	* @return	自己への参照
	*/
	virtual RNSString& append(const RNSString &iStr);

	/**
	* 指定した数だけ文字を追加する
	*
	* @param	n		追加する文字数
	* @param	iChar	追加する文字
	*
	* @return	自己への参照
	*/
	virtual RNSString& append(size_type n, char &iChar);

	/**
	* 文字列を代入する
	*
	* @param	iStr	追加する文字列
	*
	* @return	自己への参照
	*/
	virtual RNSString& operator = (const std::string &iStr);

	/**
	* 文字列定数を代入する
	*
	* @param	iStr	追加する文字列
	*
	* @return	自己への参照
	*/
	virtual RNSString& operator = (const char *iStr);

	/* ----- 削除 ----- */
	/**
	 * コメント文を削除する（string 型）
	 *
	 * デフォルトでは # 移行の文字列がコメントと判断される。
	 *
	 * @param	iStr	コメント文字列
	 */
	void eraseComment(const std::string& iStr);

	/**
	 * コメント文を削除する（const char 型）
	 *
	 * デフォルトでは # 移行の文字列がコメントと判断される。
	 *
	 * @param	iStr	コメント文字列
	 */
	void eraseComment(const char* iStr = "#");

	/**
	 * 文字列両端の余分なものを除く
	 *
	 * デフォルトではスペースとタブが除かれる
	 *
	 * @param	iTrimChars
	 */
	RNSString trimedString(const char* iTrimChars);

	/* ----- 取得 ----- */
	/**
	 * 文字列を分割する（string セパレータ）
	 *
	 * 指定したセパレータで、この文字列を分割する。
	 * 例えば、セパレータが ":" で 文字列が "CODE : VALUE" の場合、"CODE " と " VALUE" という２つの文字列に分割される。
	 *
	 * @param	oDivideStrList	分割された文字列一覧
	 * @param	iSeparator		セパレータ
	 * @param	iTrimChars		分割した際、両端からトリムする文字
	 * @return	分割された文字列数
	 */
	int divide(std::vector< RNSRef<RNSString> >* oDivideStrList, const std::string& iDelim, const char* iTrimChars = NULL) const;

	/* ----- 追加オペレータ ----- */
	/// 文字列を追加する
	RNSString& operator += (const char* iStr);
	/// 文字列を追加する
	RNSString& operator += (const RNSString &iStr);
	/// 文字を追加する
	RNSString& operator += (char iChar );
	/// 数字を文字として追加する
	RNSString& operator += (int iValue);
	/// 符号なし数字を文字として追加する
	RNSString& operator += (unsigned int iValue);

	/// 文字列を追加する
	RNSString& operator << (const RNSString &iStr);
	/// 文字を追加する
	RNSString& operator << (char iChar);
	/// 数字を文字として追加する
	RNSString& operator << (int iValue);

private:
	/// 追加、代入を行う前に行う処理
 	virtual void appendPreEffect(){};

	/// 追加、代入を行った後に行う処理
	virtual void appendEpiEffect(){};

};

_RNSOBJECT_TYPEDEF(RNSString);

const std::string narrow(const std::wstring& cInput);
const std::wstring widen(const std::string& cInput);

#endif /* __RNS_STRING__ */
