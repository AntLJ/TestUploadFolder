#include "AheGlobals.h"
/**
* @file InifileHelper.inl
*
* @brief IniFile関連を操作するクラス群のインライン関数ファイル
*/
inline CInifileHelperRoot::CInifileHelperRoot( LPCTSTR lpcszPath)
{
	m_cStrInifile = lpcszPath;
}

inline CInifileHelperRoot::~CInifileHelperRoot()
{
}

/// m_cStrInifileが定義されているか？のチェック
inline BOOL CInifileHelperRoot::IsEmpty()
{
	return m_cStrInifile.IsEmpty();
}

/// 固有設定ファイルからの数値取得関数
inline int CInifileHelperRoot::GetPrivateProfileInt( LPCTSTR lpAppName, LPCTSTR lpKeyName, int nDefaule)
{
	ATLASSERT( !m_cStrInifile.IsEmpty());
	return ::GetPrivateProfileInt( lpAppName, lpKeyName, nDefaule, m_cStrInifile);
}

/// 固有設定ファイルからの文字列取得関数
inline DWORD CInifileHelperRoot::GetPrivateProfileString( LPCTSTR lpAppName, LPCTSTR lpKeyName, LPCTSTR lpDefault, CString& cString, int nSize)
{
	ATLASSERT( !m_cStrInifile.IsEmpty());
	DWORD dwResult = ::GetPrivateProfileString( lpAppName, lpKeyName, lpDefault, cString.GetBuffer( nSize), nSize, m_cStrInifile);
	cString.ReleaseBuffer();
	return dwResult;
}

/// 固有設定ファイルからの数値設定関数
inline DWORD CInifileHelperRoot::WritePrivateProfileInt( LPCTSTR lpAppName, LPCTSTR lpKeyName, int nData)
{
	CString cStr;
	cStr.Format( TEXT( "%ld"), nData);
	return ::WritePrivateProfileString( lpAppName, lpKeyName, cStr, m_cStrInifile);
}

/// 固有設定ファイルからの文字列設定関数
inline DWORD CInifileHelperRoot::WritePrivateProfileString( LPCTSTR lpAppName, LPCTSTR lpKeyName, LPCTSTR lpcszString)
{
	return ::WritePrivateProfileString( lpAppName, lpKeyName, lpcszString, m_cStrInifile);
}

/// 管理対象のINIファイル取得関数
inline void CInifileHelperRoot::GetPrivateProfilePath( CString& cStrPath)
{
	cStrPath = m_cStrInifile;
}

/// 管理対象のINIファイル設定関数
inline void CInifileHelperRoot::SetPrivateProfilePath( LPCTSTR lpszPath)
{
	m_cStrInifile = lpszPath;
}


/// ラスタ定義ファイルアクセスクラス
inline CRasterInifileHelper::CRasterInifileHelper( LPCTSTR lpcszPath) : CInifileHelperRoot( lpcszPath)
{
}

inline CRasterInifileHelper::~CRasterInifileHelper()
{
}

/// ベースディレクトリパス取得関数
inline void CRasterInifileHelper::GetBaseDir( CString& cStrBaseDir)
{
	CInifileHelperRoot::GetPrivateProfileString( TEXT( "rasterfile"), TEXT( "basedir"), TEXT( ""), cStrBaseDir, _MAX_BASEDIR);
	cStrBaseDir.ReleaseBuffer();
}

/// ベースディレクトリパス設定関数
inline void CRasterInifileHelper::WriteBaseDir( LPCTSTR lpcszBaseDir)
{
	CInifileHelperRoot::WritePrivateProfileString( TEXT( "rasterfile"), TEXT( "basedir"), lpcszBaseDir);
}

/// ラスタの種類取得関数
inline void CRasterInifileHelper::GetPurpose( int nIndex, CString& cStrPurpose)
{
	CString cStrEntry;
	cStrEntry.Format( TEXT( "purpose%d"), nIndex);
	CInifileHelperRoot::GetPrivateProfileString( TEXT( "rasterfile"), cStrEntry, TEXT( ""), cStrPurpose, _MAX_PURPOSE);
	cStrPurpose.ReleaseBuffer();
}

/// ラスタの最大表示枚数取得関数
inline void CRasterInifileHelper::GetRasterMax( int& nMax)
{
	nMax = CInifileHelperRoot::GetPrivateProfileInt( TEXT( "rasterfile"), TEXT( "rastermax"), -1);
}

/// ラスタの種類設定関数
inline void CRasterInifileHelper::WritePurpose( int nIndex, LPCTSTR lpcszPurpose)
{
	CString cStrEntry;
	cStrEntry.Format( TEXT( "purpose%d"), nIndex);
	CInifileHelperRoot::WritePrivateProfileString( TEXT( "rasterfile"), cStrEntry, lpcszPurpose);
}

/// 入力コード名称取得関数
inline void CRasterInifileHelper::GetAlias( LPCTSTR lpcszPurpose, CString& cStrAlias)
{
	CInifileHelperRoot::GetPrivateProfileString( lpcszPurpose, TEXT( "codealias"), TEXT( ""), cStrAlias, _MAX_CODEALIAS);
	cStrAlias.ReleaseBuffer();
}

/// 入力コード名称設定関数
inline void CRasterInifileHelper::WriteAlias( LPCTSTR lpcszPurpose, LPCTSTR lpcszAlias)
{
	CInifileHelperRoot::WritePrivateProfileString( lpcszPurpose, TEXT( "codealias"), lpcszAlias);
}

/// 入力コード最大長最小長取得関数
inline void CRasterInifileHelper::GetMaxMin( LPCTSTR lpcszPurpose, int& nMax, int& nMin)
{
	nMax = CInifileHelperRoot::GetPrivateProfileInt( lpcszPurpose, TEXT( "codemax"), -1);
	nMin = CInifileHelperRoot::GetPrivateProfileInt( lpcszPurpose, TEXT( "codemin"), -1);
}

/// 入力コード最大長最小長設定関数
inline void CRasterInifileHelper::WriteMaxMin( LPCTSTR lpcszPurpose, int nMax, int nMin)
{
	CInifileHelperRoot::WritePrivateProfileInt( lpcszPurpose, TEXT( "codemax"), nMax);
	CInifileHelperRoot::WritePrivateProfileInt( lpcszPurpose, TEXT( "codemax"), nMin);
}

/// コードパス変換書式取得関数
inline void CRasterInifileHelper::GetSeed( LPCTSTR lpcszPurpose, CString& cStrSeed)
{
	CInifileHelperRoot::GetPrivateProfileString( lpcszPurpose, TEXT( "code2path"), TEXT( ""), cStrSeed, _MAX_CODE2PATH);
	cStrSeed.ReleaseBuffer();
}
/// ラスタファイル拡張子取得関数
inline void CRasterInifileHelper::GetFileExtension( LPCTSTR lpcszPurpose, CString& cStrExt)
{
	CInifileHelperRoot::GetPrivateProfileString( lpcszPurpose, TEXT( "extension"), TEXT( ""), cStrExt, _MAX_CODE2PATH);
	cStrExt.ReleaseBuffer();
}

/// コードパス変換書式設定関数
inline void CRasterInifileHelper::WriteSeed( LPCTSTR lpcszPurpose, LPCTSTR lpcszSeed)
{
	CInifileHelperRoot::WritePrivateProfileString( lpcszPurpose, TEXT( "code2path"), lpcszSeed);
}

/// サブディレクトリ取得関数
inline void CRasterInifileHelper::GetSubdir( LPCTSTR lpcszPurpose, CString& cStrSubdir)
{
	CInifileHelperRoot::GetPrivateProfileString( lpcszPurpose, TEXT( "subdir"), TEXT( ""), cStrSubdir, _MAX_SUBDIR);
	cStrSubdir.ReleaseBuffer();
}

/// サブディレクトリ設定関数
inline void CRasterInifileHelper::WriteSubdir( LPCTSTR lpcszPurpose, LPCTSTR lpcszSubdir)
{
	CInifileHelperRoot::WritePrivateProfileString( lpcszPurpose, TEXT( "subdir"), lpcszSubdir);
}

/// ピラミッド構造構築判定取得関数
inline void CRasterInifileHelper::GetPyramid(LPCTSTR lpcszPurpose, int& nPyramid)
{
	nPyramid = CInifileHelperRoot::GetPrivateProfileInt( lpcszPurpose, TEXT( "Pyramid"), -1);
}

/// ピラミッド構造構築判定設定関数
inline void CRasterInifileHelper::WritePyramid(LPCTSTR lpcszPurpose, int nPyramid)
{
    CInifileHelperRoot::WritePrivateProfileInt( lpcszPurpose, TEXT( "Pyramid"), nPyramid);
}

/// 透過率判定取得関数
inline void CRasterInifileHelper::GetTransmissivity(LPCTSTR lpcszPurpose, int& nTransmissivity)
{
	nTransmissivity = CInifileHelperRoot::GetPrivateProfileInt( lpcszPurpose, TEXT( "Transmissivity"), -1);
}

/// グループレイヤ作成判定関数
inline void CRasterInifileHelper::GetGroupLayer(LPCTSTR lpcszPurpose, int& nGroupLayer)
{
	nGroupLayer = CInifileHelperRoot::GetPrivateProfileInt( lpcszPurpose, TEXT( "group"), -1);
}

/// 選択エリア半径値取得
inline UINT CInifileHelper::GetSelectTolerance( void)
{
	return AheGetSelectTolerance( m_strInifilePath);
}

// 傾斜レイヤファイル
inline int CInifileHelper::GetGradLayerFile( CString& cGradLayerFile)
{
	::GetPrivateProfileString( TEXT("GradEdit"), TEXT("LayerFileName"), TEXT( ""), cGradLayerFile.GetBuffer( MAX_PATH), MAX_PATH, (LPCTSTR)m_strInifilePath);
	cGradLayerFile.ReleaseBuffer();
	return cGradLayerFile.GetLength();
}
// 傾斜レイヤ
inline int CInifileHelper::GetGradLayer( CString& cGradLayer)
{
	::GetPrivateProfileString( TEXT("GradEdit"), TEXT("LayerName"), TEXT( "傾斜モディファイ"), cGradLayer.GetBuffer( MAX_PATH), MAX_PATH, (LPCTSTR)m_strInifilePath);
	cGradLayer.ReleaseBuffer();
	return cGradLayer.GetLength();
}
