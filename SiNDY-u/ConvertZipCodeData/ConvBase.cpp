#include "stdafx.h"
#include "ConvBase.h"
#include "Log.h"
#include "Common.h"

bool CConvBase::init()
{
	CLog::GetInstance().PrintLog(true, false, true, false, getNowTime() + _T(" 初期化開始"), true);
	// データ出力先のワークスペース取得
	m_ipOutWork = sindy::create_workspace(m_strOutDir);
	if(!m_ipOutWork){
		CLog::GetInstance().PrintLog1(false, true, false, true, err_type::fatal, err_code::NgDBConnect, m_strOutDir);
		return false;
	}

	// 既存データ削除
	if(!deleteShapefile()){
		return false;
	}

	// Shapefile作成
	if(!createShapefile()){
		return false;
	}

	// フィールドインデックス取得
	if(!createFieldIndex()){
		return false;
	}
	CLog::GetInstance().PrintLog(true, false, true, false, getNowTime() + _T(" 初期化終了"), true);

	return true;
}

bool CConvBase::convert()
{
	// 元データ取得
	CLog::GetInstance().PrintLog(true, false, true, false, getNowTime() + _T(" データ取得開始"), true);
	if(!loadData()){
		return false;
	}
	CLog::GetInstance().PrintLog(true, false, true, false, getNowTime() + _T(" データ取得完了"), true);

	// 特殊変換
	if(!convSpecial()){
		return false;
	}

	// Shapefile出力
	CLog::GetInstance().PrintLog(true, false, true, false, getNowTime() + _T(" データ出力開始"), true);
	if(!outputData()){
		return false;
	}
	CLog::GetInstance().PrintLog(true, false, true, false, getNowTime() + _T(" データ出力終了"), true);

	return true;
}

bool CConvBase::convSpecial()
{
	// 基底クラスで特殊変換はしない

	return true;
}

bool CConvBase::createShapefile()
{
	// フィールド作成
	IFieldsPtr ipFields(CLSID_Fields);
	if(!createFields(ipFields)){
		return false;
	}

	if(m_bFeatureClass){
		// フィーチャクラス作成
		IFeatureClassPtr ipFClass;
		if(S_OK != IFeatureWorkspacePtr(m_ipOutWork)->CreateFeatureClass(CComBSTR(m_strShapefileName), ipFields, 0, 0, esriFTSimple, CComBSTR(shp05::schema::kShape), CComBSTR(_T("")), &ipFClass)){
			CLog::GetInstance().PrintLog2(false, true, false, true, err_type::error, err_code::NgCreateTable, m_strShapefileName, _T(""));
			return false;
		}
		m_ipOutShapefile = ipFClass;
	} else {
		// テーブル作成
		if(S_OK != IFeatureWorkspacePtr(m_ipOutWork)->CreateTable(CComBSTR(m_strShapefileName), ipFields, 0, 0, CComBSTR(_T("")), &m_ipOutShapefile)){
			CLog::GetInstance().PrintLog2(false, true, false, true, err_type::error, err_code::NgCreateTable, m_strShapefileName, _T(""));
			return false;
		}
	}

	return true;
}

bool CConvBase::deleteShapefile()
{
	// フィーチャクラスとテーブルのデータセット名を取得
	IEnumDatasetNamePtr ipEnumDatasetName;
	esriDatasetType eDatasetType = m_bFeatureClass ? esriDTFeatureClass : esriDTTable;
	if(S_OK != m_ipOutWork->get_DatasetNames(eDatasetType, &ipEnumDatasetName)){
		CLog::GetInstance().PrintLog1(false, true, false, true, err_type::fatal, err_code::NgGetEnumDatasetName, CString(m_strOutDir));
		return false;
	}
	if(S_OK != ipEnumDatasetName->Reset()){
		CLog::GetInstance().PrintLog1(false, true, false, true, err_type::fatal, err_code::NgResetEnumDatasetName, CString(m_strOutDir));
		return false;
	}
	// データセット名を取得し、インプットの名称と合致するものは削除する
	IDatasetNamePtr ipDatasetName;
	while(S_OK == ipEnumDatasetName->Next(&ipDatasetName) && ipDatasetName){
		CComBSTR bstrName;
		if(S_OK != ipDatasetName->get_Name(&bstrName)){
			CLog::GetInstance().PrintLog1(false, true, false, true, err_type::fatal, err_code::NgGetTableName, CString(m_strOutDir));
			return false;
		}
		CString strName(bstrName);

		if(0 == strName.CompareNoCase(m_strShapefileName)){
			if(!deleteTable(m_ipOutWork, strName))
				return false;
		}
	}

	return true;
}

bool CConvBase::deleteTable(const IWorkspacePtr ipOutdirWork, const CString& strTableName)
{
	ITablePtr ipOutdirTable;
	if(S_OK != IFeatureWorkspacePtr(ipOutdirWork)->OpenTable(CComBSTR(strTableName), &ipOutdirTable)){
		CLog::GetInstance().PrintLog2(false, true, false, true, err_type::error, err_code::NgOpenTable, strTableName, _T(""));
		return false;
	}
	if(S_OK != IDatasetPtr(ipOutdirTable)->Delete()){
		CLog::GetInstance().PrintLog2(false, true, false, true, err_type::error, err_code::NgDeleteTable, strTableName, _T(""));
		return false;
	}
	return true;
}

bool CConvBase::addAttrField(const CString&strTableName, const CString& strFieldName, esriFieldType eFieldType, VARIANT_BOOL vaIsEditable, VARIANT_BOOL vaIsNullable, long lLength, long lPrecision, IFieldsPtr& ipFields)
{
	IFieldEditPtr ipFieldEdit(CLSID_Field);
	if(S_OK != ipFieldEdit->put_Name(CComBSTR(strFieldName))){
		CLog::GetInstance().PrintLog2(false, true, false, true, err_type::error, err_code::NgPutName, strTableName, _T(""), strFieldName);
		return false;
	}
	if(S_OK != ipFieldEdit->put_Type(eFieldType)){
		CLog::GetInstance().PrintLog2(false, true, false, true, err_type::error, err_code::NgPutType, strTableName, _T(""), strFieldName);
		return false;
	}
	if(S_OK != ipFieldEdit->put_Editable(vaIsEditable)){
		CLog::GetInstance().PrintLog2(false, true, false, true, err_type::error, err_code::NgPutEditable, strTableName, _T(""), strFieldName);
		return false;
	}
	if(S_OK != ipFieldEdit->put_IsNullable(vaIsNullable)){
		CLog::GetInstance().PrintLog2(false, true, false, true, err_type::error, err_code::NgPutIsNullable, strTableName, _T(""), strFieldName);
		return false;
	}
	if(0 != lLength){
		if(S_OK != ipFieldEdit->put_Length(lLength)){
			CLog::GetInstance().PrintLog2(false, true, false, true, err_type::error, err_code::NgPutLength, strTableName, _T(""), strFieldName);
			return false;
		}
	}
	if(0 != lPrecision){
		if(S_OK != ipFieldEdit->put_Precision(lPrecision)){
			CLog::GetInstance().PrintLog2(false, true, false, true, err_type::error, err_code::NgPutPrecision, strTableName, _T(""), strFieldName);
			return false;
		}
	}
	if(S_OK != IFieldsEditPtr(ipFields)->AddField(IFieldPtr(ipFieldEdit))){
		CLog::GetInstance().PrintLog2(false, true, false, true, err_type::error, err_code::NgAddField, strTableName, _T(""), strFieldName);
		return false;
	}
	return true;
}

bool CConvBase::addShapeField(const CString&strTableName, esriGeometryType eGeoType, IFieldsPtr& ipFields)
{
	// GeometryDefの作成
	IGeometryDefEditPtr ipGeoDef(CLSID_GeometryDef);
	IUnknownCoordinateSystemPtr ipUnkCoordSys(CLSID_UnknownCoordinateSystem);
	if(S_OK != ipGeoDef->putref_SpatialReference(ipUnkCoordSys)){
		CLog::GetInstance().PrintLog2(false, true, false, true, err_type::error, err_code::NgPutSpRef, strTableName, _T(""));
		return false;
	}
	if(S_OK != ipGeoDef->put_GeometryType(eGeoType)){
		CLog::GetInstance().PrintLog2(false, true, false, true, err_type::error, err_code::NgPutGeoType, strTableName, _T(""));
		return false;
	}

	// 形状フィールドの作成
	IFieldEditPtr ipFieldEdit(CLSID_Field);

	if(S_OK != ipFieldEdit->put_Name(CComBSTR(shp05::schema::kShape))){
		CLog::GetInstance().PrintLog2(false, true, false, true, err_type::error, err_code::NgPutName, strTableName, _T(""), shp05::schema::kShape);
		return false;
	}
	if(S_OK != ipFieldEdit->put_Type(esriFieldTypeGeometry)){
		CLog::GetInstance().PrintLog2(false, true, false, true, err_type::error, err_code::NgPutType, strTableName, _T(""), shp05::schema::kShape);
		return false;
	}
	if(S_OK != ipFieldEdit->put_IsNullable(VARIANT_FALSE)){
		CLog::GetInstance().PrintLog2(false, true, false, true, err_type::error, err_code::NgPutIsNullable, strTableName, _T(""), shp05::schema::kShape);
		return false;
	}
	if(S_OK != ipFieldEdit->putref_GeometryDef(ipGeoDef)){
		CLog::GetInstance().PrintLog2(false, true, false, true, err_type::error, err_code::NgPutIsNullable, strTableName, _T(""), shp05::schema::kShape);
		return false;
	}
	if(S_OK != IFieldsEditPtr(ipFields)->AddField(IFieldPtr(ipFieldEdit))){
		CLog::GetInstance().PrintLog2(false, true, false, true, err_type::error, err_code::NgAddField, strTableName, _T(""), shp05::schema::kShape);
		return false;
	}
	return true;
}

bool CConvBase::setFieldIndex(const CString&strTableName, const ITablePtr& ipTable, std::map<CString,long>& mapFieldIndex)
{
	IFieldsPtr ipFields;
	if(S_OK != ipTable->get_Fields(&ipFields)){
		CLog::GetInstance().PrintLog2(false, true, false, true, err_type::error, err_code::NgGetFields, strTableName, _T(""));
		return false;
	}
	long lFieldCount = 0;
	if(S_OK != ipFields->get_FieldCount(&lFieldCount)){
		CLog::GetInstance().PrintLog2(false, true, false, true, err_type::error, err_code::NgGetFieldCount, strTableName, _T(""));
		return false;
	}
	for(long l = 0; l < lFieldCount; ++l){
		IFieldPtr ipField;
		if(S_OK != ipFields->get_Field(l, &ipField)){
			CLog::GetInstance().PrintLog2(false, true, false, true, err_type::error, err_code::NgGetField, strTableName, _T(""));
			return false;
		}
		CComBSTR bstrName;
		if(S_OK != ipField->get_Name(&bstrName)){
			CLog::GetInstance().PrintLog2(false, true, false, true, err_type::error, err_code::NgGetFieldName, strTableName, _T(""));
			return false;
		}
		mapFieldIndex.insert(std::make_pair(CString(bstrName), l));
	}
	return true;
}
