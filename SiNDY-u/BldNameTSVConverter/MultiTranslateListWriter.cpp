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
	// �w�b�_�����`(�Œ蕔�j
	m_headerList = boost::assign::list_of
		( _T("�f�[�^���ރR�[�h") )
		( _T("�����|���S��OID") )
		( _T("������") )
		( _T("���ǂ�") )
		( _T("��������") )
		( _T("�����ǂ�") )
		;

	// ������|�󃊃X�g�ǉ��J�����ݒ�t�@�C���ɋL�ڂ��ꂽ�w�b�_����ǉ�
	for( const auto& column : addColumnList )
		m_headerList.push_back( column );
}

bool MultiTranslateListWriter::open()
{
	CString file = uh::str_util::format(FILENAME_FORMAT, m_outputDir, m_grpcode, ++m_fileSepCounter);
	if( !TsvWriter::open( file, Encoding::UTF8WithBom, LineFeed::CrLf ) )
	{
		LogMgr::getInstance().writeRunLog( _T("������|�󃊃X�g���I�[�v���ł��܂��� : ") + file );
		return false;
	}

	// �w�b�_����������
	TsvWriter::writeRecord( m_headerList );

	return true;
}

bool MultiTranslateListWriter::writeRecord( const std::vector<CString>& writeStrList )
{
	// �t�@�C���ɏ������񂾌������A�ő发���݃��R�[�h�������傫���ꍇ
	if( m_maxRecNum <= m_writeRecCounter )
	{
		// ���ݏ����ݒ��̃t�@�C���̓N���[�Y
		close();

		// �J�E���^��������
		m_writeRecCounter = 0;

		// �t�@�C�������J�E���^���C���N�������g���A�ʖ��Ńt�@�C���I�[�v��
		if( !MultiTranslateListWriter::open() )
			return false;
	}

	// ������
	TsvWriter::writeRecord( writeStrList );

	// �������݌������C���N�������g
	++m_writeRecCounter;

	return true;
}
