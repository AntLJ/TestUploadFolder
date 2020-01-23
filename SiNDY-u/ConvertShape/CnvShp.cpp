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
#include "CnvShp.h"
#include <imagehlp.h>
#include <arctl/sql/expression.h>

#include "ShpMaker.h"

CCnvShp::CCnvShp(void)
{
}

CCnvShp::CCnvShp(
			const ITablePtr& ipTable,
		    const std::list<PAIR_COL>& listFieldName,
			const std::map<ORG_COLNAME,std::set<CString>>& mapFieldValue,
			const std::map<ORG_COLNAME,TRANS_VALUE>& mapTransValue,
			const std::map<DEST_COLNAME,CComVariant>& mapDefaultValue,
			const std::map<DEST_COLNAME,std::map<CString,CString>>& mapFieldInfo,
			const std::map<DEST_COLNAME,std::map<CString,CString>>& mapAttrValue,
			const IGeometryPtr& ipGeom,
			const CString& strOut,
			bool bNoShape,
			const CString& strWhereQuery,
			const CString& strWhereLoop,
			const CONVERT_INFO& stCnvInfo
			)
{
	m_ipTable = ipTable;
	m_ipClipGeom = ipGeom;
	m_strOutFile = strOut;
	m_listFieldName = listFieldName;
	m_mapFieldValue = mapFieldValue;
	m_mapTransValue = mapTransValue;
	m_mapDefaultValue = mapDefaultValue;
	m_mapFieldInfo = mapFieldInfo;
	m_mapAttrValue = mapAttrValue;
	m_strWhereQuery = strWhereQuery;
	m_strWhereLoop = strWhereLoop;
	m_bNoShape = bNoShape;
	m_stCnvInfo = stCnvInfo;
}

CCnvShp::~CCnvShp(void)
{
}

bool CCnvShp::Convert()
{
	if( !m_ipTable )
		return false;

	if( m_strOutFile.IsEmpty() )
		return false;

	CString strMsg;

	IFieldsPtr ipFields;
	m_ipTable->get_Fields( &ipFields );

	// �`�󎝂��̃e�[�u�����ǂ���
	bool bIsGeom = (IFeatureClassPtr)m_ipTable ? true : false;

	// OBJECTID�t�B�[���h�����啶��or�������𔻒f����
	CComBSTR bstrOIDFieldName;
	m_ipTable->get_OIDFieldName(&bstrOIDFieldName);
	CString strOID_Field(bstrOIDFieldName);

	if(!GetFieldNameIndex(m_ipTable, m_mapOrgFieldNameIndex)){
		return false;
	}

	std::map<ORG_COLNAME, long> mapFieldIndex;
	std::list<PAIR_FIELD> listField;
	std::list<CString> listDateType;		// Date�^�͕ϊ��ΏۊO�ɂ���
	for(std::list<PAIR_COL>::const_iterator itName = m_listFieldName.begin(); itName != m_listFieldName.end(); ++itName){
		IFieldPtr ipDstField;

		// �����t�B�[���h�̏ꍇ
		if(0 != itName->first.Left(1).Compare(FIELD_MARK_NEW)){
			// �ϊ����t�B�[���h���̃N���[�����擾
			if(!GetOrgCloneField(m_ipTable, ipFields, itName->first, itName->second, ipDstField))
				return false;
		}

		// �t�B�[���h���ݒ�
		if(!SetFieldInfos(itName->first, itName->second, ipDstField))
			return false;
		
		listField.push_back( PAIR_FIELD( itName->second, (IFieldPtr)ipDstField ) );
	}

	// �t�@�C���T�C�Y�̊Ď��̂���
	long lOneSizeDBF = GetSizeOneRecord( listField );
	long lSizeDBFHeader = 32 * ( listField.size() + 1 );		// DBF�̃w�b�_���́A�\��w�b�_��(32�o�C�g)�ƃt�B�[���h�錾��(�t�B�[���h���~32�o�C�g)
	long lSizeShpHeader = 100;		// Shp�̃w�b�_����100�o�C�g
	long lTotalSizeDBF = lSizeDBFHeader;
	long lTotalSizeShp = lSizeShpHeader;

	// �t�B���^���쐬���Ă���
	ISpatialFilterPtr ipFilter(CLSID_SpatialFilter);
	CString strWhere = GetWhere();
	// Where�傪����Ύw�肷��
	if(!strWhere.IsEmpty()){
		if(S_OK != ipFilter->put_WhereClause(CComBSTR(strWhere))){
			CLog::GetInstance().PrintLog(true, true, false, true, err_type::error, err_code::NgPutWhere, strWhere);
			return false;
		}
	}
	// ��ԃt�B���^�͐�Ɏw��w�肵�Ă����i�t�B�[�`���N���X���e�[�u������SpatialFilter��QueryFilter�ɕ�����K�v�Ȃ��j
	if(m_ipClipGeom){
		if(S_OK != ipFilter->putref_Geometry(m_ipClipGeom)){
			CLog::GetInstance().PrintLog(true, true, false, true, err_type::error, err_code::NgPutRefGeom, _T(""));
			return false;
		}
		if(S_OK != ipFilter->put_SpatialRel(esriSpatialRelIntersects)){
			CLog::GetInstance().PrintLog(true, true, false, true, err_type::error, err_code::NgPutSpRel, _T(""));
			return false;
		}
	}

	// �ȑO��min,max�Ń��[�v�����Ă������A
	// �f�[�^�����Ń��[�v������悤�C��
	long lTableCount = 0, n = 0;
	m_ipTable->RowCount(ipFilter, &lTableCount);

	// �I�v�V�����iwhere_loop�j�Ŏw�肵���t�B�[�`���擾���̃��[�v���Ńt�B���^�����O����ꍇ
	arctl::sql::expression::parse_info<LPCTSTR> aParseInfo = arctl::sql::expression::parse_info<LPCTSTR>(arctl::sql::expression::parse(static_cast<LPCTSTR>(m_strWhereLoop), ipFields));
	if(!m_strWhereLoop.IsEmpty() && !aParseInfo.full){
		CLog::GetInstance().PrintLog(true, true, false, true, err_type::error, err_code::NgPutWhere, m_strWhereLoop);
		return false;
	}

	// �ϊ������f�[�^�����ɂȂ�܂Ń��[�v����
	long lDivideNum = 0;
	long lCurrentOID = 0;

	// �\�[�g�̂��߁A�L�[������ �擪�̑������L�[�ɂȂ�
	std::list<PAIR_COL>::const_iterator itKey = m_listFieldName.cbegin();
	std::list<SHAPE_INFO> listShpInfo;
	CShpMaker cMaker;
	bool bIsNew = true;

	if(m_stCnvInfo.m_bBatch && (m_stCnvInfo.m_lTotalSizeDBF != 0) && (m_stCnvInfo.m_lTotalSizeShp != 0)){
		// �ꊇ�o�͂ŏ��񂶂�Ȃ��ꍇ
		lTotalSizeDBF	= m_stCnvInfo.m_lTotalSizeDBF;
		lTotalSizeShp	= m_stCnvInfo.m_lTotalSizeShp;
		lDivideNum		= m_stCnvInfo.m_lDivideNum;
		bIsNew			= m_stCnvInfo.m_bIsNew;
	}

	CLog::GetInstance().PrintLog(true, false, true, false, _T("\t�`��擾�J�n"));
	while(lTableCount > n){
		long lTempCurrent = lCurrentOID + FETCH_SIZE;

		// ��������������Βǉ�����
		CString strSearchWhere, strWhereBetween;

		// �S���ꊇ�����̏ꍇ�͏������Ŏ擾
		if(!m_ipClipGeom){
			strWhereBetween.Format(_T("(%s BETWEEN %d AND %d)"), strOID_Field, lCurrentOID, lTempCurrent);
			if(!strWhere.IsEmpty())
				strSearchWhere = strWhereBetween + _T(" AND (") + strWhere + _T(")");
			else
				strSearchWhere = strWhereBetween;
		}
		// Where�傪����Ύw�肷��
		if(!strSearchWhere.IsEmpty()){
			if(S_OK != ipFilter->put_WhereClause(CComBSTR(strSearchWhere))){
				CLog::GetInstance().PrintLog(true, true, false, true, err_type::error, err_code::NgPutWhere, strSearchWhere);
				return false;
			}
		}

		// long�^(��{Objectid)���������̂�����ꍇ���l�����Apair���g��
		std::list<std::pair<long, CAdapt<_IRowPtr>>> listTempRow;

		_ICursorPtr ipCursor;
		if(FAILED(m_ipTable->Search(ipFilter, VARIANT_FALSE, &ipCursor))){
			CLog::GetInstance().PrintLog(true, true, false, true, err_type::error, err_code::NgGetCursor, _T(""), m_ipTable);
			return false;
		}

		_IRowPtr ipRow;
		while(SUCCEEDED(ipCursor->NextRow(&ipRow)) && ipRow){
			// �J�E���^�C���N�������g
			n++;
			// �t�B�[�`���擾���ɃI�v�V�����iwhere_query�j�Ŏw�肵���l�Ńt�B���^�����O
			if(aParseInfo.full){
				if(!arctl::sql::expression::evaluate(ipRow, aParseInfo.solution))
					continue;
			}
			long lOID = 0;
			ipRow->get_OID( &lOID );

			listTempRow.push_back(std::make_pair(lOID, ipRow));
		}

		for(std::list<std::pair<long, CAdapt<_IRowPtr>>>::const_iterator itRow = listTempRow.begin(); itRow != listTempRow.end(); ++itRow){
			SHAPE_INFO stShpInfo;
			stShpInfo.m_strSortKey = itKey->second;	// �\�[�g�����邽�߂̃L�[��ݒ肷��

			IGeometryPtr ipOutGeom;
			if( bIsGeom ){	// �`�󂠂�e�[�u���̏ꍇ�͌`��֘A�̏������s��
				IGeometryPtr ipGeom;
				if(S_OK != ((IFeaturePtr)itRow->second.m_T)->get_ShapeCopy(&ipGeom)){
					CLog::GetInstance().PrintLog(true, true, false, true, err_type::error, err_code::NgGetShape, _T(""), itRow->second.m_T);
					return false;
				}

				// ���b�V���`��ŃN���b�v����B�}���`�p�[�g�ł����̂܂܏o�͂���
				if(!GetClipGeom(itRow->second.m_T, ipGeom, m_ipClipGeom, ipOutGeom))
					return false;

				// �W�I���g�����󂩂ǂ����m�F
				VARIANT_BOOL vbool = VARIANT_FALSE;
				if(FAILED(ipOutGeom->get_IsEmpty(&vbool))){
					CLog::GetInstance().PrintLog(true, true, false, true, err_type::error, err_code::NgGetClipShape, _T(""), itRow->second.m_T);
					return false;
				}
				if( VARIANT_TRUE == vbool )
					continue;

				if(m_bNoShape) // �`��o�͂��Ȃ����[�h
					ipOutGeom = NULL;
			}
		
			
			// �t�@�C���T�C�Y���m�F���āA�K��T�C�Y�𒴂����ꍇ��SHAPE�o�͂���
			long lSize = 0;
			if(!GetSizeGeom(itRow->second.m_T, ipOutGeom, lSize)){
				return false;
			}
			lTotalSizeDBF += lOneSizeDBF;
			lTotalSizeShp += lSize;
			if( MAX_FILE_SIZE < lTotalSizeDBF		// DBF�t�@�C���̃T�C�Y�`�F�b�N
				|| MAX_FILE_SIZE < lTotalSizeShp )	// SHP�t�@�C���̃T�C�Y�`�F�b�N(�{���ł���΁A�N���b�v�`��Ŕ�r����K�v�����邪�A�����悻�̑傫���̂��ߌ��`��Ŕ�r)
			{
				// �t�@�C�����ɁA�����ԍ���ǉ����ďo�͂���
				CString strOutFile;
				++lDivideNum;		// �V�����t�@�C�����쐬����ꍇ�͕��������C���N�������g
				strOutFile.Format( _T("%s_%d"), m_strOutFile, lDivideNum );

				if(!MakeDir())		// �f�B���N�g���̍쐬
					return false;

				listShpInfo.sort( less<SHAPE_INFO>() );
				if(!cMaker.MakeShape(true, listField, listShpInfo, strOutFile))
					return false;
				bIsNew = false;		// �V�K�쐬�����̂ŁA�ȍ~�͒ǉ�

				strMsg.Format( _T("\t�o�̓��R�[�h = %d"), listShpInfo.size() );
				CLog::GetInstance().PrintLog(true, false, true, false, strMsg);

				// �V�F�[�v�o�͂������߁A���ׂď�����
				listShpInfo.clear();
				lTotalSizeDBF = lSizeDBFHeader;
				lTotalSizeShp = lSizeShpHeader;
			}

			stShpInfo.m_ipGeom = ipOutGeom;

			for(const auto field_name : m_listFieldName){
				// ���݂���t�B�[���h���ǂ����ŏ����𕪂���
				std::map<CString,long>::const_iterator itOrgIndex = m_mapOrgFieldNameIndex.find(field_name.first);
				if(itOrgIndex == m_mapOrgFieldNameIndex.end()){
					// �����؂�o�����ǂ���
					std::map<DEST_COLNAME,std::map<CString,CString>>::const_iterator itAttr = m_mapAttrValue.find(field_name.second);
					bool bAttr = false;
					if(itAttr != m_mapAttrValue.end()){
						for(const auto attr : itAttr->second){
							arctl::sql::expression::parse_info<LPCTSTR> aParseAttr = arctl::sql::expression::parse_info<LPCTSTR>(arctl::sql::expression::parse(static_cast<LPCTSTR>(attr.first)));
							if(arctl::sql::expression::evaluate(itRow->second.m_T, aParseAttr.solution)){
								bAttr = true;
								stShpInfo.m_mapValue[field_name.second] = attr.second;
								break;
							}
						}
					}
					// �Ȃ���Ώ����l���Ԃ�����
					if(!bAttr){
						stShpInfo.m_mapValue[field_name.second] = m_mapDefaultValue[field_name.second];
					}
				} else {
					// ���݂���t�B�[���h�̏ꍇ�̏���
					CComVariant vaValue;
					if(S_OK != itRow->second->get_Value(itOrgIndex->second, &vaValue)){
						return false;
					}
					if(vaValue.vt != VT_NULL){
						// �ۂߍ��݂��K�v���ǂ����`�F�b�N
						std::map<ORG_COLNAME, TRANS_VALUE>::const_iterator itTransInfo = m_mapTransValue.find(itOrgIndex->first);
						if(itTransInfo != m_mapTransValue.cend()){
							// �ۂߍ��݂���l�ł���΁A�ۂߍ���
							TRANS_VALUE::const_iterator itTrans = itTransInfo->second.find(vaValue);
							if(itTrans != itTransInfo->second.cend())
								stShpInfo.m_mapValue[field_name.second] = CComVariant(itTrans->second);
							else
								stShpInfo.m_mapValue[field_name.second] = vaValue;
						}
						else
							stShpInfo.m_mapValue[field_name.second] = vaValue;
					}
					else
						stShpInfo.m_mapValue[field_name.second] = CComVariant(_T(""));
				}
			}

			listShpInfo.push_back(stShpInfo);
		}
		// 1�񖈃V�F�[�v�o�͂���B�t�@�C���T�C�Y�͊Ď���
		if( !listShpInfo.empty() )
		{
			// �K��T�C�Y�𒴂��Ă��Ȃ��̂ŁA�t�@�C���ɒǉ�����
			CString strOutFile;
			strOutFile.Format( _T("%s_%d"), m_strOutFile, lDivideNum );

			if(bIsNew){
				if(!MakeDir())		// �f�B���N�g���̍쐬
					return false;

				listShpInfo.sort( less<SHAPE_INFO>() );
				if(!cMaker.MakeShape(bIsNew, listField, listShpInfo, strOutFile))
					return false;
				bIsNew = false;		// �V�K�쐬�����̂ŁA�ȍ~�͒ǉ�
			} else {
				if(!m_stCnvInfo.m_bBatch){
					if(!cMaker.AddData(listShpInfo))
						return false;
				}
				else {
					if(!cMaker.MakeShape(bIsNew, listField, listShpInfo, strOutFile))
						return false;
				}
			}

			strMsg.Format( _T("\t�o�̓��R�[�h = %d"), listShpInfo.size() );
			CLog::GetInstance().PrintLog(true, false, true, false, strMsg);

			listShpInfo.clear();
		}

		lCurrentOID = lTempCurrent + 1;
	}

	// ��������0 �����b�V�������̏ꍇ�́A�������Ȃ��̃t�@�C�����ɖ߂�
	if((lDivideNum == 0) && !m_stCnvInfo.m_bBatch )
	{
		cMaker.Clear();
		if(!ShapeNameChange(lDivideNum, m_strOutFile))
			return false;
	}

	// �ϊ��󋵂��X�V
	m_stCnvInfo.m_lTotalSizeDBF	= lTotalSizeDBF;
	m_stCnvInfo.m_lTotalSizeShp	= lTotalSizeShp;
	m_stCnvInfo.m_lDivideNum	= lDivideNum;
	m_stCnvInfo.m_bIsNew		= bIsNew;

	return true;
}

bool CCnvShp::MakeDir()
{
	CString strMsg;

	long lidx = m_strOutFile.ReverseFind('\\');
	if(-1 == lidx){
		CLog::GetInstance().PrintLog(true, true, false, true, err_type::error, err_code::NgOpenFile, m_strOutFile);
		return false;
	}
	CString strOutDir = m_strOutFile.Left(lidx);
	if(PathIsDirectory(strOutDir))
		return true;
	
	if(!MakeSureDirectoryPathExists(CT2CA(strOutDir+_T("\\")))){
		CLog::GetInstance().PrintLog(true, true, false, true, err_type::error, err_code::NgCreateDir, strOutDir);
		return false;
	}

	return true;
}

bool CCnvShp::GetClipGeom(const IFeaturePtr& ipBaseFeature, const IGeometryPtr& ipBaseGeom, const IGeometryPtr& ipClipGeom, IGeometryPtr& ipOutGeom)
{
	// �N���b�v�`�󂪂Ȃ���Ζ�����
	if(!ipClipGeom){
		ipOutGeom = ipBaseGeom;
		return true;
	}

	// �N���b�v�`�󂩂�͂ݏo���Ă��镔������������
	IGeometryPtr ipIntersectGeom;
	esriGeometryDimension resultDim = esriGeometryNoDimension;
	esriGeometryType esriGeomType = esriGeometryPoint;
	if(S_OK != ipBaseGeom->get_GeometryType(&esriGeomType)){
		CLog::GetInstance().PrintLog(true, true, false, true, err_type::error, err_code::NgGetGeomType, _T(""), m_ipTable);
		return false;
	}
	switch(esriGeomType)
	{
		case esriGeometryPoint:
		case esriGeometryMultipoint:
		{
			// �|�C���g�̏ꍇ�́A�ڐG���Ă����OK
			VARIANT_BOOL valBool = VARIANT_TRUE;
			((IRelationalOperatorPtr)ipBaseGeom)->Disjoint(ipClipGeom, &valBool);
			if(VARIANT_FALSE == valBool){
				ipOutGeom = ipBaseGeom;
			}
			return true;
			break;
		}
		case esriGeometryPolyline:
			resultDim = esriGeometry1Dimension;
			break;
		case esriGeometryPolygon:
			resultDim = esriGeometry2Dimension;
			break;
		default:
			CString strMsg;
			CLog::GetInstance().PrintLog(true, true, false, true, err_type::error, err_code::NgGeomType, _T(""), m_ipTable);
			return false;
			break;
	}
	// �}���`�`��ł����̂܂܏o�͂���
	if(FAILED( ((ITopologicalOperatorPtr)ipBaseGeom)->Intersect(ipClipGeom, resultDim, &ipOutGeom))){
		CLog::GetInstance().PrintLog(true, true, false, true, err_type::error, err_code::NgIntersect, _T(""), m_ipTable);
		return false;
	}
	
	return true;
}

bool CCnvShp::MoveShape(const CString& strOrgFile, const CString& strDestFile)
{
	CString strMsg;

	// ����shp�����݂��Ă���ꍇ�͍폜
	static LPCTSTR lpszSuffix[] = {_T("shp"), _T("shx"), _T("dbf")};
	for(int i=0; i < 3; ++i){
		CString strChkPath;
		strChkPath.Format(_T("%s.%s"), strOrgFile, lpszSuffix[i]);
		if(PathFileExists(strChkPath)){
			CString strDestPath;
			strDestPath.Format( _T("%s.%s"), strDestFile, lpszSuffix[i]);
			if(!MoveFileEx(strChkPath, strDestPath, MOVEFILE_REPLACE_EXISTING)){
				CLog::GetInstance().PrintLog(true, true, false, true, err_type::error, err_code::NgMoveFile, strChkPath);
				return false;
			}
		}
	}
	return true;
}

long CCnvShp::GetSizeOneRecord(const std::list<PAIR_FIELD>& listField)
{
	long lSize = 0;
	for( std::list<PAIR_FIELD>::const_iterator it = listField.begin(); it != listField.end(); ++it )
	{
		long lLength = 0;
		it->second.m_T->get_Length( &lLength );
		lSize += ( lLength + 10 );		// �P���ɃT�C�Y�𑫂��������ƁA���T�C�Y�������������߁A10[byte]������
	}
	return lSize;
}

bool CCnvShp::GetSizeGeom(const _IRowPtr& ipRow, const IGeometryPtr& ipGeom, long& lSize)
{
	lSize = 0;

	if(!ipGeom)
		return true;

	esriGeometryType esriType;
	if(S_OK != ipGeom->get_GeometryType(&esriType)){
		CLog::GetInstance().PrintLog(true, true, false, true, err_type::error, err_code::NgGetGeomType, _T(""), ipRow);
		return false;
	}

	long lPointCnt = 0;
	long lGeomCnt = 1;
	IPointCollectionPtr ipPointCol(ipGeom);
	if(ipPointCol){
		if(S_OK != ipPointCol->get_PointCount(&lPointCnt)){
			CLog::GetInstance().PrintLog(true, true, false, true, err_type::error, err_code::NgGetVertexCount, _T(""), ipRow);
			return false;
		}
		IGeometryCollectionPtr ipGeomCol(ipGeom);
		if(ipGeomCol)
			ipGeomCol->get_GeometryCount(&lGeomCnt);
	}

	switch(esriType)
	{
		case esriGeometryPoint:
			// �^�C�v��X,Y
			lSize = 4 + 8 + 8;
			break;
		case esriGeometryMultipoint:
			// �^�C�v�ƃ{�b�N�X�ƃ|�C���g���ƃ|�C���g�f�[�^
			lSize = 4 + ( 8*4 ) + 4 + ( ( 8 + 8 ) * lPointCnt );
			break;
		case esriGeometryPolyline:
		case esriGeometryPolygon:
		{
			// �^�C�v�ƃ{�b�N�X�ƃp�[�g���ƃ|�C���g����1�̃p�[�g�̒����ƃ|�C���g�f�[�^
			lSize = 4 + ( 8*4 ) + 4 + 4 + ( 4*lGeomCnt ) + ( ( 8+8 ) * lPointCnt );
			break;
		}
		default:
			CLog::GetInstance().PrintLog(true, true, false, true, err_type::error, err_code::NgGeomType, _T(""), ipRow);
			return false;
			break;
	}
	lSize += 10;	// �P���ɃT�C�Y�𑫂��������ƁA���T�C�Y�������������߁A10[byte]������
	return true;
}

bool CCnvShp::GetOrgCloneField(ITablePtr ipTable, IFieldsPtr ipFields, const CString& strOrgFieldName, const CString& strDstFieldName, IFieldPtr& ipDstField)
{
	CString strMsg;

	// �t�B�[���h�C���f�b�N�X�̎擾
	long lIdx = -1;
	if(FAILED(ipFields->FindField(CComBSTR(strOrgFieldName), &lIdx))){
		CLog::GetInstance().PrintLog(true, true, false, true, err_type::error, err_code::NgGetFieldIndex, strOrgFieldName, ipTable);
		return false;
	}
	// �t�B�[���h�擾
	IFieldPtr ipField;
	if(FAILED(ipFields->get_Field(lIdx, &ipField))){
		CLog::GetInstance().PrintLog(true, true, false, true, err_type::error, err_code::NgGetField, strOrgFieldName, ipTable);
		return false;
	}

	// Date�^�̏ꍇ�͑ΏۊO�Ƃ���
	esriFieldType esriField = esriFieldTypeInteger;
	if(S_OK != ipField->get_Type(&esriField)){
		CLog::GetInstance().PrintLog(true, true, false, true, err_type::error, err_code::NgGetFieldType, strOrgFieldName, ipTable);
		return false;
	}
	if(esriFieldTypeDate == esriField){
		CLog::GetInstance().PrintLog(true, true, false, true, err_type::error, err_code::NgFieldType, strOrgFieldName, ipTable);
		return false;
	}

	long lPrecision = -1;
	if(S_OK != ipField->get_Precision(&lPrecision)){
		CLog::GetInstance().PrintLog(true, true, false, true, err_type::error, err_code::NgGetFieldPrecision, strOrgFieldName, ipTable);
		return false;
	}

	IClonePtr ipClone(ipField);
	IClonePtr ipCloneField;
	if(S_OK != ipClone->Clone(&ipCloneField) || !ipCloneField){
		CLog::GetInstance().PrintLog(true, true, false, true, err_type::error, err_code::NgCreateClone, strOrgFieldName, ipTable);
		return false;
	}

	// �t�B�[���h����ϊ���p�ɐݒ肷��
	if(S_OK != ((IFieldEditPtr)ipCloneField)->put_AliasName(CComBSTR(strDstFieldName))){
		CLog::GetInstance().PrintLog(true, true, false, true, err_type::error, err_code::NgPutFieldAttr, _T("AliasName"));
		return false;
	}
	if(S_OK != ((IFieldEditPtr)ipCloneField)->put_Name(CComBSTR(strDstFieldName))){
		CLog::GetInstance().PrintLog(true, true, false, true, err_type::error, err_code::NgPutFieldAttr, _T("Name"));
		return false;
	}

	// �t�B�[���h�^�C�v�����l�^�ŁA���x��10�ȏ�̏ꍇ�͐��x�𗎂Ƃ��i���O�o�́j
	if(esriFieldTypeInteger == esriField && lPrecision >= 10){
		CLog::GetInstance().PrintLog(true, true, false, true, err_type::warning, err_code::ChangePrecision, strDstFieldName);
		if(S_OK != ((IFieldEditPtr)ipCloneField)->put_Precision(9)){ // ������10����double�ɕύX����Ă��܂�����
			CLog::GetInstance().PrintLog(true, true, false, true, err_type::error, err_code::NgPutFieldAttr, _T("Precision"));
			return false;
		}
	}
	// [Arc10.3.1�Ή�] Shape�t�B�[���h�����ّΉ�
	//   FGDB�̏ꍇ�A���l�^�̃t�B�[���h�̐��x��0�Ŏ擾����邽�߁AShape�t�@�C���̓f�t�H���g�̐��x�ō쐬����Ă����B
	//   Arc10.3.1����d�l�ύX�ɂ��A���l�^�t�B�[���h�̃f�t�H���g�̐��x��9��10�ɕύX���ꂽ���߁A
	//   Arc10.1�܂łƓ����l�ɂȂ�悤���x��9���w�肷��B
	else if (esriFieldTypeInteger == esriField && lPrecision == 0) {
		if (S_OK != ((IFieldEditPtr)ipCloneField)->put_Precision(9)) {
			CLog::GetInstance().PrintLog(true, true, false, true, err_type::error, err_code::NgPutFieldAttr, _T("Precision"));
			return false;
		}
	}
	// OBJECTID�̎��̂݁A�t�B�[���h�^�C�v��ύX����
	if(0 == strOrgFieldName.CompareNoCase(_T("OBJECTID"))){
		if(S_OK != ((IFieldEditPtr)ipCloneField)->put_Type(esriFieldTypeInteger)){
			CLog::GetInstance().PrintLog(true, true, false, true, err_type::error, err_code::NgPutFieldAttr, _T("Type"));
			return false;
		}
		if(S_OK != ((IFieldEditPtr)ipCloneField)->put_Precision(9)){ // ������10����double�ɕύX����Ă��܂�����
			CLog::GetInstance().PrintLog(true, true, false, true, err_type::error, err_code::NgPutFieldAttr, _T("Precision"));
			return false;
		}
	}

	// �N���[����ԋp�pIFieldPtr�ɃZ�b�g
	ipDstField = IFieldPtr(ipCloneField);
	if(!ipField){
		CLog::GetInstance().PrintLog(true, true, false, true, err_type::error, err_code::NgFieldType, strOrgFieldName, ipTable);
		return false;
	}

	return true;
}

bool CCnvShp::SetFieldInfos(const CString& strOrgFieldName, const CString& strDstFieldName, IFieldPtr& ipDstField)
{
	// �t�B�[���h�ݒ��񂪂Ȃ���ΏI��
	std::map<DEST_COLNAME,std::map<CString,CString>>::const_iterator it = m_mapFieldInfo.find(strDstFieldName);
	if(it == m_mapFieldInfo.end())
		return true;

	// NULL�̏ꍇ�͐V�K�쐬�t�B�[���h�Ƃ݂Ȃ���
	bool bNewField = false;
	if(!ipDstField){
		ipDstField.CreateInstance(CLSID_Field);
		bNewField = true;
	}

	if(bNewField){
		// �V�K�t�B�[���h�̏ꍇ�͂��ׂĐݒ肷��
		std::vector<CString> vecFieldInfo;
		vecFieldInfo.push_back(FIELDINFO_TYPE);
		vecFieldInfo.push_back(FIELDINFO_NAME);
		vecFieldInfo.push_back(FIELDINFO_SCALE);
		vecFieldInfo.push_back(FIELDINFO_PRECISION);
		vecFieldInfo.push_back(FIELDINFO_LENGTH);
		vecFieldInfo.push_back(FIELDINFO_NULLABLE);
		vecFieldInfo.push_back(FIELDINFO_EDITABLE);

		for(const auto info : vecFieldInfo){
			std::map<CString,CString>::const_iterator itFieldInfo = it->second.find(info);
			if(itFieldInfo != it->second.end()){
				if(!SetFieldInfo(strDstFieldName, itFieldInfo->first, itFieldInfo->second, ipDstField))
					return false;
			} else {
				// �ݒ肵�Ȃ��Ƃ����Ȃ��t�B�[���h���܂Ń`�F�b�N���邩�ǂ���
				// ��CFieldAttr�N���X�Ń`�F�b�N���Ă���̂ł��Ȃ��Ă悢
			}
		}

	} else {
		// �����t�B�[���h�̏ꍇ�͎w�肳�ꂽ���̂�����ݒ�
		for(const auto fieldinfo : it->second){
			if(!SetFieldInfo(strDstFieldName, fieldinfo.first, fieldinfo.second, ipDstField))
				return false;
		}
	}

	return true;
}

bool CCnvShp::SetFieldInfo(const CString& strFieldName, const CString& strFieldInfo, const CString& strFieldValue, IFieldPtr& ipDstField)
{
	CString strMsg;

	bool bReturn = true;
	if(0 == strFieldInfo.CompareNoCase(FIELDINFO_TYPE)){
		esriFieldType eFieldType;
		if(0 == strFieldValue.CompareNoCase(FIELDTYPE_LONG)){
			eFieldType = esriFieldTypeInteger;
		} else if(0 == strFieldValue.CompareNoCase(FIELDTYPE_DOUBLE)){
			eFieldType = esriFieldTypeDouble;
		} else if(0 == strFieldValue.CompareNoCase(FIELDTYPE_STRING)){
			eFieldType = esriFieldTypeString;
		}
		if(S_OK != IFieldEditPtr(ipDstField)->put_Type(eFieldType)){
			bReturn = false;
		}
	} else if(0 == strFieldInfo.CompareNoCase(FIELDINFO_NAME)){
		if(S_OK != IFieldEditPtr(ipDstField)->put_Name(CComBSTR(strFieldValue)) ||
			S_OK != IFieldEditPtr(ipDstField)->put_AliasName(CComBSTR(strFieldValue))){
				bReturn = false;
		}
	} else if(0 == strFieldInfo.CompareNoCase(FIELDINFO_SCALE)){
		if(S_OK != IFieldEditPtr(ipDstField)->put_Scale(_ttol(strFieldValue))){
			bReturn = false;
		}
	} else if(0 == strFieldInfo.CompareNoCase(FIELDINFO_PRECISION)){
		if(S_OK != IFieldEditPtr(ipDstField)->put_Precision(_ttol(strFieldValue))){
			bReturn = false;
		}
	} else if(0 == strFieldInfo.CompareNoCase(FIELDINFO_LENGTH)){
		if(S_OK != IFieldEditPtr(ipDstField)->put_Length(_ttol(strFieldValue))){
			bReturn = false;
		}
	} else if(0 == strFieldInfo.CompareNoCase(FIELDINFO_NULLABLE)){
		VARIANT_BOOL vaIsNullable = _ttol(strFieldValue) == 1 ? VARIANT_TRUE : VARIANT_FALSE;
		if(S_OK != IFieldEditPtr(ipDstField)->put_IsNullable(vaIsNullable)){
			bReturn = false;
		}
	} else if(0 == strFieldInfo.CompareNoCase(FIELDINFO_EDITABLE)){
		VARIANT_BOOL vaIsEditable = _ttol(strFieldValue) == 1 ? VARIANT_TRUE : VARIANT_FALSE;
		if(S_OK != IFieldEditPtr(ipDstField)->put_Editable(vaIsEditable)){
			bReturn = false;
		}
	}

	if(!bReturn){
		CLog::GetInstance().PrintLog(true, true, false, true, err_type::error, err_code::NgPutFieldAttr, _T(""));
	}

	return bReturn;
}
CString CCnvShp::GetWhere()
{
	CString strWhere;
	for(const auto field_value : m_mapFieldValue){
		CString strIn;
		for(const auto val : field_value.second)
			strIn.AppendFormat(_T("%s,"), val);
		strIn.TrimRight(_T(","));
		strWhere.AppendFormat(_T("%s IN (%s) AND "), field_value.first, strIn);
	}
	strWhere.TrimRight(_T(" AND "));

	// �I�v�V�����iwhere_query�j�Ŏw�肵��Where�������
	if(!m_strWhereQuery.IsEmpty()){
		strWhere.AppendFormat(_T(" AND %s"), m_strWhereQuery);
		strWhere.TrimLeft(_T(" AND "));
	}
	return strWhere;
}

bool CCnvShp::ShapeNameChange(const long lDivideNum, const CString csOutFile)
{
	// ��������0�̏ꍇ�́A�������Ȃ��̃t�@�C�����ɖ߂�
	if( lDivideNum == 0 )
	{
		CString strOutFile;
		strOutFile.Format( _T("%s_%d"), csOutFile, lDivideNum );

		if(!MoveShape(strOutFile, csOutFile))
			return false;
	}
	return true;
}

void CCnvShp::GetConvertInfo(CONVERT_INFO& stCnvInfo)
{
	// �ϊ��󋵎擾
	stCnvInfo.m_lTotalSizeDBF	= m_stCnvInfo.m_lTotalSizeDBF;
	stCnvInfo.m_lTotalSizeShp	= m_stCnvInfo.m_lTotalSizeShp;
	stCnvInfo.m_lDivideNum		= m_stCnvInfo.m_lDivideNum;
	stCnvInfo.m_bIsNew			= m_stCnvInfo.m_bIsNew;
}
