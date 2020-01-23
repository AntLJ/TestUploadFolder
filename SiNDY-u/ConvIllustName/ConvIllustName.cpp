// ConvIllustName.cpp : コンソール アプリケーションのエントリ ポイントを定義します。

#include "stdafx.h"
#include "LogSys.h"
#include "schema/illust.h"
#include "VersionInfo.h"
#include "ConvIllustName.h"

//オプション設定のパラメータ
struct Param {
	CString illust_settings;	//処理情報設定ファイル
	CString input_list;			//イラスト対応表
	CString output_dir;			//出力ディレクトリ
	CString runlog;				//実行ログ
	CString errlog;				//エラーログ
};

/**
 * @brief 引数のチェック
 * @param argc [in] 引数の数
 * @param argv [in] 引数リスト
 * @param param [out] 取得した引数リスト
 * @retval true  成功
 * @retval false 失敗
 */
bool arg_check(int argc, _TCHAR *argv[], Param &param)
{
	boost::program_options::options_description opt("オプション");

	opt.add_options()
		("help,h", "ヘルプを表示")
		("illust_settings,s", uh::tvalue<uh::tstring>(), "処理情報設定ファイルの指定")
		("input_list,l", uh::tvalue<uh::tstring>(), "イラストファイル対応表の指定")
		("output_dir,o", uh::tvalue<uh::tstring>(), "出力ディレクトリの指定")
		("runlog,r", uh::tvalue<uh::tstring>(), "実行ログファイルの指定")
		("errlog,e", uh::tvalue<uh::tstring>(), "エラーログファイルの指定");

	boost::program_options::variables_map vm;
	boost::program_options::store(boost::program_options::parse_command_line(argc, argv, opt), vm);
	boost::program_options::notify(vm);

	//ヘルプ表示
	if (vm.count("help") ||
		! vm.count("illust_settings") ||
		! vm.count("input_list") ||
		! vm.count("output_dir") ||
		! vm.count("runlog") ||
		! vm.count("errlog")
		) {
		std::cerr << opt << std::endl;
	}

	//エラーログファイル名
	if(vm["errlog"].empty()){
		LogSys::GetInstance().WriteErrLog(true, false, _T("ERROR\tエラーログファイル名が指定されていません\n"));
		return false;
	}

	//エラーログファイルの存在確認
	param.errlog = vm["errlog"].as<uh::tstring>().c_str();
	if (! LogSys::GetInstance().SetErrLog(param.errlog)) {
		CString errlogMessage;
		errlogMessage.Format(_T("ERROR\tエラーログがオープンできません\n\t%s\n"),param.errlog);
		LogSys::GetInstance().WriteErrLog(true, false, errlogMessage);
		return false;
	}

	//実行ログファイル名
	if(vm["runlog"].empty()){
		LogSys::GetInstance().WriteErrLog(true, false, _T("ERROR\t実行ログファイル名が指定されていません\n"));
		return false;
	}

	//実行ログファイルの存在確認
	param.runlog = vm["runlog"].as<uh::tstring>().c_str();
	if (! LogSys::GetInstance().SetProcLog(param.runlog)) {
		CString runlogMessage;
		runlogMessage.Format(_T("ERROR\t実行ログがオープンできません\n\t%s\n"),param.runlog);
		LogSys::GetInstance().WriteErrLog(true, false, runlogMessage);
		return false;
	}
	//基本情報を実行ログに出力
	else{
		CVersion cVer;				//リソースファイル情報取得クラス
		CString fileVersion;
		CString productVersion;
		fileVersion.Format(_T("\tF:%s"),cVer.GetFileVersion());
		productVersion.Format(_T("\tP:%s\n"),cVer.GetProductVersion());
		LogSys::GetInstance().WriteProcLog(false, false,cVer.GetInternalName());	//プログラム名
		LogSys::GetInstance().WriteProcLog(false, false, fileVersion);			//バージョン情報文字列
		LogSys::GetInstance().WriteProcLog(false, false, productVersion);			//バージョン情報文字列
		CString oppMessage;

		//オプション設定を実行ログに出力
		for(auto outputOption : vm){
			CString vmKey = outputOption.first.c_str();
			CString vmValue = outputOption.second.as<uh::tstring>().c_str();
			oppMessage.Format(_T("\t%s\t\t\t%s\n"),vmKey,vmValue);
			LogSys::GetInstance().WriteProcLog(false, false, oppMessage);
		}
	}

	//処理情報設定ファイル名
	if(vm["illust_settings"].empty()){
		LogSys::GetInstance().WriteErrLog(true, false, _T("ERROR\t処理情報設定ファイル名が指定されていません\n"));
		return false;
	}
	param.illust_settings = vm["illust_settings"].as<uh::tstring>().c_str();

	//処理情報設定ファイルの存在確認
	if (!PathFileExists(param.illust_settings)){
		CString illustSettingsMessage;
		illustSettingsMessage.Format(_T("ERROR\t処理情報設定ファイルがオープンできません\n\t%s\n"),param.illust_settings);
		LogSys::GetInstance().WriteErrLog(true, false, illustSettingsMessage);
		return false;
	}

	//イラストファイル対応表名
	if(vm["input_list"].empty()){
		LogSys::GetInstance().WriteErrLog(true, false, _T("ERROR\tイラストファイル対応表名が指定されていません\n"));
		return false;
	}
	//イラストファイル対応表の存在確認
	param.input_list = vm["input_list"].as<uh::tstring>().c_str();
	if (!PathFileExists(param.input_list)){
		CString inputListMessage;
		inputListMessage.Format(_T("ERROR\t%s:\tイラストファイル対応表がオープンできません\n"),param.input_list);
		LogSys::GetInstance().WriteErrLog(true, false, inputListMessage);
		return false;
	}

	//出力ディレクトリ名
	if(vm["output_dir"].empty()){
		LogSys::GetInstance().WriteErrLog(true, false, _T("ERROR\t出力ディレクトリ名が指定されていません\n"));
		return false;
	}
	//出力ディレクトリの存在確認
	param.output_dir = vm["output_dir"].as<uh::tstring>().c_str();
	if (! PathIsDirectory(param.output_dir)){
		CString errorMessage;
		errorMessage.Format(_T("ERROR\t%s : \t出力ディレクトリがオープンできません\n"),param.output_dir);
		LogSys::GetInstance().WriteErrLog(true, false, errorMessage);
		return false;
	}
	//出力ディレクトリ内に「illust」ディレクトリ作成
	param.output_dir += _T("\\illust");
	if(PathIsDirectory(param.output_dir)){
		CString IsDirError;
		IsDirError.Format(_T("ERROR\t%sは既に存在しています。\n"),param.output_dir);
		LogSys::GetInstance().WriteErrLog(true, false,IsDirError);
		return false;
	} else {
		CreateDirectory(param.output_dir,NULL);
	}
	return true;
}

//情報処理設定ファイルの要素
struct IllustSettingsFields{
	CString prefix;				//イラストファイルのプレフィックス
	CString extension;			//イラストファイルの拡張子
	CString namingConvention;	//イラストファイルの命名規則
	CString storeDir;			//イラストファイルの格納先ディレクトリ
};

//イラスト対応表の要素
struct IllustNameFields{
	std::pair<int,int> illustNameRecordKey; //イラスト種別と仕向け先をkeyにする。
	CString OID;				//イラストリンクOID
	CString formerFileName;		//変換前ファイル名
};

/**
 * @brief 処理情報取得ファイルをカラム(トークン)に分解
 * @param param [in] 取得した引数リスト
 * @param illustSettingsMap [out] 処理情報設定ファイルの情報を格納する用のマップ
 * @retval true  成功
 * @retval false 失敗
 */
bool canSetIllustSettings(const Param &param,std::map<std::pair<int,int>,IllustSettingsFields>& illustSettingsMap)
{
	// 処理情報設定ファイルを取得
	CString illustSettings;
	illustSettings = param.illust_settings;
	std::ifstream reading_file(illustSettings);
	if (reading_file.fail()){
		CString illustSettingsMessage;
		illustSettingsMessage.Format(_T("ERROR\t処理情報設定ファイルがオープンできません\n\t%s\n"),param.illust_settings);
		LogSys::GetInstance().WriteErrLog(true, false, illustSettingsMessage);
		return false;
	}

	//行を読みこみ
	std::string textRecord = ""; //テキストのレコード格納用
	while (getline(reading_file,textRecord)){
		CString csTextRecord = textRecord.c_str();

		//空行や先頭文字が♯だった場合は処理をスキップ
		if(csTextRecord.IsEmpty()){continue;}
		if(csTextRecord.Left(1) == _T("#")){continue;}

		CStringTokenizer tkz(csTextRecord, _T("\t"), TOKEN_RET_EMPTY_ALL);
		//処理情報設定ファイルのフォーマットが正常か確認
		if(tkz.CountTokens()!=7){				//tab数が7以外なら異常なフォーマット
			CString unusualSetFileFormat;
			unusualSetFileFormat.Format(_T("ERROR\tファイルのフォーマットが異常です\n\t%s\n"),illustSettings);
			LogSys::GetInstance().WriteErrLog(true, false, unusualSetFileFormat);
			return false;
		}

		IllustSettingsFields illustSettingsField;  //イラスト対応表のフィールド値格納用
		std::pair<int,int>illustSettingsRecordkey; //イラスト種別コードと仕向け先コードをkeyにする。

		CString tempStoreIllKindValue;
		CString tempStoreDestinationValue;
		tempStoreIllKindValue = tkz.GetNextToken();
		//イラスト種別コードが整数値か確認
		if(!canCheckFigure(tempStoreIllKindValue)){
			CString unusualIllSetValue;
			unusualIllSetValue.Format(_T("ERROR\t整数値ではないイラスト種別コードがセットされています\n\t%s\n"),illustSettings);
			LogSys::GetInstance().WriteErrLog(true, false, unusualIllSetValue);
			return false;
		}

		//仕向け先コード値が整数値か確認
		tempStoreDestinationValue = tkz.GetNextToken();
		if(!canCheckFigure(tempStoreDestinationValue)){
			CString unusualIllNameValue;
			unusualIllNameValue.Format(_T("ERROR\t整数値ではない仕向け先コードがセットされています\n\t%s\n"),illustSettings);
			LogSys::GetInstance().WriteErrLog(true, false, unusualIllNameValue);
			return false;
		}

		//レコードから各フィールド値を取得
		illustSettingsRecordkey.first = _ttoi(tempStoreIllKindValue);		//イラスト種別コード
		illustSettingsRecordkey.second =_ttoi(tempStoreDestinationValue);	//仕向け先コード
		illustSettingsField.prefix = tkz.GetNextToken();					//イラストファイルのプレフィックス
		illustSettingsField.extension = tkz.GetNextToken();					//イラストファイルの拡張子
		illustSettingsField.namingConvention = tkz.GetNextToken();			//イラストファイルの命名規則
		illustSettingsField.storeDir = tkz.GetNextToken();					//イラストファイル格納ディレクトリ
		tkz.GetNextToken();													//出力除外条件

		//イラスト種別コード値が正常か確認
		if(!(canCheckIllustkind(illustSettingsRecordkey.first,illustSettings))){return false;}

		//仕向け先コード値が正常か確認
		if(!(canCheckDestination(illustSettingsRecordkey.second,illustSettings))){return false;}

		//イラストファイル格納ディレクトリがオープンできるか確認
		if (! PathIsDirectory(illustSettingsField.storeDir)){
			CString 	cantOpenIllDirMessage;
			cantOpenIllDirMessage.Format(_T("ERROR\tイラストファイル格納ディレクトリがオープンできません\n\t%s\n"),illustSettingsField.storeDir);
			LogSys::GetInstance().WriteErrLog(true, false, cantOpenIllDirMessage);
			return false;
		}

		// 挿入に失敗（キーの重複）
		if(!illustSettingsMap.insert(std::make_pair(illustSettingsRecordkey,illustSettingsField)).second){
			CString duplicationKeyErr;
			duplicationKeyErr.Format(_T("ERROR\t処理情報設定ファイルのキーが重複しています:\n\tイラスト種別コード:%d,仕向け先コード:%d\n"),illustSettingsRecordkey.first,illustSettingsRecordkey.second);
			LogSys::GetInstance().WriteErrLog(true, false, duplicationKeyErr);
			return false;
		}
	}
return true;
}

/**
 * @brief 対応表のレコード抽出＆タブ区切りの要素抽出
 * @param param [in] 取得した引数リスト
 * @param illustNameMap [out] イラスト対応表の情報を格納する用のマップ
 * @retval true  成功
 * @retval false 失敗
 */
bool canSetIllustName(const Param &param,std::map<CString,IllustNameFields>& illustNameMap)
{
	//イラスト対応表を取得
	CString illustName;
	illustName = param.input_list;
	std::ifstream reading_file(illustName);
	if (reading_file.fail()){
		CString inputListMessage;
		inputListMessage.Format(_T("ERROR\t%s:\tイラストファイル対応表がオープンできません\n"),param.input_list);
		LogSys::GetInstance().WriteErrLog(true, false, inputListMessage);
		return false;
	}

	//テキストをレコード区切りにするためのループ
	std::string textRecord = ""; //テキストのレコード格納用
	while (getline(reading_file,textRecord)){
		CString csTextRecord = textRecord.c_str();

		//空行や先頭文字が♯だった場合は処理をスキップ
		if(csTextRecord.IsEmpty()){continue;}
		if(csTextRecord.Left(1) == _T("#")){continue;}

		IllustNameFields illustNameField;
		CString latterFileName;
		CStringTokenizer tkz(csTextRecord, _T("\t"), TOKEN_RET_EMPTY_ALL);

		//イラスト対応表のフォーマットが正常か確認
		if(tkz.CountTokens()!=5){					//tab数が5以外なら異常なフォーマット
			CString unusualNameFileFormat;
			unusualNameFileFormat.Format(_T("ERROR\tファイルのフォーマットが異常です\n\t%s\n"),illustName);
			LogSys::GetInstance().WriteErrLog(true, false, unusualNameFileFormat);
			return false;
		}


		CString tempStoreIllKindValue;			//イラスト種別コード値を一時的に格納するための変数
		CString tempStoreValueDestination;		//仕向け先コード値を一時的に格納するための変数

		//イラスト種別コードが整数値か確認
		tempStoreIllKindValue = tkz.GetNextToken();
		if(!canCheckFigure(tempStoreIllKindValue)){
			CString unusualIllSetValue;
			unusualIllSetValue.Format(_T("ERROR\t整数値ではないイラスト種別コードがセットされています\n\t%s\n"),illustName);
			LogSys::GetInstance().WriteErrLog(true, false, unusualIllSetValue);
			return false;
		}

		//仕向け先コード値が整数値か確認
		tempStoreValueDestination = tkz.GetNextToken();
		if(!canCheckFigure(tempStoreValueDestination)){
			CString unusualIllNameValue;
			unusualIllNameValue.Format(_T("ERROR\t整数値ではない仕向け先コードがセットされています\n\t%s\n"),illustName);
			LogSys::GetInstance().WriteErrLog(true, false, unusualIllNameValue);
			return false;
		}

		//レコードから各フィールド値を取得
		illustNameField.illustNameRecordKey.first = _ttoi(tempStoreIllKindValue);		//イラスト種別コード
		illustNameField.illustNameRecordKey.second =_ttoi(tempStoreValueDestination);	//仕向け先コード
		illustNameField.OID = tkz.GetNextToken();										//オブジェクトID
		illustNameField.formerFileName = tkz.GetNextToken();							//整備で使用しているイラストファイル名
		latterFileName= tkz.GetNextToken();												//リリースデータで使用するイラストファイル名

		//イラスト種別コード値が正常か確認
		if(!(canCheckIllustkind(illustNameField.illustNameRecordKey.first,illustName))){return false;}

		//仕向け先コード値が正常か確認
		if(!(canCheckDestination(illustNameField.illustNameRecordKey.second,illustName))){return false;}

		auto it = illustNameMap.find(latterFileName);
		//既存のkeyと一致する場合は格納しない
		if(it == illustNameMap.end()){
			//mapにtab区切りにした情報を格納
			illustNameMap.insert(std::make_pair(latterFileName,illustNameField));
		}

		/**
		 * 「変換後ファイル名」が重複した二種のレコードが任意の重複かチェック
		 * 下記条件に一つでも当てはまる場合は処理を終了
		 * 	・イラスト種別コードが異なる
		 * 	・仕向け先コードが異なる
		 * 	・変換前ファイル名が異なる
		 * 	・オブジェクトIDが同一
		 */
		else if(	it -> second.illustNameRecordKey.first != illustNameField.illustNameRecordKey.first ||
			it -> second.illustNameRecordKey.second != illustNameField.illustNameRecordKey.second ||
			it -> second.formerFileName != illustNameField.formerFileName ||
			it -> second.OID == illustNameField.OID
		){
			CString unusualLatterFileNameErr;
			unusualLatterFileNameErr.Format(_T("ERROR\tレコードの値が異常です\n\tOID:%s\n"),illustNameField.OID);
			LogSys::GetInstance().WriteErrLog(true, false, unusualLatterFileNameErr);
			return false;
		}
	}
	return true;
}

/**
 * @brief 値が整数値かチェック
 * @      (仕向け先コードは0が正常値である。もし数値ではなく文字が入力されていた場合、
 * @        0と認識され処理が進むことを防ぐ目的で作成した)
 * @param CSTypeFigure [in] CString型の引数
 * @retval true  引数が整数値
 * @retval false 引数が整数値でない
 */
bool canCheckFigure(CString CSTypeFigure){
	int tempINTValue = _ttoi(CSTypeFigure);
	CString tempCSValue;
	tempCSValue.Format(_T("%d"),tempINTValue);
	if(tempCSValue != CSTypeFigure){
		return false;
	}
	return true;
}

/**
 * @brief イラスト種別コード値が正常かチェック
 * @param illustKind [in] イラスト種別コード値
 * @param targetFilePass [in] チェック対象のファイル名
 * @retval true  イラスト種別コード値が正常値
 * @retval false イラスト種別コード値が異常値
 */
bool canCheckIllustkind(int illustKind,CString &targetFilePass)
{
	using namespace sindy::schema::illust_link::illust_class;
	//イラスト種別コード値が正常か確認
	if (illustKind != kSAPA &&
		illustKind != kETC &&
		illustKind != kETC2 &&
		illustKind != kProperJCT &&
		illustKind != kBranch &&
		illustKind != kBranch2 &&
		illustKind != kEntrance &&
		illustKind != kExit &&
		illustKind != kInIC &&
		illustKind != kAirport &&
		illustKind != kRealCross &&
		illustKind != kGrade
	) {
		CString unusualIllustKind;
		unusualIllustKind.Format(_T("ERROR\t下記ファイルのイラスト種別コード値が異常です:\n\t%s(イラスト種別コード : %d)\n"),targetFilePass,illustKind);
		LogSys::GetInstance().WriteErrLog(true, false, unusualIllustKind);
		return false;
	}
	return true;
}

/**
 * @brief 仕向け先コード値が正常かチェック
 * @param illustKind [in] 仕向け先コード値
 * @param targetFilePass [in] チェック対象のファイル名
 * @retval true  イラスト種別コード値が正常値
 * @retval false イラスト種別コード値が異常値
 */
bool canCheckDestination(int destination,CString &targetFilePass)
{
	using namespace sindy::schema::illust_link::customer;
	//仕向け先コード値が正常か確認
	if (destination != kUnset &&
		destination != kCommon &&
		destination != kTF &&
		destination != kExternalSale &&
		destination != kTR
	) {
		CString unusualDestination;
		unusualDestination.Format(_T("ERROR\t下記ファイルの仕向け先コード値が異常です:\n\t%s(仕向け先コード : %d)\n"),targetFilePass,destination);
		LogSys::GetInstance().WriteErrLog(true, false, unusualDestination);
		return false;
	}
	return true;
}

/**
 * @brief イラストファイルの変換
 * @param param [in] 取得した引数リスト
 * @param illustNameMap [in] イラスト対応表の情報を格納したマップ
 * @param illustSettingsMap [in] 処理情報設定ファイルのを格納したマップ
 * @retval true  成功
 * @retval false 失敗
 */
bool canConvIllusName(const Param &param,std::map<CString,IllustNameFields>& illustNameMap,std::map<std::pair<int,int>,IllustSettingsFields>& illustSettingsMap)
{
	std::map<CString,IllustNameFields>::iterator itr = illustNameMap.begin();	//イラスト対応表のレコードを回すためのイテレータ
	//イラスト対応表のレコードを格納したmapを回す
	while(itr != illustNameMap.end())
	{
		//イラスト種別コードと同名のディレクトリを作成
		CString outputDir;
		outputDir = param.output_dir;
		CString createOutputDir;
		createOutputDir.Format(_T("%s\\%d"),param.output_dir,itr->second.illustNameRecordKey.first);
		if(! PathIsDirectory(createOutputDir)){
			CreateDirectory(createOutputDir,NULL);
		}

		//処理情報設定ファイルのキーが格納されているか確認
		if(illustSettingsMap.find(itr->second.illustNameRecordKey) == illustSettingsMap.end()){
			CString notExistIllSetFieldError;
			notExistIllSetFieldError.Format(_T("ERROR\tイラスト種別コード値及び仕向け先コード値の取得に失敗しています\n\tイラスト種別コード：%s,仕向け先コード：%s\n"),itr->second.illustNameRecordKey);
			LogSys::GetInstance().WriteErrLog(true, false,notExistIllSetFieldError);
			return false;
		}

		//パスを生成(変換前イラストファイル,変換後イラストファイル)
		CString formerIllustFilePass;
		CString latterIllustFilePass;
		formerIllustFilePass.Format(_T("%s\\%s.%s"),illustSettingsMap[itr->second.illustNameRecordKey].storeDir,itr->second.formerFileName,illustSettingsMap[itr->second.illustNameRecordKey].extension);		//変換元ファイルのフルパス
		latterIllustFilePass.Format(_T("%s\\%d\\%s.%s"),param.output_dir,itr->second.illustNameRecordKey.first,itr->first,illustSettingsMap[itr->second.illustNameRecordKey].extension);						//変換先ファイルのフルパス

		//イラスト格納ディレクトリの指定が正常か確認
		if (!PathFileExists(formerIllustFilePass)){
			CString notExistIllFile;
			notExistIllFile.Format(_T("ERROR\tイラストファイルをオープンできません\n\t%s\n"),formerIllustFilePass);
			LogSys::GetInstance().WriteErrLog(true, false,notExistIllFile);
			return false;
		}

		//イラストファイル変換処理
		CString copyIllMessage;
		if(CopyFile(formerIllustFilePass,latterIllustFilePass,TRUE)){
			copyIllMessage.Format(_T("\t%s\t -> \t%s\n"),formerIllustFilePass,latterIllustFilePass);
			LogSys::GetInstance().WriteProcLog(false, false, copyIllMessage);
		}
		else{
			copyIllMessage.Format(_T("ERROR\tイラストファイルの変換に失敗しました。\n\t%s\t -> \t%s\n"),formerIllustFilePass,latterIllustFilePass);
			LogSys::GetInstance().WriteErrLog(true, false,copyIllMessage);
			return false;
		}
		++itr;
	}
	return true;
}

/**
 * @brief 全体処理
 * @param param [in] 取得した引数リスト
 * @retval true  成功
 * @retval false 失敗
 */
bool isOutput(const Param &param)
{
	std::map<std::pair<int,int>,IllustSettingsFields> illustSettingsMap;	//処理情報設定ファイルのレコードを格納するためのmap
	std::map<CString,IllustNameFields> illustNameMap;						//イラスト対応表のレコードを格納するためのmap
	//情報処理設定ファイルの取得処理
	if(!canSetIllustSettings(param,illustSettingsMap)){return false;}
	//イラスト対応表の取得処理
	if(!canSetIllustName(param,illustNameMap)){return false;}
	//イラストファイル名変換処理
	if(!canConvIllusName(param,illustNameMap,illustSettingsMap)){return false;}
	return true;
}

int _tmain(int argc, _TCHAR* argv[])
{
	int retval = 0;
	try {
		Param param;
		if (! arg_check(argc, argv, param)) {
			LogSys::GetInstance().WriteProcLog(true, true, _T("異常終了\n"));
			retval = 1;
		} else{
			LogSys::GetInstance().WriteProcLog(true, true, _T("処理開始\n"));
			if(! isOutput(param)) {
				LogSys::GetInstance().WriteProcLog(true, true, _T("異常終了\n"));
				retval = 1;
			} else {
				LogSys::GetInstance().WriteProcLog(true, true, _T("正常終了\n"));
				retval = 0;
			}
		}
	} catch (...) {
		LogSys::GetInstance().WriteProcLog(true, true, _T("異常終了（例外発生）\n"));
		retval = 2;
	}
	return retval;
}