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

#ifndef __SINDY__GET_VALUE_FUNC_H__
#ifdef __SINDY_ROAD_ATTR_COMMUNITY__
#ifdef __SINDY_FIELDVALUE_GETTER_H__
#define __SINDY__GET_VALUE_FUNC_H__

namespace sindy {

template <LPCTSTR _FieldNames>
struct _GetValueFunc
{

};

#define _SINDY_VALUESFUNCTION_DEFINE(name, count) \
template <> \
struct _GetValueFunc<name##0> \
{ \
	template <typename _Row, typename _OutputType> \
	static HRESULT getValue(unsigned int n, _Row const& rRow, _OutputType& rOutput) \
	{ \
		long nFieldIndex; \
		switch(n) \
		{ \
		_ROOP_TO_1_##count(_SINDY_VALUESFUNCTION_CASTCASE, name) \
		default: return E_FAIL; \
		} \
		return sindy::getValue(rRow, nFieldIndex, rOutput); \
	} \
};

#define _ROOP_TO_1_1(macro, data) macro(1, data)
#define _ROOP_TO_1_2(macro, data) macro(2, data) _ROOP_TO_1_1(macro, data)
#define _ROOP_TO_1_3(macro, data) macro(3, data) _ROOP_TO_1_2(macro, data)
#define _ROOP_TO_1_4(macro, data) macro(4, data) _ROOP_TO_1_3(macro, data)
#define _ROOP_TO_1_5(macro, data) macro(5, data) _ROOP_TO_1_4(macro, data)
#define _ROOP_TO_1_6(macro, data) macro(6, data) _ROOP_TO_1_5(macro, data)
#define _ROOP_TO_1_7(macro, data) macro(7, data) _ROOP_TO_1_6(macro, data)
#define _ROOP_TO_1_8(macro, data) macro(8, data) _ROOP_TO_1_7(macro, data)
#define _ROOP_TO_1_9(macro, data) macro(9, data) _ROOP_TO_1_8(macro, data)
#define _ROOP_TO_1_10(macro, data) macro(10, data) _ROOP_TO_1_9(macro, data)

#define _SINDY_VALUESFUNCTION_CASTCASE(num, data) case num: nFieldIndex = sindy::fieldindex_cast<data ## num>(rRow); break;

_SINDY_VALUESFUNCTION_DEFINE(sindy::inf_turnreg::kStartMonth, 5)
_SINDY_VALUESFUNCTION_DEFINE(sindy::inf_turnreg::kStartDay, 5)
_SINDY_VALUESFUNCTION_DEFINE(sindy::inf_turnreg::kStartHour, 5)
_SINDY_VALUESFUNCTION_DEFINE(sindy::inf_turnreg::kStartMin, 5)
_SINDY_VALUESFUNCTION_DEFINE(sindy::inf_turnreg::kEndMonth, 5)
_SINDY_VALUESFUNCTION_DEFINE(sindy::inf_turnreg::kEndDay, 5)
_SINDY_VALUESFUNCTION_DEFINE(sindy::inf_turnreg::kEndHour, 5)
_SINDY_VALUESFUNCTION_DEFINE(sindy::inf_turnreg::kEndMin, 5)
_SINDY_VALUESFUNCTION_DEFINE(sindy::inf_turnreg::kDayOfWeek, 5)

_SINDY_VALUESFUNCTION_DEFINE(sindy::inf_dirguide::kGuideClass, 10)
_SINDY_VALUESFUNCTION_DEFINE(sindy::inf_dirguide::kNameKanji, 10)
_SINDY_VALUESFUNCTION_DEFINE(sindy::inf_dirguide::kNameYomi, 10)

_SINDY_VALUESFUNCTION_DEFINE(sindy::highway_text::kForeId, 5)
_SINDY_VALUESFUNCTION_DEFINE(sindy::highway_text::kBackId, 5)

} // namespace sindy

#endif // __SINDY_FIELDVALUE_GETTER_H__
#endif // __SINDY_ROAD_ATTR_COMMUNITY__
#endif // __SINDY__GET_VALUE_FUNC_H__
