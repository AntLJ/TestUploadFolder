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
	// �ϊ����擾
	if(!m_cConvertTable.init(args.m_conv_table_path.c_str(), args.m_conv_field_path.c_str())){
		return false;
	}

	// �o�̓I�v�V�����擾
	if(!m_cOutputFormat.init(args)){
		return false;
	}

	// �o�̓f�B���N�g���擾
	m_strOutputDir = args.m_output_dir.c_str();

	// �ϊ���DB�ڑ�
	m_ipSrcWork = sindy::create_workspace(args.m_db_info.c_str());
	if(!m_ipSrcWork){
		CLog::GetInstance().PrintLog1(false, true, false, true, err_type::fatal, err_code::NgDBConnect, CString(args.m_db_info.c_str()));
		return false;
	}

	// �ݒ肵���ϊ��e�[�u���A�ϊ��t�B�[���h�Ή��\�����f�[�^�Ƃ��đ��݂��邩�`�F�b�N
	if(!m_cConvertTable.checkConvTableFeild(IFeatureWorkspacePtr(m_ipSrcWork))){
		return false;
	}

	// �n��p�����[�^�ǂݍ���
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

	// �ϊ����̃O���[�v�P�ʂŏ���
	for(auto& convTableInfo : m_cConvertTable.m_mapConvTableInfo){
		CString strMsg;
		strMsg.Format(_T("��%s"), convTableInfo.first);
		CLog::GetInstance().PrintLog(true, false, true, false, strMsg);

		// �o�̓t�B�[���h���J���}��؂�Ŏ擾
		CString strSubFields;
		if(!getSubFields(convTableInfo.second.m_vecFieldDef, convTableInfo.second.m_oidField, strSubFields)){
			CLog::GetInstance().PrintLog1(false, true, false, true, err_type::error, err_code::NgGetSubFields, convTableInfo.second.m_GroupName);
			bReturn = false;
			continue;
		}

		// �e�[�u����������/���Ȃ��ŏ����𕪂���
		_ICursorPtr ipCursor;
		long size = 0;
		switch(convTableInfo.second.m_searchType){
		case SEARCHTYPE::NORMAL:
			// �ʏ�e�[�u���p�̃J�[�\���擾
			if(!getCursorForNormal(convTableInfo.second, strSubFields, ipCursor, size)){
				bReturn = false;
				continue;
			}
			break;
		case SEARCHTYPE::QUERYDEF_LIMITED:
		case SEARCHTYPE::QUERYDEF_FREE:
			// �e�[�u�������p�̃J�[�\���擾
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

		// �ϊ����e�[�u�����̃t�B�[���h�C���f�b�N�X���X�V
		if(!updateFieldIndex(ipCursor, convTableInfo.second)){
			bReturn = false;
			continue;
		}

		// Z,M,�`��^�C�v���擾
		bool isZ = false, isM = false;
		esriGeometryType geomType = esriGeometryType::esriGeometryNull;
		if(!getHasZM(convTableInfo.second.m_GroupName, ipCursor, isZ, isM, geomType)){
			bReturn = false;
			continue;
		}

		// �o�̓f�[�^�쐬�p�N���X
		COutputString cOutputString(m_cnv, m_cOutputFormat, isZ, isM, geomType);
		CString preper;
		time_t st_time;
		time(&st_time);
		long n = 0;

		// �o�̓t�@�C���쐬
		CString strOutputFile;
		strOutputFile.Format(_T("%s\\%s"), m_strOutputDir, convTableInfo.second.m_dstFileName);
		std::ofstream ofs;
		ofs.open(strOutputFile, std::ios::out | std::ios::binary);
		if(ofs.fail()){
			CLog::GetInstance().PrintLog1(false, true, false, true, err_type::error, err_code::NgOpenOutputFile, strOutputFile);
			bReturn = false;
			continue;
		}

		// �o�̓t�@�C���Ƀw�b�_����ݒ�
		if(m_cOutputFormat.m_Header){
			CString strHeader;
			if(!cOutputString.createHeader(convTableInfo.second, isZ, isM, strHeader)){
				bReturn = false;
				continue;
			}
			// �����R�[�h�ϊ����o��
			switch(m_cOutputFormat.m_Encode){
			case ENCODE::SHIFT_JIS:
				ofs << CT2A(strHeader);
				break;
			case ENCODE::UTF_8:
				ofs << UTF16ToUTF8(CStringW(CT2W(strHeader)));
				break;
			default:
				// �����ɂ͂��Ȃ��͂�
				CLog::GetInstance().PrintLog1(false, true, false, true, err_type::error, err_code::NgEncode);
				bReturn = false;
				break;
			}
		}

		strMsg.Format(_T("�f�[�^�o�͏����J�n : %s"), getNowTime());
		CLog::GetInstance().PrintLog(true, false, true, false, strMsg);

		// �f�[�^�����o���iQUERYDEF_LIMITED��ORDER BY��̐ݒ�{DISTINCT�ݒ肪����Ă���ꍇ�̓��j�[�N�o�́j
		if(SEARCHTYPE::QUERYDEF_LIMITED == convTableInfo.second.m_searchType && !convTableInfo.second.m_vecOrderbyField.empty() && 0 <= convTableInfo.second.m_prefix.Find(CT_PREFIX_DISTINCT)){
			if(!outputUniqData(ipCursor, cOutputString, convTableInfo.second, ofs, size))
				bReturn = false;
		} else {
			if(!outputData(ipCursor, cOutputString, convTableInfo.second, ofs, size))
				bReturn = false;
		}

		ofs.close();

		strMsg.Format(_T("�f�[�^�o�͏������� : %s"), getNowTime());
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

		// �o�͕�����쐬
		CString strOutput;
		if(!cOutputString.createString(cConvTableInfo, ipRow, strOutput)){
			bReturn = false;
			continue;
		}

		// �t�@�C���ɏ����o��
		if(!writeData(m_cOutputFormat.m_Encode, strOutput, ofs))
			bReturn = false;

		// �����󋵕\��
		CString per;
		double dper = (double)(((double)n / (double)size) * 100.0);
		per.Format(_T("%.1f"), dper);
		if(0 != preper.CompareNoCase(per)){
			ofs << std::flush;
			time_t do_time;
			time(&do_time);
			double dtime = difftime(do_time, st_time);
			std::cout << CT2A(per) << "% ( �o�ߎ��� " << long(dtime) << "[s] )\r";
			preper = per;
		}
	}
	time_t end_time;
	time(&end_time);
	double dtime = difftime(end_time, st_time);
	std::cout << "100.0% ( �o�ߎ��� " << long(dtime) << "[s] )\n";
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

	// ORDRE BY��Ɏw�肳��Ă���t�B�[���h�̃C���f�b�N�X�擾
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

		// �o�͕�����쐬
		CString strOutput;
		if(!cOutputString.createString(cConvTableInfo, ipRow, strOutput)){
			bReturn = false;
			continue;
		}
		
		// �S�f�[�^����DISTINCT�͂ł��Ȃ��̂ŁAORDER BY�Ŏw�肵���t�B�[���h�l�̃O���[�v�P�ʂŃ`�F�b�N����
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
		// ORDER BY��̃L�[���ς��΃��j�[�N�m�F�p�̃R���e�i���N���A
		if(vecPreOrderByValue != vecOrderByValue){
			setUniqCheck.clear();
			vecPreOrderByValue = vecOrderByValue;
		}

		if(setUniqCheck.insert(strOutput).second){
			// �t�@�C���ɏ����o��
			if(!writeData(m_cOutputFormat.m_Encode, strOutput, ofs))
				bReturn = false;
		}

		// �����󋵕\��
		CString per;
		double dper = (double)(((double)n / (double)size) * 100.0);
		per.Format(_T("%.1f"), dper);
		if(0 != preper.CompareNoCase(per)){
			ofs << std::flush;
			time_t do_time;
			time(&do_time);
			double dtime = difftime(do_time, st_time);
			std::cout << CT2A(per) << "% ( �o�ߎ��� " << long(dtime) << "[s] )\r";
			preper = per;
		}
	}

	time_t end_time;
	time(&end_time);
	double dtime = difftime(end_time, st_time);
	std::cout << "100.0% ( �o�ߎ��� " << long(dtime) << "[s] )\n";
	ofs << std::flush;

	return bReturn;
}

bool CADAMConverter::writeData(const ENCODE::ECode encode, const CString& strOutputData, std::ofstream& ofs)
{
	// �����R�[�h�ϊ����o��
	switch(encode){
	case ENCODE::SHIFT_JIS:
		ofs << CT2A(strOutputData);
		break;
	case ENCODE::UTF_8:
		ofs << UTF16ToUTF8(CStringW(CT2W(strOutputData)));
		break;
	default:
		// �����ɂ͂��Ȃ��͂�
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
		// �Ή�����t�B�[���h�����݂��邩
		long index = -1;
		CString fieldName;
		if(convTableInfo.m_subTableNames.IsEmpty())
			fieldName.Format(_T("%s"), fieldDef.m_srcFieldName);
		else
			fieldName.Format(_T("%s.%s"), fieldDef.m_srcTableName, fieldDef.m_srcFieldName);

		if(S_OK == ipCursor->FindField(CComBSTR(fieldName), &index) && 0 <= index){
			// �Ή��\�̕ϊ����t�B�[���h�C���f�b�N�X���X�V
			fieldDef.m_srcFieldIndex = index;
		} else {
			bool bCheck = false;
			if(!convTableInfo.m_prefix.IsEmpty()){
				// Prefix�̐ݒ肪����ꍇ�A1�Ԗڂ̃t�B�[���h����Prefix�Őݒ肵���l���܂܂��̂ŁA�A�A
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
	// ������
	subFields.Empty();

	// �t�B�[���h��񂪂Ȃ���΃G���[
	if(vecFieldDef.empty()){
		return false;
	}
	// �G���[���R�[�h����p�̃t�B�[���h�����ݒ肳��Ă��Ȃ��ꍇ��NG
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
	// �ϊ��t�B�[���h�Ή��\�ɂ���Βǉ����Ȃ�
	if(!bOid)
		subFields.AppendFormat(_T("%s"), oidField);
	subFields.TrimRight(_T(","));

	return true;
}


bool CADAMConverter::getCursorForNormal(const CConvTableInfo& cConvTableInfo, const CString& strSubFields, _ICursorPtr& ipCursor, long& size)
{
	// �ϊ����e�[�u���擾
	ITablePtr ipSrcTable;
	if(S_OK != IFeatureWorkspacePtr(m_ipSrcWork)->OpenTable(CComBSTR(cConvTableInfo.m_srcTableName), &ipSrcTable) || !ipSrcTable){
		CLog::GetInstance().PrintLog1(false, true, false, true, err_type::error, err_code::NgOpenTable, cConvTableInfo.m_GroupName, cConvTableInfo.m_srcTableName);
		return false;
	}

	// �t�B���^�쐬�i�擾�t�B�[���h�ݒ�̂݁j
	IQueryFilterPtr ipQueryFilter(CLSID_QueryFilter);
	if(S_OK != ipQueryFilter->put_SubFields(CComBSTR(strSubFields))){
		CLog::GetInstance().PrintLog1(false, true, false, true, err_type::error, err_code::NgSetSubFields, cConvTableInfo.m_GroupName);
		return false;
	}

	CString strMsg;
	strMsg.Format(_T("���R�[�h���擾�J�n : %s"), getNowTime());
	CLog::GetInstance().PrintLog(true, false, true, false, strMsg);

	// ���R�[�h�������擾
	if(S_OK != ipSrcTable->RowCount(NULL, &size)){
		CLog::GetInstance().PrintLog1(false, true, false, true, err_type::error, err_code::NgGetRowCount, cConvTableInfo.m_GroupName);
		return false;
	}
	strMsg.Format(_T("���R�[�h���擾�I�� : %s"), getNowTime());
	CLog::GetInstance().PrintLog(true, false, true, false, strMsg);

	strMsg.Format(_T("���R�[�h�擾�J�n : %s"), getNowTime());
	CLog::GetInstance().PrintLog(true, false, true, false, strMsg);

	// �J�[�\���擾
	if(S_OK != ipSrcTable->Search(ipQueryFilter, VARIANT_FALSE, &ipCursor) || !ipCursor){
		CLog::GetInstance().PrintLog1(false, true, false, true, err_type::error, err_code::NgGetCursor, cConvTableInfo.m_GroupName);
		return false;
	}
	strMsg.Format(_T("���R�[�h�擾���� : %s"), getNowTime());
	CLog::GetInstance().PrintLog(true, false, true, false, strMsg);

	return true;
}

bool CADAMConverter::getCursorForQueryDef(const CConvTableInfo& cConvTableInfo, const CString& strSubFields, _ICursorPtr& ipCursor, long& size)
{
	CString strMsg;
	strMsg.Format(_T("���R�[�h���擾�J�n : %s"), getNowTime());
	CLog::GetInstance().PrintLog(true, false, true, false, strMsg);

	// ���R�[�h�����擾
	if(!getRowCountForQueryDef(cConvTableInfo, size)){
		return false;
	}
	strMsg.Format(_T("���R�[�h���擾���� : %s"), getNowTime());
	CLog::GetInstance().PrintLog(true, false, true, false, strMsg);

	strMsg.Format(_T("���R�[�h�擾�J�n : %s"), getNowTime());
	CLog::GetInstance().PrintLog(true, false, true, false, strMsg);

	// �J�[�\���擾
	if(!getCursorFromQueryDef(cConvTableInfo, strSubFields, ipCursor)){
		return false;
	}

	strMsg.Format(_T("���R�[�h�擾���� : %s"), getNowTime());
	CLog::GetInstance().PrintLog(true, false, true, false, strMsg);

	return true;
}

bool CADAMConverter::getRowCountForQueryDef(const CConvTableInfo& cConvTableInfo, long &size)
{
	// ���R�[�h��������
	size = 0;

	// �J�[�\���擾
	_ICursorPtr ipCursor;
	if(!getCursorFromQueryDef(cConvTableInfo, _T("COUNT(*)"), ipCursor)){
		return false;
	}

	// ���R�[�h�擾
	_IRowPtr ipRow;
	if(!SUCCEEDED(ipCursor->NextRow(&ipRow))){
		CLog::GetInstance().PrintLog1(false, true, false, true, err_type::error, err_code::NgNextRowCount, cConvTableInfo.m_GroupName);
		return false;
	}
	// ���R�[�h�擾�ł��Ă���Ό������擾�iFGDB�ł�0���̏ꍇ�̓��R�[�h�擾����Ȃ����߁j
	if(ipRow){
		// �����擾
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

	// ��������e�[�u����ݒ�
	CString strTables;
	if(SEARCHTYPE::QUERYDEF_LIMITED == cConvTableInfo.m_searchType)
		strTables.Format(_T("%s,%s"), cConvTableInfo.m_srcTableName, cConvTableInfo.m_subTableNames);
	else
		strTables.Format(_T("%s"), cConvTableInfo.m_joinKeys);
	if(S_OK != ipQueryDef->put_Tables(CComBSTR(strTables))){
		CLog::GetInstance().PrintLog1(false, true, false, true, err_type::error, err_code::NgJoinTable, cConvTableInfo.m_GroupName);
		return false;
	}

	// �擾�t�B�[���h�ݒ�
	if(S_OK != ipQueryDef->put_SubFields(CComBSTR(strSubFields))){
		CLog::GetInstance().PrintLog1(false, true, false, true, err_type::error, err_code::NgSetSubFields, cConvTableInfo.m_GroupName);
		return false;
	}

	if(SEARCHTYPE::QUERYDEF_LIMITED == cConvTableInfo.m_searchType){
		// �����L�[�ݒ�
		if(S_OK != ipQueryDef->put_WhereClause(CComBSTR(cConvTableInfo.m_joinKeys))){
			CLog::GetInstance().PrintLog1(false, true, false, true, err_type::error, err_code::NgSetFilter, cConvTableInfo.m_GroupName);
			return false;
		}
	}

	if(!cConvTableInfo.m_postfix.IsEmpty()){
		// PostfixClause�ݒ�
		if(S_OK != IQueryDef2Ptr(ipQueryDef)->put_PostfixClause(CComBSTR(cConvTableInfo.m_postfix))){
			CLog::GetInstance().PrintLog1(false, true, false, true, err_type::error, err_code::NgSetPostfixClause, cConvTableInfo.m_GroupName);
			return false;
		}
	}

	// �v���t�B�b�N�X�̐ݒ�͌�������ł̂�
	if(SEARCHTYPE::QUERYDEF_FREE == cConvTableInfo.m_searchType){
		if(!cConvTableInfo.m_prefix.IsEmpty()){
			// PrefixClause�ݒ�
			if(S_OK != IQueryDef2Ptr(ipQueryDef)->put_PrefixClause(CComBSTR(cConvTableInfo.m_prefix))){
				CLog::GetInstance().PrintLog1(false, true, false, true, err_type::error, err_code::NgSetPrefixClause, cConvTableInfo.m_GroupName);
				return false;
			}
		}
	}

	// �J�[�\���擾
	if(S_OK != ipQueryDef->Evaluate2(VARIANT_FALSE, &ipCursor) || !ipCursor){
		CLog::GetInstance().PrintLog1(false, true, false, true, err_type::error, err_code::NgEvalute, cConvTableInfo.m_GroupName);
		return false;
	}

	return true;
}

bool CADAMConverter::getHasZM(const CString& strGroupName, const _ICursorPtr& ipCursor, bool& isZ, bool& isM, esriGeometryType& geomType)
{
	// �o�̓f�[�^�̏�����
	isZ = false;
	isM = false;

	// �`��t�B�[���h�������Z,M,�`��^�C�v���擾
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
		// �`��t�B�[���h���ǂ����`�F�b�N
		IGeometryDefPtr ipGeomDef;
		if(S_OK != ipField->get_GeometryDef(&ipGeomDef)){
			CLog::GetInstance().PrintLog1(false, true, false, true, err_type::error, err_code::NgGetGeomDef, strGroupName, l);
			return false;
		}
		if(ipGeomDef){
			// �`��t�B�[���h�ł����ZM�l���L�����ǂ���
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

			// �`��^�C�v���擾���Ă���
			if(S_OK != ipGeomDef->get_GeometryType(&geomType)){
				CLog::GetInstance().PrintLog1(false, true, false, true, err_type::error, err_code::NgGetGeomType, strGroupName);
				return false;
			}
		}
	}
	return true;
}
