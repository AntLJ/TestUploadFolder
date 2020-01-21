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

/**
 * @file prog_modify_stamper.h
 * @brief プログラム最終更新履歴刻印クラス定義ヘッダ
 *
 * sindy::prog_modify_stamperを定義するヘッダ。
 */
#ifndef SINDY_PROG_MODIFY_STAMPER_H_
#define SINDY_PROG_MODIFY_STAMPER_H_

#include <boost/shared_ptr.hpp>
#include <arctl/field/index.h>
#include <arclib/sindy/stamper/modify_stamper.h>
#include <arclib/sindy/stamper/modify_date_stamper.h>
#include <sindy/schema/common.h>

namespace sindy {

//////////////////////////////////////////////////////////////////////
// prog_modify_stamper 定義
//////////////////////////////////////////////////////////////////////

/**
 * @brief プログラムによる更新が為されたことを示す履歴を刻印するクラス
 *
 * @par 刻印される属性:
 * - 最終更新プログラム名
 * - 最終プログラム更新日時
 * - 更新更新内容
 *
 * @test ProgModifyStamper - @ref unit_test 
 */
class prog_modify_stamper : public modify_stamper
{
public:
// 定義
	ARCTL_FIELD_INDEX_CLASS(index_type, (schema::ipc_table::kModifyProgName)(schema::ipc_table::kProgModifyDate)(schema::ipc_table::kUpdateType));

///@name コンストラクタとデストラクタ
//@{
	prog_modify_stamper();
	prog_modify_stamper(IFields* ipFields);
//@}

protected:
///@name 純粋仮想関数のオーバーライド
//@{
	virtual bool stamp(IRowBuffer* ipRowBuffer, schema::ipc_table::update_type::ECode eUpdateType) const;
//@}

private:
// 変数
	boost::shared_ptr<index_type> m_pIndex; ///< 属性フィールドインデックスキャッシュ。
};

//////////////////////////////////////////////////////////////////////
// prog_modify_stamper 実装
//////////////////////////////////////////////////////////////////////

/**
 * @brief 様々なテーブルに流用可能なオブジェクトを構築する。
 */
inline prog_modify_stamper::prog_modify_stamper()
{
}

/**
 * @brief １つのテーブルに最適化されたオブジェクトを構築する。
 *
 * 属性フィールドインデックスをキャッシュすることで IRowBuffer::FindField() のコストを省く。<br>
 * 他のテーブルに所属するオブジェクトを渡してはならない。
 */
inline prog_modify_stamper::prog_modify_stamper(IFields* ipFields) :
m_pIndex(new index_type(ipFields))
{
}

inline bool prog_modify_stamper::stamp(IRowBuffer* ipRowBuffer, schema::ipc_table::update_type::ECode eUpdateType) const
{
	using namespace arctl::field;
	using namespace schema::ipc_table;

	TCHAR szBuffer[_MAX_PATH];

	// 属性フィールドインデックスを求める。
	boost::shared_ptr<index_type>
		pIndex(m_pIndex ? m_pIndex : boost::shared_ptr<index_type>(new index_type(ipRowBuffer)));

	// 最終更新プログラム名を設定する。
	::GetModuleFileName(NULL, szBuffer, _MAX_PATH);
	LPCTSTR p = ::_tcsrchr(szBuffer, _T('\\'));
	atl2::valid(ipRowBuffer->put_Value(index_t<kModifyProgName>(*pIndex), _variant_t(p ? p + 1 : szBuffer)));

	// 最終プログラム更新日時を設定する。
	if(! prog_modify_date_stamper(*pIndex)(ipRowBuffer))
		return false;

	// 最終更新内容を設定する。
	atl2::valid(ipRowBuffer->put_Value(index_t<kUpdateType>(*pIndex), _variant_t(static_cast<long>(eUpdateType))));

	return true;
}

} // namespace sindy

#endif // SINDY_PROG_MODIFY_STAMPER_H_
