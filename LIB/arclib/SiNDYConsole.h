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

#ifndef __SINDY_CONSOLE_H__
#define __SINDY_CONSOLE_H__

#include <iostream>
#include "SiNDYTraits.h"
#include "SiNDYConnectionFile.h"
#include "exportimport.h"

namespace sindy {

/**
 * @brief 標準エラー出力機能付き処理クラス
 */
class SINDYLIB_API Console
{
public:
// 定義
	/**
	 * @brief メッセージ付きでテーブルを開くテンプレート関数クラス
	 *
	 * - TTable : ITable あるいは IFeatureClass 。
	 */
	template <typename TTable>
	class OpenTable
	{
	public:
	// 定義
		typedef _COM_SMARTPTR<_COM_SMARTPTR_LEVEL2<TTable, &__uuidof(TTable)> > TableInterfacePtr; ///< スマートポインタ。

	// 構築
		OpenTable() {}

	// 処理
		/**
		 * @brief 状況報告付きでテーブルを開く
		 *
		 * @param rCerr [in] メッセージ出力先
		 * @param ipFeatureWorkspace [in] フィーチャワークスペース。
		 * @param bstrTableName [in] テーブル名。
		 * @return テーブル。開けなかった場合はnull。
		 */
		TableInterfacePtr operator()(std::ostream& rCerr, IFeatureWorkspace* ipFeatureWorkspace, BSTR bstrTableName)
		{
			USES_CONVERSION;

			(rCerr << '\"' << OLE2CA(bstrTableName) << "\" 展開 ... ").flush();

			TableInterfacePtr ipTable;
			rCerr
				<< ((sindy::traits<IFeatureWorkspace>::accessor<TableInterfacePtr>::get(ipFeatureWorkspace, bstrTableName, &ipTable) == S_OK && ipTable != 0) ? "OK" : "NG")
				<< std::endl;

			return ipTable;
		}
	};

// 構築
	Console(std::ostream& rCerr);

// 処理
	IWorkspacePtr openWorkspace(const ConnectionFile::Folder& rFolder, LPCTSTR lpszPath);
	IWorkspacePtr openWorkspace(LPCTSTR lpszPath);
	IWorkspacePtr openWorkspace(IWorkspaceFactory* ipWorkspaceFactory, IPropertySet* ipPropertySet);
	IWorkspacePtr openWorkspace(IPropertySet* ipPropertySet);

	IFeatureClassPtr openFeatureClass(IFeatureWorkspace* ipFeatureWorkspace, BSTR bstrTableName);
	ITablePtr openTable(IFeatureWorkspace* ipFeatureWorkspace, BSTR bstrTableName);

// shapeファイル系処理
	IWorkspacePtr openShapefileWorkspace(BSTR bstrShapeFolderPath);

	IFeatureClassPtr createShapefile(LPCTSTR lpszShapeFilePath, IFields* ipFields);
	IFeatureClassPtr createShapefile(IFeatureWorkspace* ipShapeWorkspace, BSTR bstrShapeFileName, IFields* ipFields);

private:
// 変数
	std::ostream& m_rCerr;
};

SINDYLIB_EXPIMP template class SINDYLIB_API Console::OpenTable<ITable>;
SINDYLIB_EXPIMP template class SINDYLIB_API Console::OpenTable<IFeatureClass>;

} // namespace sindy

#endif // __SINDY_CONSOLE_H__
