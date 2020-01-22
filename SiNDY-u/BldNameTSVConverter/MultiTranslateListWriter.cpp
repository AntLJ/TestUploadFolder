#include "stdafx.h"
#include "MultiTranslateListWriter.h"
#include "LogMgr.h"
#include "common.h"

#include <TDC/useful_headers/str_util.h>
#include <boost/assign.hpp>

MultiTranslateListWriter::MultiTranslateListWriter(
			const CString& outputDir,
			int maxRecNum,
			const CString& grpcode,
			const std::vector<CString>& addColumnList) 
			: m_outputDir(outputDir), m_maxRecNum(maxRecNum), m_fileSepCounter(0), 
				m_writeRecCounter(0), m_grpcode(grpcode)
{
	// ヘッダ名を定義(固定部）
	m_headerList = boost::assign::list_of
		( _T("データ分類コード") )
		( _T("建物ポリゴンOID") )
		( _T("元名称") )
		( _T("元読み") )
		( _T("分割名称") )
		( _T("分割読み") )
		;

	// 多言語翻訳リスト追加カラム設定ファイルに記載されたヘッダ名を追加
	for( const auto& column : addColumnList )
		m_headerList.push_back( column );
}

bool MultiTranslateListWriter::open()
{
	CString file = uh::str_util::format(FILENAME_FORMAT, m_outputDir, m_grpcode, ++m_fileSepCounter);
	if( !TsvWriter::open( file, Encoding::UTF8WithBom, LineFeed::CrLf ) )
	{
		LogMgr::getInstance().writeRunLog( _T("多言語翻訳リストをオープンできません : ") + file );
		return false;
	}

	// ヘッダ名を書込み
	TsvWriter::writeRecord( m_headerList );

	return true;
}

bool MultiTranslateListWriter::writeRecord( const std::vector<CString>& writeStrList )
{
	// ファイルに書き込んだ件数が、最大書込みレコード件数より大きい場合
	if( m_maxRecNum <= m_writeRecCounter )
	{
		// 現在書込み中のファイルはクローズ
		close();

		// カウンタを初期化
		m_writeRecCounter = 0;

		// ファイル分割カウンタをインクリメントし、別名でファイルオープン
		if( !MultiTranslateListWriter::open() )
			return false;
	}

	// 書込み
	TsvWriter::writeRecord( writeStrList );

	// 書き込み件数をインクリメント
	++m_writeRecCounter;

	return true;
}
