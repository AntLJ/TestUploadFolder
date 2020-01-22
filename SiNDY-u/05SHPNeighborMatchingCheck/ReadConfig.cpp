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
#include "ReadConfig.h"
#include <fstream>
#include <boost/tokenizer.hpp>
#include <shlwapi.h>
#include "Function.h"

CReadConfig::CReadConfig(void)
{
}

CReadConfig::~CReadConfig(void)
{
}

bool CReadConfig::getenv(const _TCHAR* ConfigFile, Parameters * pParams)
{
	std::ifstream ifs(ConfigFile);

	if(! ifs.is_open()){
		std::cerr << "�ݒ�t�@�C���̓ǂݍ��݂Ɏ��s���܂���" << std::endl;
		return false;
	}

	while(!ifs.eof())
	{
		char aBuffer[256];
		ifs.getline(aBuffer,sizeof(aBuffer),'\n');

		if(aBuffer[0] == _T('#') || aBuffer[0]==0){
			continue;
		}

		std::string aStr = aBuffer;

		typedef boost::char_separator<char> char_separator;
		typedef boost::tokenizer<char_separator,std::string::const_iterator,std::string> tokenizer;

		char_separator sep("\t");
		tokenizer tokens(aStr,sep);

		tokenizer::iterator tok_iter = tokens.begin();

		std::string aCategory = tok_iter->c_str();		
		tok_iter++;

		if(aCategory == "MESHLIST"){
			pParams->e_MeshList = tok_iter->c_str();
		}
		else if(aCategory == "LAYERLIST"){
			pParams->e_LayerList = tok_iter->c_str();
		}
		else if(aCategory == "TKYDATAPATH"){
			pParams->e_TKYDataPath = tok_iter->c_str();
		}
		else if(aCategory == "JGDDATAPATH"){
			pParams->e_JGDDataPath = tok_iter->c_str();
		}
		else if(aCategory == "AREA"){
			pParams->e_Area = tok_iter->c_str();
		}
		else if(aCategory == "DATASET"){
			pParams->e_DataSet = tok_iter->c_str();
		}
		else if(aCategory == "ERROR"){
			pParams->e_ErrorDir = tok_iter->c_str();
		}
		else if(aCategory == "BORDER"){
			pParams->e_Border = std::atof(tok_iter->c_str());
		}
		else if(aCategory == "WALK"){
			pParams->e_WalkFlag = true;
		}

	}

	//MeshList�̑��݊m�F
	if(! PathFileExists(Function::filestr(pParams->e_MeshList.c_str()).c_str())){
		std::cerr << "�ݒ�t�@�C����MESHLIST���s���ł�" << std::endl;
		return false;
	}
	
	//���C�����X�g�̑��݊m�F
	if(! PathFileExists(Function::filestr(pParams->e_LayerList.c_str()).c_str())){
		std::cerr << "�ݒ�t�@�C����LAYERLIST���s���ł�" << std::endl;
		return false;
	}

	//TKYDATAPATH�̃f�B���N�g���m�F
	if(! PathIsDirectory(Function::filestr(pParams->e_TKYDataPath.c_str()).c_str())){
		std::cerr << "�ݒ�t�@�C����TKYDATAPATH���s���ł�" << std::endl;
		return false;
	}

	//JGDDATAPATH�̃f�B���N�g���m�F
	if(! PathIsDirectory(Function::filestr(pParams->e_JGDDataPath.c_str()).c_str())){
		std::cerr << "�ݒ�t�@�C����JGDDATAPATH���s���ł�" << std::endl;
		return false;
	}

	//AREA�̐ݒ�m�F
	if(pParams->e_Area != "all" && pParams->e_Area != "part"){
		std::cerr << "�ݒ�t�@�C����AREA���s���ł�" << std::endl;
		std::cerr << "�S�̃`�F�b�N�Fall �����I�ȃ`�F�b�N�Fpart" << std::endl;
		return false;
	}

	//DATASET�̐ݒ�m�F
	if(pParams->e_DataSet != "top" && pParams->e_DataSet != "middle" && pParams->e_DataSet != "base"
		/*&& pParams->e_DataSet != "road"*/ && pParams->e_DataSet != "city" ){
		std::cerr << "�ݒ�t�@�C����DATASET���s���ł�" << std::endl;
		std::cerr << "�g�b�v�}�b�v�Ftop �~�h���}�b�v�Fmiddle �x�[�X�}�b�v�E���H:base �s�s�n�}:city" << std::endl;
		return false;
	}

	//ERROR�̐ݒ�m�F
	if(!PathIsDirectory(Function::filestr(pParams->e_ErrorDir.c_str()).c_str())){
		std::cerr << "�ݒ�t�@�C����ERROR���s���ł�" << std::endl;
		return false;
	}

	return true;
}


		

		



	    











	
