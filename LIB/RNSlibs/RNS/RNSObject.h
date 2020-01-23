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

#ifndef __RNS_OBJECT__
#define __RNS_OBJECT__

/* ������������������������������������������������

	RNS��b�N���X���C�u���� 
 	
 	���e : 
 	���� : 
 
 	���� :	Fukuta.K

������������������������������������������������ */

/* ------------------------------------------------------------
	�N���X��`
------------------------------------------------------------ */

/**
 * �Q�ƃJ�E���^�𓋍ڂ����A�S�Ă�RNS�N���X�V���[�Y�̊��N���X
 *
 * �I�u�W�F�N�g���Q�Ƃ���Ă���������L�����A�Q�Ƃ��S�ĉ����ꂽ���_�Ŏ���� delete ����B
 *
 * - new �ɂ�萶�����ꂽ�I�u�W�F�N�g�̎Q�ƃJ�E���^�� 0 �������Ă���
 * - retain ���\�b�h�����s���邱�ƂŁA�Q�ƃJ�E���^�͂P������
 * - release ���\�b�h�����s���邱�ƂŁA�Q�ƃJ�E���^�͂P����
 * - �Q�ƃJ�E���^���P�ȉ��̂Ƃ��� release ���\�b�h�����s����ƁA�I�u�W�F�N�g�� delete �����
 *
 * �����̋@�\�����p���邽�߁ARNS�N���X�ɂ͎��̂悤�ȃ��[�������݂���
 *
 * - new �Ő������ꂽRNS�I�u�W�F�N�g���u�I�[�i�[�V�b�v���[�������I�u�W�F�N�g�v�ƌĂ�
 * - �����ϐ��Ƃ��Đ������ꂽ�I�u�W�F�N�g���u�I�[�i�[�V�b�v���[���������Ȃ��I�u�W�F�N�g�v�ƌĂ�
 * - �I�[�i�[�V�b�v�Ƃ́u���̃I�u�W�F�N�g��ێ����錠���v�ł���\n
 *	 ����͌���������΁u�I�u�W�F�N�g�𓮓I��delete�ł���v���Ƃ�����
 * - �I�[�i�[�V�b�v�����I�u�W�F�N�g�́ARNSPtr�Ɋi�[���邱�ƂŃK�x�[�W�R���N�g����������
 * - �I�[�i�[�V�b�v�������Ȃ��I�u�W�F�N�g�́ARNSPtr�Ɋi�[���Ă͂Ȃ�Ȃ�\n
 *   ����́u�����ϐ���delete�������邱�ƂɂȂ�v����ł���
 * - �I�[�i�[�V�b�v�����I�u�W�F�N�g�ւ̃|�C���^���A���̃I�u�W�F�N�g�̃I�[�i�[�ƌĂ�
 */

class RNSObject {
	mutable int __retain_counter__;	///< �Q�ƃJ�E���^

 public:
	/* ----- ������ ----- */
	/// �f�t�H���g�R���X�g���N�^
	RNSObject()
	{
		__retain_counter__ = 0;
	}

	/// �f�X�g���N�^
	virtual ~RNSObject()
	{
	}

	/* ----- ���� ----- */
	/// �Q�ƃJ�E���^���P���₷
	void retain() const
	{
		++__retain_counter__;
	}

	/// �Q�ƃJ�E���^���P���炵�A�Q�Ƃ���Ȃ��Ȃ�����������
	void release() const
	{
		if( --__retain_counter__ <= 0) {
			delete this;
		}
	}

	/// �Q�ƃJ�E���^���P���炷
	void dummyRelease() const {	--__retain_counter__; }

	/// �Q�ƃJ�E���g���擾����
	int retainCount() const
	{
		return __retain_counter__;
	}
};

/**
 * RNS�N���X�I�u�W�F�N�g�|�C���^�ێ��N���X
 *
 * RNS�N���X�I�u�W�F�N�g�̎Q�ƃJ�E���^�̑���������ōs��
 *
 * - RNS�N���X�I�u�W�F�N�g�̃|�C���^���󂯂�ƁA���̃I�u�W�F�N�g�� retain ���ď�������
 * - �I�u�W�F�N�g���������Ă���Ƃ��ɁA�ʂ̃|�C���^���󂯂��<BR>
 *   ���݂� �I�u�W�F�N�g �� release ���A�V�����󂯂� �I�u�W�F�N�g �� retain ����
 * - RNSPtr ���j�������Ƃ��A������ �I�u�W�F�N�g �� release �����
 */
template<typename T>
class _RNSPtr {
public:
	// ----- ������ ----- //
	/// �R���X�g���N�^
	_RNSPtr()
	{
		mObj = 0;
	}

	/// �R���X�g���N�^
	_RNSPtr(T* iObj)
	{
		mObj = 0;
		setObj(iObj);
	}

	/// �R���X�g���N�^
	_RNSPtr(const _RNSPtr<T>& iPtr)
	{
		mObj = 0;
		setObj(const_cast<T*>(iPtr.obj()));
	}

	/// �f�X�g���N�^
	~_RNSPtr()
	{
		release();
	}

	// ----- �ݒ� ----- //
	/// �I�u�W�F�N�g��ݒ肷��
	void setObj(T* iObj)
	{
		release();
		mObj = iObj;
		retain();
	}

	// ----- �擾 ----- //
	/// �I�u�W�F�N�g���擾
	T* obj() const					{ return mObj; }

	// ----- �I�y���[�^ ----- //

	/// �Q�Ǝ擾
	T& operator * ()				{ return *mObj; }
	/// �Q�Ǝ擾�iconst�j
	const T& operator * () const	{ return *mObj;	}

	/// �|�C���^�擾
	operator T* ()					{ return mObj; }
	/// �|�C���^�擾�iconst�j
	operator const T* () const		{ return mObj; }

	/// ���
	T* operator -> ()				{ return &(operator*()); }
	/// ���iconst�j
	const T* operator -> () const	{ return &(operator*()); }

	/// ��r���Z�q�i�����j
	bool operator == (const T* iTgt) const	{ return mObj == iTgt; }
	/// ��r���Z�q�i�s�����j
	bool operator != (const T* iTgt) const	{ return !(mObj == iTgt); }

protected:
	/// �ێ�����
	void retain() const {
		if( mObj ) {
			mObj->retain();
		}
	}

	/// �������
	void release() {
		if( mObj ) {
			const T* tmpObj = mObj;
			mObj = 0;
			tmpObj->release();
		}
	}

private:
	T* mObj;
};

template<typename T>
class RNSPtr : public _RNSPtr<T> {
public:
	/* --- �R���X�g���N�^�E�f�X�g���N�^ --- */
	/// �f�t�H���g�R���X�g���N�^
	RNSPtr() {}

	/// �R�s�[�R���X�g���N�^
	RNSPtr(RNSPtr<T>& iPtr) : _RNSPtr<T>(iPtr) {}

	/// �R���X�g���N�^�i�I�u�W�F�N�g����j
	RNSPtr(T* iObj) : _RNSPtr<T>(iObj) {}

	template<typename T2> 
	RNSPtr(const RNSPtr<T2>& iPtr) : _RNSPtr<T>(iPtr.obj()) {}

	/// �|�C���^���
	RNSPtr<T>& operator = (T* iPtr)
	{
		setObj(iPtr);
		return *this;
	}

	RNSPtr<T>& operator = (const RNSPtr<T>& iPtr) 
	{
		setObj(iPtr.obj());
		return *this;
	}

	bool operator < (const RNSPtr<T>& iPtr) const	{ return obj() < iPtr.obj(); }
};

#define	_RNSPTR_TYPEDEF(Type)	\
	typedef	RNSPtr<Type>		\
		Type ## Ptr;

_RNSPTR_TYPEDEF(RNSObject);

/**
 * RNS�N���X�I�u�W�F�N�g�Q�ƃN���X
 *
 * RNSPtr�Ɠ��l�ɁA���I��RNSObject�̃|�C���^��ێ�����B\n
 * ����_�́ARNSPtr�̔�r���Z���|�C���^�������A�h���X�̔�r�ł���̂ɑ΂��ARNSRef�͎��̂̔�r���s���B\n
 * set��map�Ɋi�[����ꍇ�͂�����̕����֗��B
 */
template <typename T>
class RNSRef : public _RNSPtr<T> {
public:
	/* --- �R���X�g���N�^�E�f�X�g���N�^ --- */
	/// �f�t�H���g�R���X�g���N�^
	RNSRef() {}

	/// �R�s�[�R���X�g���N�^
	RNSRef(RNSRef<T>& iRef) : _RNSPtr<T>(iRef) {}

	/// �R���X�g���N�^�i�I�u�W�F�N�g����j
	RNSRef(T* iObj) : _RNSPtr<T>(iObj) {}

	template<typename T2> 
	RNSRef(const RNSRef<T2>& iRef) : _RNSPtr<T>(iRef.obj()) {}

	RNSRef<T>& operator = (T* iRef)
	{
		setObj(iRef);
		return *this;
	}

	RNSRef<T>& operator = (RNSRef<T>& iRef) 
	{
		setObj(iRef.obj());
		return *this;
	}
	
	template <typename T2>
	RNSRef<T>& operator = (const RNSRef<T2>& iRef)
	{
		setObj(iRef.obj());
		return *this;
	}

	// --- �I�y���[�^ --- //
	bool operator < (const T& iRef) const { return **this < *iRef; }
	bool operator > (const T& iRef) const { return **this > *iRef; }
	bool operator == (const T& iRef) const { return **this == *iRef; }
	bool operator != (const T& iRef) const { return **this != *iRef; }

	bool operator < (const _RNSPtr<T>& iRef) const { return **this < *iRef; }
	bool operator > (const _RNSPtr<T>& iRef) const { return **this > *iRef; }
	bool operator == (const _RNSPtr<T>& iRef) const { return **this == *iRef; }
	bool operator != (const _RNSPtr<T>& iRef) const { return **this != *iRef; }
};

#define _RNSREF_TYPEDEF(Type)	\
	typedef RNSRef<Type>		\
		Type ## Ref;			

#define _RNSOBJECT_TYPEDEF(Type)	\
	_RNSPTR_TYPEDEF(Type);	\
	_RNSREF_TYPEDEF(Type)

_RNSOBJECT_TYPEDEF(RNSObject);

/**
 * RNS�N���X�I�u�W�F�N�g�i�����ϐ��j�ی�N���X
 *
 * 	�����ϐ��������release����Ă��܂������ȂƂ��͂���ɂ��܂��Ă���<BR>
 *	�������retain�A�j������dummyRelease�i�Q�ƃJ�E���^��0���w���Ă�delete���Ȃ�release�j���s��
 */

template<typename T>
class _RNSRetainPtr {
	mutable const T* mObj;
	
 public:
	/* --- �R���X�g���N�^�E�f�X�g���N�^ --- */
	/// �R���X�g���N�^
	_RNSRetainPtr(const T* iObj)
		{ mObj = iObj; mObj->retain(); }
	/// �f�X�g���N�^
	~_RNSRetainPtr()
		{ mObj->dummyRelease(); }
};

typedef _RNSRetainPtr<RNSObject>		RNSRetainPtr;

#define RNSReleaseProtect(e)	RNSRetainPtr __protecter__(&e)

#endif /* __RNS_OBJECT__ */
