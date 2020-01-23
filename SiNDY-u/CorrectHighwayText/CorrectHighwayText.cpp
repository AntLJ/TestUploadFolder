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
#include "CorrectHighwayText.h"
#include <ArcHelperEx/GlobalFunctions.h>
#include <ArcHelperEx/AheGlobalsCoord.h>
#include <ErrorObject2.h>
#include <RowContainer.h>
#include <TableContainer.h>
#include <TDC/useful_headers/str_util.h>
#include <TDC/useful_headers/enumerate_elements.h>
#include <TDC/useful_headers/scope_guard.h>
#include <boost/assign.hpp>

using namespace std;
using namespace sindy;

// ���s
bool CorrectHighwayText::execute()
{
	using namespace sindy::schema;
	// ������
	if (!init())
		return false;

	// �ҏW�J�n
	if (!startEdit())
	{
		RUNLOGGER.Error(_T("�ҏW�J�n�Ɏ��s���܂���"));
		RUNLOGGER.writeExitLog(false);
		return false;
	}
	// HIGHWAY_TEXT�̐������킹����
	if (!correctHwyText())
	{
		stopEdit(false);
		RUNLOGGER.writeExitLog(false);
		return false;
	}

	// �ҏW�I��
	if (!stopEdit(true))
	{
		RUNLOGGER.writeExitLog(false);
		return false;
	}

	RUNLOGGER.writeExitLog(true);
	return true;
}

// ������
bool CorrectHighwayText::init()
{
	if(!RUNLOGGER.Initialize(m_args))
	{
		cerr << "���s���O�t�@�C�����J���܂���ł���\t" << uh::toStr(m_args.m_runlog.c_str()) << endl;
		return false;
	}

	if (!m_tgtWork.Connect(m_args.m_target.c_str()))
	{
		cerr << "�����Ώېڑ�����J���܂���ł���\t" << uh::toStr(m_args.m_target.c_str()) << endl;
		RUNLOGGER.writeExitLog(false);
		return false;
	}

	return true;
}

// �ҏW�J�n
bool CorrectHighwayText::startEdit()
{
	// FGDB�EPGDB�Ȃ�
	if (!(m_tgtWork.GetNameString()->IsSDE()))
	{
		return (errorcode::sindyErrCode::sindyErr_NoErr == m_tgtWork.StartEditing(false));
	}

	// SDE�̏ꍇ
	// �o�[�W�����Ή��̐ڑ���̂�OK�Ƃ���
	{
		IVersionedWorkspacePtr ipVerWork((IWorkspace*)m_tgtWork);
		if (!ipVerWork)
		{
			RUNLOGGER.Error(_T("�ڑ��悪�o�[�W�����Ή��ł͂���܂���"));
			return false;
		}
		IVersionPtr ipVersion((IWorkspace*)m_tgtWork);
		CComBSTR bstr;
		ipVersion->get_VersionName(&bstr);
		CString strVerName = COLE2T(bstr);
		if (strVerName.Find(_T("DEFAULT")) != -1)
		{
			RUNLOGGER.Error(_T("�ڑ��悪DEFAULT�o�[�W�����ł��B"));
			return false;
		}

		if (errorcode::sindyErrCode::sindyErr_NoErr != m_tgtWork.StartEditing(false))
		{
			return false;
		}
		if (errorcode::sindyErrCode::sindyErr_NoErr != m_tgtWork.StartEditOperation())
		{
			m_tgtWork.StopEditing(false);
			return false;
		}
	}
	return true;
}

// �ҏW�I��
bool CorrectHighwayText::stopEdit(bool bSave)
{
	if (m_tgtWork.GetNameString()->IsSDE())
	{
		if (errorcode::sindyErrCode::sindyErr_NoErr != m_tgtWork.StopEditOperation())
		{
			RUNLOGGER.Error(_T("�ҏW�Z�b�V�����I���Ɏ��s���܂���"));
			bSave = false;
		}
	}
	if (!bSave)
		RUNLOGGER.Info(_T("�ҏW��j�����܂�"));

	if (errorcode::sindyErr_NoErr != m_tgtWork.StopEditing(bSave))
	{
		RUNLOGGER.Error(_T("�ҏW�I���Ɏ��s���܂���"));
		return false;
	}
	return true;
}

// �ҏW�Z�b�V������j������
bool CorrectHighwayText::abortEditOperation()
{
	if (m_tgtWork.GetNameString()->IsSDE() && errorcode::sindyErrCode::sindyErr_NoErr != m_tgtWork.AbortEditOperation())
		return false;
	return true;
}

// HIGHWAY_TEXT�̐������킹����
bool CorrectHighwayText::correctHwyText()
{
	using namespace sindy::schema;
	// HIGHWAY_TEXT���J��
	CTableFinder finder;
	if (!initTableFinder(highway_text::kTableName, finder))
	{
		return false;
	}
	CSPTableContainer spHwyT(finder.FindTable(CString(highway_text::kTableName)));

	// �s����POINT_ID�̒l���C������
	if (!correctIllegalPointID(spHwyT))
		return false;

	// IN�吶���̂��߁Amap�̃L�[��vector�Ɋi�[
	std::vector<long> pointIDs;
	for (const auto& pointID : m_illegalPointIDs)
		pointIDs.emplace_back(pointID.first);

	// �s����POINT_ID�Ɠ����l��POINT_ID�����݂��邩
	// ���݂���ꍇ�́A�X�V���s����POINT_ID�Ɋ֘A����t�B�[���h�̐���������Ȃ��Ȃ�̂ōX�V���Ȃ�
	if (existBeforeEditPointID(spHwyT, pointIDs))
		return false;

	// �s����POINT_ID�Ɠ����l�������Ă���֘A�t�B�[���h�Q���C������
	if (!correctRelPointIDFields(spHwyT, pointIDs))
		return false;

	return true;
}

// �e�[�u���t�@�C���_������
bool CorrectHighwayText::initTableFinder(const CString& table, CTableFinder& finder)
{
	CModel model;
	ITablePtr ipTable(m_tgtWork.OpenTable(table));
	if (!ipTable)
	{
		CString error = uh::str_util::format(_T("%s���J���܂���ł���"), table);
		RUNLOGGER.Error(uh::toTStr(error.GetString()));
		return false;
	}
	if (m_tgtWork.GetNameString()->IsSDE())
	{
		IVersionedObjectPtr ipVersionedObjectPtr(ipTable);
		VARIANT_BOOL isVersionRayer = VARIANT_FALSE;
		ipVersionedObjectPtr->get_IsRegisteredAsVersioned(&isVersionRayer);
		if (!isVersionRayer)
		{
			CString error = uh::str_util::format(_T("%s���o�[�W�����Ή����C���ł͂���܂���"), table);
			RUNLOGGER.Error(uh::toTStr(error.GetString()));
			return false;
		}
	}

	finder.InitCollection(ipTable, model);
	return true;
}

// �s����POINT_ID�̒l���C������
bool CorrectHighwayText::correctIllegalPointID(CSPTableContainer& spHwyTextT)
{
	using namespace sindy::schema;
	uh::scope_guard guard([&spHwyTextT] { spHwyTextT->clear(); });

	// OBJECTID��POINT_ID�̒l���قȂ�I�u�W�F�N�g�͕s���ȃf�[�^
	CString whereClause = uh::str_util::format(_T("%s<>%s"), kObjectID, highway_text::kPointID);
	spHwyTextT->clear();

	IQueryFilterPtr ipQuery(AheInitQueryFilter(nullptr, _T(""), whereClause));
	spHwyTextT->_Select(ipQuery, false, false);
	spHwyTextT->CreateCache();

	bool good = true;
	for (auto& con : *spHwyTextT)
	{
		CSPRowBase row = CAST_ROWB(CAST_ROWC(con));
		long oid = row->GetOID();
		long beforePointID = row->GetValue(highway_text::kPointID, -1L);
		if (-1 == beforePointID)
		{
			// POINT_ID�̒l���擾�ł��Ȃ���΃G���[
			CString error = uh::str_util::format(_T("%s[%ld]��%s�̒l���擾�ł��܂���ł���"), kObjectID, oid, highway_text::kPointID);
			RUNLOGGER.Error(uh::toTStr(error.GetString()));
			good = false;
			continue;
		}

		// �s����POINT_ID�Ɠ����l�������Ă���֘A�t�B�[���h�̒l���X�V���邽�߂ɁA�l���L���b�V�����Ă���
		if (!m_illegalPointIDs.insert(std::make_pair(beforePointID, oid)).second)
		{
			// ���ɓ���POINT_ID�̒l���������Ă����ꍇ�A�f�[�^�Ƃ��Ă��������̂ŃG���[
			CString error = uh::str_util::format(_T("%s[%ld]��%s[%ld]������%s�̒l�������Ă��܂�"), kObjectID, oid, kObjectID, m_illegalPointIDs[beforePointID], highway_text::kPointID);
			RUNLOGGER.Error(uh::toTStr(error.GetString()));
			good = false;
			continue;
		}
		// OBJECTID�̒l�ōX�V����B
		if (errorcode::sindyErr_NoErr != row->SetValue(highway_text::kPointID, oid))
		{
			CString error = uh::str_util::format(_T("%s[%ld]��%s�̍X�V�Ɏ��s���܂���"), kObjectID, oid, highway_text::kPointID);
			RUNLOGGER.Error(uh::toTStr(error.GetString()));
			good = false;
			continue;
		}
		CString info = uh::str_util::format(_T("%s[%ld]��%s�̒l���X�V���܂��B%ld -> %ld"),
			kObjectID, oid, highway_text::kPointID, beforePointID, oid);
		RUNLOGGER.Info(uh::toTStr(info.GetString()));

	}
	
	// �ۑ�
	if (!good || !store(spHwyTextT))
		return false;

	return true;
}

bool CorrectHighwayText::existBeforeEditPointID(CSPTableContainer& spHwyTextT, const std::vector<long>& pointIDs)
{
	using namespace sindy::schema::highway_text;
	bool isExist = false;
	std::list<long> listPIDs(pointIDs.begin(), pointIDs.end());
	spHwyTextT->clear();
	spHwyTextT->_Select(listPIDs, kPointID, true, false);
	// �������ꂽ���R�[�h�͊m�F���K�v
	for (auto& con : *spHwyTextT)
	{
		CSPRowBase row = CAST_ROWB(CAST_ROWC(con));
		long pid = row->GetValue(kPointID, -1L);
		CString error = uh::str_util::format(_T("�s����%s�̒l�Ɠ����l�������Ă��郌�R�[�h�����݂��܂��B%s[%ld]��%s[%ld]"),
			kPointID, kObjectID, row->GetOID(), kPointID, pid);
		RUNLOGGER.Error(uh::toTStr(error.GetString()));
		isExist = true;
	}
	spHwyTextT->clear();
	return isExist;
}

// �s����POINT_ID�Ɠ����l�������Ă���֘A�t�B�[���h�Q���C������
bool CorrectHighwayText::correctRelPointIDFields(CSPTableContainer& spHwyTextT, const std::vector<long>& pointIDs)
{
	using namespace sindy::schema::highway_text;
	// POINT_ID�Ɗ֘A�̂���t�B�[���h�Q
	std::vector<CString> targetFields = boost::assign::list_of
		(kNextID)
		(kPrevID)
		(kParentID)
		(kForeID1)
		(kForeID2)
		(kForeID3)
		(kForeID4)
		(kBackID1)
		(kBackID2)
		(kBackID3)
		(kBackID4)
		;

	bool good = true;
	std::list<long> listPIDs(pointIDs.begin(), pointIDs.end());
	// �t�B�[���h�P�ʂŃ��[�v
	for (const auto& field : targetFields)
	{
		// POINT_ID�Q�P�ʂŃ��[�v
		spHwyTextT->clear();
		spHwyTextT->_Select(listPIDs, field, false, false);
		for (auto& con : *spHwyTextT)
		{
			CSPRowBase row = CAST_ROWB(CAST_ROWC(con));
			// GetValue()����l��WHERE��̏����Ȃ̂ŁA�l�������Ă��邩�̃`�F�b�N�͏ȗ�
			long fieldVal = row->GetValue(field, -1L);
			// m_illegalPointIDs�̃L�[����Ɍ������Ă�̂ŁA�Ή�����l�͕K������
			long setVal = m_illegalPointIDs[fieldVal];

			// �֘A�t�B�[���h�̒l���X�V
			if (errorcode::sindyErr_NoErr != row->SetValue(field, setVal))
			{
				CString error = uh::str_util::format(_T("%s[%ld]��%s�̍X�V�Ɏ��s���܂���"), kObjectID, row->GetOID(), field);
				RUNLOGGER.Error(uh::toTStr(error.GetString()));
				good = false;
				continue;
			}
			CString info = uh::str_util::format(_T("%s[%ld]��%s�̒l���X�V���܂��B%ld -> %ld"),
				kObjectID, row->GetOID(), field, fieldVal, setVal);
			RUNLOGGER.Info(uh::toTStr(info.GetString()));
		}
		// �ۑ�
		if (!store(spHwyTextT))
			good = false;

	}

	spHwyTextT->clear();

	return good;
}

bool CorrectHighwayText::store(sindy::CSPTableContainer& spHwyTextT)
{
	// �ҏW�ۑ�
	CErrorObjectsBase errs;
	if (errorcode::sindyErr_NoErr != spHwyTextT->Store(errs))
	{
		RUNLOGGER.Error(_T("�ҏW�ۑ��Ɏ��s���܂����B"));
		return false;
	}
	return true;
}
