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

// App.cpp: CApp クラスのインプリメンテーション
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "CheckAccessRoad.h"
#include "App.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// 構築/消滅
//////////////////////////////////////////////////////////////////////

CApp::CApp() : m_JobNo(0),m_pDoc(NULL)
{
	m_aMeshcodeFile = _T("");	//全国版リスト
}
CApp::~CApp()
{
	if( m_pDoc ){
		delete m_pDoc;
	}
}
//------------------------------------------------------------------------------
//	プログラム起動時パラメタの初期化
//------------------------------------------------------------------------------
bool CApp::init(int cArgc, TCHAR *cArgv[])
{
	//各種起動時パラメタの取得
	m_pDoc = new CDoc();
	if( !m_pDoc ){
		cout << "ERROR\tCApp::init()." << endl;
		return false;
	}
	CString	aConfigFile;
	aConfigFile.Format("%s",CONFIGFILE);
	if( cArgc==2 ){
		aConfigFile.Format("%s",cArgv[1]);
	}
	//起動時パラメタ取得
	if(! getenv(aConfigFile)){
		return false;
	}

	return true;
}
int CApp::selectjobnum(char aCh)
{
	switch(aCh)
	{
	case 'c':
		return -1;
	case 'q':
	case 'e':
		return 1;
	case '1':
		m_JobNo = 1;
		break;
	case '2':
		m_JobNo = 2;
		break;
	case '3':
		m_JobNo = 3;
		break;
	case '4':
		m_JobNo = 4;
		break;
	default:
		std::cout << "\t【" << aCh << "】 illegal key!!" << endl;
		return -1;
	}
	return 0;
}
//------------------------------------------------------------------------------
//	プログラムメニューを表示
//------------------------------------------------------------------------------
int CApp::showMenu(void)
{
	//プログラムメニュー表示
	char	aCh;
	int iRET = 0;
	int iRoopCount = 0;
	do {
		cout << "-------------------------------------------------------------------" << endl;
		cout << "\tSiNDYアクセス道路チェックツール" << endl;
		cout << "-------------------------------------------------------------------" << endl;
		cout << "\t【 1 】施設ポイントのShapeFile変換"	<< endl;
		cout << "\t【 2 】アクセス道路チェック"			<< endl;
		cout << "\t【 3 】アクセス道路リスト作成"		<< endl;
		cout << "\t【 4 】施設ポイント変換～アクセス道路リスト作成まで連続処理" << endl;
		cout << endl;
		cout << "select Job number '【 1～4 】'."	<< endl;
		cout << "enter 'q' to stop." << endl;
		cout << ">";
		cin >> aCh;
		//指定の値のいずれかが入力されていればループ脱出
		iRET = selectjobnum(aCh);
		if(iRET != -1)
			break;
		iRoopCount++;
		if(iRoopCount == 5)
		{
			cout << "Job Number の入力に5回失敗しました。強制終了します" << std::endl; 
			return 1;
		}
	} while( 1 );
	if(iRET == 0){
		//起動待ち
		int	aSleepTime = 0;
		cout << "起動まち時間（秒の指定）";
		cout << ">";
		cin >> aSleepTime;

		cout << "Sleep\t" << aSleepTime << "秒" << endl;
		Sleep(aSleepTime*1000);
	}
	return iRET;
}
//------------------------------------------------------------------------------
//起動時パラメタ取得
//------------------------------------------------------------------------------
bool CApp::getenv(LPCTSTR cConfigFile)
{
	CString aStrConf=cConfigFile;
	FILE *fp;
	if((fp = fopen(aStrConf, "r"))){
		cout << "-------------------------------------------------------------------" << endl;
		cout << "\tツールパラメータ" << endl;
		cout << "-------------------------------------------------------------------" << endl;

		char    aStr[256];
		char    aBuff[3][64];
		while(fgets(aStr, 256, fp) != NULL){
			aStr[strlen(aStr)-1] = '\0';
			if(aStr[0] == '#')	continue;
			aBuff[0][0] = aBuff[1][0] = aBuff[2][0] = '\0';
			sscanf( aStr,"%s%s%s",aBuff[0],aBuff[1],aBuff[2]);
			if(( 0 == strcmp("DBROAD",aBuff[0]))&&(strlen(aBuff[1]))){
				m_pDoc->setDBRoadStr(aBuff[1]);	cout << "DBROAD	= "	<< aBuff[1] << endl;
			}
			else if(( 0 == strcmp("DBPOI",aBuff[0]))&&(strlen(aBuff[1]))){
				m_pDoc->setDBPOIStr(aBuff[1]);	cout << "DBPOI	= "	<< aBuff[1] << endl;
			}
			else if(( 0 == strcmp("LOGDIR",aBuff[0]))&&(strlen(aBuff[1]))){
				if(! ::PathIsDirectory(aBuff[1])){
					cout << "ERROR\tCApp::getenv().指定ディレクトリが存在しない.\tLOGDIR		= " << aBuff[1] << endl;
					return false;
				}
				m_pDoc->setLogDirStr(aBuff[1]);		cout << "LOGDIR		= "	<< aBuff[1] << endl;
			}
			else if(( 0 == strcmp("CONTENTS_LIST",aBuff[0]))&&(strlen(aBuff[1]))){
				m_pDoc->setContentsListStr(aBuff[1]);	cout << "CONTENTS_LIST	= "	<< aBuff[1] << endl;
			}
			else if(( 0 == strcmp("LOGFILE",aBuff[0]))&&(strlen(aBuff[1]))){
				m_pDoc->setLogFileStr(aBuff[1]);	cout << "LOGFILE		= "	<< aBuff[1] << endl;
			}
			else if(( 0 == strcmp("MESHLIST",aBuff[0]))&&(strlen(aBuff[1]))){
				m_aMeshcodeFile = aBuff[1];			cout << "MESHLIST	= "	<< aBuff[1] << endl;
			}
			else if(( 0 == strcmp("DISTANCE",aBuff[0]))&&(strlen(aBuff[1]))){
				m_pDoc->setDistance(atoi(aBuff[1]));	cout << "DISTANCE	= "	<< aBuff[1] << endl;
			}
			else if(( 0 == strcmp("DEBUG",aBuff[0]))&&(strlen(aBuff[1]))){
				if( atoi(aBuff[1])){ m_pDoc->setDebugMode();
				cout << "DEBUG_FILE作成	= "	<< aBuff[1]  << endl; }
			}
			else if( 0 == strcmp("FACIL_LIST_WRITE",aBuff[0]) ){
				m_pDoc->setFacilListWrite(true);	cout << "FACIL_LIST_WRITE	= TRUE" << endl;
			}
			else if(( 0 == strcmp("JOB_NUMBER",aBuff[0]))&&(strlen(aBuff[1]))){
				m_JobNo = atol(aBuff[1]);		cout << "JOB_NUMBER	= " << aBuff[1] << endl;
			}
		}
		(void)fclose(fp);
	}
	return true;
}
//------------------------------------------------------------------------------
//	指定タスク実行エンジン
//------------------------------------------------------------------------------
int CApp::run(void)
{
	int iRET = 0;

	char	sShapeFileStr[256];
	if( m_pDoc )
	{
		CString	aStartTime;
		getTime(aStartTime);

		// Arc9文字化け対策。
		{ ICommandHostPtr ipCommandHost(CLSID_CommandHost); }

		//施設情報ポイントリストの読み込み
		sprintf(sShapeFileStr,"%s\\%s",m_pDoc->getLogDirStr(),"FacilPoint");
		if( m_pDoc->Init())
		{
			switch( m_JobNo )
			{
			case 1:	//施設ポイントのＳｈａｐｅ変換
				if( m_pDoc->getFacilPoint())
				{
					if( !m_pDoc->convertFacilPoint(sShapeFileStr))
					{
						cout << "\t施設ポイントのShape変換でエラーあり." << endl;
						iRET = 1;
					}
				}
				else
				{
					std::cerr << "施設情報ポイントの取得に失敗" << std::endl;
					iRET = 1;
				}
				break;
			case 2:	//アクセス道チェック
				if( m_pDoc->getFacilPoint())
				{
					if( !m_pDoc->convertFacilPoint(sShapeFileStr))
					{
						cout << "\t施設ポイントのShape変換でエラーあり." << endl;
						iRET = 1;
					}
					iRET = m_pDoc->checkFacilPoint();
					if (iRET == 1)
					{
						cout << "\tアクセス道チェックでエラーあり." << endl;
					}
				}
				else
				{
					std::cerr << "施設情報ポイントの取得に失敗" << std::endl;
					iRET = 1;
				}
				break;
			case 3:	//アクセス道路リスト作成
				if( !m_pDoc->makeAccessRoadList( ))
				{
					cout << "\tアクセス道路リスト作成処理でエラーあり." << endl;
					iRET = 1;
				}
				break;
			case 4:
				//施設ポイントのＳｈａｐｅ変換
				if( m_pDoc->getFacilPoint())
				{
					sprintf(sShapeFileStr,"%s\\%s",m_pDoc->getLogDirStr(),"FacilPoint");
					if( m_pDoc->convertFacilPoint(sShapeFileStr))
					{
						//アクセス道チェック
						iRET = m_pDoc->checkFacilPoint();
						if(iRET != 1)
						{
							//アクセス道チェックでエラーが見つかった際にアクセス道路リストの返り値で塗り替えられないため
							//※アクセス道チェックはチェックツールなのでエラーが見つかっても最後まで実行される。
							//アクセス道路リスト作成
							if( ! m_pDoc->makeAccessRoadList( ))
							{
								cout << "\tアクセス道路リスト作成処理ででエラーあり." << endl;
								if(iRET != 3)
									iRET = 1;
							}
						}
						else
						{
							cout << "\tアクセス道チェックでエラーあり." << endl;
							iRET = 1;
						}
					}
					else
					{
						std::cerr << "施設情報ポイントの取得に失敗" << std::endl;
						iRET = 1;
					}
				}
				else
				{
					std::cerr << "施設情報ポイントの取得に失敗" << std::endl;
					iRET = 1;
				}
				break;
			default:
				break;
			}
		}
		else
		{
			std::cerr << "SDE接続失敗" << std::endl;
			return 1;
		}
		m_pDoc->Close();
		CString	aEndTime;
		getTime(aEndTime);
		cout << "開始：" << aStartTime.GetBuffer(256) << endl;
		cout << "終了：" << aEndTime.GetBuffer(256) << endl;
	}
	return iRET;
}
/**
* 現在時刻の登録
* @note 現在時刻の登録を行います。
* @param  cStr		[in]	現在時間登録バッファ
* @return なし
*/
void CApp::getTime(CString &cStr)
{
	cStr = _T("");
	SYSTEMTIME stSystemTime;
	::GetLocalTime( &stSystemTime);
	cStr.Format( TEXT( "%04d/%02d/%02d %02d:%02d:%02d"),
		stSystemTime.wYear,	stSystemTime.wMonth, stSystemTime.wDay,
		stSystemTime.wHour,	stSystemTime.wMinute,stSystemTime.wSecond);
}
