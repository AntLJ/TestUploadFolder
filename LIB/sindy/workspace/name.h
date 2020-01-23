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
	CString strPrefConnect_AS = _T("[AS]"); //!< �A�v���P�[�V�����T�[�o�[�ڑ���Workspace���J���ꍇ�́A�ڑ�������ɂ���ړ���("[AS]trial@coral2(SDE.DEFAULT)"�ȂǂƎw�肷��)

	// �ړ�����"[AS]"�ł���΁AAS�ڑ��Ɣ��f
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
 * @brief <td>host=<em>host</em>;port=<em>port</em>;user=<em>user</em>;password=<em>password</em>;dbname=<em>database</em> �Ƃ����`����PostGIS�ڑ������񂩂� WorkspaceName �𐶐�����B
 *
 * @param lpszConnection [in] �ڑ�������B
 * @retval 0 �ڑ�������̉�͂Ɏ��s�����B
 * @retval !0 �ڑ������񂩂琶�����ꂽ WorkspaceName �B
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
* @brief Oracle�_�C���N�g�R�l�N�V������ WorkspaceName �𐶐�
*
* @param tnsName	[in] TNS�T�[�r�X��
* @param user		[in] ���[�U
* @param password	[in] �p�X���[�h
* @param version	[in] �o�[�W����
* @return �ڑ������񂩂琶�����ꂽ WorkspaceName �B
*/
inline IWorkspaceName2Ptr create_workspace_name_oracle_dc(const CComVariant& tnsName, const CComVariant& user, const CComVariant& password, const CComVariant& version)
{
	// SDE�p��WorkspaceName�𐶐����A�ڑ��v���p�e�B��ݒ肷��B
	const IPropertySetPtr ipPropertySet(__uuidof(PropertySet));

	// DBCLIENT, AUTHENTICATION_MODE�v���p�e�B�͌��ߑł�
	atl2::valid(ipPropertySet->SetProperty(CComBSTR(_T("DBCLIENT")), CComVariant(_T("oracle"))));
	atl2::valid(ipPropertySet->SetProperty(CComBSTR(_T("AUTHENTICATION_MODE")), CComVariant(_T("DBMS"))));

	// �e�����̒l���v���p�e�B�ɃZ�b�g
	atl2::valid(ipPropertySet->SetProperty(CComBSTR(_T("DB_CONNECTION_PROPERTIES")), tnsName));
	atl2::valid(ipPropertySet->SetProperty(CComBSTR(_T("USER")), user));
	atl2::valid(ipPropertySet->SetProperty(CComBSTR(_T("PASSWORD")), password));
	atl2::valid(ipPropertySet->SetProperty(CComBSTR(_T("VERSION")), version));

	// IWorkspaceName�𐶐����ĕԂ�
	const IWorkspaceName2Ptr ipWorkspaceName(__uuidof(WorkspaceName));
	atl2::valid(ipWorkspaceName->put_WorkspaceFactoryProgID(CComBSTR(_T("esriDataSourcesGDB.SdeWorkspaceFactory"))));
	atl2::valid(ipWorkspaceName->put_ConnectionProperties(ipPropertySet));
	return ipWorkspaceName;
}

/**
* @brief Oracle�A�v���P�[�V�����T�[�o�[�ڑ��� WorkspaceName �𐶐�
*
* @param server		[in] �T�[�o�[��
* @param user		[in] ���[�U
* @param password	[in] �p�X���[�h
* @param version	[in] �o�[�W����
* @param instance	[in] �C���X�^���X
* @return �ڑ������񂩂琶�����ꂽ WorkspaceName �B
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
* @brief <em>user</em>\@<em>server(TNS�T�[�r�X��)</em>(<em>version</em>) �Ƃ����`���̐ڑ������񂩂� WorkspaceName �𐶐�����B
*
* @param lpszConnection [in] �ڑ�������B
* @retval 0 �ڑ�������̉�͂Ɏ��s�����B
* @retval !0 �ڑ������񂩂琶�����ꂽ WorkspaceName �B
*/
inline IWorkspaceName2Ptr create_at_style_workspace_string_name([SA_Pre(Null = SA_No, NullTerminated = SA_Yes)] LPCTSTR lpszConnection)
{
	bool bConnectAS = false; // �A�v���P�[�V�����T�[�o�[�ڑ���
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

	// �ڑ��v���p�e�B�̒l�ƂȂ镶����B
	// �ŏI�I�� VARIANT �Ɏ��߂Ȃ���΂Ȃ�Ȃ��̂� CComBSTR ���g���Ă���̂́A
	// �u������擪�A�h���X�v�{�u�������v�ɂ�鏉�������s�Ȃ����߁B
	const CComBSTR bstrServer(static_cast<int>(nServerLen), p1 + 1);
	const CComBSTR bstrUser(static_cast<int>(nUserLen), pConnection);
	const CComBSTR bstrVersion(static_cast<int>(nVersionLen), p2 + 1);

	// AS�ڑ��łȂ�
	if(!bConnectAS)
	{
		// OracleDC�ڑ��̃T�u���[�`�����Ăяo��
		return create_workspace_name_oracle_dc(
			CComVariant(bstrServer),
			CComVariant(bstrUser),
			CComVariant(bstrUser),
			CComVariant(bstrVersion)
			);
	}
	else
	{
		// AS�ڑ��̃T�u���[�`�����Ăяo��
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
* @brief <em>user</em>/<em>password</em>/<em>version</em>/<em>instance</em>/<em>server(TNS�T�[�r�X��)</em> �Ƃ����`���̐ڑ������񂩂� WorkspaceName �𐶐�����B
* @note �uinstance�v�̒l�͎g�p����Ȃ����t�H�[�}�b�g�̉��ʌ݊��̂��߂Ɏc��
*
* @param lpszConnection [in] �ڑ�������B
* @retval 0 �ڑ�������̉�͂Ɏ��s�����B
* @retval !0 �ڑ������񂩂琶�����ꂽ WorkspaceName �B
*/
inline IWorkspaceName2Ptr create_slash_style_workspace_string_name([SA_Pre(Null = SA_No, NullTerminated = SA_Yes)] LPCTSTR lpszConnection)
{
	bool bConnectAS = false; // �A�v���P�[�V�����T�[�o�[�ڑ���
	CString pConnection;
	is_connect_string_as(lpszConnection, bConnectAS, pConnection);

	LPCTSTR ps[5] = { 0 };

	// 4�̃X���b�V���ʒu��1��NULL�����߂�B
	for (LPCTSTR p1 = pConnection, *p2 = ps; p2 < ps + 5; p1++, p2++) {
		*p2 = p1 = ::_tcschr(p1, _T('/'));
		if (p1 == 0)
			break;
	}

	if (!ps[3] || ps[4])
		return 0;

	typedef std::pair<LPCTSTR, size_t> pair_type;

	// �ڑ��v���p�e�B�l������̐擪�ƕ��������v�Z�B
	const pair_type pairs[5] = {
		pair_type(pConnection, ps[0] - pConnection),
		pair_type((ps[0] + 1), ps[1] - (ps[0] + 1)),
		pair_type((ps[1] + 1), ps[2] - (ps[1] + 1)),
		pair_type((ps[2] + 1), ps[3] - (ps[2] + 1)),
		pair_type((ps[3] + 1), ::_tcslen(ps[3] + 1)),
	};

	// �l�̏ȗ��͋�����Ȃ��B
	for (int i = 0; i < 5; i++) {
		if (pairs[i].second == 0)
			return 0;
	}
	// AS�ڑ��łȂ�
	if(!bConnectAS)
	{
		// OracleDC�ڑ��̃T�u���[�`�����Ăяo��
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
* @brief <em>user</em>/<em>password</em>/<em>version</em>/<em>server(TNS�T�[�r�X��)</em> �Ƃ����`���̐ڑ������񂩂�WorkspaceName�𐶐�����B
*
* @param lpszConnection [in] �ڑ�������B
* @retval 0 �ڑ�������̉�͂Ɏ��s�����B
* @retval !0 �ڑ������񂩂琶�����ꂽ WorkspaceName �B
*/
inline IWorkspaceName2Ptr create_slash_style_workspace_string_name2([SA_Pre(Null = SA_No, NullTerminated = SA_Yes)] LPCTSTR lpszConnection)
{
	// 3�̃X���b�V���ʒu��1��NULL�����߂�B
	LPCTSTR ps[4] = { 0 };
	for (LPCTSTR p1 = lpszConnection, *p2 = ps; p2 < ps + 4; p1++, p2++) {
		*p2 = p1 = ::_tcschr(p1, _T('/'));
		if (p1 == 0)
			break;
	}

	// �X���b�V����3�łȂ���Ή�͎��s
	if (!ps[2] || ps[3])
		return 0;

	typedef std::pair<LPCTSTR, size_t> pair_type;

	// �ڑ��v���p�e�B�l������̐擪�ƕ��������v�Z�B
	const pair_type pairs[4] = {
		pair_type(lpszConnection, ps[0] - lpszConnection),
		pair_type((ps[0] + 1), ps[1] - (ps[0] + 1)),
		pair_type((ps[1] + 1), ps[2] - (ps[1] + 1)),
		pair_type((ps[2] + 1), ::_tcslen(ps[2] + 1))
	};

	// �l�̏ȗ��͋�����Ȃ��B
	for (int i = 0; i < 4; i++) {
		if (pairs[i].second == 0)
			return 0;
	}

	// OracleDC�ڑ��̃T�u���[�`�����Ăяo��
	return create_workspace_name_oracle_dc(
		CComVariant(CComBSTR(static_cast<int>(pairs[3].second), pairs[3].first)),
		CComVariant(CComBSTR(static_cast<int>(pairs[0].second), pairs[0].first)),
		CComVariant(CComBSTR(static_cast<int>(pairs[1].second), pairs[1].first)),
		CComVariant(CComBSTR(static_cast<int>(pairs[2].second), pairs[2].first))
	);
}

/**
 * @brief �t�@�C���p�X���� WorkspaceName �𐶐�����B
 *
 * @param lpszConnection [in] �ڑ�������B
 * @retval 0 �ڑ�������� WorkspaceName �𐶐��\�ȃt�@�C���p�X�ł͂Ȃ��B
 * @retval !0 �ڑ������񂩂琶�����ꂽ WorkspaceName �B
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
			// �uC:\test.gdb�v�Ƃ����f�B���N�g���Ȃ�GDB�Ɣ��f
			atl2::valid(ipWorkspaceName->put_WorkspaceFactoryProgID(CComBSTR(_T("esriDataSourcesGDB.FileGDBWorkspaceFactory"))));
		} else {
			// �P�Ȃ�f�B���N�g���Ȃ��SHP�Ɣ��f
			atl2::valid(ipWorkspaceName->put_WorkspaceFactoryProgID(CComBSTR(_T("esriDataSourcesFile.ShapefileWorkspaceFactory"))));
		}
	}
	else {
		if(! p)
			return 0;

		atl2::valid(ipWorkspaceName.CreateInstance(__uuidof(WorkspaceName)));
		if(::_tcsicmp(p + 1, _T("sde")) == 0) {
			// �g���q���usde�v�Ȃ�ASDE�ւ̐ڑ��t�@�C���ł���Ɣ��f�B
			atl2::valid(ipWorkspaceName->put_WorkspaceFactoryProgID(CComBSTR(_T("esriDataSourcesGDB.SdeWorkspaceFactory"))));
		}
		else if(::_tcsicmp(p + 1, _T("mdb")) == 0) {
			// �g���q���umdb�v�Ȃ�APGDB�t�@�C���ł���Ɣ��f�B
			atl2::valid(ipWorkspaceName->put_WorkspaceFactoryProgID(CComBSTR(_T("esriDataSourcesGDB.AccessWorkspaceFactory"))));
		}
		else if(::_tcsicmp(p + 1, _T("pgs")) == 0) {
			// �g���q���upgs�v�Ȃ�APostGIS�ւ̐ڑ��t�@�C���ł���Ɣ��f�B
			atl2::valid(ipWorkspaceName->put_WorkspaceFactoryProgID(CComBSTR(_T("PostGISExtension.PostGISWorkspaceFactory"))));
		}
		else if(::_tcsicmp(p + 1, _T("xls")) == 0 || ::_tcsicmp(p + 1, _T("xlsx")) == 0) {
			// �g���q���uxls�vor�uxlsx�v�Ȃ�AExcel�t�@�C���ł���Ɣ��f�B
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
 * @brief �F��Ȏ�ނ̃��[�N�X�y�[�X���J���B
 *
 * @param lpszConnection [in] �ڑ�������B
 * @retval 0 ���[�N�X�y�[�X�̎�ނ����ʂł��Ȃ������B
 * @retval !0 ���[�N�X�y�[�X���B
 * @par ������ƃ��[�N�X�y�[�X�̊֌W:
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
 * @warn Excel��.xlsx�͊J���Ȃ��̂Œ���
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
 * @brief create_workspace_name ���ĂԊ֐��N���X
 *
 * ATL ���g�p���Ă���ꍇ�A MBCS �� UNICODE �̑��ݕϊ��@�\�����܂��Ƃ��Ă������Ă���B
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
 * @brief �F��Ȏ�ނ̃��[�N�X�y�[�X���J��
 *
 * @param lpszConnection	[in]	�ڑ�������
 * @param hErrCode			[out]	�G���[�R�[�h�o��
 * @param bRetry			[in]	true=���s�����烊�g���C����, false=���s������I���i�����l=true�j
 * @param lRetryTime		[in]	���g���C�񐔁i�����l=5,�ŏ��l=1,�ő�l=10�j
 * @param dSec				[in]	���g���C�^�C�~���O����[�b]�i�����l=3.0,�ŏ��l=0.1,�ő�l=10.0�j
 * @return IWorkspacePtr
 */
inline IWorkspacePtr _create_workspace([SA_Pre(Null=SA_No, NullTerminated=SA_Yes)] LPCTSTR lpszConnection, HRESULT& hErrCode, bool bRetry=true, long lRetryTime=5, double dSec=3)
{
	// ������
	hErrCode = S_OK;

	// ���g���C�񐔂̍ő�ŏ����m�F
	if(lRetryTime < 1 || 10 < lRetryTime)
		lRetryTime = 5;
	// ���g���C�^�C�~���O�̎��Ԃ̍ő�ŏ����m�F
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

	// �G���[�R�[�h�ŏ�����ς���
	switch(hErrCode){
		// �P�Ƀo�[�W���������Ԉ���Ă���ꍇ�͂��̂܂܏I��
		case FDO_E_SE_VERSION_NOEXIST:	//!< -2147215997
			return NULL;
			break;
		// �l�b�g���[�NIO�G���[�̏ꍇ�͍Đڑ����邩�m�F����
		// �l�b�g���[�NIO�G���[�ȊO�̏ꍇ���Đڑ�������
		// ����G���[���e��������΍ו�������
		case FDO_E_SE_NET_FAILURE: //!< -2147216117
		default:
			break;
	}

	// ���g���C����ꍇ�̏���
	if(bRetry){
		for(long l = 1; l <= lRetryTime; ++l){
			Sleep(static_cast<DWORD>(dSec*1000.0));	// WIN32API�����ǂ������...
			HRESULT hr = ipName->Open(&ipUnk);
			ipWorkspace = ipUnk;
			if(ipWorkspace && S_OK == hr)
				return ipWorkspace;
		}
	}
	return NULL;
}

/**
 * @brief �F��Ȏ�ނ̃��[�N�X�y�[�X���J���i�G���[�R�[�h�Ȃ��Łj
 */
inline IWorkspacePtr create_workspace([SA_Pre(Null=SA_No, NullTerminated=SA_Yes)] LPCTSTR lpszConnection, bool bRetry=true, long lRetryTime=5, double dSec=3)
{
	HRESULT hr = 0;
	return _create_workspace(lpszConnection, hr, bRetry, lRetryTime, dSec);
}

} // namespace sindy

#endif // SINDY_WORKSPACE_NAME_H_
