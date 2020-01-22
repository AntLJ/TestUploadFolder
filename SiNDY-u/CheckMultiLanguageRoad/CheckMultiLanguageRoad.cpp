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

// CheckMultiLanguageRoad.cpp : コンソール アプリケーションのエントリ ポイントを定義します。
//


#include "CheckMultiLanguageRoad.h"
#include <WinLib/VersionInfo.h>

using namespace std;

#define OPTION_MAX 4


/**
 * @brief	使用法
 */
void	CCheckMultiLanguageRoad::printUsage()
{
	cerr
		<< "【多言語誘導案内リリース用データチェックツール" << "】\n"
		<< "(Usage)CheckMultiLanguageRoad.exe [option]\n"
		<< "・オプション\n"
		<< "-l[InLayerName]				入力レイヤ名\n"
		<< "-m[meshlist]				メッシュリスト指定\n"
		<< "-i[InShapeFilePath]			チェック対象05Shapeの基点パス\n"
		<< "-x[logfile]			ログ出力ファイル指定\n"
		<< endl;
}

/**
 * @brief	初期化
 * @param	argc	[in]	引数の数
 * @param	argv	[in]	引数
 * @return	bool
 */
bool	CCheckMultiLanguageRoad::init(int argc, char *argv[])
{
	if( !arg_check( argc, argv ) )
		return false;

	CVersion cVer;
	::GetLocalTime( &m_st );

	m_ErrorLog << "#" << CT2CA(cVer.GetOriginalFilename()) << " FILEVERSION " <<CT2CA(cVer.GetFileVersion()) << " PRODUCTVERSION " << CT2CA(cVer.GetProductVersion()) << endl;
	m_ErrorLog << "#開始時間 : " << m_st.wYear << "/" << m_st.wMonth << "/" << m_st.wDay << " " << m_st.wHour << ":" << m_st.wMinute << ":" << m_st.wSecond << endl;

	return true;
}


/**
 * @brief	実行
 * @return	bool
 */
bool	CCheckMultiLanguageRoad::execute()
{
	// OLE2T等の一部マクロを使用する場合に必要
	USES_CONVERSION;

	vector<string>::iterator	itrMesh;
	int							iMesh = 0;
	
	string	strInFullPath;

	for( itrMesh = m_vsMesh.begin() ; itrMesh != m_vsMesh.end() ; itrMesh++ )
	{
		strInFullPath		= make_mesh_fname( itrMesh->c_str(), m_InFilePath, m_InLayer );
		if( strInFullPath.empty() )
		{
			m_bIsError = true;
			continue;
		}

		// dbfファイルを読み込みチェック
		if( !CheckDBF( itrMesh->c_str(), strInFullPath ) )
		{
			m_ErrorLog << "#ERROR " << itrMesh->c_str() <<  " チェック失敗！！(dbf)" << endl;
			m_bIsError = true;
			return false;
		}

		m_ErrorLog << "#" << itrMesh->c_str() << " チェック完了" << endl;
	}

	::GetLocalTime( &m_st );
	m_ErrorLog << "#終了時間 : " << m_st.wYear << "/" << m_st.wMonth << "/" << m_st.wDay << " " << m_st.wHour << ":" << m_st.wMinute << ":" << m_st.wSecond << endl; 

	return true;
}

/**
 * @brief	引数チェック
 * @param	argc	[in]	引数の数
 * @param	arrv	[in]	引数
 * @return	bool
 */
bool	CCheckMultiLanguageRoad::arg_check( int argc, char* argv[] )
{
	int		OptionCount[OPTION_MAX];				// オプション重複カウント用
	int		i = 0;
	CString temp;

	for( i = 0 ; i < OPTION_MAX ; ++i )
	{
		OptionCount[i] = 0;
	}

	for( i = 0 ; i < argc ; ++i )
	{
		if( strcmp( argv[i], "-l" ) == 0 )				// レイヤー名
		{
			++OptionCount[0];
			m_InLayer = argv[++i];
		}
		else if( strcmp( argv[i], "-m" ) == 0 )			// メッシュ
		{
			m_Meshlistname = argv[++i];
			temp = m_Meshlistname.c_str();
			if( !loadlist( temp, m_vsMesh ) )			// メッシュリスト読み込み
			{
				std::cerr << "#ERROR メッシュリストの読み込みに失敗！" << endl;
				return false;
			}
			++OptionCount[1];
		}
		else if( strcmp( argv[i], "-i" ) == 0 )			// 入力ファイル
		{
			m_InFilePath = argv[++i];
			++OptionCount[2];
		}
		else if( strcmp( argv[i], "-x" ) == 0 )			// ログファイル
		{
			++OptionCount[3];
			m_ErrorLog.open( argv[++i], std::ios::out );
			if( m_ErrorLog == false )
			{
				std::cerr << "#ERROR ログ出力ファイルのオープンに失敗！" << endl;
				return false;
			}
		}
		else
		{
			if( i != 0 )
			{
				std::cerr << "不正な引数 (" << argv[i] << ") を確認してください。" << endl;
				return false;
			}
		}
	}

	// オプション重複のチェック
	for(i = 0; i < OPTION_MAX; ++i)
	{
		if(OptionCount[i] > 1) {
			cerr << "#ERROR オプションの指定が重複しています!!" << endl;
			return false;
		}
		if(OptionCount[i] == 0){
			cerr << "#ERROR オプションが不足しています、確認してください!!" << endl;
			return false;
		}
	}

	return true;
}

/**
 * @brief	リストファイル読み込み
 * @param	lpszFile	[in]	読み込みたいリスト名
 * @param	VectorList	[out]	読込先Vector
 * @return	bool
 */
bool	CCheckMultiLanguageRoad::loadlist(LPCTSTR lpszFile, std::vector<std::string> &VectorList)
{
	ifstream ifs( static_cast<CT2CW>(lpszFile) );
	if ( !ifs )
		return false;

	copy(
		istream_iterator<string>(ifs),
		istream_iterator<string>(),
		back_inserter( VectorList )
		);

	return ifs.eof();
}

/**
 * @brief	メッシュ単位ファイル名の作成
 * @param	iMeshCode	[in]	メッシュコード
 * @param	cPathName	[in]	ファイル基点パス
 * @param	strLayer	[in]	レイヤ名
 * @return	string		[out]	ファイルのフルパス
*/
string	CCheckMultiLanguageRoad::make_mesh_fname ( string strMesh,	string strPath, string strLayer )
{
	const int		iMeshCode = atoi( strMesh.c_str() );
	stringstream	ssSubStr;
	stringstream	ssFullPath;
	const string	sDataSet = "Road";

	if(  100000 <= iMeshCode && iMeshCode < 1000000)
	{
		ssSubStr << iMeshCode/100 << "\\" << iMeshCode;
	}
	else
	{
		m_ErrorLog << "#ERROR " << strMesh << "：不正なメッシュコードです" << endl;
		return "";
	}

	ssFullPath << strPath.c_str() << "\\" << sDataSet.c_str() << "\\" << ssSubStr.str() << "\\" << strMesh.c_str() << "_" << strLayer.c_str();

	return ssFullPath.str();
}

/**
 * @brief	dbfファイルを読み込み、チェックを行う
 * @param	strMesh			[in]	変換メッシュ
 * @param	strInFullPath	[in]	入力ファイルのフルパス
 * @return	bool
 */	
bool	CCheckMultiLanguageRoad::CheckDBF( string strMesh, string strInFullPath )
{
	string			strInFullPathPlus		= strInFullPath + ".dbf";
	CDBFHandle		cInDBFHdl;		/// 入力DBFファイルハンドルクラス

	// チェック対象dbfオープン
	if( !cInDBFHdl.open((char*)strInFullPath.c_str(), "rb") )
	{
		cerr << "#ERROR DBFファイル<" << strInFullPathPlus << ">のオープンに失敗！" << endl;
		m_ErrorLog << "#ERROR DBFファイル<" << strInFullPathPlus << ">のオープンに失敗！" << endl;
		return false;
	}

	// レコード数の取得
	int		iRecCount		= cInDBFHdl.get_RecordCount();

	// 各レコードのチェック
	int		iCount = 0;
	for( iCount = 0 ; iCount < iRecCount ; iCount++ )
	{
		// 1レコードのロード
		ShpRecord		stShpRecord;
		if( !LoadRecord( cInDBFHdl, iCount, stShpRecord ) )
		{
			cerr << "#ERROR DBFファイル<" << strInFullPathPlus << ">の<" << iCount << "番目>のレコード読み込みに失敗！" << endl;
			m_ErrorLog << "#ERROR DBFファイル<" << strInFullPathPlus << ">の<" << iCount << "番目>のレコード読み込みに失敗！" << endl;
			return false;
		}

		// 漢字・読み・翻訳が全て空ならば、チェックしない
		if( !stShpRecord.wsNameKanji.empty() || !stShpRecord.wsNameYomi.empty() || !stShpRecord.wsNameEng.empty() )
		{
			// 文字自体と文字並びのチェック
			MojiStrChk( stShpRecord );

			// 元漢字と元読み、翻訳名称の関連性チェック
			MojiRelationChk( stShpRecord );
		}
	}

	return true;
}

/**
 * @brief	dbfファイルの1レコードを読み込む
 * @param	cDBFHdl			[in]	該当dbfのdbfHandleクラス
 * @param	iRecNum			[in]	読み込みたいレコード番号
 * @param	stShpRecord		[out]	読み込み先の構造体
 * @return	bool
 */	
bool	CCheckMultiLanguageRoad::LoadRecord( CDBFHandle cDBFHdl, int iRecNum, ShpRecord &stShpRecord )
{
	// フィールド数の取得
	int		iFieldCount	= cDBFHdl.get_FieldCount();

	int		iFidx;
	for( iFidx = 0 ; iFidx < iFieldCount ; iFidx++ )
	{
		// フィールド情報取得
		char			*FieldName;
		string			sFieldName;
		DBFFieldType	dbfType	= FTInvalid;
		dbfType			= cDBFHdl.get_FieldInfo( iFidx, &FieldName, 0, 0 );
		sFieldName		= FieldName;

		// 要素取得用
		bool			bReadRet = true;
		const char*		cpData;
		wchar_t			wcWStr[256];
		wmemset( wcWStr, '\0', 256 );

		// 日本語名称フィールドであれば
		if( sFieldName == "NAME_KANJI" )
		{
			bReadRet				= cDBFHdl.read_Attribute( iRecNum, iFidx, &cpData );
			MultiByteToWideChar(CP_UTF8, 0, cpData, -1, wcWStr, 256);	// 4番目の引数が-1はNULL終端とみなされる
			stShpRecord.wsNameKanji	= wcWStr;
		}

		// 日本語読みフィールドであれば
		else if( sFieldName == "NAME_YOMI" )
		{
			bReadRet				= cDBFHdl.read_Attribute( iRecNum, iFidx, &cpData );
			MultiByteToWideChar(CP_UTF8, 0, cpData, -1, wcWStr, 256);	// 4番目の引数が-1はNULL終端とみなされる
			stShpRecord.wsNameYomi	= wcWStr;
		}

		// 英語フィールドであれば
		else if( sFieldName == "NAME_ENG" )
		{
			bReadRet				= cDBFHdl.read_Attribute( iRecNum, iFidx, &cpData );
			MultiByteToWideChar(CP_UTF8, 0, cpData, -1, wcWStr, 256);	// 4番目の引数が-1はNULL終端とみなされる
			stShpRecord.wsNameEng	= wcWStr;
		}

		if( bReadRet == false )
		{
			cerr << "#ERROR " << iRecNum << "番目のレコードの" << iFidx << "番目のフィールド取得失敗!" << endl;
			m_ErrorLog << "#ERROR " << iRecNum << "番目のレコードの" << iFidx << "番目のフィールド取得失敗!" << endl;
			return false;
		}
	}

	return true;
}

/**
 * @brief	文字列ルールチェック
 * @param	stShpRecord		[in]	shapeの1レコード(漢字、読み、翻訳のみ)	
 */	
void	CCheckMultiLanguageRoad::MojiStrChk( ShpRecord &stShpRecord )
{
	string		sErrorMessage;

	//--------------------------------------------------------------------------------------------------
	// 英語注記チェック
	//--------------------------------------------------------------------------------------------------
	// アルファベットと数字、ハイフン等許容されている文字以外の文字が入っていないか
	// アルファベットと数字、ハイフン等許容されている文字が全て半角で入っているか
	// スペースが全て半角表示になっているかもここでチェック可
	if( stShpRecord.wsNameEng != _T("") ) {	// 文字列が空の場合はやらない
		if( !e_sChk.isalnumsp_str ( stShpRecord.wsNameEng ) )
		{
			m_bIsError = true;
			sErrorMessage = "英語文字コードが範囲外";
			OutputError( stShpRecord, sErrorMessage );
		}
		// スペース後の最初の1文字目は大文字になっているか
		// スペース後の文字が「of」「at」「on」「for」「in」といった前置詞以外大文字になっているか
		if( !e_sChk.Chk_Space_After_Big ( stShpRecord.wsNameEng ) )
		{
			m_bIsError = true;
			sErrorMessage = "大文字小文字のミス";
			OutputError( stShpRecord, sErrorMessage );
		}
		// スペースが2個以上重複していないか
		if( !e_sChk.Space_Dupli_Chk ( stShpRecord.wsNameEng ) )
		{
			m_bIsError = true;
			sErrorMessage = "スペースが2個以上重複";
			OutputError( stShpRecord, sErrorMessage );
		}
		// 中黒の前後に不正なスペースがないか
		if( !e_sChk.Nakaguro_Chk ( stShpRecord.wsNameEng ) )
		{
			m_bIsError = true;
			sErrorMessage = "「・」の前後に不正なスペース";
			OutputError( stShpRecord, sErrorMessage );
		}
		// 括弧の前後に不正なスペースがないか
		if( !e_sChk.Kakko_Chk ( stShpRecord.wsNameEng ) )
		{
			m_bIsError = true;
			sErrorMessage = "「(」と「)」の前後に不正なスペース";
			OutputError( stShpRecord, sErrorMessage );
		}
	}
}

/**
 * @brief	文字列関連性チェック
 * @param	stShpRecord		[in]	shapeの1レコード(漢字、読み、翻訳のみ)	
 */	
void	CCheckMultiLanguageRoad::MojiRelationChk( ShpRecord &stShpRecord )
{
	string		sErrorMessage;

	//----------------------------------------------------------------------------------------------------------------
	// 英語注記チェック
	//----------------------------------------------------------------------------------------------------------------
	// 元漢字と元読みが入っていないのに、翻訳名称が入っているか？
	if( stShpRecord.wsNameKanji.empty() && stShpRecord.wsNameYomi.empty() && !stShpRecord.wsNameEng.empty() )
	{
		m_bIsError = true;
		sErrorMessage = "元名称と元読みが空なのに英語名称が入っている";
		OutputError( stShpRecord, sErrorMessage );
	}

	// 元漢字と元読みが入ってるのに、翻訳名称が入っていない？
	if( !stShpRecord.wsNameKanji.empty() && !stShpRecord.wsNameYomi.empty() && stShpRecord.wsNameEng.empty() )
	{
		m_bIsError = true;
		sErrorMessage = "元名称と元読みが入っているのに英語名称が空";
		OutputError( stShpRecord, sErrorMessage );
	}
}

/**
 * @brief	UTF16⇒SJISにし、ログ出力
 * @param	stShpRecord		[in]	shapeの1レコード(漢字、読み、翻訳のみ)	
 * @param	sErrorMessage	[in]	エラーメッセージ
 */	
void	CCheckMultiLanguageRoad::OutputError( ShpRecord stShpRecord, string sErrorMessage )
{
	string		sSjisNameKanji;
	string		sSjisNameYomi;
	string		sSjisNameEng;

	char		cStr[512];
	memset( cStr, '\0', 512 );

	// 漢字名称をSJIS化
	WideCharToMultiByte(CP_ACP, 0, stShpRecord.wsNameKanji.c_str(), -1, cStr, 512, NULL, 0 );	// 4番目の引数が-1はNULL終端とみなされる
	sSjisNameKanji	= cStr;

	memset( cStr, '\0', 512 );

	// ヨミ名称をSJIS化
	WideCharToMultiByte(CP_ACP, 0, stShpRecord.wsNameYomi.c_str(), -1, cStr, 512, NULL, 0 );	// 4番目の引数が-1はNULL終端とみなされる
	sSjisNameYomi	= cStr;

	memset( cStr, '\0', 512 );

	// 英語名称をSJIS化
	WideCharToMultiByte(CP_ACP, 0, stShpRecord.wsNameEng.c_str(), -1, cStr, 512, NULL, 0 );	// 4番目の引数が-1はNULL終端とみなされる
	sSjisNameEng	= cStr;

	m_ErrorLog << "#ERROR " << sErrorMessage << ", " << sSjisNameKanji << ", " << sSjisNameYomi << ", " << sSjisNameEng << endl; 
}
