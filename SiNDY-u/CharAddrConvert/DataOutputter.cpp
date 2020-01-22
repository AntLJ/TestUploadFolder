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
#include "DataOutputter.h"

extern "C"
{
#include <WKF_LIB/wkf.h>
}

/**
 * @brief	�������SJIS->EUC�R�[�h�֕ϊ�
 * @param	lpcszData	[in]	������
 * @return	�ϊ��f�[�^
 */
inline CString ConvDataSJIS2EUC( const _TCHAR* lpcszData )
{
	char szConvData[200] = "";
	return CONV_OK == wkfConvertKanjiCodeOfString(KC_SJIS, CT2A(lpcszData), KC_EUC, szConvData, sizeof(szConvData))?
		   CString(szConvData) : CString(_T(""));
}

namespace data_output
{
	// �i�r����Pnt���R�[�h�o��
	void OutPntFile( std::ofstream& stream, const PNTMAP& rPntList )
	{
		for( PNTMAP::const_iterator itr = rPntList.begin(); itr != rPntList.end(); ++itr )
		{
			const CPntRec& cRec = itr->second;
			stream << cRec.m_cMXY.m_nMeshCode << "\t" << cRec.m_cMXY.m_nX << "\t" << cRec.m_cMXY.m_nY << "\t"								// MeshXY
				   << 51 << "\t" << 4 << "\t"																								// ���W�񋟉�ЁE���W���x��ʃR�[�h
				   << cRec.m_strAddrCode.Left(2) << "\t" << cRec.m_strAddrCode.Mid(2,3) << "\t"												// �s���{���E�s�撬���R�[�h
				   << cRec.m_strAddrCode.Mid(5,3) << "\t" << cRec.m_strAddrCode.Mid(8, 3) << "\t" << cRec.m_strAddrCode.Right(5) << "\t"	// �厚�E���E�n�ԃR�[�h
				   << ConvDataSJIS2EUC(addr_func::GetZenkakuData(cRec.m_strPntName)) << "\t"												// �n�Ԗ���
				   << ConvDataSJIS2EUC(addr_func::GetZenkakuData(cRec.m_strPntYomi)) << "\t"												// �n�ԃ��~
				   << cRec.m_strZipCode << "\t" << cRec.m_lRepDomariF << "\n";																// �X�֔ԍ��E��\�_�E�n�Ԏ~�܂�t���O
			stream.flush();
		}
	}

	// �O���񋟌���Pnt���R�[�h�o��
	void OutPntFile_Outside( std::ofstream& stream, const PNTMAP& rPntList )
	{
		for( PNTMAP::const_iterator itr = rPntList.begin(); itr != rPntList.end(); ++itr )
		{
			const CPntRec& cRec = itr->second;

			// ���W�������_��11���܂ŕK���\�������
			CString strLonLat;
			strLonLat.Format( _T("%.11f\t%.11f"), cRec.m_dLon, cRec.m_dLat );
			stream << cRec.m_strAddrCode.Left(2) << "\t" << cRec.m_strAddrCode.Mid(2,3) << "\t"												// �s���{���E�s�撬���R�[�h
				   << cRec.m_strAddrCode.Mid(5,3) << "\t" << cRec.m_strAddrCode.Mid(8, 3) << "\t" << cRec.m_strAddrCode.Right(5) << "\t"	// �厚�E���E�n�ԃR�[�h
				   << addr_func::GetZenkakuData(cRec.m_strPntName) << "\t" << addr_func::GetZenkakuData(cRec.m_strPntYomi) << "\t"			// �n�Ԗ��́E���~
				   << CT2A(strLonLat) << "\t"																								// �ܓx�E�o�x
				   << cRec.m_lRepDomariF << "\r\n";																							// ��\�_�E�n�Ԏ~�܂�t���O
			stream.flush();
		}
	}

	// �i�r����Gou���R�[�h�o��
	void OutGouFile( std::ofstream& stream, const GOUMAP& rGouList )
	{
		for( GOUMAP::const_iterator itr = rGouList.begin(); itr != rGouList.end(); ++itr )
		{
			const CGouRec& cRec = itr->second;
			
			// ��4��[0000]�ɂȂ��Ă���ꍇ�́A�o�͂��Ȃ�
			if( cRec.m_strAddrCode.Right(4) != _T("0000") )
			{
				stream << cRec.m_cMXY.m_nMeshCode << "\t" << cRec.m_cMXY.m_nX << "\t" << cRec.m_cMXY.m_nY << "\t"						// MeshXY
					   << 51 << "\t" << 5 << "\t"																						// ���W�񋟉�ЁE���W���x��ʃR�[�h
					   << cRec.m_strAddrCode.Left(2) << "\t" << cRec.m_strAddrCode.Mid(2,3) << "\t"										// �s���{���E�s�撬���R�[�h
					   << cRec.m_strAddrCode.Mid(5,3) << "\t" << cRec.m_strAddrCode.Mid(8, 3) << "\t"									// �厚�E���R�[�h
					   << cRec.m_strAddrCode.Mid(11, 5) << "\t" << cRec.m_strAddrCode.Right(4) << "\t"									// �n�ԁE�Z���ԍ��R�[�h
					   << ConvDataSJIS2EUC(addr_func::GetZenkakuData(cRec.m_strGouName)) << "\t"										// �Z���ԍ�����
					   << ConvDataSJIS2EUC(addr_func::GetZenkakuData(cRec.m_strGouYomi)) << "\t"										// �Z���ԍ����~
					   << cRec.m_strZipCode << "\n";																					// �X�֔ԍ�
				stream.flush();
			}
		}
	}

	// �O���񋟌���Gou���R�[�h�o��
	void OutGouFile_Outside( std::ofstream& stream, const GOUMAP& rGouList )
	{
		for( GOUMAP::const_iterator itr = rGouList.begin(); itr != rGouList.end(); ++itr )
		{
			const CGouRec& cRec = itr->second;
			
			// ��4��[0000]�ɂȂ��Ă���ꍇ�́A�o�͂��Ȃ�
			if( cRec.m_strAddrCode.Right(4) != _T("0000") )
			{
				// ���W�������_��11���܂ŕK���\�������
				CString strLonLat;
				strLonLat.Format( _T("%.11f\t%.11f"), cRec.m_dLon, cRec.m_dLat );
				stream << cRec.m_strAddrCode.Left(2) << "\t" << cRec.m_strAddrCode.Mid(2,3) << "\t"										// �s���{���E�s�撬���R�[�h
					   << cRec.m_strAddrCode.Mid(5,3) << "\t" << cRec.m_strAddrCode.Mid(8, 3) << "\t"									// �厚�E���R�[�h
					   << cRec.m_strAddrCode.Mid(11, 5) << "\t" << cRec.m_strAddrCode.Right(4) << "\t"									// �n�ԁE�Z���ԍ��R�[�h
					   << addr_func::GetZenkakuData(cRec.m_strGouName) << "\t" << addr_func::GetZenkakuData(cRec.m_strGouYomi) << "\t"	// �Z���ԍ����́E�Z���ԍ����~
					   << CT2A(strLonLat) << "\r\n";																					// �ܓx�E�o�x
				stream.flush();
			}
		}
	}
}
