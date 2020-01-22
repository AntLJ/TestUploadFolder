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
#include "ConvertTable.h"

void CConvTableInfo::CreateOrderByFieldList()
{
	// POSTFIX��ORDER BY�傪�w�肳��Ă��邩
	int iFindOrderBy = findStr(m_postfix, CT_POSTFIX_ORDERBY);

	// �Ȃ���ΏI��
	if(0 > iFindOrderBy)
		return;

	// ORDER BY��̕����܂ō폜
	CString postfix = m_postfix;
	postfix.Delete(0, CString(CT_POSTFIX_ORDERBY).GetLength() + iFindOrderBy);

	// �J���}�ŕ���
	std::vector<CString> vecInfo = uh::str_util::split(postfix, CTF_COMMA_CHAR, false);
	for(auto& info : vecInfo){
		// �O��̃X�y�[�X�폜
		info.Trim(CTF_SPACE_CHAR);

		// ���̏�ԂŔ��p�X�y�[�X�����݂���ꍇ�A���p�X�y�[�X�ȍ~��ORDER BY��Ƃ͕ʂ̋�ƂȂ�
		long space = info.Find(CTF_SPACE_CHAR);
		if(0 > space){
			m_vecOrderbyField.push_back(info);
		} else {
			m_vecOrderbyField.push_back(info.Left(space));
			break;
		}
	}
}

bool CConvertTable::init(const CString& convTablePath, const CString& convFieldPath)
{
	bool bReturn = true;

	// �ϊ��e�[�u�����擾
	std::map<CString,CConvTableInfo> mapConvTableInfo1, mapConvTableInfo2;
	if(!loadConvTable(convTablePath, mapConvTableInfo1)){
		bReturn = false;
	}
	// �ϊ��e�[�u�����������ݒ肳��Ă��邩�`�F�b�N
	for(const auto& convTableInfo : mapConvTableInfo1){
		if(!checkConvTableInfo(convTableInfo.second)){
			bReturn = false;
		}
	}

	// �ϊ��t�B�[���h�Ή��\�擾
	if(!loadConvField(convFieldPath, mapConvTableInfo2)){
		bReturn = false;
	}
	// �ϊ��t�B�[���h�Ή��\���������ݒ肳��Ă��邩�`�F�b�N
	for(const auto& convTableInfo : mapConvTableInfo2){
		if(!checkConvFieldInfo(convTableInfo.second)){
			bReturn = false;
		}
	}

	if(bReturn){
		// �O���[�v����v���Ă��Ȃ���NG
		if(!checkConvGroup(mapConvTableInfo1, mapConvTableInfo2)){
			bReturn = false;
		}
		if(!checkConvGroup(mapConvTableInfo2, mapConvTableInfo1)){
			bReturn = false;
		}
	}

	if(bReturn){
		// �ϊ��e�[�u���ō쐬����ConvertTable�ƕϊ��t�B�[���h�Ή��\�ō쐬����ContertTable���K�b�`�����R
		m_mapConvTableInfo = mapConvTableInfo1;
		for(auto& mapConvTableInfo : m_mapConvTableInfo){
			mapConvTableInfo.second.m_vecFieldDef = mapConvTableInfo2[mapConvTableInfo.first].m_vecFieldDef;
		}
	}

	return bReturn;
}

bool CConvertTable::loadConvTable(const CString& convTablePath, std::map<CString,CConvTableInfo>& mapConvTableInfo)
{
	// ������
	mapConvTableInfo.clear();

	std::ifstream ifs(convTablePath);
	if(ifs.fail()){
		CLog::GetInstance().PrintLog1(false, true, false, true, err_type::fatal, err_code::NgLoadConvTable, convTablePath);
		return false;
	}

	bool bReturn = true;

	std::string line;
	CConvTableInfo cConvTableInfo;
	std::set<CString> setColumns;
	while(std::getline(ifs, line)){
		CString strLine(line.c_str());

		// ��s�A�R�����g�s�̓X���[
		if(strLine.IsEmpty() ||
			0 == strLine.Left(1).Compare(CTF_COMMENT_CHAR))
			continue;

		CStringTokenizer cToken(strLine, CTF_SPLIT_CHAR, TOKEN_RET_EMPTY_ALL);
		long size = cToken.CountTokens();

		// �g�[�N������1�A����[]�ł������Ă��邩
		if(1 == size && 0 == strLine.Left(1).CompareNoCase(CTF_GROUP_CHAR_S) && 0 == strLine.Right(1).CompareNoCase(CTF_GROUP_CHAR_E)){
			if(!cConvTableInfo.m_GroupName.IsEmpty()){
				// �O���[�v���̏d����NG
				if(!mapConvTableInfo.insert(std::make_pair(cConvTableInfo.m_GroupName, cConvTableInfo)).second){
					CLog::GetInstance().PrintLog1(false, true, false, true, err_type::fatal, err_code::DuplicateConvTableGroup, cConvTableInfo.m_GroupName);
					bReturn = false;
				}
			} else if(!cConvTableInfo.m_srcTableName.IsEmpty() && !cConvTableInfo.m_dstFileName.IsEmpty()){
				// �K�{���ڂ͐ݒ肳��Ă��邪�A�O���[�v������̏ꍇ��NG
				CLog::GetInstance().PrintLog1(false, true, false, true, err_type::fatal, err_code::NoneConvTableGroup2, cConvTableInfo.m_GroupName);
				bReturn = false;
			}
			cConvTableInfo.clear();
			setColumns.clear();
			strLine.TrimLeft(CTF_GROUP_CHAR_S);
			strLine.TrimRight(CTF_GROUP_CHAR_E);
			cConvTableInfo.m_GroupName = strLine;
		} else {
			if(CT_TABCOUNT != size){
				strLine.Replace(CTF_SPLIT_CHAR, CTF_SPACE_CHAR);
				CLog::GetInstance().PrintLog1(false, true, false, true, err_type::fatal, err_code::NgConTableTab, cConvTableInfo.m_GroupName, CT_TABCOUNT, strLine);
				bReturn = false;
				continue;
			}
			CString column1 = cToken.GetNextToken();
			CString column2 = cToken.GetNextToken();

			if(!setColumns.insert(column1).second){
				CLog::GetInstance().PrintLog1(false, true, false, true, err_type::fatal, err_code::DuplicateConvTableAttribute, cConvTableInfo.m_GroupName, column1);
				bReturn = false;
			}

			if(0 == column1.CompareNoCase(CT_SEARCHTYPE)){
				// �w��̒l�ȊO�̓G���[
				if(0 == column2.CompareNoCase(CT_SEARCHTYPE_NORMAL))
					cConvTableInfo.m_searchType = SEARCHTYPE::NORMAL;
				else if(0 == column2.CompareNoCase(CT_SEARCHTYPE_QUERYDEF_LIMITED))
					cConvTableInfo.m_searchType = SEARCHTYPE::QUERYDEF_LIMITED;
				else if(0 == column2.CompareNoCase(CT_SEARCHTYPE_QUERYDEF_FREE))
					cConvTableInfo.m_searchType = SEARCHTYPE::QUERYDEF_FREE;
				else {
					CLog::GetInstance().PrintLog1(false, true, false, true, err_type::fatal, err_code::UnknownSearchType, cConvTableInfo.m_GroupName, column2);
					bReturn = false;
				}
			} else if(0 == column1.CompareNoCase(CT_SRCTABLE)){
				cConvTableInfo.m_srcTableName = column2;
			} else if(0 == column1.CompareNoCase(CT_DSTFILE)){
				cConvTableInfo.m_dstFileName = column2;
			} else if(0 == column1.CompareNoCase(CT_SUBTABLES)){
				cConvTableInfo.m_subTableNames = column2;
			} else if(0 == column1.CompareNoCase(CT_JOINKEYS)){
				cConvTableInfo.m_joinKeys = column2;
			} else if(0 == column1.CompareNoCase(CT_OIDFIELD)){
				cConvTableInfo.m_oidField = column2;
				int iPos = column2.Find(_T("."));
				if(0 < iPos){
					cConvTableInfo.m_oidTableName = column2.Left(iPos);
				} else {
					CLog::GetInstance().PrintLog1(false, true, false, true, err_type::fatal, err_code::NgOidTableName, cConvTableInfo.m_GroupName, column2);
					bReturn = false;
				}
			} else if(0 == column1.CompareNoCase(CT_POSTFIX)){
				cConvTableInfo.m_postfix = column2;
				// ORDER BY��Ɏw�肳�ꂽ�t�B�[���h���X�g���擾
				cConvTableInfo.CreateOrderByFieldList();
			} else if(0 == column1.CompareNoCase(CT_PREFIX)){
				cConvTableInfo.m_prefix = column2;
			} else {
				CLog::GetInstance().PrintLog1(false, true, false, true, err_type::fatal, err_code::UnknownConvTableAttribute, cConvTableInfo.m_GroupName, column1);
				bReturn = false;
			}
		}
	}
	if(!cConvTableInfo.m_GroupName.IsEmpty()){
		if(!mapConvTableInfo.insert(std::make_pair(cConvTableInfo.m_GroupName, cConvTableInfo)).second){
			CLog::GetInstance().PrintLog1(false, true, false, true, err_type::fatal, err_code::DuplicateConvTableGroup, cConvTableInfo.m_GroupName);
			bReturn = false;
		}
	} else {
		CLog::GetInstance().PrintLog1(false, true, false, true, err_type::fatal, err_code::NoneConvTableGroup);
		bReturn = false;
	}

	return bReturn;
}

bool CConvertTable::loadConvField(const CString& convFieldPath, std::map<CString,CConvTableInfo>& mapConvTableInfo)
{
	// ������
	mapConvTableInfo.clear();

	std::ifstream ifs(convFieldPath);
	if(ifs.fail()){
		CLog::GetInstance().PrintLog1(false, true, false, true, err_type::fatal, err_code::NgLoadConvField, convFieldPath);
		return false;
	}

	bool bReturn = true;

	std::string line;
	CConvTableInfo cConvTableInfo;
	std::set<CString> setColumns;
	while(std::getline(ifs, line)){
		CString strLine(line.c_str());

		// ��s�A�R�����g�s�̓X���[
		if(strLine.IsEmpty() ||
			0 == strLine.Left(1).Compare(CTF_COMMENT_CHAR))
			continue;

		CStringTokenizer cToken(strLine, CTF_SPLIT_CHAR, TOKEN_RET_EMPTY_ALL);
		long size = cToken.CountTokens();

		// �g�[�N������1�A����[]�ł������Ă���ꍇ�̓O���[�v�ݒ�
		if(1 == size && 0 == strLine.Left(1).CompareNoCase(CTF_GROUP_CHAR_S) && 0 == strLine.Right(1).CompareNoCase(CTF_GROUP_CHAR_E)){
			if(!cConvTableInfo.m_GroupName.IsEmpty()){
				// �O���[�v���̏d����NG
				if(!mapConvTableInfo.insert(std::make_pair(cConvTableInfo.m_GroupName, cConvTableInfo)).second){
					CLog::GetInstance().PrintLog1(false, true, false, true, err_type::fatal, err_code::DuplicateConvFieldGroup, cConvTableInfo.m_GroupName);
					bReturn = false;
					continue;
				}
			} else if(!cConvTableInfo.m_vecFieldDef.empty()){
				// �t�B�[���h�ݒ�͂���Ă��邪�A�O���[�v������̏ꍇ��NG
				CLog::GetInstance().PrintLog1(false, true, false, true, err_type::fatal, err_code::NoneConvFieldGroup2, cConvTableInfo.m_GroupName);
				bReturn = false;
			}
			cConvTableInfo.clear();
			setColumns.clear();
			strLine.TrimLeft(CTF_GROUP_CHAR_S);
			strLine.TrimRight(CTF_GROUP_CHAR_E);
			cConvTableInfo.m_GroupName = strLine;
		} else {
			// �ϊ����t�B�[���h�ݒ�
			CFieldDefInfo cFieldDef;
			if(!cFieldDef.init(cConvTableInfo.m_GroupName, strLine)){
				bReturn = false;
				continue;
			}
			cConvTableInfo.m_vecFieldDef.push_back(cFieldDef);
		}
	}
	if(!cConvTableInfo.m_GroupName.IsEmpty()){
		if(!mapConvTableInfo.insert(std::make_pair(cConvTableInfo.m_GroupName, cConvTableInfo)).second){
			CLog::GetInstance().PrintLog1(false, true, false, true, err_type::fatal, err_code::DuplicateConvFieldGroup, cConvTableInfo.m_GroupName);
			bReturn = false;
		}
	} else {
		CLog::GetInstance().PrintLog1(false, true, false, true, err_type::fatal, err_code::NoneConvFieldGroup, cConvTableInfo.m_GroupName);
		bReturn = false;
	}

	return bReturn;
}

bool CConvertTable::checkConvGroup(const std::map<CString,CConvTableInfo>& mapConvTableInfo1, const std::map<CString,CConvTableInfo>& mapConvTableInfo2)
{
	bool bReturn = true;
	for(const auto& convTableInfo1 : mapConvTableInfo1){
		if(mapConvTableInfo2.cend() == mapConvTableInfo2.find(convTableInfo1.first)){
			CLog::GetInstance().PrintLog1(false, true, false, true, err_type::fatal, err_code::NgMatchGroup, convTableInfo1.first);
			bReturn = false;
		}
	}
	return bReturn;
}

bool CConvertTable::checkConvTableInfo(const CConvTableInfo& cConvTableInfo)
{
	bool bReturn = true;

	// �K�{���ځiSEARCHTYPE�j�̐ݒ�m�F
	if(SEARCHTYPE::UNKNOWN_TYPE == cConvTableInfo.m_searchType){
		CLog::GetInstance().PrintLog1(false, true, false, true, err_type::fatal, err_code::NoneConvTableAttribute, cConvTableInfo.m_GroupName, CT_SEARCHTYPE);
		bReturn = false;
	}

	// �K�{���ځiSRCTABLE�j�̐ݒ�m�F
	if(cConvTableInfo.m_srcTableName.IsEmpty()){
		CLog::GetInstance().PrintLog1(false, true, false, true, err_type::fatal, err_code::NoneConvTableAttribute, cConvTableInfo.m_GroupName, CT_SRCTABLE);
		bReturn = false;
	}
	// �K�{���ځiDSTFILE�j�̐ݒ�m�F
	if(cConvTableInfo.m_dstFileName.IsEmpty()){
		CLog::GetInstance().PrintLog1(false, true, false, true, err_type::fatal, err_code::NoneConvTableAttribute, cConvTableInfo.m_GroupName, CT_DSTFILE);
		bReturn = false;
	}
	// �K�{���ځiOIDFIELD�j�̐ݒ�m�F
	if(cConvTableInfo.m_oidField.IsEmpty()){
		CLog::GetInstance().PrintLog1(false, true, false, true, err_type::fatal, err_code::NoneConvTableAttribute, cConvTableInfo.m_GroupName, CT_OIDFIELD);
		bReturn = false;
	}
	// �����^�C�v��QUERYDEF_LIMITED, QUERYDEF_FREE�̏ꍇ
	if(SEARCHTYPE::QUERYDEF_LIMITED == cConvTableInfo.m_searchType ||
		SEARCHTYPE::QUERYDEF_FREE == cConvTableInfo.m_searchType){
		if(cConvTableInfo.m_subTableNames.IsEmpty()){
			CLog::GetInstance().PrintLog1(false, true, false, true, err_type::fatal, err_code::NoneConvTableAttribute, cConvTableInfo.m_GroupName, CT_SUBTABLES);
			bReturn = false;
		}
		if(cConvTableInfo.m_joinKeys.IsEmpty()){
			CLog::GetInstance().PrintLog1(false, true, false, true, err_type::fatal, err_code::NoneConvTableAttribute, cConvTableInfo.m_GroupName, CT_JOINKEYS);
			bReturn = false;
		}
	}
	return bReturn;
}

bool CConvertTable::checkConvFieldInfo(const CConvTableInfo& cConvTableInfo)
{
	bool bReturn = true;
	std::set<CString> setDstFieldName;
	for(const auto& fieldDef : cConvTableInfo.m_vecFieldDef){
		if(!setDstFieldName.insert(fieldDef.m_dstFieldName).second){
			CLog::GetInstance().PrintLog1(false, true, false, true, err_type::fatal, err_code::DuplicateDstField, cConvTableInfo.m_GroupName, fieldDef.m_dstFieldName);
			bReturn = false;
		}
	}
	return bReturn;
}

bool CConvertTable::checkConvTableFeild(const IFeatureWorkspacePtr& ipFWork)
{
	bool bReturn = true;
	for(const auto& convTableInfo : m_mapConvTableInfo){
		// �ϊ����e�[�u�����I�[�v���ł��邩�`�F�b�N
		std::map<CString,ITablePtr> mapTable;
		ITablePtr ipSrcTable;
		if(S_OK != ipFWork->OpenTable(CComBSTR(convTableInfo.second.m_srcTableName), &ipSrcTable) || !ipSrcTable){
			CLog::GetInstance().PrintLog1(false, true, false, true, err_type::error, err_code::NgOpenTable, convTableInfo.second.m_GroupName, convTableInfo.second.m_srcTableName);
			bReturn = false;
		} else {
			mapTable.insert(std::make_pair(convTableInfo.second.m_srcTableName, ipSrcTable));
		}

		// �����e�[�u�������`�F�b�N
		if(!convTableInfo.second.m_subTableNames.IsEmpty()){
			CStringTokenizer cToken(convTableInfo.second.m_subTableNames, CTF_COMMA_CHAR, TOKEN_RET_EMPTY_ALL);
			long count = cToken.CountTokens();
			for(long l = 0; l < count; ++l){
				CString strSubTableName = cToken.GetNextToken();
				ITablePtr ipSubTable;
				if(S_OK != ipFWork->OpenTable(CComBSTR(strSubTableName), &ipSubTable) || !ipSubTable){
					CLog::GetInstance().PrintLog1(false, true, false, true, err_type::error, err_code::NgOpenTable, convTableInfo.second.m_GroupName, strSubTableName);
					bReturn = false;
				} else {
					mapTable.insert(std::make_pair(strSubTableName, ipSubTable));
				}
			}
		}

		// OIDFIELD�����݂��邩
		CStringTokenizer cOIDToken(convTableInfo.second.m_oidField, CTF_PELIOD_CHAR, TOKEN_RET_EMPTY_ALL);
		CString strOIDTableName = cOIDToken.GetNextToken();
		CString strOIDFieldName = cOIDToken.GetNextToken();
		const auto& oidTable = mapTable.find(strOIDTableName);
		if(mapTable.cend() != oidTable){
			long index = -1;
			if(S_OK != oidTable->second->FindField(CComBSTR(strOIDFieldName), &index) || 0 > index){
				CLog::GetInstance().PrintLog1(false, true, false, true, err_type::error, err_code::NgFindField, convTableInfo.second.m_GroupName, CT_OIDFIELD, strOIDTableName, strOIDFieldName);
				bReturn = false;
			}
		} else {
			CLog::GetInstance().PrintLog1(false, true, false, true, err_type::error, err_code::NgConvTableFieldName, convTableInfo.second.m_GroupName, CT_OIDFIELD, strOIDTableName);
			bReturn = false;
		}

		for(const auto& fieldDef : convTableInfo.second.m_vecFieldDef){
			const auto& fieldDefTable = mapTable.find(fieldDef.m_srcTableName);
			// �e�[�u�������݂��邩
			if(mapTable.cend() != fieldDefTable){
				long index = -1;
				// �t�B�[���h�����݂��邩
				if(S_OK != fieldDefTable->second->FindField(CComBSTR(fieldDef.m_srcFieldName), &index) || 0 > index){
					CLog::GetInstance().PrintLog1(false, true, false, true, err_type::error, err_code::NgFindField, convTableInfo.second.m_GroupName, _T("�t�B�[���h�Ή��\"), fieldDef.m_srcTableName, fieldDef.m_srcFieldName);
					bReturn = false;
				}
			} else {
				CLog::GetInstance().PrintLog1(false, true, false, true, err_type::error, err_code::NgConvTableFieldName, convTableInfo.second.m_GroupName, _T("�t�B�[���h�Ή��\"), fieldDef.m_srcTableName);
				bReturn = false;
			}
		}
	}
	
	return bReturn;
}
