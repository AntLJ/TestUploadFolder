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

#ifndef ATL2_TYPE_TRAITS_IS_COM_PTR_T_H_
#define ATL2_TYPE_TRAITS_IS_COM_PTR_T_H_

#include <comip.h>

namespace atl2 {

/**
 * @brief スマートポインタ識別クラス
 */
template<typename T>
struct is_com_ptr_t
{
	typedef char yestype;
	typedef struct { char dummy[2]; } notype;
	
	template <typename TIIID> inline static yestype tester(const volatile _com_ptr_t<TIIID>&);
	inline static notype tester(...);
	
	inline static T& make_t();
	enum { value = sizeof(tester(make_t()))==sizeof(yestype) };
};

} // namespace atl2

#endif // ATL2_TYPE_TRAITS_IS_COM_PTR_T_H_
