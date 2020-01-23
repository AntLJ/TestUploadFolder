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

#ifndef SINDY_WORKSPACE_ADDRESS_H_
#define SINDY_WORKSPACE_ADDRESS_H_

#include <sstream>
#include <atl2/io.h>
#include <atl2/exception.h>
#include <arctl/catalog.h>

namespace sindy {

inline std::basic_string<TCHAR> workspace_address(IWorkspace* ipWorkspace)
{
	// 接続プロパティを取得できる場合。
	IPropertySetPtr ipPropertySet;
	atl2::valid(ipWorkspace->get_ConnectionProperties(&ipPropertySet));
	if(ipPropertySet) {
		_variant_t vUser;
		if(ipPropertySet->GetProperty(CComBSTR(_T("user")), &vUser) == S_OK) {
			std::basic_ostringstream<TCHAR> os;
			_variant_t vServer;
			if(ipPropertySet->GetProperty(CComBSTR(_T("host")), &vServer) == S_OK) {
				_variant_t vDBName;
				atl2::valid(ipPropertySet->GetProperty(CComBSTR(_T("dbname")), &vDBName));
				os << vUser << _T('@') << vServer << _T('(') << _T("PostGIS.") << vDBName << _T(')');
				return os.str();
			}
			else if(ipPropertySet->GetProperty(CComBSTR(_T("DB_CONNECTION_PROPERTIES")), &vServer) == S_OK || 
				    ipPropertySet->GetProperty(CComBSTR(_T("server")), &vServer) == S_OK) {
				_variant_t vVersion;
				///< PostGISExtensionを使用しないPostGISレイヤは「VERSION」プロパティが存在しないためエラーハンドリングはしない（bug 9777）
				ipPropertySet->GetProperty(CComBSTR(_T("version")), &vVersion);
				os << vUser << _T('@') << vServer  << _T('(') << vVersion << _T(')');
				return os.str();
			}
		}
	}

	// パス指定で接続している場合。
	CComBSTR bstrPathName;
	ipWorkspace->get_PathName(&bstrPathName);
	if(bstrPathName.Length() != 0) {
		std::basic_string<TCHAR> strPathName(static_cast<COLE2CT>(bstrPathName));
		arctl::alias_sde_file_directory_path(strPathName);
		return strPathName;
	}

	return std::basic_string<TCHAR>(_T(""));
}

} // namespace sindy

#endif // SINDY_WORKSPACE_ADDRESS_H_
