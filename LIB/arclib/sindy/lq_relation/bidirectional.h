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
// bidirectional_lq_relation ��`
//////////////////////////////////////////////////////////////////////

/**
 * @brief �o�w���������N��֌W�N���X�̎����B
 *
 * �Ƃ肠���������sindytl�W���̑o�w���������N��֌W�N���X�Ƃ���B<br>
 * �o�b�`���������̎����Ȃ̂ŁA�G�f�B�^�n�ł͎g���h�����B
 */
class bidirectional_lq_relation : public sindy::directional_lq_relation_mutable
{
public:
	typedef boost::shared_ptr<bidirectional_lq_relation> ref;

// �������z�֐��̉���
	virtual bool exist_lq(const link_id_type& id) const;
	virtual bool exist_directional_lq(const link_id_type& link_id, const link_dir_type& link_dir) const;
	virtual bool attach_lq(const link_id_type& link_id, const link_dir_type& link_dir, const inf_id_type& inf_id);
	virtual bool detach_lq(const link_id_type& link_id, const inf_id_type& inf_id);

// �t�@�N�g�����\�b�h
	static ref create();

private:
// ��`
	/// �����NID���L�[�Ƃ����A���E�t���������N��ID�ꗗ�B
	typedef std::map<long, std::pair<long, long> > data_t;

// �ϐ�
	data_t data;
};

//////////////////////////////////////////////////////////////////////
// bidirectional_lq_relation ����
//////////////////////////////////////////////////////////////////////

inline bool bidirectional_lq_relation::exist_lq(const link_id_type& id) const
{
	// �����NID�ɑΉ�����l������΁A�����N��͊֘A�t�����Ă���B
	return data.find(id.value()) != data.end();
}

inline bool bidirectional_lq_relation::exist_directional_lq(const link_id_type& link_id, const link_dir_type& link_dir) const
{
	//using namespace schema;
	// �����NID�ɑΉ����郊���N��ID�̑΂�T���B
	data_t::const_iterator it = data.find(link_id.value());

	// �w�肳�ꂽ�����Ƀ����N��ID���^�����Ă���΁A�����N��͊֘A�t�����Ă���B
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

	// �����NID�ɑΉ�����l�����߂�B
	// �l�����ɑ��݂���ꍇ�͂�����A���Ȃ��ꍇ�͐V���ɍ쐬����B
	data_t::mapped_type& value = data[link_id.value()];

	// �ȉ��̏ꍇ�A�����N��̊֘A�t���͎��s����B
	// - �w�肳�ꂽ�����Ɋ��Ƀ����N�񂪊֘A�t�����Ă���B
	// - �w�肳�ꂽ�̂Ƃ͋t�����ɓ��������N�񂪊֘A�t�����Ă���B
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

	// �֘A�t���B
	*p = inf_id.value();

	return true;
}

inline bool bidirectional_lq_relation::detach_lq(const link_id_type& link_id, const inf_id_type& inf_id)
{
	// �����NID�ɑΉ����郊���N��ID�̑΂�T���B
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
