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
 * @file delete_iterator.h
 * @brief for iterator.h
 */

#ifndef ARCTL_DELETE_ITERATOR_H_
#define ARCTL_DELETE_ITERATOR_H_

#include <boost/function_output_iterator.hpp>
#include <arctl/object_massacrer.h>

namespace arctl {

template <typename Class>
class delete_iterator : public boost::function_output_iterator<object_massacrer<Class> >
{
	typedef boost::function_output_iterator<object_massacrer<Class> > super;
public:
	delete_iterator(Class* table, LPCTSTR field_name) :
	super(object_massacrer<Class>(table, field_name))
	{
	}

	/**
	 * �I�u�W�F�N�g���j�󂳂��O�� object_massacrer::flush() �����s����B<br>
	 * ���̍ۂɔ���������O�͎̂Ă���B
	 */
	~delete_iterator()
	{
		try {
			(**this).m_f.flush();
		}
		catch(...) {}
	}
};

/**
 * @brief delete_iterator �\�z�����֐��B
 *
 * @param rTable [in] �폜�Ώۃe�[�u���B
 * @param lpszFieldName [in] �����t�B�[���h���́B
 *
 * @test @ref unit_test DeleteIteratorTest
 */
template <typename TClass>
delete_iterator<typename class_traits<TClass>::interface_type> deleter(TClass const& rTable, LPCTSTR lpszFieldName)
{
	return
#if _MSC_VER <= 1200
		delete_iterator<class_traits<TClass>::interface_type>(rTable, lpszFieldName);
#else
		delete_iterator<typename class_traits<TClass>::interface_type>(rTable, lpszFieldName);
#endif // _MSC_VER <= 1200
}

} // namespace arctl

#endif // ARCTL_DELETE_ITERATOR_H_
