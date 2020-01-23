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

#ifndef __RNS_DIRECTORY__
#define	__RNS_DIRECTORY__

/* ************************************************

 	初稿 :
 	改訂 :  
 	追記 : 
 
 	制作 :	Fukuta.K
 	
************************************************* */

#ifdef _WIN32
#pragma comment(lib, "imagehlp.lib")
#endif

/* ----- 定義 ----- */

#include "RNS.h"
#include "RNSString.h"

/* ////// クラス定義  ////// */

/**
 * ディレクトリ名称クラス
 *
 * 常に最後尾にディレクトリ区切り記号を付けて文字列を管理している 
 */
class RNSDirectory : public RNSString {
public:

// 構築/消滅
	RNSDirectory();
	RNSDirectory(const char *iStr);
	RNSDirectory(const std::string &iStr);
	virtual ~RNSDirectory();

// 操作
	virtual RNSString& append(const RNSString &iStr);

	bool	mkdir() const;

// 判定
	bool	exists() const;

// オペレータ
	virtual RNSString& operator = (const std::string &iStr);
	virtual RNSString& operator = (const char *iStr);

protected:

	virtual void	appendEpiEffect();

	RNSDirectory&	dirComplete(char iTrMark = _pathChr);

};

_RNSOBJECT_TYPEDEF(RNSDirectory);

#endif /* __RNS_DIRECTORY__ */
