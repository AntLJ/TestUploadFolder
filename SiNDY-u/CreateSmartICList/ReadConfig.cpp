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

namespace{
//������̖�����\�}�[�N���������ꍇ�̓J�b�g�����������Ԃ�
CString CutEndYenMark(CString str)
{
	int len = str.GetLength();
	int index = str.Find(_T("\\"), len-1);
	if(index != -1){
		CString str2 = 	str.Left(len-1);
		return str2;
	}
	return str;
}

/*
 * @brief	Meshlist��胁�b�V���R�[�h�̎擾
 */
bool GetMeshlist(CString strMeshlistPath, std::set<long>& r_MeshlistSet)
{
	//�t�@�C���̓ǂݍ���
	std::ifstream aStream(strMeshlistPath);

	if( !aStream ){
		std::cout << std::endl;
		std::cout << LPCTSTR(strMeshlistPath) << "\t��ǂݍ��ނ��Ƃ��ł��܂���." << std::endl;
		return false;
	}

	//1�s���ǂݍ���
	char	aLineBuffer[128];
	while( aStream.good() ){
		aStream.getline(aLineBuffer,128);
		if(( strlen(aLineBuffer))&&( aLineBuffer[0] == '#')){
			continue;
		}
		int	aMeshcode = atoi(aLineBuffer);
		if( r_MeshlistSet.find(aMeshcode) == r_MeshlistSet.end() && aMeshcode < 700000 && aMeshcode > 300000 ){
			r_MeshlistSet.insert(aMeshcode); 
		}
		else if(aMeshcode != 0){
			std::cout << _T("\t�w�肳�ꂽ���b�V���R�[�h���s��.") << std::endl;
			return false;
		}
	}
	aStream.close();

	return true;
}

/*
 *	@brief	�ݒ�t�@�C���̕\��
 *	@param	eParams	[in]	�p�����[�^
 */
void dispConfig(const Parameters& eParams)
{
	std::cout << _T("���H�ڑ��T�[�o:\t") << LPCTSTR(eParams.m_strNwDB) << std::endl;
	std::cout << _T("�o�͐�:\t") << LPCTSTR(eParams.m_strOut) << std::endl;
	std::cout << _T("�Ώۃ��b�V��:\t") << eParams.m_lMeshSet.size() << _T(" ���b�V��") << std::endl;
	std::cout << _T("--") << std::endl;
}
} //anonymous-namespace


CReadConfig::CReadConfig(void)
{
}

CReadConfig::~CReadConfig(void)
{
}

//�ݒ�t�@�C�����̎擾
Parameters CReadConfig::getenv(const char* strConfigFile)
{
	Parameters eParams;

	FILE *fp;
	if(! fopen_s(&fp, strConfigFile, "r")){
		char    aStr[256];
		while(fgets(aStr, 256, fp) != NULL){
			CString strLine = aStr;
			strLine.Replace(_T('\n'), _T('\0'));

			if(strLine.Left(0) == _T("#")) 
				continue;

			if(aStr[0] == '#')	continue;

			int iStart = 0;
			CString strToken = NULL;
			strToken = strLine.Tokenize(_T("\t"),iStart);
			if(strToken == _T(""))
				continue;
			CString strToken2 = strLine.Tokenize(_T("\t"),iStart);

			//< # �Ώ�NWDB��
			if(strToken == _T("NwDB")){
				eParams.m_strNwDB = strToken2;
			}
			//< # ���b�V�����X�g�p�X
			else if(strToken == _T("Meshlist")){
				if(! GetMeshlist(strToken2, eParams.m_lMeshSet))
					throw std::runtime_error(_T("���b�V�����X�g�̎擾�Ɏ��s\t���������b�V�����X�g���w�肵�ĉ�����."));
			}
			//< # �o��
			else if(strToken == _T("Out")){
				CString tmp = strToken2;
				eParams.m_strOut = CutEndYenMark(tmp);
			}
			else{
				continue;
			}
		}
		(void)fclose(fp);
	}
	else{
		throw std::runtime_error(_T("�ݒ�t�@�C���̓ǂݍ��݂Ɏ��s."));
	}
	if(eParams.m_strNwDB.GetLength() == 0){
		throw std::runtime_error(_T("���H�ڑ��T�[�o�����ݒ�\tNwDB�Ŏw�肵�ĉ�����."));
	}
	if(eParams.m_strOut.GetLength() == 0){
		throw std::runtime_error(_T("�o�͐悪���ݒ�\tOut�Ŏw�肵�ĉ�����."));
	}
	if(eParams.m_lMeshSet.empty()){
		throw std::runtime_error(_T("���b�V�����X�g�����ݒ�\tMeshlist�Ŏw�肵�ĉ�����."));
	}

	dispConfig(eParams);

	return eParams;

}
