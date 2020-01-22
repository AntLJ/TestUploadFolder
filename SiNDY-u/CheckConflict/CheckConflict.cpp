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

#include "CheckConflict.h"
#include <boost/regex.hpp>
#include <ArcHelperEx/GlobalFunctions.h>
#include <ArcHelperEx/AheGlobalsCoord.h>
#include <GeometryBase.h>
#include <RowContainer.h>
#include <TableContainer.h>
#include <TDC/useful_headers/str_util.h>

using namespace std;
using namespace sindy;

namespace {
	const long EXECCOUNT = 50000; // �����P��
}

// ���s
bool CheckConflict::execute()
{
	// ������
	if( !init() )
		return false;

	checkConflict();
	RUNLOGGER.writeExitLog(true);
	return true;
}

// ������
bool CheckConflict::init()
{
	// ���O�t�@�C���I�[�v��
	if(!logInit())
		return false;

	bool good = true;
	if(!m_tgtWork.Connect(m_args.m_tgtdb.c_str()))
	{
		ERRLOGGER.Log(_T("#�`�F�b�N�Ώېڑ���ɐڑ��ł��܂���ł���\t%s"), m_args.m_tgtdb);
		good = false;
	}

	if(!m_diffWork.Connect(m_args.m_diffdb.c_str()))
	{
		ERRLOGGER.Log(_T("#����DB�ڑ���ɐڑ��ł��܂���ł���\t%s"), m_args.m_diffdb);
		good = false;
	}
	if(!readConfig())
		good = false;

	if(!good)
	{
		RUNLOGGER.writeExitLog(false);
		return false;
	}

	return true;
}

bool CheckConflict::logInit()
{
	if(!RUNLOGGER.Initialize(m_args.m_runlog))
	{
		cerr << "���s���O�t�@�C�����J���܂���ł���\t" << uh::toStr(m_args.m_runlog.c_str()) << endl;
		return false;
	}

	if(!ERRLOGGER.Initialize(m_args.m_errlog))
	{
		cerr << "�G���[���O�t�@�C�����J���܂���ł���\t" << uh::toStr(m_args.m_errlog.c_str()) << endl;
		RUNLOGGER.writeExitLog(false);
		return false;
	}
	return true;
}

// �`�F�b�N�R���t���N�g
void CheckConflict::checkConflict()
{
	using namespace sindy::schema;
	// �����Ώۃe�[�u���Ń��[�v
	for(const auto& table : m_tgtTables)
	{
		CTableFinder tgtFinder, diffFinder;
		// �e�[�u���t�@�C���_������
		// �e�[�u�����Ȃ��i�������Ȃ��j�ꍇ�͔�΂�
		if( !initTableFinder(m_tgtWork, table, tgtFinder) ||
			!initTableFinder(m_diffWork, table, diffFinder) )
		{
			continue;
		}

		CSPTableContainer tgtT(tgtFinder.FindTable( CString(table.c_str()) ));
		CSPTableContainer diffT(diffFinder.FindTable( CString(table.c_str()) ));
		// �K�v�ȃt�B�[���h�����݂��邩
		if( -1 == diffT->GetFieldMap()->FindField(global::ipc_diff_feature::kOrgId) ||
			-1 == diffT->GetFieldMap()->FindField(global::ipc_diff_feature::kChangeType) )
		{
			ERRLOGGER.Log(_T("#����DB��%s�ɕK�v�ȃt�B�[���h�����݂��܂���B�`�F�b�N�ł��܂���B"), uh::toTStr(table).c_str());
			continue;
		}

		// ������x�̌����ɍi���ď������s���B
		// 1��1�̍�����r�ׁ̈A���b�V����`�ōi��̂ł͂Ȃ��AOBJECTID�ōi��
		// ����DB�́AExtractDifferenceTool�ɂ���ċ@�B�I�ɍ쐬�����DB�Ȃ̂ŁAOBJECTID�͏��ԂɐU���Ă���Ƃ��ď�������B
		long maxOID = diffT->RowCount(AheInitQueryFilter()); // �����ɂ̓��R�[�h���ŁAMAX(OBJECTID)�ł͂Ȃ����A�@�B�������DB�Ȃ̂ŁA���Ȃ�

		for(long i = 0; (i*EXECCOUNT) < maxOID ; ++i)
		{
			std::list<long> listTgtOIDs; // �����Ώ�OBJECTID�Q
			getCheckRecords(i, diffT, listTgtOIDs); // diffT�͂����Ń��R�[�h���L���b�V�������
			// ��r���郌�R�[�h������Ȃ���Ύ���
			if(listTgtOIDs.empty())
				continue;

			tgtT->_Select(listTgtOIDs, kObjectID, false, false); // tgtT�̃f�[�^�L���b�V��
			checkConflict(listTgtOIDs, tgtT, diffT); // �R���t���N�g�`�F�b�N
		}
	}
}

// �ݒ�t�@�C����ǂݎ��
bool CheckConflict::readConfig()
{
	using namespace boost;
	std::ifstream ifs( uh::toStr(m_args.m_config.c_str()) );
	if( !ifs.is_open() )
	{
		ERRLOGGER.Log(_T("#�ݒ�t�@�C���̓ǂݍ��݂Ɏ��s���܂���"));
		return false;
	}

	bool good = true;
	long count = 0;
	std::string line;
	// �e�[�u�������L�ڂ��ꂽ�t�@�C���ł��邽�߁A�e�[�u�����ł͂Ȃ�����������΃G���[�Ƃ���
	regex re1( "\\W" ); // �A���t�@�x�b�g�A�����A�A���_�[�X�R�A�ȊO�̕������o�p
	regex re2( "^[a-zA-Z].*" ); // �ŏ��̕������A���t�@�x�b�g�ł��邩�����p
	while(std::getline(ifs, line))
	{
		++count;
		// ��s�A�R�����g�s���O
		if( line.empty() || line.front() == '#' )
			continue;

		if( regex_search( line, re1 ) || !regex_match( line, re2 ) )
		{
			ERRLOGGER.Log(_T("#�ݒ�t�@�C���ɖ����ȕ����񂪑��݂��܂��F%d�s��\t%s"), count, CString(line.c_str()));
			good = false;
			continue;
		}
		m_tgtTables.push_back(line);
	}
	return good;
}

// �e�[�u���t�@�C���_������
bool CheckConflict::initTableFinder(const sindy::CWorkspace& workspace, const string& table, CTableFinder& finder)
{
	uh::tstring tableName = uh::toTStr(table);
	CModel model;
	ITablePtr ipTable( workspace.OpenTable(tableName.c_str()) );
	if(!ipTable)
	{
		CString info = uh::str_util::format(_T("Table[%s] ���J���Ȃ������̂ŁA�`�F�b�N���΂��܂�"), tableName.c_str());
		RUNLOGGER.Info(info.GetString());
		return false;
	}
	finder.InitCollection( ipTable, model );
	return true;
}

void CheckConflict::getCheckRecords(const long roopCount, sindy::CSPTableContainer& table, std::list<long>& listTgtOIDs)
{
	using namespace sindy::schema;
	// CHANGE_TYPE���u4�F�X�V�O�v�̃I�u�W�F�N�g���Ώ�
	CString whereClause = uh::str_util::format(_T("%s = %d AND %s > %ld AND %s <= %ld"),
		global::ipc_diff_feature::kChangeType, changetypecode::kBeforeUpdate,
		kObjectID, roopCount * EXECCOUNT,
		kObjectID, (roopCount + 1) * EXECCOUNT); // WHERE��

	table->clear();
	IQueryFilterPtr ipQuery(AheInitQueryFilter(nullptr, _T(""), whereClause));
	table->_Select(ipQuery, false, false);
	for( const auto& con : *table )
	{
		CSPRowBase row = CAST_ROWB(CAST_ROWC(con));
		CComVariant orgOID = row->GetValue(global::ipc_diff_feature::kOrgId);
		if( VT_I4 != orgOID.vt )
		{
			ERRLOGGER.Log(_T("#����DB��%s[%s]�̒l���s���ł��BOBJECTID�F%ld"),
				table->GetTableName(), global::ipc_diff_feature::kOrgId, row->GetOID());
			continue; // �s���Ȓl�͔�΂����A����Ȓl�̓`�F�b�N����
		}
		listTgtOIDs.push_back(orgOID.lVal);
	}
}

void CheckConflict::checkConflict(const std::list<long>& listTgtOIDs, const sindy::CSPTableContainer& tgtT, const sindy::CSPTableContainer& diffT)
{
	using namespace sindy::schema;

	for( const auto& oid : listTgtOIDs )
	{
		CContainerBase::iterator itTgt = tgtT->find_by_key(kObjectID, oid); // �Ώ�DB�����R�[�h�C�e���[�^
		CContainerBase::iterator itDiff = diffT->find_by_key(global::ipc_diff_feature::kOrgId, oid); // ����DB�����R�[�h�C�e���[�^
		// �Ώ�DB�Ń��R�[�h���폜����Ă��邩
		if(itTgt == tgtT->end())
		{
			ERRLOGGER.writeError( tgtT->GetTableName(), oid, _T("#�Ώ�DB���Ń��R�[�h���폜����Ă��܂�") );
			continue;
		}
		
		CSPRowBase tgtRow = CAST_ROWB(CAST_ROWC(*itTgt));
		CSPRowBase diffRow = CAST_ROWB(CAST_ROWC(*itDiff));
		CString errorMessage;
		// �`�F�b�N�Ώۃe�[�u�������t�B�[���h��S�Ĕ�r����B
		for(const auto& field : *tgtT->GetFieldMap())
		{
			// OBJECTID�͏��O
			if(field.name.CompareNoCase(kObjectID) == 0)
				continue;

			// �t�B�[���h���Ȃ��ꍇ�iSHAPE.LEN�Ȃǂ�FGDB�ɂ͂Ȃ��̂Łj
			if(0 > diffT->GetFieldMap()->FindField(field.name))
				continue;

			// �`��t�B�[���h�̏ꍇ
			if( esriFieldTypeGeometry == field.type )
			{
				// NULL�`��͑����֌W�Ȃ����������̂ŁA��������I��
				CGeometryBase cTgtGeom(tgtRow->GetShapeCopy());
				if( !(IGeometry*)cTgtGeom )
				{
					errorMessage = _T("�Ώ�DB���̃��R�[�h��NULL�`�󂪂���܂��B");
					break;
				}
				IGeometryPtr ipDiffGeom(diffRow->GetShapeCopy());
				if( !ipDiffGeom )
				{
					errorMessage.Format(_T("����DB���̃��R�[�h��NULL�`�󂪂���܂��B����DB����%s[%ld]���m�F��������"), kObjectID, diffRow->GetOID());
					break;
				}
				if(!CGeometryBase(cTgtGeom).Equals(ipDiffGeom))
				{
					if( errorMessage.IsEmpty() )
						errorMessage = _T("�Ώ�DB���̃��R�[�h���X�V����Ă��܂��B");
					errorMessage.AppendFormat(_T("[%s]"), field.name);
				}
			}
			// �`��ȊO�̃t�B�[���h�͒l���r
			else if( tgtRow->GetValue(field.name) != diffRow->GetValue(field.name) )
			{
				if( errorMessage.IsEmpty() )
					errorMessage = _T("�Ώ�DB���̃��R�[�h���X�V����Ă��܂��B");
				errorMessage.AppendFormat(_T("[%s]"), field.name);
			}
		}

		// �G���[������Ώo��
		if( !errorMessage.IsEmpty() )
			ERRLOGGER.writeError(tgtT->GetTableName(), oid, errorMessage.GetString());

	}
}
