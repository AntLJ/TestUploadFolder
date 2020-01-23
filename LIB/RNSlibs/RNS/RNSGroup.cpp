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

/** @file
 *
 * @author	f_kik
 * @date	2002/04/26
 */

#include "RNSException.h"
#include "RNSGroup.h"

/*
 * **************************************************
 *      RNSGroup
 * **************************************************
 */
// ===== public ���\�b�h ===== //
// ----- �����ݒ� ----- //
/// �f�t�H���g�R���X�g���N�^
RNSGroup::RNSGroup()
{
	mID = 0; mParent = 0; mNext = 0; mPrev = 0;
}

/**
 * �R���X�g���N�^
 */
RNSGroup::RNSGroup(RNSIDRef iID)
{
	mID = iID; mParent = 0; mNext = 0; mPrev = 0;
}

/**
 * �R���X�g���N�^
 */
RNSGroup::RNSGroup(RNSIDRef iID, RNSGroup* iParent)
{
	mID = iID; mParent = iParent; mNext = 0; mPrev = 0;
}

/**
 * ������́A���̃O���[�v��ݒ肷��
 */
void RNSGroup::setNextGroup(RNSGroup* iGroup)
{
	mNext = iGroup;
}

/**
 * ������́A�O�̃O���[�v��ݒ肷��
 */
void RNSGroup::setPrevGroup(RNSGroup* iGroup)
{
	mPrev = iGroup;
}

// ----- �擾 ----- //
/// �O���[�v���ʔԍ��擾
RNSIDRef RNSGroup::id()
{
	return mID; 
}

/// �e�O���[�v�擾
RNSGroup* RNSGroup::parent()
{
	return mParent;
}

/// ������́A���̃O���[�v���擾
RNSGroup* RNSGroup::nextGroup()
{
	return mNext;
}

/// ������́A�O�̃O���[�v���擾
RNSGroup* RNSGroup::prevGroup()
{
	return mPrev;
}

/**
 * �ŏ�ʂ̃O���[�v���擾����
 *
 * @return	�O���[�v�ւ̃|�C���^
 */
RNSGroup* RNSGroup::master()
{
	RNSGroup*	pParent = parent();
	return pParent ? pParent->master() : this;
}

/// �ŏ��̃����o�����������q���擾����
RNSGroup::least_iterator RNSGroup::first()
{
	using namespace std;
	least_iterator itr;

	(iterator&)itr = begin();

	if(empty()) {
		// ��f�[�^�̔����q
		itr.setCommunity(this);
		itr.setGroup(this);
		itr.setIsLast(true);
	}
	else {
		// �O���[�v���ʂ̃O���[�v���i�[���Ă���ꍇ�A�i�[����Ă���O���[�v�̍ŏ��̃����o�𔽕��q������

		if(isCommunity()) {
			RNSGroup* pGroup = dynamic_cast<RNSGroup*>(itr->second.obj());
			itr = pGroup->first();
		}
		else {
			itr.setGroup(this);
			itr.setIsLast(false);
		}
		itr.setCommunity(this);
	}

	return itr;
}

/// �Ō�̃����o�����������q���擾����
RNSGroup::least_iterator RNSGroup::last()
{
	least_iterator itr;

	(iterator&)itr = end();

	if(empty()) {
		// ��f�[�^�̔����q
		itr.setCommunity(this);
		itr.setGroup(this);
		itr.setIsLast(true);
	}
	else {
		--itr;
		if(isCommunity()) {
			RNSGroup* pGroup = dynamic_cast<RNSGroup*>(itr->second.obj());
			itr = pGroup->last();
		}
		else {
			itr.setGroup(this);
			itr.setIsLast(false);
		}
		itr.setCommunity(this);
	}

	return itr;
}

// �����o�� iterator ��T�����Ď擾
RNSGroup::least_iterator RNSGroup::search(RNSIDRef iMemberID)
{
	least_iterator	itr;

	RNSIDRef	pID = extractID(iMemberID);

	if(isCommunity()) {
		if(! pID) {
			itr = last();
		}
		else {
			RNSGroup*	pGroup = group(pID);
			if(pGroup) {
				itr = pGroup->search(iMemberID);
			}
			else {
				itr = last();
			}
		}
	}
	else {
		RNSIDRef	pMemberID = ! pID ? iMemberID : pID;

		(iterator&)itr = find(pMemberID);

		if((iterator&)itr == end()) {
			itr.setGroup(this);
			itr.setIsLast(true);
		}
		else {
			itr.setGroup(this);
			itr.setIsLast(false);
		}
	}

	itr.setCommunity(this);
	return itr;
}

// �����o���擾����
RNSObject*	RNSGroup::member(RNSIDRef iMemberID)
{
	least_iterator itr = search(iMemberID);
	return itr.isLast() ? 0 : itr->second;
}

/**
 * �w�肵��ID�̃O���[�v���擾����
 *
 * @return	�O���[�v�ւ̃|�C���^\n
 *			���N���X���R�~���j�e�B�ł͂Ȃ��ꍇ�A�܂��͓��R�~���j�e�B��ID�����L���Ȃ��ꍇ�͎��Ȃւ̃|�C���^\n
 *			�Y������O���[�v�����݂��Ȃ������ꍇ�� NULL
 *
 */
RNSGroup* RNSGroup::group(RNSIDRef iGroupID)
{
	RNSGroup*		pResult = 0;
	
	if(isCommunity()) {
		RNSIDRef	pID = extractID(iGroupID);

		if(! pID) {
			// ID���擾�ł��Ȃ������ꍇ�͎��Ȃւ̃|�C���^
			pResult = this;
		}
		else {
			// �Ή�����O���[�v��T��
			// ���݂��Ȃ������� NULL
			iterator itr = find(pID);
			pResult = (itr == end() ? 0 : dynamic_cast<RNSGroup*>(itr->second.obj())->group(iGroupID));
		}
	}
	else {
		// �R�~���j�e�B�ł͂Ȃ������ꍇ���Ȃւ̃|�C���^
		pResult = this;
	}

	return pResult;
}

/* ----- �ǉ� ----- */
/// �����o�ǉ�
bool RNSGroup::add(RNSIDRef iMemberID, RNSObjectPtr iMember)
{
	bool			result = false;

	RNSIDRef	pID = extractID(iMemberID);

	if(isCommunity()) {
		// ���N���X���R�~���j�e�B�ł�������A�Y������ŏ��O���[�v��T���i���邢�͐����j���Ă����ɓo�^
		RNSGroup* pGroup = addGroup(pID);
		result = pGroup->add(iMemberID, iMember);
	}
	else {
		// �R�~���j�e�B�łȂ�������A���̃O���[�v�ɓo�^
		RNSIDRef	pMemberID = pID ? pID : iMemberID;

		std::pair<iterator, bool>	aPair;
		aPair = insert(RNSObjectMap::value_type(pMemberID, iMember));
		result = aPair.second;
	}

	return result;
}

/**
 * �O���[�v�ǉ�
 *
 * @param	iGroupID	�ǉ�����O���[�v�̃O���[�vID\n
 *						�O���[�v���O���[�v���ǉ��\
 * @return	�ǉ����ꂽ�O���[�v�ւ̃|�C���^\n
 *			���ɊY������O���[�v�����݂��Ă����ꍇ�́A���̒ǉ������ɂ��̃O���[�v�ւ̃|�C���^��Ԃ��B
 */
RNSGroup* RNSGroup::addGroup(RNSIDRef iGroupID)
{
	RNSGroup*		result = 0;

	// �R�~���j�e�B�łȂ���΂��̃��\�b�h�͖���
	if(isCommunity()) {
		RNSIDRef	pID = extractID(iGroupID);

		// �O���[�vID���擾�ł�����Ή��O���[�v��T��
		// �Ή��O���[�v��������Ȃ������琶���A�ǉ�
		if(pID) {
			iterator	itr;
			
			if((itr = find(pID)) != end()) {
				RNSGroup* son = dynamic_cast<RNSGroup*>(itr->second.obj());
				result = son->addGroup(iGroupID);
				if(! result) {
					result = son;
				}
			}
			else {
				result = addGroup(iGroupID, newGroup(pID));
			}
		}
	}

	return result;
}

/// �O���[�v�ǉ�
RNSGroup* RNSGroup::addGroup(RNSIDRef iGroupID, RNSPtr<RNSGroup> iGroup)
{
	RNSGroup*		result = 0;

	// �R�~���j�e�B�łȂ���΂��̃��\�b�h�͖���
	if(isCommunity()) {
		RNSIDRef	pID = extractID(iGroupID);

		if(pID) {
			iterator	itr;
			
			if((itr = find(pID)) != end()) {
				RNSGroup* son = dynamic_cast<RNSGroup*>(itr->second.obj());
				result = son->addGroup(iGroupID, iGroup);
				if(! result) {
					result = son;
				}
			}
			else {
				using namespace std;
				pair<iterator, bool>	aPair;
				aPair = RNSObjectMap::insert(RNSObjectMap::value_type(pID, iGroup));
				
				if(aPair.second) {
					iterator aThis;
					iterator aPrev;
					iterator aNext;
					
					result = dynamic_cast<RNSGroup*>((aThis = aPrev = aNext = aPair.first)->second.obj());
					--aPrev;
					++aNext;
					
					RNSGroup*	aThisGroup = dynamic_cast<RNSGroup*>(aThis->second.obj());
					
					if(aThis != begin()) {
						RNSGroup*	aPrevGroup = dynamic_cast<RNSGroup*>(aPrev->second.obj());
						aPrevGroup->setNextGroup(aThisGroup);
						aThisGroup->setPrevGroup(aPrevGroup);
					}
					else {
						aThisGroup->setPrevGroup(0);
					}
					
					if(aNext != end()) {
						RNSGroup*	aNextGroup = dynamic_cast<RNSGroup*>(aNext->second.obj());
						aNextGroup->setPrevGroup(aThisGroup);
						aThisGroup->setNextGroup(aNextGroup);
					}
					else {
						aThisGroup->setNextGroup(0);
					}
				}
			}
		}
	}

	return result;
}

// ----- �폜 ----- //
/// �w�肵�������o���폜����
void RNSGroup::remove(RNSIDRef iMemberID)
{
	RNSIDRef	pID = extractID(iMemberID);
	RNSGroup*		pGroup;

	if(pID) {
		pGroup = group(iMemberID);
	}
	else {
		pGroup = this;
	}

//	RNSReleaseProtect(iMemberID);
	pGroup->erase(iMemberID);
}

/// �w�肵���O���[�v���폜����
void RNSGroup::removeGroup(RNSIDRef iGroupID)
{
	// �w�肳�ꂽ�O���[�v��T��
	RNSGroup*	pGroup = group(iGroupID);

	if(pGroup) {
		// �O���[�v�𔭌�������A���̐e�O���[�v��T��
		RNSGroup* pParent = pGroup->parent();

		// �e�O���[�v�����݂��Ȃ��������O����
		if(! pParent)
			throw RNSException("cant remove group");

		// �e�O���[�v�ɍ폜�˗���\������
		RNSIDRef	pID = pParent->extractID(iGroupID);

		if(! pID)
			throw RNSException("cant remove group");

		pParent->erase(pID);
	}
}

// ----- ���z�֐� ----- //
/// �����oID����A���̃O���[�v�ł̎���ID���擾����
RNSIDRef RNSGroup::extractID(RNSIDRef iMemberID)
{
	return 0;
}

/// �V�K�O���[�v����
RNSPtr<RNSGroup> RNSGroup::newGroup(RNSIDRef iGroupID)
{
	return new RNSGroup(iGroupID, this);
}

// ===== public ���\�b�h ===== //
/// �f�t�H���g�R���X�g���N�^/// �R�~���j�e�B���H
bool RNSGroup::isCommunity() const
{
	return false;
}

/*
 * **************************************************
 *      RNSGroup::least_iterator
 * **************************************************
 */


RNSGroup::least_iterator::least_iterator()
{
	mCommunity = 0;
	mGroup = 0;
	mIsLast = true;
}

// ----- �擾 ----- //
/**
 * �R�~���j�e�B�i�����q���A�N�Z�X����S�Ẵ����o���i�[���ꂽ�O���[�v�j���擾����
 *
 * @return	�R�~���j�e�B
 */
RNSGroup* RNSGroup::least_iterator::community()
{
	return mCommunity;
}

/**
 * �����q�����݃A�N�Z�X���Ă��郁���o��������O���[�v���擾����
 *
 * @param	iGroup	�O���[�v
 */
RNSGroup* RNSGroup::least_iterator::group()
{
	return mGroup;
}

/**
 * �����q���R�~���j�e�B�̍Ō�̃����o�ɃA�N�Z�X���I����������擾����
 *
 * @retval	true	�A�N�Z�X�I��
 * @retval	false	�A�N�Z�X���I��
 */
bool RNSGroup::least_iterator::isLast() const
{
	return mIsLast;
}

// ----- �I�y���[�^ ----- //
/**
 * ���̃����o�ֈړ�
 */
RNSGroup::least_iterator& RNSGroup::least_iterator::operator ++ ()
{
	using namespace std;

	// �p�����̔����q�̗v�̂Ŏ��̃����o���擾
	++((iterator&)*this);

	if((iterator&)(*this) == group()->end()){
		setIsLast(true);

		// ���݃A�N�Z�X���Ă���O���[�v�̍Ō�̃f�[�^�ɓ��B������A���̃O���[�v�ֈړ�����
		if(group() == community()) {
			// ���݃A�N�Z�X���Ă���O���[�v���R�~���j�e�B���̂��̂�������A�Ō�܂œ��B����
			least_iterator tmpItr = community()->last();
			tmpItr.setCommunity(community());
			tmpItr.setIsLast(true);
			*this = tmpItr;
			return *this;
		}

		// ���̃O���[�v�����݂��Ă����炻����֔����q���ړ��A�Ȃ���ΏI��
		RNSGroup* pNextGroup = group()->nextGroup();
		if(! pNextGroup) {
			RNSGroup* pParentGroup = group()->parent();
			if(pParentGroup && pParentGroup != community()) {
				pNextGroup = pParentGroup->nextGroup();
			}
		}

		if(pNextGroup) {
			least_iterator tmpItr = pNextGroup->first();
			tmpItr.setCommunity(community());
			*this = tmpItr;
		}
	}
	else {
		// �܂��Ō�̃f�[�^�ł͂Ȃ�������A���̂܂ܑ��s
		mIsLast = false;
	}

	return *this;
}

// ===== private ���\�b�h ===== //
// ----- �����ݒ� ----- //
/**
 * �R���X�g���N�^�i�R�~���j�e�B�A�O���[�v�w��j
 *
 * @param	iCommunity	�R�~���j�e�B
 * @param	iGroup		�����q�����݃A�N�Z�X���Ă��郁���o��������O���[�v
 */
RNSGroup::least_iterator::least_iterator(RNSGroup* iCommunity, RNSGroup* iGroup)
{
	mCommunity = iCommunity;
	mGroup = iGroup;
	mIsLast = false;
}

// ----- �ݒ� ----- //
/**
 * �R�~���j�e�B��ݒ肷��
 *
 * @param	iCommunity	�R�~���j�e�B
 */
void RNSGroup::least_iterator::setCommunity(RNSGroup* iCommunity)
{
	mCommunity = iCommunity;
}

/**
 * �����q�����݃A�N�Z�X���Ă��郁���o��������O���[�v��ݒ肷��
 *
 * @param	iGroup	�O���[�v
 */
void RNSGroup::least_iterator::setGroup(RNSGroup* iGroup)
{
	mGroup = iGroup;
}

/**
 * �����q���R�~���j�e�B�̍Ō�̃����o�ɃA�N�Z�X���I���������ݒ肷��
 *
 * @param	iIsLast	�A�N�Z�X���I��������H
 */
void RNSGroup::least_iterator::setIsLast(bool iIsLast)
{
	mIsLast = iIsLast;
}

/*
 * **************************************************
 *      RNSCommunity
 * **************************************************
 */

/// �R�~���j�e�B���H
bool RNSCommunity::isCommunity() const
{
	return true;
}
