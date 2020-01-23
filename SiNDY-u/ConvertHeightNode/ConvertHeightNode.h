#pragma once

#include "Arguments.h"
#include "LogSys.h"


/**
 * @brief   メインクラス
 */
class CConvertHeightNode
{

public:

	/**
	 * @brief   コンストラクタ
	 * @param   args [in]  引数
	 */
	CConvertHeightNode( const Arguments& args ) : m_args( args ){}

	/**
	 * @brief   デストラクタ
	 */
	~CConvertHeightNode(){};

	/**
	 * @brief   実行
	 * @retval  true  成功
	 * @retval  false 失敗
	 */
	bool execute();

private:

	Arguments m_args;      //!< 引数
};

