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

/* ************************************************
	���e :
	���� :  
	�ǋL : 
 
	���� :  Fukuta.K
************************************************* */

/* ----- �C���N���[�h�t�@�C�� ----- */

#include <stdlib.h>
#include <iostream>

#include <RNS.h>
#include "RNSOptionSet.h"
#include "RNSMessage.h"

/* ----- �p�u���b�N�֐� ----- */

/* ==========
========== */
void RNSOptionSet::regestOptionSimbols(const char *iSimbols)
{
	const char *i = iSimbols;

	mOptionSet.clear();

	while(*i != 0){
		char simbol = *i++;
		bool neadValue = false;

		if(*i == ':'){
			neadValue = true;
			++i;
		}

		mOptionSet.insert(new RNSOption(simbol, neadValue));
	}
}

/**
 *	�I�v�V�������
**/
bool RNSOptionSet::setOption(int argc, char *argv[], RNSArgList *oNotOptionArg)
{
	using namespace rns_message;

	bool errFlag = false;

	// ���������
	for(int i = 0; i < argc; ++i){

		const char *c = argv[i];

		// �I�v�V��������Ȃ�������p�X
		if(*c != '-'){
			if(oNotOptionArg != NULL){
				oNotOptionArg->push_back(argv[i]);
				continue;
			}
		}

		// �ꕶ�����I�v�V�����`�F�b�N
		for(++c; *c != 0; ++c){
			RNSOption *opt;

			// ��`����Ă��Ȃ��I�v�V������������G���[
			if((opt = (*this)[*c]) == NULL){
				//�I�v�V���� -%c �͐���������܂���
				std::cerr << getMsg(eOptionError1) << c << getMsg(eOptionError2) << std::endl;
				errFlag = true;
				break;
			}

			if(opt->isNeadValue()){
				if(*++c == 0){
					if(i+1 == argc){
						errFlag = true;
						break;
					}
					c = argv[++i];
				}
				opt->setValue(c);
				break;
			}

			opt->setFlag(true);
		}
	}

	return !errFlag;
}

/**
 *	�I�v�V�����擾
**/
bool RNSOptionSet::getOption(char iSimbol) const
{
	const RNSOption *opt = (*this)[iSimbol];

	if(opt != NULL)
		return opt->flag();

	return false;
}

bool RNSOptionSet::getOption(char iSimbol, int *oValue) const
{
	RNSOption *opt = (*this)[iSimbol];

	if(opt != NULL){
		if(opt->flag()){
			*oValue = atol(opt->value());
			return true;
		}
	}

	return false;
}

bool RNSOptionSet::getOption(char iSimbol, double *oValue) const
{
	RNSOption *opt = (*this)[iSimbol];

	if(opt != NULL){
		if(opt->flag()){
			*oValue = atof(opt->value());
			return true;
		}
	}

	return false;
}

bool RNSOptionSet::getOption(char iSimbol, RNSString *oValue) const
{
	RNSOption *opt = (*this)[iSimbol];

	if(opt != NULL){
		if(opt->flag()){
			*oValue = opt->value();
			return true;
		}
	}

	return false;
}

bool RNSOptionSet::getOption(char iSimbol, char *oValue, int iN) const
{
	RNSOption *opt = (*this)[iSimbol];

	if(opt != NULL){
		if(opt->flag()){
			strncpy(oValue, opt->value(), iN);
			return true;
		}
	}

	return false;
}

bool RNSOptionSet::getOptionHex(char iSimbol, int *oValue) const
{
	RNSOption *opt = (*this)[iSimbol];

	if(opt != NULL){
		if(opt->flag()){
			sscanf(opt->value(), "%x", oValue);
			return true;
		}
	}

	return false;
}

/**
 *	���ϐ��擾
**/
bool RNSOptionSet::getEnv(const char *iEnvName, int *oValue) const
{
	const char *env = getenv(iEnvName);

	if(env != NULL){
		*oValue = atol(env);
		return true;
	}

	return false;
}

bool RNSOptionSet::getEnv(const char *iEnvName, double *oValue) const
{
	const char *env = getenv(iEnvName);

	if(env != NULL){
		*oValue = atof(env);
		return true;
	}

	return false;
}

bool RNSOptionSet::getEnv(const char *iEnvName, RNSString *oValue) const
{
	const char *env = getenv(iEnvName);

	if(env != NULL){
		*oValue = env;
		return true;
	}

	return false;
}

bool RNSOptionSet::getEnv(const char *iEnvName, char *oValue, int iN) const
{
	const char *env = getenv(iEnvName);

	if(env != NULL){
		strncpy(oValue, env, iN);
		return true;
	}

	return false;
}

/**
 *	�I�v�V�����擾�i�I�v�V�������w�肳��Ă��Ȃ��ꍇ�͊��ϐ��j
**/
bool RNSOptionSet::getOptionOrEnv(char iSimbol, int *oValue, const char *iEnvName) const
{
	if(! getOption(iSimbol, oValue)){
		if(! getEnv(iEnvName, oValue))
			return false;
	}
	return true;
}

bool RNSOptionSet::getOptionOrEnv(char iSimbol, double *oValue, const char *iEnvName) const
{
	if(! getOption(iSimbol, oValue)){
		if(! getEnv(iEnvName, oValue))
			return false;
	}
	return true;
}

bool RNSOptionSet::getOptionOrEnv(char iSimbol, RNSString *oValue, const char *iEnvName) const
{
	if(! getOption(iSimbol, oValue)){
		if(! getEnv(iEnvName, oValue))
			return false;
	}
	return true;
}

bool RNSOptionSet::getOptionOrEnv(char iSimbol, char *oValue, int iN, const char *iEnvName) const
{
	if(! getOption(iSimbol, oValue, iN)){
		if(! getEnv(iEnvName, oValue, iN))
			return false;
	}
	return true;
}

/**
 *	�I�y���[�^
**/
RNSOption* RNSOptionSet::operator [] (char iSimbol) const
{
	std::set<RNSOptionRef>::const_iterator itr;

	if((itr = mOptionSet.find(RNSOptionRef(new RNSOption(iSimbol, false)))) == mOptionSet.end())
		return NULL;

	return const_cast<RNSOption*>(&**itr);
}
