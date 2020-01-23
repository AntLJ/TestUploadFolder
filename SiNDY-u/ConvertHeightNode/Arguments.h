#pragma once

#include <TDC/useful_headers/boost_program_options.h>
#include "common.h"


namespace po = boost::program_options;

/**
 * @brief コマンドライン引数クラス
 */
class Arguments
{
public:
	/**
	 * @brief コンストラクタ
	 */
	Arguments()
		:m_buffer(0.0)
	{}

	/**
	 * @brief	コマンドライン引数の解析
	 * @param	argc [in]	コマンドライン引数の数
	 * @param	argv [in]	コマンドライン引数の内容
	 * @retval	true	成功
	 * @retval	false	失敗
	 */
	bool parse(int argc, TCHAR* argv[]);

	/**
	 *	@brief	設定ファイルヘルプ出力
	 *	@note	各種設定に誤りがあった場合に仕様する関数のため、戻り値は必ずfalse
	 *	@retval false
	 */
	bool WriteUsage() const;

	/**
	 *	@brief	引数チェック
	 *	@param	vm [in] 取得したコマンドライン引数
	 *	@retval	true 正常
	 *	@retval false 不正
	 */
	bool CheckParam(const po::variables_map& vm) const;

	/**
	 *	@brief	オプション存在チェック
	 *	@param	vm [in]		オプション群
	 *	@param	option [in]  オプション名
	 *	@retval true 存在する
	 *	@retval false 存在しない
	 */
	bool OptionExistChk(const po::variables_map& vm, LPCSTR option) const;

	/**
	 *	@brief	ファイル存在チェック
	 *	@param	vm [in]		オプション群
	 *	@param	option [in]	オプション名
	 *	@retval true ファイルが存在する。
	 *	@retval false ファイルが存在しない。
	 */
	bool FileExistChk(const po::variables_map& vm, LPCSTR option) const;

	/**
	 *	@brief	ディレクトリ存在チェック
	 *	@param	vm [in]		オプション群
	 *	@param	option [in] オプション名
	 *	@retval true ディレクトリが存在する。
	 *	@retval false ディレクトリが存在しない。
	 */
	bool DirExistChk(const po::variables_map& vm, LPCSTR option) const;

	/**
	 *	@brief	出力ファイル格納ディレクトリ存在チェック
	 *	@param	vm [in]		オプション群
	 *	@param	option [in] オプション名
	 *	@retval true ディレクトリが存在する.
	 *	@retval false ディレクトリが存在しない。
	 */
	bool FileDirExistChk(const po::variables_map& vm, LPCSTR option) const;

	/**
	 *	@brief	パラメータセット
	 *	@param	vm [in]
	 */
	void SetParam(const po::variables_map& vm);

	/**
	 *	@brief	設定ファイルチェック
	 *	@param	vm [in] 取得したコマンドライン引数
	 *	@retval	true 正常
	 *	@retval false 不正
	 */
	bool CheckIni(const po::variables_map& vm) const;

	/**
	 *	@brief	設定ファイルセット
	 *	@param	vm [in]
	 */
	void SetIni(const po::variables_map& vm);

	/**
	 *	@brief ツールバージョン・オプションの表示
	 */
	void ProcInfoWrite(const po::variables_map& vm) const;

	/**
	 *	@brief	オプションの表示
	 *	@param	option [in] オプション名
	 *	@param	val [in] 設定内容
	 */
	void WriteOptionVal(LPCSTR option, LPCTSTR val) const;

public:
	// 引数
	uh::tstring m_iniFile;
	uh::tstring m_procLog;			//!< 実行ログ
	uh::tstring m_errLog;			//!< エラーログ

	// 設定ファイル
	uh::tstring m_inputDir;			//!< 変換元Shape格納ディレクトリ
	uh::tstring m_heightNoodeDB;	//!< 相対高さノード格納DB
	uh::tstring m_heightNodeSchema;	//!< 相対高さ格納スキーマ
	uh::tstring m_baseMeshDB;		//!< BASEMESH格納DB
	uh::tstring m_baseMeshSchema;	//!< BASEMESH格納スキーマ
	uh::tstring m_meshList;			//!< メッシュリスト
	uh::tstring m_outputDir;		//!< 出力ディレクトリ
	double m_buffer;				//!< 道路・歩行者リンク、相対高さノード空間検索用バッファ

};
