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

#include "stdafx.h"
#include <stdio.h>
#include <math.h>
#include <iostream>
#include <stdlib.h>
#include <vector>
#include <list>
#include <set>
#include <io.h>
#include <direct.h>

#ifndef	_UTIL_CLASS_H_
#define	_UTIL_CLASS_H_

using	namespace	std;	// 2007.09.04

class	util_class
{
public:
	enum	SRC_STR_MODE {
		Unicode		= 0x01,	///< SiNDY上のデータの場合
		Sjis		= 0x02	///< PGDB上のデータの場合
	};

	/**
		ディレクトリ作成
		※ディレクトリの区切りは'\'or'/'を対象
	*/
	bool	makedir ( char*	c_cpDirName ) {
		char*	a_cpCutStr = "\\";
		char*	a_cpDNStr = new char[strlen(c_cpDirName)+1];
		char*	a_cpTmpStr = new char[strlen(c_cpDirName)+1];

		memset	( a_cpDNStr, '\0', strlen(c_cpDirName)+1 );
		memset	( a_cpTmpStr, '\0', strlen(c_cpDirName)+1 );
		sprintf	( a_cpDNStr, "%s", c_cpDirName );

		char*	a_cpStartPt	= NULL;
		char*	a_cpEndPt	= NULL;
		//---------------------------------------------------------
	//	if( CreateDirectory ( c_cpDirName, NULL ) == 0 ) {
	//		return ( false );
	//	}
		//---------------------------------------------------------
		int		a_iFailFlag = 0;
		int i = 0;
		for(i = 0;; i++)
		{
			// パスのカット
			char*	a_cpCutPath = NULL;
			if( i == 0 ) {
				a_cpStartPt	= a_cpDNStr;
			}else {
				a_cpStartPt	= a_cpEndPt+1;
			}
			a_cpEndPt	= strchr ( a_cpStartPt, '\\' );
			if( a_cpEndPt == NULL ) {
				// カットした最後の文字列を追加
				strcat ( a_cpTmpStr, a_cpStartPt );
				// 書き込み、読み込み可能か調べる
				if( _access( a_cpTmpStr, 6 ) == -1 ) {
					if( _mkdir ( a_cpTmpStr ) == -1 ) {
						a_iFailFlag	= 1;
						break;
					}
				}
				break;
			}
			if( i == 0 ) {
				// 最初が"\\"の場合は次まで見る
				if( (char)(*(a_cpEndPt+1)) == '\\' ) {
					a_cpEndPt	= strchr ( a_cpEndPt+2, '\\' );
					// 最初はマシン名なので、次のフォルダまでみないとだめ
					a_cpEndPt	= strchr ( a_cpEndPt+1, '\\' );
					if( a_cpEndPt == NULL ) {	// ここでNULLになるということはフォルダ名が間違い
						delete [] a_cpDNStr;
						delete [] a_cpTmpStr;
						return	( false );
					}
				}
			}
			// 終点の1バイト前が0x82,0x83なら、SJISの文字
			if( (u_char)(*(a_cpEndPt-1)) == 0x82 || (u_char)(*(a_cpEndPt-1)) == 0x83 ) {
				a_cpEndPt	= strchr ( a_cpEndPt+1, '\\' );
			}

			if( a_cpEndPt != NULL ) {
				strncat ( a_cpTmpStr, a_cpStartPt, (a_cpEndPt-a_cpStartPt));
				strcat	( a_cpTmpStr, "\\" );
				// 書き込み、読み込み可能か調べる
				if( _access( a_cpTmpStr, 6 ) == -1 ) {
					if( _mkdir ( a_cpTmpStr ) == -1 ) {
						a_iFailFlag	= 1;
						break;
					}
				}
			}else {
				break;
			}
		}
		delete [] a_cpDNStr;
		delete [] a_cpTmpStr;
		if( a_iFailFlag == 1 ) {
			return ( false );
		}else {
			return ( true );
		}
	}

};

#endif

