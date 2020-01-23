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
#include "ConvertShape.h"
#include "CnvShp.h"

bool CConvertShape::Init(const CTransParam& cTransParam)
{
	m_cTransParam = cTransParam;

	// �o�̓t�@�C�����w�肳��Ă���ꍇ�̓p�X�����݂��邩�m�F
	if(!cTransParam.GetFileName().empty()){
		CString strShpPath(cTransParam.GetFileName().c_str());
		long lPos = strShpPath.ReverseFind('\\');
		if(0 > lPos){
			CLog::GetInstance().PrintLog(true, true, false, true, err_type::fatal, err_code::NgOpenFile, CString(m_cTransParam.GetFileName().c_str()));
			return false;
		}
		// �V�F�[�v�t�@�C�������������p�X�����݂��邩�`�F�b�N
		strShpPath = strShpPath.Left(lPos);
		if(!PathIsDirectory(strShpPath)){
			CLog::GetInstance().PrintLog(true, true, false, true, err_type::fatal, err_code::NgOpenFile, CString(m_cTransParam.GetFileName().c_str()));
			return false;
		}
	}

	// ���b�V���p�̏o�̓f�B���N�g�����w�肳��Ă���ꍇ�̓p�X�����݂��邩�m�F
	if(!cTransParam.GetOutDir().empty()){
		if(!PathIsDirectory(CString(m_cTransParam.GetOutDir().c_str()))){
			CLog::GetInstance().PrintLog(true, true, false, true, err_type::fatal, err_code::NgOpenFile, CString(m_cTransParam.GetOutDir().c_str()));
			return false;
		}
	}

	// DB�ڑ�
	CLog::GetInstance().PrintLog(true, false, true, false, _T("�^�[�Q�b�gDB�̎擾�J�n"));
	m_ipTargetWork = sindy::create_workspace(CString(m_cTransParam.GetTaregetDB().c_str()));
	if(!m_ipTargetWork){
		CLog::GetInstance().PrintLog(true, true, false, true, err_type::fatal, err_code::NgOpenWorkspace, CString(m_cTransParam.GetTaregetDB().c_str()));
		return false;
	}

	// �e�[�u���擾
	CLog::GetInstance().PrintLog(true, false, true, false, _T("�^�[�Q�b�g�e�[�u���̎擾�J�n"));
	if(S_OK !=IFeatureWorkspacePtr(m_ipTargetWork)->OpenTable(CComBSTR(CString(m_cTransParam.GetLayer().c_str())), &m_ipTargetTable) || !m_ipTargetTable){
		CLog::GetInstance().PrintLog(true, true, false, true, err_type::fatal, err_code::NgOpenTable, CString(m_cTransParam.GetLayer().c_str()));
		return false;
	}

	// �t�B�[���h�^�C�v�擾
	std::map<CString,esriFieldType> mapOrgFieldType;
	if(!GetFieldTypes(m_ipTargetTable, mapOrgFieldType)){
		CLog::GetInstance().PrintLog(true, true, false, true, err_type::fatal, err_code::NgGetFieldType, CString(m_cTransParam.GetLayer().c_str()));
		return false;
	}

	// �t�B�[���h�����̑Ή��\�̐ݒ�
	if(!m_cFieldAttr.Init(CString(m_cTransParam.GetColList().c_str()), mapOrgFieldType))
		return false;

	// ���b�V���֌W�̏�����
	if(!m_cTransParam.GetMeshDB().empty()){
		CLog::GetInstance().PrintLog(true, false, true, false, _T("���b�V���t�B�[�`���̎擾�J�n"));
		if(!InitMesh())
			return false;
	}
	return true;
}


bool CConvertShape::InitMesh()
{
	// ���b�V�����[�N�X�y�[�X�̎擾
	CString strMeshDB(m_cTransParam.GetMeshDB().c_str());
	m_ipMeshWork = sindy::create_workspace(strMeshDB);
	if(!m_ipMeshWork){
		CLog::GetInstance().PrintLog(true, true, false, true, err_type::fatal, err_code::NgOpenWorkspace, strMeshDB);
		return false;
	}

	// ���b�V�����X�g���烁�b�V���`��擾
	CLog::GetInstance().PrintLog(true, false, true, false, _T("���b�V�����X�g�̎擾�J�n"));
	if(!LoadMeshlist((CString)m_cTransParam.GetMeshlist().c_str())){
		return false;
	}

	return true;
}

bool CConvertShape::LoadMeshlist(const CString& strStrMeshlistPath)
{
	//���b�V�����X�g�i�[�Z�b�g�̏�����
	m_mapMeshGeom.clear();

	//�t�@�C���̓ǂݍ���
	std::ifstream ifstream(static_cast<CT2CW>(strStrMeshlistPath));

	if(!ifstream){
		CLog::GetInstance().PrintLog(true, true, false, true, err_type::fatal, err_code::NgOpenFile, strStrMeshlistPath);
		return false;
	}

	//1�s���ǂݍ���
	char szLine[128];
	long lMeshDigit = 0, lMeshDigitBefore = 0;
	while(ifstream.good()){
		ifstream.getline(szLine,128);
		CString strMeshCode(szLine);
		if(strMeshCode.IsEmpty() || strMeshCode.Left(1) == _T("#"))
			continue;

		strMeshCode.Trim(_T(" "));
		strMeshCode.Trim(_T("�@"));
		lMeshDigit = strMeshCode.GetLength();
		long lMeshcode = _ttol(strMeshCode);

		// ���b�V�����X�g�̃��b�V���R�[�h�̌��͂��ׂē����łȂ���NG
		if(lMeshDigitBefore == 0 || lMeshDigitBefore == lMeshDigit){
			// ���b�V���N���X���擾�i1��,2��,�s�s�n�}�Ƃ��邽�߃t�@�C����ǂݍ��񂾌�łȂ��ƃ��b�V���N���X�����ł��Ȃ��j
			if(!m_ipMeshClass){
				CString strMeshTableName;
				if(!GetMeshTableName(lMeshDigit, strMeshCode, strMeshTableName))
					return false;
				if(S_OK != IFeatureWorkspacePtr(m_ipMeshWork)->OpenFeatureClass(CComBSTR(strMeshTableName), &m_ipMeshClass) || !m_ipMeshClass){
					CLog::GetInstance().PrintLog(true, true, false, true, err_type::error, err_code::NgOpenTable, strMeshTableName);
					return false;
				}
			}
			IGeometryPtr ipMeshGeom;
			if(!GetMeshGeom(lMeshcode, ipMeshGeom))
				return false;

			CString strOutputDir;
			if(!CreateOutputDir(lMeshDigit, strMeshCode, strOutputDir)){
				return false;
			}
			m_mapMeshGeom.insert(std::make_pair(lMeshcode, std::make_pair(strOutputDir,ipMeshGeom)));
		} else {
			CLog::GetInstance().PrintLog(true, true, false, true, err_type::error, err_code::NgMeshCodeDigit, strMeshCode);
			return false;
		}
		lMeshDigitBefore = lMeshDigit;
	}

	return true;
}

bool CConvertShape::GetMeshTableName(long lDigit, const CString& strMeshCode, CString& strMeshTableName)
{
	CString strMeshTable;
	switch(lDigit)
	{
		case 4:
			strMeshTableName.Format( _T("%s"), sindy::schema::middlemesh::kTableName );
			break;
		case 6:
			strMeshTableName.Format( _T("%s"), sindy::schema::basemesh::kTableName );
			break;
		case 8:
			strMeshTableName.Format( _T("%s"), sindy::schema::citymesh::kTableName );
			break;
		default:
			CLog::GetInstance().PrintLog(true, true, false, true, err_type::fatal, err_code::NgMeshCodeDigit, strMeshCode);
			return false;
			break;
	}
	CString strSchema(m_cTransParam.GetMeshSchema().c_str());
	if(!strSchema.IsEmpty()){
		// �X�L�[�}�w�肪����̂͊�{�I��PostgreSQL�̎�
		// �e�[�u�����͏������ł��邽�߁A�������ɂ���
		strMeshTableName = strSchema + _T(".") + strMeshTableName;
		strMeshTableName.MakeLower();
	}
	return true;
}

bool CConvertShape::GetMeshGeom(long lMeshCode, IGeometryPtr& ipMeshGeom)
{
	IQueryFilterPtr ipQFilter(CLSID_QueryFilter);
	CString strWhere;
	strWhere.Format(_T("%s=%ld"), sindy::schema::meshcode_class::kMeshCode, lMeshCode);
	if(S_OK != ipQFilter->put_WhereClause(CComBSTR(strWhere))){
		CLog::GetInstance().PrintLog(true, true, false, true, err_type::error, err_code::NgPutWhere, strWhere, m_ipMeshClass);
		return false;
	}
	IFeatureCursorPtr ipMeshCursor;
	if(S_OK != m_ipMeshClass->Search(ipQFilter, VARIANT_FALSE, &ipMeshCursor) || !ipMeshCursor){
		CLog::GetInstance().PrintLog(true, true, false, true, err_type::error, err_code::NgGetCursor, _T(""), m_ipMeshClass);
		return false;
	}

	IFeaturePtr ipMesh;
	while(S_OK == ipMeshCursor->NextFeature(&ipMesh) && ipMesh){
		if(S_OK != ipMesh->get_ShapeCopy(&ipMeshGeom) || !ipMeshGeom){
			CString strMsg;
			strMsg.Format(_T("meshcode:ld"), lMeshCode);
			CLog::GetInstance().PrintLog(true, true, false, true, err_type::error, err_code::NgGetShape, _T(""), m_ipMeshClass);
			return false;
		}
		break;
	}
	if(ipMeshGeom)
		return true;
	else {
		CString strMsg;
		strMsg.Format(_T("meshcode:ld"), lMeshCode);
		CLog::GetInstance().PrintLog(true, true, false, true, err_type::error, err_code::NgGetShape, strMsg, m_ipMeshClass);
		return false;
	}
}

bool CConvertShape::CreateOutputDir(long lMeshDigit, const CString& strMeshCode, CString& strOutputDir)
{
	// �o�̓f�B���N�g���E�t�@�C�����̍쐬
	CString strMesh1;
	CString strMesh2;
	CString strMesh3;

	strOutputDir = m_cTransParam.GetOutDir().c_str();
	CString strIdentifier(m_cTransParam.GetIdentifier().c_str());
	switch(lMeshDigit)
	{
		case 4:
		{
			strMesh1.Format( _T("%s"), strMeshCode );
			if( strIdentifier.IsEmpty() )
				strOutputDir.AppendFormat( _T("\\%s\\%s"), strMesh1, strMesh1 );
			else
				strOutputDir.AppendFormat( _T("\\%s\\%s_%s"), strMesh1, strMesh1, strIdentifier );
			break;
		}
		case 6:
		{
			strMesh2.Format( _T("%s"), strMeshCode );
			strMesh1 = strMesh2.Left( 4 );
			if( strIdentifier.IsEmpty() )
				strOutputDir.AppendFormat( _T("\\%s\\%s\\%s"), strMesh1, strMesh2, strMesh2 );
			else
				strOutputDir.AppendFormat( _T("\\%s\\%s\\%s_%s"), strMesh1, strMesh2, strMesh2, strIdentifier );
			break;
		}
		case 8:
		{
			strMesh3.Format( _T("%s"), strMeshCode );
			strMesh2 = strMesh3.Left( 6 );
			strMesh1 = strMesh3.Left( 4 );
			if( strIdentifier.IsEmpty() )
				strOutputDir.AppendFormat( _T("\\%s\\%s\\%s\\%s"), strMesh1, strMesh2, strMesh3, strMesh3 );
			else
				strOutputDir.AppendFormat( _T("\\%s\\%s\\%s\\%s_%s"), strMesh1, strMesh2, strMesh3, strMesh3, strIdentifier );
			break;
		}
		default:
			CString strMsg;
			strMsg.Format(_T("���b�V����1��,2��,�s�s�n�}�łȂ�:"), strMeshCode);
			CLog::GetInstance().PrintLog(true, true, false, true, err_type::fatal, err_code::NgCreateDir, strMsg);
			return false;
			break;
	}
	return true;
}

bool CConvertShape::run()
{
	CONVERT_INFO stCnvInfo;
	CString fileName(m_cTransParam.GetFileName().c_str());
	if(m_ipMeshClass){
		if(fileName.IsEmpty()){
			// ���b�V�������o��
			stCnvInfo.m_bBatch = false;
		}
		for(const auto mesh : m_mapMeshGeom){
			if(!stCnvInfo.m_bBatch){
				// ���b�V�������o�͂̏ꍇ�A���b�V�����Ƀt�@�C������ݒ�
				fileName = mesh.second.first;
			}
			CCnvShp cCnvShp(
				m_ipTargetTable,
				m_cFieldAttr.GetListFieldName(),
				m_cFieldAttr.GetColValue(),
				m_cFieldAttr.GetTransValue(),
				m_cFieldAttr.GetDefautlValue(),
				m_cFieldAttr.GetFieldInfo(),
				m_cFieldAttr.GetAttrValue(),
				mesh.second.second.m_T,
				fileName,
				m_cTransParam.GetNoShape(),
				CString(m_cTransParam.GetWhereQuery().c_str()),
				CString(m_cTransParam.GetWhereLoop().c_str()),
				stCnvInfo
				);
			CString strMesh;
			strMesh.Format(_T("%ld"), mesh.first);
			CLog::GetInstance().PrintLog(true, false, true, false, strMesh);

			// �������s
			if(!cCnvShp.Convert()){
				return false;
			}

			if(stCnvInfo.m_bBatch){
				// ���b�V���ꊇ�o�͂̏ꍇ�͏��擾
				cCnvShp.GetConvertInfo(stCnvInfo);
			}
		}
	} else {
		// �ꊇ�o��
		CCnvShp cCnvShp(
			m_ipTargetTable,
			m_cFieldAttr.GetListFieldName(),
			m_cFieldAttr.GetColValue(),
			m_cFieldAttr.GetTransValue(),
			m_cFieldAttr.GetDefautlValue(),
			m_cFieldAttr.GetFieldInfo(),
			m_cFieldAttr.GetAttrValue(),
			NULL,
			fileName,
			m_cTransParam.GetNoShape(),
			CString(m_cTransParam.GetWhereQuery().c_str()),
			CString(m_cTransParam.GetWhereLoop().c_str()),
			stCnvInfo		// �ꊇ�o�͎��͏����ݒ�̂܂܂�OK
			);

		// �������s
		if(!cCnvShp.Convert())
			return false;

		// ���擾
		cCnvShp.GetConvertInfo(stCnvInfo);
	}

	if(stCnvInfo.m_bBatch){
		// �ꊇ�o�͂Ńt�@�C���������Ȃ������ꍇ�A�Ō�Ƀt�@�C�����ύX
		CCnvShp cCnvShp;
		if(!cCnvShp.ShapeNameChange(stCnvInfo.m_lDivideNum, fileName)){
			return false;
		}
	}
	return true;
}
