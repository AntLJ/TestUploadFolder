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

#ifndef __RNS_MEMBER__
#define __RNS_MEMBER__

/* ************************************************

 	初稿 :
 	改訂 : 
 
 	制作 :	Fukuta.K
 	
************************************************* */

#include <RNS.h>
#include <map>

#include "RNSID.h"

/*
 * **************************************************
 *      RNSMember
 * **************************************************
 */

class RNSGroup;
class RNSMember : public RNSObject {
	RNSGroup*	mGroup;	// メンバが所属するグループ
	RNSMember*	mNext;	// 次のメンバ
	RNSMember*	mPrev;	// 前のメンバ

public:
	RNSMember(RNSGroup* iGroup) { mGroup = iGroup; }
	virtual RNSMember(){}

	void setNext(RNSMember* iNext) { mNext = iNext; }
	void setPrev(RNSMember* iPrev) { mPrev = iPrev; }

	RNSGroup*	group() { return mGroup; }
	RNSMember*	next() { return mNext; }
	RNSMember*	prev() { return mPrev; }
};

#endif /* __RNS_MEMBER__ */
