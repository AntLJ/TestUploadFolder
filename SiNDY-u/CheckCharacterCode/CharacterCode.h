#include "stdafx.h"

/**
 * @brief 文字コード範囲クラス
 */
class CharacterCode
{
public:
	/**
	 * @brief コンストラクタ
	 */
	CharacterCode() : CodeRangeMin(0), CodeRangeMax(0){}

public:
	unsigned long CodeRangeMin;		//!< 文字コード範囲指定の始まり（コードが小さい方）
	unsigned long CodeRangeMax;		//!< 文字コード範囲指定の終わり（コードが大きい方）
};
