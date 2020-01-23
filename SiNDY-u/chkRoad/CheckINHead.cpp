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

#include "StdAfx.h"
#include "CheckINHead.h"
#include "ErrorInfo.h"

CCheckINHead::CCheckINHead(void) : m_bIsError( false )
{
}

CCheckINHead::~CCheckINHead(void)
{
}

//////////////////////////////////////////////////////////////////////
// INデータのヘッダ情報チェック
//////////////////////////////////////////////////////////////////////
bool CCheckINHead::check( CString& rMntDir, int iMeshCode, HeadAttrList& rHeadAttr)
{
	int iBaseMesh = iMeshCode/100;
	CString aMntFileName, aRdgFileName;
	
	// .mntファイルヘッダのチェック
	aMntFileName.Format("%s\\%d\\%d.mnt", rMntDir, iBaseMesh, iMeshCode );
	checkFile(aMntFileName, rHeadAttr, iMeshCode);

	// .rdgファイルヘッダのチェック
	aRdgFileName.Format("%s\\%d\\%d.rdg", rMntDir, iBaseMesh, iMeshCode );
	checkFile(aRdgFileName, rHeadAttr, iMeshCode);

	return true;
}

//////////////////////////////////////////////////////////////////////
// 指定ファイルのヘッダをチェック
//////////////////////////////////////////////////////////////////////
bool CCheckINHead::checkFile( CString &rFilePath, HeadAttrList& rHeadAttr, int rMeshCode)
{
	ifstream	aFileStream(rFilePath, ios::in | ios::binary);
	char aBuffer[64] ={0};
	CString rFileName = rFilePath.Right(rFilePath.GetLength()-rFilePath.ReverseFind(_T('\\'))-1);
	CString rFileExtention = rFileName.Right(3);

	ErrorHead	aError;
	aError.eFileName = rFileName;
	aError.eMesh = rMeshCode;
	aError.eError = rHeadAttr.eError;

	aFileStream.seekg(4, ios::cur);
	// メッシュコード次数
	aFileStream.read(aBuffer, 4);
	int iMeshNum = ReverseWord(aBuffer, false);
	if( iMeshNum != 2 )
		PrintError( error::head::kmeshnum, aError, iMeshNum );

	//2次メッシュコード
	aFileStream.read(aBuffer, 4);
	int lMeshCode = ReverseWord(aBuffer, true);
	if( lMeshCode != (rMeshCode*100) )
		PrintError( error::head::kmeshcode, aError, lMeshCode );

	//ファイル判別子
	aFileStream.read(aBuffer, 4);
	CString aFileEx = aBuffer;
	if( (rFileExtention == _T("mnt") && aFileEx != _T("mnt")  ) || (rFileExtention == _T("rdg") && aFileEx != _T("rdg")  ) ||
		(rFileExtention != _T("mnt") && rFileExtention != _T("rdg")  ) )
		PrintStringError( error::head::kfileapp, aError, aFileEx );

	aFileStream.seekg(4, ios::cur);
	//縮尺
	aFileStream.read(aBuffer, 4);
	int lSyuku = ReverseWord(aBuffer, false);
	if( lSyuku != 25000 )
		PrintError( error::head::kscale, aError,lSyuku );

	aFileStream.seekg(44, ios::cur);
	//ポリゴン最大形状点数
	aFileStream.read(aBuffer, 4);
	int iMaxPointCount = ReverseWord(aBuffer, false);
	if( iMaxPointCount != 0 )
		PrintError( error::head::kmaxpili, aError, iMaxPointCount );

	aFileStream.seekg(128, ios::beg);
	//レイヤ数
	aFileStream.read(aBuffer, 4);
	int iLayerCount = ReverseWord(aBuffer, false);	
	if( iLayerCount > 6 )
		PrintError( error::head::klayercount, aError, iLayerCount );

	aFileStream.seekg(12, ios::cur);
	//データアドレス
	aFileStream.read(aBuffer, 4);
	int iAddress = ReverseWord(aBuffer, false);	

	//データサイズ
	aFileStream.read(aBuffer, 4);
	int iDataSize = ReverseWord(aBuffer, false);	

	int iTotalSize = 0;
	for( int iCount = 0; iCount < iLayerCount; ++iCount )
	{
		//レイヤID
		aFileStream.read(aBuffer, 4);
		int iLayerID = ReverseWord(aBuffer, false);	
		CString cstrLayerID;
		SetObjectID( cstrLayerID, iLayerID );

		if( rFileExtention == _T("mnt") )
		{
			set<int>::iterator iter_mnt = rHeadAttr.eMnt.find(iLayerID);
			if( iter_mnt == rHeadAttr.eMnt.end() )
				PrintStringError( error::head::klayerid, aError, cstrLayerID ); 
		}
		else if( rFileExtention == _T("rdg") )
		{
			set<int>::iterator iter_rdg = rHeadAttr.eRdg.find(iLayerID);
			if( iter_rdg == rHeadAttr.eRdg.end() )
				PrintStringError( error::head::klayerid, aError, cstrLayerID ); 
		}

		aFileStream.seekg(4, ios::cur);
		//レイヤアドレス
		aFileStream.read(aBuffer, 4);
		int iLayerAddress = ReverseWord(aBuffer, false);	
		if( iLayerAddress != iTotalSize )
			PrintError( error::head::klayeradd, aError, iLayerAddress );

		//レイヤサイズ
		aFileStream.read(aBuffer, 4);
		int iLayerSize = ReverseWord(aBuffer, false);

		iTotalSize += iLayerSize;
	}
	aFileStream.close();
	return true;
}

//////////////////////////////////////////////////////////////////////
// エラー出力(コード値:10進数)
//////////////////////////////////////////////////////////////////////
void CCheckINHead::PrintError(int rKey, ErrorHead &rError, int rCode)
{
	cout << rError.eMesh << "\tERROR" << rError.eError[rKey] << "\t";
	cout << static_cast<LPCTSTR>( rError.eFileName ) << "\t" << rCode << endl;

	m_bIsError = true;
}

//////////////////////////////////////////////////////////////////////
// エラー出力(コード値:文字列)
//////////////////////////////////////////////////////////////////////
void CCheckINHead::PrintStringError(int rKey, ErrorHead &rError, CString &rWord)
{
	cout << rError.eMesh << "\tERROR" << rError.eError[rKey] << "\t";
	cout << static_cast<LPCTSTR>( rError.eFileName );
	cout << "\t" << static_cast<LPCTSTR>( rWord ) << endl;

	m_bIsError = true;
}

//////////////////////////////////////////////////////////////////////
// バイナリコードを数字に変換( bBcd:TRUE[16進数] FALSE[10進数] )
//////////////////////////////////////////////////////////////////////
#ifdef	LITTLE_ENDIAN
int CCheckINHead::ReverseWord(char *rWord, bool bBcd)
{
	int *p;

	char temp;
	temp = rWord[0];
	rWord[0] = rWord[3];
	rWord[3] = temp;
		
	temp = rWord[1];
	rWord[1] = rWord[2];
	rWord[2] = temp;
	p = (int*)&rWord[0];
	if(bBcd)
	{
		CString aConv;
		aConv.Format("%x",*p);
		int hex = _ttoi(aConv);
		return hex;
	}

	return *p;
}
#else
int CCheckINHead::ReverseWord(char *rWord, bool bBcd)
{
	int *p;
	p = (int*)&rWord[0];

	if(bBcd)
	{
		CString aConv;
		aConv.Format("%x",*p);
		int hex = _ttoi(aConv);
		return hex;
	}
	return *p;
}
#endif
