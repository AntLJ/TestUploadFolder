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

#include "LogSys.h"
#include "common.h"
#include "TransParam.h"
#include "PgDataBaseEx.h"
#include "IllConverter.h"

#include <WinLib/CStringTokenizer.h>

/**
 *	@brief	�C���X�g�t�@�C�����Ή��\�擾�֐�
 *	@param	inputListPath	[in] �C���X�g�t�@�C�����Ή��\�p�X
 *	@param	inputListInfo	[out] �C���X�g�t�@�C�����Ή��\���e�i�[�R���e�i
 *	@return	true ����
 *	@return false ���s
 */
bool get_input_list(LPCTSTR inputListPath, std::map<ClassCode, std::map<OID, IllFileNames>>& inputListInfo)
{
	CString errMsg;
	inputListInfo.clear();
	// �t�@�C���I�[�v��
	std::ifstream ifs(inputListPath);
	if (! ifs.is_open()) {
		errMsg.Format(_T("%s�̓ǂݍ��݂Ɏ��s���܂���"), inputListPath);
		LogSys::GetInstance().WriteFreeStyleLog(true, err_level::err, errMsg);
		return false;
	}
	// �ǂݍ���
	std::string temp;
	long count = 0;
	while (std::getline(ifs, temp)) {
		++count;
		CString line = CA2CT(temp.c_str());
		line.Trim();
		// ���R�[�h���� or �擪��#�̏ꍇ��continue
		if (line.IsEmpty() || _T("#") == line.Left(1))
			continue;
		// �^�u�ŕ���
		CStringTokenizer cTokenizer(line, _T("\t"), TOKEN_RET_EMPTY_ALL);
		if (ILLFILEFNUM != cTokenizer.CountTokens()) {
			errMsg.Format(_T("%s�̃t�H�[�}�b�g���s���ł��B:%d�s"), inputListPath, count);
			LogSys::GetInstance().WriteFreeStyleLog(true, err_level::err, errMsg);
			return false;
		}
		// �e�����擾
		// �t�H�[�}�b�g�F
		// �C���X�g��ʃR�[�h\t�d������R�[�h\t�C���X�gOBJECTID\t���C���X�g��\t�����[�X�p�C���X�g��
		CString illCls = cTokenizer.GetNextToken();
		CString dstCode = cTokenizer.GetNextToken();
		ClassCode classCode(illCls, dstCode);
		OID illId = _ttol(cTokenizer.GetNextToken());
		IllFileNames illFileNames;
		illFileNames.e_orgIllName = cTokenizer.GetNextToken();
		illFileNames.e_tgtIllName = cTokenizer.GetNextToken();
		// �R���e�i�ւ̊i�[
		std::map<ClassCode, std::map<OID, IllFileNames>>::iterator clsCodeItr = inputListInfo.find(classCode);
		if (inputListInfo.end() == clsCodeItr) {
			std::map<OID, IllFileNames> id2names;
			id2names.insert(std::pair<OID, IllFileNames>(illId, illFileNames));
			inputListInfo.insert(std::pair<ClassCode, std::map<OID, IllFileNames>>(classCode, id2names));
		} else {
			std::map<OID, IllFileNames>::_Pairib pib;
			pib = clsCodeItr->second.insert(std::pair<OID, IllFileNames>(illId, illFileNames));
			if (! pib.second) {
				errMsg.Format(_T("���objectid�ɑ΂��ĕ����̃����[�X�p�C���X�g�t�@�C�������֘A�t���Ă��܂��B�F�ΏۃC���X�gid�F%d"), illId);
				LogSys::GetInstance().WriteFreeStyleLog(true, err_level::err, errMsg);
				return false;
			}
		}
		
	}
	return true;
}

/**
 *	@brief	�C���X�g�������ݒ�t�@�C���擾�֐�
 *	@param	illSetPath	[in] �C���X�g�������ݒ�t�@�C���p�X
 *	@param	illSetInfo	[out] �C���X�g�������ݒ�t�@�C�����e�i�[�R���e�i
 *	@return true ����
 *	@return false ���s
 */
bool get_ill_set(LPCTSTR illSetPath, std::vector<IllSetRecord>& illSetInfo)
{
	CString errMsg;
	illSetInfo.clear();
	// �t�@�C���I�[�v��
	std::ifstream ifs(illSetPath);
	if (! ifs.is_open()) {
		errMsg.Format(_T("%s�̓ǂݍ��݂Ɏ��s���܂���"), illSetPath);
		LogSys::GetInstance().WriteFreeStyleLog(true, err_level::err, errMsg);
		return false;
	}
	// �ǂݍ���
	std::string temp;
	long count = 0;
	while (std::getline(ifs, temp)) {
		++count;
		CString line = CA2CT(temp.c_str());
		// ���R�[�h���� or �擪��#�̏ꍇ��continue
		if (line.IsEmpty() || _T("#") == line.Left(1) )
			continue;
		// �^�u�ŕ���
		CStringTokenizer cTokenizer(line, _T("\t"), TOKEN_RET_EMPTY_ALL);
		if (ILLSETFNUM != cTokenizer.CountTokens()) {
			errMsg.Format(_T("%s�̃t�H�[�}�b�g���s���ł��B:%d�s"), illSetPath, count);
			LogSys::GetInstance().WriteFreeStyleLog(true, err_level::err, errMsg);
			return false;
		}
		IllSetRecord illSetRecord;
		// �e�����擾
		// �t�H�[�}�b�g
		// �C���X�g��ʃR�[�h\t�d������R�[�h\t�v���t�B�b�N�X\t�g���q\t�t�@�C�������K��\t�i�[�f�B���N�g��
		illSetRecord.e_illCls = CString(cTokenizer.GetNextToken()).Trim();
		illSetRecord.e_dstCode = CString(cTokenizer.GetNextToken()).Trim();
		illSetRecord.e_prefix = CString(cTokenizer.GetNextToken()).Trim();
		illSetRecord.e_suffix = CString(cTokenizer.GetNextToken()).Trim();
		illSetRecord.e_nameRule = CString(cTokenizer.GetNextToken()).Trim();
		illSetRecord.e_illDir = CString(cTokenizer.GetNextToken()).Trim();
		illSetRecord.e_excCond = CString(cTokenizer.GetNextToken()).Trim();
		// �R���e�i�֑}��
		illSetInfo.push_back(illSetRecord);
	}
	return true;
}

bool exec(TransParam &transParam)
{
	CString errMsg;
	// �C���X�g�������ݒ�t�@�C���ǂݍ���
	LogSys::GetInstance().WriteProcLog(true, true, _T("�C���X�g�������ݒ�t�@�C���ǂݍ���\n"));
	std::vector<IllSetRecord> illSetInfo;
	if (! get_ill_set(transParam.GetIllSetFile(), illSetInfo))
		return false;
	// �C���X�g�t�@�C�����Ή��\�ǂݍ���
	LogSys::GetInstance().WriteProcLog(true, true, _T("�C���X�g�t�@�C�����Ή��\�ǂݍ���\n"));
	std::map<ClassCode, std::map<OID, IllFileNames>> inputListInfo;
	if (! get_input_list(transParam.GetInputList(), inputListInfo))
		return false;
	// DB�ڑ�
	LogSys::GetInstance().WriteProcLog(true, true, _T("DB�ڑ�\n"));
	std::shared_ptr<PgDataBaseEx> orgDataBase(new PgDataBaseEx(CT2CA(transParam.GetRepDB())));
	std::shared_ptr<PgDataBaseEx> tgtDataBase(new PgDataBaseEx(CT2CA(transParam.GetEggDB())));
	if (! orgDataBase->connected()) {
		errMsg.Format(_T("DB�ڑ��Ɏ��s���܂����F%s"), transParam.GetRepDB());
		LogSys::GetInstance().WriteFreeStyleLog(true, err_level::err, errMsg);
		return false;
	}
	if (! tgtDataBase->connected()) {
		errMsg.Format(_T("DB�ڑ��Ɏ��s���܂���:%s"), transParam.GetEggDB());
		LogSys::GetInstance().WriteFreeStyleLog(true, err_level::err, errMsg);
		return false;
	}
	// �ϊ�
	LogSys::GetInstance().WriteProcLog(true, true, _T("�ϊ��J�n\n"));
	IllConverter illConverter(orgDataBase, 
							  tgtDataBase, 
							  transParam.GetRepSchema(), 
							  transParam.GetEggSchema());
	if (! illConverter.Convert(illSetInfo, inputListInfo))
		return false;
	LogSys::GetInstance().WriteProcLog(true, true, _T("�ϊ�����\n"));

	return true;
}
