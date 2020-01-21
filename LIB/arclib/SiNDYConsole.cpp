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
 * @brief ファイルを指定してワークスペースを開く
 *
 * ワークスペースへの接続には ConnectionFile::ConsoleConnector を使用する。
 *
 * @param rFolder [in] "データベース コネクション"階層。
 * @param lpszPath [in] パス。
 */ 
IWorkspacePtr Console::openWorkspace(const ConnectionFile::Folder& rFolder, LPCTSTR lpszPath)
{
	return ConnectionFile::ConsoleConnector(m_rCerr, rFolder)(lpszPath);
}

/**
 * @brief ファイルを指定してワークスペースを開く
 *
 * 関数内で ConnectionFile::Folder のインスタンスを生成し、 Console::openWorkspace(const ConnectionFile::Folder, LPCTSTR) を呼ぶ。
 *
 * @param lpszPath [in] パス。
 */ 
IWorkspacePtr Console::openWorkspace(LPCTSTR lpszPath)
{
	m_rCerr << '\"' << static_cast<LPCTSTR>(lpszPath) << _T("\" 接続 ... ");

	IWorkspacePtr ipWorkspace = ConnectionInfoString::connectWorkspace(lpszPath);
	m_rCerr << (ipWorkspace != 0 ? "OK" : "NG") << std::endl;

	return ipWorkspace;
}

/**
 * @brief 接続プロパティを指定してワークスペースを開く
 *
 * @param ipWorkspaceFactory [in] ワークスペースファクトリ。
 * @param ipPropertySet [in] プロパティ一覧。
 */
IWorkspacePtr Console::openWorkspace(IWorkspaceFactory* ipWorkspaceFactory, IPropertySet* ipPropertySet)
{
	m_rCerr << '\"' << static_cast<LPCTSTR>(static_cast<ConnectionInfoString>(ipPropertySet)) << _T("\" 接続 ... ");

	IWorkspacePtr ipWorkspace;
	m_rCerr << ((ipWorkspaceFactory->Open(ipPropertySet, 0, &ipWorkspace) == S_OK && ipWorkspace != 0) ? "OK" : "NG") << std::endl;

	return ipWorkspace;
}

/**
 * @brief 接続プロパティを指定してワークスペースを開く
 *
 * 関数内で WorkspaceFactory のインスタンスを生成し、 Console::openWorkspace(IWorkspaceFactory*, IPropertySet*) を呼ぶ。
 *
 * @param ipPropertySet [in] プロパティ一覧。
 */
IWorkspacePtr Console::openWorkspace(IPropertySet* ipPropertySet)
{
	IWorkspaceFactoryPtr ipWorkspaceFactory(CLSID_WorkspaceFactory);
	return openWorkspace(ipWorkspaceFactory, ipPropertySet);
}

/**
 * @brief フィーチャクラスを開く
 *
 * フィーチャクラスの展開には Console::OpenTable<IFeatureClass> を使用する。
 */
IFeatureClassPtr Console::openFeatureClass(IFeatureWorkspace* ipFeatureWorkspace, BSTR bstrTableName)
{
	return OpenTable<IFeatureClass>()(m_rCerr, ipFeatureWorkspace, bstrTableName);
}

/**
 * @brief テーブルを開く
 *
 * テーブルの展開には Console::OpenTable<ITable> を使用する。
 */
ITablePtr Console::openTable(IFeatureWorkspace* ipFeatureWorkspace, BSTR bstrTableName)
{
	return OpenTable<ITable>()(m_rCerr, ipFeatureWorkspace, bstrTableName);
}

// shapeファイル系処理
//////////////////////////////////////////////////////////////////////

/**
 * @brief shapeファイル階層を開く
 *
 * @param bstrShapeFolderPath [in] shapeファイル階層。
 * @return shapeファイルワークスペース。開けなかった場合はnull。
 */
IWorkspacePtr Console::openShapefileWorkspace(BSTR bstrShapeFolderPath)
{
	USES_CONVERSION;

	// 処理の開始を告知する。
	m_rCerr << "shapeファイル階層 \"" << OLE2CA(bstrShapeFolderPath) << "\" 展開 ... ";

	// shapeファイル工房を用意。
	IWorkspaceFactoryPtr ipWorkspaceFactory(CLSID_ShapefileWorkspaceFactory);

	// shapeファイル階層を開く。
	IWorkspacePtr ipWorkspace;
	ipWorkspaceFactory->OpenFromFile(bstrShapeFolderPath, 0, &ipWorkspace);

	// 処理の完了を告知する。
	m_rCerr << (ipWorkspace != 0 ? "OK" : "NG") << std::endl;

	return ipWorkspace;
}

/**
 * @brief shapeファイルを作成する
 *
 * @param lpszShapeFilePath [in] shapeファイルパス。
 * @param ipFields [in] shapeファイル属性欄。
 * @return shapeファイル。生成できなかった場合はnull。
 */
IFeatureClassPtr Console::createShapefile(LPCTSTR lpszShapeFilePath, IFields* ipFields)
{
	if(! (lpszShapeFilePath && ipFields))
		return 0;

	// 作業用にパスをコピー。
	TCHAR szShapeFilePath[MAX_PATH];
	::_tcscpy(szShapeFilePath, lpszShapeFilePath);

	LPTSTR lpszFolderPath = 0, lpszFileName = 0;

	// 階層パスとファイル名に分離する。
	TCHAR* p = ::_tcsrchr(szShapeFilePath, _T('\\'));
	if(p) {
		*p = 0;
		lpszFolderPath = szShapeFilePath;
		lpszFileName = p + 1;
	}
	else {
		lpszFileName = szShapeFilePath;
	}

	// shapeファイル拡張子を外す。
	p = ::_tcsrchr(lpszFileName, _T('.'));
	if(p && ::_tcsicmp(p + 1, _T("shp")) == 0) {
		*p = 0;
	}

	// shapeファイル階層を開く。
	IFeatureWorkspacePtr ipFeatureWorkspace = openShapefileWorkspace(CComBSTR(lpszFolderPath));
	if(ipFeatureWorkspace == 0)
		return 0;

	// shapeファイルを生成する。
	IFeatureClassPtr ipFeatureClass = createShapefile(ipFeatureWorkspace, CComBSTR(lpszFileName), ipFields);

	return ipFeatureClass;
}

/**
 * @brief shapeファイルを作成する
 *
 * @warning 既存のshapeファイルは削除される。
 *
 * @param ipFeatureWorkspace [in] shapeファイルワークスペース。
 * @param bstrShapeFileName [in] shapeファイル名（拡張子抜き）。
 * @param ipFields [in] shapeファイル属性欄。
 * @return shapeファイル。生成できなかった場合はnull。
 */
IFeatureClassPtr Console::createShapefile(IFeatureWorkspace* ipFeatureWorkspace, BSTR bstrShapeFileName, IFields* ipFields)
{
	if(! (ipFeatureWorkspace && bstrShapeFileName && ipFields))
		return 0;

	USES_CONVERSION;

	m_rCerr << "shapeファイル \"" << OLE2CA(bstrShapeFileName) << "\" ... ";

	// 既存のShapeファイルを削除する。
	IDatasetPtr ipDataset = getData<IFeatureClassPtr>(ipFeatureWorkspace, bstrShapeFileName);
	if(ipDataset != 0) {
		m_rCerr << "削除 ... ";
		if(ipDataset->Delete() != S_OK) {
			m_rCerr << "NG" << std::endl;
			return 0;
		}
	}

	m_rCerr << "形状欄確認 ... ";

	IFieldPtr ipField = find_if(ipFields, boost::bind<bool>(data_equal<IField>(), _1, esriFieldTypeGeometry));
	if(ipField == 0) {
		m_rCerr << "NG" << std::endl;
		return 0;
	}

	CComBSTR bstrShapeFieldName;
	ipField->get_Name(&bstrShapeFieldName);

	m_rCerr << "生成 ... ";
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
