#include "stdafx.h"
#include "ConvZipPol.h"
#include "Log.h"
#include "Common.h"
#include <AheGlobalsMisc.h>

bool CConvZipPol::convSpecial()
{
	// �`��}�[�W
	CLog::GetInstance().PrintLog(true, false, true, false, getNowTime() + _T(" �`��}�[�W�J�n"), true);
	if(!mergeZipGeom(m_mapZipGeom, m_mapMergedZipGeom)){
		return false;
	}
	CLog::GetInstance().PrintLog(true, false, true, false, getNowTime() + _T(" �`��}�[�W�I��"), true);

	// �D��x�ݒ�
	CLog::GetInstance().PrintLog(true, false, true, false, getNowTime() + _T(" �D��x�ݒ�J�n"), true);
	if(!createZipCodePriority(m_mapZipPriority)){
		return false;
	}
	CLog::GetInstance().PrintLog(true, false, true, false, getNowTime() + _T(" �D��x�ݒ�I��"), true);

	return true;
}

bool CConvZipPol::createFieldIndex()
{
	if(!setFieldIndex(sindy::schema::zipcode_area::kTableName, m_ipZipAreaClass, m_mapFieldIndexZipArea) ||
		!setFieldIndex(m_strShapefileName, m_ipOutShapefile, m_mapFieldIndexOutShapefile)){
			return false;
	}
	return true;
}

bool CConvZipPol::createFields(IFieldsPtr& ipFields)
{
	if(!addAttrField(m_strShapefileName, zip_pol::schema::zip_pol::kZipCode, esriFieldType::esriFieldTypeString, VARIANT_FALSE, VARIANT_FALSE, zip_pol::schema::zip_pol::eZipCode, 0, ipFields) ||
		!addAttrField(m_strShapefileName, zip_pol::schema::zip_pol::kPriority, esriFieldType::esriFieldTypeInteger, VARIANT_FALSE, VARIANT_FALSE, 0, zip_pol::schema::zip_pol::ePriority, ipFields) ||
		!addShapeField(m_strShapefileName, esriGeometryPolygon, ipFields)){
			return false;
	}

	return true;
}

bool CConvZipPol::loadData()
{
	m_mapZipGeom.clear();

	// �擾�Ώۂ͑S���R�[�h
	IFeatureCursorPtr ipFCursor;
	if(S_OK != m_ipZipAreaClass->Search(NULL, VARIANT_FALSE, &ipFCursor)){
		CLog::GetInstance().PrintLog2(false, true, false, true, err_type::error, err_code::NgGetCursor, sindy::schema::zipcode_area::kTableName, _T(""));
		return false;
	}
	bool bReturn = true;
	IFeaturePtr ipFeature;
	while(S_OK == ipFCursor->NextFeature(&ipFeature) && ipFeature){
		// ZIPCODE1,ZIPCODE2,�`����擾
		CComVariant vaZipCode1, vaZipCode2;
		if(S_OK != ipFeature->get_Value(m_mapFieldIndexZipArea[sindy::schema::zipcode_area::kZipCode1], &vaZipCode1) ||
			S_OK != ipFeature->get_Value(m_mapFieldIndexZipArea[sindy::schema::zipcode_area::kZipCode2], &vaZipCode2)){
				CLog::GetInstance().PrintLog2(false, true, false, true, err_type::error, err_code::NgGetValue, sindy::schema::zipcode_area::kTableName, getID(ipFeature));
				bReturn = false;
				continue;
		}
		// �����R�[�h���i�[����Ă���ꍇ��NG�i��܂ށj
		if(vaZipCode1 == vaZipCode2){
			CLog::GetInstance().PrintLog2(false, true, false, true, err_type::error, err_code::NgSameZipCode, sindy::schema::zipcode_area::kTableName, getID(ipFeature));
			bReturn = false;
			continue;
		}

		IGeometryPtr ipGeom;
		if(S_OK != ipFeature->get_ShapeCopy(&ipGeom)){
			CLog::GetInstance().PrintLog2(false, true, false, true, err_type::error, err_code::NgGetShape, sindy::schema::zipcode_area::kTableName, getID(ipFeature));
			bReturn = false;
			continue;
		}
		long lOID = 0;
		if(S_OK != ipFeature->get_OID(&lOID)){
			CLog::GetInstance().PrintLog2(false, true, false, true, err_type::error, err_code::NgGetOID, sindy::schema::zipcode_area::kTableName, _T(""));
			bReturn = false;
			continue;
		}

		// OID�̏d���͂Ȃ��̂�insert�̃`�F�b�N�͂��܂���
		m_mapZipGeom[CString(vaZipCode1.bstrVal)].insert(std::make_pair(lOID, ipGeom));

		// ZIPCODE2����łȂ���Βǉ�����
		if(VT_EMPTY != vaZipCode2.vt && VT_NULL != vaZipCode2.vt){
			m_mapZipGeom[CString(vaZipCode2.bstrVal)].insert(std::make_pair(lOID, ipGeom));
		}
	}

	return bReturn;
}

bool CConvZipPol::mergeZipGeom(std::map<CString,std::map<long,IGeometryPtr>>& mapZipGeom, std::map<CString,IGeometryPtr>& mapMergedZipGeom)
{
	mapMergedZipGeom.clear();

	bool bReturn = true;
	for(auto& zipGeoms : mapZipGeom){
		bool bUnion = true;
		ITopologicalOperatorPtr ipTopo;
		for(auto& zipGeom : zipGeoms.second){
			IGeometryPtr ipGeom(AheGetClone(zipGeom.second));
			if(ipTopo){
				IGeometryPtr ipUnionGeom;
				if(S_OK != ipTopo->Union(ipGeom, &ipUnionGeom)){
					CLog::GetInstance().PrintLog2(false, true, false, true, err_type::error, err_code::NgUnion, sindy::schema::zipcode_area::kTableName, std::to_string(zipGeom.first).c_str());
					bUnion = false;
					break;
				}
				ipTopo = ipUnionGeom;
			} else {
				ipTopo = ipGeom;
			}
			if(!ipTopo){
				CLog::GetInstance().PrintLog2(false, true, false, true, err_type::error, err_code::NgQueryInterface, sindy::schema::zipcode_area::kTableName, std::to_string(zipGeom.first).c_str());
				bUnion = false;
				break;
			}
		}
		// mapMergedZipGeom�ɂ��i�[����ƃ�������H���̂�
		// �g�p�ς݂�mapZipGeom�v�f�͉��������
		zipGeoms.second.clear();

		if(!bUnion){
			bReturn = false;
			continue;
		}

		IGeometryPtr ipMergedGeom(ipTopo);
		if(!ipMergedGeom){
			CLog::GetInstance().PrintLog2(false, true, false, true, err_type::error, err_code::NgUnion, sindy::schema::zipcode_area::kTableName, _T("ZIPCODE:") + zipGeoms.first);
			bReturn = false;
			continue;
		}

		mapMergedZipGeom.insert(std::make_pair(zipGeoms.first, ipMergedGeom));
	}

	return bReturn;
}

bool CConvZipPol::createZipCodePriority(std::map<CString,long>& mapZipPriority)
{
	m_mapZipPriority.clear();

	// �D��x�ݒ�p�̗X�֔ԍ����X�g�Əd�Ȃ�`�F�b�N�p�̗X�֔ԍ����X�g���擾
	std::map<CString,std::set<CString>> mapOverlapZip;
	if(!getZipCodeList(mapZipPriority, mapOverlapZip)){
		return false;
	}

	// �D��x�ݒ���s
	return updatePriority(mapZipPriority, mapOverlapZip);
}

bool CConvZipPol::getZipCodeList(std::map<CString,long>& mapZipPriority, std::map<CString,std::set<CString>>& mapOverlapZip)
{
	mapZipPriority.clear();
	
	bool bReturn = true;

	// ZIPCODE_AREA�̃J�[�\���擾
	IFeatureCursorPtr ipFCursor;
	if(S_OK != m_ipZipAreaClass->Search(NULL, VARIANT_FALSE, &ipFCursor)){
		CLog::GetInstance().PrintLog2(false, true, false, true, err_type::error, err_code::NgGetCursor, sindy::schema::zipcode_area::kTableName, _T(""));
		return false;
	}

	IFeaturePtr ipFeature;
	std::map<CString,std::set<CString>> mapNgOverlapZip;
	while(S_OK == ipFCursor->NextFeature(&ipFeature) && ipFeature){
		// ZIPCODE1,ZIPCODE2�̒l�擾
		CComVariant vaZipCode1, vaZipCode2;
		if(S_OK != ipFeature->get_Value(m_mapFieldIndexZipArea[sindy::schema::zipcode_area::kZipCode1], &vaZipCode1) ||
			S_OK != ipFeature->get_Value(m_mapFieldIndexZipArea[sindy::schema::zipcode_area::kZipCode2], &vaZipCode2)){
				CLog::GetInstance().PrintLog2(false, true, false, true, err_type::error, err_code::NgGetValue, sindy::schema::zipcode_area::kTableName, getID(ipFeature));
				bReturn = false;
				continue;
		}
		// ZIPCODE2��NULL�Ŗ����ꍇ�͏d�Ȃ�`�F�b�N�p�̃f�[�^�m��
		if(vaZipCode2.vt != VT_EMPTY && vaZipCode2.vt != VT_NULL){
			// �d�Ȃ�`�F�b�N�p�Ɋm��
			mapOverlapZip[vaZipCode1.bstrVal].insert(vaZipCode2.bstrVal);
			// �D��x�ݒ�p�Ɋm��
			mapZipPriority.insert(std::make_pair(vaZipCode2.bstrVal,1));
		}
		// ZIPCODE1�͗D��x�ݒ�p�̃R���e�i�ɂ����f�[�^�m��
		mapZipPriority.insert(std::make_pair(vaZipCode1.bstrVal,1));
	}

	return bReturn;
}
bool CConvZipPol::updatePriority(std::map<CString,long>& mapZipPriority, const std::map<CString,std::set<CString>>& mapOverlapZip)
{
	// ���͂Ƃ��Č`��d�����Ă���X�֔ԍ��iZIPCODE_AREA�̃��R�[�h��ZIPCODE1��ZIPCODE2���ݒ肳��Ă�����́j
	bool bReturn = true;
	for(const auto& overlapZip : mapOverlapZip){
		// ���[�v�`�F�b�N�p�̃R���e�i
		std::set<CString> setCheckedZip;
		// ��͍ċA�I�ɗD��x��ݒ�
		if(!setPriority(mapZipPriority, mapOverlapZip, overlapZip.first, overlapZip.second, setCheckedZip)){
			bReturn = false;
		}
	}
	return bReturn;
}

bool CConvZipPol::setPriority(std::map<CString,long>& mapZipPriority, const std::map<CString,std::set<CString>>& mapOverlapZip, const CString &strZip, const std::set<CString>& setZip, std::set<CString> setCheckedZip)
{
	// ���[�v�`�F�b�N�p�̃R���e�i�ɍ���X�V�Ώۂ̗X�֔ԍ����i�[
	setCheckedZip.insert(strZip);

	for(const auto& opZip : setZip){
		const auto& checkedZip = setCheckedZip.find(opZip);
		if(setCheckedZip.cend() != checkedZip){
			// �z���Ă�̂ŃG���[
			CLog::GetInstance().PrintLog2(false, true, false, true, err_type::error, err_code::NgOverlapZipValue, sindy::schema::zipcode_area::kTableName, _T(""), strZip, opZip);
			return false;
		}
		// �D��x��ݒ�
		if(mapZipPriority[strZip] >= mapZipPriority[opZip]){
			mapZipPriority[opZip] = mapZipPriority[strZip] + 1;
		}
		// �[�_�łȂ��Ȃ玟��T��
		const auto& nextOverlapZip = mapOverlapZip.find(opZip);
		if(mapOverlapZip.cend() != nextOverlapZip){
			if(!setPriority(mapZipPriority, mapOverlapZip, nextOverlapZip->first, nextOverlapZip->second, setCheckedZip)){
				return false;
			}
		}
	}
	return true;
}

bool CConvZipPol::outputData()
{
	IFeatureCursorPtr ipFCursor;
	if(S_OK != IFeatureClassPtr(m_ipOutShapefile)->Insert(VARIANT_FALSE, &ipFCursor)){
		CLog::GetInstance().PrintLog2(false, true, false, true, err_type::error, err_code::NgInsertCursor, m_strShapefileName, _T(""));
		return false;
	}

	long n = 0;
	for(const auto& zipMergedGeom : m_mapMergedZipGeom){
		++n;
		IFeatureBufferPtr ipFeatureBuffer;
		if(S_OK != IFeatureClassPtr(m_ipOutShapefile)->CreateFeatureBuffer(&ipFeatureBuffer)){
			CLog::GetInstance().PrintLog2(false, true, false, true, err_type::error, err_code::NgCreateRowBuffer, m_strShapefileName, _T(""));
			return false;
		}
		if(S_OK != ipFeatureBuffer->put_Value(m_mapFieldIndexOutShapefile[zip_pol::schema::zip_pol::kZipCode], CComVariant(zipMergedGeom.first))){
			CLog::GetInstance().PrintLog2(false, true, false, true, err_type::error, err_code::NgPutValue, m_strShapefileName, _T(""));
			return false;
		}
		const auto& zipPriority = m_mapZipPriority.find(zipMergedGeom.first);
		if(m_mapZipPriority.cend() == zipPriority){
			CLog::GetInstance().PrintLog2(false, true, false, true, err_type::error, err_code::NgZipPriority, m_strShapefileName, _T(""), zipMergedGeom.first);
			continue;
		}
		if(S_OK != ipFeatureBuffer->put_Value(m_mapFieldIndexOutShapefile[zip_pol::schema::zip_pol::kPriority], CComVariant(zipPriority->second))){
			CLog::GetInstance().PrintLog2(false, true, false, true, err_type::error, err_code::NgPutValue, m_strShapefileName, _T(""));
			return false;
		}
		IGeometryPtr ipGeom(AheGetClone(zipMergedGeom.second));
		if(S_OK != ipFeatureBuffer->putref_Shape(ipGeom)){
			CLog::GetInstance().PrintLog2(false, true, false, true, err_type::error, err_code::NgPutShape, m_strShapefileName, _T(""));
			return false;
		}

		CComVariant vaOID;
		if(S_OK != ipFCursor->InsertFeature(ipFeatureBuffer, &vaOID)){
			CLog::GetInstance().PrintLog2(false, true, false, true, err_type::error, err_code::NgInsertRow, m_strShapefileName, _T(""));
			return false;
		}
		// Flush�͂��܂߂�
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
