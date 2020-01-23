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
namespace util
{

/**
 * @brief 対象のスキーマにロックをかけるクラス
 */
class SchemaGuard
{
public:
	/**
	 * @brief コンストラクタ
	 * @param target [in] ロック対象
	 */
	SchemaGuard(ISchemaLockPtr target):
		m_target(target)
	{
		lock();
	}

	/**
	 * @brief デストラクタ
	 */
	virtual ~SchemaGuard()
	{
		releaseLock();
	}

protected:
	/**
	 * @brief 対象のスキーマをロックする
	 */
	void lock();

	/**
	 * @brief 対象のスキーマのロックを解除する
	 */
	void releaseLock();

	ISchemaLockPtr m_target; //!< ロック対象
};

/**
 * @brief 文字列の大文字小文字を無視して比較
 * @retval true  文字は一致している
 * @retval false 文字は一致していない
 */
inline bool eq( const uh::tstring& s1, const uh::tstring& s2 )
{
	return 0 == _tcsicmp( s1.c_str(), s2.c_str() );
}

/**
 * @param フォルダかどうか判定する
 * @note FGDBフォルダはフォルダとみなさない
 * @param path [in] 判定パス
 * @retval true  フォルダ
 * @retval false フォルダ以外
 */
bool isDirectory( const uh::tstring& path );

/**
 * @param ファイルかどうか判定する
 * @note FGDBフォルダはファイルとみなす
 * @param path [in] 判定パス
 * @retval true  ファイル
 * @retval false ファイル以外
 */
bool isFile( const uh::tstring& path );

/**
 * @brief 二つの空間参照が同等かどうかを判定する
 * @param src [in] 比較元
 * @param dst [in] 比較先
 * @retval true  一致している
 * @retval false 一致していない
 */
bool isEqual(ISpatialReferencePtr src, ISpatialReferencePtr dst);

/**
 * @brief ワークスペースから指定のデータセット群を取得
 * @param workspace [in] データセットを取得対象のワークスペース
 * @param type      [in] 取得するデータセットのタイプ
 * @return 取得したデータセット群
 * @retval null 取得に失敗
 */
IEnumDatasetPtr getDatasets( IWorkspacePtr workspace, esriDatasetType type );

/**
 * @brief UIDのコピーを取得
 * @param srcUID [in] コピー元
 * @return コピーされたUID
 * @retval null コピーに失敗
 */
IUIDPtr copyUID(IUIDPtr srcUID);

/**
 * @brief 文字列が数字だけで構成されているか判定
 * @retval true  数字のみ
 * @retval false 数字以外が含まれている
 */
bool isNumber( const uh::tstring& str );

/**
 * @brief オーナー名の部分を削除した文字列を返す
 */
uh::tstring eraseOwnerName( const uh::tstring& str);

/**
 * @brief フィールドマップを生成
 * @detail フィールド名は大文字になる
 * @param layer      [in]  フィールドマップ生成対象
 * @param shapeIndex [out] 形状フィールドのインデックス番号が出力される(ない場合-1)
 * @return フィールドマップ
 */
std::map<uh::tstring,long> createFieldMap(ITablePtr layer, long& shapeIndex);

} // util
