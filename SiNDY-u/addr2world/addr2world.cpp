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

// addr2world.cpp : コンソール アプリケーションのエントリ ポイントを定義します。
//

#include "stdafx.h"


static std::set<CString> Filenames;
static CString Srcdir = _T("");
static CString Dstdir = _T("");

static crd_cnv Crdcnv;
static bool To_jgd2000 = false;

struct AddrFile {
	LPCTSTR		m_fileName;
	int			m_columnCount;
	int			m_lonCol;
};

static AddrFile AddrFileInfo[] = {
	{_T("pref.txt"), 5, 3},
	{_T("city.txt"), 6, 4},
	{_T("addr*.txt"), 11, 9},
	{_T("pnt*.txt"), 10, 7},
	{_T("gou*.txt"), 10, 8},
	{NULL, 0, 0}
};

bool usage()
{
	std::cerr << "住所データを世界測地系変換するプログラム" << std::endl;
	std::cerr << "Usage : addr2world.exe [Option]" << std::endl;
	std::cerr << "Option : " << std::endl;
	std::cerr << "         -i [日本測地系住所データフォルダの指定(必須)]" << std::endl;
	std::cerr << "         -o [世界測地系住所データフォルダ(出力先)の指定(必須)]" << std::endl;
	std::cerr << "         -r [地域パラメータのパス指定(任意、指定された場合は地域パラメータ、指定しない場合は3パラメータでの変換)]" << std::endl;
	std::cerr << "※入力ファイルは、ＤＢ販売向けの10進数座標の住所データテキストファイルと、郵便番号データ" << std::endl;
	std::cerr << "※指定したフォルダ以下の該当ファイルを検索して処理します" << std::endl;
	return false;
}

bool arg_check(int argc, _TCHAR **argv)
{
	// 引数がなければusageを出力
	if (argc <= 1)
		return usage();

	// オプション解析
	while (--argc) {
		_TCHAR *s = *++argv;
		switch (*s) {
		case _T('-'):
			switch (*++s) {
			case _T('i'):
				Srcdir = *++argv; --argc; break;
			case _T('o'):
				Dstdir = *++argv; --argc; break;
			case _T('r'):
				{
					CString fname = *++argv; --argc;
					std::string param = CT2CA(fname);
					std::cerr << param.c_str() << "で初期化中...";
					if (! Crdcnv.JGD2000_RP_Init((char *)param.c_str())) {
						std::cerr << "地域パラメータの初期化に失敗" << std::endl;
						return false;
					}
					std::cerr << "完了" << std::endl;
					To_jgd2000 = true;
				}
				break;
			default:
				std::cerr << "invalid option." << std::endl;
				return usage();
			}
			break;
		default:
			return usage();
		}
	}

	// 必須オプションの有無チェック
	if (Srcdir.GetLength() == 0 || Dstdir.GetLength() == 0) {
		std::cerr << "-iオプションと-oオプションは必須です" << std::endl;
		return usage();
	}

	return true;
}

/**
 *	@brief	<b>ディレクトリ作成</b>\n
 *	@note
 *	@param	lpDirectory [in] 作成するディレクトリ
 *	@return	bool
 */
bool make_folder(LPCTSTR lpDirectory)
{
	if (! lpDirectory || ! lstrlen(lpDirectory))
		return false;

	CString cDirectory = _T("");

	for (INT i = 0; i < lstrlen(lpDirectory); i++) {
		if (cDirectory.GetLength() && cDirectory.GetAt(cDirectory.GetLength()-1) != _T(':') && lpDirectory[i] == _T('\\') && _taccess(cDirectory, 0) != 0) {
			_tmkdir(cDirectory);
		}
		cDirectory += lpDirectory[i];
	}

	if (cDirectory.GetAt(cDirectory.GetLength()-1) != _T(':') &&  _taccess(cDirectory, 0) != 0) {
		_tmkdir(cDirectory);
	}

	return true;
}

void get_out_path(LPCTSTR source, CString &destination, CString &destination_folder)
{
	CString srcpath = source;
	CString subpath = srcpath.Right(srcpath.GetLength() - Srcdir.GetLength());
	CString dstpath = Dstdir;
	if (dstpath[dstpath.GetLength()-1] == _T('\\')) {
		dstpath = dstpath.Left(dstpath.GetLength()-1);
	}
	destination = dstpath + subpath;

	TCHAR title[BUFSIZ*sizeof(TCHAR)+sizeof(TCHAR)];
	GetFileTitle(destination, title, BUFSIZ*sizeof(TCHAR));

	destination_folder = destination.Left(destination.GetLength() - lstrlen(title));
	if (destination_folder[destination_folder.GetLength()-1] == _T('\\')) {
		destination_folder = destination_folder.Left(destination_folder.GetLength()-1);
	}
}

/**
 *	@brief	世界測地系変換
 *	@param	filepattern		ファイル名またはパターンの指定[in]
 *	@param	tokencheck		1レコードのカラム数の指定[in]
 *	@param	loncol			経度のカラム番号(この次に緯度が必ず来る前提)[in]
 *	@return	bool			成功：true、失敗：false
 */
int to_world(LPCTSTR filePattern, int tokencheck, int loncol, int *filenum)
{
	FileFinder finder;
	if (! finder.Search(Srcdir, filePattern, true)) {
		std::cerr << CT2CA(filePattern) << "の検索に失敗" << std::endl;
		return -1;
	}
	int count = finder.GetCount(), recnum = 0;
	*filenum = count;
	for (int i = 0; i < count; i++) {
		// 出力先のパスの生成
		CString dstpath = _T(""), dstdir = _T("");
		get_out_path(finder[i], dstpath, dstdir);
		make_folder(dstdir);

		// 読み取り
		TSVFileReader reader;
		if (reader.read(finder[i]) < 0) {
			std::cerr << CT2CA(finder[i]) << "の読み取りに失敗" << std::endl;
			return -1;
		}

		std::cerr << CT2CA(finder[i]) << "を処理中...";

		// 出力ファイルのオープン
		std::ofstream file(dstpath);
		if (! file.is_open()) {
			std::cerr << "出力ファイルのオープンに失敗" << std::endl;
			return -1;
		}

		// 出力
		for (unsigned int n = 0; n < reader.size(); n++) {

			// レコードのトークン数のチェック
			if (reader[n].size() != tokencheck) {
				std::cerr << CT2CA(finder[i]) << "のフォーマットが不正" << std::endl;
				return -1;
			}

			for (unsigned int m = 0; m < reader[n].size(); m++) {
				if (m != 0) {
					file << "\t";
				}
				if (m != loncol) {
					std::string tmp = CT2CA(reader[n][m]);
					file << tmp.c_str();
				} else {
					double lon = _tcstod(reader[n][m], 0);
					double lat = _tcstod(reader[n][m+1], 0);
					double lon2, lat2, alt2;
					if (To_jgd2000) {
						Crdcnv.TOKYOtoJGD2000_RP(lon, lat, &lon2, &lat2, true);
					} else {
						Crdcnv.TOKYOtoJGD2000(lon, lat, 0.0, &lon2, &lat2, &alt2);
					}
					file.setf(std::ios_base::fixed, std::ios_base::floatfield);
					file.precision(11);
					file << lon2 << "\t" << lat2;
					file.clear();
					m++; // 緯度分のカラム数を追加
				}
			}
			file << "\n";
			recnum++;
		}

		std::cerr << "正常終了" << std::endl;
	}
	return recnum;
}

int to_world_zip(int *filenum)
{
		FileFinder finder;
	if (! finder.Search(Srcdir, _T("zip.txt"), true)) {
		std::cerr << "zip.txtの検索に失敗" << std::endl;
		return -1;
	}
	int count = finder.GetCount(), recnum = 0;
	*filenum = count;
	for (int i = 0; i < count; i++) {
		// 出力先のパスの生成
		CString dstpath = _T(""), dstdir = _T("");
		get_out_path(finder[i], dstpath, dstdir);
		make_folder(dstdir);

		// 読み取り
		TSVFileReader reader;
		if (reader.read(finder[i]) < 0) {
			std::cerr << CT2CA(finder[i]) << "の読み取りに失敗" << std::endl;
			return -1;
		}

		// 出力ファイルのオープン
		std::ofstream file(dstpath);
		if (! file.is_open()) {
			std::cerr << "出力ファイルのオープンに失敗" << std::endl;
			return -1;
		}

		std::cerr << CT2CA(finder[i]) << "を処理中...";

		// 出力
		for (unsigned int n = 0; n < reader.size(); n++) {

			// レコードのトークン数のチェック
			if (reader[n].size() != 13) {
				std::cerr << CT2CA(finder[i]) << "のフォーマットが不正" << std::endl;
				return -1;
			}

			int mesh = _ttoi(reader[n][0]); // メッシュコード
			int x = _ttoi(reader[n][1]); // X座標
			int y = _ttoi(reader[n][2]); // Y座標
			// 緯度経度に変換
			double lon, lat;
			Crdcnv.MeshtoLL(mesh, x, y, &lon, &lat);
			double lon2, lat2, alt2;
			if (To_jgd2000) {
				Crdcnv.TOKYOtoJGD2000_RP(lon, lat, &lon2, &lat2, true);
			} else {
				Crdcnv.TOKYOtoJGD2000(lon, lat, 0.0, &lon2, &lat2, &alt2);
			}
			file.setf(std::ios_base::fixed, std::ios_base::floatfield);
			file.precision(11);
			file << lon2 << "\t" << lat2;
			file.clear();

			for (unsigned int m = 3; m < reader[n].size(); m++) {
				file << "\t";
				std::string tmp = CT2CA(reader[n][m]);
				file << tmp.c_str();
			}

			file << "\n";
			recnum++;
		}

		std::cerr << "正常終了" << std::endl;
	}
	return recnum;
}

bool run()
{
	// 情報の出力
	std::cerr << "----------入力情報----------" << std::endl;
	std::cerr << "　入力フォルダ：" << CT2CA(Srcdir) << std::endl;
	std::cerr << "　出力フォルダ：" << CT2CA(Dstdir) << std::endl;
	std::cerr << "　座標変換方式：";
	if (To_jgd2000) {
		std::cerr << "JGD2000" << std::endl;
	} else {
		std::cerr << "3パラメータ" << std::endl;
	}
	std::cerr << "----------------------------" << std::endl;

	// 出力フォルダの作成
	make_folder(Dstdir);

	CString src = Srcdir; src.MakeLower();
	CString dst = Srcdir.Right(Srcdir.GetLength() - src.GetLength());

	int recnum[10] = {0, }, filenum[10] = {0, }, n = 0;

	// 住所系
	for (int i = 0; AddrFileInfo[i].m_fileName != NULL; i++, n++) {
		if ((recnum[n] = to_world(AddrFileInfo[i].m_fileName, AddrFileInfo[i].m_columnCount, AddrFileInfo[i].m_lonCol, &filenum[n])) < 0)
			return false;
	}

	// 郵便番号
	if ((recnum[n] = to_world_zip(&filenum[n])) < 0)
		return false;
	n++;

	// 結果の出力
	std::cerr << "----------出力結果----------" << std::endl;
	std::cerr << "　処理したファイル数：" << std::endl;
	for (int i = 0; i < (n-1); i++) {
		std::cerr << "　　" << CT2CA(AddrFileInfo[i].m_fileName) << "：" << filenum[i] << std::endl;
	}
	std::cerr << "　　zip.txt：" << filenum[n-1] << std::endl;
	std::cerr << "　処理したレコード数：" << std::endl;
	for (int i = 0; i < (n-1); i++) {
		std::cerr << "　　" << CT2CA(AddrFileInfo[i].m_fileName) << "：" << recnum[i] << std::endl;
	}
	std::cerr << "　　zip.txt：" << recnum[n-1] << std::endl;
	std::cerr << "----------------------------" << std::endl;

	return true;
}

int _tmain(int argc, _TCHAR* argv[])
{
	clock_t start_time = clock();

	int retval = 0;
	try {
		if (! arg_check(argc, argv)) {
			retval = -1;
		} else if (! run()) {
			retval = 1;
		} else {
			retval = 0;
		}
	} catch (...) {
		retval = 2;
	}
	switch (retval) {
	case 0: std::cerr << "正常終了しました" << std::endl; break;
	case 1: std::cerr << "異常終了しました" << std::endl; break;
	case 2: std::cerr << "例外が発生しました" << std::endl; break;
	default: break;
	}
	clock_t end_time = clock();
	double proc_time = (double)(end_time - start_time) / CLOCKS_PER_SEC;
	std::cerr << "実行時間：" << proc_time << "秒" << std::endl;
	return retval;
}

