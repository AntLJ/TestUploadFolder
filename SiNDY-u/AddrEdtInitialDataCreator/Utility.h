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

#pragma once

#include "stdafx.h"
#include <regex>
#include <TDC/useful_headers/str_util.h>

namespace Utility
{

/**
 * @brief ���s���[�h
 */
enum ExecuteType
{
	None,         // ���w��
	PlaceNameRep, // �n����\�|�C���g�C���|�[�g
	AddrPoly,     // �Z���|���S���C���|�[�g
	AddrPoint,    // �Z���|�C���g�C���|�[�g
	AddrAll       // �Z���|���S���{�Z���|���S���C���|�[�g
};

/**
 * @brief ���s���O�o�̓X�e�[�^�X
 */
enum RunLogMsgStatus
{
	Normal,    // �X�e�[�^�X�Ȃ�
	Info,      // INFO
	Error,     // ERROR
	Fatal      // FATAL
};

}

/**
 * @brief �s�撬���R�[�h�����pregex
 * @note 5���̔��p����
 */
const std::wregex citycode_re(L"^[0-9]{5}$");

/**
 * @brief �g���R�[�h���X�g���
 */
struct ExtcodeListRec
{
	/**
	 * @brief   �R���X�g���N�^
	 */
	ExtcodeListRec() {}

	/**
	 * @brief   �R���X�g���N�^
	 * @param   code [in]  �g���R�[�h
	 * @param   name [in]  ����
	 * @param   yomi [in]  �ǂ�
	 */
	ExtcodeListRec(const CString& code, const CString& name, const CString& yomi) :
											extcode(code), extName(name), extYomi(yomi) {}

	CString extcode;  //!< �g���R�[�h
	CString extName;  //!< ����
	CString extYomi;  //!< �ǂ�
};

/**
 * @brief �Z���|���S�����
 */
struct EdtAddrPolygonRec
{
	/**
	 * @brief   �R���X�g���N�^
	 */
	EdtAddrPolygonRec() : oid(-1), addrType(0), colorCode(0) {}

	/**
	 * @brief   �Z���R�[�h2�ݒ�
	 * @param   addrcode [in]  �Z���R�[�h
	 */
	void setAddrcode2(const CString& addrcode) 
	{
		addrcode2 = ( 0 == addrcode.CompareNoCase(_T(" ")) ) ? _T("") : addrcode;
	}

	long oid;            //!< �I�u�W�F�N�gID
	CString citycode;    //!< �s�撬���R�[�h
	CString addrcode1;   //!< �Z���R�[�h1
	CString extName1;    //!< �g�����́i���́j
	CString extYomi1;    //!< �g���ǂ݁i���́j
	CString addrcode2;   //!< �Z���R�[�h2
	CString extName2;    //!< �g�����́i�ʏ́j
	CString extYomi2;    //!< �g���ǂ݁i�ʏ́j
	CString gaikuFugo;   //!< �X�敄��
	int addrType;        //!< �Z��������ʃR�[�h
	int colorCode;       //!< �X��F�R�[�h
	IGeometryPtr geo;    //!< �`��
};

/**
 * @brief �Z���|�C���g���
 */
struct EdtAddrPointRec
{
	/**
	 * @brief   �R���X�g���N�^
	 */
	EdtAddrPointRec() : oid(-1), priorityF(0), ignoreF(0), pointType(0),
						confirmC(0), onAddrPolyBoundary(false)  {}

	/**
	 * @brief   �Z���R�[�h�i11���j�擾
	 * @return �Z���R�[�h�i11���j
	 */
	CString getAddrcode11() const { return ( citycode + addrcode ); }

	long oid;            //!< �I�u�W�F�N�gID
	CString citycode;    //!< �s�撬���R�[�h
	CString addrcode;    //!< �Z���R�[�h
	CString gouNo;       //!< ���ԍ�
	CString yomi;        //!< �ǂ�
	int priorityF;       //!< �D��t���O
	int ignoreF;         //!< �X�敄�������t���O
	int pointType;       //!< �|�C���g������ʃR�[�h
	int confirmC;        //!< �m�F�X�e�[�^�X�R�[�h
	CString geoBldId;    //!< Geospace����ID
	CString ipcBldId;    //!< iPC����ID
	IGeometryPtr geo;    //!< �`��
	bool onAddrPolyBoundary; //!< �Z���|���S���̋��E��ɑ��݂��邩�itrue�F���݂���Afalse�F���݂��Ȃ��j
};

/**
 * @brief �S�p��
 * @param[in]  str �ϊ��Ώە�����
 * @return �S�p������������
 */
inline CString ToFull(LPCTSTR str)
{
	INT nTextLen = lstrlen( str ) + 1;
	CString strTextBuf;
	LCMapString( GetUserDefaultLCID(), LCMAP_FULLWIDTH, str, nTextLen, strTextBuf.GetBuffer(nTextLen), nTextLen );

	CString strResult(strTextBuf);
	strTextBuf.ReleaseBuffer();
	return strResult;
}

/**
 * @brief �g���R�[�h���X�g���
 */
struct CountInfo
{
	/**
	 * @brief   �R���X�g���N�^
	 */
	CountInfo() {}

	/**
	 * @brief   �R���X�g���N�^
	 * @param   layerName [in]  ���C����
	 */
	CountInfo(const CString& layerName) :
					layer(layerName), totalCount(0), importCount(0), exceptCount(0) {}


	/**
	 * @brief   ���O�o�͗p������擾
	 * @return ���O�o�͗p������
	 */
	CString getLogMsg() const { 
		return uh::str_util::format(_T("%s\t%ld\t%ld\t%ld\n"),
									layer, totalCount, importCount, exceptCount);
	}

	/**
	 * @brief   �C���|�[�g�������Z�b�g
	 */
	void resetImportCount() { importCount = 0; }


	CString layer;      //!< ���C����
	long totalCount;    //!< �ΏۑS����
	long importCount;   //!< �C���|�[�g����
	long exceptCount;   //!< �C���|�[�g���O����
};

/**
 * @brief ���s���O���b�Z�[�W�o��
 * @param[in]  msg           �o�̓��b�Z�[�W
 * @param[in]  runMsgStatus  ���s���O�o�̓X�e�[�^�X
 * @param[out] isWriteStdOut �W���o�͂ɏo�͂��邩�ǂ����itrue�F����Afalse�F���Ȃ��j
 */
void writeMessage(const CString& msg, Utility::RunLogMsgStatus runMsgStatus, bool isWriteStdOut);

/**
 * @brief �G���[���O���b�Z�[�W�o��
 * @param[in]  layerName     ���C����
 * @param[in]  oid           �I�u�W�F�N�gID
 * @param[in]  errLevel      �G���[���x��
 * @param[in]  msg           ���b�Z�[�W
 * @param[in]  valueList     �l���X�g
 */
void writeErrMsg(const CString& layerName, 
				 long oid,
				 const CString& errLevel, 
				 const CString& msg,
				 const std::vector<CString>& valueList);

/**
 * @brief �G���[���O���b�Z�[�W�o��
 * @param[in]  layerName     ���C����
 * @param[in]  oid           �I�u�W�F�N�gID
 * @param[in]  errLevel      �G���[���x��
 * @param[in]  msg           ���b�Z�[�W
 * @param[in]  valueList     �l
 */
void writeErrMsg(const CString& layerName, 
				 long oid,
				 const CString& errLevel, 
				 const CString& msg,
				 const CString& value);