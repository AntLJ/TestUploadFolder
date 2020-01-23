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

#include "ConvSJIStoUTF8.h"
#include "stdafx.h"

#include <WinLib/VersionInfo.h>

#define OPTION_MAX 8

using namespace std;

/**
 * @brief	使用法
 */
void	CConvSJIStoUTF8::printUsage()
{
	cerr
		<< "【05Shape文字コード変換ツール" << "】\n"
		<< "(Usage)ConvSJIStoUTF8.exe [option]\n"
		<< "・オプション\n"
		<< "-l[Layername]			処理対象レイヤ名指定(必須)\n"
		<< "-m[meshlist]			メッシュリスト指定(citycode, roadcode以外必須)\n"
		<< "-d[Datasetname]			データセット名(Citymap,Road等)(必須)\n"
		<< "-i[filepass]			変換対象シェイプファイルパス(基点)(必須)\n"
		<< "-o[filepass]			出力先シェイプファイルパス(基点)(必須)\n"
		<< "-x[logfile]				ログ出力ファイル指定(必須)\n"
		<< "-a[AllLayerList]		レイヤ指定ミス判別用全レイヤ記述ファイル(必須)\n"
		<< "-g[IgnoreFieldList]		変換無視フィールド指定ファイル(必須)\n"
		<< "-dbf					dbf変換モード(任意)\n"
		<< endl;
}

/**
 * @brief	初期化
 * @param	argc	[in]	引数の数
 * @param	argv	[in]	引数
 * @return	bool
 */
bool	CConvSJIStoUTF8::init(int argc, char *argv[])
{
	if( !arg_check( argc, argv ) )
		return false;

	return true;
}

/**
 * @brief	引数チェック
 * @param	argc	[in]	引数の数
 * @param	argv	[in]	引数
 * @return	bool
 */
bool	CConvSJIStoUTF8::arg_check(int argc, char *argv[])
{
	int		a_OptionCount[OPTION_MAX];		// オプション重複カウント用
	int		i = 0;
	CString csTemp;
	string	sTemp;

	for( i = 0 ; i < OPTION_MAX ; i++ )
	{
		a_OptionCount[i]	= 0;
	}

	for( i = 0 ; i < argc ; ++i )
	{
		if( strcmp( argv[i], "-l" ) == 0 )
		{
			if( argc < i + 2 )	return false;
			++a_OptionCount[0];
			m_sLayerName	= argv[++i];

			if( m_sLayerName == "citycode" )
				m_iExeMode	|= ON_CITYCODE_MODE;
			else if( m_sLayerName == "roadcode" ) {
				m_iExeMode	|= ON_ROADCODE_MODE;
				m_DbfMode = true;
			}
		}
		else if( strcmp( argv[i], "-m" ) == 0 )
		{
			if( argc < i + 2 )	return false;
			++a_OptionCount[1];
			csTemp = argv[++i];
			if( !loadlist( csTemp, m_vsMesh ) )
			{
				cerr << "#ERROR メッシュリストの読み込みに失敗！" << endl;
				return false;
			}
		}
		else if( strcmp( argv[i], "-d" ) == 0 )
		{
			if( argc < i + 2 )	return false;
			++a_OptionCount[2];
			m_sDataSet		= argv[++i];
		}
		else if( strcmp( argv[i], "-i" ) == 0 )
		{
			if( argc < i + 2 )	return false;
			++a_OptionCount[3];
			m_sFilePass		= argv[++i];
		}
		else if( strcmp( argv[i], "-o" ) == 0 )
		{
			if( argc < i + 2 )	return false;
			++a_OptionCount[4];
			m_sDestFilePass	= argv[++i];
		}
		else if( strcmp( argv[i], "-x" ) == 0 )		
		{
			if( argc < i + 2 )	return false;
			++a_OptionCount[5];
			m_ErrorLog.open( argv[++i], ios::out );
			if( m_ErrorLog == false )
			{
				cerr << "#ERROR ログ出力ファイルのオープンに失敗！" << endl;
				return false;
			}
		}
		else if( strcmp( argv[i], "-a" ) == 0 )
		{
			if( argc < i + 2 )	return false;
			++a_OptionCount[6];
			csTemp = argv[++i];
			if( !loadlist( csTemp, m_vsAllLayer ) )
			{
				cerr << "#ERROR 全レイヤ記述ファイルの読み込みに失敗！" << endl;
				return false;
			}
		}
		else if( strcmp( argv[i], "-g" ) == 0 )
		{
			if( argc < i + 2 )	return false;
			++a_OptionCount[7];
			csTemp = argv[++i];
			if( !loadlist( csTemp, m_vsIgnoreField ) )
			{
				cerr << "#ERROR 変換無視フィールド指定ファイルの読み込みに失敗！" << endl;
				return false;
			}
		}
		else if( strcmp( argv[i], "-dbf" ) == 0 )
		{
			++a_OptionCount[8];
			m_DbfMode = true;
		}
		else
		{
			if( i != 0 )
			{
				cerr << "不正な引数 (" << argv[i] << ") を確認してください！" << endl;
				return false;
			}
		}
	}

	// オプション重複チェック
	for( i = 0 ; i < OPTION_MAX ; i++ )
	{
		if( a_OptionCount[i] > 1 ){
			cerr << "#ERROR オプションの指定が重複しています！" << endl;
			return false;
		}
		// roadcode.dbfとcitycode.dbfのメッシュリスト指定は無くてもよい
		if( i != 1 || ( (m_iExeMode ^ ON_ROADCODE_MODE) && (m_iExeMode ^ ON_CITYCODE_MODE) )){
			if( a_OptionCount[i] == 0 ){
				cerr << "#ERROR オプションの指定が不足しています！" << endl;
				return false;
			}	
		}
	}

	// 指定レイヤ存在確認チェック
	vector<string>::iterator	pos;
	pos	= find( m_vsAllLayer.begin(), m_vsAllLayer.end(), m_sLayerName );
	if( pos == m_vsAllLayer.end() ){
		cerr << "#ERROR 指定レイヤが存在しません！" << endl;
		return false;
	}

	// DBFのみのレイヤの処理モードを定義
	if( m_iExeMode ^ ON_ROADCODE_MODE && m_iExeMode ^ ON_CITYCODE_MODE )
	{
		m_iExeMode |= ON_NORMAL_MODE;
	}

	return true;
}


/**
 * @brief	実行
 * @return	bool
 */
bool	CConvSJIStoUTF8::execute()
{

	CVersion cVer;
	CString strMsg;

	::GetLocalTime( &m_st );

	m_ErrorLog << "#" << CT2CA(cVer.GetOriginalFilename()) << " FILEVERSION " <<CT2CA(cVer.GetFileVersion()) << " PRODUCTVERSION " << CT2CA(cVer.GetProductVersion()) << endl;
	m_ErrorLog << "#開始時間 : " << m_st.wYear << "/" << m_st.wMonth << "/" << m_st.wDay << " " << m_st.wHour << ":" << m_st.wMinute << ":" << m_st.wSecond << endl;

	m_ErrorLog << "#" << m_sLayerName.c_str() << " レイヤ：処理開始" << endl;

	string	strFullPass;
	string	strDestFullPass;

	if( m_iExeMode & ON_CITYCODE_MODE || m_iExeMode & ON_ROADCODE_MODE )
	{
		strFullPass		= m_sFilePass + "\\" + m_sDataSet + "\\" + m_sLayerName;
		strDestFullPass	= m_sDestFilePass + "\\" + m_sDataSet + "\\" + m_sLayerName;

		if( !GetFieldInfo( strFullPass ) )
			return false;

		if( !ConvertDBF( "", strFullPass, strDestFullPass ) )
		{
			m_ErrorLog << "#ERROR 変換失敗" << endl;
			return false;
		}
	}
	else
	{
		vector<string>::iterator	a_itrMesh;

		for( a_itrMesh = m_vsMesh.begin() ; a_itrMesh != m_vsMesh.end() ; a_itrMesh++ )
		{
			string			strMesh(a_itrMesh->c_str());
			strFullPass		= make_mesh_fname( strMesh, (char*)m_sFilePass.c_str() );
			strDestFullPass	= make_mesh_fname( strMesh, (char*)m_sDestFilePass.c_str() );

			if( a_itrMesh == m_vsMesh.begin() )
			{
				if( !GetFieldInfo( strFullPass ) )
					return false;
			}

			m_ErrorLog << "#" << a_itrMesh->c_str() << endl;
			if( !ConvertDBF( a_itrMesh->c_str(), strFullPass, strDestFullPass ) )
			{
				m_ErrorLog << "#ERROR 変換失敗" << endl;
				return false;
			}
		}
	}
	::GetLocalTime( &m_st );

	m_ErrorLog << "#" << m_sLayerName.c_str() << " レイヤ：正常終了" << endl;
	m_ErrorLog << "#終了時間 : " << m_st.wYear << "/" << m_st.wMonth << "/" << m_st.wDay << " " << m_st.wHour << ":" << m_st.wMinute << ":" << m_st.wSecond << endl;

	return true;
}

/**
 * @brief	リストファイル読み込み
 * @param	lpszFile	[in]	読み込みたいリスト名
 * @param	vsVectorList	[out]	読込先Vector(1フィールドごと)
 * @return	bool
 */
bool	CConvSJIStoUTF8::loadlist(LPCTSTR lpszFile, vector<string>& vsVectorList )
{
	ifstream ifs( static_cast<CT2CW>(lpszFile) );
	if ( !ifs )
		return false;

	copy(
		istream_iterator<string>(ifs),
		istream_iterator<string>(),
		back_inserter( vsVectorList )
		);

	return ifs.eof();
}

/**
 * @brief	dbfファイルのUTF8変換
 * @param	メッシュ	[in]	変換メッシュ
 * @param	パス		[in]	入力ファイルのフルパス
 * @param	パス		[in]	出力ファイルのフルパス
 * @return	bool
 */
bool	CConvSJIStoUTF8::ConvertDBF( string Mesh, string sFullPass, string sDestFullPass )
{
	string			strFullPassPlus		= sFullPass + ".dbf";
	string			strDestFullPassPlus	= sDestFullPass + ".dbf"; 

	CDBFHandle		a_cDBFHdl;		/// 入力DBFファイルハンドルクラス
	CDBFHandle		a_cDestDBFHdl;	/// 出力DBFファイルハンドルクラス

	// 読み込みdbfオープン
	if( !a_cDBFHdl.open((char*)sFullPass.c_str(), "rb") )
	{
		cerr << "#ERROR DBFファイル<" << strFullPassPlus << ">のオープンに失敗！" << endl;
		m_ErrorLog << "#ERROR DBFファイル<" << strFullPassPlus << ">のオープンに失敗！" << endl;
		return false;
	}

	// 出力先ディレクトリ作成(imagehlp.hのインクルードとimagehlp.libのリンク)
	MakeSureDirectoryPathExists( sDestFullPass.c_str() );

	// 出力dbf作成
	if( !a_cDestDBFHdl.create((char*)sDestFullPass.c_str()) ){
		cerr << "#ERROR DBFファイル<" << strDestFullPassPlus << ">の作成に失敗！" << endl;
		m_ErrorLog << "#ERROR DBFファイル<" << strDestFullPassPlus << ">の作成に失敗！" << endl;
		return false;
	}

	// フィールド追加
	vector<FieldInfo>::iterator	itrFieldInfo;
	for( itrFieldInfo = m_vfFieldInfo.begin() ; itrFieldInfo != m_vfFieldInfo.end() ; itrFieldInfo++ )
	{
		if( itrFieldInfo->dType == FTString && itrFieldInfo->iIgnoreFlag == 0 ){
			// 変換対象フィールドの場合はサイズを1.5倍(254を超える場合は254に設定)
			long fieldLength = (itrFieldInfo->iWidth)*3/2;
			if (fieldLength > 254)
				fieldLength = 254;
			if( a_cDestDBFHdl.add_Field( itrFieldInfo->sName.c_str(), itrFieldInfo->dType, fieldLength, itrFieldInfo->iDecimals ) < 0 )	return false;
		}else{
			if( a_cDestDBFHdl.add_Field( itrFieldInfo->sName.c_str(), itrFieldInfo->dType, itrFieldInfo->iWidth, itrFieldInfo->iDecimals ) < 0 )	return false;
		}
	}

	// レコード数の取得
	int a_iRecCount		= a_cDBFHdl.get_RecordCount();

	// 各レコードの情報取得
	int		a_iCount = 0;
	for( a_iCount = 0 ; a_iCount < a_iRecCount ; a_iCount++ )
	{
		for( itrFieldInfo = m_vfFieldInfo.begin() ; itrFieldInfo != m_vfFieldInfo.end() ; itrFieldInfo++ )
		{
			const char*		a_cpData;
			int				a_ciData	= 0;
			double			a_cdData	= 0;

			bool			bReadRet	= false;
			int				iWriteRet	= 0;

			// 読み込み
			switch( itrFieldInfo->dType )
			{
				case FTString:
					bReadRet	= a_cDBFHdl.read_Attribute( a_iCount, itrFieldInfo->iID, &a_cpData );
					if( !CheckShiftJIS(a_cpData) ){
						cerr << "#ERROR 文字列「" << a_cpData << "」の文字コードが Shift-JIS ではありません!" << endl;
						m_ErrorLog << "#ERROR 文字列「" << a_cpData << "」の文字コードが Shift-JIS ではありません!" << endl;
						bReadRet = false;
					}
					break;
				case FTInteger:
					bReadRet	= a_cDBFHdl.read_Attribute( a_iCount, itrFieldInfo->iID, &a_ciData );
					break;
				case FTDouble:
					bReadRet	= a_cDBFHdl.read_Attribute( a_iCount, itrFieldInfo->iID, &a_cdData );
					break;
				case FTLogical:
				case FTInvalid:
				default:
					bReadRet	= false;
			}
			if( bReadRet == false )
			{
				cerr << "#ERROR DBFファイル<" << strFullPassPlus << ">の" << a_iCount << "番目のレコードの" << itrFieldInfo->iID << "番目のフィールド取得失敗!" << endl;
				m_ErrorLog << "#ERROR DBFファイル<" << strFullPassPlus << ">の" << a_iCount << "番目のレコードの" << itrFieldInfo->iID << "番目のフィールド取得失敗!" << endl;
				return false;
			}

			char	temp[512];
			wchar_t	temp_w[256];
			long	len = 0, wlen = 0, wwlen = 0;

			// 書き込み
			switch( itrFieldInfo->dType )
			{
				case FTString:
					if( itrFieldInfo->iIgnoreFlag == 0 )
					{
						// S-JIS ⇒ UTF16 ⇒ UTF8の順で読み取ったフィールド値を変換
						len = strlen( a_cpData );
						memset( temp, '\0', 512 );
						memset( temp_w, '\0', 512 );
						wlen = MultiByteToWideChar( CP_ACP, 0, a_cpData, len, (LPWSTR)temp_w, 0 );					// コピー領域の計算
						MultiByteToWideChar( CP_ACP, 0, a_cpData, len, (LPWSTR)temp_w, wlen );						// 実際のS-JIS→UTF-16の変換コピー
						wwlen = WideCharToMultiByte( CP_UTF8, 0, (LPCWSTR)temp_w, wlen, (LPSTR)temp, 0, NULL, 0 );	// コピー領域の計算
						WideCharToMultiByte( CP_UTF8, 0, (LPCWSTR)temp_w, wlen, (LPSTR)temp, wwlen, NULL, 0 );		// 実際のUTF-16→UTF-8の変換コピー
						iWriteRet	= a_cDestDBFHdl.write_Attribute( a_iCount, itrFieldInfo->iID, temp );
					}
					else
						iWriteRet	= a_cDestDBFHdl.write_Attribute( a_iCount, itrFieldInfo->iID, a_cpData );
					break;
				case FTInteger:
					iWriteRet	= a_cDestDBFHdl.write_Attribute( a_iCount, itrFieldInfo->iID, a_ciData );
					break;
				case FTDouble:
					iWriteRet	= a_cDestDBFHdl.write_Attribute( a_iCount, itrFieldInfo->iID, a_cdData );
					break;
				case FTLogical:
				case FTInvalid:
				default:
					iWriteRet	= 0;
			}
			if( iWriteRet == 0 )
			{
				cerr << "#ERROR DBFファイル<" << strDestFullPassPlus << ">の" << a_iCount << "番目のレコードの" << itrFieldInfo->iID << "番目のフィールドに書き込み失敗!" << endl;
				m_ErrorLog << "#ERROR DBFファイル<" << strDestFullPassPlus << ">の" << a_iCount << "番目のレコードの" << itrFieldInfo->iID << "番目のフィールドに書き込み失敗!" << endl;
				return false;
			}
		}
	}
	a_cDBFHdl.close();
	a_cDestDBFHdl.close();

	// EOF(0x1A)を書き込むための特殊処理(工藤さんロジック流用)
	FILE*	a_fpTmpDBFFp	= NULL;
	a_fpTmpDBFFp	= fopen ( strDestFullPassPlus.c_str(), "ab+" );
	if( a_fpTmpDBFFp == NULL ) {
		cerr << "#ERROR DBFファイル<" << strDestFullPassPlus << ">のEOF追加処理に失敗!" << endl;
		m_ErrorLog << "#ERROR DBFファイル<" << strDestFullPassPlus << ">のEOF追加処理に失敗!" << endl;
		return false;
	}else {
		fprintf	( a_fpTmpDBFFp, "%c", 0x1A );
		fclose	( a_fpTmpDBFFp );
	}

	// shpファイルを持つレイヤはshpをコピー
	if(! m_DbfMode )
	{
		strFullPassPlus		= sFullPass + ".shp";
		strDestFullPassPlus	= sDestFullPass + ".shp"; 

		CSHPHandle		a_cSHPHdl;		/// 入力SHPファイルハンドルクラス
		CSHPHandle		a_cDestSHPHdl;	/// 出力SHPファイルハンドルクラス

		// 読み込みshpオープン
		if( !a_cSHPHdl.open((char*)sFullPass.c_str(), "rb+") )
		{
			cerr << "#ERROR SHPファイル<" << strFullPassPlus << ">のオープンに失敗！" << endl;
			m_ErrorLog << "#ERROR SHPファイル<" << strFullPassPlus << ">のオープンに失敗！" << endl;
			return false;
		}

		int		iShpType	= 0;
		int		iShpNum		= 0;
		a_cSHPHdl.get_FileInfo( &iShpNum, &iShpType, 0, 0 );

		// 出力shp作成(出力先パスのフォルダはdbfファイル作成の際に作っているので省略)
		if( !a_cDestSHPHdl.create((char*)sDestFullPass.c_str(), iShpType ) ){
			cerr << "#ERROR SHPファイル<" << strDestFullPassPlus << ">の作成に失敗！" << endl;
			m_ErrorLog << "#ERROR SHPファイル<" << strDestFullPassPlus << ">の作成に失敗！" << endl;
			return false;
		}

		// 各レコードの情報取得
		for( a_iCount = 0 ; a_iCount < iShpNum ; a_iCount++ )
		{
			CSHPObject		a_cSHPObj;	/// Shpオブジェクトハンドルクラス

			// Shpのオブジェクト読み込み
			if( a_cSHPHdl.read_Object( a_iCount, &a_cSHPObj ) == -1 )
			{
				cerr << "#ERROR SHPファイル<" << strFullPassPlus << ">の" << a_iCount << "番目のオブジェクト取得失敗!" << endl;
				m_ErrorLog << "#ERROR SHPファイル<" << strFullPassPlus << ">の" << a_iCount << "番目のオブジェクト取得失敗!" << endl;
				return false;
			}

			// Shpのオブジェクト書き込み
			if( a_cDestSHPHdl.write_Object( &a_cSHPObj ) == -1 )
			{
				cerr << "#ERROR SHPファイル<" << strDestFullPassPlus << ">の" << a_iCount << "番目のオブジェクト書き込み失敗!" << endl;
				m_ErrorLog << "#ERROR SHPファイル<" << strDestFullPassPlus << ">の" << a_iCount << "番目のオブジェクト書き込み失敗!" << endl;
				return false;
			}
		}
		a_cSHPHdl.close();
		a_cDestSHPHdl.close();
	}
	return true;
}


/**
 * @brief	メッシュ単位ファイル名の作成
 * @param	MeshCode	[in]	メッシュコード
 * @param	cPathName	[in]	ファイル基点パス
 * @return	ファイルのフルパス
*/
string	CConvSJIStoUTF8::make_mesh_fname ( string MeshCode,	char* cpPathName )
{
	char	a_cSubStr[256];
	memset	( a_cSubStr, '\0', 256 );

	string	a_sSubDir		= "";
	string	a_sFullPass		= "";
	char	a_cTemp[4096];	

	if( 2 == MeshCode.length() || 4 == MeshCode.length() )
		sprintf( a_cSubStr, "%s", MeshCode.c_str() );

	else if( 6 == MeshCode.length() )
		sprintf( a_cSubStr, "%s\\%s", MeshCode.substr(0, 4).c_str(), MeshCode.c_str() );

	else if( 8 == MeshCode.length() )
		sprintf( a_cSubStr, "%s\\%s\\%s", MeshCode.substr(0, 4).c_str(), MeshCode.substr(0, 6).c_str(), MeshCode.c_str() );

	else
		m_ErrorLog << "#ERROR" << MeshCode.c_str() << "：不正なメッシュコードです" << endl;

	a_sSubDir	= a_cSubStr;

	sprintf( a_cTemp, "%s\\%s\\%s\\%s_%s", cpPathName, m_sDataSet.c_str(), a_sSubDir.c_str(), MeshCode.c_str(), m_sLayerName.c_str() );

	a_sFullPass	= a_cTemp;
	
	return a_sFullPass;
}

/**
 * @brief	変換対象レイヤから、文字列フィールドのインデックスを取得
 * @param	fullpass	[in]	ファイルのフルパス
 * @return	bool
 */
bool	CConvSJIStoUTF8::GetFieldInfo( string sFullPass )
{
	CDBFHandle		a_cDBFHdl;	/// DBFファイルハンドルクラス

	if( !a_cDBFHdl.open((char*)sFullPass.c_str(), "rb") )
	{
		cerr << "#ERROR DBFファイル<" << sFullPass << ">のオープンに失敗！" << endl;
		m_ErrorLog << "#ERROR DBFファイル<" << sFullPass << ">のオープンに失敗！" << endl;
		return false;
	}

	// フィールド数の取得
	int		a_iFieldCount	= 0;
	a_iFieldCount		= a_cDBFHdl.get_FieldCount();

	// string型を含むか判別用
	int		a_iJudge = 0;

	int		a_iFidx;
	for( a_iFidx = 0 ; a_iFidx < a_iFieldCount ; a_iFidx++ )
	{
		FieldInfo	a_FieldInfo;
		a_FieldInfo.iID	= a_iFidx;

		// フィールド情報取得
		char	*a_FieldName;
		a_FieldInfo.dType	= a_cDBFHdl.get_FieldInfo( a_FieldInfo.iID, &a_FieldName, &a_FieldInfo.iWidth, &a_FieldInfo.iDecimals );
		a_FieldInfo.sName	= a_FieldName;

		if( a_FieldInfo.dType == FTString )
		{
			vector<string>::iterator itr = find( m_vsIgnoreField.begin(), m_vsIgnoreField.end(), a_FieldInfo.sName );
			if( itr == m_vsIgnoreField.end() )
				a_iJudge++;
			else
				a_FieldInfo.iIgnoreFlag = 1;
		}

		// vectorにフィールド番号、フィールドタイプ、フィールド名、フィールドサイズ、精度、変換無視フラグを追加(該当Shapeのフィールド数分持つ)
		m_vfFieldInfo.push_back( a_FieldInfo );
	}
	a_cDBFHdl.close();

	if( a_iJudge == 0 ){
		cerr << "#ERROR " << m_sLayerName.c_str() << " に変換対象フィールドが存在しないため変換しません！" << endl;
		m_ErrorLog << "#ERROR " << m_sLayerName.c_str() << " に変換対象フィールドが存在しないため変換しません！" << endl;
		return false;
	}

	return true;
}

/**
 * @brief	文字コードがShift-JISか判定する。
 * @param	cChars	[in]	文字列
 * @return	bool
 */
bool	CConvSJIStoUTF8::CheckShiftJIS( const char* cChars )
{
	const unsigned char* p = ( unsigned char* ) cChars;
	unsigned char c1, c2;
	int len = strlen(cChars);
	
	for( int i=0; i < len; ++i, ++p)
	{
		c1 = *p;
		if( c1 == 0x00 )
			return false;
		else if( c1 <= 0x7F )	// ASCII文字
			continue;
		else if( c1 >= 0xa0 && c1 <= 0xdf)	//半角カナ
			continue;
		else if( ((c1 >= 0x81 && c1 <= 0x9F )|| (c1 >= 0xE0 && c1 <= 0xEF )) && i < len -1 )	// 全角 ひらがな/漢字/記号
		{
			c2 = *( p+1 );
			if( (c2 >= 0x40 && c2 <= 0x7E ) || (c2 >= 0x80 && c2 <= 0xFC) )
			{
				i++;
				p++;
			} 
			else
				return false;
		}
		else
			return false;
	}
	return true;
}
