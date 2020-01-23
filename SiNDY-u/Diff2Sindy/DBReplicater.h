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

namespace sindy {
	class CWorkspace;
	class CRowBase;
	class CTableContainer;
} // sindy

/**
 * @brief テーブルコピークラス
 */
class DBReplicater
{
public:
	DBReplicater();
	~DBReplicater();

	/**
	 * @brief テーブルをコピーする
	 *
	 * 同じスキーマでテーブルをコピーします。<br>
	 * 存在するレコードは全件コピーされます。
	 *
	 * @param work [in] コピー対象のテーブルがあるワークスペース
	 * @parm name [in] コピー元テーブル名
	 * @param work_new [in] コピー先のテーブルがあるワークスペース
	 * @param name_new [in] ペースト先テーブル名
	 * @note ペースト先のテーブルが存在する場合は処理に失敗します
	 */
	bool CopyTable2( const sindy::CWorkspace& work, const uh::tstring& name, const sindy::CWorkspace& work_new, const uh::tstring& name_new );

	/**
	 * @brief テーブルをコピーする
	 * @param work [in] コピー対象のテーブルがあるワークスペース
	 * @parm name [in] コピー元テーブル名
	 * @param work_new [in] コピー先のテーブルがあるワークスペース
	 * @param name_new [in] ペースト先テーブル名
	 * @see CopyTable2( const CWorkspace& work, const uh::tstring& name, const uh::tstring& name_new )
	 * @note ペースト先のテーブルが存在する場合は処理に失敗します
	 * @note 安全のため、SDE上では動作させないようになっています
	 * @retval true 成功
	 * @retval false 失敗
	 */
	bool CopyTable( const sindy::CWorkspace& work, const uh::tstring& name, const sindy::CWorkspace& work_new, const uh::tstring& name_new );

	/**
	 * @brief 条件に合致するもののみレコード追加する
	 *
	 * 指定カラム名が合致すれば、モデルが異なるテーブル間でもオブジェクトのコピーができます。
	 * 元がフィーチャクラスの場合は、先もフィーチャクラスである必要があります（逆はその限りではない）。
	 *
	 * @param filter [in] クエリフィルタ
	 * @param srcWorkspace [in] コピー元ワークスペース
	 * @param srcTable [in] コピー元テーブル
	 * @param destWorkspace [in] ペースト先ワークスペース
	 * @param destTable [in] ペースト先テーブル名
	 * @param listColumns [in] コピー対象カラムリスト
	 * @param all_columns [in,optional] trueならすべてのカラムをコピー対象とする（デフォルト：false）
	 * @note ペースト先のテーブルが存在しないと処理に失敗します
	 * @note フィーチャクラスの場合はジオメトリは自動的にコピー対象となります
	 * @retval true 成功
	 * @retval false 失敗
	 */
	bool InsertTable( IQueryFilterPtr filter, IWorkspacePtr srcWorkspace, const uh::tstring& srcTable, IWorkspacePtr destWorkspace, const uh::tstring& destTable, const std::list<uh::tstring>& listColumns, bool all_columns = false );

private:

};
