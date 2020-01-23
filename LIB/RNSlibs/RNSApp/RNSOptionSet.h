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

 	���e :
 	���� :  
 	�ǋL : 
 
 	���� :	Fukuta.K
 	
************************************************* */

/* ----- ��` ----- */

#include <set>
#include <vector>

#include <RNSObject.h>
#include <RNSString.h>
#include "RNSOption.h"

/* ////// �N���X��`  ////// */

typedef std::vector<char*> RNSArgList;

/**
 *�@�I�v�V������̓N���X
**/
class RNSOptionSet : public RNSObject {
	std::set<RNSOptionRef>	mOptionSet;				// �o�^���ꂽ�I�v�V��������

	RNSOption* operator [] (char iSimbol) const;	// �I�y���[�^���`����܂ł�...

 public:
	RNSOptionSet(){}
	RNSOptionSet(const char *iSimbols)
		{regestOptionSimbols(iSimbols);}

	//! �I�v�V�����Ɏg����V���{���̒�`
	void regestOptionSimbols(const char *iSimbols);

	//! �������X�g����I�v�V��������͂���
	bool setOption(int argc, char *argv[], RNSArgList *oNotOptionArg);
	
	//! �w�肳�ꂽ�I�v�V������Ԃ��i�w�肪�Ȃ��ꍇ�͕ω������j
	// �I�v�V�������w�肳��Ă�����true
	bool getOption(char simbol) const;
	bool getOption(char simbol, int *oValue) const;
	bool getOption(char simbol, double *oValue) const;
	bool getOption(char simbol, RNSString *oValue) const;
	bool getOption(char simbol, char *oValue, int iN) const;

	bool getOptionHex(char simbol, int *oValue) const;

	//! �w�肳�ꂽ���ϐ���Ԃ��i�w�肪�Ȃ��ꍇ�͕ω������j
	// ���ϐ����w�肳��Ă�����true
	bool getEnv(const char *iEnvName, int *oValue) const;
	bool getEnv(const char *iEnvName, double *oValue) const;
	bool getEnv(const char *iEnvName, RNSString *oValue) const;
	bool getEnv(const char *iEnvName, char *oValue, int iN) const;

	//! �w�肳�ꂽ�I�v�V������Ԃ��i�w�肪�Ȃ��ꍇ�͊��ϐ��̒l���A�ω������j
	// �I�v�V���������ϐ����w�肳��Ă�����true
	bool getOptionOrEnv(char simbol, int *oValue, const char *iEnvName) const;
	bool getOptionOrEnv(char simbol, double *oValue, const char *iEnvName) const;
	bool getOptionOrEnv(char simbol, RNSString *oValue, const char *iEnvName) const;
	bool getOptionOrEnv(char simbol, char *oValue, int iN, const char *iEnvName) const;
};

#endif /* __RNS_OPTION__ */
