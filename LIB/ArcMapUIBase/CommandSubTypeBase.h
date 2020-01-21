/*
 Copyright 1995-2004 ESRI

 All rights reserved under the copyright laws of the United States.

 You may freely redistribute and use this sample code, with or without modification.

 Disclaimer: THE SAMPLE CODE IS PROVIDED "AS IS" AND ANY EXPRESS OR IMPLIED 
 WARRANTIES, INCLUDING THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS 
 FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL ESRI OR 
 CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, 
 OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF 
 SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS 
 INTERRUPTION) SUSTAINED BY YOU OR A THIRD PARTY, HOWEVER CAUSED AND ON ANY 
 THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT ARISING IN ANY 
 WAY OUT OF THE USE OF THIS SAMPLE CODE, EVEN IF ADVISED OF THE POSSIBILITY OF 
 SUCH DAMAGE.

 For additional information contact: Environmental Systems Research Institute, Inc.

 Attn: Contracts Dept.

 380 New York Street

 Redlands, California, U.S.A. 92373 

 Email: contracts@esri.com
*/
// MapCommandBase.h: interface for the CMapCommandBase class.
//
//////////////////////////////////////////////////////////////////////

#ifndef __COMMANDSUBTYPE_H_
#define __COMMANDSUBTYPE_H_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "CommandBase.h"
// 
// Provides basic implementation of a command that requires a focus map
// Inherit this into your command object, then override any required functions
//
// Default behaviour
//   Enabled - True if there is a focus map
//   Checked - False
//   Name    - Combination of Caption and category
//   Caption - supplied from constructor
//   Tooltip - defaults to Caption if not supplied
//   Message - defaults to Caption is not supplied
//   OnCreate - Initialises hook
//   all others just return S_OK
//
// The constructor will initialise the base class with bitmaps and
// strings specified by resource ID's
//
struct SINDY_SUBTYPE_ENTRY {
	UINT bitmapID;
	_U_STRINGorID caption;
	_U_STRINGorID name;
	_U_STRINGorID category;
	_U_STRINGorID tooltip;
	_U_STRINGorID message;
	_U_STRINGorID helpfile;
	UINT helpID;
};

#define BEGIN_SUBTYPE_MAP(x) \
	static const struct SINDY_SUBTYPE_ENTRY* GetSubTypeMap() {\
	static const struct SINDY_SUBTYPE_ENTRY _Info[] = {
#define SUBTYPE_ENTRY( bitmapID, caption, name, category, tooltip, message, helpfile, helpID ) { \
	bitmapID, caption, name, category, tooltip, message, helpfile, helpID },
#define END_SUBTYPE_MAP() \
	{ 0, 0U, 0U, 0U, 0U, 0U, 0U, 0U } };\
	 return &_Info[0]; }

template<class T, class TBase>
class CCommandSubTypeBase : public ICommandSubType, public T
{
public:
	CCommandSubTypeBase(LONG count = 0) :
		m_lCount(count)
	{
		  SetSubType(1);	// デフォルトのサブタイプは1
	}
	virtual ~CCommandSubTypeBase()
	{
		T::Clear();
	}
public:
// ICommandSubType
	STDMETHOD(GetCount)(long * Count)
	{
		if( ! Count )
			return E_POINTER;
		*Count = m_lCount;

		return S_OK;
	}
	STDMETHOD(SetSubType)(long SubType)
	{
		if( 0 >= SubType || m_lCount < SubType )
			return E_INVALIDARG;
		
		m_lSubType = SubType;
		
		T::Clear();

		const SINDY_SUBTYPE_ENTRY* pInfo = TBase::GetSubTypeMap();
		long idx = m_lSubType - 1;

		T::SetInfo( TBase::GetSubTypeMap()[idx].bitmapID, 
			pInfo[idx].caption,
			pInfo[idx].name,
			pInfo[idx].category,
			pInfo[idx].tooltip,
			pInfo[idx].message,
			pInfo[idx].helpfile,
			pInfo[idx].helpID );

		return S_OK;
	}
protected:
	long			m_lCount;
	long			m_lSubType;
};

#endif // ifndef __COMMANDSUBTYPE_H_
