#ifndef __ATLWFILE_H__
#define __ATLWFILE_H__

#pragma once

/////////////////////////////////////////////////////////////////////////////
// Windows File API wrappers
//
// Written by Bjarke Viksoe (bjarke@viksoe.dk)
// Copyright (c) 2001 Bjarke Viksoe.
//
// This code may be used in compiled form in any way you desire. This
// file may be redistributed by any means PROVIDING it is 
// not sold for profit without the authors written consent, and 
// providing that this notice and the authors name is included. 
//
// This file is provided "as is" with no expressed or implied warranty.
// The author accepts no liability if it causes any damage to you or your
// computer whatsoever. It's free, so don't hassle me about it.
//
// Beware of bugs.



#ifndef __cplusplus
   #error ATL requires C++ compilation (use a .cpp suffix)
#endif

#ifndef INVALID_SET_FILE_POINTER
#define INVALID_SET_FILE_POINTER ((DWORD)-1)
#endif

#ifndef _ATL_DLL_IMPL
namespace ATL
{
#endif

// Win32 File wrapper class
// Important: Don't make the destructor "virtual" because we need
//            the class v-table to look like the HANDLE type!

class CFile
{
public:
   HANDLE m_hFile;
public:
   CFile() 
   {
      m_hFile = INVALID_HANDLE_VALUE;
   };
   ~CFile()
   { 
      Close(); 
   };
   operator HFILE() const { return (HFILE)m_hFile; };
   operator HANDLE() const { return m_hFile; };
   BOOL Open(LPCTSTR pstrFileName, 
             DWORD dwAccess=GENERIC_READ, 
             DWORD dwShareMode=FILE_SHARE_READ, 
             DWORD dwFlags=OPEN_EXISTING,
             DWORD dwAttributes=FILE_ATTRIBUTE_NORMAL)
   {
      ATLASSERT(!::IsBadStringPtr(pstrFileName,_MAX_PATH));
      Close();
      // Attempt file creation
      HANDLE hFile = ::CreateFile(pstrFileName, 
         dwAccess, 
         dwShareMode, 
         NULL,
         dwFlags, 
         dwAttributes, 
         NULL);
      if( hFile == INVALID_HANDLE_VALUE ) return FALSE;
      m_hFile = hFile;
      return TRUE;
   };
   BOOL Create(LPCTSTR pstrFileName,
               DWORD dwAccess=GENERIC_WRITE, 
               DWORD dwShareMode=0 /*DENY ALL*/, 
               DWORD dwFlags=CREATE_ALWAYS,
               DWORD dwAttributes=FILE_ATTRIBUTE_NORMAL)
   {
      return Open(pstrFileName, dwAccess, dwShareMode, dwFlags, dwAttributes);
   };
   void Close()
   {
      if( m_hFile==INVALID_HANDLE_VALUE ) return;
      ::CloseHandle(m_hFile);
      m_hFile = INVALID_HANDLE_VALUE;
   };
   BOOL IsOpen() const
   {
      return m_hFile!=INVALID_HANDLE_VALUE;
   }
   void Attach(HANDLE hHandle)
   {
      Close();
      m_hFile = hHandle;
   }   
   HANDLE Detach()
   {
      HANDLE h = m_hFile;
      m_hFile = INVALID_HANDLE_VALUE;
      return h;
   }
   BOOL Read(LPVOID lpBuf, DWORD nCount)
   {
      ATLASSERT(m_hFile!=INVALID_HANDLE_VALUE);
      ATLASSERT(lpBuf!=NULL);
      ATLASSERT(!::IsBadWritePtr(lpBuf, nCount));
      if( nCount==0 ) return TRUE;   // avoid Win32 "null-read"
      DWORD dwRead;
      if( !::ReadFile(m_hFile, lpBuf, nCount, &dwRead, NULL) ) return FALSE;
      // Win32s will not return an error all the time (usually DISK_FULL)
      return nCount==dwRead;
   }
   BOOL Read(LPVOID lpBuf, DWORD nCount, LPDWORD pdwRead)
   {
      ATLASSERT(m_hFile!=INVALID_HANDLE_VALUE);
      ATLASSERT(lpBuf);
      ATLASSERT(!::IsBadWritePtr(lpBuf, nCount));
      ATLASSERT(pdwRead);
      *pdwRead = 0;
      if( nCount==0 ) return TRUE;   // avoid Win32 "null-read"
      if( !::ReadFile(m_hFile, lpBuf, nCount, pdwRead, NULL) ) return FALSE;
      // Win32s will not return an error all the time (usually DISK_FULL)
      return nCount==*pdwRead;
   }
   BOOL Write(LPCVOID lpBuf, DWORD nCount)
   {
      ATLASSERT(m_hFile!=INVALID_HANDLE_VALUE);
      ATLASSERT(lpBuf!=NULL);
      ATLASSERT(!::IsBadReadPtr(lpBuf, nCount));   
      if( nCount==0 ) return TRUE; // avoid Win32 "null-write" option
      DWORD dwWritten;
      if( !::WriteFile(m_hFile, lpBuf, nCount, &dwWritten, NULL) ) return FALSE;
      // Win32s will not return an error all the time (usually DISK_FULL)
      return nCount==dwWritten;
   }
   BOOL Write(LPCVOID lpBuf, DWORD nCount, LPDWORD pdwWritten)
   {
      ATLASSERT(m_hFile!=INVALID_HANDLE_VALUE);
      ATLASSERT(lpBuf);
      ATLASSERT(!::IsBadReadPtr(lpBuf, nCount));
      ATLASSERT(pdwWritten);    
      *pdwWritten = 0;
      if( nCount==0 ) return TRUE; // avoid Win32 "null-write" option

      if( !::WriteFile(m_hFile, lpBuf, nCount, pdwWritten, NULL) ) return FALSE;
      // Win32s will not return an error all the time (usually DISK_FULL)
      return nCount==*pdwWritten;
   }
   /* 
   上野の2つのWrite関数はUNICODEビルドの場合UNICODEで書き込まれる。
   それはそれで良いのだが、通常はSJISで書き込まれて欲しいので新たに
   Write関数を用意 by 米澤
	*/
   BOOL Write( CString strWriteBuff )
   {
      ATLASSERT(m_hFile!=INVALID_HANDLE_VALUE);

#ifdef	UNICODE
	  // 書き込み文字列のバイト数を取得
	  int result = ::WideCharToMultiByte(CP_ACP, 0, LPCTSTR(strWriteBuff), -1, NULL, 0, NULL, NULL);

	  // SJISへの変換用バッファアロケート
	  char* chFileName = new char [result+1];
	  ::ZeroMemory( chFileName, result+1 );

	  // UNICODEからSJISへ変換
	  result = ::WideCharToMultiByte(CP_ACP, 0, LPCTSTR(strWriteBuff), -1, chFileName, result+1, NULL, NULL);

	  // 変換バイト数が1以上の場合は書き込み
	  if (result > 0){
		  Write( chFileName, strlen( chFileName ) );
		  delete [] chFileName;
		  return TRUE;
	  }
	  delete [] chFileName;
	  return FALSE;
#else
	// 確認内容を書き込む
      if( !Write(  LPCTSTR(strWriteBuff), strWriteBuff.GetLength() ) ) 
		  return FALSE;

	  return TRUE;
#endif
   }

   // 米澤追加 \nまでの1行を読み込む
   BOOL GetLineText( CString& strBuff, DWORD nBuffSize = 2048 )
   {
      if( m_hFile==INVALID_HANDLE_VALUE ) 
		  return FALSE;
	  
	    char chBuff[8];
		char* pBuffer = new char[nBuffSize];
		::ZeroMemory( pBuffer, nBuffSize );

		DWORD nReadCount;
		DWORD nCharCnt = 0;
		BOOL  bResult = TRUE;

		while (1) {
			if( nBuffSize <= nCharCnt ){
				bResult = FALSE;	// バッファオーバーフロー
				break;
			}
			Read( chBuff, 1, &nReadCount );
			if( nReadCount == 0 ){
				if( strlen( pBuffer ) != 0 ){
					bResult = TRUE;	// 1行のけつにEOFがある場合
					break;
				}else{
					bResult = FALSE;	// EOF
					break;
				}
			}
			chBuff[nReadCount] = NULL;
			
			// 改行なら抜ける
			if( chBuff[0] == '\r' ){	// 0x0d
				Read( chBuff, 1, &nReadCount );	// 読み捨て(0x0a)
				bResult = TRUE;	// EOL
				break;
			}
			if( chBuff[0] == '\n' ){	// 0x0a（UNIX改行の場合もありうる)
				bResult = TRUE;	// EOL
				break;
			}

			pBuffer[ nCharCnt++ ] = chBuff[0];
		}
		strBuff = pBuffer;
		delete [] pBuffer;

		return bResult;
   }
   DWORD Seek(LONG lOff, UINT nFrom)
   {
      ATLASSERT(m_hFile!=INVALID_HANDLE_VALUE);
      DWORD dwNew = ::SetFilePointer(m_hFile, lOff, NULL, (DWORD)nFrom);
      if( dwNew==INVALID_SET_FILE_POINTER ) return (DWORD)-1;
      return dwNew;
   }
   DWORD GetPosition() const
   {
      ATLASSERT(m_hFile!=INVALID_HANDLE_VALUE);
      DWORD dwPos = ::SetFilePointer(m_hFile, 0, NULL, FILE_CURRENT);
      if( dwPos == INVALID_SET_FILE_POINTER ) return (DWORD)-1;
      return dwPos;
   }
   BOOL Flush()
   {
      ATLASSERT(m_hFile!=INVALID_HANDLE_VALUE);
      if( m_hFile==INVALID_HANDLE_VALUE ) return FALSE;
      if( ::FlushFileBuffers(m_hFile)==0 ) return FALSE;
      return TRUE;
   }
   DWORD GetSize() const
   {
      ATLASSERT(m_hFile!=INVALID_HANDLE_VALUE);
      return ::GetFileSize(m_hFile, NULL);
   }
   DWORD GetType() const
   {
      ATLASSERT(m_hFile!=INVALID_HANDLE_VALUE);
      return ::GetFileType(m_hFile);
   }
   BOOL GetFileTime(FILETIME *ftCreate, FILETIME *ftAccess, FILETIME *ftModified)
   {
      ATLASSERT(m_hFile!=INVALID_HANDLE_VALUE);
      return ::GetFileTime(m_hFile, ftCreate, ftAccess, ftModified);
   }
   static BOOL FileExists(LPCTSTR pstrFileName)
   {
      ATLASSERT(!::IsBadStringPtr(pstrFileName, MAX_PATH));
      return ::GetFileAttributes(pstrFileName)!=0xFFFFFFFF;
   };
   static BOOL Delete(LPCTSTR pstrFileName)
   {
      ATLASSERT(!::IsBadStringPtr(pstrFileName, MAX_PATH));
      return ::DeleteFile(pstrFileName);
   };
   static BOOL Rename(LPCTSTR pstrSourceFileName, LPCTSTR pstrTargetFileName)
   {
      ATLASSERT(!::IsBadStringPtr(pstrSourceFileName, MAX_PATH));
      ATLASSERT(!::IsBadStringPtr(pstrTargetFileName, MAX_PATH));
      return ::MoveFile(pstrSourceFileName, pstrTargetFileName);
   };
   static BOOL Copy(LPCTSTR pstrSourceFileName, LPCTSTR pstrTargetFileName)
   {
		ATLASSERT(!::IsBadStringPtr(pstrSourceFileName, MAX_PATH));
		ATLASSERT(!::IsBadStringPtr(pstrTargetFileName, MAX_PATH));
		return CopyFile( pstrSourceFileName, pstrTargetFileName, FALSE );
   }

};

class CTemporaryFile : public CFile
{
public:
   TCHAR m_szFileName[MAX_PATH];
public:
   ~CTemporaryFile()
   { 
      Close();
      Delete(m_szFileName);
   };
   BOOL Create(LPTSTR pstrFileName, UINT cchFilename,
               DWORD dwAccess=GENERIC_WRITE, 
               DWORD dwShareMode=0 /*DENY ALL*/, 
               DWORD dwFlags=CREATE_ALWAYS,
               DWORD dwAttributes=FILE_ATTRIBUTE_NORMAL)
   {
      ATLASSERT(!::IsBadStringPtr(pstrFileName, cchFilename));
      if( cchFilename>0 ) {
         ::GetTempPath(cchFilename, pstrFileName);
         ::GetTempFileName(pstrFileName, _T("BV"), 0, pstrFileName);
      }
      ::lstrcpy(m_szFileName, pstrFileName);
      return Open(pstrFileName, dwAccess, dwShareMode, dwFlags, dwAttributes);
   };
};


#ifndef _ATL_DLL_IMPL
}; //namespace ATL
#endif


#endif // __ATLWFILE_H___
