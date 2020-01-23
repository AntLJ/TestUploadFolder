#include "stdafx.h"
#include "TransParam.h"


TransParam::TransParam(void)
{
}


TransParam::~TransParam(void)
{
}

bool TransParam::parse(int argc, _TCHAR* argv[])
{
	po::options_description desc("How to use");
	desc.add_options()
		(ROADDB, uh::tvalue<uh::tstring>(&m_roadDB), "道路DB")
		(ROADOWNER, uh::tvalue<uh::tstring>(&m_roadOwner), "道路テーブルの所有ユーザ(任意)")
		(ROADWHERE, uh::tvalue<uh::tstring>(&m_roadWhere), "対象道路リンクの検索条件")
		(WARINIGSIGNDB, uh::tvalue<uh::tstring>(&m_warningSignDB), "警戒標識DB")
		(WARINGSIGNOWNER, uh::tvalue<uh::tstring>(&m_warningSignOwner), "警戒標識テーブルの所有ユーザ(任意)")
		(BUFFER, uh::tvalue<double>(&m_buffer), "警戒標識に設定するバッファ")
		(OUTPUTLOG, uh::tvalue<uh::tstring>(&m_outputLog), "出力ログ")
		(RUNLOG, uh::tvalue<uh::tstring>(&m_runLog), "実行ログ")
		(ERRLOG, uh::tvalue<uh::tstring>(&m_errLog), "エラーログ");

	po::variables_map vm;
	po::store(po::parse_command_line(argc, argv, desc), vm);
	po::notify(vm);

	// 引数が無い場合は、使い方を出力
	if (1 == argc) {
		std::cout << desc << std::endl;
		return false;
	}

	// 必須オプションの有無チェック
	if (! optionExistChk(vm, ROADDB) || ! optionExistChk(vm, WARINIGSIGNDB) ||
		! optionExistChk(vm, BUFFER) || ! optionExistChk(vm, OUTPUTLOG) ||
		! optionExistChk(vm, RUNLOG) || ! optionExistChk(vm, ERRLOG)) {
			std::cout << desc << std::endl;
			return false;
	}

	return true;

}

std::string TransParam::getOptions()
{
	std::string options = "[option]\n";
	options.append(getOptionLine(ROADDB, m_roadDB));
	options.append(getOptionLine(ROADOWNER, m_roadOwner));
	options.append(getOptionLine(ROADWHERE, m_roadWhere));
	options.append(getOptionLine(WARINIGSIGNDB, m_warningSignDB));
	options.append(getOptionLine(WARINGSIGNOWNER, m_warningSignOwner));
	options.append(getOptionLine(BUFFER, uh::toTStr(std::to_string(m_buffer))));
	options.append(getOptionLine(OUTPUTLOG, m_outputLog));
	options.append(getOptionLine(RUNLOG, m_runLog));
	options.append(getOptionLine(ERRLOG, m_errLog));
	return options;
}

bool TransParam::optionExistChk(const po::variables_map& vm, LPCSTR option)
{
	if (vm.count(option) == 0) {
		std::cerr << "ERROR\t" << option << "オプションが設定されていません" << std::endl;
		return false;
	}
	return true;
}


std::string TransParam::getOptionLine(LPCSTR option, uh::tstring content)
{
	std::string optionLine = "  --";
	optionLine.append(option);
	if (0 != content.length()) {
		optionLine.append("\t");
		optionLine.append(uh::toStr(content));
	}
	optionLine.append("\n");

	return optionLine;
}