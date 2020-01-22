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
#include "ADAMConverter.h"

bool CADAMConverter::init(const Arguments& args)
{
	// 変換情報取得
	if(!m_cConvertTable.init(args.m_conv_table_path.c_str(), args.m_conv_field_path.c_str())){
		return false;
	}

	// 出力オプション取得
	if(!m_cOutputFormat.init(args)){
		return false;
	}

	// 出力ディレクトリ取得
	m_strOutputDir = args.m_output_dir.c_str();

	// 変換元DB接続
	m_ipSrcWork = sindy::create_workspace(args.m_db_info.c_str());
	if(!m_ipSrcWork){
		CLog::GetInstance().PrintLog1(false, true, false, true, err_type::fatal, err_code::NgDBConnect, CString(args.m_db_info.c_str()));
		return false;
	}

	// 設定した変換テーブル、変換フィールド対応表が実データとして存在するかチェック
	if(!m_cConvertTable.checkConvTableFeild(IFeatureWorkspacePtr(m_ipSrcWork))){
		return false;
	}

	// 地域パラメータ読み込み
	if(esriSRGeoCS_Tokyo != m_cOutputFormat.m_GeoRef){
		if(!m_cnv.JGD2000_RP_Init(CT2A(args.m_cnv_param_path.c_str()))){
			CLog::GetInstance().PrintLog1(false, true, false, true, err_type::fatal, err_code::NgLoadCnvParam, CString(args.m_cnv_param_path.c_str()));
			return false;
		}
	}

	return true;
}

bool CADAMConverter::run()
{
	bool bReturn = true;

	// 変換情報のグループ単位で処理
	for(auto& convTableInfo : m_cConvertTable.m_mapConvTableInfo){
		CString strMsg;
		strMsg.Format(_T("■%s"), convTableInfo.first);
		CLog::GetInstance().PrintLog(true, false, true, false, strMsg);

		// 出力フィールドをカンマ区切りで取得
		CString strSubFields;
		if(!getSubFields(convTableInfo.second.m_vecFieldDef, convTableInfo.second.m_oidField, strSubFields)){
			CLog::GetInstance().PrintLog1(false, true, false, true, err_type::error, err_code::NgGetSubFields, convTableInfo.second.m_GroupName);
			bReturn = false;
			continue;
		}

		// テーブル結合する/しないで処理を分ける
		_ICursorPtr ipCursor;
		long size = 0;
		switch(convTableInfo.second.m_searchType){
		case SEARCHTYPE::NORMAL:
			// 通常テーブル用のカーソル取得
			if(!getCursorForNormal(convTableInfo.second, strSubFields, ipCursor, size)){
				bReturn = false;
				continue;
			}
			break;
		case SEARCHTYPE::QUERYDEF_LIMITED:
		case SEARCHTYPE::QUERYDEF_FREE:
			// テーブル結合用のカーソル取得
			if(!getCursorForQueryDef(convTableInfo.second, strSubFields, ipCursor, size)){
				bReturn = false;
				continue;
			}
			break;
		default:
			bReturn = false;
			CLog::GetInstance().PrintLog1(false, true, false, true, err_type::fatal, err_code::UnknownSearchType);
			continue;
			break;
		}

		// 変換元テーブル情報のフィールドインデックスを更新
		if(!updateFieldIndex(ipCursor, convTableInfo.second)){
			bReturn = false;
			continue;
		}

		// Z,M,形状タイプを取得
		bool isZ = false, isM = false;
		esriGeometryType geomType = esriGeometryType::esriGeometryNull;
		if(!getHasZM(convTableInfo.second.m_GroupName, ipCursor, isZ, isM, geomType)){
			bReturn = false;
			continue;
		}

		// 出力データ作成用クラス
		COutputString cOutputString(m_cnv, m_cOutputFormat, isZ, isM, geomType);
		CString preper;
		time_t st_time;
		time(&st_time);
		long n = 0;

		// 出力ファイル作成
		CString strOutputFile;
		strOutputFile.Format(_T("%s\\%s"), m_strOutputDir, convTableInfo.second.m_dstFileName);
		std::ofstream ofs;
		ofs.open(strOutputFile, std::ios::out | std::ios::binary);
		if(ofs.fail()){
			CLog::GetInstance().PrintLog1(false, true, false, true, err_type::error, err_code::NgOpenOutputFile, strOutputFile);
			bReturn = false;
			continue;
		}

		// 出力ファイルにヘッダ情報を設定
		if(m_cOutputFormat.m_Header){
			CString strHeader;
			if(!cOutputString.createHeader(convTableInfo.second, isZ, isM, strHeader)){
				bReturn = false;
				continue;
			}
			// 文字コード変換しつつ出力
			switch(m_cOutputFormat.m_Encode){
			case ENCODE::SHIFT_JIS:
				ofs << CT2A(strHeader);
				break;
			case ENCODE::UTF_8:
				ofs << UTF16ToUTF8(CStringW(CT2W(strHeader)));
				break;
			default:
				// ここにはこないはず
				CLog::GetInstance().PrintLog1(false, true, false, true, err_type::error, err_code::NgEncode);
				bReturn = false;
				break;
			}
		}

		strMsg.Format(_T("データ出力処理開始 : %s"), getNowTime());
		CLog::GetInstance().PrintLog(true, false, true, false, strMsg);

		// データ書き出し（QUERYDEF_LIMITEDでORDER BY句の設定＋DISTINCT設定がされている場合はユニーク出力）
		if(SEARCHTYPE::QUERYDEF_LIMITED == convTableInfo.second.m_searchType && !convTableInfo.second.m_vecOrderbyField.empty() && 0 <= convTableInfo.second.m_prefix.Find(CT_PREFIX_DISTINCT)){
			if(!outputUniqData(ipCursor, cOutputString, convTableInfo.second, ofs, size))
				bReturn = false;
		} else {
			if(!outputData(ipCursor, cOutputString, convTableInfo.second, ofs, size))
				bReturn = false;
		}

		ofs.close();

		strMsg.Format(_T("データ出力処理完了 : %s"), getNowTime());
		CLog::GetInstance().PrintLog(true, false, true, false, strMsg);
	}

	return bReturn;
}


bool CADAMConverter::outputData(const _ICursorPtr& ipCursor, COutputString& cOutputString, const CConvTableInfo& cConvTableInfo, std::ofstream& ofs, long size)
{
	bool bReturn = true;

	long n = 0;
	CString preper;
	time_t st_time;
	time(&st_time);
	_IRowPtr ipRow;
	while(S_OK == ipCursor->NextRow(&ipRow) && ipRow){
		++n;

		// 出力文字列作成
		CString strOutput;
		if(!cOutputString.createString(cConvTableInfo, ipRow, strOutput)){
			bReturn = false;
			continue;
		}

		// ファイルに書き出し
		if(!writeData(m_cOutputFormat.m_Encode, strOutput, ofs))
			bReturn = false;

		// 処理状況表示
		CString per;
		double dper = (double)(((double)n / (double)size) * 100.0);
		per.Format(_T("%.1f"), dper);
		if(0 != preper.CompareNoCase(per)){
			ofs << std::flush;
			time_t do_time;
			time(&do_time);
			double dtime = difftime(do_time, st_time);
			std::cout << CT2A(per) << "% ( 経過時間 " << long(dtime) << "[s] )\r";
			preper = per;
		}
	}
	time_t end_time;
	time(&end_time);
	double dtime = difftime(end_time, st_time);
	std::cout << "100.0% ( 経過時間 " << long(dtime) << "[s] )\n";
	ofs << std::flush;

	return bReturn;
}

bool CADAMConverter::outputUniqData(const _ICursorPtr& ipCursor, COutputString& cOutputString, const CConvTableInfo& cConvTableInfo, std::ofstream& ofs, long size)
{
	bool bReturn = true;

	long n = 0;
	CString preper;
	time_t st_time;
	time(&st_time);

	// ORDRE BY句に指定されているフィールドのインデックス取得
	std::vector<long> vecOrderByFieldIndex;
	for(const auto& orderbyField : cConvTableInfo.m_vecOrderbyField){
		long lIndex = -1;
		if(S_OK != ipCursor->FindField(CComBSTR(orderbyField), &lIndex) || 0 > lIndex){
			CLog::GetInstance().PrintLog1(false, true, false, true, err_type::error, err_code::NgExistField, cConvTableInfo.m_GroupName, cConvTableInfo.m_srcTableName, orderbyField);
			continue;
		}
		vecOrderByFieldIndex.push_back(lIndex);
	}

	std::set<CString> setUniqCheck;
	std::vector<CComVariant> vecPreOrderByValue;

	_IRowPtr ipRow;
	while(S_OK == ipCursor->NextRow(&ipRow) && ipRow){
		++n;

		// 出力文字列作成
		CString strOutput;
		if(!cOutputString.createString(cConvTableInfo, ipRow, strOutput)){
			bReturn = false;
			continue;
		}
		
		// 全データからDISTINCTはできないので、ORDER BYで指定したフィールド値のグループ単位でチェックする
		std::vector<CComVariant> vecOrderByValue;
		for(const auto& index : vecOrderByFieldIndex){
			CComVariant vaValue;
			if(S_OK != ipRow->get_Value(index, &vaValue)){
				// err
				CLog::GetInstance().PrintLog2(false, true, true, true, err_type::error, err_code::NgGetValue, cConvTableInfo.m_oidTableName, _T(""));
				return false;
			}
			vecOrderByValue.push_back(vaValue);
		}
		// ORDER BY句のキーが変わればユニーク確認用のコンテナをクリア
		if(vecPreOrderByValue != vecOrderByValue){
			setUniqCheck.clear();
			vecPreOrderByValue = vecOrderByValue;
		}

		if(setUniqCheck.insert(strOutput).second){
			// ファイルに書き出し
			if(!writeData(m_cOutputFormat.m_Encode, strOutput, ofs))
				bReturn = false;
		}

		// 処理状況表示
		CString per;
		double dper = (double)(((double)n / (double)size) * 100.0);
		per.Format(_T("%.1f"), dper);
		if(0 != preper.CompareNoCase(per)){
			ofs << std::flush;
			time_t do_time;
			time(&do_time);
			double dtime = difftime(do_time, st_time);
			std::cout << CT2A(per) << "% ( 経過時間 " << long(dtime) << "[s] )\r";
			preper = per;
		}
	}

	time_t end_time;
	time(&end_time);
	double dtime = difftime(end_time, st_time);
	std::cout << "100.0% ( 経過時間 " << long(dtime) << "[s] )\n";
	ofs << std::flush;

	return bReturn;
}

bool CADAMConverter::writeData(const ENCODE::ECode encode, const CString& strOutputData, std::ofstream& ofs)
{
	// 文字コード変換しつつ出力
	switch(encode){
	case ENCODE::SHIFT_JIS:
		ofs << CT2A(strOutputData);
		break;
	case ENCODE::UTF_8:
		ofs << UTF16ToUTF8(CStringW(CT2W(strOutputData)));
		break;
	default:
		// ここにはこないはず
		CLog::GetInstance().PrintLog1(false, true, false, true, err_type::error, err_code::NgEncode);
		return false;
		break;
	}
	return true;
}

bool CADAMConverter::updateFieldIndex(const _ICursorPtr& ipCursor, CConvTableInfo& convTableInfo)
{
	bool bReturn = true;

	for(auto& fieldDef : convTableInfo.m_vecFieldDef){
		// 対応するフィールドが存在するか
		long index = -1;
		CString fieldName;
		if(convTableInfo.m_subTableNames.IsEmpty())
			fieldName.Format(_T("%s"), fieldDef.m_srcFieldName);
		else
			fieldName.Format(_T("%s.%s"), fieldDef.m_srcTableName, fieldDef.m_srcFieldName);

		if(S_OK == ipCursor->FindField(CComBSTR(fieldName), &index) && 0 <= index){
			// 対応表の変換元フィールドインデックスを更新
			fieldDef.m_srcFieldIndex = index;
		} else {
			bool bCheck = false;
			if(!convTableInfo.m_prefix.IsEmpty()){
				// Prefixの設定がある場合、1番目のフィールド名にPrefixで設定した値も含まれるので、、、
				CString fieldName2 = convTableInfo.m_prefix + _T(" ") + fieldName;
				if(S_OK == ipCursor->FindField(CComBSTR(fieldName2), &index) && 0 <= index){
					fieldDef.m_srcFieldIndex = index;
					bCheck = true;
				}
			}
			if(!bCheck){
				CLog::GetInstance().PrintLog1(false, true, false, true, err_type::error, err_code::NgExistField, convTableInfo.m_GroupName, convTableInfo.m_srcTableName, fieldDef.m_srcFieldName);
				bReturn = false;
			}
		}
	}
	return bReturn;
}

bool CADAMConverter::getSubFields(const std::vector<CFieldDefInfo>& vecFieldDef, const CString& oidField, CString& subFields)
{
	// 初期化
	subFields.Empty();

	// フィールド情報がなければエラー
	if(vecFieldDef.empty()){
		return false;
	}
	// エラーレコード特定用のフィールド名が設定されていない場合はNG
	if(oidField.IsEmpty()){
		return false;
	}

	bool bOid = false;
	for(const auto& field : vecFieldDef){
		CString strFieldName;
		strFieldName.Format(_T("%s.%s"),field.m_srcTableName, field.m_srcFieldName);
		if(0 == strFieldName.CompareNoCase(oidField))
			bOid = true;
		subFields.AppendFormat(_T("%s,"), strFieldName);
	}
	// 変換フィールド対応表にあれば追加しない
	if(!bOid)
		subFields.AppendFormat(_T("%s"), oidField);
	subFields.TrimRight(_T(","));

	return true;
}


bool CADAMConverter::getCursorForNormal(const CConvTableInfo& cConvTableInfo, const CString& strSubFields, _ICursorPtr& ipCursor, long& size)
{
	// 変換元テーブル取得
	ITablePtr ipSrcTable;
	if(S_OK != IFeatureWorkspacePtr(m_ipSrcWork)->OpenTable(CComBSTR(cConvTableInfo.m_srcTableName), &ipSrcTable) || !ipSrcTable){
		CLog::GetInstance().PrintLog1(false, true, false, true, err_type::error, err_code::NgOpenTable, cConvTableInfo.m_GroupName, cConvTableInfo.m_srcTableName);
		return false;
	}

	// フィルタ作成（取得フィールド設定のみ）
	IQueryFilterPtr ipQueryFilter(CLSID_QueryFilter);
	if(S_OK != ipQueryFilter->put_SubFields(CComBSTR(strSubFields))){
		CLog::GetInstance().PrintLog1(false, true, false, true, err_type::error, err_code::NgSetSubFields, cConvTableInfo.m_GroupName);
		return false;
	}

	CString strMsg;
	strMsg.Format(_T("レコード数取得開始 : %s"), getNowTime());
	CLog::GetInstance().PrintLog(true, false, true, false, strMsg);

	// レコード総件数取得
	if(S_OK != ipSrcTable->RowCount(NULL, &size)){
		CLog::GetInstance().PrintLog1(false, true, false, true, err_type::error, err_code::NgGetRowCount, cConvTableInfo.m_GroupName);
		return false;
	}
	strMsg.Format(_T("レコード数取得終了 : %s"), getNowTime());
	CLog::GetInstance().PrintLog(true, false, true, false, strMsg);

	strMsg.Format(_T("レコード取得開始 : %s"), getNowTime());
	CLog::GetInstance().PrintLog(true, false, true, false, strMsg);

	// カーソル取得
	if(S_OK != ipSrcTable->Search(ipQueryFilter, VARIANT_FALSE, &ipCursor) || !ipCursor){
		CLog::GetInstance().PrintLog1(false, true, false, true, err_type::error, err_code::NgGetCursor, cConvTableInfo.m_GroupName);
		return false;
	}
	strMsg.Format(_T("レコード取得完了 : %s"), getNowTime());
	CLog::GetInstance().PrintLog(true, false, true, false, strMsg);

	return true;
}

bool CADAMConverter::getCursorForQueryDef(const CConvTableInfo& cConvTableInfo, const CString& strSubFields, _ICursorPtr& ipCursor, long& size)
{
	CString strMsg;
	strMsg.Format(_T("レコード数取得開始 : %s"), getNowTime());
	CLog::GetInstance().PrintLog(true, false, true, false, strMsg);

	// レコード総数取得
	if(!getRowCountForQueryDef(cConvTableInfo, size)){
		return false;
	}
	strMsg.Format(_T("レコード数取得完了 : %s"), getNowTime());
	CLog::GetInstance().PrintLog(true, false, true, false, strMsg);

	strMsg.Format(_T("レコード取得開始 : %s"), getNowTime());
	CLog::GetInstance().PrintLog(true, false, true, false, strMsg);

	// カーソル取得
	if(!getCursorFromQueryDef(cConvTableInfo, strSubFields, ipCursor)){
		return false;
	}

	strMsg.Format(_T("レコード取得完了 : %s"), getNowTime());
	CLog::GetInstance().PrintLog(true, false, true, false, strMsg);

	return true;
}

bool CADAMConverter::getRowCountForQueryDef(const CConvTableInfo& cConvTableInfo, long &size)
{
	// レコード数初期化
	size = 0;

	// カーソル取得
	_ICursorPtr ipCursor;
	if(!getCursorFromQueryDef(cConvTableInfo, _T("COUNT(*)"), ipCursor)){
		return false;
	}

	// レコード取得
	_IRowPtr ipRow;
	if(!SUCCEEDED(ipCursor->NextRow(&ipRow))){
		CLog::GetInstance().PrintLog1(false, true, false, true, err_type::error, err_code::NgNextRowCount, cConvTableInfo.m_GroupName);
		return false;
	}
	// レコード取得できていれば件数情報取得（FGDBでは0件の場合はレコード取得されないため）
	if(ipRow){
		// 件数取得
		CComVariant vaCount;
		if(S_OK != ipRow->get_Value(0, &vaCount)){
			CLog::GetInstance().PrintLog1(false, true, false, true, err_type::error, err_code::NgGetValueCount, cConvTableInfo.m_GroupName);
			return false;
		}
		vaCount.ChangeType(VT_I4);
		size = vaCount.lVal;
	}

	return true;
}

bool CADAMConverter::getCursorFromQueryDef(const CConvTableInfo& cConvTableInfo, const CString& strSubFields, _ICursorPtr& ipCursor)
{
	IQueryDefPtr ipQueryDefTmp;
	if(S_OK != IFeatureWorkspacePtr(m_ipSrcWork)->CreateQueryDef(&ipQueryDefTmp) || !ipQueryDefTmp){
		CLog::GetInstance().PrintLog1(false, true, false, true, err_type::error, err_code::NgQueryDef, cConvTableInfo.m_GroupName);
		return false;
	}
	IQueryDef2Ptr ipQueryDef = ipQueryDefTmp;

	// 結合するテーブルを設定
	CString strTables;
	if(SEARCHTYPE::QUERYDEF_LIMITED == cConvTableInfo.m_searchType)
		strTables.Format(_T("%s,%s"), cConvTableInfo.m_srcTableName, cConvTableInfo.m_subTableNames);
	else
		strTables.Format(_T("%s"), cConvTableInfo.m_joinKeys);
	if(S_OK != ipQueryDef->put_Tables(CComBSTR(strTables))){
		CLog::GetInstance().PrintLog1(false, true, false, true, err_type::error, err_code::NgJoinTable, cConvTableInfo.m_GroupName);
		return false;
	}

	// 取得フィールド設定
	if(S_OK != ipQueryDef->put_SubFields(CComBSTR(strSubFields))){
		CLog::GetInstance().PrintLog1(false, true, false, true, err_type::error, err_code::NgSetSubFields, cConvTableInfo.m_GroupName);
		return false;
	}

	if(SEARCHTYPE::QUERYDEF_LIMITED == cConvTableInfo.m_searchType){
		// 結合キー設定
		if(S_OK != ipQueryDef->put_WhereClause(CComBSTR(cConvTableInfo.m_joinKeys))){
			CLog::GetInstance().PrintLog1(false, true, false, true, err_type::error, err_code::NgSetFilter, cConvTableInfo.m_GroupName);
			return false;
		}
	}

	if(!cConvTableInfo.m_postfix.IsEmpty()){
		// PostfixClause設定
		if(S_OK != IQueryDef2Ptr(ipQueryDef)->put_PostfixClause(CComBSTR(cConvTableInfo.m_postfix))){
			CLog::GetInstance().PrintLog1(false, true, false, true, err_type::error, err_code::NgSetPostfixClause, cConvTableInfo.m_GroupName);
			return false;
		}
	}

	// プレフィックスの設定は限定解除版のみ
	if(SEARCHTYPE::QUERYDEF_FREE == cConvTableInfo.m_searchType){
		if(!cConvTableInfo.m_prefix.IsEmpty()){
			// PrefixClause設定
			if(S_OK != IQueryDef2Ptr(ipQueryDef)->put_PrefixClause(CComBSTR(cConvTableInfo.m_prefix))){
				CLog::GetInstance().PrintLog1(false, true, false, true, err_type::error, err_code::NgSetPrefixClause, cConvTableInfo.m_GroupName);
				return false;
			}
		}
	}

	// カーソル取得
	if(S_OK != ipQueryDef->Evaluate2(VARIANT_FALSE, &ipCursor) || !ipCursor){
		CLog::GetInstance().PrintLog1(false, true, false, true, err_type::error, err_code::NgEvalute, cConvTableInfo.m_GroupName);
		return false;
	}

	return true;
}

bool CADAMConverter::getHasZM(const CString& strGroupName, const _ICursorPtr& ipCursor, bool& isZ, bool& isM, esriGeometryType& geomType)
{
	// 出力データの初期化
	isZ = false;
	isM = false;

	// 形状フィールドがあればZ,M,形状タイプを取得
	IFieldsPtr ipFields;
	if(S_OK != ipCursor->get_Fields(&ipFields) || !ipFields){
		CLog::GetInstance().PrintLog1(false, true, false, true, err_type::error, err_code::NgGetFields, strGroupName);
		return false;
	}
	long fieldCount = 0;
	if(S_OK != ipFields->get_FieldCount(&fieldCount) || 0 >= fieldCount){
		CLog::GetInstance().PrintLog1(false, true, false, true, err_type::error, err_code::NgGetFieldCount, strGroupName);
		return false;
	}
	for(long l = 0; l < fieldCount; ++l){
		IFieldPtr ipField;
		if(S_OK != ipFields->get_Field(l, &ipField) || !ipField){
			CLog::GetInstance().PrintLog1(false, true, false, true, err_type::error, err_code::NgGetField, strGroupName, l);
			return false;
		}
		// 形状フィールドかどうかチェック
		IGeometryDefPtr ipGeomDef;
		if(S_OK != ipField->get_GeometryDef(&ipGeomDef)){
			CLog::GetInstance().PrintLog1(false, true, false, true, err_type::error, err_code::NgGetGeomDef, strGroupName, l);
			return false;
		}
		if(ipGeomDef){
			// 形状フィールドであればZM値が有効かどうか
			VARIANT_BOOL vaIsZ = VARIANT_FALSE;
			if(S_OK != ipGeomDef->get_HasZ(&vaIsZ)){
				CLog::GetInstance().PrintLog1(false, true, false, true, err_type::error, err_code::NgGetHasZ, strGroupName);
				return false;
			}
			isZ = vaIsZ ? true : false;

			VARIANT_BOOL vaIsM = VARIANT_FALSE;
			if(S_OK != ipGeomDef->get_HasM(&vaIsM)){
				CLog::GetInstance().PrintLog1(false, true, false, true, err_type::error, err_code::NgGetHasM, strGroupName);
				return false;
			}
			isM = vaIsM ? true : false;

			// 形状タイプも取得しておく
			if(S_OK != ipGeomDef->get_GeometryType(&geomType)){
				CLog::GetInstance().PrintLog1(false, true, false, true, err_type::error, err_code::NgGetGeomType, strGroupName);
				return false;
			}
		}
	}
	return true;
}
