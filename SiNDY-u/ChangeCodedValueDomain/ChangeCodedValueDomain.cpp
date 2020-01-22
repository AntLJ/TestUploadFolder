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

// ChangeCodedValueDomain.cpp : コンソール アプリケーションのエントリ ポイントを定義します。
//

#include "stdafx.h"
#include <arctl/coinitializer.h>

typedef std::map< long, CString > CODEDVALUEMAP; // コード値と説明のマップ

CString m_strServer; //!< コード値ドメイン出力したいサーバ名
CString m_strOutputFile; //!< コード値ドメイン出力するファイル名
CString m_strFolder; //!< コード値ドメインを変更したいファイルが入っているフォルダ名 
CString m_strChangeDomainServer; //!< コード値ドメインを変更したいサーバ名
CString m_strChangeCodedValueFile; //!< 変更するコード値ドメインのファイル名
std::map< CString, CODEDVALUEMAP > m_mapChangedDomain; //!< ドメイン名をキーとしたコード値ドメインを変更する内容

namespace return_code {
	enum ECode {
		unknown	= -1,		//!< 初期状態
		ok	= 0,			//!< 正常終了
		tool_error = 1,		//!< ツールが出すエラー
		exception_error = 2,//!< 例外エラー
		licence_error = 3,	//!< ライセンス認証失敗
	};
}

// フォルダの中にあるPGDB、FGDBファイルを見つけてくる
void FindFile(HANDLE hFind, WIN32_FIND_DATA& fd, LPCTSTR lpPath, std::list<CString> &listFileName)
{
	do {
		if (lstrcmp(fd.cFileName, _T(".")) == 0 || lstrcmp(fd.cFileName, _T("..")) == 0) {
			// 何もしない
		} else if (fd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) {
			CString cstrPath;
			cstrPath.Format(_T("%s\\%s"), lpPath, fd.cFileName);
			CString strExtention = cstrPath.Right( 4 );

			if (strExtention.CompareNoCase( _T(".gdb") ) == 0  )
			{
				listFileName.push_back(cstrPath);
			}
			else
			{
				CString cstrFind = cstrPath + CString(_T("\\*"));

				WIN32_FIND_DATA fd2;
				HANDLE hFind2 = FindFirstFile(cstrFind, &fd2);
				FindFile(hFind2, fd2, cstrPath, listFileName);
				FindClose(hFind2);
			}
		} else {
			CString cstrName;
			cstrName.Format(_T("%s\\%s"), lpPath, fd.cFileName);
			CString cstrExtension = PathFindExtension(cstrName);
			if (cstrExtension.CompareNoCase( _T(".mdb") ) == 0  )
				listFileName.push_back(cstrName);
		}

	} while (FindNextFile(hFind, &fd));
}

// 使用方法出力関数
bool Usage()
{
	std::cerr << "コード値ドメインの説明を出力・変換するツール" << std::endl;
	std::cerr << "Usage  : ChangeCodedValueDomain"  << std::endl;
	std::cerr << "		-s <コード値ドメイン出力したいサーバ情報>"  << std::endl;
	std::cerr << "		-o <コード値ドメイン出力するファイル名>"  << std::endl;
	std::cerr << "      -f <コード値ドメインを変換したいファイルが入っているフォルダ名>" << std::endl;
	std::cerr << "      -d <コード値ドメインを変換したいサーバ名> " << std::endl;
	std::cerr << "      -c <変換するドメインの対応表> " << std::endl;
	return false;
}

// 初期化
int Init( int argc, _TCHAR* argv[] )
{
	if (argc <= 1)
		return Usage();

	//TODO このあたりboost::program_optionsでやるべき
	while (--argc)
	{
		_TCHAR *s = *++argv;
		switch (*s)
		{
		case '-':
			switch (*++s)
			{
			case 's':
				m_strServer = *++argv;
				--argc;
				break;
			case 'o':
				m_strOutputFile = *++argv;
				--argc;
				break;
			case 'f':
				m_strFolder = *++argv;
				--argc;
				break;
			case 'd':
				m_strChangeDomainServer = *++argv;
				--argc;
				break;
			case 'c':
				m_strChangeCodedValueFile = *++argv;
				--argc;
				break;
			default:
				return Usage();
			}
			break;
		default:
			break;
		}
	}

	if( !m_strServer.IsEmpty() )
	{
		if( m_strOutputFile.IsEmpty() )
		{
			std::cerr << "コード値ドメイン出力モードの場合には、-oオプションで出力ファイル名を指定してください" << std::endl;
			return false;			
		}
	}

	if( !m_strFolder.IsEmpty() && !m_strChangeDomainServer.IsEmpty() )
	{
		std::cerr << "コード値ドメイン変換モードの場合には、-f、-dのオプションどちらかを指定してください" << std::endl;
		return false;			
	}

	return true;
}

// コード値を変換するファイルの内容読み取り
bool ReadChangeDomainFile()
{
	std::ifstream file(m_strChangeCodedValueFile);
	if (! file.is_open())
	{
		std::cerr << "ファイルが開けませんでした" << std::endl;
		return false;
	}

	while (file.good())
	{
		char szBuffer[2001];
		file.getline( szBuffer, 2000 );
		// カンマ区切りであること前提
		CString strLine = szBuffer;
		CString strDomainName = strLine.Left( strLine.Find( _T(",") ) );
		strLine = strLine.Right( strLine.GetLength() - strLine.Find( _T(",") ) -1 );
		CString strDomainValue = strLine.Left( strLine.Find( _T(",") ) );
		strLine = strLine.Right( strLine.GetLength() - strLine.Find( _T(",") ) -1 );
		CString strNewExpalin = strLine;

		if( strDomainName.IsEmpty() || strDomainValue.IsEmpty() || strNewExpalin.IsEmpty() )
			continue;

		CODEDVALUEMAP mapNewDomainMap = m_mapChangedDomain[ strDomainName ];
		mapNewDomainMap[ _ttol( strDomainValue ) ] = strNewExpalin;

		m_mapChangedDomain[ strDomainName ] = mapNewDomainMap;
	}
	return true;
}

// 実体
int _tmain(int argc, _TCHAR* argv[])
{
	int retcode = return_code::unknown;

	try{
		const arctl::coinitializer aCoInitializer;
		::setlocale(LC_ALL, "Japanese");
		
		try
		{
			if( !Init( argc, argv ) )
				return return_code::tool_error;

			// コード値を出力するモード
			if( !m_strServer.IsEmpty() )
			{
				// コード値ドメインの値をすべて出力するには、このコメントアウト外す
				// 現在のコード値ドメイン出力するモード作成すべきかな
				FILE *fStream = NULL;
				_wfopen_s( &fStream, m_strOutputFile, _T("a") );

				IWorkspacePtr ipWorkspace = sindy::create_workspace( m_strServer );
				IWorkspaceDomains3Ptr ipWorkspaceDomain = ipWorkspace;
				IEnumDomainPtr ipEnumDomain;
				ipWorkspaceDomain->get_Domains( &ipEnumDomain );
				IDomainPtr ipDomain;
				long l = 0;
				while( ipEnumDomain->Next( &ipDomain ) == S_OK )
				{
					CComBSTR bstrDomainName;
					ipDomain->get_Name( &bstrDomainName );
					std::map< long, CString > mapDomain = m_mapChangedDomain[ CString( bstrDomainName ) ];

					ICodedValueDomain2Ptr ipCodedValueDomain = ipDomain;
					if( ipCodedValueDomain )
					{
						long lCount = 0;
						ipCodedValueDomain->get_CodeCount( &lCount );

						for( int i = 0; i < lCount; ++i )
						{
							CComVariant vaValue;
							CComBSTR bstrName;
							ipCodedValueDomain->get_Name( i, &bstrName );
							ipCodedValueDomain->get_Value( i, &vaValue );
							CString strValue;
							strValue.Format( _T("%s,%ld,%s\n"), bstrDomainName, vaValue.lVal, bstrName );
							fwprintf( fStream, strValue );

						}
					}
				}
				fclose( fStream );
			}
			// コード値を変換するモード
			else
			{
				// 変更するコード値ドメインファイル設定の読み込み
				if( !ReadChangeDomainFile() )
					return return_code::tool_error;

				std::list< CString > listChangeServerName;
				// PGDBとかFGDBとかフォルダ内のものを一括でやる場合
				if( m_strChangeDomainServer.IsEmpty() )
				{
 					// フォルダ内のファイル検索する
					WIN32_FIND_DATA		ffd;
					CString				strDirPath;
					strDirPath = m_strFolder + CString( _T("\\*") );

					HANDLE hFile = FindFirstFile( strDirPath, &ffd );
					if ( hFile == INVALID_HANDLE_VALUE )
						return return_code::tool_error;	// ファイルが存在しない？
					// objファイル見つけてくる
					FindFile( hFile, ffd, m_strFolder, listChangeServerName );
					FindClose(hFile);
				}
				// サーバの場合はサーバへの接続をリストに突っ込んでおく
				else
					listChangeServerName.push_back( m_strChangeDomainServer );

				for( std::list< CString >::const_iterator it = listChangeServerName.begin(); it != listChangeServerName.end(); ++it )
				{
					// ワークスペース取得して、コード値ドメインすべて取得
					IWorkspacePtr ipWorkspace = sindy::create_workspace( *it );
					IWorkspaceDomains3Ptr ipWorkspaceDomain = ipWorkspace;
					IEnumDomainPtr ipEnumDomain;
					ipWorkspaceDomain->get_Domains( &ipEnumDomain );
					IDomainPtr ipDomain;
					while( ipEnumDomain->Next( &ipDomain ) == S_OK )
					{
						CComBSTR bstrDomainName;
						ipDomain->get_Name( &bstrDomainName );
						// 変換対象のコード値ドメインを取得する
						std::map< CString, CODEDVALUEMAP >::const_iterator mapDomain = m_mapChangedDomain.find( CString( bstrDomainName ) );
						if( mapDomain == m_mapChangedDomain.end() )
							continue;

						ICodedValueDomain2Ptr ipCodedValueDomain = ipDomain;
						if( ipCodedValueDomain )
						{
							// コード値をいったん削除
							// ファイルで指定しているものだけなので、指定していないものに対しては変換はかからない
							for( CODEDVALUEMAP::const_iterator it = mapDomain->second.begin(); it != mapDomain->second.end(); ++it )
							{
								HRESULT hr = ipCodedValueDomain->DeleteCode( _variant_t( it->first )  );
							}
							// ファイルで指定したコード値を追加する
							for( CODEDVALUEMAP::const_iterator it = mapDomain->second.begin(); it != mapDomain->second.end(); ++it )
							{
								HRESULT hr = ipCodedValueDomain->AddCode( _variant_t( it->first ), _bstr_t( it->second ) );
							}

							// 昇順でソートしておく
							ipCodedValueDomain->SortByValue( VARIANT_FALSE );
							// コード値更新
							HRESULT hr = ipWorkspaceDomain->AlterDomain( (IDomainPtr)ipCodedValueDomain  );
						}
					}
				}
			}
			retcode = return_code::ok;

		}
		catch (const _com_error& com_err)
		{
			// ツール例外エラー
			std::cerr << CT2A( com_err.ErrorMessage() ) << std::endl;
			retcode = return_code::exception_error;
		}
	}
	catch(const std::exception &e)
	{
		// ライセンス認証失敗
		std::cerr << e.what() << std::endl;
		retcode = return_code::licence_error;
	}

	if( retcode == return_code::ok )
		std::cerr << "正常終了"  << std::endl;
	else
		std::cerr << "異常終了"  << std::endl;

	return retcode;
}

