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
#include "ErrorHandle.h"


// ログ出力
void CErrorHandle::printLog(ATL::CString FileName, std::string LayerName )
{
	m_fout
		<< "#Error " << FileName << " 内に " << LayerName << " レイヤはありません！！" << std::endl;
}


void CErrorHandle::printLog(ATL::CString FileName, int MeshSuccessFlag)
{
	if( MeshSuccessFlag == 0 )
	{
		m_fout
			<< "# " << FileName << "   ○" << std::endl;
	}
	if( MeshSuccessFlag == 1 )
	{
		m_fout
			<< "# " << FileName << "   ×" << std::endl;
	}
}

void CErrorHandle::printPGDBOpenError(ATL::CString FileName, ATL::CString ErrorMsg)
{
	m_fout
		<< "#Error " << FileName << " への接続に失敗しました！" << std::endl
		<< "       " << ErrorMsg << std::endl;

}
