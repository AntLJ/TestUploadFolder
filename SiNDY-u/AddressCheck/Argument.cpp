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

// �K�v�ȃI�v�V������񂪂��邩���`�F�b�N����
bool Argument::checkParameter(const po::variables_map& vm, LPCSTR option, LPCSTR description) const
{
	if( vm.count(option) == 0 ){
		cerr << description << "�����ݒ�ł��B" << std::endl;
		return false;
	}
	return true;
}

// �R�}���h���C�������̉��
bool Argument::setParameter(int argc, TCHAR** argv)
{
	po::options_description desc("Allowed options");

	const char * const k_edtAddrDB  = "edt_addr_db";
	const char * const k_meshDB     = "mesh_db";
	const char * const k_mapDB      = "map_db";
	const char * const k_cityMeshList   = "city_mesh_list";
	const char * const k_baseMeshList   = "base_mesh_list";
	
	const char * const k_layer      = "layer";
	const char * const k_logDir     = "log_dir";
	const char * const k_master     = "master";
	const char * const k_iniFile    = "ini_file";
	const char * const k_runLog     = "run_log";

	// �R�}���h���C����������
	Arguments args;
	desc.add_options()
		(k_edtAddrDB,  uh::tvalue<uh::tstring>(&args.edtAddrDB),"[�K�{]�Z���i�ҏW�jDB")
		(k_meshDB,     uh::tvalue<uh::tstring>(&args.meshDB),   "[�K�{]�s�s�n�}���b�V��DB")
		(k_mapDB,     uh::tvalue<uh::tstring>(&args.mapDB),     "[�K�{]�w�i�|���S��DB")
		(k_cityMeshList,   uh::tvalue<uh::tstring>(&args.cityMeshList), "[�C��]�s�s�n�}���b�V���R�[�h���X�g")
		(k_baseMeshList,   uh::tvalue<uh::tstring>(&args.baseMeshList), "[�C��]�x�[�X���b�V���R�[�h���X�g")
		(k_layer,      uh::tvalue<uh::tstring>(&args.layer),    "[�K�{]�`�F�b�N�Ώۃ��X�g")
		(k_logDir,     uh::tvalue<uh::tstring>(&args.logDir),   "[�K�{]���O�t�H���_")
		(k_iniFile,    uh::tvalue<uh::tstring>(&args.iniFile),  "[�K�{]�ݒ�t�@�C��")
		(k_master,     uh::tvalue<uh::tstring>(&args.master),   "[�C��]�e�L�X�g�Z���}�X�^")
		(k_runLog,     uh::tvalue<uh::tstring>(&args.runLog),   "[�C��]���s���O�t�@�C��")
		;

	if(argc == 1) {
		cerr << "�R�}���h���C�������ɖ�肪����܂�" << endl;
		return false;
	}

	// argc, argv ����͂��āA���ʂ�vm�Ɋi�[
	po::variables_map vm;
	store(parse_command_line(argc, argv, desc), vm);
	notify(vm);

	// �K�{�p�����[�^�`�F�b�N
	bool bMust = true;

	bMust &= checkParameter( vm, k_edtAddrDB,       "[�K�{]�Z���i�ҏW�jDB");
	bMust &= checkParameter( vm, k_meshDB,          "[�K�{]�s�s�n�}���b�V��DB");
	bMust &= checkParameter( vm, k_mapDB,           "[�K�{]�w�i�|���S��DB");
	bMust &= checkParameter( vm, k_layer,           "[�K�{]�`�F�b�N�Ώۃ��X�g");
	bMust &= checkParameter( vm, k_logDir,          "[�K�{]���O�t�H���_");
	bMust &= checkParameter( vm, k_iniFile,         "[�K�{]�ݒ�t�@�C��");

	// �p�����[�^������ł͂Ȃ��ꍇ�́A�I�v�V�����\�����ďI��
	if( !bMust){
		cerr << "�Z���n�`�F�b�N�c�[��" << std::endl;
		cerr << desc << std::endl;
		return false;
	}	

	m_addressDBPath = args.edtAddrDB.c_str();
	m_meshDBPath    = args.meshDB.c_str();
	m_mapDBPath     = args.mapDB.c_str();
	m_masterPath    = args.master.c_str();
	m_iniFilePath   = args.iniFile.c_str();
	m_cityMeshListPath  = args.cityMeshList.c_str();
	m_baseMeshListPath  = args.baseMeshList.c_str();
	m_layer         = args.layer.c_str();
	m_logDir        = args.logDir.c_str();
	m_runLog        = args.runLog.c_str();

	return true;
}

// �����̎擾
const CString& Argument::getValue(argumentType type)
{
	switch	( type )
	{
	case kAddressDBPath:
		return m_addressDBPath;
	case kMeshDBPath:
		return m_meshDBPath;
	case kMapDBPath:
		return m_mapDBPath;
	case kMasterPath:
		return m_masterPath;
	case kIniFilePath:
		return m_iniFilePath;
	case kCityMeshListPath:
		return m_cityMeshListPath;
	case kBaseMeshListPath:
		return m_baseMeshListPath;
	case kLayer:
		return m_layer;
	case kLogDir:
		return m_logDir;
	case kRunLog:
		if(m_runLog.IsEmpty())
		{
			CString strLogSuffix;
			SYSTEMTIME st;
			::GetLocalTime( &st );
			strLogSuffix.Format( _T("\\AddressCheck_%d%02d%02d%02d%02d%02d.log"), st.wYear, st.wMonth, st.wDay, st.wHour, st.wMinute, st.wSecond );

			m_runLog = m_logDir + strLogSuffix;
		}
		return m_runLog;
	default:
		break;
	}

	return _T("");
}