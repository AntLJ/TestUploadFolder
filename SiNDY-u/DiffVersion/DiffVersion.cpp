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

// DiffVersion.cpp : �R���\�[�� �A�v���P�[�V�����p�̃G���g�� �|�C���g�̒�`
//

#include "stdafx.h"
#include "VerDiff.h"
#include <boost/format.hpp>
#include <arctl/coinitializer.h>
#include <sindy/workspace.h>
#include <WinLib/VersionInfo.h>

namespace {
#ifdef _DEBUG
	LPCTSTR DEF_ANY_CONNECT = _T("sinup/sinup/SDE.SiNUP200307/5151/garnet");
	LPCTSTR DEF_ADR_CONNECT = _T("sinup/sinup/SDE.SiNUP200307/5151/garnet");
	LPCTSTR DEF_MAP_CONNECT = _T("sindy/sindy/SDE.DEFAULT/5151/ruby");
#else
	LPCTSTR DEF_ANY_CONNECT = _T("sindy/sindy/SDE.DEFAULT/5151/ruby");
	LPCTSTR DEF_ADR_CONNECT = _T("sindy/sindy/SDE.DEFAULT/5151/topaz");
	LPCTSTR DEF_MAP_CONNECT = _T("sindy/sindy/SDE.DEFAULT/5151/ruby");
#endif

	LPCTSTR DEF_MESHINDEX = _T("Reference.CityMesh");
	LPCTSTR DEF_ADDR_LAYER = _T("City_Admin");

	CString gConnect = DEF_ANY_CONNECT;
	CString gAdrConnect = DEF_ADR_CONNECT;
	CString gMapConnect = DEF_MAP_CONNECT;
	CString gIndex = DEF_MESHINDEX;
	CString gAddrLayer = DEF_ADDR_LAYER;

	LPCTSTR gTargetLayer = NULL;
	CString gOldVersion;
	CString gOldAdmVersion;
	CString gOutFile;

	LPCTSTR gConfigFile = NULL;

	int gMode = 0;

	std::set<CString,std::less<CString> > gTargetLayers;
}

void PrintUsage(LPCTSTR App)
{
	LPCTSTR AppName;
	AppName = _tcsrchr(App, '\\');
	if(AppName != NULL){
		AppName++;
	}else{
		AppName = App;
	}
	_tprintf( _T("�o�[�W�����Ԃ̍������o�v���O����\n")
		    _T("Usage:%s [�I�v�V����] <��r�o�[�W����> <���C����>\n")
			_T("      %s [�I�v�V����] -C <�ݒ�t�@�C��>\n")
			_T("\t[�I�v�V����]\n")
			_T("\t  -M <�o�̓t�@�C����>:���b�V�����X�g�̂ݍ쐬(-A,-K�Ƃ̕��p�͂ł��܂���)\n")
			_T("\t  -A <�o�̓t�@�C����>:�Z�����X�g�̂ݍ쐬(--K,M�Ƃ̕��p�͂ł��܂���)\n")
			_T("\t  -K <�o�̓t�@�C����>:���b�V�����X�g�i�ƌ`�����N�p�j�̂ݍ쐬(-A,-M�Ƃ̕��p�͂ł��܂���)\n")
			_T("\t�T�[�o�A�o�[�W��������ύX����ɂ́A���ϐ��uDB_ANY�v���͐ݒ�t�@�C����\n")
			_T("\t�g�p���ĉ������B\n")
			, AppName, AppName);
}

void PrintHelp(LPCTSTR App)
{
	PrintUsage(App);
	printf("�w��o�[�W�����Ԃ̍��������쐬���܂��B\n"
		"�ڑ�����o�[�W�����i�V�j�ƁA�w��o�[�W�����i���j�Ƃ̍����ɂȂ�܂��B\n"
		"�ݒ�t�@�C���̏������́ADiffVersion.txt�������������B\n"
		);
}

bool AnalyseConfig()
{
	if(gConfigFile == NULL){
		return true;
	}
	FILE *fp;
	char Buf[256];
	bool aResult = true;
	char *p;
	if((fp = _tfopen(gConfigFile, _T("rt"))) != NULL){
		while(fgets(Buf, 256, fp)){
			if(Buf[0] != '#' && Buf[0] != '\n'){
				int aLen = strlen(Buf) -1;
				if(Buf[aLen] == '\n' || Buf[aLen] == '\r'){
					Buf[aLen] = '\0';
				}
				if(strncmp(Buf, "DB_ANY", 6) == 0){
					if((p = strchr(Buf, '\t')) != NULL){
						p++;
						gConnect = p;
					}else{
						fprintf(stderr, "�ݒ�t�@�C���̃t�H�[�}�b�g���Ⴂ�܂�:%s\n", Buf);
						aResult = false;
						break;
					}
				}else if(strncmp(Buf, "DB_ADDRESS", 10) == 0){
					if((p = strchr(Buf, '\t')) != NULL){
						p++;
						gAdrConnect = p;
					}else{
						fprintf(stderr, "�ݒ�t�@�C���̃t�H�[�}�b�g���Ⴂ�܂�:%s\n", Buf);
						aResult = false;
						break;
					}
				}else if(strncmp(Buf, "DB_MAP", 6) == 0){
					if((p = strchr(Buf, '\t')) != NULL){
						p++;
						gMapConnect = p;
					}else{
						fprintf(stderr, "�ݒ�t�@�C���̃t�H�[�}�b�g���Ⴂ�܂�:%s\n", Buf);
						aResult = false;
						break;
					}
				}else if(strncmp(Buf, "OLD_VERSION", 11) == 0){
					if((p = strchr(Buf, '\t')) != NULL){
						p++;
						gOldVersion = p;
					}else{
						fprintf(stderr, "�ݒ�t�@�C���̃t�H�[�}�b�g���Ⴂ�܂�:%s\n", Buf);
						aResult = false;
						break;
					}
				}else if(strncmp(Buf, "OLD_ADM_VERSION", 15) == 0){
					if((p = strchr(Buf, '\t')) != NULL){
						p++;
						gOldAdmVersion = p;
					}else{
						fprintf(stderr, "�ݒ�t�@�C���̃t�H�[�}�b�g���Ⴂ�܂�:%s\n", Buf);
						aResult = false;
						break;
					}
					
				}else if(strncmp(Buf, "TGT_LAYERS", 10) == 0){
					while(fgets(Buf, 256, fp)){
						if(Buf[0] != '#' && Buf[0] != '\n'){
							if(strncmp(Buf, "END", 3) != 0){
								aLen = strlen(Buf) -1;
								if(Buf[aLen] == '\n' || Buf[aLen] == '\r'){
									Buf[aLen] = '\0';
								}
								if((p = strchr(Buf, '\t')) != NULL){
									p++;
									CString aTgt = p;
									gTargetLayers.insert(aTgt);
								}else{
									fprintf(stderr, "�ݒ�t�@�C���̃t�H�[�}�b�g���Ⴂ�܂�:%s\n", Buf);
									aResult = false;
									break;
								}
							}else{
								break;
							}
						}
					}
				}else if(strncmp(Buf, "FC_MESH", 7) == 0){
					if((p = strchr(Buf, '\t')) != NULL){
						p++;
						gIndex = p;
					}else{
						fprintf(stderr, "�ݒ�t�@�C���̃t�H�[�}�b�g���Ⴂ�܂�:%s\n", Buf);
						aResult = false;
						break;
					}
				}else if(strncmp(Buf, "FC_CITY_ADMIN", 13) == 0){
					if((p = strchr(Buf, '\t')) != NULL){
						p++;
						gAddrLayer = p;
					}else{
						fprintf(stderr, "�ݒ�t�@�C���̃t�H�[�}�b�g���Ⴂ�܂�:%s\n", Buf);
						aResult = false;
						break;
					}
				}else{
					fprintf(stderr, "�ݒ�t�@�C���̃t�H�[�}�b�g���Ⴂ�܂�:%s\n", Buf);
					aResult = false;
					break;
				}
			}
		}
		fclose(fp);
	}else{
		_ftprintf(stderr, _T("�ݒ�t�@�C�����J���܂���:%s\n"), gConfigFile);
		aResult = false;
	}
	return aResult;
}

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
					case 'c':
					case 'C':
						i++;
						gConfigFile = argv[i];
						break;
					case 'a':
					case 'A':
						i++;
						if(gMode == 0){
							gMode = 2;
							gOutFile = argv[i];
						}else{
							fprintf(stderr, "-M,-K��-A�́A�����Ɏw��ł��܂���B\n");
							return false;
						}
						break;
					case 'm':
					case 'M':
						i++;
						if(gMode == 0){
							gMode = 1;
							gOutFile = argv[i];
						}else{
							fprintf(stderr, "-A,-K��-M�́A�����Ɏw��ł��܂���B\n");
							return false;
						}
						break;
					case 'k':
					case 'K':
						i++;
						if(gMode == 0){
							gMode = 3;
							gOutFile = argv[i];
						}else{
							fprintf(stderr, "-M,-M��-K�́A�����Ɏw��ł��܂���B\n");
							return false;
						}
						break;
					case '?':
						PrintHelp(argv[0]);
						return false;
						break;
					default:
						fprintf(stderr, "�s���ȃI�v�V�������w�肳��܂����B[%c]\n", argv[i][1]);
						PrintUsage(argv[0]);
						return false;
						break;
				}
			}else{
				break;
			}
			i++;
		}
		if(i == argc -2){
			gOldVersion = argv[i];
			gTargetLayer = argv[i+1];
		}else if(i == argc){
			if(gConfigFile == NULL){
				PrintUsage(argv[0]);
				return false;
			}
		}else{
			PrintUsage(argv[0]);
			return false;
		}
	}
	return AnalyseConfig();
}


bool GetEnv(void)
{
	LPCTSTR aEnv = _tgetenv(_T("DB_ANY"));
	if(aEnv != NULL){
		gConnect = aEnv;
	}
	aEnv = _tgetenv(_T("DB_MAP"));
	if(aEnv != NULL){
		gMapConnect = aEnv;
	}
	aEnv = _tgetenv(_T("DB_ADDRESS"));
	if(aEnv != NULL){
		gAdrConnect = aEnv;
	}
	aEnv = _tgetenv(_T("FC_MESH"));
	if(aEnv != NULL){
		gIndex = aEnv;
	}
	aEnv = _tgetenv(_T("FC_CITY_ADMIN"));
	if(aEnv != NULL){
		gAddrLayer = aEnv;
	}
	return true;
}

IWorkspacePtr ConnectSDE( LPCTSTR lpszConnectStr )
{
	// �ڑ�
	INamePtr ipName( sindy::create_workspace_name(lpszConnectStr) );
	if( NULL == ipName )
	{
		throw std::runtime_error((boost::format("[%s]�̍\�����ς��x��") % CT2CA(lpszConnectStr).m_psz).str());
	}

	_tprintf(_T("%s�ɐڑ���..."), lpszConnectStr);
	IUnknownPtr ipUnknown;
	ipName->Open( &ipUnknown );
	IWorkspacePtr ipWorkspace = ipUnknown;
	if( NULL == ipWorkspace )
	{
		throw std::runtime_error((boost::format("[%s]�ɐڑ��ł��Ȃ������x��") % CT2CA(lpszConnectStr).m_psz).str());
	}
	printf("�ڑ����܂���\n");
	fprintf(stderr, "#%s�ɐڑ�\n", static_cast<CT2A>(sindy::workspace_address(ipWorkspace).c_str()).m_psz);
//	std::cout << "�ڑ��� : " << static_cast<CT2A>(sindy::workspace_address(ipWorkspace).c_str()).m_psz << std::endl;
	return ipWorkspace;
}

void Execute()
{
	// �ڑ�
	IWorkspacePtr ipAdrWorkspace;
	if(gMode == 2){
		//�o�[�W�������擾����֌W�ŁA��ɐڑ�
		ipAdrWorkspace = ::ConnectSDE( gAdrConnect );
	}else if(gMode == 3){
		ipAdrWorkspace = ::ConnectSDE( gMapConnect );
	}
	IWorkspacePtr ipWorkspace = ::ConnectSDE( gConnect );
	ATLASSERT( ipWorkspace != NULL );

	VerDiff aVerDiff;
	LPCTSTR gIndexLayer;
	if(gMode == 2){
		gIndexLayer = gAddrLayer;
	}else{
		gIndexLayer = gIndex;
	}
	if(aVerDiff.init(ipWorkspace, ipAdrWorkspace, gIndexLayer, gOldVersion, gOldAdmVersion, gMode)){
		if(gTargetLayer != NULL){
			if(gMode == 2){
				if(CString(gIndexLayer).CompareNoCase(gTargetLayer) ==0){
					//�Z�����X�g�̎��A�s���E�͂��ꎩ�̂ō��������
					aVerDiff.execAddrDiff();
				}else if(CString(gTargetLayer).CompareNoCase(_T("CS_Addr_Point")) == 0 || CString(gTargetLayer).CompareNoCase(_T("KLink_Point")) == 0){
					aVerDiff.execCSDiff(gTargetLayer);
					aVerDiff.execDiff(gTargetLayer);
				}else{
					aVerDiff.execDiff(gTargetLayer);
				}
			}else{
				aVerDiff.execDiff(gTargetLayer);
			}
		}else{
			std::set<CString, std::less<CString> >::iterator aTarget;
			for(aTarget = gTargetLayers.begin(); aTarget != gTargetLayers.end(); aTarget++){
				if(CString(*aTarget).CompareNoCase(gIndexLayer) != 0){
					if(gMode == 2 && (CString(*aTarget).CompareNoCase(_T("CS_Addr_Point")) == 0 || CString(*aTarget).CompareNoCase(_T("KLink_Point")) == 0)){
						aVerDiff.execCSDiff(*aTarget);
						aVerDiff.execDiff(*aTarget);
					}else{
						aVerDiff.execDiff(*aTarget);
					}
				}else{
					if(gMode == 2){
						//�Z�����X�g�̎��A�s���E�͂��ꎩ�̂ō��������
						aVerDiff.execAddrDiff();
					}
//					fprintf(stderr, "���X�g��r�p�Ɠ����Ȃ̂Ŕ�΂��܂��B:%s\n", *aTarget);
				}
			}
		}
		if(gMode != 0){
			aVerDiff.resultOut(gOutFile);
		}
	}
}

int _tmain(int argc, _TCHAR* argv[])
{
	try {
		const arctl::coinitializer coinit;
		time_t st_time;
		time(&st_time);
		GetEnv();
		if(AnalyseArg(argc, argv)){
			// �c�[���o�[�W����
			CVersion cVer;
			_ftprintf(stderr, _T("#%s FILEVERSION %s PRODUCTVERSION %s\n"), static_cast<LPCTSTR>(cVer.GetOriginalFilename()), static_cast<LPCTSTR>(cVer.GetFileVersion()), static_cast<LPCTSTR>(cVer.GetProductVersion()));
			SYSTEMTIME  aTime;
			GetLocalTime(&aTime);
			fprintf(stderr, "#�J�n����: %04d/%02d/%02d %02d:%02d:%02d\n",
			aTime.wYear, aTime.wMonth, aTime.wDay, aTime.wHour,
			aTime.wMinute, aTime.wSecond);

			Execute();

			time_t tm;
			time(&tm);
			GetLocalTime(&aTime);
			fprintf(stderr, "#�I������: %04d/%02d/%02d %02d:%02d:%02d\n",
				aTime.wYear, aTime.wMonth, aTime.wDay, aTime.wHour,
				aTime.wMinute, aTime.wSecond);
			printf( " %6.0f�b\n", difftime( tm, st_time));
			fprintf(stderr, "#�g�[�^������%6.0f�b\n", difftime( tm, st_time));
			printf("�I�����܂�");
		}
	}
	catch(const std::exception& e) {
		printf("%s\n", e.what());
		fprintf(stderr, "#Error,%s\n", e.what());
	}
	return 0;
}



