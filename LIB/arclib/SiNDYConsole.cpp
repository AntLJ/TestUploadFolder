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
#pragma warning(disable : 4290)
#include <boost/bind.hpp>
#include "SiNDYTraits.h"
#include "SiNDYAlgorithm.h"
#include "SiNDYConnectionInfoString.h"
#include "SiNDYConsole.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

using namespace sindy;

//////////////////////////////////////////////////////////////////////
// Console
//////////////////////////////////////////////////////////////////////

Console::Console(std::ostream& rCerr) :
m_rCerr(rCerr)
{
}

// 
//////////////////////////////////////////////////////////////////////

/**
 * @brief �t�@�C�����w�肵�ă��[�N�X�y�[�X���J��
 *
 * ���[�N�X�y�[�X�ւ̐ڑ��ɂ� ConnectionFile::ConsoleConnector ���g�p����B
 *
 * @param rFolder [in] "�f�[�^�x�[�X �R�l�N�V����"�K�w�B
 * @param lpszPath [in] �p�X�B
 */ 
IWorkspacePtr Console::openWorkspace(const ConnectionFile::Folder& rFolder, LPCTSTR lpszPath)
{
	return ConnectionFile::ConsoleConnector(m_rCerr, rFolder)(lpszPath);
}

/**
 * @brief �t�@�C�����w�肵�ă��[�N�X�y�[�X���J��
 *
 * �֐����� ConnectionFile::Folder �̃C���X�^���X�𐶐����A Console::openWorkspace(const ConnectionFile::Folder, LPCTSTR) ���ĂԁB
 *
 * @param lpszPath [in] �p�X�B
 */ 
IWorkspacePtr Console::openWorkspace(LPCTSTR lpszPath)
{
	m_rCerr << '\"' << static_cast<LPCTSTR>(lpszPath) << _T("\" �ڑ� ... ");

	IWorkspacePtr ipWorkspace = ConnectionInfoString::connectWorkspace(lpszPath);
	m_rCerr << (ipWorkspace != 0 ? "OK" : "NG") << std::endl;

	return ipWorkspace;
}

/**
 * @brief �ڑ��v���p�e�B���w�肵�ă��[�N�X�y�[�X���J��
 *
 * @param ipWorkspaceFactory [in] ���[�N�X�y�[�X�t�@�N�g���B
 * @param ipPropertySet [in] �v���p�e�B�ꗗ�B
 */
IWorkspacePtr Console::openWorkspace(IWorkspaceFactory* ipWorkspaceFactory, IPropertySet* ipPropertySet)
{
	m_rCerr << '\"' << static_cast<LPCTSTR>(static_cast<ConnectionInfoString>(ipPropertySet)) << _T("\" �ڑ� ... ");

	IWorkspacePtr ipWorkspace;
	m_rCerr << ((ipWorkspaceFactory->Open(ipPropertySet, 0, &ipWorkspace) == S_OK && ipWorkspace != 0) ? "OK" : "NG") << std::endl;

	return ipWorkspace;
}

/**
 * @brief �ڑ��v���p�e�B���w�肵�ă��[�N�X�y�[�X���J��
 *
 * �֐����� WorkspaceFactory �̃C���X�^���X�𐶐����A Console::openWorkspace(IWorkspaceFactory*, IPropertySet*) ���ĂԁB
 *
 * @param ipPropertySet [in] �v���p�e�B�ꗗ�B
 */
IWorkspacePtr Console::openWorkspace(IPropertySet* ipPropertySet)
{
	IWorkspaceFactoryPtr ipWorkspaceFactory(CLSID_WorkspaceFactory);
	return openWorkspace(ipWorkspaceFactory, ipPropertySet);
}

/**
 * @brief �t�B�[�`���N���X���J��
 *
 * �t�B�[�`���N���X�̓W�J�ɂ� Console::OpenTable<IFeatureClass> ���g�p����B
 */
IFeatureClassPtr Console::openFeatureClass(IFeatureWorkspace* ipFeatureWorkspace, BSTR bstrTableName)
{
	return OpenTable<IFeatureClass>()(m_rCerr, ipFeatureWorkspace, bstrTableName);
}

/**
 * @brief �e�[�u�����J��
 *
 * �e�[�u���̓W�J�ɂ� Console::OpenTable<ITable> ���g�p����B
 */
ITablePtr Console::openTable(IFeatureWorkspace* ipFeatureWorkspace, BSTR bstrTableName)
{
	return OpenTable<ITable>()(m_rCerr, ipFeatureWorkspace, bstrTableName);
}

// shape�t�@�C���n����
//////////////////////////////////////////////////////////////////////

/**
 * @brief shape�t�@�C���K�w���J��
 *
 * @param bstrShapeFolderPath [in] shape�t�@�C���K�w�B
 * @return shape�t�@�C�����[�N�X�y�[�X�B�J���Ȃ������ꍇ��null�B
 */
IWorkspacePtr Console::openShapefileWorkspace(BSTR bstrShapeFolderPath)
{
	USES_CONVERSION;

	// �����̊J�n�����m����B
	m_rCerr << "shape�t�@�C���K�w \"" << OLE2CA(bstrShapeFolderPath) << "\" �W�J ... ";

	// shape�t�@�C���H�[��p�ӁB
	IWorkspaceFactoryPtr ipWorkspaceFactory(CLSID_ShapefileWorkspaceFactory);

	// shape�t�@�C���K�w���J���B
	IWorkspacePtr ipWorkspace;
	ipWorkspaceFactory->OpenFromFile(bstrShapeFolderPath, 0, &ipWorkspace);

	// �����̊��������m����B
	m_rCerr << (ipWorkspace != 0 ? "OK" : "NG") << std::endl;

	return ipWorkspace;
}

/**
 * @brief shape�t�@�C�����쐬����
 *
 * @param lpszShapeFilePath [in] shape�t�@�C���p�X�B
 * @param ipFields [in] shape�t�@�C���������B
 * @return shape�t�@�C���B�����ł��Ȃ������ꍇ��null�B
 */
IFeatureClassPtr Console::createShapefile(LPCTSTR lpszShapeFilePath, IFields* ipFields)
{
	if(! (lpszShapeFilePath && ipFields))
		return 0;

	// ��Ɨp�Ƀp�X���R�s�[�B
	TCHAR szShapeFilePath[MAX_PATH];
	::_tcscpy(szShapeFilePath, lpszShapeFilePath);

	LPTSTR lpszFolderPath = 0, lpszFileName = 0;

	// �K�w�p�X�ƃt�@�C�����ɕ�������B
	TCHAR* p = ::_tcsrchr(szShapeFilePath, _T('\\'));
	if(p) {
		*p = 0;
		lpszFolderPath = szShapeFilePath;
		lpszFileName = p + 1;
	}
	else {
		lpszFileName = szShapeFilePath;
	}

	// shape�t�@�C���g���q���O���B
	p = ::_tcsrchr(lpszFileName, _T('.'));
	if(p && ::_tcsicmp(p + 1, _T("shp")) == 0) {
		*p = 0;
	}

	// shape�t�@�C���K�w���J���B
	IFeatureWorkspacePtr ipFeatureWorkspace = openShapefileWorkspace(CComBSTR(lpszFolderPath));
	if(ipFeatureWorkspace == 0)
		return 0;

	// shape�t�@�C���𐶐�����B
	IFeatureClassPtr ipFeatureClass = createShapefile(ipFeatureWorkspace, CComBSTR(lpszFileName), ipFields);

	return ipFeatureClass;
}

/**
 * @brief shape�t�@�C�����쐬����
 *
 * @warning ������shape�t�@�C���͍폜�����B
 *
 * @param ipFeatureWorkspace [in] shape�t�@�C�����[�N�X�y�[�X�B
 * @param bstrShapeFileName [in] shape�t�@�C�����i�g���q�����j�B
 * @param ipFields [in] shape�t�@�C���������B
 * @return shape�t�@�C���B�����ł��Ȃ������ꍇ��null�B
 */
IFeatureClassPtr Console::createShapefile(IFeatureWorkspace* ipFeatureWorkspace, BSTR bstrShapeFileName, IFields* ipFields)
{
	if(! (ipFeatureWorkspace && bstrShapeFileName && ipFields))
		return 0;

	USES_CONVERSION;

	m_rCerr << "shape�t�@�C�� \"" << OLE2CA(bstrShapeFileName) << "\" ... ";

	// ������Shape�t�@�C�����폜����B
	IDatasetPtr ipDataset = getData<IFeatureClassPtr>(ipFeatureWorkspace, bstrShapeFileName);
	if(ipDataset != 0) {
		m_rCerr << "�폜 ... ";
		if(ipDataset->Delete() != S_OK) {
			m_rCerr << "NG" << std::endl;
			return 0;
		}
	}

	m_rCerr << "�`�󗓊m�F ... ";

	IFieldPtr ipField = find_if(ipFields, boost::bind<bool>(data_equal<IField>(), _1, esriFieldTypeGeometry));
	if(ipField == 0) {
		m_rCerr << "NG" << std::endl;
		return 0;
	}

	CComBSTR bstrShapeFieldName;
	ipField->get_Name(&bstrShapeFieldName);

	m_rCerr << "���� ... ";
	IFeatureClassPtr ipFeatureClass;
	ipFeatureWorkspace->CreateFeatureClass(
		bstrShapeFileName,
		ipFields,
		0,
		0,
		esriFTSimple,
		bstrShapeFieldName,
		CComBSTR(_T("")),
		&ipFeatureClass);

	std::cerr << ((ipFeatureClass != 0) ? "OK" : "NG") << std::endl;

	return ipFeatureClass;
}
