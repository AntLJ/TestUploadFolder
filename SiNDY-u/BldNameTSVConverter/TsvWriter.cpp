#include "stdafx.h"
#include "TsvWriter.h"
#include "common.h"

#include <TDC/useful_headers/str_util.h>

bool TsvWriter::open(const CString& file, Encoding encoding, LineFeed lineFeed)
{
	// ���ɃI�[�v�����Ă���ꍇ�̓G���[
	if( !m_ofs || m_ofs.is_open() )
		return false;

	m_encoding = encoding;
	m_lineFeed = lineFeed;

	// �t�@�C���I�[�v���i�o�C�i�����[�h�ŊJ���j
	m_ofs.open( file, std::ios::out | std::ios::trunc | std::ios::binary );
	if( !m_ofs )
		return false;

	// UTF8�iBOM�t���j�̏ꍇ�ABOM����������
	if( Encoding::UTF8WithBom == m_encoding )
		m_ofs.write((char*)&BOM, sizeof(BOM));

	return true;
}

bool TsvWriter::writeRecord( const std::vector<CString>& writeStrList )
{
	// �I�[�v�����Ă��Ȃ��ꍇ�̓G���[
	if( !m_ofs || !m_ofs.is_open() )
		return false;

	// ���R�[�h������
	return writeRecord( uh::str_util::join(writeStrList, _T("\t")) );
}


bool TsvWriter::writeRecord( const CString& writeStr )
{
	// �I�[�v�����Ă��Ȃ��ꍇ�̓G���[
	if( !m_ofs || !m_ofs.is_open() )
		return false;

	// �������ލۂ̉��s�R�[�h�̌���
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

	// �ݒ肳�ꂽ�����G���R�[�f�B���O�ɉ�������������
	switch( m_encoding )
	{
	case Encoding::Euc : // EUC
		{
			m_ofs << common_proc::convSJIS2EUC( std::string(CT2A(writeStr + lineFeedStr)) ).c_str();
		}
		break;
	case Encoding::UTF8 :  // UTF-8�iBOM�Ȃ��j
	case Encoding::UTF8WithBom : // UTF-8�iBOM�t���j
		{
			CString writeStrWithLineFeed(writeStr + lineFeedStr);

			CStringA strUTF8;
			// �ϊ��㕶����̃o�b�t�@�擾
			int len = WideCharToMultiByte(CP_UTF8, 0, writeStrWithLineFeed, -1, NULL, 0, 0, 0);
			if( len > 1 )
			{
				// �ϊ�
				char *ptr = strUTF8.GetBuffer( len - 1 );
				WideCharToMultiByte(CP_UTF8, 0, writeStrWithLineFeed, -1, ptr, len, 0, 0);
				strUTF8.ReleaseBuffer();
			}

			// ���R�[�h������
			m_ofs << strUTF8;
		}
		break;
	}

	m_ofs.flush();

	return true;
}

