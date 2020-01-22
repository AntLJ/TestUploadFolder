#pragma once

#include "codechecker.h"
#include <libshape.h>
#include <DBFHandle.h>

/**
 * @brief 文字コードチェックShape用クラス
 */
class ShapeCodeChecker : public CodeChecker
{
public:
	/**
	 * @brief コンストラクタ
	 */
	ShapeCodeChecker(void){};

	/**
	 * @brief デストラクタ
	 */
	~ShapeCodeChecker(void){};

	/**
	 * @brief 文字コードチェック対象ファイルを読み込んで文字コードチェックを行う関数
	 * @param isImmediateExit [in] 不正検出時即時終了フラグ
	 * @retval true 処理成功
	 * @retval false 処理失敗
	 */
	bool CheckCharacterCode(bool isImmediateExit);
};
