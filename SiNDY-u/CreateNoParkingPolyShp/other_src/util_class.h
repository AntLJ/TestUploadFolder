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
		Unicode		= 0x01,	///< SiNDY��̃f�[�^�̏ꍇ
		Sjis		= 0x02	///< PGDB��̃f�[�^�̏ꍇ
	};

	/**
		�f�B���N�g���쐬
		���f�B���N�g���̋�؂��'\'or'/'��Ώ�
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
			// �p�X�̃J�b�g
			char*	a_cpCutPath = NULL;
			if( i == 0 ) {
				a_cpStartPt	= a_cpDNStr;
			}else {
				a_cpStartPt	= a_cpEndPt+1;
			}
			a_cpEndPt	= strchr ( a_cpStartPt, '\\' );
			if( a_cpEndPt == NULL ) {
				// �J�b�g�����Ō�̕������ǉ�
				strcat ( a_cpTmpStr, a_cpStartPt );
				// �������݁A�ǂݍ��݉\�����ׂ�
				if( _access( a_cpTmpStr, 6 ) == -1 ) {
					if( _mkdir ( a_cpTmpStr ) == -1 ) {
						a_iFailFlag	= 1;
						break;
					}
				}
				break;
			}
			if( i == 0 ) {
				// �ŏ���"\\"�̏ꍇ�͎��܂Ō���
				if( (char)(*(a_cpEndPt+1)) == '\\' ) {
					a_cpEndPt	= strchr ( a_cpEndPt+2, '\\' );
					// �ŏ��̓}�V�����Ȃ̂ŁA���̃t�H���_�܂ł݂Ȃ��Ƃ���
					a_cpEndPt	= strchr ( a_cpEndPt+1, '\\' );
					if( a_cpEndPt == NULL ) {	// ������NULL�ɂȂ�Ƃ������Ƃ̓t�H���_�����ԈႢ
						delete [] a_cpDNStr;
						delete [] a_cpTmpStr;
						return	( false );
					}
				}
			}
			// �I�_��1�o�C�g�O��0x82,0x83�Ȃ�ASJIS�̕���
			if( (u_char)(*(a_cpEndPt-1)) == 0x82 || (u_char)(*(a_cpEndPt-1)) == 0x83 ) {
				a_cpEndPt	= strchr ( a_cpEndPt+1, '\\' );
			}

			if( a_cpEndPt != NULL ) {
				strncat ( a_cpTmpStr, a_cpStartPt, (a_cpEndPt-a_cpStartPt));
				strcat	( a_cpTmpStr, "\\" );
				// �������݁A�ǂݍ��݉\�����ׂ�
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

