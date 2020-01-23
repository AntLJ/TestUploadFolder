#pragma once
#include "common.h"
#include <TDC/useful_headers/boost_program_options.h>
#include <TDC/useful_headers/tstring.h>


using boost::program_options::option_description;
using boost::program_options::value;

namespace po = boost::program_options;

/**
* @brief オプション管理クラス
*/
class TransParam
{
public:
	/**
	 * @brief コンストラクタ
	 */
	TransParam(void);

	/**
	 * @brief デストラクタ
	 */
	~TransParam(void);

	/**
	 * @brief オプションの格納
	 * @param argc [in] 引数の数
	 * @param argv [in] 引数リスト
	 */
	bool parse(int argc, _TCHAR* argv[]);

	CString getRoadDB() const { return CString(m_roadDB.c_str()); };
	CString getRoadOwner() const { return CString(m_roadOwner.c_str()); };
	CString getRoadWhere() const { return CString(m_roadWhere.c_str()); };
	CString getWarningSignDB() const { return CString(m_warningSignDB.c_str()); };
	CString getWaringSignOwner() const { return CString(m_warningSignOwner.c_str()); };
	double  getBuffer() const { return m_buffer; };
	CString getOutputLog() const { return CString(m_outputLog.c_str()); };
	CString getRunLog() const { return CString(m_runLog.c_str()); };
	CString getErrLog() const { return CString(m_errLog.c_str()); };


	std::string getOptions();

private:
	/**
	 * @brief オプションの格納
	 * @param vm [in] オプション群
	 * @param option [in] オプション名
	 */
	bool optionExistChk(const po::variables_map& vm, LPCSTR option); 

	/**
	 * @brief オプションの格納
	 * @param option [in] オプション名
	 * @param content [in] 引数
	 */
	std::string getOptionLine(LPCSTR option, uh::tstring content); 

private:
	uh::tstring m_roadDB;				// 道路DB
	uh::tstring m_roadOwner;			// 道路DBユーザ
	uh::tstring m_roadWhere;			// 道路リンクの検索条件
	uh::tstring m_warningSignDB;		// 警戒標識DB
	uh::tstring m_warningSignOwner;		// 警戒標識DBユーザ
	double m_buffer;					// 警戒標識ポイントにかけるバッファ長
	uh::tstring m_outputLog;			// 出力ログ
	uh::tstring m_runLog;				// 実行ログ
	uh::tstring m_errLog;				// エラーログ

};

