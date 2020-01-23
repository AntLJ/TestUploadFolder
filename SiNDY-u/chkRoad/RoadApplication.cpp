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
#include "RoadApplication.h"
#include "CheckINHead.h"
#include "CheckLinkLayer.h"
#include "CheckNodeLayer.h"
#include "CheckRegLayer.h"
#include "CheckNodeInfoLayer.h"
#include "CheckLaneLayer.h"
#include "CheckLinkInfoLayer.h"
#include "CheckLinkExLayer.h"
#include "SetRoadAttrList.h"

CRoadApplication::CRoadApplication(void)
{
}

CRoadApplication::~CRoadApplication(void)
{
}

//////////////////////////////////////////////////////////////////////
// IN�f�[�^�̃G���[�`�F�b�N
//////////////////////////////////////////////////////////////////////
long CRoadApplication::run(CString& rConfig, LPCTSTR lpcszCustomer, bool bLatest )
{
	long lReturn = 0;

	GetConf(rConfig);
	Customer eCustomer = GetCustomer( lpcszCustomer );				// �d�����挈��
	m_setDataMeshlist = CreateList( m_INPathData.eMntDir );			// data�p���b�V�����X�g
	m_setLatestMeshlist = CreateList( m_INPathData.eLatestDir );	// B�ňȍ~�p���b�V�����X�g
	if( m_setLatestMeshlist.empty() )
		m_setLatestMeshlist = m_setDataMeshlist;

	AttrList aAttrList;
	CSetRoadAttrList aTotalAttr;

	// pol_code.txt�����݂��Ȃ��ꍇ�́A�ُ�I����Ԃ�
	if( !aTotalAttr.setList( m_INPathData.ePolDir, aAttrList ))
		return 1;

	// .mnt���w��t�H���_�ȉ��Ɉ�����݂��Ȃ��ꍇ�́A�ُ�I����Ԃ�
	if( (int)m_setDataMeshlist.size() == 0 )
	{
		cout << "# ERROR\t�w��p�X(MNT_DIR)�ȉ��ɓ��H�nIN�f�[�^�����݂��܂���B\t" << static_cast<LPCTSTR>(m_INPathData.eMntDir) << endl;
		return 1;
	}

	for( set<int>::iterator p = m_setDataMeshlist.begin(); p != m_setDataMeshlist.end(); ++ p )
	{
		cerr << *p << "���`�F�b�N���E�E�E�E�E" << endl;
		// IN�f�[�^�̃w�b�_�����`�F�b�N
		CCheckINHead aCheckHead;
		if( !aCheckHead.check( m_INPathData.eMntDir, *p, aAttrList.eINHead ) )
			lReturn = 1;

		if( lReturn != 1 && aCheckHead.IsError() )
			lReturn = 3;

		// ���H�����N���C�����`�F�b�N
		CCheckLinkLayer aCheckLink;
		if( !aCheckLink.check( m_INPathData.eMntDir, *p, aAttrList.eLinkLayer, eCustomer, m_setLatestMeshlist ) )
			lReturn = 1;

		if( lReturn != 1 && aCheckLink.IsError() )
			lReturn = 3;

		// ���H�m�[�h���C�����`�F�b�N
		CCheckNodeLayer aCheckNode;
		if( !aCheckNode.check( m_INPathData.eMntDir, *p, aAttrList.eNodeLayer ) )
			lReturn = 1;

		if( lReturn != 1 && aCheckNode.IsError() )
			lReturn = 3;

		// �V��ʋK�����C�����`�F�b�N
		CCheckRegLayer aCheckReg;
		if( !aCheckReg.check( m_INPathData.eMntDir, *p, aAttrList.eRegLayer ) )
			lReturn = 1;

		if( lReturn != 1 && aCheckReg.IsError() )
			lReturn = 3;

		// �m�[�h��񃌃C�����`�F�b�N
		CCheckNodeInfoLayer aCheckNInfo;
		if( !aCheckNInfo.check( m_INPathData.eMntDir, *p, aAttrList.eNInfoLayer ) )
			lReturn = 1;

		if( lReturn != 1 && aCheckNInfo.IsError() )
			lReturn = 3;

		// ���[�����C�����`�F�b�N
		CCheckLaneLayer aCheckLane;
		if( !aCheckLane.check( m_INPathData.eMntDir, *p, aAttrList.eLaneLayer ) )
			lReturn = 1;

		if( lReturn != 1 && aCheckReg.IsError() )
			lReturn = 3;

		// �����N��񃌃C�����`�F�b�N
		// ���݃����[�X����Ȃ����C��
		CCheckLinkInfoLayer aCheckLInfo;
		if( !aCheckLInfo.check( m_INPathData.eMntDir, *p, aAttrList.eLInfoLayer ) )
			lReturn = 1;

		// �����N�g�����C�����`�F�b�N
		CCheckLinkExLayer aCheckLinkEx;
		if( !aCheckLinkEx.check( m_INPathData.eMntDir, *p, aAttrList.eLinkExLayer ) )
			lReturn = 1;

		if( lReturn != 1 && aCheckLinkEx.IsError() )
			lReturn = 3;
	}

	return lReturn;
}

//////////////////////////////////////////////////////////////////////
// Config�t�@�C���̐ݒ�ǂݍ���
//////////////////////////////////////////////////////////////////////
void CRoadApplication::GetConf(CString &rConfig)
{
	ifstream ifs(rConfig);
	char rStrLine[1024] ={0};
	while( ifs.getline( rStrLine, sizeof(rStrLine) ) )
	{
		CString csLine = rStrLine;
		if( _tcscmp( csLine.Left(1), _T("#") ) == 0 )
			continue;
		if( strstr( csLine, _T("MNT_DIR")) != 0 )
		{
			csLine.Delete(0,7);
			csLine.Remove(_T(' '));
			csLine.Remove(_T('	'));
			m_INPathData.eMntDir = csLine;
		}
		else if( strstr( csLine, _T("POL_DIR")) != 0 )
		{
			csLine.Delete(0,7);
			csLine.Remove(_T(' '));
			csLine.Remove(_T('	'));
			m_INPathData.ePolDir = csLine;
		}
		else if( strstr( csLine, _T("LATEST_DIR")) != 0 )
		{
			csLine.Delete(0,10);
			csLine.Remove(_T(' '));
			csLine.Remove(_T('	'));
			m_INPathData.eLatestDir = csLine;
		}
	}
}

//////////////////////////////////////////////////////////////////////
// �d�����挈��
//////////////////////////////////////////////////////////////////////
Customer CRoadApplication::GetCustomer( LPCTSTR lpcszCustomer )
{
	Customer eCustomer;
	if( _tcscmp( lpcszCustomer, _T("PEC") ) == 0 || _tcscmp( lpcszCustomer, _T("TR") ) == 0 )
		eCustomer = PEC;
	else
		eCustomer = OTHER;

	return eCustomer;
}

//////////////////////////////////////////////////////////////////////
// IN�f�[�^��2�����b�V�����X�g�쐬
//////////////////////////////////////////////////////////////////////
std::set<int> CRoadApplication::CreateList( LPCTSTR lpcszMntDir )
{
	std::set<int> setRet;

	WIN32_FIND_DATA fd;
	HANDLE hFind = FindFirstFile(lpcszMntDir, &fd);
	if( hFind != INVALID_HANDLE_VALUE )
	{
		if( fd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY )
		{
			CString cstrFind;
			cstrFind.Format(_T("%s\\*"), lpcszMntDir );
			WIN32_FIND_DATA fd2;
			HANDLE hFind2 = FindFirstFile(cstrFind, &fd2);
			FindFile( hFind2, fd2,lpcszMntDir, setRet );
		}
	}
	FindClose(hFind);

	return setRet;
}

void CRoadApplication::FindFile( HANDLE hFind, WIN32_FIND_DATA &fd, LPCTSTR lpPath, std::set<int>& setMesh )
{
	do {
		if( _tcscmp( fd.cFileName, _T(".") ) == 0 || _tcscmp( fd.cFileName, _T("..") ) == 0 )
			continue;

		if( fd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY )
		{
			CString cstrPath;
			cstrPath.Format(_T("%s\\%s"), lpPath, fd.cFileName);
			CString cstrFind = cstrPath + CString(_T("\\*"));

			WIN32_FIND_DATA fd2;
			HANDLE hFind2 = FindFirstFile(cstrFind, &fd2);
			FindFile(hFind2, fd2, cstrPath, setMesh);
			FindClose(hFind2);

		} 
		else 
		{
			// mnt�t�@�C���݂̂Ō���
			if( _tcsstr( fd.cFileName, _T(".mnt") ) != NULL )
			{
				if( CString(fd.cFileName).GetLength() == 10 )
					setMesh.insert(_ttoi(CString(fd.cFileName).Left(6)));
			}
		}
	} while (FindNextFile(hFind, &fd));
}
