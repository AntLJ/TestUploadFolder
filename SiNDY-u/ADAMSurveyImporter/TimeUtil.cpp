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
#include "TimeUtil.h"


TimeUtil::TimeUtil(void)
{
}


TimeUtil::~TimeUtil(void)
{
}

uh::tstring TimeUtil::GetTime()
{
	//éûä‘Çê›íË
	time_t now = time(NULL);
	struct tm pnow;
	localtime_s(&pnow, &now);

	uh::tstring timeStr =  (uh::tformat(_T("%04d%02d%02d_%02d%02d%02d")) %
			(pnow.tm_year + 1900 )%
			(pnow.tm_mon + 1) %
			pnow.tm_mday %
			pnow.tm_hour %
			pnow.tm_min %
			pnow.tm_sec
			).str();

	return timeStr;
}

uh::tstring TimeUtil::GetDate()
{
	//ì˙ïtÇê›íË
	time_t now = time(NULL);
	struct tm pnow;
	localtime_s(&pnow, &now);

	uh::tstring timeStr =  (uh::tformat(_T("%04d/%02d/%02d")) %
			(pnow.tm_year + 1900 )%
			(pnow.tm_mon + 1) %
			pnow.tm_mday
			).str();

	return timeStr;
}