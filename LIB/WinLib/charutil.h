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
 * @file charutil.h
 * @brief <b>�����񑀍샆�[�e�B���e�B�[</b>\n
 * @author �n�}�c�a���암�J���O���[�v �Ð�M�G
 * @version $Id$
 */
#ifndef _CHARUTIL_H_
#define _CHARUTIL_H_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/// ASCII�R�[�h�̔���
#define isasciikatakana(c)				( 0xa6 <= (unsigned char)((c)&0xff) && (unsigned char)(c&0xff) <= 0xdf )
//{ ADD 02.11.21 by hkakeno
#define isasciinumber(c)				( 0x30 <= (unsigned char)((c)&0xff) && (unsigned char)(c&0xff) <= 0x39 )
#define isasciialphabet_large(c)		( 0x41 <= (unsigned char)((c)&0xff) && (unsigned char)(c&0xff) <= 0x5a )
#define isasciialphabet_small(c)		( 0x61 <= (unsigned char)((c)&0xff) && (unsigned char)(c&0xff) <= 0x7a )
#define isasciiminus(c)					( 0x2d == (unsigned char)((c)&0xff) )
#define isasciiperiod(c)				( 0x2e == (unsigned char)((c)&0xff) )
#define isasciislash(c)                 ( 0x2f == (unsigned char)((c)&0xff) )
#define isasciizero(c)					( 0x30 == (unsigned char)((c)&0xff) )
#define isasciichouon(c)				( 0xB0 == (unsigned char)((c)&0xff) )
//} ADD 02.11.21 by hkakeno
/// SJIS�̊������ȃR�[�h�̔���
#define issjiswide1(c)					((0x81 <= (unsigned char)(c&0xff) && (unsigned char)(c&0xff) <= 0x9f) || (0xe0 <= (unsigned char)(c&0xff) && (unsigned char)(c&0xff) <= 0xfc))
#define issjiswide2(c)					((0x40 <= (unsigned char)(c&0xff) && (unsigned char)(c&0xff) <= 0x7e) || (0x80 <= (unsigned char)(c&0xff) && (unsigned char)(c&0xff) <= 0xfc))	//!< 2�o�C�g�ڂ̔���
#define issjiswide(c1,c2)				( issjiswide1(c1) && issjiswide2(c2) )
/// SJIS�S�p��������
/// issjiswide�͑��o�C�g�̔��肪 0xE0�`0xFC �iCP932�j�ɑ΂��A
/// isjis0208kanji1�� 0xE0�`0xEF�iMS_Kanji�j�ōs��
/// �Q�l�Fhttp://www5d.biglobe.ne.jp/~noocyte/Programming/CharCode.html
#define isjis0208kanji1(c)					((0x81 <= (unsigned char)(c&0xff) && (unsigned char)(c&0xff) <= 0x9f) || (0xe0 <= (unsigned char)(c&0xff) && (unsigned char)(c&0xff) <= 0xef))
#define isjis0208kanji2(c)					((0x40 <= (unsigned char)(c&0xff) && (unsigned char)(c&0xff) <= 0x7e) || (0x80 <= (unsigned char)(c&0xff) && (unsigned char)(c&0xff) <= 0xfc))	//!< 2�o�C�g�ڂ̔���
#define isjis0208kanji(c1,c2)				( isjis0208kanji1(c1) && isjis0208kanji2(c2) )

#define issjiswidehiragana(c1,c2)		( ( 0x82 == (unsigned char)((c1)&0xff) ) && ( 0x9f <= (unsigned char)((c2)&0xff) ) && ( (unsigned char)((c2)&0xff) <= 0xf1 ) )
#define issjiswidekatakana(c1,c2)		( ( 0x83 == (unsigned char)((c1)&0xff) ) && ( 0x40 <= (unsigned char)((c2)&0xff) ) && ( (unsigned char)((c2)&0xff) <= 0x96 ) )
/// SJIS�����i���A��񐅏��j����
/// �@��ˑ������͍l������Ȃ�
#define issjiskanji1(c)					((0x88 <= (unsigned char)(c&0xff) && (unsigned char)(c&0xff) <= 0x9f) || (0xe0 <= (unsigned char)(c&0xff) && (unsigned char)(c&0xff) <= 0xfc))
#define issjiskanji(c1,c2)				( issjiskanji1(c1) && issjiswide2(c2) && !( ( 0xee == (unsigned char)((c1)&0xff) ) && ( 0xef <= (unsigned char)((c2)&0xff) ) && ( (unsigned char)((c2)&0xff) <= 0xfc ) ) )
#define issjishalfkana(c)				((0xa1 <= (unsigned char)(c&0xff) && (unsigned char)(c&0xff) <= 0xdf))
#define issjiswidenumber(c1,c2)			( ( 0x82 == (unsigned char)((c1)&0xff) ) && ( 0x4f <= (unsigned char)((c2)&0xff) ) && ( (unsigned char)((c2)&0xff) <= 0x58 ) )
#define issjiswidealphabet(c1,c2)		( ( 0x82 == (unsigned char)((c1)&0xff) ) && ( 0x60 <= (unsigned char)((c2)&0xff) ) && ( (unsigned char)((c2)&0xff) <= 0x9a ) )
#define issjiswidegreek(c1,c2)			( ( 0x83 == (unsigned char)((c1)&0xff) ) && ( ( ( 0x9f <= (unsigned char)((c2)&0xff) ) && ( (unsigned char)((c2)&0xff) <= 0xb6 ) ) || ( ( 0xbf <= (unsigned char)((c2)&0xff) ) && ( (unsigned char)((c2)&0xff) <= 0xd6 ) ) ) )					// �M���V������ 2002/11/14 Hide.Yamamoto(CCS)
#define issjis3kanji(c1,c2)				( ( ( 0x87 == (unsigned char)((c1)&0xff) ) && ( ( 0x40 <= (unsigned char)((c2)&0xff) ) && ( (unsigned char)((c2)&0xff) <= 0x9f ) ) ) || ( ( ( 0xed <= (unsigned char)((c1)&0xff) ) && ( (unsigned char)((c1)&0xff) <= 0xee ) ) && ( ( 0x40 <= (unsigned char)((c2)&0xff) ) && ( (unsigned char)((c2)&0xff) <= 0xff ) ) ) || ( ( ( 0xfa <= (unsigned char)((c1)&0xff) ) && ( (unsigned char)((c1)&0xff) <= 0xfc ) ) && ( ( 0x40 <= (unsigned char)((c2)&0xff) ) && ( (unsigned char)((c2)&0xff) <= 0xff ) ) ) )	// ��O��������
// �L��
#define issjiswidenakaguro(c1,c2)		( ( 0x81 == (unsigned char)((c1)&0xff) ) && ( 0x45 == (unsigned char)((c2)&0xff) ) )	// �u�E�v
#define issjiswideperiod(c1,c2)			( ( 0x81 == (unsigned char)((c1)&0xff) ) && ( 0x44 == (unsigned char)((c2)&0xff) ) )	// �u�D�v
#define issjiswidesmallkakko(c1,c2)		( ( 0x81 == (unsigned char)((c1)&0xff) ) && ( 0x69 == (unsigned char)((c2)&0xff) ) )	// �u�i�v
#define issjiswidesmalltojikakko(c1,c2)	( ( 0x81 == (unsigned char)((c1)&0xff) ) && ( 0x6a == (unsigned char)((c2)&0xff) ) )	// �u�j�v
#define issjiswidenyoro(c1,c2)			( ( 0x81 == (unsigned char)((c1)&0xff) ) && ( 0x60 == (unsigned char)((c2)&0xff) ) )	// �u�`�v
#define issjiswidechouon(c1,c2)			( ( 0x81 == (unsigned char)((c1)&0xff) ) && ( 0x5b == (unsigned char)((c2)&0xff) ) )	// �u�[�v
#define issjiswidenun(c1,c2)			( ( 0x81 == (unsigned char)((c1)&0xff) ) && ( 0x58 == (unsigned char)((c2)&0xff) ) )	// �u�X�v
#define issjiswidekanma(c1,c2)			( ( 0x81 == (unsigned char)((c1)&0xff) ) && ( 0x41 == (unsigned char)((c2)&0xff) ) )	// �u�A�v
#define issjiswideshime(c1,c2)			( ( 0x81 == (unsigned char)((c1)&0xff) ) && ( 0x59 == (unsigned char)((c2)&0xff) ) )	// �u�Y�v
#define issjiswideminus(c1,c2)			( ( 0x81 == (unsigned char)((c1)&0xff) ) && ( 0x7c == (unsigned char)((c2)&0xff) ) )	// �u�|�v
#define issjiswidekigou(c1,c2)			( ( 0x81 == (unsigned char)((c1)&0xff) ) && ( 0x41 <= (unsigned char)((c2)&0xff) ) && ( (unsigned char)((c2)&0xff) <= 0xfc ) )						// �L��(���L������p) 2002/11/14 Hide.Yamamoto(CCS)
#define issjiswidedash(c1,c2)			( ( 0x81 == (unsigned char)((c1)&0xff) ) && ( 0x5c == (unsigned char)((c2)&0xff) ) )	// �u�\�v�S�p�_�b�V��
// �J�i
#define issjiswidesmallke(c1,c2)		( ( 0x83 == (unsigned char)((c1)&0xff) ) && ( 0x96 == (unsigned char)((c2)&0xff) ) )	// �u���v

#endif // _CHARUTIL_H_
