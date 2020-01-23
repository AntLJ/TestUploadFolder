#include "stdafx.h"
#include "ConvRep.h"
#include "Log.h"
#include "Common.h"
#include <AheGlobalsMisc.h>

bool CConvRep::createFieldIndex()
{
	if(!setFieldIndex(sindy::schema::rel_addr::kTableName20, m_ipRelAddr, m_mapFieldIndexRelAddr) ||
		!setFieldIndex(m_strShapefileName, m_ipOutShapefile, m_mapFieldIndexOutShapefile)){
			return false;
	}
	return true;
}

bool CConvRep::createFields(IFieldsPtr& ipFields)
{
	if(!addAttrField(m_strShapefileName, zip_pol::schema::zip_rep::kZipCode, esriFieldType::esriFieldTypeString, VARIANT_FALSE, VARIANT_FALSE, zip_pol::schema::zip_rep::eZipCode, 0, ipFields) ||
		!addAttrField(m_strShapefileName, zip_pol::schema::zip_rep::kPrefCode, esriFieldType::esriFieldTypeString, VARIANT_FALSE, VARIANT_FALSE, zip_pol::schema::zip_rep::ePrefCode, 0, ipFields) ||
		!addAttrField(m_strShapefileName, zip_pol::schema::zip_rep::kCityCode, esriFieldType::esriFieldTypeString, VARIANT_FALSE, VARIANT_FALSE, zip_pol::schema::zip_rep::eCityCode, 0, ipFields) ||
		!addShapeField(m_strShapefileName, esriGeometryPoint, ipFields)){
			return false;
	}

	return true;
}

bool CConvRep::loadData()
{
	bool bReturn = true;

	IQueryFilterPtr ipQFilter(CLSID_QueryFilter);
	CString strWhere;
	// 整備状態が「4：使用中（5桁）」のものだけ取得
	strWhere.Format(_T("%s=%ld"), sindy::schema::rel_addr::kModifyStatus, sindy::schema::rel_addr::ModifyStatusCode::Used5digit);
	if(S_OK != ipQFilter->put_WhereClause(CComBSTR(strWhere))){
		CLog::GetInstance().PrintLog2(false, true, false, true, err_type::error, err_code::NgPutWhereClause, sindy::schema::rel_addr::kTableName20, _T(""));
		return false;
	}

	_ICursorPtr ipCursor;
	if(S_OK != m_ipRelAddr->Search(ipQFilter, VARIANT_FALSE, &ipCursor)){
		CLog::GetInstance().PrintLog2(false, true, false, true, err_type::error, err_code::NgGetCursor, sindy::schema::rel_addr::kTableName20, _T(""));
		return false;
	}
	bool bRet = true;
	_IRowPtr ipRow;
	while(S_OK == ipCursor->NextRow(&ipRow) && ipRow){
		// ZIPCODE,ADDRCODE取得
		CComVariant vaZipCode, vaAddrCode;
		if(S_OK != ipRow->get_Value(m_mapFieldIndexRelAddr[sindy::schema::rel_addr::kZipCode], &vaZipCode) ||
			S_OK != ipRow->get_Value(m_mapFieldIndexRelAddr[sindy::schema::rel_addr::kAddrCode], &vaAddrCode)){
				CLog::GetInstance().PrintLog2(false, true, false, true, err_type::error, err_code::NgGetValue, sindy::schema::rel_addr::kTableName20, getID(ipRow));
				return false;
		}

		// 住所コードが5桁でない場合はNG
		if(CString(vaAddrCode.bstrVal).GetLength() != REP_ADDRCODE_COUNT){
			CLog::GetInstance().PrintLog2(false, true, false, true, err_type::error, err_code::NgAddrCode5Digit, sindy::schema::rel_addr::kTableName20, getID(ipRow), CString(vaAddrCode.bstrVal));
			bReturn = false;
			continue;
		}
		// 郵便番号の下2桁が「00」出ない場合もNG
		if(0 != CString(vaZipCode.bstrVal).Right(2).CompareNoCase(REP_ZIPCODE)){
			CLog::GetInstance().PrintLog2(false, true, false, true, err_type::error, err_code::NgZipCodeUnder00, sindy::schema::rel_addr::kTableName20, getID(ipRow), CString(vaZipCode.bstrVal));
			bReturn = false;
			continue;
		}
		CString strZipCode(vaZipCode.bstrVal);
		CString strPrefCode = CString(vaAddrCode.bstrVal).Left(2);
		CString strCityCode = CString(vaAddrCode.bstrVal).Right(3);

		// 住所DBから市区町村代表点取得
		CString strSQL;
		strSQL.Format(_T("SELECT LONGITUDE,LATITUDE FROM ADDRESS_POINT WHERE KEN_CODE='%s' AND SHI_CODE='%s' AND OAZA_CODE='000' AND AZA_CODE='000' AND CHIBAN_CODE='00000' AND BANGO_CODE='0000'"), strPrefCode, strCityCode);
		auto cursor = m_cADOBase->search(CT2A(strSQL));

		if(FAILED(cursor.err())){
			bReturn = false;
			CLog::GetInstance().PrintLog2(false, true, false, true, err_type::error, err_code::NgGetCursor, m_strAddrDB, _T(""), cursor.GetErrorMessage());
			continue;
		}

		for(const auto& row : cursor){
			IPointPtr ipPoint(CLSID_Point);
			// TODO:DECIMAL型→double型へのもっとスマートな変換方法があれば、、、、
			CString strX, strY;
			strX.Format(_T("%lld"), row[0].decVal.Lo64);
			strY.Format(_T("%lld"), row[1].decVal.Lo64);
			strX = strX.Left(strX.GetLength()-row[0].decVal.signscale) + _T(".") + strX.Right(row[0].decVal.signscale);
			strY = strY.Left(strY.GetLength()-row[1].decVal.signscale) + _T(".") + strY.Right(row[1].decVal.signscale);
			double x = _ttof(strX);
			double y = _ttof(strY);
			if(S_OK != ipPoint->PutCoords(x, y)){
				CLog::GetInstance().PrintLog2(false, true, false, true, err_type::error, err_code::NgPutCoords, m_strAddrDB, _T(""), _T("ZIPCODE:")+strZipCode);
				bReturn = false;
				continue;
			}
			// 無いと思うけど複数レコード取得できる場合は住所DBがNG
			if(!m_mapZipRep[strZipCode].insert(std::make_pair(std::make_pair(strPrefCode,strCityCode), ipPoint)).second){
				bReturn = false;
				CLog::GetInstance().PrintLog2(false, true, false, true, err_type::error, err_code::NgDuplicateAddrRec, m_strAddrDB, _T(""), _T("SQL:")+strSQL);
				continue;
			}
		}
	}

	return bReturn;
}

bool CConvRep::outputData()
{
	IFeatureCursorPtr ipFCursor;
	if(S_OK != IFeatureClassPtr(m_ipOutShapefile)->Insert(VARIANT_FALSE, &ipFCursor)){
		CLog::GetInstance().PrintLog2(false, true, false, true, err_type::error, err_code::NgInsertCursor, m_strShapefileName, _T(""));
		return false;
	}

	long n = 0;
	for(const auto& zipRep : m_mapZipRep){
		for(const auto& rep : zipRep.second){
			++n;
			IFeatureBufferPtr ipFeatureBuffer;
			if(S_OK != IFeatureClassPtr(m_ipOutShapefile)->CreateFeatureBuffer(&ipFeatureBuffer)){
				CLog::GetInstance().PrintLog2(false, true, false, true, err_type::error, err_code::NgCreateRowBuffer, m_strShapefileName, _T(""));
				return false;
			}
			if(S_OK != ipFeatureBuffer->put_Value(m_mapFieldIndexOutShapefile[zip_pol::schema::zip_rep::kZipCode], CComVariant(zipRep.first))){
				CLog::GetInstance().PrintLog2(false, true, false, true, err_type::error, err_code::NgPutValue, m_strShapefileName, _T(""));
				return false;
			}
			if(S_OK != ipFeatureBuffer->put_Value(m_mapFieldIndexOutShapefile[zip_pol::schema::zip_rep::kPrefCode], CComVariant(rep.first.first))){
				CLog::GetInstance().PrintLog2(false, true, false, true, err_type::error, err_code::NgPutValue, m_strShapefileName, _T(""));
				return false;
			}
			if(S_OK != ipFeatureBuffer->put_Value(m_mapFieldIndexOutShapefile[zip_pol::schema::zip_rep::kCityCode], CComVariant(rep.first.second))){
				CLog::GetInstance().PrintLog2(false, true, false, true, err_type::error, err_code::NgPutValue, m_strShapefileName, _T(""));
				return false;
			}
			IGeometryPtr ipGeom(AheGetClone(rep.second));
			if(S_OK != ipFeatureBuffer->putref_Shape(ipGeom)){
				CLog::GetInstance().PrintLog2(false, true, false, true, err_type::error, err_code::NgPutShape, m_strShapefileName, _T(""));
				return false;
			}

			CComVariant vaOID;
			if(S_OK != ipFCursor->InsertFeature(ipFeatureBuffer, &vaOID)){
				CLog::GetInstance().PrintLog2(false, true, false, true, err_type::error, err_code::NgInsertRow, m_strShapefileName, _T(""));
				return false;
			}
			// Flushはこまめに
			if(0 == n % FLUSH_COUNT){
				if(FAILED(ipFCursor->Flush())){
					CLog::GetInstance().PrintLog2(false, true, false, true, err_type::error, err_code::NgFlush, m_strShapefileName, _T(""));
					return false;
				}
			}
		}
	}
	if(FAILED(ipFCursor->Flush())){
		CLog::GetInstance().PrintLog2(false, true, false, true, err_type::error, err_code::NgFlush, m_strShapefileName, _T(""));
		return false;
	}

	return true;
}
