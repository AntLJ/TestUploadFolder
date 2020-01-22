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

/*
** TSmartPointer.h �Q�ƃJ�E���g���������|�C���^
**   Programmed by Takashi Togawa
**   EDITION: 2001/10/12 �쐬
**            2001/10/15 delete�̒��ۉ�
**            2002/02/06 setNULL�̒ǉ�
**
*/

#ifndef ___T_SMART_POINTER_H___
#define ___T_SMART_POINTER_H___

#define _TSP_NULL 0
#define _TSP_ZERO (Count)0

/*
** �C���N���[�h�t�@�C��
*/

#include <functional>

/*!
  delete�̒��ۉ�
*/

template <class T>
struct _tsp_delete : std::unary_function<T, void>
{
	void operator()(T* cPointer) const
		{ delete cPointer; }
};

template <class T>
struct _tsp_delete_n : std::unary_function<T, void>
{
	void operator()(T* cPointer) const
		{ delete [] cPointer; }
};

/*!
  �X�}�[�g�|�C���^
  delete �͒��ۉ����Ă��邯�ǁA++Count --Count�͒��ۉ����Ă��Ȃ��B
*/

template <class T, class Count = int, class Destruct = _tsp_delete<T> >
class TSmartPointer
{
public:
	// T* T& �� typedef
	typedef T* pointer;
	typedef T& reference;

	//! �X�}�[�g�|�C���^�̃A�C�e���{�b�N�X
	class tsp_box
	{
		Count mCount;       //! �J�E���^
		pointer mSubstance; //! ���̂ւ̃|�C���^
	public:
		//! �f�t�H���g�R���X�g���N�^
		tsp_box() : mCount(_TSP_ZERO), mSubstance(_TSP_NULL) {}
		//! ���̎w��R���X�g���N�^
		/*!
		  @param cSubstance [in] ���̂ւ̃|�C���^
		*/
		tsp_box(pointer cSubstance) : mCount(_TSP_ZERO), mSubstance(cSubstance) {}

		//! �f�X�g���N�^
		~tsp_box() { Destruct()(mSubstance); }

		//! �|�C���^��NULL�Ɏw��
		void setNULL() { mSubstance = _TSP_NULL; }
		//! �J�E���^�P���Z
		void increment() { ++mCount; }
		//! �J�E���^�P���Z
		void decrement()
		{
			if(mCount > _TSP_ZERO)
			{
				--mCount; //�J�E���^���Z
				if(mCount <= _TSP_ZERO)
				{
					Destruct()(mSubstance); // ���̏���
					mSubstance = _TSP_NULL;
				}
			}
		}
		//! �|�C���^�̎擾
		pointer getSubstance() { return mSubstance; }
		//! �J�E���^�̎擾
		Count getCount() const { return mCount; }
		//! �J�E���^�͂O���H
		bool isZero() const { return mCount == _TSP_ZERO; }
		//! void�|�C���^�̎擾
		void* getPointer() const { return (void*)mSubstance; }
	};
	typedef tsp_box box;

private:
	box* mBox; //! �A�C�e���{�b�N�X�ւ̃|�C���^

public:
	//! �f�t�H���g�R���X�g���N�^
	TSmartPointer() : mBox(_TSP_NULL) {}
	//! ���̎w��R���X�g���N�^
	/*!
	  @param cSubstance [in] ���̂ւ̃|�C���^
	*/
	TSmartPointer(pointer cSubstance) : mBox(_TSP_NULL)
	{
		if(cSubstance)
		{
			mBox = new box(cSubstance);
			if(mBox) mBox->increment();
		}
	}
	//! �A�C�e���w��R���X�g���N�^
	/*!
	  @param cBox [in] �A�C�e���ւ̃|�C���^
	*/
	TSmartPointer(box* cBox) : mBox(cBox)
		{ if(mBox) mBox->increment(); }
	//! �R�s�[�R���X�g���N�^
	/*!
	  @param cSmartPointer [in] �R�s�[����X�}�[�g�|�C���^
	*/
	TSmartPointer(const TSmartPointer& cSmartPointer) :
		mBox(cSmartPointer.mBox)
		{ if(mBox) mBox->increment(); }
	//! �f�X�g���N�^
	~TSmartPointer()
	{
		if(mBox)
		{
			mBox->decrement();
			if(mBox->isZero()) _tsp_delete<box>()(mBox);
		}
	}

	//! �|�C���^��NULL�ɂ���
	void setNULL()
	{
		if(mBox)
		{
			mBox->setNULL();
			delete mBox;
			mBox = _TSP_NULL;
		}
	}

	//! = (���̃|�C���^�̑��)
	/*!
	  @param cSubstance [in] ���̂ւ̃|�C���^
	  @return �����̎Q��
	*/
	TSmartPointer& operator = (pointer cSubstance)
	{
		if(mBox)
		{
			mBox->decrement();
			if(mBox->isZero()) _tsp_delete<box>()(mBox);
		}
		if(cSubstance != _TSP_NULL)
		{
			mBox = new box(cSubstance);
			if(mBox) mBox->increment();
		}
		else
		{
			mBox = NULL;
		}
		return *this;
	}
	//! = (�X�}�[�g�|�C���^�̑��)
	/*!
	  @param cSmartPointer [in] �R�s�[����X�}�[�g�|�C���^
	  @return �����̎Q��
	*/
	TSmartPointer& operator = (const TSmartPointer& cSmartPointer)
	{
		if(mBox)
		{
			mBox->decrement();
			if(mBox->isZero()) _tsp_delete<box>()(mBox);
		}
		mBox = cSmartPointer.mBox;
		if(mBox) mBox->increment();
		return *this;
	}

	//! �|�C���^�̎擾
	T* getPointer()
	{
		return mBox ? mBox->getSubstance() : (T*)_TSP_NULL;
	}
	const T* getConstPointer() const
	{
		return mBox ? mBox->getSubstance() : (T*)_TSP_NULL;
	}
	// 2001/12/25 �R�����g�� 
	// �|�C���^�̎擾(�L���X�g�͍��Ȃ����������炵��)
	//operator pointer ()
	//{
	//	if(mBox) return mBox->getSubstance();
	//	return (pointer)_TSP_NULL;
	//}
	//! �Q�Ƃ̎擾
	// operator reference () { return *(pointer)*this; }
	//! *���Z�q
	T& operator * () { return *getPointer(); }
	const T& operator * () const { return *getConstPointer(); }
	//! ->���Z�q
	T* operator -> () { return getPointer(); }
	const T* operator -> () const { return getConstPointer(); }

	//! �Q�ƃJ�E���^�̎擾
	Count getCount() const
	{
		if(mBox) return mBox->getCount();
		return _TSP_ZERO;
	}
	//! �Q�ƃA�C�e���̎擾
	box* getBox()
	{
		if(mBox) return mBox;
		return _TSP_NULL;
	}
	//! �X���b�v
	void swap(TSmartPointer& cSmartPointer)
	{
		if(mBox != cSmartPointer.mBox)
		{
			box* aTemp = mBox;
			mBox = cSmartPointer.mBox;
			cSmartPointer.mBox = aTemp;
		}
	}

	//! void*�ւ̃L���X�g
	//operator void* () const
	//{
	//	if(mBox) return mBox->getPointer();
	//	return _TSP_NULL;
	//}
	//! bool�ւ̃L���X�g
	operator bool () const
	{
		return ((mBox ? mBox->getPointer() : NULL) != NULL);
	}

	//! == ���Z�q
	bool operator == (TSmartPointer& cSmartPointer)
		{ return getPointer() == cSmartPointer.getPointer(); }
	//! != ���Z�q
	bool operator != (TSmartPointer& cSmartPointer)
		{ return getPointer() != cSmartPointer.getPointer(); }
	//! < ���Z�q
	bool operator < (TSmartPointer& cSmartPointer)
		{ return this < &cSmartPointer; }
	//! > ���Z�q
	bool operator > (TSmartPointer& cSmartPointer)
		{ return this > &cSmartPointer; }
	//! <= ���Z�q
	bool operator <= (TSmartPointer& cSmartPointer)
		{ return this <= &cSmartPointer; }
	//! >= ���Z�q
	bool operator >= (TSmartPointer& cSmartPointer)
		{ return this >= &cSmartPointer; }
};

#endif // ___T_SMART_POINTER_H___
