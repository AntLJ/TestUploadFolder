#pragma once

#include "stdafx.h"
#include <boost/program_options.hpp>
#include "Arguments.h"

// エラーコード
namespace err_code {
	enum ECode {
		errOverBuffer                    = 1001,     //!< 比較先のオブジェクトが比較元の検索範囲をはみ出したため、スキップ
		errSrcNotExist                   = 1002,     //!< 重なっている比較元が存在しないため、スキップ
		errOverlaySrcNotExist            = 1003,     //!< 重畳率の閾値を満たす比較元が存在しないため、スキップ
		errScaleSrcNotExist              = 1004,     //!< 拡縮率の閾値を満たす比較元が存在しないため、スキップ
		errBothSrcNotExist               = 1005,     //!< 重畳率と拡縮率を両方とも満たす比較元が存在しないため、スキップ
		errMulitSrcSkipMode              = 1006,     //!< 最も重畳率が高いフィーチャが2つ以上あり、スキップモードのため、スキップ
		errMulitSrcAttrNoMatch           = 1007,     //!< 最も重畳率が高いフィーチャが2つ以上あり、一致判定モードで比較元同士の属性が一致していないため、スキップ
		errAttrNotInAttrList             = 1008,     //!< フィーチャーの属性値が変換表に存在していないため、スキップ
		errNotSameField                  = 1009,     //!< 比較先と比較元のフィールドの型が一致していないため、スキップ
		errPutValueFailed                = 1010,     //!< 比較先のフィールド値への書き込みが失敗したため、スキップ
		errSuccessUpdate                 = 1011,     //!< 正常更新
		errUpdateFailed                  = 1012,     //!< 属性の更新に失敗
		errFailureOverlay                = 1013,     //!< 重畳率が閾値を満たさないため、スキップ
		errFailureScale                  = 1014,     //!< 拡縮率が閾値を満たさないため、スキップ
		errNotSameSpRef                  = 1015,     //!< 比較先と比較元の測地系が一致していない
		errNone                          = 0         //!< なし
	};
}; // err_code

// エラーレベル
namespace err_level {
	enum ECode {
		levelError                      = 1001,     //!< ERROR LEVEL
		levelWarning                    = 1002,     //!< WARNING LEVEL
		levelInfo                       = 1003     //!< INFO LEVEL
	};
}; // err_code

// エラー出力情報
struct ErrItem
{
	long          nFlag;                  //!< 対応状況：デフォルト0
	CString       strLayer;               //!< 比較先のレイヤ名
	long          nOID;                   //!< 比較先のOID
	double        dLon;                   //!< 経度（空文字列）
	double        dLat;                   //!< 緯度（空文字列）
	CString       strErrLevel;            //!< エラーレベル
	long          nErrCode;               //!< エラーコード
	CString       strErrMsg;              //!< エラーメッセージ
	unsigned long nMeshCode;              //!< メッシュコード
	long          nSrcOID;                //!< 重なった比較元のOID（存在しない場合は-1、複数存在の場合はカンマ区切りで出力）
	CString       strNote;                //!< 備考

	// コンストラクタ
	ErrItem() :
		nFlag(0),
		nOID(-1),
		dLon(0.0),
		dLat(0.0),
		nErrCode(0),
		nMeshCode(0),
		nSrcOID(-1)
	{}

	virtual ~ErrItem()
	{
	}
};

/**
 * @brief ログ書き込みのための基底クラス
 */
class LogWriter
{
public:
	/**
	 * @brief  コンストラクタ
	 */
	LogWriter();

	/**
	 * @brief  デストラクタ
	 */
	virtual ~LogWriter(void);

public:
	/**
	 * @brief  ログファイルストリームを確保する
	 * @param  runLogFileName [in] 実行ログファイル名
	 * @param  errLogFileName [in] エラーログファイル名
	 * @param  variable_map [in] プログラムオプション格納マップ
	 * @retval true ストリーム確保成功
	 * @retval false ストリーム確保失敗
	 */
	bool openLogFiles(const CString& runLogFileName, const CString& errLogFileName, const Arguments& variable_map);

protected:
	/**
	 * @brief  実行ログファイルにログを書き込む
	 * @note   標準出力する必要のないログの場合には、第2引数をfalseにすること
	 * @param  message  [in] 書き込むメッセージ
	 * @param  putTime  [in] メッセージに時間を追記するか(デフォルト:true)
	 * @param  disp   [in] 標準出力をするかどうか（デフォルト:true）
	 */
	void writeRunLog(LPCTSTR message, bool putTime = true, bool disp = true);

	/**
	 * @brief  エラーログファイルにログを書き込む
	 * @note   標準エラー出力する必要のないログの場合には、第2引数をfalseにすること
	 * @param  message  [in] 書き込むメッセージ
	 * @param  disp   [in] 標準出力をするかどうか（デフォルト:true）
	 */
	void writeErrLog(LPCTSTR message, bool disp = true);

	/**
	 * @brief  エラーログファイルにログを書き込む
	 * @note   標準エラー出力する必要のないログの場合には、第2引数をfalseにすること
	 * @param  cErrItem  [in] 書き込むメッセージの構造体
	 * @param  disp   [in] 標準出力をするかどうか（デフォルト:true）
	 */
	void writeErrLog(ErrItem cErrItem, bool disp = true);

	/**
	 * @brief  ログファイルをクローズする
	 * @note 実行ログ/エラーログの双方をクローズする
	 */
	void closeLogFiles();

	/**
	 * @brief  ログファイルにプログラム開始としてツール情報、オプション引数を出力する
	 * @param  args [in] オプション引数
	 */
	void startLogFile(const Arguments& args );

	/**
	 * @brief  実行ログファイルにプログラム終了時刻と処理結果を出力する
	 */
	void endLogFile();

	/**
	 * @brief  実行ログファイルにツール名とバージョンを出力する
	 */
	void writeFileVersion();

	/**
	 * @brief  現在時刻を取得する
	 * @return 現在時刻を示す文字列
	 */
	uh::tstring getNowTime() const;

	/**
	 * @brief エラーメッセージ文字列取得
	 * @param errCode [in] エラーコード
	 * @return エラーコードに対応するエラーメッセージ文字列
	 */
	static CString getErrCodeMsg(err_code::ECode errCode);

	/**
	 * @brief エラーレベル文字列取得
	 * @param errLevel [in] レベルコード
	 * @return レベルコードに対応するエラーレベル文字列
	 */
	static CString getErrLevelMsg(err_level::ECode errLevel);

private:
	time_t m_startTime;              //!< 変換スタート時間

	std::wofstream m_runLogStream;   //!< 実行ログ出力用ファイルストリーム
	std::wofstream m_errLogStream;   //!< エラーログ出力用ファイルストリーム
};
