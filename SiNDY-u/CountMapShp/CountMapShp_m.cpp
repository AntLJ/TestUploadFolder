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

// CommandLine.cpp : Defines the entry point for the console application.
//

#include "main_rootin.h"

int main(int argc, char* argv[])
{

	DWORD			a_dBegin_Time	= 0;
	a_dBegin_Time	= timeGetTime();

	const arctl::coinitializer coinit;

	main_rootin	a_mr;	// プログラムの設定

	if( !a_mr.arg_check ( argc, argv ) ) {
		a_mr.print_usage( argv );
		fflush(stdout);
		fflush(stderr);
		return	( 1 );
	}

	HRESULT		hr;
	
	try
	{
		// Execute the command
		string		a_Del_DB_Name;


		IWorkspaceFactoryPtr	ipWorkspaceFactory	( CLSID_SdeWorkspaceFactory );	// 
		IPropertySetPtr			ipPropertySet		( CLSID_PropertySet );			// 接続プロパティー格納用
		IWorkspacePtr			ipWorkspace;

		USES_CONVERSION;	// OLE2T 等の一部のマクロを使用する場合には必ず必要

		// DBへの接続
		if( !a_mr.DB_Open	(	ipWorkspaceFactory, ipPropertySet, ipWorkspace	) ) {
			fprintf	(stderr, "接続出来なかった\n");
			fflush(stdout);
			fflush(stderr);
			return	( 1 );
		//	End	();
		}else {
			printf("接続完了");
		}
		printf(_T("\n"));

		// DB接続にかかった時間の表示
		DWORD				a_dConnect_Time		= 0;
		a_dConnect_Time		= timeGetTime();

		fprintf	( stdout,	"SiNDY-DB接続処理時間 = %0.2f(s)でした\n",
							(double)((a_dConnect_Time - a_dBegin_Time) / 1000.0) );

		bool bRet = a_mr.exec_command( ipWorkspace );
		if( !bRet )
		{
			fprintf	( stderr, "異常終了\n" );
			fflush(stdout);
			fflush(stderr);
			return 1;
		}
	}
	catch ( ... ){
		fprintf	( stderr, "例外が発生しています\n" );
		if( a_mr.m_fpOutLogFp != stderr && a_mr.m_fpOutLogFp != NULL ) {
			fprintf ( a_mr.m_fpOutLogFp, "例外が発生しています\n" );
		}
		fflush(stdout);
		fflush(stderr);
		return 2;
	}

	fprintf	( stderr, "正常終了\n" );
	fflush(stdout);
	fflush(stderr);
	return ( 0 );
}
