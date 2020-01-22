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
#include "OutputString.h"

#define DOUBLE_VALUE(T) "%." #T "f"


bool COutputString::createString(const CConvTableInfo& cConvTableInfo, const _IRowPtr& ipRow, CString& strOutput)
{
	// �o�͕�����̏�����
	strOutput.Empty();

	bool bReturn = true;

	CCheckString cCheckString;

	// �J�������擾
	std::vector<std::vector<CString>> vecShapeString;
	for(const auto& fieldDef : cConvTableInfo.m_vecFieldDef){
		CComVariant vaValue;
		if(S_OK != ipRow->get_Value(fieldDef.m_srcFieldIndex, &vaValue)){
			CLog::GetInstance().PrintLog2(false, true, true, true, err_type::error, err_code::NgGetValue, cConvTableInfo.m_oidTableName, getID(cConvTableInfo, ipRow));
			bReturn = false;
			continue;
		}
	
		CString strValue;
		switch(fieldDef.m_FieldType){
		case FIELDTYPE::SHAPE:
			// �`��f�[�^�̕�����ϊ�
			if(!shape2String(ipRow, cConvTableInfo, vaValue, vecShapeString)){
				bReturn = false;
				continue;
			}
			switch(m_cOutputFormat.m_ShapeType){
			case SHAPETYPE::WKT:
				// WKT�̏ꍇ�͊i�[����vector��1�Ԗڂ݂̂�OK
				strOutput.AppendFormat(_T("%s%s"), vecShapeString[0][0], m_cOutputFormat.m_Delimiter);
				break;
			case SHAPETYPE::NORMAL:
				// �m�[�}���̏ꍇ�͎n�_�̂ݏo�͂�����
				for(const auto& coord : vecShapeString[0]){
					strOutput.AppendFormat(_T("%s%s"), coord, m_cOutputFormat.m_Delimiter);
				}
				break;
			default:
				// �����ɂ͂��Ȃ��͂�
				CLog::GetInstance().PrintLog2(false, true, true, true, err_type::error, err_code::NgShapeType, cConvTableInfo.m_srcTableName, getID(cConvTableInfo, ipRow));
				bReturn = false;
				continue;
				break;
			}
			break;
		default:
			if(fieldDef.m_bFixedValue){
				// �Œ�l���ݒ肳��Ă���ꍇ
				strOutput.AppendFormat(_T("%s%s"), fieldDef.m_FixedValue, m_cOutputFormat.m_Delimiter);
			} else {
				// �`��ȊO�̃f�[�^�𕶎���ϊ�
				if(!value2String(ipRow, cConvTableInfo, fieldDef, vaValue, strValue)){
					bReturn = false;
					continue;
				}
				// �ۂߍ���
				if(!roundValue(fieldDef.m_mapRoundValue, strValue)){
					bReturn = false;
					continue;
				}
				// �f�[�^�`�F�b�N
				if(!cCheckString.checkOutputString(ipRow, cConvTableInfo, fieldDef, strValue)){
					bReturn = false;
					continue;
				}
				// NULL�l�ϊ�
				if(!null2String(ipRow, cConvTableInfo, fieldDef, strValue)){
					bReturn = false;
					continue;
				}
				// �o�͕������A��
				strOutput.AppendFormat(_T("%s%s"), strValue, m_cOutputFormat.m_Delimiter);
			}
			break;
		}
	}

	// �Ō�̃f���~�^���폜
	strOutput = strOutput.Left(strOutput.GetLength() - 1);
	strOutput.AppendFormat(_T("%s"), m_cOutputFormat.m_Newline);
	// �m�[�}���̏ꍇ�A�n�_�ȍ~�̃|�C���g�o��
	if(SHAPETYPE::NORMAL == m_cOutputFormat.m_ShapeType && esriGeometryType::esriGeometryNull != m_geomType){
		long size = vecShapeString.size();
		if(2 <= size){
			for(long l = 1; l < size; ++l){
				for(const auto& coord : vecShapeString[l]){
					strOutput.AppendFormat(_T("%s%s"), coord, m_cOutputFormat.m_Delimiter);
				}
				strOutput.TrimRight(m_cOutputFormat.m_Delimiter);
				strOutput.AppendFormat(_T("%s"), m_cOutputFormat.m_Newline);
			}
		}
	}
	return bReturn;
}
bool COutputString::createHeader(const CConvTableInfo& cConvTableInfo, bool isZ, bool isM, CString& strHeader)
{
	strHeader = _T("#");

	for(const auto& fieldDef : cConvTableInfo.m_vecFieldDef){
		// �`��t�B�[���h�̏ꍇ�͓���
		if(FIELDTYPE::SHAPE == fieldDef.m_FieldType){
			switch(m_cOutputFormat.m_ShapeType){
			case SHAPETYPE::NORMAL:
				strHeader.AppendFormat(_T("LON%sLAT%s"), m_cOutputFormat.m_Delimiter, m_cOutputFormat.m_Delimiter);
				if(isZ){
					strHeader.AppendFormat(_T("HEIGHT%s"), m_cOutputFormat.m_Delimiter);
				}
				if(isM){
					strHeader.AppendFormat(_T("MEASURE%s"), m_cOutputFormat.m_Delimiter);
				}
				break;
			case SHAPETYPE::WKT:
				strHeader.AppendFormat(_T("SHAPE%s"), m_cOutputFormat.m_Delimiter);
				break;
			default:
				// �����ɂ͂��Ȃ��͂�
				CLog::GetInstance().PrintLog1(false, true, true, true, err_type::error, err_code::NgShapeType, cConvTableInfo.m_srcTableName);
				return false;
			}
		} else {
			strHeader.AppendFormat(_T("%s%s"), fieldDef.m_dstFieldName, m_cOutputFormat.m_Delimiter);
		}
	}
	// �Ō�̃f���~�^�폜
	if(!strHeader.IsEmpty()){
		strHeader = strHeader.Left(strHeader.GetLength() - 1);
	}
	strHeader.AppendFormat(_T("%s"), m_cOutputFormat.m_Newline);

	return true;
}

bool COutputString::value2String(const _IRowPtr& ipRow, const CConvTableInfo& cConvTableInfo, const CFieldDefInfo& cFieldDefInfo, const CComVariant& vaValue, CString& strColumnString)
{
	switch(vaValue.vt){
	case VT_I2:
		if(!numVal2String(cConvTableInfo, cFieldDefInfo, vaValue.iVal, ipRow, strColumnString)){
			return false;
		}
		break;
	case VT_I4:
		if(!numVal2String(cConvTableInfo, cFieldDefInfo, vaValue.lVal, ipRow, strColumnString)){
			return false;
		}
		break;
	case VT_INT:
		if(!numVal2String(cConvTableInfo, cFieldDefInfo, vaValue.intVal, ipRow, strColumnString)){
			return false;
		}
		break;
	case VT_R4:
		if(!doubleVal2String(cConvTableInfo, cFieldDefInfo, vaValue.fltVal, ipRow, strColumnString)){
			return false;
		}
		break;
	case VT_R8:
		if(!doubleVal2String(cConvTableInfo, cFieldDefInfo, vaValue.dblVal, ipRow, strColumnString)){
			return false;
		}
		break;
	case VT_BSTR:
		{
			// UTF-16�i���C�h������j�Ŏ󂯎��
			CStringW strTmp(vaValue.bstrVal);

			// T�^�ɕϊ��iUNICODE�r���h�Ȃ�UTF-16�i���C�h������j�A�}���`�o�C�g�r���h�Ȃ�SHIFT_JIS�i�}���`�o�C�g������j�j
			if(!str2String(cConvTableInfo, cFieldDefInfo, strTmp, ipRow, strColumnString)){
				return false;
			}
			break;
		}
	case VT_NULL:
	case VT_EMPTY:
		strColumnString.Empty();
		break;
	default:
		// �z��O�̌^�̏ꍇ�̓G���[
		CLog::GetInstance().PrintLog2(false, true, true, true, err_type::error, err_code::NgDataType, cConvTableInfo.m_oidTableName, getID(cConvTableInfo, ipRow));
		return false;
		break;
	}

	return true;
}

bool COutputString::shape2String(const _IRowPtr& ipRow, const CConvTableInfo& cConvTableInfo, const CComVariant& vaValue, std::vector<std::vector<CString>>& vecShapeString)
{
	// �o�̓f�[�^������
	vecShapeString.clear();

	switch(m_geomType){
	case esriGeometryPoint:
		// �|�C���g�`�󕶎���
		if(!point2String(ipRow, cConvTableInfo, vaValue, vecShapeString)){
			return false;
		}
		break;
	case esriGeometryPolyline:
		// �|�����C���`�󕶎���
		if(!polyline2String(ipRow, cConvTableInfo, vaValue, vecShapeString)){
			return false;
		}
		break;
	case esriGeometryPolygon:
		// �|���S���`�󕶎���
		if(!polygon2String(ipRow, cConvTableInfo, vaValue, vecShapeString)){
			return false;
		}
		break;
	default:
		// �z��O�̃W�I���g���̏ꍇ�̓G���[
		CLog::GetInstance().PrintLog2(false, true, true, true, err_type::error, err_code::NgGeomType, cConvTableInfo.m_oidTableName, getID(cConvTableInfo, ipRow));
		return false;
		break;
	}

	return true;
}

bool COutputString::point2String(const _IRowPtr& ipRow, const CConvTableInfo& cConvTableInfo, const CComVariant& vaValue, std::vector<std::vector<CString>>& vecShapeString)
{
	// �o�̓f�[�^������
	vecShapeString.clear();

	// �|�C���g�ɕϊ�
	IPointPtr ipPoint(vaValue);
	if(!ipPoint){
		CLog::GetInstance().PrintLog2(false, true, true, true, err_type::error, err_code::NgQueryInterface, cConvTableInfo.m_oidTableName, getID(cConvTableInfo, ipRow), _T("CComVariant"), _T("IPoint"));
		return false;
	}
	// �|�C���g�f�[�^����ܓx�o�x�i�{ZM�j�𕶎���ϊ�
	std::vector<CString> vecCoordString;
	if(!point2CoordString(ipRow, cConvTableInfo, IPointPtr(vaValue), vecCoordString)){
		return false;
	}

	// �o�̓t�H�[�}�b�g�ɍ��킹�Č`�󕶎���쐬
	switch(m_cOutputFormat.m_ShapeType){
	case SHAPETYPE::NORMAL:
		vecShapeString.push_back(vecCoordString);
		break;
	case SHAPETYPE::WKT:
		{
			CString strWKTPoint;
			if(!normal2WKTString(ipRow, cConvTableInfo, vecCoordString, strWKTPoint)){
				return false;
			}
			strWKTPoint = _T("POINT") + setGeomTypeWKT(strWKTPoint);
			std::vector<CString> vecTmpString;
			vecTmpString.push_back(strWKTPoint);
			vecShapeString.push_back(vecTmpString);
			break;
		}
	default:
		// �����ɂ͂��Ȃ��͂�
		CLog::GetInstance().PrintLog2(false, true, true, true, err_type::error, err_code::NgShapeType, cConvTableInfo.m_oidTableName, getID(cConvTableInfo, ipRow));
		return false;
		break;
	}
	return true;
}

bool COutputString::polyline2String(const _IRowPtr& ipRow, const CConvTableInfo& cConvTableInfo, const CComVariant& vaValue, std::vector<std::vector<CString>>& vecShapeString)
{
	// �o�̓f�[�^������
	vecShapeString.clear();

	// �|�C���g�R���N�V�����ɕϊ�
	IPointCollectionPtr ipPointCol(vaValue);
	if(!ipPointCol){
		CLog::GetInstance().PrintLog2(false, true, true, true, err_type::error, err_code::NgQueryInterface, cConvTableInfo.m_oidTableName, getID(cConvTableInfo, ipRow), _T("CComVariant"), _T("IPointCollection"));
		return false;
	}
	// �\���_���擾
	long size = 0;
	if(S_OK != ipPointCol->get_PointCount(&size) || 1 >= size){
		CLog::GetInstance().PrintLog2(false, true, true, true, err_type::error, err_code::NgGetPointCount, cConvTableInfo.m_oidTableName, getID(cConvTableInfo, ipRow));
		return false;
	}

	// �e�\���_���擾
	std::vector<std::vector<CString>> vecPolylineString;
	for(long l = 0; l < size; ++l){
		IPointPtr ipPoint;
		if(S_OK != ipPointCol->get_Point(l, &ipPoint) || !ipPoint){
			CLog::GetInstance().PrintLog2(false, true, true, true, err_type::error, err_code::NgGetPoint, cConvTableInfo.m_oidTableName, getID(cConvTableInfo, ipRow));
			return false;
		}
		// �擾�����|�C���g����ܓx�o�x�i�{ZM�l�j�擾
		std::vector<CString> vecCoordString;
		if(!point2CoordString(ipRow, cConvTableInfo, ipPoint, vecCoordString)){
			return false;
		}
		vecPolylineString.push_back(vecCoordString);
	}

	// �o�̓t�H�[�}�b�g�ɍ��킹�Č`�󕶎���쐬
	switch(m_cOutputFormat.m_ShapeType){
	case SHAPETYPE::NORMAL:
		vecShapeString.swap(vecPolylineString);
		break;
	case SHAPETYPE::WKT:
		{
			CString strWKTPolyline;
			for(const auto& vecCoordString : vecPolylineString){
				CString strWKTPoint;
				if(!normal2WKTString(ipRow, cConvTableInfo, vecCoordString, strWKTPoint)){
					return false;
				}
				strWKTPolyline.AppendFormat(_T("%s,"), strWKTPoint);
			}
			strWKTPolyline.TrimRight(_T(","));
			strWKTPolyline = _T("MULTILINESTRING") + setGeomTypeWKT(strWKTPolyline);
			std::vector<CString> vecTmpString;
			vecTmpString.push_back(strWKTPolyline);
			vecShapeString.push_back(vecTmpString);
			break;
		}
	default:
		// �����ɂ͂��Ȃ��͂�
		CLog::GetInstance().PrintLog2(false, true, true, true, err_type::error, err_code::NgShapeType, cConvTableInfo.m_oidTableName, getID(cConvTableInfo, ipRow));
		return false;
		break;
	}
	return true;
}
typedef std::vector<std::vector<CString>> hoges;

bool COutputString::polygon2String(const _IRowPtr& ipRow, const CConvTableInfo& cConvTableInfo, const CComVariant& vaValue, std::vector<std::vector<CString>>& vecShapeString)
{
	// TODO:�|���S���Ɋւ��Ă�IPolygon��^�Ƃ��ď��������Ă��邽�߁A�O�����������O�̎擾������Ɉ��������Ă���B
	//      ���������邩�ǂ����̓|���S���̏o�͂��K�v�ɂȂ������_�ōl����B

	// �o�̓f�[�^������
	vecShapeString.clear();

	CString strWKTPolygon;

	IPolygon3Ptr ipPolygon(vaValue);
	if(!ipPolygon){
		CLog::GetInstance().PrintLog2(false, true, true, true, err_type::error, err_code::NgQueryInterface, cConvTableInfo.m_oidTableName, getID(cConvTableInfo, ipRow), _T("CComVariant"), _T("IPolygon"));
		return false;
	}

	// �O�����O�擾
	long exRingCount = 0;
	if(S_OK != ipPolygon->get_ExteriorRingCount(&exRingCount) || 0 >= exRingCount){
		CLog::GetInstance().PrintLog2(false, true, true, true, err_type::error, err_code::NgGetExteriorRingCount, cConvTableInfo.m_oidTableName, getID(cConvTableInfo, ipRow));
		return false;
	}
	std::vector<IRingPtr> vecExRing(exRingCount);
	if(S_OK != ipPolygon->QueryExteriorRingsEx(exRingCount, &vecExRing[0])){
		CLog::GetInstance().PrintLog2(false, true, true, true, err_type::error, err_code::NgQueryExteriorRings, cConvTableInfo.m_oidTableName, getID(cConvTableInfo, ipRow));
		return false;
	}

	// �O�����O�Ń��[�v
	for(const auto& exRing : vecExRing){
		// �O�����O�̌`��𕶎���ϊ�
		std::vector<std::vector<CString>> vecExRingPoint;
		if(!ring2vector(ipRow, cConvTableInfo, exRing, vecExRingPoint)){
			return false;
		}

		// �O�����O�ɂ���������O���擾
		long inRingCount = 0;
		std::vector<std::vector<std::vector<CString>>> vecInRingPoints;
		if(S_OK != ipPolygon->get_InteriorRingCount(exRing, &inRingCount)){
			CLog::GetInstance().PrintLog2(false, true, true, true, err_type::error, err_code::NgGetInteriorRingCount, cConvTableInfo.m_oidTableName, getID(cConvTableInfo, ipRow));
			return false;
		}
		if(0 < inRingCount){
			std::vector<IRingPtr> vecInRing(inRingCount);
			if(S_OK != ipPolygon->QueryInteriorRingsEx(exRing, inRingCount, &vecInRing[0])){
				CLog::GetInstance().PrintLog2(false, true, true, true, err_type::error, err_code::NgQueryInteriorRings, cConvTableInfo.m_oidTableName, getID(cConvTableInfo, ipRow));
				return false;
			}

			// �������O�Ń��[�v
			for(const auto& inRing : vecInRing){
				// �������O�̌`��𕶎���ϊ�
				std::vector<std::vector<CString>> vecInRingPoint;
				if(!ring2vector(ipRow, cConvTableInfo, inRing, vecInRingPoint)){
					return false;
				}
				vecInRingPoints.push_back(vecInRingPoint);
			}
		}

		// �o�̓t�H�[�}�b�g�ɍ��킹�Č`�󕶎���쐬
		switch(m_cOutputFormat.m_ShapeType){
		case SHAPETYPE::NORMAL:
			for(const auto& exRing : vecExRingPoint){
				vecShapeString.push_back(exRing);
			}
			for(const auto& vecInRingPoint : vecInRingPoints){
				for(const auto& inRing : vecInRingPoint){
					vecShapeString.push_back(inRing);
				}
			}
			break;
		case SHAPETYPE::WKT:
			{
				CString strPolygon;
				for(const auto& vecCoordString : vecExRingPoint){
					CString strWKTPoint;
					if(!normal2WKTString(ipRow, cConvTableInfo, vecCoordString, strWKTPoint)){
						return false;
					}
					strPolygon.AppendFormat(_T("%s,"), strWKTPoint);
				}
				strPolygon.TrimRight(_T(","));
				strPolygon = _T("(") + strPolygon + _T("),");

				for(const auto& vecInRingPoint : vecInRingPoints){
					CString strInRing;
					for(const auto& vecCoordString : vecInRingPoint){
						CString strWKTPoint;
						if(!normal2WKTString(ipRow, cConvTableInfo, vecCoordString, strWKTPoint)){
							return false;
						}
						strInRing.AppendFormat(_T("%s,"), strWKTPoint);
					}
					strInRing.TrimRight(_T(","));
					strPolygon.AppendFormat(_T("(%s),"), strInRing);
				}
				strPolygon.TrimRight(_T(","));
				strWKTPolygon.AppendFormat(_T("(%s),"), strPolygon);
				break;
			}
		default:
			// �����ɂ͂��Ȃ��͂�
			CLog::GetInstance().PrintLog2(false, true, true, true, err_type::error, err_code::NgShapeType, cConvTableInfo.m_oidTableName, getID(cConvTableInfo, ipRow));
			return false;
			break;
		}
	}
	switch(m_cOutputFormat.m_ShapeType){
	case SHAPETYPE::WKT:
		{
			strWKTPolygon.TrimRight(_T(","));
			if(1 == exRingCount){
				strWKTPolygon = strWKTPolygon.Left(strWKTPolygon.GetLength() - 1);
				strWKTPolygon = strWKTPolygon.Right(strWKTPolygon.GetLength() - 1);
			}
			strWKTPolygon = _T("MULTIPOLYGON") + setGeomTypeWKT(strWKTPolygon);
			std::vector<CString> vecTmpString;
			vecTmpString.push_back(strWKTPolygon);
			vecShapeString.push_back(vecTmpString);
			break;
		}
	default:
		break;
	}

	return true;
}
bool COutputString::ring2vector(const _IRowPtr& ipRow, const CConvTableInfo& cConvTableInfo, const IRingPtr& ipRing, std::vector<std::vector<CString>>& vecRing)
{
	// �����O���|�C���g�R���N�V������
	IPointCollectionPtr ipPointCol(ipRing);
	if(!ipPointCol){
		CLog::GetInstance().PrintLog2(false, true, true, true, err_type::error, err_code::NgQueryInterface, cConvTableInfo.m_oidTableName, getID(cConvTableInfo, ipRow), _T("IRing"), _T("IPointCollection"));
		return false;
	}
	// �����O�̍\���_���擾
	long size = 0;
	if(S_OK != ipPointCol->get_PointCount(&size) || 3 >= size){
		CLog::GetInstance().PrintLog2(false, true, true, true, err_type::error, err_code::NgGetPointCount, cConvTableInfo.m_oidTableName, getID(cConvTableInfo, ipRow));
		return false;
	}
	// �e�\���_�̈ܓx�o�x�i�{ZM�l�j�擾
	for(long l = 0; l < size; ++l){
		IPointPtr ipPoint;
		if(S_OK != ipPointCol->get_Point(l, &ipPoint) || !ipPoint){
			CLog::GetInstance().PrintLog2(false, true, true, true, err_type::error, err_code::NgGetPoint, cConvTableInfo.m_oidTableName, getID(cConvTableInfo, ipRow));
			return false;
		}
		std::vector<CString> vecCoordString;
		if(!point2CoordString(ipRow, cConvTableInfo, ipPoint, vecCoordString)){
			return false;
		}
		vecRing.push_back(vecCoordString);
	}
	return true;
}

bool COutputString::point2CoordString(const _IRowPtr& ipRow, const CConvTableInfo& cConvTableInfo, const IPointPtr& ipPoint, std::vector<CString>& vecCoordStr)
{
	vecCoordStr.clear();

	double srcX = 0.0, srcY = 0.0, srcZ = 0.0, srcM = 0.0;
	
	CString strCoord;
	if(S_OK != ipPoint->QueryCoords(&srcX, &srcY)){
		CLog::GetInstance().PrintLog2(false, true, true, true, err_type::error, err_code::NgQueryCoord, cConvTableInfo.m_oidTableName, getID(cConvTableInfo, ipRow));
		return false;	
	}

	double x = 0.0, y = 0.0;
	switch(m_cOutputFormat.m_GeoRef){
	case GEOREF::JGD2000:
	case GEOREF::JGD2011:
		if(0 == m_cnv.TOKYOtoJGD2000_RP(srcX,srcY,&x,&y,true)){
			CLog::GetInstance().PrintLog2(false, true, true, true, err_type::error, err_code::NgTOKYOtoJGD2000_RP, cConvTableInfo.m_oidTableName, getID(cConvTableInfo, ipRow));
			return false;
		}
		break;
	default:
		x = srcX;
		y = srcY;
		break;
	}
	
	strCoord.Format(COORD_DEC_X, x);
	if(strCoord.Find(_T(".")) > COORD_NUM_X){
		CLog::GetInstance().PrintLog2(false, true, true, true, err_type::error, err_code::NgCoordIntegerDigitOver, cConvTableInfo.m_oidTableName, getID(cConvTableInfo, ipRow), _T("X"), COORD_NUM_X, strCoord);
		return false;
	}
	vecCoordStr.push_back(strCoord);

	strCoord.Format(COORD_DEC_Y, y);
	if(strCoord.Find(_T(".")) > COORD_NUM_Y){
		CLog::GetInstance().PrintLog2(false, true, true, true, err_type::error, err_code::NgCoordIntegerDigitOver, cConvTableInfo.m_oidTableName, getID(cConvTableInfo, ipRow), _T("Y"), COORD_NUM_Y, strCoord);
		return false;
	}
	vecCoordStr.push_back(strCoord);

	if(m_isZ){
		if(S_OK != ipPoint->get_Z(&srcZ)){
			CLog::GetInstance().PrintLog2(false, true, true, true, err_type::error, err_code::NgGetZ, cConvTableInfo.m_oidTableName, getID(cConvTableInfo, ipRow));
			return false;
		}
		strCoord.Format(COORD_DEC_Z, srcZ);
		if(strCoord.Find(_T(".")) > COORD_NUM_Z){
			CLog::GetInstance().PrintLog2(false, true, true, true, err_type::error, err_code::NgCoordIntegerDigitOver, cConvTableInfo.m_oidTableName, getID(cConvTableInfo, ipRow), _T("Z"), COORD_NUM_Z, strCoord);
			return false;
		}
		vecCoordStr.push_back(strCoord);
	}
	if(m_isM){
		if(S_OK != ipPoint->get_M(&srcM)){
			CLog::GetInstance().PrintLog2(false, true, true, true, err_type::error, err_code::NgGetM, cConvTableInfo.m_oidTableName, getID(cConvTableInfo, ipRow));
			return false;
		}
		strCoord.Format(COORD_DEC_M, srcM);
		if(strCoord.Find(_T(".")) > COORD_NUM_M){
			CLog::GetInstance().PrintLog2(false, true, true, true, err_type::error, err_code::NgCoordIntegerDigitOver, cConvTableInfo.m_oidTableName, getID(cConvTableInfo, ipRow), _T("M"), COORD_NUM_M, strCoord);
			return false;
		}
		vecCoordStr.push_back(strCoord);
	}

	return true;
}


bool COutputString::normal2WKTString(const _IRowPtr& ipRow, const CConvTableInfo& cConvTableInfo, const std::vector<CString>& vecCoordStr, CString& strWKTPoint)
{
	// �o�̓f�[�^�̏�����
	strWKTPoint.Empty();

	// �|�C���g�f�[�^�Ƃ��Ă�XY�͕K�{�̂���2���R�[�h�͊i�[����Ă���͂�
	if(2 > vecCoordStr.size()){
		CLog::GetInstance().PrintLog2(false, true, true, true, err_type::error, err_code::NgPointCoord, cConvTableInfo.m_oidTableName, getID(cConvTableInfo, ipRow));
		return false;
	}

	for(const auto& coord : vecCoordStr){
		strWKTPoint.AppendFormat(_T("%s "), coord);
	}
	strWKTPoint.TrimRight();

	return true;
}

CString COutputString::setGeomTypeWKT(const CString& strWKTGeomType)
{
	// WKT�̌`���`��ZM��ǉ����邩�ǂ���
	CString strWKT = _T(" ");
	if(m_isZ){
		strWKT += _T("Z");
	}
	if(m_isM){
		strWKT += _T("M");
	}
	if(strWKT.Right(1) != _T(" ")){
		strWKT += _T(" ");
	}
	strWKT += _T("(") + strWKTGeomType + _T(")");
	
	return strWKT;
}

bool COutputString::roundValue(const std::map<CString,CString>& mapRoundValue, CString& strValue)
{
	const auto& it = mapRoundValue.find(strValue);
	if(mapRoundValue.cend() != it){
		strValue = it->second;
	}
	return true;
}

bool COutputString::null2String(const _IRowPtr& ipRow, const CConvTableInfo& cConvTableInfo, const CFieldDefInfo& cFieldDef, CString& strValue)
{
	// NULL�l�ݒ肳��Ă��Ȃ���΃X���[
	if(!cFieldDef.m_bNullValue)
		return true;

	// NULL�l�łȂ��Ȃ�X���[
	if(!strValue.IsEmpty())
		return true;

	strValue = cFieldDef.m_NullValue;

	return true;
}

bool COutputString::numVal2String(const CConvTableInfo& cConvTableInfo, const CFieldDefInfo& cFieldDefInfo, long lValue, const _IRowPtr& ipRow, CString& strColumnString)
{
	switch(cFieldDefInfo.m_FieldType){
	case FIELDTYPE::INTEGER:
	case FIELDTYPE::STRING:
		strColumnString.Format(_T("%ld"), lValue);
		break;
	case FIELDTYPE::DOUBLE:
		{
			double dTmp = lValue;
			CString strTmp;
			strTmp.Format(_T("%%.%ldf"), cFieldDefInfo.m_Decimal);
			strColumnString.Format(strTmp, dTmp);
		}
		break;
	default:
		// �����ɂ͂��Ȃ��͂�
		CLog::GetInstance().PrintLog2(false, true, true, true, err_type::error, err_code::NgCheckFieldType, cConvTableInfo.m_oidTableName, getID(cConvTableInfo, ipRow), cFieldDefInfo.m_srcTableName, cFieldDefInfo.m_srcFieldName, _T(""));
		return false;
	}
	return true;
}

bool COutputString::doubleVal2String(const CConvTableInfo& cConvTableInfo, const CFieldDefInfo& cFieldDefInfo, double dValue, const _IRowPtr& ipRow, CString& strColumnString)
{
	switch(cFieldDefInfo.m_FieldType){
	case FIELDTYPE::INTEGER:
		// INTEGER�ւ̕ϊ��Ȃ珬���_�ȉ����폜���邾��
		strColumnString.Format(_T("%.0f"), dValue);
		break;
	case FIELDTYPE::STRING:
		// �����_�ȉ������̎w�肪�ł��Ȃ����A���̂܂܏o�͂�����B�i�\�t�g�E�F�A�J�������ɒ��ӓ_�Ƃ��ċL���ς݁j
		strColumnString.Format(_T("%f"), dValue);
		break;
	case FIELDTYPE::DOUBLE:
		{
			CString strTmp;
			strTmp.Format(_T("%%.%ldf"), cFieldDefInfo.m_Decimal);
			strColumnString.Format(strTmp, dValue);
			break;
		}
		break;
	default:
		// �����ɂ͂��Ȃ��͂�
		CLog::GetInstance().PrintLog2(false, true, true, true, err_type::error, err_code::NgCheckFieldType, cConvTableInfo.m_oidTableName, getID(cConvTableInfo, ipRow), cFieldDefInfo.m_srcTableName, cFieldDefInfo.m_srcFieldName, _T(""));
		return false;
	}
	// �Ō�Ɂu-0�v�u-0.000�v�̂悤�ȃP�[�X���u0�v�u0.000�v�Ƃ����鏈��
	if(0 == strColumnString.Compare(strColumnString.SpanIncluding(_T("-.0")))){
		strColumnString.TrimLeft(_T("-"));
	}
	return true;
}

bool COutputString::str2String(const CConvTableInfo& cConvTableInfo, const CFieldDefInfo& cFieldDefInfo, CStringW& strValue, const _IRowPtr& ipRow, CString& strColumnString)
{
	switch(cFieldDefInfo.m_FieldType){
	case FIELDTYPE::INTEGER:
		{
			// �����I�ɕϊ��i�\�t�g�E�F�A�J�������ɒ��ӓ_�Ƃ��ċL���ς݁j
			long lTmp = _ttol(strValue);
			strColumnString.Format(_T("%ld"), lTmp);
			if(0 == strColumnString.Compare(_T("-0"))){
				strColumnString = _T("0");
			}
		}
		break;
	case FIELDTYPE::STRING:
		strColumnString = strValue;
		break;
	case FIELDTYPE::DOUBLE:
		{
			// �����I�ɕϊ��i�\�t�g�E�F�A�J�������ɒ��ӓ_�Ƃ��ċL���ς݁j
			double dTmp = _ttof(strValue);
			CString strTmp;
			strTmp.Format(_T("%%.%ldf"), cFieldDefInfo.m_Decimal);
			strColumnString.Format(strTmp, dTmp);
			if(0 == strColumnString.Compare(strColumnString.SpanIncluding(_T("-.0")))){
				strColumnString.TrimLeft(_T("-"));
			}
		}
		break;
	default:
		// �����ɂ͂��Ȃ��͂�
		CLog::GetInstance().PrintLog2(false, true, true, true, err_type::error, err_code::NgCheckFieldType, cConvTableInfo.m_oidTableName, getID(cConvTableInfo, ipRow), cFieldDefInfo.m_srcTableName, cFieldDefInfo.m_srcFieldName, _T(""));
		return false;
	}
	return true;
}