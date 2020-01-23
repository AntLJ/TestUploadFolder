#include "StdAfx.h"
#include "Arguments.h"
#include "LogSys.h"
#include <TDC/useful_headers/directory_util.h>
#include <WinLib/VersionInfo.h>

// コマンドライン引数の解析
bool Arguments::parse(int argc, _TCHAR* argv[])
{
	po::options_description desc("How to use");

	try{
		// パラメータ読み込み
		desc.add_options()
			(INI, uh::tvalue<uh::tstring>(), "設定ファイル(必須)")
			(PROCLOG, uh::tvalue<uh::tstring>(), "実行ログ(任意)")
			(ERRLOG, uh::tvalue<uh::tstring>(), "エラーログ(任意)");
		if (1 == argc) {
			std::cout << desc << std::endl;
			return WriteUsage();
		}

		po::variables_map vm;
		po::store(po::parse_command_line(argc, argv, desc), vm);
		po::notify(vm);

		// パラメータチェック
		if (! CheckParam(vm)) {
			std::cout << desc << std::endl;
			return WriteUsage();
		}

		// パラメータ読み込み
		SetParam(vm);

		// ログオープン
		if (m_procLog.size() != 0) {
			if (! LogSys::GetInstance().SetProcLog(m_procLog.c_str())) {
				std::cerr << "EEROR\t" << PROCLOG << "オプションが不正です" << std::endl;
				return false;
			}
		}
		if (m_errLog.size() != 0) {
			if (! LogSys::GetInstance().SetErrLog(m_errLog.c_str())) {
				std::cerr << "ERROR\t" << ERRLOG << "オプションが不正です" << std::endl;
				return false;
			}
			LogSys::GetInstance().WriteErrLog(false, false, _T("# FREESTYLELOG\n"));
			LogSys::GetInstance().WriteErrLog(false, false, _T("# フラグ\tレイヤ名\tオブジェクトID\tエラーレベル\tエラーメッセージ\n"));
		}
		// 設定ファイル読み込み
		po::variables_map vmContent;
		po::options_description descContent("How to set a ini file");
		descContent.add_options()
			(INPUTDIR, uh::tvalue<uh::tstring>(), "変換元Shape格納ディレクトリ(Roadディレクトリまでのパス)(必須)")
			(HEIGHTDB, uh::tvalue<uh::tstring>(), "HEIGHT_NODE格納DB(必須)")
			(HEIGHTSCHEMA, uh::tvalue<uh::tstring>(), "HEIGHT_NODE格納スキーマ(任意)")
			(MESHDB, uh::tvalue<uh::tstring>(), "BASEMESH格納DB(必須)")
			(MESHSCHEMA, uh::tvalue<uh::tstring>(), "BASEMESH格納スキーマ(任意)")
			(MESHLIST, uh::tvalue<uh::tstring>(), "変換対象メッシュリスト(必須)")
			(BUFFER, uh::tvalue<double>(), "分割ノードバッファ(任意:初期値=0.0)")
			(OUTPUTDIR, uh::tvalue<uh::tstring>(), "出力ディレクトリ(必須)");
		std::ifstream ifsIniFile(CT2CA(CString(m_iniFile.c_str())));
		if (! ifsIniFile.is_open()) {
			std::cerr << "ERROR\t設定ファイルのオープンに失敗しました" << std::endl;
			std::cout << desc << std::endl;
			return false;
		}
		po::store(po::parse_config_file(ifsIniFile, descContent), vmContent);
		po::notify(vmContent);

		// 設定ファイルチェック
		if (! CheckIni(vmContent)){
			std::cout << desc << std::endl;
			return WriteUsage();
		}
		
		// 設定ファイル読み込み
		SetIni(vmContent);

		// バージョン・オプション情報表示
		ProcInfoWrite(vmContent);
	}
	catch(const std::exception& e){
		std::cerr << e.what() << std::endl;
		std::cerr << desc << std::endl;
		return false;
	}
	return true;
}

bool Arguments::WriteUsage() const
{
	std::cout << "How to set ini_file" << std::endl;
	std::cout << "  " << INPUTDIR << " = <変換元Shape格納ディレクトリ(Roadディレクトリまでのパス)(必須)>" << std::endl;
	std::cout << "  " << HEIGHTDB << " = <HEIGHT_NODE格納DB>(必須)" << std::endl;
	std::cout << "  " << HEIGHTSCHEMA << " = <HEIGHT_NODE格納スキーマ(任意)>" << std::endl;
	std::cout << "  " << MESHDB << " = <BASEMESH格納DB>(必須)" << std::endl;
	std::cout << "  " << MESHSCHEMA << " = <BASEMESH格納スキーマ(任意)>" << std::endl;
	std::cout << "  " << MESHLIST << " = <変換対象メッシュリスト>(必須)" << std::endl;
	std::cout << "  " << BUFFER  << " = <分割ノードバッファ>(任意:初期値=0.0)" << std::endl;
	std::cout << "  " << OUTPUTDIR << " = <出力ディレクトリ>(必須)" << std::endl;
	return false;
}

bool Arguments::CheckParam(const po::variables_map& vm) const
{
	bool ret = true;
	ret &= FileExistChk(vm, INI);
	if (vm.count(PROCLOG) != 0) {
		ret &= FileDirExistChk(vm, PROCLOG);
	}
	if (vm.count(ERRLOG) != 0) {
		ret &= FileDirExistChk(vm, ERRLOG);
	}
	return ret;
}

bool Arguments::OptionExistChk(const po::variables_map& vm, LPCSTR option) const
{
	if (vm.count(option) == 0) {
		std::cerr << "ERROR\t" << option << "が設定されていません" << std::endl;
		return false;
	}
	return true;
}

bool Arguments::FileExistChk(const po::variables_map& vm, LPCSTR option) const
{
	if (! OptionExistChk(vm, option))
		return false;
	if (! uh::isFile(vm[option].as<uh::tstring>().c_str())) {
		std::cerr << "ERROR\t" << option << "で指定したファイルが存在しません" << std::endl;
		return false;
	}
	return true;
}

bool Arguments::DirExistChk(const po::variables_map& vm, LPCSTR option) const
{
	if (! OptionExistChk(vm, option))
		return false;
	if (! uh::isDirectory(vm[option].as<uh::tstring>().c_str())) {
		std::cerr << "ERROR\t" << option << "で指定したディレクトリが存在しません" << std::endl;
		return false;
	}
	return true;
}

bool Arguments::FileDirExistChk(const po::variables_map& vm, LPCSTR option) const
{
	CString filePath = vm[option].as<uh::tstring>().c_str();
	long dirPos = filePath.ReverseFind(_T('\\'));
	CString dirPath = filePath.Left(dirPos);
	if (! uh::isDirectory(dirPath)) {
		std::cerr << "ERROR\t" << option << "で指定したファイルを格納するディレクトリが存在しません" << std::endl;
		return false;
	}
	return true;
}

void Arguments::SetParam(const po::variables_map& vm)
{
	m_iniFile = vm[INI].as<uh::tstring>();
	if (vm.count(PROCLOG) != 0) {
		m_procLog = vm[PROCLOG].as<uh::tstring>();
	}
	if (vm.count(ERRLOG) != 0) {
		m_errLog = vm[ERRLOG].as<uh::tstring>();
	}
}

bool Arguments::CheckIni(const po::variables_map& vm) const
{
	bool ret = true;
	ret &= DirExistChk(vm, INPUTDIR);
	ret &= OptionExistChk(vm, HEIGHTDB);
	ret &= OptionExistChk(vm, MESHDB);
	ret &= DirExistChk(vm, OUTPUTDIR);
	ret &= FileExistChk(vm, MESHLIST);
	return ret;
}

void Arguments::SetIni(const po::variables_map& vm)
{
	m_inputDir = vm[INPUTDIR].as<uh::tstring>();
	m_heightNoodeDB = vm[HEIGHTDB].as<uh::tstring>();
	if (vm.count(HEIGHTSCHEMA) != 0)
		m_heightNodeSchema = vm[HEIGHTSCHEMA].as<uh::tstring>();
	m_baseMeshDB = vm[MESHDB].as<uh::tstring>();
	if (vm.count(MESHSCHEMA) != 0)
		m_baseMeshSchema = vm[MESHSCHEMA].as<uh::tstring>();
	m_meshList = vm[MESHLIST].as<uh::tstring>();
	m_outputDir = vm[OUTPUTDIR].as<uh::tstring>();
	if (vm.count(BUFFER) != 0)
		m_buffer = vm[BUFFER].as<double>();
}

void Arguments::ProcInfoWrite(const po::variables_map& vm) const
{
	// ツール情報の表示
	CVersion toolVersion;
	CString toolInfo;
	toolInfo.Format(_T("%s VERSION:%s\n"), toolVersion.GetOriginalFilename(), toolVersion.GetFileVersion());
	LogSys::GetInstance().WriteProcLog(true, false, toolInfo);
	// 設定内容の表示
	LogSys::GetInstance().WriteProcLog(true, false, _T("option\n"));
	WriteOptionVal(INPUTDIR, m_inputDir.c_str());
	WriteOptionVal(HEIGHTDB, m_heightNoodeDB.c_str());
	if (vm.count(HEIGHTSCHEMA) > 0)
		WriteOptionVal(HEIGHTSCHEMA, m_heightNodeSchema.c_str());
	WriteOptionVal(MESHDB, m_baseMeshDB.c_str());
	if (vm.count(MESHSCHEMA) > 0)
		WriteOptionVal(MESHSCHEMA, m_baseMeshSchema.c_str());
	WriteOptionVal(MESHLIST, m_meshList.c_str());
	if (vm.count(BUFFER) > 0)
		WriteOptionVal(BUFFER, CA2CT(std::to_string(m_buffer).c_str()));
	else
		WriteOptionVal(BUFFER, _T("0.0"));
	WriteOptionVal(OUTPUTDIR, m_outputDir.c_str());
}

void Arguments::WriteOptionVal(LPCSTR option, LPCTSTR val) const
{
	CString opAndVal;
	opAndVal.Format(_T("  %s\t%s\n"), CString(option), val);
	LogSys::GetInstance().WriteProcLog(true, false, opAndVal);
}