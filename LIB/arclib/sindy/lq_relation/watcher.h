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

#ifndef SINDY_LQ_RELATION_WATCHER_H_
#define SINDY_LQ_RELATION_WATCHER_H_

#include <stdexcept>
#include <sindy/lq_relation/edit_watcher.h>

namespace sindy {

//////////////////////////////////////////////////////////////////////
// lq_relation_watcher 定義
//////////////////////////////////////////////////////////////////////

#pragma warning( push )
#pragma warning( disable : 4250 )

class lq_relation_watcher : public lq_relation_edit_watcher, virtual public lq_relation_mutable
{
public:
// コンストラクタとデストラクタ
	lq_relation_watcher(const boost::shared_ptr<lq_relation_mutable>& pMutable);
	lq_relation_watcher(
		const boost::shared_ptr<lq_relation_edit>& pEditor,
		const boost::shared_ptr<const lq_relation_exist>& pExister);

// 取得
	virtual const lq_relation_exist* get_exister() const;

// 純粋仮想関数の解決
	virtual bool exist_lq(const link_id_type& id) const;

protected:
private:
// 変数
	const boost::shared_ptr<const lq_relation_exist> m_pExister;
};

#pragma warning( pop )

//////////////////////////////////////////////////////////////////////
// lq_relation_watcher 実装
//////////////////////////////////////////////////////////////////////

inline lq_relation_watcher::lq_relation_watcher(const boost::shared_ptr<lq_relation_mutable>& pMutable) :
lq_relation_edit_watcher(pMutable),
m_pExister(pMutable)
{
}

inline lq_relation_watcher::lq_relation_watcher(
	const boost::shared_ptr<lq_relation_edit>& pEditor,
	const boost::shared_ptr<const lq_relation_exist>& pExister) :
lq_relation_edit_watcher(pEditor),
m_pExister(pExister)
{
}

inline const lq_relation_exist* lq_relation_watcher::get_exister() const
{
	return m_pExister.get();
}

inline bool lq_relation_watcher::exist_lq(const link_id_type& id) const
{
	if(! m_pExister)
		throw std::runtime_error("exister is null.");

	return m_pExister->exist_lq(id);
}

} // namespace sindy

#endif // SINDY_LQ_RELATION_WATCHER_H_
