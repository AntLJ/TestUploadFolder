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

#include "StdAfx.h"
#include "CreateSmartICList.h"
#include <sindy/workspace.h>
#include <sindy/schema/road.h>
#include <sindy/schema/reference.h>

CSmartICList::CSmartICList(void)
{
}

CSmartICList::~CSmartICList(void)
{
}

CSmartICList::CSmartICList(CString strConfigPath)
{
	//�e������
	CReadConfig cConfig;

	//�p�����[�^�̎擾
	m_eParams = cConfig.getenv(strConfigPath);

	//�o�̓��O�̐ݒ�
	openfs(m_eParams.m_strOut);

	//���[�N�X�y�[�X�̃I�[�v��
	const IWorkspacePtr	ipRoadWorksapce(INamePtr(sindy::create_workspace_name(m_eParams.m_strNwDB))->_Open());

	//�t�B�[�`���N���X�Q�̎擾
	getfc(ipRoadWorksapce);

	//�t�B�[���h�C���f�b�N�X�̐ݒ�
	gerfiledindex();
}

//------------------�����ݒ肱������--------------------------------

//���O�t�@�C���̃I�[�v��
void CSmartICList::openfs(CString strOutPath)
{
	CString strLinkofs, strNodeofs;
	strLinkofs = strOutPath + _T("\\smartic_link.txt");
	strNodeofs = strOutPath + _T("\\smartic_node.txt");

	m_linkofs.open( static_cast<CT2CW>(strLinkofs) );
	m_nodeofs.open( static_cast<CT2CW>(strNodeofs) );

	if(! ( m_linkofs.is_open() && m_nodeofs.is_open() ))
	{
		throw std::runtime_error(_T("���O�t�@�C���̃I�[�v���Ɏ��s."));
	}

	//header�o��
	setlinkheader();
	setnodeheader();
}

//�w�b�_�̏o��
void CSmartICList::setlinkheader()
{
	m_linkofs << _T("#�X�}�[�gIC�����N�t�H�[�}�b�g [smartic_link.txt]	Ver.1.0") << std::endl;
	m_linkofs << _T('#') << std::endl;
	m_linkofs << _T("#���e�F") << std::endl;
	m_linkofs << _T("#	�X�}�[�gIC�����N�̃��X�g.") << std::endl;
	m_linkofs << _T("#�t�H�[�}�b�g�F") << std::endl;
	m_linkofs << _T("#	�S��1�t�@�C��(SJIS)�Ƃ���B�擪'#'�̓R�����g�s�Ƃ���B") << std::endl;
	m_linkofs << _T("#	�ȉ��A1�`2�̃��R�[�h��<TAB>�ŋ�؂�P�s��1�f�[�^�Ƃ���.") << std::endl;
	m_linkofs << _T("#	1.�Q�����b�V���R�[�h	�F�Ώۃ����N�̑�����Q�����b�V���R�[�h���L�q.") << std::endl;
	m_linkofs << _T("#	2.�I�u�W�F�N�gID	�F<�Q�����b�V���R�[�h>_RLK.dbf ��OBJECTID�t�B�[���h�̑����l���L�q.") << std::endl;
	m_linkofs << _T('#') << std::endl;
}

//�w�b�_�̏o��
void CSmartICList::setnodeheader()
{
	m_nodeofs << _T("#�X�}�[�gIC�m�[�h�t�H�[�}�b�g [smartic_node.txt]	Ver.1.0") << std::endl;
	m_nodeofs << _T('#') << std::endl;
	m_nodeofs << _T("#���e�F") << std::endl;
	m_nodeofs << _T("#	�X�}�[�gIC�����N[smartic_link.txt]�Ԉ�����ɐ�����A�����m�[�h�̃��X�g.") << std::endl;
	m_nodeofs << _T("#�t�H�[�}�b�g�F") << std::endl;
	m_nodeofs << _T("#	�S��1�t�@�C��(SJIS)�Ƃ���B�擪'#'�̓R�����g�s�Ƃ���.") << std::endl;
	m_nodeofs << _T("#	�ȉ��A1�`2�̃��R�[�h��<TAB>�ŋ�؂�P�s��1�f�[�^�Ƃ���.") << std::endl;
	m_nodeofs << _T("#	1.�Q�����b�V���R�[�h	�F�Ώۃm�[�h�̑�����Q�����b�V���R�[�h���L�q.") << std::endl;
	m_nodeofs << _T("#	2.�I�u�W�F�N�gID	�F<�Q�����b�V���R�[�h>_RND.dbf ��OBJECTID�t�B�[���h�̑����l���L�q.") << std::endl;
	m_nodeofs << _T('#') << std::endl;
}

//�t�B�[�`���N���X�̎擾
void CSmartICList::getfc(IWorkspacePtr ipNwWorkspace)
{
	IFeatureWorkspacePtr ipNwFW(ipNwWorkspace);
	m_NwFC.ipRoadLinkFC		= ipNwFW->_OpenFeatureClass(::sindy::schema::road_link::kTableName);
	m_NwFC.ipRoadNodeFC		= ipNwFW->_OpenFeatureClass(::sindy::schema::road_node::kTableName);
	m_NwFC.ipBaseMeshFC		= ipNwFW->_OpenFeatureClass(_T("REFERENCE.BaseMesh"));	
}

//�t�B�[���h�C���f�b�N�X�̎擾
void CSmartICList::gerfiledindex()
{
	m_FieldIndex.lFromID	= m_NwFC.ipRoadLinkFC->_FindField(::sindy::schema::road_link::kFromNodeID);
	m_FieldIndex.lToID		= m_NwFC.ipRoadLinkFC->_FindField(::sindy::schema::road_link::kToNodeID);
	m_FieldIndex.lLinkOID	= m_NwFC.ipRoadLinkFC->_FindField(_T("OBJECTID"));

	m_FieldIndex.lMeshcode	= m_NwFC.ipBaseMeshFC->_FindField(::sindy::schema::basemesh::kMeshCode);
}

//------------------�����ݒ肱���܂�--------------------------------

//�����̎��s
void CSmartICList::Execute()
{
	//UPDOWNCLASS_C��5�̃����N��OID������MESHCODE ���擾�A�o�́B
	//FROM_NODE_ID, TO_NODE_ID,���擾����
	std::cerr << m_eParams.m_strOut << _T("\\smartic_link.txt");
	CreateLinkList();
	std::cerr << _T("\tOK") << std::endl;

	//�X�}�[�gIC�����N���X�g�Ԉ�����ɕ����m�[�h�ɂȂ郊�X�g�̍쐬
	std::cerr << m_eParams.m_strOut << _T("\\smartic_node.txt");
	CreateFloatNodeList();
	std::cerr << _T("\tOK") << std::endl;
}

//�X�}�[�g�����N���X�g�̍쐬
void CSmartICList::CreateLinkList()
{
	//�o�̓f�[�^���^�p
	llSET llOutSet;

	IFeatureCursorPtr ipFCr = m_cDBManager.GetFeatureCursor(m_NwFC.ipRoadLinkFC, _T("UPDOWNCLASS_C = 5"));

	IFeaturePtr ipFeature;

	while(S_OK == ipFCr->NextFeature(&ipFeature))
	{
		//OID�̎擾
		long lOID = m_cDBManager.GetOID(ipFeature);

		//�o�͗p���X�g�̍쐬
		CreateList(lOID, GetMeshCode(m_cDBManager.GetGeom(ipFeature), esriSpatialRelWithin), llOutSet);

		//���̊i�[
		m_SmartICLinkSet.insert(lOID);
		m_SmartICNodeSet.insert(m_cDBManager.GetValue(ipFeature, m_FieldIndex.lFromID).lVal);
		m_SmartICNodeSet.insert(m_cDBManager.GetValue(ipFeature, m_FieldIndex.lToID).lVal);
	}
	//���X�g�o��
	Output(llOutSet, m_linkofs);
}

/*
m_SmartICNodeSet�Ɋi�[����Ă���m�[�h�ɐڑ����Ă��郊���N���擾���A
m_SmartICLinkSet�Ɋi�[����Ă��郊���N�����ڑ�����Ă��Ȃ������ꍇ�͕����m�[�h�ƂȂ�
*/
void CSmartICList::CreateFloatNodeList()
{	
	//�o�̓f�[�^���^�p
	llSET llOutSet;

	//�m�[�h��1�I�u�W�F�N�g���`�F�b�N
	for(ConstItr aIter = m_SmartICNodeSet.begin(); aIter != m_SmartICNodeSet.end(); ++aIter)
	{
		//�m�[�h�̌`����擾����
		IGeometryPtr ipGeom = m_cDBManager.GetGeomFromOID(*aIter, m_NwFC.ipRoadNodeFC);

		//�m�[�h�`��ɐڐG���Ă��郊���NOID�̎擾
		vaSET vaTouchLinkIDSet = m_cDBManager.GetHitValSet(m_NwFC.ipRoadLinkFC, ipGeom, m_FieldIndex.lLinkOID, esriSpatialRelTouches);
		
		//�Y���m�[�h��SmartIC�����N�����ڑ����Ă��Ȃ��ꍇ�͕����m�[�h�ɂȂ�
		if(IsFloatNode(vaTouchLinkIDSet))
		{
			CreateList(*aIter, GetMeshCode(ipGeom, esriSpatialRelIntersects), llOutSet);
		}
	}
	//���X�g�o��
	Output(llOutSet, m_nodeofs);
}

//SmartIC�����N�Ԉ����㕂���m�[�h�ɂȂ邩�ǂ����̃`�F�b�N
bool CSmartICList::IsFloatNode(const vaSET&  vaTouchLinkIDSet)
{
	for(ConstVaItr aIter = vaTouchLinkIDSet.begin(); aIter != vaTouchLinkIDSet.end(); ++aIter)
	{
		CComVariant vaLinkOID = *aIter;
		if(m_SmartICLinkSet.end() == m_SmartICLinkSet.find(vaLinkOID.lVal))
			return false;
	}
	return true;
}

//���b�V���R�[�h�̎擾
vaSET CSmartICList::GetMeshCode(IGeometryPtr ipGeom, esriSpatialRelEnum eStType)
{
	return m_cDBManager.GetHitValSet(m_NwFC.ipBaseMeshFC, ipGeom, m_FieldIndex.lMeshcode, eStType);
}

//�o�͗p���X�g�̍쐬
void CSmartICList::CreateList(long lOID, const vaSET& vaMeshCodeSet, llSET& llOutSet)
{
	for(ConstVaItr aIter = vaMeshCodeSet.begin(); aIter != vaMeshCodeSet.end(); ++aIter)
	{
		CComVariant vaMeshCode = *aIter;
		//�o�̓f�[�^�𐬌^���邽�߂ɃR���e�i�ɓo�^
		llOutSet.insert(std::pair<long,long>(vaMeshCode.lVal, lOID));
	}
}

//�쐬�������X�g���o�͂���
void CSmartICList::Output(const llSET& llOutSet, std::ofstream& ofs)
{
	for(ConstItr2 aIter = llOutSet.begin(); aIter != llOutSet.end(); ++aIter)
	{
		//�Ώۃ��b�V���݂̂��o�͂���
		if(m_eParams.m_lMeshSet.find(aIter->first) != m_eParams.m_lMeshSet.end())
		{
			ofs << aIter->first << _T("\t") << aIter->second << std::endl;
		}
	}

}

