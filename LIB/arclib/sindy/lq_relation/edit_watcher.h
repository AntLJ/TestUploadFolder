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

#ifndef SINDY_LQ_RELATION_EDIT_WATCHER_H_
#define SINDY_LQ_RELATION_EDIT_WATCHER_H_

#include <boost/utility.hpp>
#include <boost/logic/tribool.hpp> 
#include <sindy/lq_relation/interface.h>

namespace sindy {

/**
 * @brief �����N��֌W����ɂ��t���O�̏�ԕω����Ď�����N���X�B
 *
 * �����N��֌W�m�F�I�u�W�F�N�g��Ԃ��֐� get_exister() ���������邱�ƂŁA��茵���ȊĎ����s����B
 */
class lq_relation_edit_watcher : virtual public lq_relation_edit, boost::noncopyable
{
public:
// ��`
	typedef boost::tribool flag_type; ///< �t���O�𗧂Ă�(true)���~�낷(false)���v�m�F(indeterminate)���B

	typedef std::map<long, flag_type> flag_map; ///< �����NID�ʃt���O����ꗗ�B
	typedef flag_map::value_type value_type;
	typedef flag_map::const_iterator const_iterator;

	typedef boost::shared_ptr<lq_relation_edit> lq_relation_edit_ref;

// �R���X�g���N�^�ƃf�X�g���N�^
	lq_relation_edit_watcher();
	lq_relation_edit_watcher(const lq_relation_edit_ref& pEditor);

// �ݒ�
	void clear();

// �擾
	const_iterator begin() const;
	const_iterator end() const;

	virtual const lq_relation_exist* get_exister() const { return 0; }

// �������z�֐��̉���
	virtual bool attach_lq(const link_id_type& link_id, const link_dir_type& link_dir, const inf_id_type& inf_id);
	virtual bool detach_lq(const link_id_type& link_id, const inf_id_type& inf_id);

private:
// �ϐ�
	const lq_relation_edit_ref m_pEditor; ///< �ҏW�ҁB
	flag_map m_FlagMap; ///< �t���O����ꗗ�B
};

//////////////////////////////////////////////////////////////////////
// lq_relation_edit_watcher ����
//////////////////////////////////////////////////////////////////////

inline lq_relation_edit_watcher::lq_relation_edit_watcher()
{
}

inline lq_relation_edit_watcher::lq_relation_edit_watcher(const lq_relation_edit_ref& pEditor) :
m_pEditor(pEditor)
{
}

inline void lq_relation_edit_watcher::clear()
{
	m_FlagMap.clear();
}

inline lq_relation_edit_watcher::const_iterator lq_relation_edit_watcher::begin() const
{
	return m_FlagMap.begin();
}

inline lq_relation_edit_watcher::const_iterator lq_relation_edit_watcher::end() const
{
	return m_FlagMap.end();
}

inline bool lq_relation_edit_watcher::attach_lq(
	const link_id_type& link_id,
	const link_dir_type& link_dir,
	const inf_id_type& inf_id)
{
	const lq_relation_exist* pExist = get_exister();

	if(pExist) {
		flag_map::iterator it = m_FlagMap.end();

		// �����N�ɏ��߂Ẵ����N�񂪊֘A�t����ꂽ��A�t���O�𗧂Ă�B
		if(! pExist->exist_lq(link_id)) {
			it = m_FlagMap.insert(value_type(link_id.value(), true)).first;
		}

		// �����N����֘A�t����B
		bool bResult = m_pEditor ? m_pEditor->attach_lq(link_id, link_dir, inf_id) : true;

		// �����N�ɏ��߂Ẵ����N����֘A�t���悤�Ƃ��Ď��s������A�t���O���~�낻���Ƃ��Ă��Ȃ�����A�t���O���������߂�B
		if(it != m_FlagMap.end() && (bResult ^ (static_cast<bool>(it->second)))) {
			m_FlagMap.erase(it);
		}
		
		return bResult;
	}
	else {
		if(! m_pEditor || m_pEditor->attach_lq(link_id, link_dir, inf_id)) {
			m_FlagMap[link_id.value()] = true;
			return true;
		}
		else
			return false;
	}
}

inline bool lq_relation_edit_watcher::detach_lq(
	const link_id_type& link_id,
	const inf_id_type& inf_id)
{
	// �����N�����菜���B
	if(m_pEditor && m_pEditor->detach_lq(link_id, inf_id))
		return false;

	const lq_relation_exist* pExist = get_exister();

	if(pExist) {
		// �����N����S�Ẵ����N�񂪎�菜���ꂽ��A�t���O���~�낷�B
		if(! pExist->exist_lq(link_id)) {
			flag_map::iterator it = m_FlagMap.insert(value_type(link_id.value(), false)).first;
			// �t���O�𗧂Ă�\��ɂȂ��Ă����ꍇ�́A�t���O���������߂�B
			if(it->second) {
				m_FlagMap.erase(it);
			}
		}
	}
	else {
		m_FlagMap[link_id.value()] = boost::indeterminate;
	}

	return true;
}

} // namespace sindy

#endif // SINDY_LQ_RELATION_FLAG_WATCHER_H_
