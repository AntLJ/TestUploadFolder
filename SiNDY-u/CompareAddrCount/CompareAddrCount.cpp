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
#include "CompareAddrCount.h"
#include <sindy/workspace.h>

void CompareAddrCount::PrintUsage()
{
	cerr << "�y�Z���f�[�^������r�c�[���z" << endl
		 << "�s�g�p���@�t" << endl
		 << "CompareAddrCount.exe [-charaddr] {-ca|-cr <�͈�>|-cl <���X�g�t�@�C��>|-cs <5���R�[�h>} -o <���O�t�@�C��>" << endl
		 << endl
		 << "�s�I�v�V�����t" << endl
		 << "\t-charaddr" << endl 
		 << "\t\t�����t���Z���f�[�^��ΏۂɃJ�E���g�����{" << endl
		 << "\t-ca" << endl
		 << "\t\t�y�f�t�H���g�z�S����ΏۂɃJ�E���g�����{" << endl
		 << "\t-cr <�J�n5���R�[�h>-<�I��5���R�[�h>" << endl
		 << "\t\t�w�肵��5���R�[�h�͈̔͂�ΏۂɃJ�E���g�����{" << endl
		 << "\t-cl <���X�g�t�@�C��>" << endl
		 << "\t\t���X�g�t�@�C���Ɋi�[����Ă���Z���R�[�h���X�g(5/8/11��)��ΏۂɃJ�E���g�����{" << endl
		 << "\t-cs <�Z���R�[�h(5/8/11��)>" << endl
		 << "\t\t�w�肵���Z���R�[�h��ΏۂɃJ�E���g�����{" << endl
		 << "\t-o <���O�t�@�C��>" << endl
		 << "\t\t�y�K�{�z�o�͐惍�O�t�@�C�����w��" << endl
		 << endl
		 << "�s���ϐ��t" << endl
		 << "\tDB_ADDRESS_OLD" << endl
		 << "\t\t�Z���nSDE�ڑ���(��)" << endl
		 << "\tDB_ADDRESS_NEW" << endl
		 << "\t\t�Z���nSDE�ڑ���(�V)" << endl
		 << "\tFC_GOU_POINT" << endl
		 << "\t\t���|�C���g�t�B�[�`���N���X��" << endl
		 << "\tFC_CITY_ADMIN" << endl
		 << "\t\t�s�s�n�}�s���E�t�B�[�`���N���X��" << endl
		 << "\tFC_CS_ADDR_POINT" << endl
		 << "\t\tCS�|�C���g�t�B�[�`���N���X��" << endl
		 << "\tJDB_CONNECT_OLD" << endl
		 << "\t\t�Z��DB�ڑ���(��)" << endl
		 << "\tJDB_CONNECT_NEW" << endl
		 << "\t\t�Z��DB�ڑ���(�V)" << endl
		 << endl;
}

bool CompareAddrCount::Init(int cArgc, _TCHAR *cArgv[])
{
	// �������Ȃ��ꍇ�͎g�p���@��\�����ďI��
	if(cArgc == 1) {
		PrintUsage();
		return false;
	}

	// �����E���ϐ��̎擾
	if(!fnAnalyzeArg(cArgc, cArgv))	return false;
	if(!fnGetEnvVar())	return false;

	// ���O�t�@�C���̃I�[�v��
	locale aLoc(locale("Japanese"), "C", locale::numeric);
	mLog.open(static_cast<CT2CW>(mLogFilename));
	if(!mLog.is_open()) {
		cerr << "#Error ���O�t�@�C�����J���܂��� : " << CT2A(mLogFilename) << endl;
		return false;
	}
	mLog.imbue(aLoc);

	// �Z��DB�ڑ�
	if(!mACHelperOld.Connect(mJDBOld)) {
		cerr << "#Error �Z��DB(��)�ɐڑ��ł��܂��� : " << CT2A(mJDBOld) << endl;
		return false;
	}
	if(!mACHelperNew.Connect(mJDBNew)) {
		cerr << "#Error �Z��DB(�V)�ɐڑ��ł��܂��� : " << CT2A(mJDBNew) << endl;
		return false;
	}

	// �eDB����t�B�[�`���N���X���擾
	IFeatureClassPtr ipGouPointOld, ipCityAdminOld, ipCSAddrPointOld;
	IFeatureClassPtr ipGouPointNew, ipCityAdminNew, ipCSAddrPointNew;
	if(!fnGetFeatureClasses(mDBOld, ipGouPointOld, ipCityAdminOld, ipCSAddrPointOld)) return false;
	if(!fnGetFeatureClasses(mDBNew, ipGouPointNew, ipCityAdminNew, ipCSAddrPointNew)) return false;

	// �J�E���g�N���X�̏�����
	if(!mAddrCountOld.Init(mCharF, mTarget, mTgtOpt, ipGouPointOld, ipCityAdminOld, ipCSAddrPointOld)) return false;
	if(!mAddrCountNew.Init(mCharF, mTarget, mTgtOpt, ipGouPointNew, ipCityAdminNew, ipCSAddrPointNew)) return false;

	return true;
}

bool CompareAddrCount::Execute()
{
	cerr << "��DB�ł̃J�E���g���J�n" << endl;
	if(!mAddrCountOld.Count()) return false;

	cerr << "�VDB�ł̃J�E���g���J�n" << endl;
	if(!mAddrCountNew.Count()) return false;

	// ���O�ւ̌��ʏo��
	for(AddrCount::const_iterator itrOld = mAddrCountOld.begin(), itrNew = mAddrCountNew.begin();
		itrOld != mAddrCountOld.end() || itrNew != mAddrCountNew.end(); )
	{
		CString aShiko, aHaishi;

		if(itrOld != mAddrCountOld.end() && (itrNew == mAddrCountNew.end() || (*itrOld).first < (*itrNew).first))
		{
			// �u���v���R�[�h�̂ݏo��
			fnGetShikoHaishiDate((*itrOld).first, aShiko, aHaishi);
			mLog << CT2A((*itrOld).first) << "\t" << CT2A(mACHelperOld.GetAddrKanji((*itrOld).first)) << "\t" << (*itrOld).second << "\t0\t" << -(*itrOld).second << "\t"
				<< CT2A(aShiko) << "\t" << CT2A(aHaishi) << "\t0���ɕω�" << endl;
			++itrOld;
		}
		else if(itrNew != mAddrCountNew.end() && (itrOld == mAddrCountOld.end() || (*itrOld).first > (*itrNew).first))
		{
			// �u�V�v���R�[�h�̂ݏo��
			fnGetShikoHaishiDate((*itrNew).first, aShiko, aHaishi);
			mLog << CT2A((*itrNew).first) << "\t" << CT2A(mACHelperNew.GetAddrKanji((*itrNew).first))<< "\t0\t" << (*itrNew).second << "\t" << (*itrNew).second << "\t"
				<< CT2A(aShiko) << "\t" << CT2A(aHaishi) << "\t" << endl;
			++itrNew;
		}
		else
		{
			// �����R�[�h���o��
			fnGetShikoHaishiDate((*itrNew).first, aShiko, aHaishi);
			int aDiff = (*itrNew).second - (*itrOld).second;
			mLog << CT2A((*itrOld).first) << "\t" << CT2A(mACHelperNew.GetAddrKanji((*itrNew).first)) << "\t" << (*itrOld).second << "\t" << (*itrNew).second << "\t" << aDiff << "\t"
				<< CT2A(aShiko) << "\t" << CT2A(aHaishi) << "\t";
			if((*itrOld).second != 0 && (*itrNew).second == 0) {
				mLog << "0���ɕω�" << endl;
			}
			else if((*itrOld).second >= 2 && (*itrOld).second / 2.0 >= (*itrNew).second) {
				mLog << "�����ȉ��ɕω�" << endl;
			}
			else if(aDiff <= -20) {
				mLog << "20���ȏ㌸��" << endl;
			}
			else {
				mLog << endl;
			}
			++itrOld;
			++itrNew;
		}
	}

	mLog.close();

	return true;
}

bool CompareAddrCount::fnAnalyzeArg(int cArgc, _TCHAR *cArgv[])
{
	for(int i = 1; i < cArgc; ++i) {
		CString aTmp1(cArgv[i]);
		if(aTmp1.Left(1) != _T("-")) {
			cerr << "#Error �������s���ł�" << endl;
			return false;
		}

		if(aTmp1 == _T("-charaddr")) {
			mCharF = true;
			continue;
		}
		if(aTmp1 == "-ca") {
			mTarget = kTgtAll;
			continue;
		}

		if(i + 1 < cArgc) {
			CString aTmp2(cArgv[++i]);
			if(aTmp1 == "-cr") {
				mTarget = kTgtRange;
				mTgtOpt = aTmp2;
				if(mTgtOpt.GetLength() != 11 || mTgtOpt.Find(_T('-')) != 5) {
					cerr << "#Error �͈͎w�肪�s���ł� : " << CT2A(mTgtOpt) << endl;
					return false;
				}
				continue;
			}
			if(aTmp1 == "-cl") {
				mTarget = kTgtList;
				mTgtOpt = aTmp2;
				// �����Ńt�@�C���̑��݃`�F�b�N������H
				continue;
			}
			if(aTmp1 == "-cs") {
				mTarget = kTgtSingle;
				mTgtOpt = aTmp2;
				if(mTgtOpt.GetLength() != 5 && mTgtOpt.GetLength() != 8 && mTgtOpt.GetLength() != 11) {
					cerr << "#Error �Z���R�[�h�w�肪�s���ł�(5/8/11���̂݉�) : " << CT2A(mTgtOpt) << endl;
					return false;
				}
				continue;
			}
			if(aTmp1 == "-o") {
				mLogFilename = aTmp2;
				continue;
			}
		}

		cerr << "#Error �������s���ł�" << endl;
		return false;
	}

	return true;
}

bool CompareAddrCount::fnGetEnvVar()
{
	if(!mDBOld.GetEnvironmentVariable(_T("DB_ADDRESS_OLD"))) {
		cerr << "#Error ���ϐ� DB_ADDRESS_OLD �̒l���擾�ł��܂���" << endl;
		return false;
	}

	if(!mDBNew.GetEnvironmentVariable(_T("DB_ADDRESS_NEW"))) {
		cerr << "#Error ���ϐ� DB_ADDRESS_NEW �̒l���擾�ł��܂���" << endl;
		return false;
	}

	if(!mFCGouPoint.GetEnvironmentVariable(_T("FC_GOU_POINT"))) {
		cerr << "#Error ���ϐ� FC_GOU_POINT �̒l���擾�ł��܂���" << endl;
		return false;
	}

	if(!mFCCityAdmin.GetEnvironmentVariable(_T("FC_CITY_ADMIN"))) {
		cerr << "#Error ���ϐ� FC_CITY_ADMIN �̒l���擾�ł��܂���" << endl;
		return false;
	}

	if(!mFCCSAddrPoint.GetEnvironmentVariable(_T("FC_CS_ADDR_POINT"))) {
		cerr << "#Error ���ϐ� FC_CS_ADDR_POINT �̒l���擾�ł��܂���" << endl;
		return false;
	}

	if(!mJDBOld.GetEnvironmentVariable(_T("JDB_CONNECT_OLD"))) {
		cerr << "#Error ���ϐ� JDB_CONNECT_OLD �̒l���擾�ł��܂���" << endl;
		return false;
	}

	if(!mJDBNew.GetEnvironmentVariable(_T("JDB_CONNECT_NEW"))) {
		cerr << "#Error ���ϐ� JDB_CONNECT_NEW �̒l���擾�ł��܂���" << endl;
		return false;
	}

	return true;
}

bool CompareAddrCount::fnGetFeatureClasses(const CString& cDB, IFeatureClassPtr& cGouPoint, IFeatureClassPtr& cCityAdmin, IFeatureClassPtr& cCSAddrPoint)
{
	IWorkspacePtr ipWorkspace;
	INamePtr ipName = sindy::create_workspace_name(cDB);
	if(ipName) {
		IUnknownPtr ipUnknown;
		ipName->Open(&ipUnknown);
		ipWorkspace = ipUnknown;
	}

	if(!ipWorkspace) {
		cerr << "#Error SDE�ڑ��Ɏ��s : " << CT2A(cDB) << endl;
		return false;
	}
	cout << "SDE�ڑ� : " << CT2A(cDB) << endl;

	IFeatureWorkspacePtr(ipWorkspace)->OpenFeatureClass(CComBSTR(mFCGouPoint), &cGouPoint);
	if(!cGouPoint) {
		cerr << "#Error ���|�C���g�t�B�[�`���N���X�̎擾�Ɏ��s : " << CT2A(mFCGouPoint) << endl;
		return false;
	}

	IFeatureWorkspacePtr(ipWorkspace)->OpenFeatureClass(CComBSTR(mFCCityAdmin), &cCityAdmin);
	if(!cCityAdmin) {
		cerr << "#Error �s�s�n�}�s���E�t�B�[�`���N���X�̎擾�Ɏ��s : " << CT2A(mFCCityAdmin) << endl;
		return false;
	}

	IFeatureWorkspacePtr(ipWorkspace)->OpenFeatureClass(CComBSTR(mFCCSAddrPoint), &cCSAddrPoint);
	if(!cCSAddrPoint) {
		cerr << "#Error �s�s�n�}�s���E�t�B�[�`���N���X�̎擾�Ɏ��s : " << CT2A(mFCCSAddrPoint) << endl;
		return false;
	}

	return true;
}

bool CompareAddrCount::fnGetShikoHaishiDate(const CString& cAddrCode, CString& cShikoDate, CString& cHaishiDate)
{
	CString aSQL;
	aSQL.Format(_T("select Shiko_Date, Haishi_Date from Areacode_Master where Ken_Code = '%s' and Shi_Code = '%s' and Oaza_Code = '%s' and Aza_Code = '%s'"),
		cAddrCode.Left(2), cAddrCode.Mid(2, 3), cAddrCode.Mid(5, 3), cAddrCode.Mid(8, 3));

	CJusyoRecordSet aRecSet;
	if(mACHelperNew.Select(aSQL, aRecSet) && aRecSet.GetRecordCount() == 1) {
		cShikoDate	= aRecSet.GetStringValue(0L);
		cHaishiDate	= aRecSet.GetStringValue(1L);
	} else {
//		cerr << "#Notice �{�s�N���E�p�~�N���𐳂����擾�ł��܂��� : " << CT2A(cAddrCode) << endl;
		return false;
	}

	return true;
}
