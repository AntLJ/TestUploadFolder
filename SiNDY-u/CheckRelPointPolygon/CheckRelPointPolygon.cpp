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
#include "Arguments.h"
#include "CheckRelPointPolygon.h"

#include <define.h>
#include <define_enum.h>
#include <GlobalFunctions.h>
#include <AheGlobals.h>
#include <AheLayerFunctions.h>
#include <sindy/workspace/name.h>
#include <VersionInfo.h>
#include <TDC/useful_headers/str_util.h>

using namespace sindy;
using namespace sindy::schema;

// �`�F�b�N�Ώۂ�1000000���ȏ�̏ꍇ�͌x�������o��
const int WARNINGCNT = 1000000;

// �|�C���g�ƃ|���S���̊֌W���`�F�b�N����
bool CCheckRelPointPolygon::Check(const Arguments& args)
{
	if(!Init(args))
		return false;

	// �J�n���̃��O����������
	WriteStartLog(args);

	// ���[�N�X�y�[�X�擾
	IWorkspacePtr ipPointWork, ipPolyWork;
	if(!GetWorkspace(ipPointWork, ipPolyWork))
	{
		WriteEndLog(false);
		return false;
	}

	// �t�B�[�`���N���X�擾
	IFeatureClassPtr ipPointFC, ipPolyFC;
	if(!GetFeatureClass(ipPointWork, ipPolyWork, ipPointFC, ipPolyFC))
	{
		WriteEndLog(false);
		return false;
	}
	
	long pointCnt = -1, polyCnt = -1; // �`�F�b�N�Ώۂ̐�
	if(ipPointFC->FeatureCount(AheInitQueryFilter(NULL, m_PointSubF, m_PointWhere), &pointCnt) == S_OK &&
		ipPolyFC->FeatureCount(AheInitQueryFilter(NULL, m_PolySubF, m_PolyWhere), &polyCnt) == S_OK)
	{
		long allCheckCnt = pointCnt + polyCnt;
		if(allCheckCnt > WARNINGCNT)
		{
			std::cout << "# WARNING\t�`�F�b�N�Ώۂ� " << allCheckCnt << " ������܂�" << std::endl
				<< "# �`�F�b�N�Ɏ��Ԃ�������A�T�[�o�ɂ����S��������܂�" << std::endl;
		}
	}
	else
	{
		m_RunLog << "�`�F�b�N�Ώۂ̃|���S���������̓|�C���g�̌����擾�Ɏ��s���܂���" << std::endl;
		m_RunLog << "�I�v�V�����̐ݒ���m�F���Ă�������" << std::endl;
		WriteEndLog(false);
		return false;
	}

	m_RunLog << "�`�F�b�N�Ώۃ|�C���g�F" << pointCnt << "��" << std::endl
		<< "�`�F�b�N�Ώۃ|���S���F" << polyCnt << "��" << std::endl;

	// �|�C���g�`�F�b�N
	IFeatureCursorPtr ipCur = ipPointFC->_Search(AheInitQueryFilter(NULL, m_PointSubF, m_PointWhere), true);
	IFeaturePtr ipFeature;
	if(ipCur)
	{
		while(ipCur->NextFeature(&ipFeature) == S_OK && ipFeature)
		{
			CheckIntersects(ipFeature, ipPolyFC, _T("�w�肳�ꂽ�|���S����ɂ���܂���"));
		}
	}
	else
	{
		m_RunLog << "�`�F�b�N�Ώۃ|�C���g�̎擾�Ɏ��s���܂���" << std::endl;
		m_RunLog << "�I�v�V�����̐ݒ���m�F���Ă�������" << std::endl;
		WriteEndLog(false);
		return false;
	}
	std::cout << "�|�C���g�`�F�b�N����" << std::endl;
	m_RunLog << "�|�C���g�`�F�b�N����" << std::endl;

	// �|���S���`�F�b�N
	ipCur = ipPolyFC->_Search(AheInitQueryFilter(NULL, m_PolySubF, m_PolyWhere), true);
	if(ipCur)
	{
		while(ipCur->NextFeature(&ipFeature) == S_OK && ipFeature)
		{
			CheckIntersects(ipFeature, ipPointFC, _T("�w�肳�ꂽ�|�C���g���܂�ł��܂���"));
			CheckIntersects(ipFeature, ipPolyFC, _T("���C�����m�ŏd�Ȃ��Ă��܂�"), true);
		}
	}
	else
	{
		m_RunLog << "�`�F�b�N�Ώۃ|���S���̎擾�Ɏ��s���܂���" << std::endl;
		m_RunLog << "�I�v�V�����̐ݒ���m�F���Ă�������" << std::endl;
		WriteEndLog(false);
		return false;
	}
	std::cout << "�|���S���`�F�b�N����" << std::endl;
	m_RunLog << "�|���S���`�F�b�N����" << std::endl;

	WriteEndLog();
	return true;
}

// ������
bool CCheckRelPointPolygon::Init(const Arguments& args)
{
	using namespace std;
	m_PointConnect = args.m_PointConnect;
	m_PolyConnect = args.m_PolyConnect;
	m_Point = args.m_Point;
	m_Poly = args.m_Poly;
	m_PointSubF.Format(_T("%s,SHAPE"), kObjectID);
	m_PointWhere = args.m_PointWhere.c_str();
	m_PolySubF.Format(_T("%s,SHAPE"), kObjectID);
	m_PolyWhere = args.m_PolyWhere.c_str();
	m_RunLog.open(uh::toWStr(args.m_RunLog));
	m_ErrLog.open(uh::toWStr(args.m_ErrLog));

	if(!m_RunLog.is_open())
	{
		cerr << "���s���O�t�@�C�����J���܂���ł���" << endl;
		return false;
	}
	if(!m_ErrLog.is_open())
	{
		cerr << "�G���[���O�t�@�C�����J���܂���ł���" << endl;
		return false;
	}
	
	return true;
}

// ���[�N�X�y�[�X�擾
bool CCheckRelPointPolygon::GetWorkspace(IWorkspacePtr &ipPointWork, IWorkspacePtr &ipPolyWork)
{
	ipPointWork = INamePtr(create_workspace_name(m_PointConnect.c_str()))->_Open();
	if(!ipPointWork)
	{
		m_RunLog << "�|�C���g�̐ڑ���ɐڑ��ł��܂���ł���" << std::endl;
		return false;
	}
	std::cout << uh::toStr(m_PointConnect) << "�ɐڑ����܂���" << std::endl;
	
	ipPolyWork = INamePtr(create_workspace_name(m_PolyConnect.c_str()))->_Open();
	if(!ipPolyWork)
	{
		m_RunLog << "�|���S���̐ڑ���ɐڑ��ł��܂���ł���" << std::endl;
		return false;
	}
	std::cout << uh::toStr(m_PolyConnect) << "�ɐڑ����܂���" << std::endl;

	return true;
}

// �t�B�[�`���N���X�擾
bool CCheckRelPointPolygon::GetFeatureClass(IWorkspacePtr ipPointWork, IWorkspacePtr ipPolyWork, IFeatureClassPtr& ipPointFC, IFeatureClassPtr& ipPolyFC)
{
	IFeatureWorkspacePtr(ipPointWork)->OpenFeatureClass(CComBSTR(m_Point.c_str()), &ipPointFC);

	if(!ipPointFC)
	{
		m_RunLog << "[point]�Ŏw�肳�ꂽ�t�B�[�`���N���X���擾�ł��܂���ł���" << std::endl;
		return false;
	}

	IFeatureWorkspacePtr(ipPolyWork)->OpenFeatureClass(CComBSTR(m_Poly.c_str()), &ipPolyFC);
	if(!ipPolyFC)
	{
		m_RunLog << "[polygon]�Ŏw�肳�ꂽ�t�B�[�`���N���X���擾�ł��܂���ł���" << std::endl;
		return false;
	}
	return true;
}

// �w�肳�ꂽ�t�B�[�`�����w�肳�ꂽ�t�B�[�`���N���X�ɏd�Ȃ��Ă��邩���`�F�b�N����
void CCheckRelPointPolygon::CheckIntersects(IFeaturePtr ipFeature, IFeatureClassPtr ipFeatureClass, const CString& strErrMsg, bool bIsMyself)
{
	long OID = ipFeature->GetOID();
	IGeometryPtr ipGeom = ipFeature->GetShape();
	esriGeometryType type = ipGeom->GetGeometryType();
	IFeatureCursorPtr ipFeatureCur;
	CString strName = AheGetFeatureClassName(ipFeature);

	// ��܂���Ă��邩�m�F(intersects�ƂȂ��Ă��邩)
	LONG cnt = -1;
	switch(type)
	{
	case esriGeometryPoint:
		cnt= AheSelectByShapeAndGetCount(ipGeom, ipFeatureClass, &ipFeatureCur, esriSpatialRelIntersects, m_PolySubF, m_PolyWhere);
		break;
	case esriGeometryPolygon:
		cnt= AheSelectByShapeAndGetCount(ipGeom, ipFeatureClass, &ipFeatureCur, esriSpatialRelIntersects, m_PointSubF, m_PointWhere);
		break;
	default:
		WriteErrLog(strName, OID, _T("�`��������Ȃ��I�u�W�F�N�g������܂�"));
		// fall through
		return;
	}

	// �����C���Ƃ̔�r�̏ꍇ
	if(bIsMyself)
	{
		--cnt; // �������g���܂܂��̂�
		if(cnt > 0)
			WriteErrLog(strName, OID, strErrMsg);
	}
	else if(cnt == 0)
		WriteErrLog(strName, OID, strErrMsg);

}

// �J�n���̃��O����������
void CCheckRelPointPolygon::WriteStartLog(const Arguments& args)
{
	SYSTEMTIME aTime;
	GetLocalTime(&aTime);
	CVersion cVer;
	// ���s���O
	m_RunLog << (LPCSTR)CStringA(cVer.GetInternalName());
	m_RunLog << "\tFILEVERSION:" << (LPCSTR)CStringA(cVer.GetFileVersion());
	m_RunLog << "\tPRODUCTVERSION:" << (LPCSTR)CStringA(cVer.GetProductVersion()) << std::endl;
	m_RunLog << "\n[option]---------------------------------------" << std::endl;
	m_RunLog << "�|�C���g�̐ڑ���\t" << uh::toStr(args.m_PointConnect) << std::endl;
	m_RunLog << "�|���S���̐ڑ���\t" << uh::toStr(args.m_PolyConnect) << std::endl;
	m_RunLog << "�|�C���g\t" << uh::toStr(m_Point) << std::endl;
	m_RunLog << "�|���S��\t" << uh::toStr(m_Poly) << std::endl;
	m_RunLog << "WHERE��(�|�C���g)\t" << CT2A(m_PointWhere) << std::endl;
	m_RunLog << "WHERE��(�|���S��)\t" << CT2A(m_PolyWhere) << std::endl;
	m_RunLog << "-----------------------------------------------\n" << std::endl;

	CString strTime = uh::str_util::ToString(&aTime);
	m_RunLog << "�J�n����\t" << CT2A(strTime) << std::endl;

	// �G���[���O
	m_ErrLog << "# SINDYSTDLOG" << std::endl;
	m_ErrLog << "# --- �G���[�o�� ---" << std::endl;
}

// �G���[���O������
void CCheckRelPointPolygon::WriteErrLog(const CString& strObject, long lOID, const CString& strErrMsg)
{
	m_ErrLog << "0\t" << CT2A(strObject) << "\t" << lOID << "\t\t\t\t\t" << CT2A(strErrMsg) << "\t" << std::endl;
}

// �I�����̃��O����������
void CCheckRelPointPolygon::WriteEndLog(bool success /*= true*/)
{
	SYSTEMTIME aTime;
	GetLocalTime(&aTime);
	CString strTime = uh::str_util::ToString(&aTime);
	m_RunLog << "�I������\t" << CT2A(strTime) << std::endl;
	if(success)
	{
		m_RunLog << "����I��" << std::endl;
		m_ErrLog << "#����I��" << std::endl;
	}
	else
	{
		m_RunLog << "�ُ�I��" << std::endl;
		m_ErrLog << "#�ُ�I��" << std::endl;
	}
}
