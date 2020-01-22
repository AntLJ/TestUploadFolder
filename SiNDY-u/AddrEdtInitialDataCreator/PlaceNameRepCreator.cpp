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
#include "PlaceNameRepCreator.h"
#include "Logger.h"

#include <sindy/workspace.h>
#include <TDC/useful_headers/text_util.h>

using namespace sindy;
using namespace sindy::schema;
using namespace Utility;

PlaceNameRepCreator::PlaceNameRepCreator(const Arguments& arg) : BaseDataCreator(arg) {}

bool PlaceNameRepCreator::init()
{
	bool ret = true;

	if( !BaseDataCreator::initBase() )
		return false;

	// ���s���O�փI�v�V��������������
	RUNLOGGER.Log(uh::str_util::format(_T("-%s\t%s\n"), 
		(CString)arg::k_imp_type, m_args.m_impTypeStr), false);
	RUNLOGGER.Log(uh::str_util::format(_T("-%s\t%s\n"),
		(CString)arg::k_edt_addr_db, m_args.m_edtAddrDb), false);
	RUNLOGGER.Log(uh::str_util::format(_T("-%s\t%s\n"), 
		(CString)arg::k_old_addr_db, m_args.m_oldAddrDb), false);
	RUNLOGGER.Log(uh::str_util::format(_T("-%s\t%s\n"),
		(CString)arg::k_run_log, m_args.m_runLog), false);
	RUNLOGGER.Log(_T("\n"), false);

	m_edtWorkspace = create_workspace(m_args.m_edtAddrDb);
	if (!m_edtWorkspace)
	{
		writeMessage(
				uh::str_util::format(_T("SiNDY�i�Z���E�ҏW�j�ւ̐ڑ��Ɏ��s�F%s\n"),
				m_args.m_edtAddrDb ),
				RunLogMsgStatus::Error,
				true);
		ret = false;
	}

	auto oldAddrWorkspace = create_workspace(m_args.m_oldAddrDb);
	if (!oldAddrWorkspace)
	{
		writeMessage(
				uh::str_util::format(_T("SiNDY�i���Z���E�ҏW�j�ւ̐ڑ��Ɏ��s�F%s\n"),
				m_args.m_oldAddrDb ),
				RunLogMsgStatus::Error,
				true);
		ret = false;
	}

	if( !ret )
	{
		writeMessage(uh::str_util::format(_T("�G���[�I��\n")), Utility::RunLogMsgStatus::Error, false);
		return ret;
	}

	m_edtLayerInfos.emplace(placename_rep_point::kTableName, true);
	if( !m_edtLayerInfos[placename_rep_point::kTableName].
				setFeatureClassInfo(m_edtWorkspace, placename_rep_point::kTableName) )
	{
		writeMessage(
				uh::str_util::format(_T("SiNDY�i�Z���E�ҏW�j��%s�t�B�[�`���N���X�擾�Ɏ��s\n"),
				placename_rep_point::kTableName ),
				RunLogMsgStatus::Error,
				true);
		ret = false;
	}

	m_baseLayerInfos.emplace(base_rep_point::kTableName, true);
	if( !m_baseLayerInfos[base_rep_point::kTableName].
				setFeatureClassInfo(oldAddrWorkspace, base_rep_point::kTableName) )
	{
		writeMessage(
				uh::str_util::format(_T("SiNDY�i���Z���E�ҏW�j��%s�t�B�[�`���N���X�擾�Ɏ��s\n"),
				base_rep_point::kTableName ),
				RunLogMsgStatus::Error,
				true);
		ret = false;
	}

	if( !ret )
		writeMessage(uh::str_util::format(_T("�G���[�I��\n")), Utility::RunLogMsgStatus::Error, false);

	return ret;
}

bool PlaceNameRepCreator::create()
{
	bool ret = true;

	CountInfo placeNameCntInfos(placename_rep_point::kTableName);

	// ���kADDR�擾
	writeMessage(
			_T("SiNDY�i���Z���E�ҏW�j���C������f�[�^�擾\n"), RunLogMsgStatus::Info, true);

	auto baseRepLayer = m_baseLayerInfos[base_rep_point::kTableName];
	IFeatureCursorPtr baseRepCursor;
	if (FAILED(baseRepLayer.getFeatureClass()->Search(nullptr, VARIANT_FALSE, &baseRepCursor)))
	{
		writeMessage(
				uh::str_util::format(_T("SiNDY(���Z���F%s�j�̌����Ɏ��s\n"),
				base_rep_point::kTableName ),
				RunLogMsgStatus::Error,
				true);
		return false;
	}

	IFeatureCursorPtr insertCursor;
	if( FAILED(m_edtLayerInfos[placename_rep_point::kTableName].getFeatureClass()->Insert(VARIANT_TRUE, &insertCursor)) )
	{
		writeErrMsg( placename_rep_point::kTableName, 0L,
					_T("ERROR"), _T("Insert�Ɏ��s"), _T(""));
		return false;
	}

	IFeaturePtr baseRepFeature;
	auto& baseRepFieldMap = baseRepLayer.getFieldMap();

	IFeatureBufferPtr featureBuf;
	if( FAILED(m_edtLayerInfos[placename_rep_point::kTableName].getFeatureClass()->CreateFeatureBuffer(&featureBuf)) )
	{
		writeErrMsg( placename_rep_point::kTableName, 0L,
					_T("ERROR"), _T("CreateFeatureBuffer�Ɏ��s"), _T(""));
		return false;
	}

	auto fieldMap = m_edtLayerInfos[placename_rep_point::kTableName].getFieldMap();

	writeMessage(_T("�C���|�[�g����\n"), RunLogMsgStatus::Info, true);

	// �ҏW�J�n
	if( !startEdit() )
	{
		writeMessage(_T("�ҏW�J�n�Ɏ��s\n"), RunLogMsgStatus::Error, true);
		return false;
	}

	while (S_OK == baseRepCursor->NextFeature(&baseRepFeature) && baseRepFeature)
	{
		++(placeNameCntInfos.totalCount);

		CComVariant addrcode, adminName, zipcode;
		long baseRepOid = baseRepFeature->GetOID();
		if( !getValue(baseRepFeature, baseRepFieldMap.FindField(base_rep_point::kAddrCode), addrcode) ||
			!getValue(baseRepFeature, baseRepFieldMap.FindField(base_rep_point::kAdminName), adminName) ||
			!getValue(baseRepFeature, baseRepFieldMap.FindField(base_rep_point::kZipCode), zipcode) )
		{
			writeErrMsg( base_rep_point::kTableName, baseRepOid,
						_T("ERROR"), _T("Feature�����get�Ɏ��s"), _T(""));
			ret = false;
			continue;
		}

		// ���ʑ����l�ݒ�
		for (const auto& attr : m_commonAttrMap)
			if( !putValue(featureBuf, fieldMap.FindField(attr.first), attr.second) )
			{
				writeErrMsg( base_rep_point::kTableName, baseRepOid,
								_T("ERROR"), _T("FeatureBuffer�ւ�put�Ɏ��s"), _T(""));
					ret = false;
			}

		// ���C���ʑ����l�ݒ�
		CString addrcode11 = addrcode.bstrVal;
		if( !putValue(featureBuf, fieldMap.FindField(placename_rep_point::kCityCode),addrcode11.Mid(0, 5)) ||
			!putValue(featureBuf, fieldMap.FindField(placename_rep_point::kAddrCode),addrcode11.Mid(5, 6)) || 
			!putValue(featureBuf, fieldMap.FindField(placename_rep_point::kName), adminName) || 
			!putValue(featureBuf, fieldMap.FindField(placename_rep_point::kZipCode),zipcode) || 
			FAILED(featureBuf->putref_Shape(baseRepFeature->GetShapeCopy())) )
		{
			writeErrMsg( base_rep_point::kTableName, baseRepOid,
						_T("ERROR"), _T("FeatureBuffer�ւ�put�Ɏ��s"), _T(""));
			ret = false;
			continue;
		}

		CComVariant oid(0);
		if( FAILED(insertCursor->InsertFeature(featureBuf, &oid)) )
		{
			writeErrMsg( base_rep_point::kTableName, baseRepOid,
						_T("ERROR"), _T("InsertFeature�Ɏ��s"), _T("") );
			ret = false;
		}
		else
			++(placeNameCntInfos.importCount);
	}

	// �C���|�[�g�������ʂɉ����ĕҏW�I��/�j����؂蕪��
	if( ret )
	{
		// �ҏW�I��
		if ( !stopEdit(ret) )
		{
			writeMessage(_T("�ҏW�I���Ɏ��s\n"), RunLogMsgStatus::Error, true);
			placeNameCntInfos.resetImportCount();
		}

		// �e�[�u�����v���X�V
		if( !tableAnalyze(placename_rep_point::kTableName) )
		{
			writeMessage(uh::str_util::format(_T("�e�[�u�����v���X�V�Ɏ��s:%s\n"),
							placename_rep_point::kTableName), RunLogMsgStatus::Error, true);
			ret = false;
		}
	}
	else
	{
		// �ҏW�j��
		abort();
		placeNameCntInfos.resetImportCount();
	}

	if( ret )
		writeMessage(uh::str_util::format(_T("����I��\n")), RunLogMsgStatus::Info, false);
	else
		writeMessage(uh::str_util::format(_T("�G���[�I��\n")), RunLogMsgStatus::Error, false);

	writeMessage(uh::str_util::format(_T("\n���C����\t�S����\t�C���|�[�g����\t���O����\n")),
					RunLogMsgStatus::Normal, false);
	writeMessage(placeNameCntInfos.getLogMsg(), RunLogMsgStatus::Normal, false);

	return ret;
}
