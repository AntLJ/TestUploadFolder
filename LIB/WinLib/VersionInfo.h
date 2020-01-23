// バージョンリソースアクセスクラス
// (C)2002,2003 Yutaka Wada, AirparkLab

/* 使い方

    CVersion Version;
    CString str = Version.GetInternalName();

    などとしてバージョンリソースを文字列で取得できる。
    バージョンリソースはリソースエディタで記述する。

    パブリックメンバ関数は以下の通り。

    CVersion()                          自分自身のバージョンリソースにアクセスする場合のコンストラクタ
    CVersion( CString Filename )        指定した実行ファイルのバージョンリソースにアクセスする場合のコンストラクタ
    CString GetFileVersion()            ファイルバージョンの取得
    CString GetProductVersion()         プロダクトバージョンの取得
    CString GetComments()               コメントの取得
    CString GetCompanyName()            会社名の取得
    CString GetFileDescription()        ファイルの説明の取得
    CString GetInternalName()           内部名の取得
    CString GetLegalCopyright()         著作権表示の取得
    CString GetLegalTrademarks()        トレードマークの取得
    CString GetOriginalFilename()       オリジナルファイル名の取得
    CString GetPrivateBuild()           プライベートビルドの取得
    CString GetProductName()            製品名の取得
    CString GetSpecialBuild()           スペシャルビルドの取得
    ~CVersion()                         デストラクタ

*/



#ifndef __VERSION_H__
#define __VERSION_H__



#include "winver.h"
#pragma comment( linker, "/DEFAULTLIB:version.lib" )



class CVersion {

  VOID *pData;
  WORD *pLang;
  VS_FIXEDFILEINFO *pFixedFileInfo;

  VOID init( LPCTSTR filename )
  {
	pData = NULL;
	pLang = NULL;
	pFixedFileInfo = NULL;

	fullPathName = filename;
    DWORD size = GetFileVersionInfoSize( (LPTSTR)filename, NULL );
    pData = new BYTE[size];

    UINT len;
    if( GetFileVersionInfo( (LPTSTR)filename, NULL, size, pData ) ){
       VerQueryValue( pData, _T("\\"), (LPVOID *)&pFixedFileInfo, &len );
       VerQueryValue( pData, _T("\\VarFileInfo\\Translation"), (LPVOID *)&pLang, &len );
    }
  }

public:

  CVersion()
  {
    //HINSTANCE hInstance = AfxGetApp()->m_hInstance;
    TCHAR szFilename[MAX_PATH];
    GetModuleFileName( NULL, szFilename, MAX_PATH );
    init( (LPTSTR)szFilename );
  }

  CVersion( LPCTSTR Filename )
  {
    init( Filename );
  }

  ~CVersion()
  {
    delete [] pData;
  }

  CString fullPathName;
  CString GetfullPath() const { return fullPathName; }

  CString GetFileVersion()
  {
    if( !pData || !pFixedFileInfo) 
		return( _T("") );
    
    CString str;
    str.Format( _T("%d.%d.%d.%d"),
               HIWORD( pFixedFileInfo->dwFileVersionMS ),
               LOWORD( pFixedFileInfo->dwFileVersionMS ),
               HIWORD( pFixedFileInfo->dwFileVersionLS ),
               LOWORD( pFixedFileInfo->dwFileVersionLS ) );
    return( str );
  }

  CString GetProductVersion()
  {
    if( !pData || !pFixedFileInfo) 
		return( _T("") );

    CString str;
    str.Format( _T("%d.%d.%d.%d"),
               HIWORD( pFixedFileInfo->dwProductVersionMS ),
               LOWORD( pFixedFileInfo->dwProductVersionMS ),
               HIWORD( pFixedFileInfo->dwProductVersionLS ),
               LOWORD( pFixedFileInfo->dwProductVersionLS ) );
    return( str );
  }

  CString GetComments()
  {
    if( pData == NULL ) return( _T("") );
    
    CString SubBlock;
    SubBlock.Format( _T("\\StringFileInfo\\%04X%04X\\Comments"), *pLang, *(pLang+1) );
    
    TCHAR *p;
    UINT len;
    VerQueryValue( pData, (LPTSTR)(LPCTSTR)SubBlock, (LPVOID *)&p, &len );
    
    CString str;
    if( len > 0 ) str = p;
    else str = _T("");
    
    return( str );
  }

  CString GetCompanyName()
  {
    if( pData == NULL ) return( _T("") );
    
    CString SubBlock;
    SubBlock.Format( _T("\\StringFileInfo\\%04X%04X\\CompanyName"), *pLang, *(pLang+1) );
    
    TCHAR *p;
    UINT len;
    VerQueryValue( pData, (LPTSTR)(LPCTSTR)SubBlock, (LPVOID *)&p, &len );
    
    CString str;
    if( len > 0 ) str = p;
    else str = "";
    
    return( str );
  }

  CString GetFileDescription()
  {
    if( pData == NULL ) return( _T("") );
    
    CString SubBlock;
    SubBlock.Format( _T("\\StringFileInfo\\%04X%04X\\FileDescription"), *pLang, *(pLang+1) );
    
    TCHAR *p;
    UINT len;
    VerQueryValue( pData, (LPTSTR)(LPCTSTR)SubBlock, (LPVOID *)&p, &len );
    
    CString str;
    if( len > 0 ) str = p;
    else str = _T("");
    
    return( str );
  }

  CString GetInternalName()
  {
    if( pData == NULL ) return( _T("") );
    
    CString SubBlock;
    SubBlock.Format( _T("\\StringFileInfo\\%04X%04X\\InternalName"), *pLang, *(pLang+1) );
    
    TCHAR *p;
    UINT len;
    VerQueryValue( pData, (LPTSTR)(LPCTSTR)SubBlock, (LPVOID *)&p, &len );
    
    CString str;
    if( len > 0 ) str = p;
    else str = _T("");
    
    return( str );
  }

  CString GetLegalCopyright()
  {
    if( pData == NULL ) return( _T("") );
    
    CString SubBlock;
    SubBlock.Format( _T("\\StringFileInfo\\%04X%04X\\LegalCopyright"), *pLang, *(pLang+1) );
    
    TCHAR *p;
    UINT len;
    VerQueryValue( pData, (LPTSTR)(LPCTSTR)SubBlock, (LPVOID *)&p, &len );
    
    CString str;
    if( len > 0 ) str = p;
    else str = _T("");
    
    return( str );
  }

  CString GetLegalTrademarks()
  {
    if( pData == NULL ) return( _T("") );
    
    CString SubBlock;
    SubBlock.Format( _T("\\StringFileInfo\\%04X%04X\\LegalTrademarks"), *pLang, *(pLang+1) );
    
    TCHAR *p;
    UINT len;
    VerQueryValue( pData, (LPTSTR)(LPCTSTR)SubBlock, (LPVOID *)&p, &len );
    
    CString str;
    if( len > 0 ) str = p;
    else str = "";
    
    return( str );
  }

  CString GetOriginalFilename()
  {
    if( pData == NULL ) return( _T("") );
    
    CString SubBlock;
    SubBlock.Format( _T("\\StringFileInfo\\%04X%04X\\OriginalFilename"), *pLang, *(pLang+1) );
    
    TCHAR *p;
    UINT len;
    VerQueryValue( pData, (LPTSTR)(LPCTSTR)SubBlock, (LPVOID *)&p, &len );
    
    CString str;
    if( len > 0 ) str = p;
    else str = _T("");
    
    return( str );
  }

  CString GetPrivateBuild()
  {
    if( pData == NULL ) return( _T("") );
    
    CString SubBlock;
    SubBlock.Format( _T("\\StringFileInfo\\%04X%04X\\PrivateBuild"), *pLang, *(pLang+1) );
    
    TCHAR *p;
    UINT len;
    VerQueryValue( pData, (LPTSTR)(LPCTSTR)SubBlock, (LPVOID *)&p, &len );
    
    CString str;
    if( len > 0 ) str = p;
    else str = "";
    
    return( str );
  }

  CString GetProductName()
  {
    if( pData == NULL ) return( _T("") );
    
    CString SubBlock;
    SubBlock.Format( _T("\\StringFileInfo\\%04X%04X\\ProductName"), *pLang, *(pLang+1) );
    
    TCHAR *p;
    UINT len;
    VerQueryValue( pData, (LPTSTR)(LPCTSTR)SubBlock, (LPVOID *)&p, &len );
    
    CString str;
    if( len > 0 ) str = p;
    else str = _T("");
    
    return( str );
  }

  CString GetSpecialBuild()
  {
    if( pData == NULL ) return( _T("") );
    
    CString SubBlock;
    SubBlock.Format( _T("\\StringFileInfo\\%04X%04X\\SpecialBuild"), *pLang, *(pLang+1) );
    
    TCHAR *p;
    UINT len;
    VerQueryValue( pData, (LPTSTR)(LPCTSTR)SubBlock, (LPVOID *)&p, &len );
    
    CString str;
    if( len > 0 ) str = p;
    else str = _T("");
    
    return( str );
  }

  CString GetCustomInfo( LPCTSTR lpcszName )
  {
    if( pData == NULL ) return( _T("") );
    
    CString SubBlock;
    SubBlock.Format( _T("\\StringFileInfo\\%04X%04X\\%s"), *pLang, *(pLang+1), lpcszName );
    
    TCHAR *p;
    UINT len;
    VerQueryValue( pData, (LPTSTR)(LPCTSTR)SubBlock, (LPVOID *)&p, &len );
    
    CString str;
    if( len > 0 ) str = p;
    else str = _T("");
    
    return( str );
  }
};



#endif