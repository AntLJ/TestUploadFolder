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

// diffshapeadd.cpp : �R���\�[�� �A�v���P�[�V�����̃G���g�� �|�C���g���`���܂��B
//

#include "stdafx.h"

#include "diffshapeadd.h"
#include "diffadd.h"
#include "DiffAddSDE.h"

#include <arctl/coinitializer.h>
#include <sindy/workspace.h>
#include <sindy/schema.h>
#include <versioninfo.h>
#include <TDC/useful_headers/flush_scope_guard.h>

int _tmain(int argc, _TCHAR* argv[])
{

	uh::flush_scope_guard fsg;

	// �c�[���̃o�[�W�����o��
	CVersion aVersion;
	CString strFileVersion = aVersion.GetFileVersion();
	CString strProductVersion = aVersion.GetProductVersion();
	std::cout << "diffshapeadd.exe\tFILEVERSION : " << CT2A(strFileVersion) << " PRODUCTVERSION : " << CT2A(strProductVersion) << "\n";
	std::cout << "start : " << CT2A(nowtime()) << "\n";

	try
	{
		const arctl::coinitializer coinit;

		CInputData cInputData;
		// �R�}���h���C�������擾 =====================================================================================
		for(int i = 0; i < argc; ++i){
			CString strArg = argv[i];
			if(_tcscmp(strArg, _T("-dt")) == 0 || _tcscmp(strArg, _T("--do-type")) == 0){
				i++;
				cInputData.lDoType = _ttol(argv[i]);
			} else if(_tcscmp(strArg, _T("-bsd")) == 0 || _tcscmp(strArg, _T("--base-shape-dir")) == 0){
				i++;
				cInputData.strSShapeDir = argv[i];
			} else if(_tcscmp(strArg, _T("-dsd")) == 0 || _tcscmp(strArg, _T("--diff-shape-dir")) == 0){
				i++;
				cInputData.strDiffShapeDir = argv[i];
			} else if(_tcscmp(strArg, _T("-dal")) == 0 || _tcscmp(strArg, _T("--diff-add-list")) == 0){
				i++;
				cInputData.strDiffAddList = argv[i];
			} else if(_tcscmp(strArg, _T("-md")) == 0 || _tcscmp(strArg, _T("--m-division")) == 0){
				i++;
				cInputData.lMDivision = _ttol(argv[i]);
			} else if(_tcscmp(strArg, _T("-sc")) == 0 || _tcscmp(strArg, _T("--sde-connect")) == 0){
				i++;
				cInputData.strSDEConnect = argv[i];
			} else if(_tcscmp(strArg, _T("-ml")) == 0 || _tcscmp(strArg, _T("--mesh-list")) == 0){
				i++;
				cInputData.strMeshlist = argv[i];
			} 
		}
		// �ڑ����\��
		if(cInputData.lDoType == 0){
			std::cout << "# BASESHAPEDIR=" << CT2A(cInputData.strSShapeDir) << "\n";
			std::cout << "# DIFFSHAPEDIR=" << CT2A(cInputData.strDiffShapeDir) << "\n";
			std::cout << "# DIFFADDLIST=" << CT2A(cInputData.strDiffAddList) << "\n";
			if(cInputData.lMDivision < 0)
				cInputData.lMDivision = 3;	// �l���w�肳��Ă��Ȃ�������u3�v�������l�Ƃ���
			std::cout << "# MDIVISION=" << cInputData.lMDivision << "\n";
			std::cout << "\n";
		} else if(cInputData.lDoType == 1) {
			std::cout << "# SDECONNECT=" << CT2A(cInputData.strSDEConnect) << "\n";
			std::cout << "# DIFFSHAPEDIR=" << CT2A(cInputData.strDiffShapeDir) << "\n";
			std::cout << "# MESHLIST=" << CT2A(cInputData.strMeshlist) << "\n";
			std::cout << "\n";
		} else {
			std::cerr << "#ERROR\t[-dt]���w�肵�Ă��������B\n";
			std::cout << "end : " << CT2A(nowtime()) << "\n";
			return false;
		}

		// �R�}���h���C�������擾 �����܂�============================================================================

		// �R�}���h���C�������̐��딻�� ===============================================================================
		if(cInputData.lDoType == 0){
			if(!PathIsDirectory(cInputData.strSShapeDir)){
				std::cerr << "#ERROR\t" << CT2A(cInputData.strSShapeDir) << "\tS�Ќ��f�[�^�̃f�B���N�g�������݂��Ȃ�\n";
				std::cout << "end : " << CT2A(nowtime()) << "\n";
				return 0;
			}
			if(!PathIsDirectory(cInputData.strDiffShapeDir)){
				std::cerr << "#ERROR\t" << CT2A(cInputData.strDiffShapeDir) << "\t�����f�[�^�̃f�B���N�g�������݂��Ȃ�\n";
				std::cout << "end : " << CT2A(nowtime()) << "\n";
				return 0;
			}
			if(!PathFileExists(cInputData.strDiffAddList)){
				std::cerr << "#ERROR\t" << CT2A(cInputData.strDiffAddList) << "\t�����ǉ����X�g�����݂��Ȃ�\n";
				std::cout << "end : " << CT2A(nowtime()) << "\n";
				return 0;
			}
		} else if(cInputData.lDoType == 1){
			if(!PathIsDirectory(cInputData.strDiffShapeDir)){
				std::cerr << "#ERROR\t" << CT2A(cInputData.strDiffShapeDir) << "\t�����f�[�^�̃f�B���N�g�������݂��Ȃ�\n";
				std::cout << "end : " << CT2A(nowtime()) << "\n";
				return 0;
			}
			if(!cInputData.strMeshlist.IsEmpty()){
				if(!PathFileExists(cInputData.strMeshlist)){
					std::cerr << "#ERROR\t" << CT2A(cInputData.strMeshlist) << "\t�����f�[�^�ǉ��̃��b�V�����X�g�����݂��Ȃ�\n";
					std::cout << "end : " << CT2A(nowtime()) << "\n";
					return 0;
				}
				std::ifstream ifs;
				ifs.open(cInputData.strMeshlist);
				if(ifs.fail())
					return false;
				char cLine[256];
				CString strNum(_T("0123456789"));
				while(ifs.getline(cLine,256)){
					CString strLine(cLine);
					if(!strLine.IsEmpty()){
						long lIndex = StrSpn(strLine, strNum);
						if(strLine.GetLength() == 6 && strLine.GetLength() == lIndex)
							cInputData.setMeshlist.insert(strLine);
						else
							std::cerr << "#WARNING\t" << CT2A(strLine) << "\t2�����b�V������Ȃ��̂ŃX���[���܂�\n";
					}
				}
			} else {
				std::cerr << "#NOTICE\t���b�V�����X�g�̎w�肪�����̂ō����f�[�^�f�B���N�g���ɂ��邷�ׂẴf�[�^���C���|�[�g�ΏۂɂȂ�܂��B\n";
			}

			// ���[�N�X�y�[�X�擾
			cInputData.ipSDEWork = sindy::create_workspace(cInputData.strSDEConnect);
			if(!cInputData.ipSDEWork){
				std::cerr << "#ERROR\t" << CT2A(cInputData.strSDEConnect) << "\tSDE���[�N�X�y�[�X�ڑ����s\n";
				std::cout << "end : " << CT2A(nowtime()) << "\n";
				return 0;
			}

			// �t�B�[�`���N���X�A�e�[�u���ނ��擾���Ă���
			IFeatureWorkspacePtr(cInputData.ipSDEWork)->OpenFeatureClass(CComBSTR(sindy::schema::road_link::kTableName), &cInputData.ipLinkClass);
			IFeatureWorkspacePtr(cInputData.ipSDEWork)->OpenFeatureClass(CComBSTR(sindy::schema::road_node::kTableName), &cInputData.ipNodeClass);

			if(!cInputData.ipLinkClass || !cInputData.ipNodeClass){
				std::cerr << "#ERROR\t" << CT2A(cInputData.strSDEConnect) << "\t�t�B�[�`���N���X�A�e�[�u���擾���s\n";
				std::cout << "end : " << CT2A(nowtime()) << "\n";
				return 0;
			}
		}
		// �R�}���h���C�������̐��딻�� �����܂� =====================================================================

		// �����J�n ===============================================
		if(cInputData.lDoType == 0){
			// dotype=0�̏ꍇ�͍���SHP�Ƀf�[�^��ǉ����郂�[�h
			CDiffAdd cDiffAdd(cInputData);
			if(cDiffAdd.run()){
				std::cout << "����I��\n";
			} else {
				std::cout << "�ُ�I��\n";
			}
		} else if(cInputData.lDoType == 1){
			// dotype=1�̏ꍇ��SDE�ɍ����f�[�^���C���|�[�g���郂�[�h
			CDiffAddSDE cDiffAddSDE(cInputData);
			if(cDiffAddSDE.run()){
				std::cout << "����I��\n";
			} else {
				std::cout << "�ُ�I��\n";
			}
		}
	}
	// ��O����
	catch(const std::exception& e) {
		std::wcout << e.what() << std::endl;
	}
	catch(const _com_error& e) {
		std::wcout << e.ErrorMessage() << std::endl;
	}

	std::cout << "end : " << CT2A(nowtime()) << "\n";
	return 0;
}

CString nowtime()
{
	SYSTEMTIME st;
	GetLocalTime(&st);
	CString strNowTime;
	strNowTime.Format(_T("%d/%02d/%02d %02d:%02d:%02d"), st.wYear, st.wMonth, st.wDay, st.wHour, st.wMinute, st.wSecond);
	return strNowTime;
}
