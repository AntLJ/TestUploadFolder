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

#include "stdafx.h"
#include "Param.h"
#include "LogSys.h"
#include "DBUtils.h"
#include "Addr2TsvBase.h"
#include "FileEUC.h"
#include "FileUTF8.h"
#include <sindy/libschema.h>
#include "Adrp.h"
#include "AdrpML.h"
#include "Zipp.h"
#include "Master.h"
#include "ConvAddrPoint.h"
#include "FileSJIS.h"

CConvAddrPoint::CConvAddrPoint(CParam& cParam) :m_cParam(cParam)
{
}


CConvAddrPoint::~CConvAddrPoint(void)
{
}

bool CConvAddrPoint::init(void)
{
	// �Z��DB�ڑ�
	CString dbAddr(m_cParam.getAddrDB().c_str());
	if (!CDBUtils::initDB(dbAddr, m_ipWorkspaceAddr))
	{
		CLogSys::GetInstance().PrintFreeStyleLog(err_level::err, _T(""), D_LONG_NULL, err_code::errConnectDB, _T("DB�ڑ����s [%s]"), dbAddr);
		return false;
	}
	CLogSys::GetInstance().WriteRunLog(true, false, _T("�Z��DB�ڑ� ... OK\n"));

	// ������DB�ڑ�
	CString dbMlang(m_cParam.getMlangDB().c_str());
	if (!dbMlang.IsEmpty()) {
		if (!CDBUtils::initDB(dbMlang, m_ipWorkspaceMlang))
		{
			CLogSys::GetInstance().PrintFreeStyleLog(err_level::err, _T(""), D_LONG_NULL, err_code::errConnectDB, _T("DB�ڑ����s [%s]"), dbMlang);
			return false;
		}
		CLogSys::GetInstance().WriteRunLog(true, false, _T("������DB�ڑ� ... OK\n"));
	}

	return true;
}

bool CConvAddrPoint::execute(void)
{
	bool bResult = true;
	// �����^�C�v�ŕ���
	switch (m_cParam.getEType())
	{
	case outtype::adr_authdb:
		bResult = adr_authdb();
		break;
	case outtype::pos_authdb:
		bResult = pos_authdb();
		break;
	case outtype::adrmst_authdb:
		bResult = adrmst_authdb();
		break;
	default:
		break;
	}

	return bResult;
}

bool CConvAddrPoint::adr_authdb(void)
{
	CLogSys::GetInstance().WriteRunLog(true, false, _T("�ҏW�pDB����-�Z���|�C���g�ϊ�\n"));

	CAdrp001 adrp001(m_cParam, m_ipWorkspaceAddr);
	CAdrp002 adrp002(m_cParam, m_ipWorkspaceAddr);
	CAdrp003 adrp003(m_cParam, m_ipWorkspaceAddr);
	CAdrp004 adrp004(m_cParam, m_ipWorkspaceAddr);
	if (!adrp001.init() || !adrp001.execute())
		return false;
	if (!adrp002.init() || !adrp002.execute())
		return false;
	if (!adrp003.init() || !adrp003.execute())
		return false;
	if (!adrp004.init() || !adrp004.execute())
		return false;

	// 5���R�[�h���X�g�擾
	std::set<std::pair<CString, CString>> codeList;
	codeList = adrp002.getCodeList();

	CAdrp005 adrp005(m_cParam, m_ipWorkspaceAddr, codeList);
	CAdrp006 adrp006(m_cParam, m_ipWorkspaceAddr, codeList);
	if (!adrp005.init() || !adrp005.execute())
		return false;
	if (!adrp006.init() || !adrp006.execute())
		return false;

	if (m_ipWorkspaceMlang)
	{
		CAdrp001ML adrp001ML(m_cParam, m_ipWorkspaceMlang);
		CAdrp002ML adrp002ML(m_cParam, m_ipWorkspaceMlang);
		CAdrp003ML adrp003ML(m_cParam, m_ipWorkspaceMlang);
		CAdrp004ML adrp004ML(m_cParam, m_ipWorkspaceMlang);
		CAdrp005ML adrp005ML(m_cParam, m_ipWorkspaceMlang);
		CAdrp006ML adrp006ML(m_cParam, m_ipWorkspaceMlang);
		if (!adrp001ML.init() || !adrp001ML.execute())
			return false;
		if (!adrp002ML.init() || !adrp002ML.execute())
			return false;
		if (!adrp003ML.init() || !adrp003ML.execute())
			return false;
		if (!adrp004ML.init() || !adrp004ML.execute())
			return false;
		if (!adrp005ML.init() || !adrp005ML.execute())
			return false;
		if (!adrp006ML.init() || !adrp006ML.execute())
			return false;
	}

	// �����\�o��
	if (!make_count_auth_adr(adrp001, adrp002, adrp003, adrp004, adrp005, adrp006))
		return false;

	return true;
}

bool CConvAddrPoint::make_count_auth_adr(const CAdrp001& adrp001, const CAdrp002& adrp002, const CAdrp003& adrp003, const CAdrp004& adrp004, const CAdrp005& adrp005, const CAdrp006& adrp006)
{
	std::cout << "  �Z�������\�o��\r";

	// �o�̓t�@�C���I�[�v��
	CFileSJIS cFile;
	std::ofstream ofs;
	if (! cFile.open(CString(m_cParam.getOutDir().c_str()), C_COUNT_AUTH_ADR_FILE, ofs))
		return false;

	// �w�b�_���o��
	ofs << "�s���{��		�s�撬��		���E�厚	���ځE��	�Ԓn			��		" << std::endl;
	ofs << "�R�[�h	����	�R�[�h	����			�Ԓn	�����t���Ԓn	�Ԓn�̂����x���t	��	�����t����	���̂����}�ԕt" << std::endl;

	// �����\�p�R�[�h���̑Ή����X�g�擾
	std::map<CString, CString> codeNameList001 = adrp001.getCodeNameList();
	std::map<std::pair<CString, CString>, CString> codeNameList002 = adrp002.getCodeNameList();
	// �����\�p���X�g�擾
	std::map<std::pair<CString, CString>, long> codeCountList003 = adrp003.getCodeCountList();
	std::map<std::pair<CString, CString>, long> codeCountList004 = adrp004.getCodeCountList();
	std::map<std::pair<CString, CString>, std::vector<long>> codeCountList005 = adrp005.getCodeCountList();
	std::map<std::pair<CString, CString>, std::vector<long>> codeCountList006 = adrp006.getCodeCountList();

	// �s���{���R�[�h(2��)�A�s�撬���R�[�h(3��)��5���R�[�hmap�ŌJ��Ԃ�
	for (const auto& it002 : codeNameList002)
	{
		CString strPrefCode = (it002.first).first;
		CString strCityCode = (it002.first).second;
		auto it001 = codeNameList001.find(strPrefCode);
		auto it003 = codeCountList003.find(std::make_pair(strPrefCode, strCityCode));
		auto it004 = codeCountList004.find(std::make_pair(strPrefCode, strCityCode));
		auto it005 = codeCountList005.find(std::make_pair(strPrefCode, strCityCode));
		auto it006 = codeCountList006.find(std::make_pair(strPrefCode, strCityCode));

		ofs << CT2A(strPrefCode) << "\t"												// �s���{���R�[�h
			<< (it001 != codeNameList001.end() ? CT2A(it001->second) : "") << "\t"		// �s���{������
			<< CT2A(strCityCode) << "\t"												// �s�撬���R�[�h
			<< CT2A(it002.second) << "\t"												// �s�撬������
			<< (it003 != codeCountList003.end() ? it003->second : 0) << "\t"			// ���E�厚�̃��R�[�h����
			<< (it004 != codeCountList004.end() ? it004->second : 0) << "\t"			// ���ځE���̃��R�[�h����
			<< (it005 != codeCountList005.end() ? (it005->second)[0] : 0) << "\t"		// �Ԓn�̃��R�[�h����
			<< (it005 != codeCountList005.end() ? (it005->second)[1] : 0) << "\t"		// �����t���Ԓn�̃��R�[�h����
			<< (it005 != codeCountList005.end() ? (it005->second)[2] : 0) << "\t"		// �Ԓn�̂����x���t�̃��R�[�h����
			<< (it006 != codeCountList006.end() ? (it006->second)[0] : 0) << "\t"		// ���̃��R�[�h����
			<< (it006 != codeCountList006.end() ? (it006->second)[1] : 0) << "\t"		// �����t�����̃��R�[�h����
			<< (it006 != codeCountList006.end() ? (it006->second)[2] : 0) << std::endl;	// ���̂����}�ԕt�̃��R�[�h����
	}

	// �f�[�^���擪�s�A�f�[�^���ŏI�s�A���v�s
	long firstLine = 3;
	long lastLine = 2 + codeNameList002.size();
	long totalLine = lastLine + 1;

	// ���v�s�o��
	ofs << boost::str(boost::format("total				\"=SUM(E%1%:E%2%)\"	\"=SUM(F%1%:F%2%)\"	\"=SUM(G%1%:G%2%)\"	\"=SUM(H%1%:H%2%)\"	\"=SUM(I%1%:I%2%)\"	\"=SUM(J%1%:J%2%)\"	\"=SUM(K%1%:K%2%)\"	\"=SUM(L%1%:L%2%)\"") % firstLine % lastLine).c_str() << std::endl;
	ofs << std::endl;
	ofs << std::endl;

	// �t�b�^���o��
	ofs << boost::str(boost::format("									����	�ʏ�Z��	\"�ʏ�Z��\n�{�����t���Z��\"")).c_str() << std::endl;
	ofs << boost::str(boost::format("									�s���{��	%1%	%1%") % codeNameList001.size()).c_str() << std::endl;
	ofs << boost::str(boost::format("									�s�撬��	%1%	%1%") % codeNameList002.size()).c_str() << std::endl;
	ofs << boost::str(boost::format("									���E�厚	=E%1%	=E%1%") % totalLine).c_str() << std::endl;
	ofs << boost::str(boost::format("									���ځE��	=F%1%	=F%1%") % totalLine).c_str() << std::endl;
	ofs << boost::str(boost::format("									�Ԓn	=G%1%	=G%1%") % totalLine).c_str() << std::endl;
	ofs << boost::str(boost::format("									�Ԓn(�����t��)	-	=H%1%") % totalLine).c_str() << std::endl;
	ofs << boost::str(boost::format("									  �Ԓn�̂����x���t	-	=I%1%") % totalLine).c_str() << std::endl;
	ofs << boost::str(boost::format("									��	=J%1%	=J%1%") % totalLine).c_str() << std::endl;
	ofs << boost::str(boost::format("									��(�����t��)	-	=K%1%") % totalLine).c_str() << std::endl;
	ofs << boost::str(boost::format("									  ���̂����}�ԕt	-	=L%1%") % totalLine).c_str() << std::endl;
	ofs << boost::str(boost::format("									total	\"=SUM(K%1%:K%2%)\"	\"=SUM(L%1%:L%3%)+SUM(L%4%:L%5%)\"") % (lastLine + 5) % (lastLine + 14) % (lastLine + 10) % (lastLine + 12) % (lastLine + 13)).c_str() << std::endl;

	CLogSys::GetInstance().WriteRunLog(true, false, _T("  �Z�������\�o�� ... OK\n"));
	return true;
}

bool CConvAddrPoint::pos_authdb(void)
{
	CLogSys::GetInstance().WriteRunLog(true, false, _T("�ҏW�pDB����-�X�֔ԍ��|�C���g�ϊ�\n"));

	CZipp001 zipp001(m_cParam, m_ipWorkspaceAddr);
	if (!zipp001.init() || !zipp001.execute())
		return false;

	// �����\�o��
	if (!make_count_auth_pos(zipp001))
		return false;

	return true;
}

bool CConvAddrPoint::make_count_auth_pos(const CZipp001& zipp001)
{
	std::cout << "  �X�֔ԍ������\�o��\r";

	// �o�̓t�@�C���I�[�v��
	CFileSJIS cFile;
	std::ofstream ofs;
	if (! cFile.open(CString(m_cParam.getOutDir().c_str()), C_COUNT_AUTH_POS_FILE, ofs))
		return false;

	// �w�b�_���o��
	ofs << "�X�֔ԍ����	������	���j�[�N����" << std::endl;

	// �����\�p���X�g�擾
	std::map<long, std::vector<long>> codeCountList = zipp001.getCodeCountList();

	// �X�֔ԍ���ʖ��̏o��
	std::map<long, std::vector<long>>::const_iterator it;
	it = codeCountList.find(sindy::schema::zip_point::zip_att::kAddress);
	ofs << "�Z��" << "\t"
		<< (it != codeCountList.end() ? it->second[0] : 0) << "\t"
		<< (it != codeCountList.end() ? it->second[1] : 0) << std::endl;

	it = codeCountList.find(sindy::schema::zip_point::zip_att::kBuilding);
	ofs << "�r��" << "\t"
		<< (it != codeCountList.end() ? it->second[0] : 0) << "\t"
		<< (it != codeCountList.end() ? it->second[1] : 0) << std::endl;

	it = codeCountList.find(sindy::schema::zip_point::zip_att::kOffice);
	ofs << "������Ə�" << "\t"
		<< (it != codeCountList.end() ? it->second[0] : 0) << "\t"
		<< (it != codeCountList.end() ? it->second[1] : 0) << std::endl;

	it = codeCountList.find(sindy::schema::zip_point::zip_att::kPostOfficeBox);
	ofs << "������" << "\t"
		<< (it != codeCountList.end() ? it->second[0] : 0) << "\t"
		<< (it != codeCountList.end() ? it->second[1] : 0) << std::endl;

	// ���v�s�o��
	ofs << "���v" << "\t" << "\"=SUM(B2:B5)\"" << "\t" << "\"=SUM(C2:C5)\"" << std::endl;

	CLogSys::GetInstance().WriteRunLog(true, false, _T("  �X�֔ԍ������\�o�� ... OK\n"));
	return true;
}

bool CConvAddrPoint::adrmst_authdb(void)
{
	CLogSys::GetInstance().WriteRunLog(true, false, _T("�ҏW�pDB����-�Z���n�}�X�^�ϊ�\n"));

	CMstAdm mstadm(m_cParam, m_ipWorkspaceAddr);
	CMstPfx mstpfx(m_cParam, m_ipWorkspaceAddr);
	CMstSfx mstsfx(m_cParam, m_ipWorkspaceAddr);
	if (!mstadm.init() || !mstadm.execute())
		return false;
	if (!mstpfx.init() || !mstpfx.execute())
		return false;
	if (!mstsfx.init() || !mstsfx.execute())
		return false;

	CCodeDomain2Tsv codeDomain2Tsv(m_cParam, m_ipWorkspaceAddr);
	if (!codeDomain2Tsv.execute(C_MST_CODEDOMAIN_NMA, C_MST_FILE_NMA))
		return false;
	if (!codeDomain2Tsv.execute(C_MST_CODEDOMAIN_RAA, C_MST_FILE_RAA))
		return false;
	if (!codeDomain2Tsv.execute(C_MST_CODEDOMAIN_CBA, C_MST_FILE_CBA))
		return false;
	if (!codeDomain2Tsv.execute(C_MST_CODEDOMAIN_GMA, C_MST_FILE_GMA))
		return false;
	if (!codeDomain2Tsv.execute(C_MST_CODEDOMAIN_CST, C_MST_FILE_CST))
		return false;
	if (!codeDomain2Tsv.execute(C_MST_CODEDOMAIN_ZPA, C_MST_FILE_ZPA))
		return false;

	return true;
}