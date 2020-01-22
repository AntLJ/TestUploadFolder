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

// App.cpp: CApp �N���X�̃C���v�������e�[�V����
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
// �\�z/����
//////////////////////////////////////////////////////////////////////

CApp::CApp() : m_JobNo(0),m_pDoc(NULL)
{
	m_aMeshcodeFile = _T("");	//�S���Ń��X�g
}
CApp::~CApp()
{
	if( m_pDoc ){
		delete m_pDoc;
	}
}
//------------------------------------------------------------------------------
//	�v���O�����N�����p�����^�̏�����
//------------------------------------------------------------------------------
bool CApp::init(int cArgc, TCHAR *cArgv[])
{
	//�e��N�����p�����^�̎擾
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
	//�N�����p�����^�擾
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
		std::cout << "\t�y" << aCh << "�z illegal key!!" << endl;
		return -1;
	}
	return 0;
}
//------------------------------------------------------------------------------
//	�v���O�������j���[��\��
//------------------------------------------------------------------------------
int CApp::showMenu(void)
{
	//�v���O�������j���[�\��
	char	aCh;
	int iRET = 0;
	int iRoopCount = 0;
	do {
		cout << "-------------------------------------------------------------------" << endl;
		cout << "\tSiNDY�A�N�Z�X���H�`�F�b�N�c�[��" << endl;
		cout << "-------------------------------------------------------------------" << endl;
		cout << "\t�y 1 �z�{�݃|�C���g��ShapeFile�ϊ�"	<< endl;
		cout << "\t�y 2 �z�A�N�Z�X���H�`�F�b�N"			<< endl;
		cout << "\t�y 3 �z�A�N�Z�X���H���X�g�쐬"		<< endl;
		cout << "\t�y 4 �z�{�݃|�C���g�ϊ��`�A�N�Z�X���H���X�g�쐬�܂ŘA������" << endl;
		cout << endl;
		cout << "select Job number '�y 1�`4 �z'."	<< endl;
		cout << "enter 'q' to stop." << endl;
		cout << ">";
		cin >> aCh;
		//�w��̒l�̂����ꂩ�����͂���Ă���΃��[�v�E�o
		iRET = selectjobnum(aCh);
		if(iRET != -1)
			break;
		iRoopCount++;
		if(iRoopCount == 5)
		{
			cout << "Job Number �̓��͂�5�񎸔s���܂����B�����I�����܂�" << std::endl; 
			return 1;
		}
	} while( 1 );
	if(iRET == 0){
		//�N���҂�
		int	aSleepTime = 0;
		cout << "�N���܂����ԁi�b�̎w��j";
		cout << ">";
		cin >> aSleepTime;

		cout << "Sleep\t" << aSleepTime << "�b" << endl;
		Sleep(aSleepTime*1000);
	}
	return iRET;
}
//------------------------------------------------------------------------------
//�N�����p�����^�擾
//------------------------------------------------------------------------------
bool CApp::getenv(LPCTSTR cConfigFile)
{
	CString aStrConf=cConfigFile;
	FILE *fp;
	if((fp = fopen(aStrConf, "r"))){
		cout << "-------------------------------------------------------------------" << endl;
		cout << "\t�c�[���p�����[�^" << endl;
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
					cout << "ERROR\tCApp::getenv().�w��f�B���N�g�������݂��Ȃ�.\tLOGDIR		= " << aBuff[1] << endl;
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
				cout << "DEBUG_FILE�쐬	= "	<< aBuff[1]  << endl; }
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
//	�w��^�X�N���s�G���W��
//------------------------------------------------------------------------------
int CApp::run(void)
{
	int iRET = 0;

	char	sShapeFileStr[256];
	if( m_pDoc )
	{
		CString	aStartTime;
		getTime(aStartTime);

		// Arc9���������΍�B
		{ ICommandHostPtr ipCommandHost(CLSID_CommandHost); }

		//�{�ݏ��|�C���g���X�g�̓ǂݍ���
		sprintf(sShapeFileStr,"%s\\%s",m_pDoc->getLogDirStr(),"FacilPoint");
		if( m_pDoc->Init())
		{
			switch( m_JobNo )
			{
			case 1:	//�{�݃|�C���g�̂r���������ϊ�
				if( m_pDoc->getFacilPoint())
				{
					if( !m_pDoc->convertFacilPoint(sShapeFileStr))
					{
						cout << "\t�{�݃|�C���g��Shape�ϊ��ŃG���[����." << endl;
						iRET = 1;
					}
				}
				else
				{
					std::cerr << "�{�ݏ��|�C���g�̎擾�Ɏ��s" << std::endl;
					iRET = 1;
				}
				break;
			case 2:	//�A�N�Z�X���`�F�b�N
				if( m_pDoc->getFacilPoint())
				{
					if( !m_pDoc->convertFacilPoint(sShapeFileStr))
					{
						cout << "\t�{�݃|�C���g��Shape�ϊ��ŃG���[����." << endl;
						iRET = 1;
					}
					iRET = m_pDoc->checkFacilPoint();
					if (iRET == 1)
					{
						cout << "\t�A�N�Z�X���`�F�b�N�ŃG���[����." << endl;
					}
				}
				else
				{
					std::cerr << "�{�ݏ��|�C���g�̎擾�Ɏ��s" << std::endl;
					iRET = 1;
				}
				break;
			case 3:	//�A�N�Z�X���H���X�g�쐬
				if( !m_pDoc->makeAccessRoadList( ))
				{
					cout << "\t�A�N�Z�X���H���X�g�쐬�����ŃG���[����." << endl;
					iRET = 1;
				}
				break;
			case 4:
				//�{�݃|�C���g�̂r���������ϊ�
				if( m_pDoc->getFacilPoint())
				{
					sprintf(sShapeFileStr,"%s\\%s",m_pDoc->getLogDirStr(),"FacilPoint");
					if( m_pDoc->convertFacilPoint(sShapeFileStr))
					{
						//�A�N�Z�X���`�F�b�N
						iRET = m_pDoc->checkFacilPoint();
						if(iRET != 1)
						{
							//�A�N�Z�X���`�F�b�N�ŃG���[�����������ۂɃA�N�Z�X���H���X�g�̕Ԃ�l�œh��ւ����Ȃ�����
							//���A�N�Z�X���`�F�b�N�̓`�F�b�N�c�[���Ȃ̂ŃG���[���������Ă��Ō�܂Ŏ��s�����B
							//�A�N�Z�X���H���X�g�쐬
							if( ! m_pDoc->makeAccessRoadList( ))
							{
								cout << "\t�A�N�Z�X���H���X�g�쐬�����łŃG���[����." << endl;
								if(iRET != 3)
									iRET = 1;
							}
						}
						else
						{
							cout << "\t�A�N�Z�X���`�F�b�N�ŃG���[����." << endl;
							iRET = 1;
						}
					}
					else
					{
						std::cerr << "�{�ݏ��|�C���g�̎擾�Ɏ��s" << std::endl;
						iRET = 1;
					}
				}
				else
				{
					std::cerr << "�{�ݏ��|�C���g�̎擾�Ɏ��s" << std::endl;
					iRET = 1;
				}
				break;
			default:
				break;
			}
		}
		else
		{
			std::cerr << "SDE�ڑ����s" << std::endl;
			return 1;
		}
		m_pDoc->Close();
		CString	aEndTime;
		getTime(aEndTime);
		cout << "�J�n�F" << aStartTime.GetBuffer(256) << endl;
		cout << "�I���F" << aEndTime.GetBuffer(256) << endl;
	}
	return iRET;
}
/**
* ���ݎ����̓o�^
* @note ���ݎ����̓o�^���s���܂��B
* @param  cStr		[in]	���ݎ��ԓo�^�o�b�t�@
* @return �Ȃ�
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
