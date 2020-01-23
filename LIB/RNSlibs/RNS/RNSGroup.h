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

#ifndef __RNS_GROUP__
#define __RNS_GROUP__

#include "RNS.h"
#include "RNSObject.h"

#include <map>

#include "RNSID.h"

typedef std::map<RNSIDRef, RNSObjectPtr>	RNSObjectMap;
typedef RNSObjectMap::iterator				RNSObjectMapItr;
typedef RNSObjectMap::const_iterator		RNSObjectMapConstItr;

/**
 * �g��std::map�N���X
 *
 * - �O���[�v��map�Ɠ��l�ɒl�i�����o�Ƃ����j���i�[����B\n
 * - �O���[�v�́A�i�[���Ă���S�Ẵ����o���V�[�P���V�����Ɏ擾�ł���B\n
 * - �O���[�v�́A�ȉ��̃��\�b�h���I�[�o�[���C�h���邱�ƂŃO���[�v���i�[�ł���悤�ɂȂ�B
 *		-# isCommunity ... true ��Ԃ��悤�ɃI�[�o�[���C�h����B
 *		-# extractID ... �����oID����A���O���[�v�ł̕���ID���擾����B
 *		-# newGroup ... �i�[����O���[�v�̍����B�i����͕K�{�ł͂Ȃ��j
 */
class RNSGroup : public RNSObjectMap,
				 virtual public RNSObject
{
	RNSIDRef	mID;		///< �O���[�vID
	RNSGroup*	mParent;	///< �e�O���[�v

	RNSGroup*	mNext;		///< ���̃O���[�v
	RNSGroup*	mPrev;		///< �O�̃O���[�v

 public:
	/* ----- least_iterator �̒�` ----- */
	/**
	 * �O���[�v���ɂ���S�Ẵ����o���V�[�P���V�����Ɏ擾���锽���q
	 */
	 class least_iterator : public RNSObjectMap::iterator {
		friend RNSGroup;

		RNSGroup*	mCommunity;	///< �����q���Ăяo�����O���[�v
		RNSGroup*	mGroup;		///< ���ݔ����q���ǂݍ���ł���O���[�v
		bool		mIsLast;	///< �����q���Ō�̃����o�ɒB������

		// ----- �����ݒ� ----- //
		/// �R���X�g���N�^�i�R�~���j�e�B��O���[�v��ݒ�j
		least_iterator(RNSGroup* iCommunity, RNSGroup* iGroup);

		// ----- �ݒ� ----- //
		void setCommunity(RNSGroup* iCommunity);
		void setGroup(RNSGroup* iGroup);
		void setIsLast(bool iIsLast);

	 public:
		// ----- �����ݒ� ----- //
		 /// �f�t�H���g�R���X�g���N�^
		least_iterator();

		/// �f�X�g���N�^
		~least_iterator(){}

		// ----- �擾 ----- //
		/// �����q���A�N�Z�X����S�Ẵ����o���i�[����Ă���O���[�v���擾
		RNSGroup*	community();

		/// ���ݔ����q���A�N�Z�X���Ă��郁���o��������O���[�v���擾
		RNSGroup*	group();

		/// �����q���R�~���j�e�B�̍Ō�̃����o�ɓ��B����
		bool	isLast() const;

		// ----- �I�y���[�^ ----- //
		/// ���̃����o��
		least_iterator& operator ++ ();
	};

	/* ----- �����ݒ� ----- */
	/// �f�t�H���g�R���X�g���N�^
	RNSGroup();

	/// �R���X�g���N�^�i�O���[�vID�ݒ�j
	RNSGroup(RNSIDRef iGroupID);

	/// �R���X�g���N�^�i�O���[�vID�A�e�O���[�v�ݒ�j
	RNSGroup(RNSIDRef iGroupID, RNSGroup* iParent);

	/// �f�X�g���N�^
	virtual ~RNSGroup(){}

	/// ������́A���̃O���[�v��ݒ肷��
	void	setNextGroup(RNSGroup* iGroup);

	/// ������́A�O�̃O���[�v��ݒ肷��
	void	setPrevGroup(RNSGroup* iGroup);

	/* ----- �ǉ� ----- */
	/// �����o��ǉ�����
	bool add(RNSIDRef iMemberID, RNSObjectPtr iProf);

	/// �O���[�v��ǉ����āA�|�C���^���擾����
	RNSGroup* addGroup(RNSIDRef iGroupID);

	/// �O���[�v��ǉ����āA�|�C���^���擾����
	RNSGroup* addGroup(RNSIDRef iGroupID, RNSPtr<RNSGroup> iGroup);

	/* ----- �폜 ----- */
	/// �����o���폜����
	void remove(RNSIDRef iMemberID);

	/// �O���[�v���폜����
	void removeGroup(RNSIDRef iGroupID);

	/* ----- �擾 ----- */
	/// �O���[�v���ʔԍ��擾����
	RNSIDRef		id();

	/// ������́A���̃O���[�v���擾����
	RNSGroup*		nextGroup();

	/// ������́A�O�̃O���[�v���擾����
	RNSGroup*		prevGroup();

	/// �e�O���[�v�擾����
	RNSGroup*		parent();
	
	/// �ŏ�ʂ̃O���[�v���擾����
	RNSGroup*	master();

	/// �ŏ��̃����o������ least_iterator ���擾����
	least_iterator	first();

	/// �Ō�̃����o������ iterator ���擾����
	least_iterator	last();

	/// �����o�̔����q���擾����
	least_iterator	search(RNSIDRef iMemberID);

	/// �����o���擾����
	RNSObject*	member(RNSIDRef iMemberID);

	/// �O���[�v���擾����
	RNSGroup*	group(RNSIDRef iGroupID);

	// ----- ���z�֐� ----- //
	/**
	 * �����oID����A���̃O���[�v�ł̎���ID���擾����
	 *
	 * @param	iMemeberID	�����oID
	 * @return	���̃O���[�v���ł̎���ID�ւ̓��I�|�C���^\n
	 *          ���̃O���[�v�Ƀ����o��������ꍇ�A�O���Ԃ�\n
	 *			�I�[�i�[�V�b�v�͏��n�����
	 */
	virtual RNSIDRef extractID(RNSIDRef iMemberID);

	/**
	 * �O���[�vID����A�V�����O���[�v�𐶐�����
	 *
	 * @param	iGroupID	�O���[�vID
	 * @return	���̃O���[�v���ł̎���ID�ւ̓��I�|�C���^\n
	 *          ���̃O���[�v�Ƀ����o��������ꍇ�A�O���Ԃ�\n
	 *			�I�[�i�[�V�b�v�͏��n�����
	 */
	virtual RNSPtr<RNSGroup> newGroup(RNSIDRef iGroupID);

	/**
	 * �R�~���j�e�B�i�����Ƀ����o�����݂��Ȃ��O���[�v�j���H
	 *
	 * @retval	true	�R�~���j�e�B�ł���
	 * @retval	false	�R�~���j�e�B�ł͂Ȃ�
	 */
	virtual bool isCommunity() const;
};

_RNSOBJECT_TYPEDEF(RNSGroup);

template<typename _BaseClass, typename _Member>
class _least_iterator : public _BaseClass::least_iterator {
public:
	_least_iterator(RNSGroup::least_iterator& iSrc)
	{ *(RNSGroup::least_iterator*)this = iSrc; }

	_Member* member()
	{ return isLast() ? 0 : dynamic_cast<_Member*>((*this)->second.obj()); }
};

/**
 * RNSGroup�i�[�N���X
 * �R�~���j�e�B�̓O���[�v�𕡐��i�[�ł���B\n
 * �R�~���j�e�B�́A�i�[���Ă���S�ẴO���[�v�̃����o���V�[�P���V�����Ɏ擾�ł���B\n
 * RNSGroup��isCommunity���I�[�o�[���C�h���������̃N���X
 */
class RNSCommunity: public RNSGroup {
public:
	/* ----- �����ݒ� ----- */
	/// �f�t�H���g�R���X�g���N�^
	RNSCommunity()
		: RNSGroup(){}

	/// �R���X�g���N�^�i�O���[�vID�ݒ�j
	RNSCommunity(RNSIDRef iGroupID)
		: RNSGroup(iGroupID){}

	/// �R���X�g���N�^�i�O���[�vID�A�e�O���[�v�ݒ�j
	RNSCommunity(RNSIDRef iGroupID, RNSGroup* iParent)
		: RNSGroup(iGroupID, iParent){}

	// ----- ���z�֐��̃I�[�o�[���C�h ----- //
	/// �R�~���j�e�B���H
	virtual bool isCommunity() const;
};

_RNSOBJECT_TYPEDEF(RNSCommunity);

#endif /* __RNS_MAP_GROUP__ */
