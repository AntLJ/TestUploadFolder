/*
 * Copyright (C) INCREMENT P CORP. All Rights Reserved.strncpy
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

#ifndef SINDY_WORKSPACE_NAME_H_
#define SINDY_WORKSPACE_NAME_H_

#include <utility>
#include <atl2/exception.h>
#include <CodeAnalysis/SourceAnnotations.h>
#include <atlstr.h>

namespace sindy {

inline void is_connect_string_as(const CString& orgConnection, bool& bConnectAS, CString& lpszConnection)
{
	bConnectAS = false;
	CString strPrefConnect_AS = _T("[AS]"); //!< アプリケーションサーバー接続でWorkspaceを開く場合の、接続文字列につける接頭辞("[AS]trial@coral2(SDE.DEFAULT)"などと指定する)

	// 接頭辞が"[AS]"であれば、AS接続と判断
	if(orgConnection.Find(strPrefConnect_AS) == 0)
	{
		bConnectAS = true;
		CString temp = orgConnection;
		lpszConnection = temp.Right(temp.GetLength() - strPrefConnect_AS.GetLength());
	}
	else
	{
		lpszConnection = orgConnection;
	}
}

/**
 * @brief <td>host=<em>host</em>;port=<em>port</em>;user=<em>user</em>;password=<em>password</em>;dbname=<em>database</em> という形式のPostGIS接続文字列から WorkspaceName を生成する。
 *
 * @param lpszConnection [in] 接続文字列。
 * @retval 0 接続文字列の解析に失敗した。
 * @retval !0 接続文字列から生成された WorkspaceName 。
 */
inline IWorkspaceName2Ptr create_postgis_workspace_string_name([SA_Pre(Null=SA_No, NullTerminated=SA_Yes)] LPCTSTR lpszConnection)
{
	if(
		::_tcsstr(lpszConnection, _T("host=")) == 0 ||
		::_tcsstr(lpszConnection, _T("port=")) == 0 ||
		::_tcsstr(lpszConnection, _T("user=")) == 0 ||
		::_tcsstr(lpszConnection, _T("password=")) == 0 ||
		::_tcsstr(lpszConnection, _T("dbname=")) == 0
	)
		return 0;

	const IWorkspaceName2Ptr ipWorkspaceName(__uuidof(WorkspaceName));

	const CComBSTR bstrConnection(lpszConnection);
	atl2::valid(ipWorkspaceName->put_BrowseName(bstrConnection));
	atl2::valid(ipWorkspaceName->put_WorkspaceFactoryProgID(CComBSTR(_T("PostGISExtension.PostGISWorkspaceFactory"))));
	atl2::valid(ipWorkspaceName->put_ConnectionString(bstrConnection));
	
	return ipWorkspaceName;
}

/**
* @brief Oracleダイレクトコネクションの WorkspaceName を生成
*
* @param tnsName	[in] TNSサービス名
* @param user		[in] ユーザ
* @param password	[in] パスワード
* @param version	[in] バージョン
* @return 接続文字列から生成された WorkspaceName 。
*/
inline IWorkspaceName2Ptr create_workspace_name_oracle_dc(const CComVariant& tnsName, const CComVariant& user, const CComVariant& password, const CComVariant& version)
{
	// SDE用のWorkspaceNameを生成し、接続プロパティを設定する。
	const IPropertySetPtr ipPropertySet(__uuidof(PropertySet));

	// DBCLIENT, AUTHENTICATION_MODEプロパティは決め打ち
	atl2::valid(ipPropertySet->SetProperty(CComBSTR(_T("DBCLIENT")), CComVariant(_T("oracle"))));
	atl2::valid(ipPropertySet->SetProperty(CComBSTR(_T("AUTHENTICATION_MODE")), CComVariant(_T("DBMS"))));

	// 各引数の値をプロパティにセット
	atl2::valid(ipPropertySet->SetProperty(CComBSTR(_T("DB_CONNECTION_PROPERTIES")), tnsName));
	atl2::valid(ipPropertySet->SetProperty(CComBSTR(_T("USER")), user));
	atl2::valid(ipPropertySet->SetProperty(CComBSTR(_T("PASSWORD")), password));
	atl2::valid(ipPropertySet->SetProperty(CComBSTR(_T("VERSION")), version));

	// IWorkspaceNameを生成して返す
	const IWorkspaceName2Ptr ipWorkspaceName(__uuidof(WorkspaceName));
	atl2::valid(ipWorkspaceName->put_WorkspaceFactoryProgID(CComBSTR(_T("esriDataSourcesGDB.SdeWorkspaceFactory"))));
	atl2::valid(ipWorkspaceName->put_ConnectionProperties(ipPropertySet));
	return ipWorkspaceName;
}

/**
* @brief Oracleアプリケーションサーバー接続の WorkspaceName を生成
*
* @param server		[in] サーバー名
* @param user		[in] ユーザ
* @param password	[in] パスワード
* @param version	[in] バージョン
* @param instance	[in] インスタンス
* @return 接続文字列から生成された WorkspaceName 。
*/

inline IWorkspaceName2Ptr create_workspace_name_oracle_as(const CComVariant& server,
														  const CComVariant& user,
														  const CComVariant& password,
														  const CComVariant& version,
														  const CComVariant& instance)
{
	const IPropertySetPtr ipPropertySet(__uuidof(PropertySet));

	const CComBSTR props[5] = {
		_T("USER"),
		_T("PASSWORD"),
		_T("VERSION"),
		_T("INSTANCE"),
		_T("SERVER"),
	};
	
	const CComVariant connectInfo[5] = {
		user,
		password,
		version,
		instance,
		server,
	};

	for(int i = 0; i < 5; ++i)
	{
		atl2::valid(ipPropertySet->SetProperty(props[i], connectInfo[i]));
	}

	const IWorkspaceName2Ptr ipWorkspaceName(__uuidof(WorkspaceName));
	atl2::valid(ipWorkspaceName->put_WorkspaceFactoryProgID(CComBSTR(_T("esriDataSourcesGDB.SdeWorkspaceFactory"))));
	atl2::valid(ipWorkspaceName->put_ConnectionProperties(ipPropertySet));

	return ipWorkspaceName;
}

/**
* @brief <em>user</em>\@<em>server(TNSサービス名)</em>(<em>version</em>) という形式の接続文字列から WorkspaceName を生成する。
*
* @param lpszConnection [in] 接続文字列。
* @retval 0 接続文字列の解析に失敗した。
* @retval !0 接続文字列から生成された WorkspaceName 。
*/
inline IWorkspaceName2Ptr create_at_style_workspace_string_name([SA_Pre(Null = SA_No, NullTerminated = SA_Yes)] LPCTSTR lpszConnection)
{
	bool bConnectAS = false; // アプリケーションサーバー接続か
	CString pConnection;
	is_connect_string_as(lpszConnection, bConnectAS, pConnection);

	LPCTSTR p1, p2, p3;
	if (!((p1 = ::_tcschr(pConnection, _T('@'))) && (p2 = ::_tcschr(p1 + 1, _T('('))) && (p3 = ::_tcsrchr(p2 + 1, _T(')'))) && (*(p3 + 1) == 0)))
		return 0;

	size_t nUserLen = p1 - pConnection;
	size_t nServerLen = p2 - (p1 + 1);
	size_t nVersionLen = p3 - (p2 + 1);
	if (nUserLen <= 0 || nServerLen <= 0 || nVersionLen <= 0)
		return 0;

	// 接続プロパティの値となる文字列。
	// 最終的に VARIANT に収めなければならないのに CComBSTR を使っているのは、
	// 「文字列先頭アドレス」＋「文字数」による初期化を行なうため。
	const CComBSTR bstrServer(static_cast<int>(nServerLen), p1 + 1);
	const CComBSTR bstrUser(static_cast<int>(nUserLen), pConnection);
	const CComBSTR bstrVersion(static_cast<int>(nVersionLen), p2 + 1);

	// AS接続でない
	if(!bConnectAS)
	{
		// OracleDC接続のサブルーチンを呼び出す
		return create_workspace_name_oracle_dc(
			CComVariant(bstrServer),
			CComVariant(bstrUser),
			CComVariant(bstrUser),
			CComVariant(bstrVersion)
			);
	}
	else
	{
		// AS接続のサブルーチンを呼び出す
		return create_workspace_name_oracle_as(
			CComVariant(bstrServer),
			CComVariant(bstrUser),
			CComVariant(bstrUser),
			CComVariant(bstrVersion),
			CComVariant(_T("5151"))
			);
	}
}

/**
* @brief <em>user</em>/<em>password</em>/<em>version</em>/<em>instance</em>/<em>server(TNSサービス名)</em> という形式の接続文字列から WorkspaceName を生成する。
* @note 「instance」の値は使用されないがフォーマットの下位互換のために残す
*
* @param lpszConnection [in] 接続文字列。
* @retval 0 接続文字列の解析に失敗した。
* @retval !0 接続文字列から生成された WorkspaceName 。
*/
inline IWorkspaceName2Ptr create_slash_style_workspace_string_name([SA_Pre(Null = SA_No, NullTerminated = SA_Yes)] LPCTSTR lpszConnection)
{
	bool bConnectAS = false; // アプリケーションサーバー接続か
	CString pConnection;
	is_connect_string_as(lpszConnection, bConnectAS, pConnection);

	LPCTSTR ps[5] = { 0 };

	// 4つのスラッシュ位置と1つのNULLを求める。
	for (LPCTSTR p1 = pConnection, *p2 = ps; p2 < ps + 5; p1++, p2++) {
		*p2 = p1 = ::_tcschr(p1, _T('/'));
		if (p1 == 0)
			break;
	}

	if (!ps[3] || ps[4])
		return 0;

	typedef std::pair<LPCTSTR, size_t> pair_type;

	// 接続プロパティ値文字列の先頭と文字数を計算。
	const pair_type pairs[5] = {
		pair_type(pConnection, ps[0] - pConnection),
		pair_type((ps[0] + 1), ps[1] - (ps[0] + 1)),
		pair_type((ps[1] + 1), ps[2] - (ps[1] + 1)),
		pair_type((ps[2] + 1), ps[3] - (ps[2] + 1)),
		pair_type((ps[3] + 1), ::_tcslen(ps[3] + 1)),
	};

	// 値の省略は許されない。
	for (int i = 0; i < 5; i++) {
		if (pairs[i].second == 0)
			return 0;
	}
	// AS接続でない
	if(!bConnectAS)
	{
		// OracleDC接続のサブルーチンを呼び出す
		return create_workspace_name_oracle_dc(
			CComVariant(CComBSTR(static_cast<int>(pairs[4].second), pairs[4].first)),
			CComVariant(CComBSTR(static_cast<int>(pairs[0].second), pairs[0].first)),
			CComVariant(CComBSTR(static_cast<int>(pairs[1].second), pairs[1].first)),
			CComVariant(CComBSTR(static_cast<int>(pairs[2].second), pairs[2].first))
			);
	}
	else
	{
		return create_workspace_name_oracle_as(
			CComVariant(CComBSTR(static_cast<int>(pairs[4].second), pairs[4].first)),
			CComVariant(CComBSTR(static_cast<int>(pairs[0].second), pairs[0].first)),
			CComVariant(CComBSTR(static_cast<int>(pairs[1].second), pairs[1].first)),
			CComVariant(CComBSTR(static_cast<int>(pairs[2].second), pairs[2].first)),
			CComVariant(CComBSTR(static_cast<int>(pairs[3].second), pairs[3].first))
			);

	}
}

/**
* @brief <em>user</em>/<em>password</em>/<em>version</em>/<em>server(TNSサービス名)</em> という形式の接続文字列からWorkspaceNameを生成する。
*
* @param lpszConnection [in] 接続文字列。
* @retval 0 接続文字列の解析に失敗した。
* @retval !0 接続文字列から生成された WorkspaceName 。
*/
inline IWorkspaceName2Ptr create_slash_style_workspace_string_name2([SA_Pre(Null = SA_No, NullTerminated = SA_Yes)] LPCTSTR lpszConnection)
{
	// 3つのスラッシュ位置と1つのNULLを求める。
	LPCTSTR ps[4] = { 0 };
	for (LPCTSTR p1 = lpszConnection, *p2 = ps; p2 < ps + 4; p1++, p2++) {
		*p2 = p1 = ::_tcschr(p1, _T('/'));
		if (p1 == 0)
			break;
	}

	// スラッシュが3つでなければ解析失敗
	if (!ps[2] || ps[3])
		return 0;

	typedef std::pair<LPCTSTR, size_t> pair_type;

	// 接続プロパティ値文字列の先頭と文字数を計算。
	const pair_type pairs[4] = {
		pair_type(lpszConnection, ps[0] - lpszConnection),
		pair_type((ps[0] + 1), ps[1] - (ps[0] + 1)),
		pair_type((ps[1] + 1), ps[2] - (ps[1] + 1)),
		pair_type((ps[2] + 1), ::_tcslen(ps[2] + 1))
	};

	// 値の省略は許されない。
	for (int i = 0; i < 4; i++) {
		if (pairs[i].second == 0)
			return 0;
	}

	// OracleDC接続のサブルーチンを呼び出す
	return create_workspace_name_oracle_dc(
		CComVariant(CComBSTR(static_cast<int>(pairs[3].second), pairs[3].first)),
		CComVariant(CComBSTR(static_cast<int>(pairs[0].second), pairs[0].first)),
		CComVariant(CComBSTR(static_cast<int>(pairs[1].second), pairs[1].first)),
		CComVariant(CComBSTR(static_cast<int>(pairs[2].second), pairs[2].first))
	);
}

/**
 * @brief ファイルパスから WorkspaceName を生成する。
 *
 * @param lpszConnection [in] 接続文字列。
 * @retval 0 接続文字列は WorkspaceName を生成可能なファイルパスではない。
 * @retval !0 接続文字列から生成された WorkspaceName 。
 */
inline IWorkspaceName2Ptr create_workspace_file_name([SA_Pre(Null=SA_No, NullTerminated=SA_Yes)] LPCTSTR lpszConnection)
{
	const DWORD dwFileAttr = ::GetFileAttributes(lpszConnection);
	if(dwFileAttr == -1)
		return 0;

	IWorkspaceName2Ptr ipWorkspaceName(__uuidof(WorkspaceName));

	LPCTSTR p = ::_tcsrchr(lpszConnection, _T('.'));

	if(dwFileAttr & FILE_ATTRIBUTE_DIRECTORY) {
		if(p && (::_tcsicmp(p + 1, _T("gdb")) == 0)){
			// 「C:\test.gdb」というディレクトリならGDBと判断
			atl2::valid(ipWorkspaceName->put_WorkspaceFactoryProgID(CComBSTR(_T("esriDataSourcesGDB.FileGDBWorkspaceFactory"))));
		} else {
			// 単なるディレクトリならばSHPと判断
			atl2::valid(ipWorkspaceName->put_WorkspaceFactoryProgID(CComBSTR(_T("esriDataSourcesFile.ShapefileWorkspaceFactory"))));
		}
	}
	else {
		if(! p)
			return 0;

		atl2::valid(ipWorkspaceName.CreateInstance(__uuidof(WorkspaceName)));
		if(::_tcsicmp(p + 1, _T("sde")) == 0) {
			// 拡張子が「sde」なら、SDEへの接続ファイルであると判断。
			atl2::valid(ipWorkspaceName->put_WorkspaceFactoryProgID(CComBSTR(_T("esriDataSourcesGDB.SdeWorkspaceFactory"))));
		}
		else if(::_tcsicmp(p + 1, _T("mdb")) == 0) {
			// 拡張子が「mdb」なら、PGDBファイルであると判断。
			atl2::valid(ipWorkspaceName->put_WorkspaceFactoryProgID(CComBSTR(_T("esriDataSourcesGDB.AccessWorkspaceFactory"))));
		}
		else if(::_tcsicmp(p + 1, _T("pgs")) == 0) {
			// 拡張子が「pgs」なら、PostGISへの接続ファイルであると判断。
			atl2::valid(ipWorkspaceName->put_WorkspaceFactoryProgID(CComBSTR(_T("PostGISExtension.PostGISWorkspaceFactory"))));
		}
		else if(::_tcsicmp(p + 1, _T("xls")) == 0 || ::_tcsicmp(p + 1, _T("xlsx")) == 0) {
			// 拡張子が「xls」or「xlsx」なら、Excelファイルであると判断。
			atl2::valid(ipWorkspaceName->put_WorkspaceFactoryProgID(CComBSTR(_T("esriDataSourcesOleDB.ExcelWorkspaceFactory"))));
		}
		else{
			return 0;
		}
	}

	const CComBSTR bstrPath(lpszConnection);
	atl2::valid(ipWorkspaceName->put_BrowseName(bstrPath));
	atl2::valid(ipWorkspaceName->put_PathName(bstrPath));

	return ipWorkspaceName;
}

/**
 * @brief 色んな種類のワークスペースを開く。
 *
 * @param lpszConnection [in] 接続文字列。
 * @retval 0 ワークスペースの種類を識別できなかった。
 * @retval !0 ワークスペース名。
 * @par 文字列とワークスペースの関係:
 * <table>
 * <tr><td>host=<em>host</em>;port=<em>port</em>;user=<em>user</em>;password=<em>password</em>;dbname=<em>database</em></td><td>SDE</td></tr>
 * <tr><td><em>folder path</em></td><td>Shape</td></tr>
 * <tr><td>*.sde</td><td>SDE</td></tr>
 * <tr><td>*.mdb</td><td>PGDB</td></tr>
 * <tr><td>*.gdb</td><td>FGDB</td></tr>
 * <tr><td>*.pgs</td><td>PostgreSQL</td></tr>
 * <tr><td>*.xls</td><td>Excel</td></tr>
 * <tr><td><em>user</em>\@<em>server</em>(<em>version</em>)</td><td>SDE</td></tr>
 * <tr><td><em>user</em>/<em>password</em>/<em>version</em>/<em>instance</em>/<em>server</em></td><td>SDE</td></tr>
 * </table>
 * @warn Excelの.xlsxは開けないので注意
 *
 * @test
 * CreateWorskpaceNameTest
 */
inline IWorkspaceName2Ptr create_workspace_name([SA_Pre(Null=SA_No, NullTerminated=SA_Yes)] LPCTSTR lpszConnection)
{
	typedef IWorkspaceName2Ptr (*function_type)(LPCTSTR);

	const function_type funcs[] = {
		&create_postgis_workspace_string_name,
		&create_workspace_file_name,
		&create_at_style_workspace_string_name,
		&create_slash_style_workspace_string_name,
		&create_slash_style_workspace_string_name2
	};

	for(int i = 0; i < sizeof(funcs) / sizeof(funcs[0]); i++) {
		if(const IWorkspaceName2Ptr ipWorkspaceName = funcs[i](lpszConnection))
			return ipWorkspaceName;
	}

	return 0;
}

/**
 * @brief create_workspace_name を呼ぶ関数クラス
 *
 * ATL を使用している場合、 MBCS ⇔ UNICODE の相互変換機構がおまけとしてくっついてくる。
 */
struct workspace_name_creator
{
	typedef IWorkspaceName2Ptr result_type;

	IWorkspaceName2Ptr operator()([SA_Pre(Null=SA_No, NullTerminated=SA_Yes)] LPCWSTR lpszConnection) const
	{
#if _ATL_VER >= 0x0700
		return create_workspace_name(ATL::CW2CT(lpszConnection));
#else
		return create_workspace_name(lpszConnection);
#endif // _ATL_VER >= 0x0700
	}

	IWorkspaceName2Ptr operator()([SA_Pre(Null=SA_No, NullTerminated=SA_Yes)] LPCSTR lpszConnection) const
	{
#if _ATL_VER >= 0x0700
		return create_workspace_name(ATL::CA2CT(lpszConnection));
#else
		return create_workspace_name(lpszConnection);
#endif // _ATL_VER >= 0x0700
	}
};

/**
 * @brief 色んな種類のワークスペースを開く
 *
 * @param lpszConnection	[in]	接続文字列
 * @param hErrCode			[out]	エラーコード出力
 * @param bRetry			[in]	true=失敗したらリトライする, false=失敗したら終了（初期値=true）
 * @param lRetryTime		[in]	リトライ回数（初期値=5,最小値=1,最大値=10）
 * @param dSec				[in]	リトライタイミング時間[秒]（初期値=3.0,最小値=0.1,最大値=10.0）
 * @return IWorkspacePtr
 */
inline IWorkspacePtr _create_workspace([SA_Pre(Null=SA_No, NullTerminated=SA_Yes)] LPCTSTR lpszConnection, HRESULT& hErrCode, bool bRetry=true, long lRetryTime=5, double dSec=3)
{
	// 初期化
	hErrCode = S_OK;

	// リトライ回数の最大最小を確認
	if(lRetryTime < 1 || 10 < lRetryTime)
		lRetryTime = 5;
	// リトライタイミングの時間の最大最小を確認
	if(dSec < 0.1 || 10.0 < dSec)
		dSec = 3.0;

	INamePtr ipName(create_workspace_name(lpszConnection));
	if(!ipName)
		return NULL;

	IUnknownPtr ipUnk;
	hErrCode = ipName->Open(&ipUnk);
	IWorkspacePtr ipWorkspace(ipUnk);

	if(ipWorkspace && S_OK == hErrCode)
		return ipWorkspace;

	// エラーコードで処理を変える
	switch(hErrCode){
		// 単にバージョン名が間違っている場合はそのまま終了
		case FDO_E_SE_VERSION_NOEXIST:	//!< -2147215997
			return NULL;
			break;
		// ネットワークIOエラーの場合は再接続するか確認する
		// ネットワークIOエラー以外の場合も再接続させる
		// 今後エラー内容が分かれば細分化する
		case FDO_E_SE_NET_FAILURE: //!< -2147216117
		default:
			break;
	}

	// リトライする場合の処理
	if(bRetry){
		for(long l = 1; l <= lRetryTime; ++l){
			Sleep(static_cast<DWORD>(dSec*1000.0));	// WIN32APIだけどいいよね...
			HRESULT hr = ipName->Open(&ipUnk);
			ipWorkspace = ipUnk;
			if(ipWorkspace && S_OK == hr)
				return ipWorkspace;
		}
	}
	return NULL;
}

/**
 * @brief 色んな種類のワークスペースを開く（エラーコードなし版）
 */
inline IWorkspacePtr create_workspace([SA_Pre(Null=SA_No, NullTerminated=SA_Yes)] LPCTSTR lpszConnection, bool bRetry=true, long lRetryTime=5, double dSec=3)
{
	HRESULT hr = 0;
	return _create_workspace(lpszConnection, hr, bRetry, lRetryTime, dSec);
}

} // namespace sindy

#endif // SINDY_WORKSPACE_NAME_H_
