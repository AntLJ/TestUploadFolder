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

// AjustChibanMesh.cpp : �R���\�[�� �A�v���P�[�V�����p�̃G���g�� �|�C���g�̒�`
//

#include "stdafx.h"
#include <boost/format.hpp>
#include <sindy/workspace.h>
#include <arctl/coinitializer.h>
#include <WinLib/VersionInfo.h>
#include "meshindex.h"
#include "AChiban.h"

namespace{
#ifdef _DEBUG
LPCTSTR DEF_ADR_CONNECT = _T("sindy2012a/sindy2012a/SDE.EDT_SINDY2012A/5151/topaz2");
#else
LPCTSTR DEF_ADR_CONNECT = _T("sindy2006b/sindy2006b/SDE.DEFAULT/5151/topaz2");
#endif

LPCTSTR DEF_CHIBAN = _T("City_Rep_Point");
LPCTSTR DEF_MESH = _T("reference.CityMesh");

LPCTSTR DEF_MESHFIELD = _T("MeshCode");

CString gAdrConnect = DEF_ADR_CONNECT;	///< �Z���n�T�[�o�ڑ�������

CString gMeshFeature = DEF_MESH;		///< �s�s�n�}���b�V���|���S���t�B�[�`���N���X��
CString gChibanFeature = DEF_CHIBAN;	///< �n�ԑ�\�_�t�B�[�`���N���X��
LPCTSTR gConfigFile = NULL;				///< �ݒ�t�@�C����
LPCTSTR gListFile = NULL;				///< ���b�V�����X�g�t�@�C����
CString gWhere;							///< ���b�V���I��p�̏����iSQL�j
CString gMeshField = DEF_MESHFIELD;		///< ���b�V���R�[�h�t�B�[���h���i�͓̂��ꂳ��Ă��Ȃ��������c�j

LPCTSTR gMeshCode = NULL;				///< ���b�V���R�[�h�i�P�̎w��p�j
bool gAllFlag = false;					///< �S�������t���O
bool gChgMode = false;					///< �ύX���[�h
}

/** 
 * @brief �g�p�@�\�� 
 * @param[in]	App	���s���R�}���h���C���̈�� 
 */ 
void PrintUsage(LPCTSTR App)
{
	LPCTSTR AppName;
	AppName = _tcsrchr(App, _T('\\'));
	if(AppName != NULL){
		AppName++;
	}else{
		AppName = App;
	}
	_tprintf(_T("���b�V�����E��̒n�Ԓ��L�����i�t���O�I�t�j�v���O����\n")
			_T("Usage:%s [-C <�ݒ�t�@�C��>] [-G] <���b�V���R�[�h>\n")
			_T(" or   %s [-C <�ݒ�t�@�C��>] [-G] -M <���b�V���R�[�h���X�g>\n")
			_T(" or   %s [-C <�ݒ�t�@�C��>] [-G] -A\n")
			_T(" -G:�ύX���[�h�w��\n")
			, AppName, AppName, AppName);
}

/** 
 * @brief �ȈՃw���v�\�� 
 * @param[in]	App	���s���R�}���h���C���̈�� 
 */ 
void PrintHelp(LPCTSTR App)
{
	PrintUsage(App);
	printf("���b�V�����E��̒n�Ԓ��L�̃t���O���I�t�ɂ��܂��B\n"
			"�T�[�o��o�[�W������ύX����ɂ́A�ݒ�t�@�C�������ϐ��uDB_ADDRESS�v\n"
			"���g�p���ĉ������B\n"
			"�ݒ�t�@�C���̏������́AAjustChibanMesh.txt�������������B\n"
			"�u-A�v�I�v�V�������g�p���A���ݒ�t�@�C�����ŏ������w�肵�Ȃ���΁A"
			"�S�Ă̓s�s�n�}�G���A���ΏۂɂȂ�܂��B\n");
}

/** 
 * @brief �ݒ�t�@�C���ǂݍ��� 
 * @retval true		����
 * @retval false	���s
 */ 
bool AnalyseConfig()
{
	FILE *fp;
	_TCHAR Buf[256];
	bool aResult = true;
	LPTSTR p;
	if((fp = _tfopen(gConfigFile, _T("rt"))) != NULL){
		while(_fgetts(Buf, 256, fp)){
			if(Buf[0] != _T('#') && Buf[0] != _T('\n')){
				int aLen = _tcslen(Buf)-1;
				if(Buf[aLen] == _T('\n') || Buf[aLen] == _T('\r')){
					Buf[aLen] = '\0';
				}
				if(_tcsncmp(Buf, _T("DB_ADDRESS"), 10) == 0){
					if((p = _tcschr(Buf, _T('\t'))) != NULL){
						p++;
						gAdrConnect = p;
					}else{
						_ftprintf(stderr, _T("�ݒ�t�@�C���̃t�H�[�}�b�g���Ⴂ�܂�:%s\n"), Buf);
						aResult = false;
						break;
					}
				}else if(_tcsncmp(Buf, _T("FC_CITY_MESH"), 12) == 0){
					if((p = _tcschr(Buf, _T('\t'))) != NULL){
						p++;
						gMeshFeature = p;
					}else{
						_ftprintf(stderr, _T("#�ݒ�t�@�C���̃t�H�[�}�b�g���Ⴂ�܂�:%s\n"), Buf);
						aResult = false;
						break;
					}
				}else if(_tcsncmp(Buf, _T("FC_CITY_REP_POINT"), 17) == 0){
					if((p = _tcschr(Buf, _T('\t'))) != NULL){
						p++;
						gChibanFeature = p;
					}else{
						_ftprintf(stderr, _T("#�ݒ�t�@�C���̃t�H�[�}�b�g���Ⴂ�܂�:%s\n"), Buf);
						aResult = false;
						break;
					}
				}else if(_tcsncmp(Buf, _T("WHERE"), 5) == 0){
					if((p = _tcschr(Buf, _T('\t'))) != NULL){
						p++;
						gWhere = p;
					}else{
						_ftprintf(stderr, _T("#�ݒ�t�@�C���̃t�H�[�}�b�g���Ⴂ�܂�:%s\n"), Buf);
						aResult = false;
						break;
					}
				}else{
					_ftprintf(stderr, _T("#�ݒ�t�@�C���̃t�H�[�}�b�g���Ⴂ�܂�:%s\n"), Buf);
					aResult = false;
					break;
				}
			}
		}
		fclose(fp);
	}else{
		_ftprintf(stderr, _T("#�ݒ�t�@�C�����J���܂���:%s\n"), gConfigFile);
		aResult = false;
	}
	return aResult;
}

/**
 * @brief �R�}���h���C����́B
 *
 * @param argc	[in]	���C���֐��̑�����
 * @param argv	[in]	���C���֐��̑�����
 *
 * @retval true		����
 * @retval false	���s�A�G���[
 */
bool AnalyseArg(int argc, _TCHAR* argv[])
{
	if(argc < 2){
		PrintUsage(argv[0]);
		return false;
	}else{
		int i = 1;
		while(i < argc){
			if(argv[i][0] == '-' || argv[i][0] == '/'){
				switch(argv[i][1] ){
					case _T('?'):
						PrintHelp(argv[0]);
						return false;
						break;
					case _T('a'):
					case _T('A'):
						if(gListFile != NULL){
							PrintUsage(argv[0]);
							return false;
						}
						gAllFlag = true;
						break;
					case _T('c'):
					case _T('C'):
						i++;
						gConfigFile = argv[i];
						break;
					case _T('g'):
					case _T('G'):
						gChgMode = true;
						break;
					case _T('m'):
					case _T('M'):
						if(gAllFlag){
							PrintUsage(argv[0]);
							return false;
						}
						i++;
						gListFile = argv[i];
						break;
					default:
						_ftprintf(stderr, _T("#�s���ȃI�v�V�������w�肳��܂����B\n"));
						PrintUsage(argv[0]);
						return false;
						break;
				}
			}else{
				break;
			}
			i++;
		}
		if(i == argc-1){
			if(!gAllFlag && gListFile == NULL){
				gMeshCode = argv[i];
			}else{
				PrintUsage(argv[0]);
				return false;
			}
		}else if (i == argc){
			if(!gAllFlag && gListFile == NULL){
				PrintUsage(argv[0]);
				return false;
			}
		}else{
			PrintUsage(argv[0]);
			return false;
		}
		if(gConfigFile != NULL){
			return AnalyseConfig();
		}
	}
	return true;
}

/**
 * @brief ���ϐ��擾
 *
 * @retval true		�����i�����Ԃ��Ȃ��j
 */
bool GetEnv(void)
{
	LPCTSTR aEnv = _tgetenv(_T("DB_ADDRESS"));
	if(aEnv != NULL){
		gAdrConnect = aEnv;
	}
	
	aEnv = _tgetenv(_T("FC_CITY_MESH"));
	if(aEnv != NULL){
		gMeshFeature = aEnv;
	}
	aEnv = _tgetenv(_T("FC_CITY_REP_POINT"));
	if(aEnv != NULL){
		gChibanFeature = aEnv;
	}
	return true;
}


/**
 * @brief ���[�N�X�y�[�X�̃I�[�v���i�T�[�o�ւ̐ڑ��j�B
 *
 * @param argc	[in]	�ڑ�������
 *
 * @retval NULL�ȊO	���[�N�X�y�[�X�̃|�C���^
 * @retval NULL	���s�A�G���[
 */
IWorkspacePtr ConnectSDE( LPCTSTR lpszConnectStr )
{
	// �ڑ�
	_tprintf(_T("%s�ɐڑ���..."), lpszConnectStr);
	IWorkspacePtr ipWorkspace =  sindy::create_workspace(lpszConnectStr);
	if( NULL == ipWorkspace )
	{
		_tprintf(_T("[%s]�ɐڑ��ł��Ȃ������x��\n") , lpszConnectStr);
		_ftprintf(stderr, _T("[%s]�ɐڑ��ł��Ȃ������x��\n") , lpszConnectStr);
		return NULL;
	}
	_tprintf(_T("�ڑ����܂���\n"));
	_ftprintf(stderr, _T("#%s�ɐڑ�\n"), sindy::workspace_address(ipWorkspace).c_str());
	return ipWorkspace;
}

/**
 * @brief ���s���{�́B
 *
 */
void Execute()
{
	// �ڑ�
	IWorkspacePtr ipAdrWorkspace = ::ConnectSDE(gAdrConnect);
	ATLASSERT( ipAdrWorkspace != NULL );
	if( ipAdrWorkspace == NULL ){
		return;
	}
	IFeatureWorkspacePtr ipAdrFeatureWorkspace(ipAdrWorkspace);
	ATLASSERT( ipAdrFeatureWorkspace != NULL );

	IFeatureClassPtr ipMeshClass;
	if(ipAdrFeatureWorkspace->OpenFeatureClass(CComBSTR(gMeshFeature), &ipMeshClass) != S_OK){
		_ftprintf(stderr, _T("#�t�B�[�`���[���J���Ȃ�,%s\n"), gMeshFeature);
		return;
	}

	IFeatureClassPtr ipRepClass;
	if(ipAdrFeatureWorkspace->OpenFeatureClass(CComBSTR(gChibanFeature), &ipRepClass) != S_OK){
		_ftprintf(stderr, _T("#�t�B�[�`���[���J���Ȃ�,%s\n"), gChibanFeature);
		return;
	}

	// �ҏW�J�n�iArcMap �̕ҏW�J�n�Ɠ����j
	IWorkspaceEditPtr ipWorkspaceEdit = ipAdrWorkspace;
	if(gChgMode){
		ipWorkspaceEdit->StartEditing( VARIANT_FALSE );	// Undo Redo ������Ȃ��Ȃ� VARIANT_FALSE
		ipWorkspaceEdit->StartEditOperation();
	}else{
		_tprintf(_T("�`�F�b�N���[�h�œ��삵�܂��B\n"));
		_ftprintf(stderr, _T("#�`�F�b�N���[�h�œ��삵�܂��B\n"));
	}
	AChiban aChiban;
	MeshIndex aMeshIndex(ipMeshClass);
	if(aChiban.init(ipRepClass, gChgMode)){
		if(gAllFlag){
			if(aMeshIndex.init(gMeshField, gWhere)){
				LONG aMesh;
				IFeaturePtr ipFeature;
				while((ipFeature = aMeshIndex.nextFeature(&aMesh)) != NULL){
					aChiban.execAdjust(ipFeature, aMesh);
					if(_kbhit()){
						int aKey = _getch();
						if(aKey == 'Q'){
							_tprintf(_T("�����͒��~����܂����B\n"));
							_ftprintf(stderr, _T("�����͒��~����܂����B\n"));
							break;
						}
					}
				}
			}
		}else if(gListFile != NULL){
			FILE *pFile;
			_TCHAR aBuf[16];
			if((pFile = _tfopen(gListFile, _T("rt"))) != NULL){
				while(_fgetts(aBuf, 16, pFile) != NULL){
					if(aBuf[0] != _T('\n') || aBuf[0] != _T('#')){
						int aLen = _tcsclen(aBuf) -1;
						if(aBuf[aLen] == '\n'){
							aBuf[aLen] = _T('\0');
						}
						if(aMeshIndex.init(aBuf, gMeshField)){
							IFeaturePtr ipFeature;
							LONG aMeshCode;
							if((ipFeature = aMeshIndex.nextFeature(&aMeshCode)) != NULL){
								aChiban.execAdjust(ipFeature, aMeshCode);
							}
						}
					}
					if(_kbhit()){
						int aKey = _getch();
						if(aKey == 'Q'){
							_tprintf(_T("�����͒��~����܂����B\n"));
							_ftprintf(stderr, _T("�����͒��~����܂����B\n"));
							break;
						}
					}
				}
				fclose(pFile);
			}else{
				_ftprintf(stderr, _T("�t�@�C�����J���Ȃ��B:%s\n"), gListFile);
			}
		}else{
			if(aMeshIndex.init(_ttol(gMeshCode), gMeshField)){
				IFeaturePtr ipFeature;
				LONG aMeshCode;
				if((ipFeature = aMeshIndex.nextFeature(&aMeshCode)) != NULL){
					aChiban.execAdjust(ipFeature, aMeshCode);
				}
			}
		}
	}
	if(gChgMode){
		ipWorkspaceEdit->StopEditOperation();
		if(SUCCEEDED(ipWorkspaceEdit->StopEditing( VARIANT_TRUE ))){	// �ҏW��ۑ�����Ȃ� VARIANT_TRUE �ɂ��邱��
			_ftprintf(stderr, _T("#�ۑ������B\n"));	
		}else{
			_ftprintf(stderr, _T("#�ύX���ۑ��ł��Ȃ��B\n"));	
		}
	}
	return ;
}

/**
 * @brief ���C���֐��B
 *
 * @param argc	[in]	�R�}���h���C���̃g�[�N�����i�H�j
 * @param argv	[in]	�R�}���h���C���̃g�[�N����i�H�j
 *
 * @retval 0		����I���i���͂����������Ă͂��Ȃ��j
 * @retval 0�ȊO	�ُ�I��
 */
int _tmain(int argc, _TCHAR* argv[])
{
	::setlocale(LC_ALL, "Japanese");
	try {
		const arctl::coinitializer coinit;
		time_t st_time;
		time(&st_time);
		if(GetEnv()){
			if(AnalyseArg(argc, argv)){
				// �c�[���o�[�W����
				CVersion cVer;
				_ftprintf(stderr, _T("#%s FILEVERSION %s PRODUCTVERSION %s\n"), static_cast<LPCTSTR>(cVer.GetOriginalFilename()), static_cast<LPCTSTR>(cVer.GetFileVersion()), static_cast<LPCTSTR>(cVer.GetProductVersion()));
				SYSTEMTIME  aTime;
				GetLocalTime(&aTime);
				_ftprintf(stderr, _T("#�J�n����: %04d/%02d/%02d %02d:%02d:%02d\n"),
					aTime.wYear, aTime.wMonth, aTime.wDay, aTime.wHour,
					aTime.wMinute, aTime.wSecond);

				Execute();

				time_t tm;
				time(&tm);
				GetLocalTime(&aTime);
				_ftprintf(stderr, _T("#�I������: %04d/%02d/%02d %02d:%02d:%02d\n"),
					aTime.wYear, aTime.wMonth, aTime.wDay, aTime.wHour,
					aTime.wMinute, aTime.wSecond);
				int h,m,s;
				double t = difftime( tm, st_time);
				h = t/3600;
				m = (int(t)%3600)/60;
				s = int(t)%60;
				_tprintf(_T("�g�[�^������ %d����%d��%d�b\n"), h, m, s);
				_ftprintf(stderr, _T("#�g�[�^������ %d����%d��%d�b\n"), h, m, s);
				_tprintf(_T("�I�����܂�\n"));
			}
		}
	}
	catch(const std::exception& e) {
		printf("%s\n", e.what());
		fprintf(stderr, "#Error,%s\n", e.what());
		return 1;
	}
	return 0;
}
