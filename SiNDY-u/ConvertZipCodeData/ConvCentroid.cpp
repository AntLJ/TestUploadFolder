#include "stdafx.h"
#include "ConvCentroid.h"
#include "Log.h"
#include "Common.h"
#include <RepPointCalculator.h>
#include <AheGlobalsMisc.h>

bool CConvCentroid::convSpecial()
{
	// 重心ポイント作成
	CLog::GetInstance().PrintLog(true, false, true, false, getNowTime() + _T(" 重心ポイント作成開始"), true);
	if(!createCentroid(m_mapMergedZipGeom)){
		return false;
	}
	CLog::GetInstance().PrintLog(true, false, true, false, getNowTime() + _T(" 重心ポイント作成終了"), true);

	return true;
}

bool CConvCentroid::createFieldIndex()
{
	if(!setFieldIndex(m_strShapefileName, m_ipOutShapefile, m_mapFieldIndexOutShapefile)){
		return false;
	}
	return true;
}

bool CConvCentroid::createFields(IFieldsPtr& ipFields)
{
	if(!addAttrField(m_strShapefileName, zip_pol::schema::zip_centroid::kZipCode, esriFieldType::esriFieldTypeString, VARIANT_FALSE, VARIANT_FALSE, zip_pol::schema::zip_centroid::eZipCode, 0, ipFields) ||
		!addShapeField(m_strShapefileName, esriGeometryPoint, ipFields)){
			return false;
	}

	return true;
}

bool CConvCentroid::loadData()
{
	// 元データの読み込みは無し

	return true;
}

bool CConvCentroid::createCentroid(const std::map<CString,IGeometryPtr>& mapMergedZipGeom)
{
	RepPointCalculator repCalculator;
	bool bReturn = true;
	for(const auto& mergedZipGeom : mapMergedZipGeom){
		IPointPtr ipPoint;
		CString strOutRingCount, strPointCount;
		// ArcHelperExの重心算出ライブラリを利用
		try {
			IGeometryPtr ipGeom(AheGetClone(mergedZipGeom.second));
			ipPoint = repCalculator.CalculateRepPoint(ipGeom, rpc::CalcMode::OnSurface);
			if(!ipPoint){
				CLog::GetInstance().PrintLog2(false, true, false, true, err_type::error, err_code::NgGetCentroid, m_strShapefileName, _T("ZIPCODE:") + mergedZipGeom.first);
				bReturn = false;
				continue;
			}
			// 郵便番号の重複はないのでinsertのチェックはなし
			m_mapZipCentorid.insert(std::make_pair(mergedZipGeom.first, ipPoint));

		} catch(...) {
			CLog::GetInstance().PrintLog2(false, true, false, true, err_type::error, err_code::NgGetCentroid2, m_strShapefileName, _T("ZIPCODE:") + mergedZipGeom.first);
			bReturn = false;
		}
	}
	return bReturn;
}

bool CConvCentroid::outputData()
{
	IFeatureCursorPtr ipFCursor;
	if(S_OK != IFeatureClassPtr(m_ipOutShapefile)->Insert(VARIANT_FALSE, &ipFCursor)){
		CLog::GetInstance().PrintLog2(false, true, false, true, err_type::error, err_code::NgInsertCursor, m_strShapefileName, _T(""));
		return false;
	}

	long n = 0;
	for(const auto& zipCentroid : m_mapZipCentorid){
		++n;
		IFeatureBufferPtr ipFeatureBuffer;
		if(S_OK != IFeatureClassPtr(m_ipOutShapefile)->CreateFeatureBuffer(&ipFeatureBuffer)){
			CLog::GetInstance().PrintLog2(false, true, false, true, err_type::error, err_code::NgCreateRowBuffer, m_strShapefileName, _T(""));
			return false;
		}
		if(S_OK != ipFeatureBuffer->put_Value(m_mapFieldIndexOutShapefile[zip_pol::schema::zip_centroid::kZipCode], CComVariant(zipCentroid.first))){
			CLog::GetInstance().PrintLog2(false, true, false, true, err_type::error, err_code::NgPutValue, m_strShapefileName, _T(""));
			return false;
		}
		IGeometryPtr ipGeom(AheGetClone(zipCentroid.second));
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
	if(FAILED(ipFCursor->Flush())){
		CLog::GetInstance().PrintLog2(false, true, false, true, err_type::error, err_code::NgFlush, m_strShapefileName, _T(""));
		return false;
	}

	return true;
}
