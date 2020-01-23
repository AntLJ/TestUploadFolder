/*
 * Copyright (C) INCREMENT P CORP. All Rights Reserved.
 *
 * THIS SOFTWARE IS PROVIDED BY INCREMENT P CORP., WITHOUT WARRANTY OF
 * ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE
 * WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE
 * AND NONINFRINGEMENT.
 *
 * IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDER BE LIABLE FOR ANY
 * CLAIM, DAMAGES SUFFERED BY LICENSEE AS A RESULT OF USING, MODIFYING
 * OR DISTRIBUTING THIS SOFTWARE OR ITS DERIVATIVES.
*/

#pragma once
#include <Workspace.h>
#include "ConvertingDefinitionManager.h"
#include "Processor.h"
#include "DBReplicater.h"
#include "Editor.h"

namespace sindy {
	class CTableContainer;
	typedef boost::shared_ptr<CTableContainer> CSPTableContainer;
}
/// フィーチャの二重処理防止クラス
class RowMutex
{
public:
	RowMutex() : m_bEnable(false){}
	/**
	* @brief 動作させるかどうか
	* @param enabled [in] 動作させる場合はtrue
	*/
	void enable( bool enabled ){ m_bEnable = enabled; }
	/**
	* @brief 対象フィーチャを登録する
	* @param table [in] テーブル名
	* @param oid [in] OBJECTID
	* @param isSrc [in] ソース側のIDの場合にはtrue
	* retval true 登録できた（ので処理してOK）
	* retval false 登録できなかった（既に処理済みなので処理不可）
	*/
	bool regist( const uh::tstring& table, long oid, bool isSrc ) const;
private:
	bool m_bEnable; //!< 動作させるかどうか（デフォルト：false）
};

// first:テーブル名、SRCのフィールド名のペア、second:(first:SRC側フィールドの値、second:DST側レコード)
// DST側をフィールドの値のみとしていないのは、実体化前ではOBJECTIDを取得できないため
typedef std::map<std::pair<uh::tstring, uh::tstring>, std::map<CComVariant, sindy::CSPRowBase>> RelationValues;

/**
 * @brief 複数PGFGDBコンバートクラス
 */
template <typename T>
class PGFGDBConvertProcessor : public Processor<T>
{
public:

	PGFGDBConvertProcessor(const uh::tstring& in, const uh::tstring& out, const ConvertingDefinitionManager& manager, const Arguments& args) : 
		Processor<T>(), 
		m_strInput(in), 
		m_manager(manager),
		m_args(args)
	{
		m_input.Workspace().m_bTrace = false;
		m_output.Workspace().m_bTrace = false;
		if( !isDirectory(in) )
		{
			m_input.Workspace().Connect(in.c_str());
			// 1対1のコンバート処理の場合はメッシュ境界フィーチャの処理が
			// 重複しないようにする
			m_limitter.enable(true);
		}
		m_output.Workspace().Connect(out.c_str());
	}

	virtual ~PGFGDBConvertProcessor(){}

	/**
	 * @brief  コンバート処理実行
	 * @param  src  [in]  メッシュコード
	 * @retval true:処理成功
	 */
	bool proc(const uh::tstring& src) override;

	/// ループ処理前処理実行
	bool preprocess() override;

	/// ループ処理後処理実行
	bool postprocess() override;

	/**
	 * @brief  一括コンバート処理実行
	 * @param  srcs  [in] メッシュコード
	 * @retval true:処理成功
	 */
	bool bulkproc(const std::vector<uh::tstring>& srcs) override;

private:
	/**
	* @brief フォルダ指定の場合のメッシュ単位のDBパスを取得する
	* @param mesh [in] メッシュコード
	* @return PGDBファイルパス/FGDBフォルダパス
	*/
	uh::tstring GetWorkspaceString( const uh::tstring& mesh ) const;

	/**
	 * @brief コンバート処理メイン部分
	 * @param hasError [out] エラーが出た場合は、trueが返る
	 * @param mesh [in] 処理対象メッシュ 指定がない場合は、bulkexeがTRUEのテーブルを全件一括処理
	 */
	void ConvertTables(bool& hasError, const uh::tstring& mesh = _T(""));

	/**
	 * @brief テーブル単体のコンバート処理を行う。実体化はこの関数では行われない。
	 * @param table [in] 更新対象テーブル名
	 * @param dstT  [in, out] 更新対象テーブル
	 * @param mesh  [in] 処理対象メッシュ 指定がない場合は、bulkexeがTRUEのテーブルを全件一括処理
	 * @retval true  処理成功
	 * @retval false 処理失敗
	 */
	bool ConvertTable(const std::pair<uh::tstring,IndexList>& table, sindy::CSPTableContainer& dstT, const uh::tstring& mesh);

private:
	uh::tstring m_strInput; //!< 入力先指定文字列
	CEditor   m_input, m_output;		//!< 入出力先のDB
	DBReplicater m_dbReplicater;		//!< レコードコピークラス
	ConvertingDefinitionManager m_manager; //!< コンバート対象定義クラス
	Arguments m_args; //!< 引数クラス
	RowMutex m_limitter; //!< フィーチャ二重処理防止用
	std::map<uh::tstring, std::map<uh::tstring, uh::tstring>> m_relationFields; //!< first:テーブル名、second:更新時にキャッシュしておく外部キーフィールド名(SRC, DST)
	RelationValues m_relationValues;           //!< 更新後の外部キーの値を格納しているマップ
};
