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

// CompShape2Shape.cpp : コンソール アプリケーションのエントリ ポイントを定義します。
//
#include "CompShape2Shape.h"
#include "stdafx.h"
#include "ShapeChk.h"

#define OPTION_MAX 13

using namespace std;

/**
 * @brief	使用法
*/
void	CCompShape2Shape::printUsage()
{
	cerr
		<< "【shapeファイル同士の属性・形状比較ツール(" << m_sVersion.c_str() << ")】\n"
		<< "(Usage)CompShape2Shape.exe [option]\n"
		<< "・オプション\n"
		<< "-m[meshlist]			メッシュリスト指定\n"
		<< "-i1[Orifilepass]		比較元シェイプファイルパス(必須)\n"
		<< "-i2[Destfilepass]		比較先シェイプファイルパス(必須)\n"
		<< "-l[Layername]			レイヤ名指定(必須)\n"
		<< "-d[Datasetname]			データセット名(必須)(例：Citymap等)\n"
		<< "-o[output]			ログ出力ファイル指定(必須)\n"
		<< "-ATTR				属性比較モード(-ATTRか-SHPどちらかは必須、両方指定しても良い)\n"
		<< "-SHP				形状比較モード\n"
		<< "-dist_b[DistanceBorder]		許容誤差(距離チェック)(誤差はパーセンテージで指定すること)（例：1.0)\n"
		<< "-FID				OBJECTIDではなく、FIDで紐付ける場合は指定\n"
		<< "-BIN				バイナリ比較モード(速度重視)\n"
		<< endl;
}

/**
 * @brief	バージョン出力
 */
void	CCompShape2Shape::print_ver()
{
	m_ofErrorLog 
		<< "#【shapeファイル同士の属性・形状比較ツール(" << m_sVersion.c_str() << ")】\n"
		<< "#CompShape2Shape.exe\n"
		<< "#FileVersion\t" << m_sVersion << "\n"
		<< endl;
}

/**
 * @brief	Shapeファイル基点パス出力
 */
void	CCompShape2Shape::print_pass()
{
	m_ofErrorLog 
		<< "#比較元基点パス:<" << m_sOriFPass << ">\n"
		<< "#比較先基点パス:<" << m_sDestFPass << ">\n";
}

/**
 * @brief	開始・終了時間出力
 */
void	CCompShape2Shape::print_begin_time()
{
	m_ofErrorLog
		<< "#開始時間: " << m_st.wYear << "/" << m_st.wMonth << "/" << m_st.wDay << " " << m_st.wHour << ":" << m_st.wMinute << ":" << m_st.wSecond << endl;
}

/**
 * @brief	開始・終了時間出力
 */
void	CCompShape2Shape::print_end_time()
{
	m_ofErrorLog
		<< "#終了時間: " << m_st.wYear << "/" << m_st.wMonth << "/" << m_st.wDay << " " << m_st.wHour << ":" << m_st.wMinute << ":" << m_st.wSecond << endl;
}

/**
 * @brief	ログヘッダ(フィールド名)出力
 */
void	CCompShape2Shape::print_log_head()
{
	m_ofErrorLog
		<< "\n#SINDYSTDLOG\n"
		<< "対応状況\tレイヤ名\tオブジェクトID(FID)\tX(経度)\tY(緯度)\tエラーレベル\tエラーコード\tエラーメッセージ\tメッシュコード\t距離(比較元)[m]\t距離(比較先)[m]\t距離差異[m]\t距離差違割合[%]\n";
}

/**
 * @brief	引数チェック
 * @param	argc	[in]	引数の数
 * @param	argv	[in]	引数
 * @return	bool
 */
bool	CCompShape2Shape::arg_check(int argc, char *argv[])
{
	int		a_OptionCount[OPTION_MAX];		// オプション重複カウント用
	int		i = 0;
	CString csTemp;
	string	sTemp;

	// ツールバージョン表示
	CVersion	a_cvTool_Var;
	CString		a_csVar;
	a_csVar		= a_cvTool_Var.GetFileVersion();
	m_sVersion	= (char*)(_bstr_t(a_csVar));

	for( i = 0 ; i < OPTION_MAX ; i++ )
	{
		a_OptionCount[i]	= 0;
	}

	for( i = 0 ; i < argc ; ++i )
	{
		if( strcmp( argv[i], "-m" ) == 0 )
		{
			++a_OptionCount[0];
			csTemp = argv[++i];
			if( !loadlist( csTemp, m_vsMesh ) )
			{
				cerr << "#ERROR メッシュリストの読み込みに失敗！" << endl;
				return false;
			}
		}
		else if( strcmp( argv[i], "-i1" ) == 0 )
		{
			if( argc < i + 2 )	return false;
			++a_OptionCount[1];
			m_sOriFPass		= argv[++i];
		}
		else if( strcmp( argv[i], "-i2" ) == 0 )
		{
			if( argc < i + 2 )	return false;
			++a_OptionCount[2];
			m_sDestFPass	= argv[++i];
		}
		else if( strcmp( argv[i], "-l" ) == 0 )
		{
			if( argc < i + 2 )	return false;
			++a_OptionCount[3];
			m_sLayerName	= argv[++i];
		}
		else if( strcmp( argv[i], "-o" ) == 0 )		
		{
			++a_OptionCount[4];
			m_ofErrorLog.open( argv[++i], ios::out );
			if( m_ofErrorLog == false )
			{
				cerr << "#ERROR ログ出力ファイルのオープンに失敗！" << endl;
				return false;
			}
		}
		else if( strcmp( argv[i], "-FID" ) == 0 )
		{
			++a_OptionCount[5];
			m_iExeMode		|= ON_FID_MODE;
		}
		else if( strcmp( argv[i], "-d" ) == 0 )
		{
			++a_OptionCount[6];
			m_sDataSet		= argv[++i];
			if( strcmp( m_sDataSet.c_str(), "ROAD" ) == 0 )
				m_iExeMode		|= ON_ROAD_MODE;
			else
				m_iExeMode		|= ON_MAP_MODE;
		}
		else if( strcmp( argv[i], "-fss" ) == 0 )
		{
			++a_OptionCount[7];
			m_iExeMode		|= ON_FSPECIFY_MODE;
			sTemp			= argv[++i];
			m_vsFSpecify.push_back( sTemp );
		}
		else if( strcmp( argv[i], "-fsl" ) == 0 )
		{
			++a_OptionCount[8];
			m_iExeMode		|= ON_FSPECIFY_MODE;
			csTemp = argv[++i];
			if( !loadlist( csTemp, m_vsFSpecify ) )
			{
				cerr << "#ERROR 比較対象フィールドリストの読み込みに失敗！" << endl;
				return false;
			}
		}
		else if( strcmp( argv[i], "-BIN" ) == 0 )
		{
			++a_OptionCount[9];
			m_iExeMode		|= ON_EXE_BINARY_MODE;
		}
		else if( strcmp( argv[i], "-ATTR" ) == 0 )
		{
			++a_OptionCount[10];
			m_bAttr = true;
		}
		else if( strcmp( argv[i], "-SHP" ) == 0 )
		{
			++a_OptionCount[11];
			m_bShp = true;
		}
		else if( strcmp( argv[i], "-dist_b" ) == 0 )
		{
			++a_OptionCount[12];
			m_dDistBorder = atof(argv[++i]);
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

	// オプションFIDが指定されていなければ、OBJECTIDモード
	if( a_OptionCount[5] == 0 )
		m_iExeMode		|= ON_OBJECTID_MODE;

	// オプション-fssか-fslで比較対象フィールドが指定されていなければ、全比較モード
	if( a_OptionCount[7] == 0 && a_OptionCount[8] == 0 )
		m_iExeMode		|= ON_FALL_MODE;
	
	// オプション-BINが指定されていなければ、通常実行モード
	if( a_OptionCount[9] == 0 )
		m_iExeMode		|= ON_EXE_NORMAL_MODE;

	//オプション-ATTR、-SHPのどちらかが指定されていなければ、エラー
	if( a_OptionCount[10] == 0 && a_OptionCount[11] == 0)
	{
		cerr << "#ERROR -ATTR か -SHP オプションを指定してください!" << endl;
		return false;
	}

	// オプション重複チェック
	for( i = 0 ; i < OPTION_MAX ; i++ )
	{
		if( a_OptionCount[i] > 1 ){
			cerr << "#ERROR オプションの指定が重複しています！" << endl;
			return false;
		}
		if( (i != 5 && i != 7 && i != 8 && i != 9 && i !=10 && i !=11 && i !=12 ) && a_OptionCount[i] == 0){
			cerr << "#ERROR オプションが不足しています、確認してください！" << endl;
			return false;
		}
	}
	if( (a_OptionCount[11] == 1) && (a_OptionCount[12] == 0 )){
		cerr << "#ERROR オプションが不足しています、確認してください！" << endl;
		return false;
	}
	return true;
}


bool	CCompShape2Shape::loadlist(LPCTSTR lpszFile, vector<string>& vsVectorList )
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
 * @brief	メッシュ単位ファイル名の作成
 * @param	iMeshCode	[in]	メッシュコード
 * @param	cPathName	[in]	ファイル基点パス
 * @return	string		[out]	ファイルのフルパス
*/
string	CCompShape2Shape::make_mesh_fname ( int iMeshCode,	char* cpPathName )
{
	char	a_cSubStr[256];
	memset	( a_cSubStr, '\0', 256 );

	string	a_sSubDir		= "";
	string	a_sFullPass		= "";
	char	a_cTemp[4096];	

	int			a_iDegit = 0;
	a_iDegit	= m_cCrd.GetMeshDigit( iMeshCode );

	switch( a_iDegit )
	{
	case 2:
		sprintf( a_cSubStr, "%02d", iMeshCode );
		break;
	case 4:
		sprintf( a_cSubStr, "%d", iMeshCode );
		break;
	case 6:
		sprintf( a_cSubStr, "%d\\%d", iMeshCode/100, iMeshCode );
		break;
	case 8:
		sprintf( a_cSubStr, "%d\\%d\\%d", iMeshCode/10000, iMeshCode/100, iMeshCode );
		break;
	default:
		break;
	}
	a_sSubDir	= a_cSubStr;

	sprintf( a_cTemp, "%s\\%s\\%s\\%02d_%s", cpPathName, (char*)m_sDataSet.c_str(), (char*)a_sSubDir.c_str(), iMeshCode, (char*)m_sLayerName.c_str() );

	a_sFullPass	= a_cTemp;
	
	return a_sFullPass;
}

/**
 * @brief	Shapeファイルを読み込み、multimap<ID,shp1>に落とし込む
 * @param	sFullPass	[in]	読み込みShapeファイルのフルパス
 * @param	mmShp1		[out]	Shapeファイル中身格納用クラス集合コンテナ
 * @return	bool
 */
bool	CCompShape2Shape::load_shape_info( string sFullPass, int iMesh, mmShp1& mmShp1 )
{
	CSHPHandle		a_cSHPHdl;	/// シェイプハンドルクラス(Shapelib)
	CDBFHandle		a_cDBFHdl;	/// DBFファイルハンドルクラス


	if( !a_cDBFHdl.open((char*)sFullPass.c_str(), "rb") )
	{
		cerr << "#ERROR DBFファイル<" << sFullPass << ">のオープンに失敗！" << endl;
		m_ofErrorLog << "#ERROR DBFファイル<" << sFullPass << ">のオープンに失敗！" << endl;
		return false;
	}
	
	// レコード数の取得
	int a_iRecCount		= 0;
	a_iRecCount			= a_cDBFHdl.get_RecordCount();
	
	// フィールド数の取得
	int	a_iFieldCount	= 0;
	a_iFieldCount		= a_cDBFHdl.get_FieldCount();

	// フィールド情報の取得用
	vector<int>			a_viField_Indexes;		// フィールドインデックス
	vector<string>		a_vsField_Names;		// フィールド名
	vector<int>			a_viField_Types;		// フィールドのデータタイプ

	int		a_iFidx;
	for( a_iFidx = 0 ; a_iFidx < a_iFieldCount ; a_iFidx++ )
	{
		char*	a_cpFieldName	= NULL;
		int		a_iWidth		= 0;
		int		a_iDecimals		= 0;
		int		a_iType			= 0;

		// フィールド情報取得
		a_iType			= a_cDBFHdl.get_FieldInfo( a_iFidx, &a_cpFieldName, &a_iWidth, &a_iDecimals );

		a_viField_Indexes.push_back	( a_iFidx );
		a_vsField_Names.push_back	( a_cpFieldName );
		a_viField_Types.push_back	( a_iType );
	}

	// 各レコードの情報取得
	int		a_iCount = 0;
	for( a_iCount = 0 ; a_iCount < a_iRecCount ; a_iCount++ )
	{
		shp1	a_shpCurData;
		
		// フィールド数保存
		a_shpCurData.iField_num		= a_iFieldCount;

		// フィールドのループ
		for( a_iFidx = 0 ; a_iFidx < a_iFieldCount ; a_iFidx++ )
		{
			int			a_iData;
			double		a_dData;
			const char*	a_cpData;
			wchar_t		a_wcWstr[512];
			string		temp;

			memset( a_wcWstr, '\0', 256 );

			bool		a_bFailed_Flag	= false;

			// フィールド名を順番に格納しておく
			a_shpCurData.vsField_name.push_back( a_vsField_Names[a_iFidx] );
			//a_shpCurData.msField_name.insert( map<int, string>::value_type( a_iFidx, a_vsField_Names[a_iFidx] ) );

			// フィールドタイプにより処理分け
			switch( a_viField_Types[a_iFidx] )
			{
			case		FTString:
				a_cDBFHdl.read_Attribute( a_iCount, a_iFidx, &a_cpData );
				MultiByteToWideChar( CP_UTF8, 0, a_cpData, -1, a_wcWstr, 512 );
				break;

			case		FTInteger:
				a_cDBFHdl.read_Attribute( a_iCount, a_iFidx, &a_iData );
				temp	= boost::lexical_cast<string>(a_iData);
				MultiByteToWideChar( CP_UTF8, 0, temp.c_str(), -1, a_wcWstr, 512 );
				break;

			case		FTDouble:
				a_cDBFHdl.read_Attribute( a_iCount, a_iFidx, &a_dData );
				temp	= boost::lexical_cast<string>(a_dData);
				MultiByteToWideChar( CP_UTF8, 0, temp.c_str(), -1, a_wcWstr, 512 );
				break;
			
			default:
				// 扱っていないタイプ
				a_bFailed_Flag	= true;
				break;
			}
			
			if( a_bFailed_Flag == true )
				return false;
			if( ( m_iExeMode & ON_FID_MODE ) && ( a_vsField_Names[a_iFidx] == "OBJECTID" || a_vsField_Names[a_iFidx] == "IPC_ID" ) )
				continue;
			else if( ( m_iExeMode & ON_OBJECTID_MODE ) && ( a_vsField_Names[a_iFidx] == "OBJECTID" || a_vsField_Names[a_iFidx] == "IPC_ID" ) )
				a_shpCurData.shp_ID		= a_iData;
			else
				a_shpCurData.Fields.insert( map<string, wstring>::value_type( a_vsField_Names[a_iFidx], a_wcWstr ) );
		}
		// FIDモードならば、ここでインデックス番号をshp_IDに格納
		if( m_iExeMode & ON_FID_MODE )
			a_shpCurData.shp_ID		= a_iCount + 1;

		// shapeの補間点情報の追加
		if( !a_cSHPHdl.open((char*)sFullPass.c_str(), "rb") )
		{
			// 道路系はshpファイルが無いものがあるので、地図系のみエラー
			if( m_iExeMode	|= ON_MAP_MODE )
			{				
				cerr << "#ERROR SHPファイル<" << sFullPass << ">のオープンに失敗！" << endl;
				m_ofErrorLog << "#ERROR SHPファイル<" << sFullPass << ">のオープンに失敗！" << endl;
				return false;
			}
		}
		else
		{
			CSHPObject		a_cTmpOBJ;
			CSHPPoint		a_cTmpPoint;

			if( a_cSHPHdl.read_Object( a_iCount, &a_cTmpOBJ ) == -1 )
			{
				cerr << "#ERROR shapeOBJECT作成失敗" << endl;
				m_ofErrorLog << "#ERROR shapeOBJECT作成失敗" << endl;
				return false;
			}
			else
			{
				// 補間点数取得
				//a_shpCurData.point_num = a_cTmpOBJ.get_PointCount();
				
				// (x,y)取得
				a_cTmpOBJ.get_Point( 0, &a_cTmpPoint );
				a_shpCurData.shp_dX = a_cTmpPoint.get_X();
				a_shpCurData.shp_dY = a_cTmpPoint.get_Y();
			}
		}
		

		// ID取得確認
		if( a_shpCurData.shp_ID == 0 )
		{
			cerr << "#ERROR IDが取得できませんでした！" << endl;
			m_ofErrorLog << "#ERROR IDが取得できませんでした！" << endl;
			return false;
		}
		a_shpCurData.iMeshCode	= iMesh;
		mmShp1.insert( mmShp1::value_type( a_shpCurData.shp_ID, a_shpCurData ) );
	}
	a_cSHPHdl.close();
	a_cDBFHdl.close();

	return true;
}

/**
 * @brief	二つのShapeファイルの属性と補間点数を比較し、異なればエラー出力
 * @param	mmShpOri	[in]	比較元Shapeファイル中身格納用クラス集合コンテナ<ID, shp1本体>
 * @param	mmShpDest	[in]	比較先Shapeファイル中身格納用クラス集合コンテナ<ID, shp1本体>
 */
bool	CCompShape2Shape::comp_shape_info( mmShp1 mmShpOri, mmShp1 mmShpDest )
{
	bool bRet = true;
	itrmmShp1						a_itrmmShpOri, a_itrmmShpDest;				// shapeコンテナイテレータ
	map<string, wstring>::iterator	a_itrmFieldsOri, a_itrmFieldsDest;			// コンテナ内のフィールドのイテレータ
	vector<string>::iterator		a_itrvsFieldNameOri, a_itrvsFieldNameDest;	// フィールド名のイテレータ
	int								i = 0;										// ループ用

	// 比較元全て
	for( a_itrmmShpOri = mmShpOri.begin() ; a_itrmmShpOri != mmShpOri.end() ; a_itrmmShpOri++ )
	{
		a_itrmmShpDest	= mmShpDest.find( a_itrmmShpOri->first );
		if( a_itrmmShpDest != mmShpDest.end() )
		{
			if( a_itrmmShpDest->second.check_flag == 0 )
			{
				// 補間点数比較
				//if( a_itrmmShpOri->second.point_num != a_itrmmShpDest->second.point_num )
				//{
				//	m_ofErrorLog
				//		<< "0\t" << m_sLayerName << "\t" << a_itrmmShpOri->second.shp_ID << "\t"
				//		<< a_itrmmShpOri->second.shp_dX << "\t" << a_itrmmShpOri->second.shp_dY << "\tERROR\t"
				//		<< "POINTERR\t補間点の個数が<" << a_itrmmShpOri->second.point_num << ">と<"
				//		<< a_itrmmShpDest->second.point_num << ">で異なる\t" << a_itrmmShpOri->second.iMeshCode << endl;
				//
				//}
				
				// フィールド数の比較
				if( a_itrmmShpOri->second.iField_num != a_itrmmShpDest->second.iField_num )
				{
					m_ofErrorLog
						<< "0\t" << m_sLayerName << "\t" << a_itrmmShpOri->second.shp_ID << "\t"
						<< a_itrmmShpOri->second.shp_dX << "\t" << a_itrmmShpOri->second.shp_dY << "\tERROR\t"
						<< "FIELDNUMERR(属性)\tフィールドの個数が異なっています\t" << a_itrmmShpOri->second.iMeshCode << endl;
					bRet=false;
				}

				// 全てのフィールドが順番通り格納されているか比較
				for( i = 0 ; i < a_itrmmShpOri->second.iField_num ; i++ )
				{
					if( strcmp( a_itrmmShpOri->second.vsField_name[i].c_str(), a_itrmmShpDest->second.vsField_name[i].c_str() ) != 0 )
					{
						m_ofErrorLog 
							<< "0\t" << m_sLayerName << "\t" << a_itrmmShpOri->second.shp_ID << "\t"
							<< a_itrmmShpOri->second.shp_dX << "\t" << a_itrmmShpOri->second.shp_dY << "\tERROR\t"
							<< "FIELDSEQERR(属性)\tフィールドの順番が異なっています\t" << a_itrmmShpOri->second.iMeshCode << endl;
						bRet=false;
					}
				}
				a_itrmFieldsDest	= a_itrmmShpDest->second.Fields.begin();
				// 全てのフィールドの属性が等しいか比較(↑でフィールドの順番をチェックしているので、フィールド順は等しいこと前提)
				for( a_itrmFieldsOri = a_itrmmShpOri->second.Fields.begin() ; a_itrmFieldsOri != a_itrmmShpOri->second.Fields.end() ; a_itrmFieldsOri++ )
				{				
					if( wcscmp( a_itrmFieldsOri->second.c_str(), a_itrmFieldsDest->second.c_str() ) != 0 )
					{
						m_ofErrorLog 
							<< "0\t" << m_sLayerName << "\t" << a_itrmmShpOri->second.shp_ID << "\t"
							<< a_itrmmShpOri->second.shp_dX << "\t" << a_itrmmShpOri->second.shp_dY << "\tERROR\t"
							<< "ATTRERR(属性)\tフィールド<" << a_itrmFieldsOri->first.c_str() << ">の値が異なる\t"
							<< a_itrmmShpOri->second.iMeshCode << endl;
						bRet=false;
					}
					a_itrmFieldsDest++;
				}
				a_itrmmShpOri->second.check_flag	= 1;
				a_itrmmShpDest->second.check_flag	= 1;
			}
		}
	}
	
	// 未チェックのものが残っていないか？
	for( a_itrmmShpOri = mmShpOri.begin() ; a_itrmmShpOri != mmShpOri.end() ; a_itrmmShpOri++ )
	{
		if( a_itrmmShpOri->second.check_flag == 0 )
		{
			m_ofErrorLog
				<< "0\t" << m_sLayerName << "\t" << a_itrmmShpOri->second.shp_ID << "\t"
				<< a_itrmmShpOri->second.shp_dX << "\t" << a_itrmmShpOri->second.shp_dY << "\tERROR\t"
				<< "DATAFINDERR(属性)\t対応するIDのものが比較先になく未チェックです\t"
				<< a_itrmmShpOri->second.iMeshCode << endl;
			bRet=false;
		}
	}
	for( a_itrmmShpDest = mmShpDest.begin() ; a_itrmmShpDest != mmShpDest.end() ; a_itrmmShpDest++ )
	{
		if( a_itrmmShpDest->second.check_flag == 0 )
		{
			m_ofErrorLog
				<< "0\t" << m_sLayerName << "\t" << a_itrmmShpDest->second.shp_ID << "\t"
				<< a_itrmmShpDest->second.shp_dX << "\t" << a_itrmmShpDest->second.shp_dY << "\tERROR\t"
				<< "DATAFINDERR(属性)\t対応するIDのものが比較元になく未チェックです\t"
				<< a_itrmmShpDest->second.iMeshCode << endl;
			bRet=false;
		}
	}
	return bRet;
}

/**
 * @brief	Shapeファイルをバイナリで読み込み、shp2に落とし込む
 * @param	sFullPass	[in]	読み込みShapeファイルのフルパス
 * @param	iMesh		[in]	メッシュコード
 * @param	cShp2		[out]	Shapeファイル中身格納用クラス（バイナリ）
 * @return	bool
 */
bool	CCompShape2Shape::load_shape_info_bin(std::string sFullPass, int iMesh, shp2 &cShp2)
{
	ifstream			a_ifShapeBin;				// Shapeファイル読み込み用（バイナリ）
	int					i = 0, j = 0, k = 0;		// ループ用

	cShp2.iMeshCode		= iMesh;

	// 直接ファイルを開くので、.dbf追加
	sFullPass			+= ".dbf";

	// バイナリモードでShapeファイルオープン
	a_ifShapeBin.open( sFullPass.c_str(), ios::in|ios::binary );
	
	if( !a_ifShapeBin )
	{
		cerr << "#ERROR ファイル<" << sFullPass << ">のオープンに失敗！" << endl;
		m_ofErrorLog << "#ERROR ファイル<" << sFullPass << ">のオープンに失敗！" << endl;
		return false;
	}

	// 必要ない分読み飛ばし
	a_ifShapeBin.seekg( 4 );
	
	char				a_cRecNum[4];
	char				a_cHeadSize[2];
	char				a_cRecSize[2];
	char				a_cFieldName[11];
	char				a_cUnicodeComp[2];
	
	// ヘッダ終点比較用符号（0DH）
	a_cUnicodeComp[0]	= 0x0d;

	// 文字フィールド空白比較用
	a_cUnicodeComp[1]	= 0x20;

	memset( a_cRecNum, '\0', sizeof( a_cRecNum ) );
	memset( a_cHeadSize, '\0', sizeof( a_cHeadSize ) );
	memset( a_cRecSize, '\0', sizeof( a_cRecSize ) );
	memset( a_cFieldName, '\0', sizeof( a_cFieldName ) );

	// レコード数取得
	a_ifShapeBin.read( a_cRecNum, sizeof( a_cRecNum ) );
	memcpy( &cShp2.iRec_num, a_cRecNum, sizeof( a_cRecNum ) );

	// ヘッダサイズ取得
	a_ifShapeBin.read( a_cHeadSize, sizeof( a_cHeadSize ) );
	memcpy( &cShp2.iHeadSize, a_cHeadSize, sizeof( a_cHeadSize ) );

	// 1レコードのサイズ取得
	a_ifShapeBin.read( a_cRecSize, sizeof( a_cRecSize ) );
	memcpy( &cShp2.iRecSize, a_cRecSize, sizeof( a_cRecSize ) );

	string	a_sTemp;			// フィールド名、1レコード取得用
	char	a_cID[128];			// 取得したレコードからID部分の抜き取りよう
	int		a_iID	= 0;		// キーにするID格納用
	char	a_cIDFieldSize[1];	// IDフィールドサイズ格納用
	int		a_iIDFieldSize = 0;	// IDフィールドサイズ格納用
	int		a_iFlag	= 0;		// チェックフラグ=0を格納用


	memset( a_cIDFieldSize, '\0', sizeof( a_cIDFieldSize ) );

	// 必要ない分読み飛ばし
	a_ifShapeBin.seekg( 20, ios::cur );
	
	// ヘッダ終点符号が現れるまで
	while(1)
	{
		// 1フィールド名取得
		a_ifShapeBin.read( a_cFieldName, sizeof( a_cFieldName ) );

		// 先頭がヘッダ終点符号ならばヘッダ部終了
		if( a_cFieldName[0] ==  a_cUnicodeComp[0] )
			break;
		if( m_iExeMode & ON_OBJECTID_MODE && ( strcmp( a_cFieldName, "OBJECTID" ) == 0 || strcmp( a_cFieldName, "IPC_ID" ) == 0 ) )
		{
			// 必要ない分読み飛ばし
			a_ifShapeBin.seekg( 5, ios::cur );
			
			// フィールドサイズ読みたい	
			a_ifShapeBin.read( a_cIDFieldSize, 1 );
			memcpy( &a_iIDFieldSize, a_cIDFieldSize, sizeof( a_cIDFieldSize ) );
			
			// 必要ない分読み飛ばし
			a_ifShapeBin.seekg( 15, ios::cur );
			
			// 下の方でフィールドサイズが格納されてなければ、FIDの代わりにレコード番号のシーケンスを格納する。
		}
		else
		{
			// 必要ない分読み飛ばし
			a_ifShapeBin.seekg( 21, ios::cur );
		}

		a_sTemp		= a_cFieldName;
		cShp2.vsField_name.push_back( a_sTemp );
	}

	// データ部最初へシークしてから開始
	a_ifShapeBin.seekg( cShp2.iHeadSize, ios::beg );
	
	// ヘッダとレコードの切れ目の余白を読み飛ばし
	a_ifShapeBin.seekg( 1, ios::cur );

	// 1レコード分のバイナリ列を順々にクラスの中のmultisetにオブジェクトIDをキーに格納する
	for( i = 0 ; i < cShp2.iRec_num ; i++ )
	{
		// 初期化
		a_iID		= 0;

		char *a_cData = new char[cShp2.iRecSize];
		memset( a_cData, '\0', cShp2.iRecSize );

		// 1レコード分のバイナリ列を丸ごと取得
		a_ifShapeBin.read( a_cData, cShp2.iRecSize );

		// OBJECTID・IPC_IDのフィールドサイズが取得されていれば、それを基にIDを取得
		if( a_iIDFieldSize != 0 )
		{
			memset( a_cID, '\0', sizeof( a_cID ) );
			memcpy( &a_cID, a_cData, a_iIDFieldSize );
			
			// キー用のオブジェクトIDをINTに変換する。
			a_iID	= atoi( a_cID );
		}
		
		// この時点でOBJECTID・IPC_IDが格納されていなければ、FIDモードかOBJECTIDのないレイヤなので、レコード番号を格納する。
		if( a_iID	== 0 )
		{
			a_iID	= i + 1;
		}
		
		// string のコンストラクタでサイズを指定することで、バイナリ列全体がきっちり格納されるようにする
		cShp2.msBinData.insert( multimap<int, pair<int, string> >::value_type( a_iID,  pair<int, string>( a_iFlag, string( a_cData, cShp2.iRecSize ) ) ) );

		delete [] a_cData;
	}
	return true;
}


/**
 * @brief	二つのShapeファイルのレコードをバイナリで比較し、異なればエラー出力
 * @param	cShp2Ori	[in]	比較元Shapeファイル中身格納用クラス（バイナリ）
 * @param	cShp2Dest	[in]	比較先Shapeファイル中身格納用クラス（バイナリ）
 */
bool	CCompShape2Shape::comp_shape_info_bin(shp2 cShp2Ori, shp2 cShp2Dest)
{
	bool bRet = true;
	// フィールド名イテレータ
	vector<string>::iterator		a_itrvsFiNameOri, a_itrvsFiNameDest;
	
	// レコードごとのバイナリデータイテレータ
	multimap<int, pair<int, string>>::iterator	a_itrmsBiDataOri, a_itrmsBiDataDest;
	//vector<string>::iterator		a_itrvsBiDataOri, a_itrvsBiDataDest;
	
	// フィールド名の個数比較
	if( cShp2Ori.vsField_name.size() != cShp2Dest.vsField_name.size() )
	{
		m_ofErrorLog
			<< "0\t" << m_sLayerName << "\t\t\t\tERROR\tFIELDNUMEERR\t"
			<< "フィールドの個数が異なっています\t" << cShp2Ori.iMeshCode << endl;
		bRet=false;
	}

	// 各フィールド名の比較
	a_itrvsFiNameDest		= cShp2Dest.vsField_name.begin();
	for( a_itrvsFiNameOri = cShp2Ori.vsField_name.begin() ; a_itrvsFiNameOri != cShp2Ori.vsField_name.end() ; a_itrvsFiNameOri++ )
	{
		if( strcmp( a_itrvsFiNameOri->c_str(), a_itrvsFiNameDest->c_str() ) != 0 )
		{
			m_ofErrorLog
				<< "0\t" << m_sLayerName << "\t\t\t\tERROR\tFIELDNAMEERR\tフィールド名が<"
				<< a_itrvsFiNameOri->c_str() << ">と<" << a_itrvsFiNameDest->c_str()
				<< ">で異なっています\t" << cShp2Ori.iMeshCode << endl;
			bRet=false;
		}
		a_itrvsFiNameDest++;
	}

	// バイナリレコードの個数比較
	if( cShp2Ori.msBinData.size() != cShp2Dest.msBinData.size() )
	{
		m_ofErrorLog
			<< "0\t" << m_sLayerName << "\t\t\t\tERROR\tDATANUMEERR\t"
			<< "データの個数が異なっています\t" << cShp2Ori.iMeshCode << endl;
		bRet=false;
	}
	for( a_itrmsBiDataOri = cShp2Ori.msBinData.begin() ; a_itrmsBiDataOri != cShp2Ori.msBinData.end() ; a_itrmsBiDataOri++)
	{
		// それぞれのShapeのバイナリデータ列からIDをキーに検索し、紐づいたら、バイナリ列比較
		a_itrmsBiDataDest		= cShp2Dest.msBinData.find( a_itrmsBiDataOri->first );
		
		// 対応するIDのものが見つからない
		if( a_itrmsBiDataDest	== cShp2Dest.msBinData.end() )
		{
			m_ofErrorLog
				<< "0\t" << m_sLayerName << "\t" << a_itrmsBiDataOri->first 
				<< "\t\t\tERROR\tBiDATAFINDERR\t対応するIDのものが比較先にありません\t" 
				<< cShp2Ori.iMeshCode << endl;
			bRet=false;
		}

		// 見つかった
		else
		{
			// 同じIDを持つデータのバイナリ列を比較する
			if( strcmp( a_itrmsBiDataOri->second.second.c_str(), a_itrmsBiDataDest->second.second.c_str() ) != 0 )
			{
				m_ofErrorLog
					<< "0\t" << m_sLayerName << "\t" << a_itrmsBiDataOri->first
					<< "\t\t\tERROR\tBiDATAERR\tバイナリ列が比較元と比較先で異なっています\t"
					<< cShp2Ori.iMeshCode << endl;
				bRet=false;
			}
			
			// 見つかった場合は比較元・比較先のデータのチェックフラグを立てる
			a_itrmsBiDataOri->second.first++;
			a_itrmsBiDataDest->second.first++;
		}
	}

	// 未チェックのものが残っていないか？
	for( a_itrmsBiDataOri = cShp2Ori.msBinData.begin() ; a_itrmsBiDataOri != cShp2Ori.msBinData.end() ; a_itrmsBiDataOri++ )
	{
		if( a_itrmsBiDataOri->second.first == 0 )
		{
			m_ofErrorLog
				<< "0\t" << m_sLayerName << "\t" << a_itrmsBiDataOri->first
				<< "\t\t\tERROR\tBiDATAFINDERR\t対応するIDのものが比較先になく未チェックです\t"
				<< cShp2Ori.iMeshCode << endl;
			bRet=false;
		}
	}
	for( a_itrmsBiDataDest = cShp2Dest.msBinData.begin() ; a_itrmsBiDataDest != cShp2Dest.msBinData.end() ; a_itrmsBiDataDest++ )
	{
		if( a_itrmsBiDataDest->second.first == 0 )
		{
			m_ofErrorLog
				<< "0\t" << m_sLayerName << "\t" << a_itrmsBiDataDest->first
				<< "\t\t\tERROR\tBiDATAFINDERR\t対応するIDのものが比較元になく未チェックです\t"
				<< cShp2Dest.iMeshCode << endl;
			bRet=false;
		}
	}
	
	return bRet;
}


/**
 * @brief	実行
 * @return	bool
 */
int	CCompShape2Shape::execute()
{
	int iErrFlag = 0;
	int iRET = 0;
	
	// ログの出力の際、桁数が丸め込まれないように設定
	m_ofErrorLog << setprecision(10);
	// 初期情報をログに出力
	::GetLocalTime( &m_st );
	print_ver();
	print_pass();
	print_begin_time();
	print_log_head();
	m_ofErrorLog.flush();

	vector<string>::iterator	a_itrMesh;
	for( a_itrMesh = m_vsMesh.begin() ; a_itrMesh != m_vsMesh.end() ; a_itrMesh++ )
	{
		cerr << "<" << a_itrMesh->c_str() << ">処理中....." << endl;
		
		int		a_iMesh	= atoi( (*a_itrMesh).c_str() );
		string	a_sFullPassOri, a_sFullPassDest;
		
		// パス作成(比較元)
		a_sFullPassOri		= make_mesh_fname( a_iMesh, (char*)m_sOriFPass.c_str() );
		
		// パス作成(比較先)
		a_sFullPassDest		= make_mesh_fname( a_iMesh, (char*)m_sDestFPass.c_str() );
	
		//オプションで-ATTRが指定されていた場合、属性比較チェックを行う
		if( m_bAttr ){
			// ノーマルモード
			if( m_iExeMode & ON_EXE_NORMAL_MODE )
			{
				mmShp1				a_mmShp1_Ori, a_mmShp1_Dest;

				// shape読み込み(比較元)
				if( !load_shape_info( a_sFullPassOri, a_iMesh, a_mmShp1_Ori ) )
				{
					cerr << "#ERROR 比較元Shapeファイルの読み込みに失敗！" << endl;
					iErrFlag = 1;
					continue;
				}

				// shape読み込み(比較先)
				if( !load_shape_info( a_sFullPassDest, a_iMesh, a_mmShp1_Dest ) )
				{
					cerr << "#ERROR 比較先Shapeファイルの読み込みに失敗！" << endl;
					iErrFlag = 1;
					continue;
				}

				// 2つのshapeの属性比較
				if (!comp_shape_info( a_mmShp1_Ori, a_mmShp1_Dest ))
				{
					iRET = 3;
					cerr << "属性比較でエラーが存在しました" << endl;
				}

				// 二つのshapeクラスの解放
				a_mmShp1_Ori.clear();
				a_mmShp1_Dest.clear();
			}
			
			// バイナリモード
			else if( m_iExeMode & ON_EXE_BINARY_MODE )
			{
				shp2				a_cShp2_Ori, a_cShp2_Dest;

				// shape読み込み(比較元)
				if( !load_shape_info_bin( a_sFullPassOri, a_iMesh, a_cShp2_Ori ) )
				{
					cerr << " #ERROR 比較元Shapeファイルの読み込みに失敗！" << endl;
					iErrFlag = 1;
					continue;
				}

				// shape読み込み(比較先)
				if( !load_shape_info_bin( a_sFullPassDest, a_iMesh, a_cShp2_Dest ) )
				{
					cerr << " #ERROR 比較先Shapeファイルの読み込みに失敗！" << endl;
					iErrFlag = 1;
					continue;
				}

				// 2つのshapeの属性比較
				if (!comp_shape_info_bin( a_cShp2_Ori, a_cShp2_Dest ))
				{
					iRET= 3;
					cerr << "属性比較でエラーが存在しました" << endl;
				}

			}else{
				return false;
			}
			if (iRET == 0){
				cerr << "属性比較完了！" << endl;
			}
		}
		//オプションで-SHPモードが指定されていたら、形状チェック開始
		if( m_bShp )
		{
			//形状チェック
			CShapeChk aShapeChk(&m_ofErrorLog,a_iMesh,m_iExeMode,m_sLayerName,a_sFullPassOri,a_sFullPassDest,m_dDistBorder);
			if( !aShapeChk.app()){
				cerr << "形状比較でエラーが存在しました" << endl;
				iRET = 3;
			}else{
				cerr << "形状比較完了!" << endl;
			}
		}

	}
	::GetLocalTime( &m_st );
	print_end_time();
	m_ofErrorLog.flush();

	if (iErrFlag == 1)
	{
		return 1;
	} else {
		return iRET;
	}
}
