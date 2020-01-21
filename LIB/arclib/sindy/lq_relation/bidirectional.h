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

#ifndef SINDY_BIDIRECTIONAL_LQ_RELATION_H_
#define SINDY_BIDIRECTIONAL_LQ_RELATION_H_

#include <sindy/lq_relation/directional_interface.h>

//namespace sindy {

//////////////////////////////////////////////////////////////////////
// bidirectional_lq_relation 定義
//////////////////////////////////////////////////////////////////////

/**
 * @brief 双指向性リンク列関係クラスの実装。
 *
 * とりあえずこれをsindytl標準の双指向性リンク列関係クラスとする。<br>
 * バッチ処理向けの実装なので、エディタ系では使い辛いか。
 */
class bidirectional_lq_relation : public sindy::directional_lq_relation_mutable
{
public:
	typedef boost::shared_ptr<bidirectional_lq_relation> ref;

// 純粋仮想関数の解決
	virtual bool exist_lq(const link_id_type& id) const;
	virtual bool exist_directional_lq(const link_id_type& link_id, const link_dir_type& link_dir) const;
	virtual bool attach_lq(const link_id_type& link_id, const link_dir_type& link_dir, const inf_id_type& inf_id);
	virtual bool detach_lq(const link_id_type& link_id, const inf_id_type& inf_id);

// ファクトリメソッド
	static ref create();

private:
// 定義
	/// リンクIDをキーとした、順・逆方向リンク列ID一覧。
	typedef std::map<long, std::pair<long, long> > data_t;

// 変数
	data_t data;
};

//////////////////////////////////////////////////////////////////////
// bidirectional_lq_relation 実装
//////////////////////////////////////////////////////////////////////

inline bool bidirectional_lq_relation::exist_lq(const link_id_type& id) const
{
	// リンクIDに対応する値があれば、リンク列は関連付けられている。
	return data.find(id.value()) != data.end();
}

inline bool bidirectional_lq_relation::exist_directional_lq(const link_id_type& link_id, const link_dir_type& link_dir) const
{
	//using namespace schema;
	// リンクIDに対応するリンク列IDの対を探す。
	data_t::const_iterator it = data.find(link_id.value());

	// 指定された方向にリンク列IDが与えられていれば、リンク列は関連付けられている。
	if(it != data.end()) {
		switch(link_dir.value()) {
		case sindy::schema::link_queue::link_dir::kFore:
			return it->second.first != 0;
		case sindy::schema::link_queue::link_dir::kReverse:
			return it->second.second != 0;
		}
	}

	return false;
}

inline bool bidirectional_lq_relation::attach_lq(
	const link_id_type& link_id,
	const link_dir_type& link_dir,
	const inf_id_type& inf_id)
{
	//using namespace schema;

	// リンクIDに対応する値を求める。
	// 値が既に存在する場合はそれを、しない場合は新たに作成する。
	data_t::mapped_type& value = data[link_id.value()];

	// 以下の場合、リンク列の関連付けは失敗する。
	// - 指定された方向に既にリンク列が関連付けられている。
	// - 指定されたのとは逆方向に同じリンク列が関連付けられている。
	long* p = 0;
	switch(link_dir.value()) {
	case sindy::schema::link_queue::link_dir::kFore:
		p = &value.first;
		if(value.second == inf_id.value())
			return false;
		break;
	case sindy::schema::link_queue::link_dir::kReverse:
		p = &value.second;
		if(value.first == inf_id.value())
			return false;
		break;
	}

	if(! p || *p)
		return false;

	// 関連付け。
	*p = inf_id.value();

	return true;
}

inline bool bidirectional_lq_relation::detach_lq(const link_id_type& link_id, const inf_id_type& inf_id)
{
	// リンクIDに対応するリンク列IDの対を探す。
	data_t::iterator it = data.find(link_id.value());

	if(it != data.end()) {
		if(it->second.first == inf_id.value()) {
			it->second.first = 0;
		}
		else if(it->second.second == inf_id.value()) {
			it->second.second = 0;
		}
		else
			return false;

		if(it->second.first == 0 && it->second.second == 0) {
			data.erase(it);
		}

		return true;
	}
	else
		return false;
}

inline bidirectional_lq_relation::ref bidirectional_lq_relation::create()
{
	return boost::shared_ptr<bidirectional_lq_relation>(new bidirectional_lq_relation());
}

//} // namespace sindy

#endif // SINDY_BIDIRECTIONAL_LQ_RELATION_H_
