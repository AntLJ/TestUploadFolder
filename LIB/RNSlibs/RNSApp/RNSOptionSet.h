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

#ifndef __RNS_OPTION_SET__
#define	__RNS_OPTION_SET__

/* ************************************************

 	初稿 :
 	改訂 :  
 	追記 : 
 
 	制作 :	Fukuta.K
 	
************************************************* */

/* ----- 定義 ----- */

#include <set>
#include <vector>

#include <RNSObject.h>
#include <RNSString.h>
#include "RNSOption.h"

/* ////// クラス定義  ////// */

typedef std::vector<char*> RNSArgList;

/**
 *　オプション解析クラス
**/
class RNSOptionSet : public RNSObject {
	std::set<RNSOptionRef>	mOptionSet;				// 登録されたオプションたち

	RNSOption* operator [] (char iSimbol) const;	// オペレータを定義するまでも...

 public:
	RNSOptionSet(){}
	RNSOptionSet(const char *iSimbols)
		{regestOptionSimbols(iSimbols);}

	//! オプションに使われるシンボルの定義
	void regestOptionSimbols(const char *iSimbols);

	//! 引数リストからオプションを解析する
	bool setOption(int argc, char *argv[], RNSArgList *oNotOptionArg);
	
	//! 指定されたオプションを返す（指定がない場合は変化無し）
	// オプションが指定されていたらtrue
	bool getOption(char simbol) const;
	bool getOption(char simbol, int *oValue) const;
	bool getOption(char simbol, double *oValue) const;
	bool getOption(char simbol, RNSString *oValue) const;
	bool getOption(char simbol, char *oValue, int iN) const;

	bool getOptionHex(char simbol, int *oValue) const;

	//! 指定された環境変数を返す（指定がない場合は変化無し）
	// 環境変数が指定されていたらtrue
	bool getEnv(const char *iEnvName, int *oValue) const;
	bool getEnv(const char *iEnvName, double *oValue) const;
	bool getEnv(const char *iEnvName, RNSString *oValue) const;
	bool getEnv(const char *iEnvName, char *oValue, int iN) const;

	//! 指定されたオプションを返す（指定がない場合は環境変数の値か、変化無し）
	// オプションか環境変数が指定されていたらtrue
	bool getOptionOrEnv(char simbol, int *oValue, const char *iEnvName) const;
	bool getOptionOrEnv(char simbol, double *oValue, const char *iEnvName) const;
	bool getOptionOrEnv(char simbol, RNSString *oValue, const char *iEnvName) const;
	bool getOptionOrEnv(char simbol, char *oValue, int iN, const char *iEnvName) const;
};

#endif /* __RNS_OPTION__ */
