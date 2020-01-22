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
#include "Arguments.h"
#include <WinLib/VersionInfo.h>
#include <iostream>

using namespace std;
using namespace uh;
namespace po = boost::program_options;

const char * const k_db_info = "db_info";
const char * const k_conv_table_path = "conv_table_path";
const char * const k_conv_field_path = "conv_field_path";
const char * const k_cnv_param_path = "cnv_param_path";
const char * const k_output_dir = "output_dir";
const char * const k_output_header = "output_header";
const char * const k_output_delimiter = "output_delimiter";
const char * const k_output_encode = "output_encode";
const char * const k_output_newline = "output_newline";
const char * const k_output_georef = "output_georef";
const char * const k_output_shapetype = "output_shapetype";
const char * const k_output_polygontype = "output_polygontype";
const char * const k_runlog_path = "runlog_path";
const char * const k_errlog_path = "errlog_path";

// �R�}���h���C�������̉��
bool Arguments::parse(int argc, _TCHAR* argv[])
{
	po::options_description desc("How to use");

	try {
		desc.add_options()
			(k_db_info,				tvalue<tstring>(&m_db_info),			"[�K�{]DB�ڑ����")
			(k_conv_table_path,		tvalue<tstring>(&m_conv_table_path),	"[�K�{]�ϊ��e�[�u���̃p�X")
			(k_conv_field_path,		tvalue<tstring>(&m_conv_field_path),	"[�K�{]�ϊ��t�B�[���h�Ή��\�̃p�X")
			(k_cnv_param_path,		tvalue<tstring>(&m_cnv_param_path),		"[�K�{]���W�ϊ��p�p�����[�^�t�@�C��")
			(k_output_dir,			tvalue<tstring>(&m_output_dir),			"[�K�{]�f�[�^�o�̓f�B���N�g��")
			(k_output_header,												"[�I��]�o�̓f�[�^�Ƀw�b�_�o�͂��s��")
			(k_output_delimiter,	tvalue<tstring>(&m_output_delimiter),	"[�K�{]�o�̓f�[�^�̃t�H�[�}�b�g (comma,tab)")
			(k_output_encode,		tvalue<tstring>(&m_output_encode),		"[�K�{]�o�̓f�[�^�̕����R�[�h (UTF-8,SHIFT_JIS)")
			(k_output_newline,		tvalue<tstring>(&m_output_newline),		"[�K�{]�o�̓f�[�^�̉��s�R�[�h (CR,LF,CRLF)")
			(k_output_georef,		tvalue<tstring>(&m_output_georef),		"[�K�{]�o�̓f�[�^�̑��n�n (4301,4612,6668)")
			(k_output_shapetype,	tvalue<tstring>(&m_output_shapetype),	"[�K�{]�o�̓f�[�^�̌`��^�C�v (NORMAL,WKT)")
			(k_runlog_path,			tvalue<tstring>(&m_runlog_path),		"[�K�{]���s���O")
			(k_errlog_path,			tvalue<tstring>(&m_errlog_path),		"[�K�{]�G���[���O")
			;

		po::store(po::parse_command_line(argc, argv, desc), m_vm);
		po::notify(m_vm);

		// �K�{�p�����[�^�̃`�F�b�N
		bool good = true;
		if(m_db_info.empty() ||
			m_conv_table_path.empty() ||
			m_conv_field_path.empty() ||
			m_cnv_param_path.empty() ||
			m_output_dir.empty() ||
			m_output_delimiter.empty() ||
			m_output_encode.empty() ||
			m_output_newline.empty() ||
			m_output_georef.empty() ||
			m_output_shapetype.empty() ||
			m_runlog_path.empty() ||
			m_errlog_path.empty()){
				std::cerr << "�K�{�I�v�V�������w�肳��Ă��܂���" << std::endl;
				good = false;
		} else {
			// �ϊ��e�[�u���̑��݊m�F
			if(!PathFileExists(m_conv_table_path.c_str())){
				std::cerr << k_conv_table_path << "�I�v�V���� : �u" << toStr(m_conv_table_path) << "�v�̃t�@�C���͑��݂��܂���" << std::endl;
				good = false;
			}
			// �ϊ��t�B�[���h�Ή��\�̑��݊m�F
			if(!PathFileExists(m_conv_field_path.c_str())){
				std::cerr << k_conv_field_path << "�I�v�V���� : �u" << toStr(m_conv_field_path) << "�v�̃t�@�C���͑��݂��܂���" << std::endl;
				good = false;
			}
			// ���n�n�ϊ��p�����[�^�̑��݊m�F
			if(!PathFileExists(m_cnv_param_path.c_str())){
				std::cerr << k_cnv_param_path << "�I�v�V���� : �u" << toStr(m_cnv_param_path) << "�v�̃t�@�C���͑��݂��܂���" << std::endl;
				good = false;
			}
			// �o�͐�f�B���N�g���̑��݊m�F
			if(!PathIsDirectory(m_output_dir.c_str())){
				std::cerr << k_output_dir << "�I�v�V���� : �u" << toStr(m_output_dir) << "�v�̃f�B���N�g���͑��݂��܂���" << std::endl;
				good = false;
			}
			// �w�b�_�o�͗L���ݒ�
			if(m_vm.count(k_output_header)){
				m_output_header = true;
			}
			// ���O������
			if(good){
				if(!initLog(m_runlog_path.c_str(), m_errlog_path.c_str())){
					good = false;
				}
			}
		}
		if(!good){
			std::cerr << desc << std::endl;
			return false;
		}
	} catch(const std::exception& e){
		std::cerr << e.what() << std::endl;
		std::cerr << desc << std::endl;
		return false;
	}
	return true;
}

CString Arguments::GetOption() const
{
	CString strOption = _T("[option]\n");
	for(const auto& op : m_vm){
		CString strOptionLine;
		if(op.first == k_output_header){
			strOptionLine.Format(_T("   --%s\n"), CString(op.first.c_str()));
		} else {
			strOptionLine.Format(_T("   --%s %s\n"), CString(op.first.c_str()), CString(op.second.as<uh::tstring>().c_str()));
		}
		strOption += strOptionLine;
	}

	return strOption;
}

CString Arguments::GetOptionLine(const CString& strParam, const CString& strValue) const
{
	CString strOptionLine;
	strOptionLine.Format(_T("   --%s %s\n"), strParam, strValue);

	return strOptionLine;
}

bool Arguments::initLog(const CString& strRunLog, const CString& strErrLog)
{
	// ���O�t�@�C���쐬
	if(!CLog::GetInstance().SetRunLog(strRunLog) ||
		!CLog::GetInstance().SetErrLog(strErrLog)){
		return false;
	}
	CLog::GetInstance().PrintLog(false, false, false, true,_T("#FREESTYLELOG"));
	CLog::GetInstance().PrintLog(false, false, false, true,_T("ERROR_TYPE\tTABLE_NAME\tOBJECTID\tERROR_CODE\tERROR_MSG\t"));

	CVersion cVer;
	CString strMsg;
	strMsg.Format(_T("%s FILEVERSION:%s  PRODUCTVERSION:%s"), cVer.GetOriginalFilename(), cVer.GetFileVersion(), cVer.GetProductVersion());
	CLog::GetInstance().PrintLog(true, false, true, false, strMsg);
	CLog::GetInstance().PrintLog(true, false, true, false, GetOption());

	strMsg.Format(_T("Start : %s\n"), getNowTime());
	CLog::GetInstance().PrintLog(true, false, true, false, strMsg);

	return true;
}
