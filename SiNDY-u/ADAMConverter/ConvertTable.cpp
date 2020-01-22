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
	// POSTFIXにORDER BY句が指定されているか
	int iFindOrderBy = findStr(m_postfix, CT_POSTFIX_ORDERBY);

	// なければ終了
	if(0 > iFindOrderBy)
		return;

	// ORDER BY句の部分まで削除
	CString postfix = m_postfix;
	postfix.Delete(0, CString(CT_POSTFIX_ORDERBY).GetLength() + iFindOrderBy);

	// カンマで分割
	std::vector<CString> vecInfo = uh::str_util::split(postfix, CTF_COMMA_CHAR, false);
	for(auto& info : vecInfo){
		// 前後のスペース削除
		info.Trim(CTF_SPACE_CHAR);

		// この状態で半角スペースが存在する場合、半角スペース以降はORDER BY句とは別の句となる
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

	// 変換テーブル情報取得
	std::map<CString,CConvTableInfo> mapConvTableInfo1, mapConvTableInfo2;
	if(!loadConvTable(convTablePath, mapConvTableInfo1)){
		bReturn = false;
	}
	// 変換テーブルが正しく設定されているかチェック
	for(const auto& convTableInfo : mapConvTableInfo1){
		if(!checkConvTableInfo(convTableInfo.second)){
			bReturn = false;
		}
	}

	// 変換フィールド対応表取得
	if(!loadConvField(convFieldPath, mapConvTableInfo2)){
		bReturn = false;
	}
	// 変換フィールド対応表が正しく設定されているかチェック
	for(const auto& convTableInfo : mapConvTableInfo2){
		if(!checkConvFieldInfo(convTableInfo.second)){
			bReturn = false;
		}
	}

	if(bReturn){
		// グループが一致していないとNG
		if(!checkConvGroup(mapConvTableInfo1, mapConvTableInfo2)){
			bReturn = false;
		}
		if(!checkConvGroup(mapConvTableInfo2, mapConvTableInfo1)){
			bReturn = false;
		}
	}

	if(bReturn){
		// 変換テーブルで作成したConvertTableと変換フィールド対応表で作成したContertTableをガッチャンコ
		m_mapConvTableInfo = mapConvTableInfo1;
		for(auto& mapConvTableInfo : m_mapConvTableInfo){
			mapConvTableInfo.second.m_vecFieldDef = mapConvTableInfo2[mapConvTableInfo.first].m_vecFieldDef;
		}
	}

	return bReturn;
}

bool CConvertTable::loadConvTable(const CString& convTablePath, std::map<CString,CConvTableInfo>& mapConvTableInfo)
{
	// 初期化
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

		// 空行、コメント行はスルー
		if(strLine.IsEmpty() ||
			0 == strLine.Left(1).Compare(CTF_COMMENT_CHAR))
			continue;

		CStringTokenizer cToken(strLine, CTF_SPLIT_CHAR, TOKEN_RET_EMPTY_ALL);
		long size = cToken.CountTokens();

		// トークン数が1、且つ[]でくくられているか
		if(1 == size && 0 == strLine.Left(1).CompareNoCase(CTF_GROUP_CHAR_S) && 0 == strLine.Right(1).CompareNoCase(CTF_GROUP_CHAR_E)){
			if(!cConvTableInfo.m_GroupName.IsEmpty()){
				// グループ名の重複はNG
				if(!mapConvTableInfo.insert(std::make_pair(cConvTableInfo.m_GroupName, cConvTableInfo)).second){
					CLog::GetInstance().PrintLog1(false, true, false, true, err_type::fatal, err_code::DuplicateConvTableGroup, cConvTableInfo.m_GroupName);
					bReturn = false;
				}
			} else if(!cConvTableInfo.m_srcTableName.IsEmpty() && !cConvTableInfo.m_dstFileName.IsEmpty()){
				// 必須項目は設定されているが、グループ名が空の場合はNG
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
				// 指定の値以外はエラー
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
				// ORDER BY句に指定されたフィールドリストを取得
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
	// 初期化
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

		// 空行、コメント行はスルー
		if(strLine.IsEmpty() ||
			0 == strLine.Left(1).Compare(CTF_COMMENT_CHAR))
			continue;

		CStringTokenizer cToken(strLine, CTF_SPLIT_CHAR, TOKEN_RET_EMPTY_ALL);
		long size = cToken.CountTokens();

		// トークン数が1、且つ[]でくくられている場合はグループ設定
		if(1 == size && 0 == strLine.Left(1).CompareNoCase(CTF_GROUP_CHAR_S) && 0 == strLine.Right(1).CompareNoCase(CTF_GROUP_CHAR_E)){
			if(!cConvTableInfo.m_GroupName.IsEmpty()){
				// グループ名の重複はNG
				if(!mapConvTableInfo.insert(std::make_pair(cConvTableInfo.m_GroupName, cConvTableInfo)).second){
					CLog::GetInstance().PrintLog1(false, true, false, true, err_type::fatal, err_code::DuplicateConvFieldGroup, cConvTableInfo.m_GroupName);
					bReturn = false;
					continue;
				}
			} else if(!cConvTableInfo.m_vecFieldDef.empty()){
				// フィールド設定はされているが、グループ名が空の場合はNG
				CLog::GetInstance().PrintLog1(false, true, false, true, err_type::fatal, err_code::NoneConvFieldGroup2, cConvTableInfo.m_GroupName);
				bReturn = false;
			}
			cConvTableInfo.clear();
			setColumns.clear();
			strLine.TrimLeft(CTF_GROUP_CHAR_S);
			strLine.TrimRight(CTF_GROUP_CHAR_E);
			cConvTableInfo.m_GroupName = strLine;
		} else {
			// 変換元フィールド設定
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

	// 必須項目（SEARCHTYPE）の設定確認
	if(SEARCHTYPE::UNKNOWN_TYPE == cConvTableInfo.m_searchType){
		CLog::GetInstance().PrintLog1(false, true, false, true, err_type::fatal, err_code::NoneConvTableAttribute, cConvTableInfo.m_GroupName, CT_SEARCHTYPE);
		bReturn = false;
	}

	// 必須項目（SRCTABLE）の設定確認
	if(cConvTableInfo.m_srcTableName.IsEmpty()){
		CLog::GetInstance().PrintLog1(false, true, false, true, err_type::fatal, err_code::NoneConvTableAttribute, cConvTableInfo.m_GroupName, CT_SRCTABLE);
		bReturn = false;
	}
	// 必須項目（DSTFILE）の設定確認
	if(cConvTableInfo.m_dstFileName.IsEmpty()){
		CLog::GetInstance().PrintLog1(false, true, false, true, err_type::fatal, err_code::NoneConvTableAttribute, cConvTableInfo.m_GroupName, CT_DSTFILE);
		bReturn = false;
	}
	// 必須項目（OIDFIELD）の設定確認
	if(cConvTableInfo.m_oidField.IsEmpty()){
		CLog::GetInstance().PrintLog1(false, true, false, true, err_type::fatal, err_code::NoneConvTableAttribute, cConvTableInfo.m_GroupName, CT_OIDFIELD);
		bReturn = false;
	}
	// 検索タイプがQUERYDEF_LIMITED, QUERYDEF_FREEの場合
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
		// 変換元テーブルがオープンできるかチェック
		std::map<CString,ITablePtr> mapTable;
		ITablePtr ipSrcTable;
		if(S_OK != ipFWork->OpenTable(CComBSTR(convTableInfo.second.m_srcTableName), &ipSrcTable) || !ipSrcTable){
			CLog::GetInstance().PrintLog1(false, true, false, true, err_type::error, err_code::NgOpenTable, convTableInfo.second.m_GroupName, convTableInfo.second.m_srcTableName);
			bReturn = false;
		} else {
			mapTable.insert(std::make_pair(convTableInfo.second.m_srcTableName, ipSrcTable));
		}

		// 結合テーブル分もチェック
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

		// OIDFIELDが存在するか
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
			// テーブルが存在するか
			if(mapTable.cend() != fieldDefTable){
				long index = -1;
				// フィールドが存在するか
				if(S_OK != fieldDefTable->second->FindField(CComBSTR(fieldDef.m_srcFieldName), &index) || 0 > index){
					CLog::GetInstance().PrintLog1(false, true, false, true, err_type::error, err_code::NgFindField, convTableInfo.second.m_GroupName, _T("フィールド対応表"), fieldDef.m_srcTableName, fieldDef.m_srcFieldName);
					bReturn = false;
				}
			} else {
				CLog::GetInstance().PrintLog1(false, true, false, true, err_type::error, err_code::NgConvTableFieldName, convTableInfo.second.m_GroupName, _T("フィールド対応表"), fieldDef.m_srcTableName);
				bReturn = false;
			}
		}
	}
	
	return bReturn;
}
