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

#ifndef __RNS_ID__
#define __RNS_ID__

/* ************************************************

 	���e :
 	���� : 
 
 	���� :	Fukuta.K
 	
************************************************* */

#include "RNS.h"

#include <functional>

#include "RNSObject.h"

// ///// �|�C���^�Z�b�g�E�}�b�v ///// //

/**
 * @brief	�|�C���^��r�N���X
 *
 * �|�C���^�̓��e���r����N���X�B
 */
template <typename T>
struct _RNSLess : std::binary_function<T, T, bool>
{
	bool operator()(const T p1, const T p2) const
	{
		return *T < *T;
	}
};

#define pointer_set(T)		std::set< T, _RNSLess<T> >		///< �|�C���^�̓��e�Ń\�[�g����std::set
#define pointer_map(T, T2)	std::map< T, T2, _RNSLess<T> >	///< �|�C���^�̓��e�Ń\�[�g����std::map


/* ///// �N���X�錾 ///// */

/**
 * @brief	ID ���N���X
 *
 * �I�u�W�F�N�g��ID���������鎞�́A���̃N���X����h�������N���X��p����B
 */
class RNSID : public RNSObject {
 public:
	/// �召��r���s��
	virtual bool operator < (const RNSID& iID) const = 0;
};

_RNSOBJECT_TYPEDEF(RNSID);

#if(0)
/**
 * RNSID �^�I�u�W�F�N�g�̕ێ��|�C���^�̂Q���q��I�u�W�F�N�g
 *
 * RNSID �^�I�u�W�F�N�g���i�[�����|�C���^���m�ő召��r�������Ȃ�
 */
struct RNSIDPtrLess : std::binary_function<RNSIDConstPtr, RNSIDConstPtr, bool>
{
	bool operator()(const RNSIDConstPtr& p1, const RNSIDConstPtr& p2) const
	{
		return *p1 < *p2;
	}
};
#endif

/**
 * �P���^�w��
 */
template<typename _Type>
class _RNSID : virtual public RNSID {
	/// ID�i�w��^�̒l�j
	_Type	mID;

	/// �w��^��ID��ݒ肷��
 	void setID(_Type iID)
	{
		mID = iID;
	}

 public:
	/// �R���X�g���N�^�iID�ݒ�j
	_RNSID(_Type iID) 	{ setID(iID); }

	/// �f�X�g���N�^
	virtual ~_RNSID(){}

	/* ----- �擾 ----- */
	/// �w��^��ID��Ԃ�
	_Type id() const
	{
		_Type tmpid = mID;
		return tmpid;
	}

	/* ----- �I�y���[�^ ----- */
	/// �w��^��ID��Ԃ�
	operator _Type () const
	{
		return mID;
	}

	/// �召��r���s��
	virtual bool operator < (const RNSID& iID) const
	{
		const _RNSID* pID = dynamic_cast<const _RNSID*>(&iID);
		return pID ? (*this < *pID) : 0;
	}

	bool operator < (const _RNSID& iID) const
	{
		return id() < iID.id();
	}
};

typedef _RNSID<int>	RNSIntID;
typedef _RNSID<MeshCode>	RNSMeshCodeID;

_RNSOBJECT_TYPEDEF(RNSIntID);

#endif /* __RNS_ID__ */
