#include "stdafx.h"
#include "TsvWriter.h"
#include "common.h"

#include <TDC/useful_headers/str_util.h>

bool TsvWriter::open(const CString& file, Encoding encoding, LineFeed lineFeed)
{
	// 既にオープンしている場合はエラー
	if( !m_ofs || m_ofs.is_open() )
		return false;

	m_encoding = encoding;
	m_lineFeed = lineFeed;

	// ファイルオープン（バイナリモードで開く）
	m_ofs.open( file, std::ios::out | std::ios::trunc | std::ios::binary );
	if( !m_ofs )
		return false;

	// UTF8（BOM付き）の場合、BOMを書き込む
	if( Encoding::UTF8WithBom == m_encoding )
		m_ofs.write((char*)&BOM, sizeof(BOM));

	return true;
}

bool TsvWriter::writeRecord( const std::vector<CString>& writeStrList )
{
	// オープンしていない場合はエラー
	if( !m_ofs || !m_ofs.is_open() )
		return false;

	// レコード書込み
	return writeRecord( uh::str_util::join(writeStrList, _T("\t")) );
}


bool TsvWriter::writeRecord( const CString& writeStr )
{
	// オープンしていない場合はエラー
	if( !m_ofs || !m_ofs.is_open() )
		return false;

	// 書き込む際の改行コードの決定
	CString lineFeedStr;
	switch (m_lineFeed)
	{
	case LineFeed::CrLf : // CR+LF
		lineFeedStr = LINE_FEED_CODE_CRLF;
		break;
	case LineFeed::Lf : // LF
		lineFeedStr = LINE_FEED_CODE_LF;
		break;
	}

	// 設定された文字エンコーディングに応じた書き込み
	switch( m_encoding )
	{
	case Encoding::Euc : // EUC
		{
			m_ofs << common_proc::convSJIS2EUC( std::string(CT2A(writeStr + lineFeedStr)) ).c_str();
		}
		break;
	case Encoding::UTF8 :  // UTF-8（BOMなし）
	case Encoding::UTF8WithBom : // UTF-8（BOM付き）
		{
			CString writeStrWithLineFeed(writeStr + lineFeedStr);

			CStringA strUTF8;
			// 変換後文字列のバッファ取得
			int len = WideCharToMultiByte(CP_UTF8, 0, writeStrWithLineFeed, -1, NULL, 0, 0, 0);
			if( len > 1 )
			{
				// 変換
				char *ptr = strUTF8.GetBuffer( len - 1 );
				WideCharToMultiByte(CP_UTF8, 0, writeStrWithLineFeed, -1, ptr, len, 0, 0);
				strUTF8.ReleaseBuffer();
			}

			// レコード書込み
			m_ofs << strUTF8;
		}
		break;
	}

	m_ofs.flush();

	return true;
}

