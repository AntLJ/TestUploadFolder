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
 * @file Misc.h
 * @brief ������̒�`�B
 */
#ifndef MISC_H_
#define MISC_H_

#include <cppunit/extensions/HelperMacros.h>
#pragma warning( push )
#pragma warning( disable : 4284 )
#include <boost/optional.hpp>
#pragma warning( pop )

/**
 * @brief ���ؗpIUnknown�B
 *
 * AddRef() �� Release() ���������s���邩�����؂���B
 */
class DummyUnknown : public IUnknown
{
public:
///@name �R���X�g���N�^�ƃf�X�g���N�^
//@{
	DummyUnknown() :
	count(0UL)
	{
	}

	~DummyUnknown()
	{
		CPPUNIT_ASSERT(! count);
	}
//@}

///@name IUnknown�̒�`
//@{

	ULONG __stdcall AddRef()
	{
		return ++*count;
	}

	ULONG __stdcall Release()
	{
		CPPUNIT_ASSERT(count);

		unsigned long nCount = --*count;

		if(nCount == 0) {
			count = boost::optional<ULONG>();
		}

		return nCount;
	}

	HRESULT __stdcall QueryInterface(REFIID riid, void** ppvObject)
	{
		if(IsEqualIID(riid, IID_IUnknown)) {
			*ppvObject = static_cast<IUnknown*>(this);
			AddRef();
		}
		else {
			*ppvObject = 0;
			return E_NOINTERFACE;
		}

		return S_OK;
	}
//@}

private:
// �ϐ�
	boost::optional<ULONG> count;
};

#endif // MISC_H_
