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

#pragma once

#include <iostream>
#include <time.h>
#include <locale.h>
#include <list>
#include <set>

using namespace std;

class timer{

public:
    timer() : begin(0), end(0) { start(); _tsetlocale ( LC_ALL, _T("") ); }
	timer( LPCTSTR strOutput ){ strText = strOutput; start();}
    ~timer() { trace(); }                            
    void start() { begin = clock(); }
    void stop() { end = clock(); }
    void trace( bool Restart = false){
		_tprintf( _T("%sÅF%lf [s]\n"), strText, ( ( clock() - begin ) /  (float)CLOCKS_PER_SEC ) );
		if( Restart )
			start();
    }
	CString Gethms(){
		double t = ( ( clock() - begin ) /  (float)CLOCKS_PER_SEC );
		CString strTime;
		strTime.Format( _T("%02d:%02d:%02d"), (int)t/3600, ((int)t%3600)/60, ((int)t%3600)%60 );
		return strTime;
	}

private:
	CString strText;
    clock_t begin, end;
};

class CComInit{
public:
	CComInit(){ ::CoInitialize(NULL); }
	~CComInit(){ ::CoUninitialize(); }
};
