#include "stdafx.h"
#include "main.h"
#include "ShapeCodeChecker.h"
#include "..\..\LIB\WinLib\VersionInfo.h"

int _tmain(int argc, _TCHAR* argv[])
{
	try{
		// ロケール関連の不具合防止
		uh::setJapaneseCharacterTypeFacet();

		CString strMsg;

		// コマンドライン引数処理
		Arguments args;
		if(!args.parse(argc, argv)){
			std::cerr << "コマンドライン引数に問題があります" << std::endl;
			return 1;
		}

		// コンソール画面にオプション表示
		std::cout << CT2A(args.GetOption()) << std::endl;

		// ファイルパスチェックでエラーがあれば終了
		if(!args.CheckOptionParameter()){
			return 1;
		}

		// 実行ログオープン
		if(!CLog::GetInstance().SetRunLog(args.strRunLogFilePath.c_str())){
			CLog::GetInstance().PrintLog(true, false, false, false, _T("エラー終了"));
			return 1;
		}

		// エラーログオープン
		if(!CLog::GetInstance().SetErrLog(args.strErrLogFilePath.c_str())){
			CLog::GetInstance().PrintLog(true, false, false, false, _T("エラー終了"));
			return 1;
		}

		// 実行ログにツールバージョン情報出力
		CVersion version;
		CString strFileName = version.GetOriginalFilename();
		CString strFileVersion = version.GetFileVersion();
		CString strProductVersion = version.GetProductVersion();
		strMsg.Format(_T("%s FILEVERSION %s PRODUCTVERSION %s\n"), strFileName, strFileVersion, strProductVersion);
		CLog::GetInstance().PrintLog(false, false, true, false, strMsg);

		// 実行ログにオプション出力
		CLog::GetInstance().PrintLog(false, false, true, false, args.GetOption());

		// エラーログにヘッダ出力
		strMsg.Format(_T("<LAYER>\t<OBJECTID>\t<FIELDNAME>\t<COMMENT>"));
		CLog::GetInstance().PrintLog(false, false, false, true, strMsg);

		// Text対応で拡張するときは、CodeCheckerからTextCodeCheckerを派生させて
		// CheckCharacterCode()をオーバーライドしてShapeとTextで切り替えること
		CodeChecker *objCC;
		if(args.strFileType.c_str() == g_strSHAPE){
			objCC = new ShapeCodeChecker();
		}

		// チェック対象ファイルリスト読み込み
		if(!objCC->SetTargetFileList(args.strFileListPath.c_str())){
			CLog::GetInstance().PrintLog(true, false, false, false, _T("エラー終了"));
			delete objCC;
			return 1;
		}

		// 文字コード範囲指定ファイル読み込み
		if(!objCC->SetCharacterCodeList(args.strCharCodeListPath.c_str())){
			CLog::GetInstance().PrintLog(true, false, false, false, _T("エラー終了"));
			delete objCC;
			return 1;
		}

		// 文字コードチェック実行
		bool isImmediateExit = false;
		if(args.strExitMode.c_str() == g_strIMMEDIATE){
			isImmediateExit = true;
		}
		if(!objCC->CheckCharacterCode(isImmediateExit)){

			// 正常終了（文字コードチェック：NG）
			CLog::GetInstance().PrintLog(true, false, true, false, _T("\n正常終了（文字コードチェック：NG）"));
			delete objCC;
			return 3;
		}

		// 正常終了（文字コードチェック：OK）
		CLog::GetInstance().PrintLog(true, false, true, false, _T("\n正常終了（文字コードチェック：OK）"));
		delete objCC;
		return 0;

	} catch(const std::exception& e) {
		std::cerr << e.what() << std::endl;
		CLog::GetInstance().PrintLog(false, true, true, false, _T("\n異常終了（例外発生）"));
		return 2;
	}
}
