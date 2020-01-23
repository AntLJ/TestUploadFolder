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

#include "ChildVerEditWorkspace.h"
#include "type_convert.h"

bool ChildVerEditWorkspace::open(const CString& connection)
{
	m_Workspace = sindy::create_workspace(connection.GetString());

	return (m_Workspace);
}

ITablePtr ChildVerEditWorkspace::openTable(const CString& table_name)
{
	ITablePtr table;
	IFeatureWorkspacePtr ipFWS(m_Workspace);
	if(ipFWS) 
		ipFWS->OpenTable(CComBSTR(table_name.GetString()), &table);

	return table;
}

bool ChildVerEditWorkspace::childCreateModeOn(const CString& ver_name, const std::vector<CString>& edit_targets)
{
	unsigned int ver(0);
	for(const auto& tableName : edit_targets ){
		// �e�[�u���I�[�v��
		ITablePtr table;
		IFeatureWorkspacePtr fws(m_Workspace);
		if(fws)
			fws->OpenTable(CComBSTR(tableName), &table);	

		IVersionedObjectPtr ipVer(table);
		if(!ipVer) return false;

		VARIANT_BOOL vb;
		if(FAILED(ipVer->get_IsRegisteredAsVersioned(&vb)))
			return false;

		if(vb)
			++ver;
	}
	
	// �S�Ẵe�[�u�����o�[�W�����Ή��łȂ���΁Afalse �Ƃ���B
	if( ver != edit_targets.size() )
		return false;

	m_IsVersioned = true;
	m_ChildCreateMode = true;
	m_ChildVerName = ver_name;

	// �q�o�[�W�������쐬
	if(! createChildVer())
		return false;

	return true;
}

bool ChildVerEditWorkspace::startEditing()
{
	if(!m_Workspace) return false;

	// �ҏW�J�n����Ă��邩���`�F�b�N
	VARIANT_BOOL vb(VARIANT_FALSE);
	if(FAILED(m_Workspace->IsBeingEdited(&vb)) || VB2bool(vb))
		return false;

	// �o�[�W�����Ή��Ƃ��ĕҏW�J�n
	if(FAILED( m_Workspace->StartEditing(VARIANT_FALSE) ))
		return false;

	return true;
}

bool ChildVerEditWorkspace::stopEditing(bool bSave)
{
	if(!m_Workspace) return false;

	if(FAILED(m_Workspace->StopEditing(bool2VB(bSave)))){
		return false;
	}

	// �q�o�[�W�����쐬��ɕҏW�Ɏ��s���Ă�����A�q�o�[�W�������폜
	if(! deleteChildVer())
		return false;

	return true;
}

bool ChildVerEditWorkspace::startEditOperation()
{
	if(!m_Workspace) return false;
	
	if(!m_IsVersioned) return true;

	if(FAILED( m_Workspace->StartEditOperation() )) {
		m_Workspace->StopEditing(false);
		return false;
	}

	return true;
}

bool ChildVerEditWorkspace::stopEditOperation()
{
	if(!m_Workspace) return false;

	if(!m_IsVersioned) return true;

	if(FAILED( m_Workspace->StopEditOperation())){
		m_Workspace->StopEditing(false);
		return false;
	}

	return true;
}

bool ChildVerEditWorkspace::abortEditOperation()
{
	if(!m_Workspace) return false;

	m_IsEditFailed = true;
	return SUCCEEDED(m_Workspace->AbortEditOperation());
}

bool ChildVerEditWorkspace::isSDE()
{
	if( IWorkspacePtr(m_Workspace)->GetType() == esriRemoteDatabaseWorkspace )
		return true;

	return false;
}

bool ChildVerEditWorkspace::createChildVer()
{
	if( m_IsCreatedChild )
		return true;

	// ���o�[�W�������ҏW�J�n����Ă���Ȃ�q�o�[�W�����͍쐬���Ȃ�
	VARIANT_BOOL vb(VARIANT_FALSE);
	if(FAILED(m_Workspace->IsBeingEdited(&vb)) || VB2bool(vb))
		return false;

	// PGDB�EFGDB �Ȃ�Ύq�o�[�W�������쐬���Ȃ�
	if(!isSDE())
		return false;

	// �q�o�[�W�����쐬
	IVersionPtr currentVersion(m_Workspace);
	IVersionPtr childVersion;
	currentVersion->CreateVersion(_bstr_t(m_ChildVerName), &childVersion);
	if(! childVersion)
		return false;
	if(FAILED(childVersion->put_Access(esriVersionAccessPublic)))
		return false;

	m_Workspace = (IWorkspaceEditPtr)childVersion;

	m_IsCreatedChild = true;
	return true;
}

bool ChildVerEditWorkspace::deleteChildVer()
{
	// �q�o�[�W�������쐬���Ă��Ȃ����A�ҏW�Ŏ��s���Ă��Ȃ���Ή������Ȃ�
	if(! m_IsCreatedChild || ! m_IsEditFailed)
		return true;

	IVersionPtr version(m_Workspace);
	if(FAILED(version->Delete()))
		return false;

	m_IsCreatedChild = false;

	return true;
}
