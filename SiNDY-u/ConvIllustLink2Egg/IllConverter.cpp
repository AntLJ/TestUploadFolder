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
#include "IllConverter.h"
#include "LogSys.h"

#include <sindy/schema/egg.h>

using namespace egg::schema;

IllConverter::IllConverter(std::shared_ptr<PgDataBaseEx> orgDataBase,
						   std::shared_ptr<PgDataBaseEx> tgtDataBase, 
						   LPCTSTR orgSchema, 
						   LPCTSTR tgtSchema)
	:m_orgDataBase(orgDataBase),m_tgtDataBase(tgtDataBase)
{
	m_illustLink.Format(_T("%s.%s"), orgSchema, sindy::schema::illust_link::kTableName);
	m_roadLink.Format(_T("%s.%s"), orgSchema, sindy::schema::road_link::kTableName);
	m_infIllustLink.Format(_T("%s.%s"), tgtSchema, ::inf_illust::kTableName);
	m_lqIllustLink.Format(_T("%s.%s"), tgtSchema, ::lq_illust::kTableName);
	m_infFields.Format(_T("%s, %s, %s"), ::inf_illust::kObjectId, ::inf_illust::kIllust, ::inf_illust::kFileName);
	m_lqFields.Format(_T("%s, %s, %s, %s"), ::lq_illust::kInfId, ::lq_illust::kLinkId, ::lq_illust::kLinkDir, ::lq_illust::kSequence);
}

IllConverter::~IllConverter(void)
{
}

bool IllConverter::Convert(const std::vector<IllSetRecord>& illSetInfo, const std::map<ClassCode, std::map<OID, IllFileNames>>& inputListInfo)
{
	CString errMsg;
	bool retVal = true;
	CString procMsg;
	// �ϊ����e�[�u�����݃`�F�b�N
	if (! m_orgDataBase->exists(ns::Database::TABLE, CT2CA(m_illustLink))) {
		LogSys::GetInstance().WriteFreeStyleLog(true, err_level::err, _T("�ϊ���DB�Ƀe�[�u�������݂��܂���"), m_illustLink);
		return false;
	}
	// �o�̓e�[�u���쐬
	LogSys::GetInstance().WriteProcLog(true, true, _T("�o�̓e�[�u���쐬\n"));
	if (! CreateIllTables())
		return false;
	//�C���X�g�������ݒ�t�@�C���̃��R�[�h�P�ʂŏ��������{
	for (const auto& illSet :illSetInfo) {
		procMsg.Format(_T("%s = %s, %s = %s\n"),  sindy::schema::illust_link::kIllustClass, illSet.e_illCls, sindy::schema::illust_link::kCustomerCode, illSet.e_dstCode);
		LogSys::GetInstance().WriteProcLog(true, false, procMsg);
		// �Ώۃ��R�[�h�Q�擾
		ns::RowsPtr rows;
		if (! GetIllLinkRows(illSet.e_illCls, illSet.e_dstCode, rows)) {
			retVal = false;
			continue;
		}
		std::vector<ns::RowPtr> tgtRows;
		ns::RowPtr row;
		while (row = rows->next()) {
			tgtRows.push_back(row);
		}
		// �Ώۃ��R�[�h��0�̏ꍇ�́A�G���[�Ƃ���B(�R�[�h�l�h���C���O�̒l���C���X�g�������ݒ�t�@�C���Ŏw�肳��Ă���\�������邽��)
		if (!tgtRows.size()) {
			errMsg.Format(_T("%s = %s, %s = %s�̏����Ɋ֘A�t�������N�����݂��܂���B�������ݒ�t�@�C���ɖ�肪���������m�F���Ă��������B\n"), sindy::schema::illust_link::kIllustClass, illSet.e_illCls, sindy::schema::illust_link::kCustomerCode, illSet.e_dstCode);
			LogSys::GetInstance().WriteFreeStyleLog(true, err_level::err, errMsg);
			retVal = false;
			continue;
		}
		// �C���X�g�����NID�̃C���X�g�摜���Ή��\�擾
		ClassCode classCode(illSet.e_illCls, illSet.e_dstCode);
		std::map<ClassCode, std::map<OID, IllFileNames>>::const_iterator itr = inputListInfo.find(classCode);
		if (inputListInfo.end() == itr) {
			errMsg.Format(_T("%s = %d, %s = %d�@�̃C���X�g�摜���Ή��\�����݂��܂���"), sindy::schema::illust_link::kIllustClass, classCode.e_illCls, sindy::schema::illust_link::kCustomerCode, classCode.e_dstCode);
			LogSys::GetInstance().WriteFreeStyleLog(true, err_level::err, errMsg, sindy::schema::illust_link::kTableName);
			retVal = false;
			continue;
		}
		// �ϊ�����
		std::map<OID,InfIllInfo> infIllInfos;
		std::map<LqIllKey, LqIllInfo> lqIllInfos;
		if (! m_illCooker.Cook(m_orgDataBase, 
							   tgtRows,
							   itr->second,
							   m_roadLink,
							   infIllInfos,
							   lqIllInfos)) {
			retVal = false;
			continue;			   
		}
		// ���R�[�h��񁨃e�L�X�g
		std::string infCopyValues, lqCopyValues;
		Info2Str(infIllInfos, lqIllInfos, infCopyValues, lqCopyValues);
		// �C���|�[�g����
		if (! importValues(CT2CA(m_infIllustLink), CT2CA(m_infFields), infCopyValues)) {
			retVal = false;
			continue;
		}
		if (! importValues(CT2CA(m_lqIllustLink), CT2CA(m_lqFields), lqCopyValues)) {
			retVal = false;
			continue;
		}
	}
	return retVal;
}
bool IllConverter::CreateIllTables()
{
	// �e�[�u�����݃`�F�b�N(���ɑc�[����ꍇ�͍폜����B
	if (m_tgtDataBase->exists(ns::Database::TABLE, CT2CA(m_infIllustLink))) {
		if (! DeleteTable(m_tgtDataBase, m_infIllustLink))
			return false;
	}
	if (m_tgtDataBase->exists(ns::Database::TABLE, CT2CA(m_lqIllustLink))) {
		if (! DeleteTable(m_tgtDataBase, m_lqIllustLink))
			return false;
	}
	// SQL�쐬
	// �t�B�[���h���
	CString infFields, lqFields;
	infFields.Format(_T("%s integer NOT NULL, %s integer NOT NULL, %s varchar(10) NOT NULL"), ::inf_illust::kObjectId, ::inf_illust::kIllust, ::inf_illust::kFileName);
	lqFields.Format(_T("%s integer NOT NULL, %s integer NOT NULL, %s integer NOT NULL, %s integer NOT NULL"), ::lq_illust::kInfId, ::lq_illust::kLinkId, ::lq_illust::kLinkDir, ::lq_illust::kSequence);
	// Create���쐬
	CString makeInf, makeLq;
	makeInf.Format(_T("CREATE TABLE %s ( %s, CONSTRAINT %s_pkey PRIMARY KEY (%s) )"), m_infIllustLink, infFields, ::inf_illust::kTableName, ::inf_illust::kObjectId);
	makeLq.Format(_T("CREATE TABLE %s ( %s, CONSTRAINT %s_pkey PRIMARY KEY (%s,\"%s\") )"), m_lqIllustLink, lqFields, ::lq_illust::kTableName, ::lq_illust::kInfId, ::lq_illust::kSequence);
	// SQL���s
	if (!m_tgtDataBase->executeCommand(CT2CA(makeInf))) {
		LogSys::GetInstance().WriteFreeStyleLog(true, err_level::err, _T("�e�[�u���쐬�Ɏ��s���܂���"), m_infIllustLink);
		return false;
	}
	if (!m_tgtDataBase->executeCommand(CT2CA(makeLq))) {
		LogSys::GetInstance().WriteFreeStyleLog(true, err_level::err, _T("�e�[�u���쐬�Ɏ��s���܂���"), m_lqIllustLink);
		return false;
	}
	// index�t�^
	CString makeLqIndex;
	makeLqIndex.Format(_T("CREATE INDEX %s_%s ON %s (%s)"), ::lq_illust::kTableName, ::lq_illust::kLinkId, m_lqIllustLink, ::lq_illust::kLinkId);
	// SQL���s
	if (! m_tgtDataBase->executeCommand(CT2CA(makeLqIndex))) {
		LogSys::GetInstance().WriteFreeStyleLog(true, err_level::err, _T("�C���f�b�N�X�t�^�Ɏ��s���܂���"), m_lqIllustLink);
		return false;
	}
	return true;
}

bool IllConverter::DeleteTable(std::shared_ptr<PgDataBaseEx> dataBase, LPCTSTR tableName)
{
	CString deleteSql;
	deleteSql.Format(_T("DROP TABLE %s;"), tableName);
	if (!dataBase->executeCommand(CT2CA(deleteSql))) {
		LogSys::GetInstance().WriteFreeStyleLog(true, err_level::err, _T("�����e�[�u���폜�Ɏ��s���܂���"), tableName);
		return false;
	}
	return true;
}


bool IllConverter::GetIllLinkRows(LPCTSTR illClass, LPCTSTR dstCode, ns::RowsPtr& rows)
{
	// ILLUST_LINK�擾SQL�쐬
	// �����Fillust_class_c=X AND customer_c=X
	CString getIllLink;
	getIllLink.Format(_T("SELECT * FROM %s WHERE %s = %s AND %s = %s"), m_illustLink, sindy::schema::illust_link::kIllustClass, illClass, sindy::schema::illust_link::kCustomerCode, dstCode);
	// rows�擾
	rows = m_orgDataBase->execute(CT2CA(getIllLink));
	if (! rows) {
		CString errMsg;
		errMsg.Format(_T("�f�[�^���o���s:%s=%s and %s=%s"), sindy::schema::illust_link::kIllustClass, illClass, sindy::schema::illust_link::kCustomerCode, dstCode);
		LogSys::GetInstance().WriteFreeStyleLog(true, err_level::err, errMsg, sindy::schema::illust_link::kTableName);
		return false;
	}
	return true;
}


void IllConverter::Info2Str(const std::map<OID, InfIllInfo>& infIllInfos,
						   const std::map<LqIllKey, LqIllInfo>& lqIllInfos,
						   std::string& infCopyValues,
						   std::string& lqCopyValues)
{
	infCopyValues.clear();
	lqCopyValues.clear();
	for (const auto& infInfo : infIllInfos) {
		CString temp;
		temp.Format(_T("%d\t%d\t%s\n"), infInfo.first, infInfo.second.e_illustCode, infInfo.second.e_fileName);
		infCopyValues += CT2CA(temp);
	}
	for (const auto& lqInfo : lqIllInfos) {
		CString temp;
		temp.Format(_T("%d\t%d\t%d\t%d\n"), lqInfo.first.e_infId, lqInfo.second.e_linkId, lqInfo.second.e_linkDir, lqInfo.first.e_Seq);
		lqCopyValues += CT2CA(temp);
	}
}

bool IllConverter::importValues(LPCSTR tableName, LPCSTR fields, const std::string& importValues)
{
	// �R�s�[�J�n
	if (! m_tgtDataBase->StartCopy(tableName, fields)) {
		LogSys::GetInstance().WriteFreeStyleLog(true, err_level::err, CA2CT(m_tgtDataBase->getErrMsg().c_str()));
		return false;
	}
	// �R�s�[���{
	if (! m_tgtDataBase->copyData(importValues.c_str(), importValues.size())) {
		LogSys::GetInstance().WriteFreeStyleLog(true, err_level::err, CA2CT(m_tgtDataBase->getErrMsg().c_str()));
		return false;
	}
	// �R�s�[����
	if (! m_tgtDataBase->EndCopy()) {
		LogSys::GetInstance().WriteFreeStyleLog(true, err_level::err, CA2CT(m_tgtDataBase->getErrMsg().c_str()));
		return false;
	}
	return true;
}
