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
#include "AddrDataCreator.h"
#include "Utility.h"
#include "Logger.h"
#include "GsSchema.h"

#include <sindy/workspace.h>
#include <TDC/sindylib_core/FieldMap.h>
#include <TDC/useful_headers/str_util.h>
#include <TDC/useful_headers/text_util.h>
#include <sindy/workspace.h>
#include <WinLib/Str2.h>

using namespace std;
using namespace sindy;
using namespace sindy::schema;
using namespace Utility;
using namespace gs::schema;

AddrDataCreator::AddrDataCreator(const Arguments& arg) : BaseDataCreator(arg) {}

bool AddrDataCreator::init()
{
	bool ret = true;

	if( !BaseDataCreator::initBase() )
		return false;

	// ���s���O�փI�v�V��������������
	RUNLOGGER.Log(uh::str_util::format(_T("-%s\t%s\n"), 
		(CString)arg::k_imp_type, m_args.m_impTypeStr), false);
	RUNLOGGER.Log(uh::str_util::format(_T("-%s\t%s\n"),
		(CString)arg::k_edt_addr_db, m_args.m_edtAddrDb), false);
	RUNLOGGER.Log(uh::str_util::format(_T("-%s\t%s\n"), 
		(CString)arg::k_gs_addr_db, m_args.m_gsAddrDb), false);
	RUNLOGGER.Log(uh::str_util::format(_T("-%s\t%s\n"),
		(CString)arg::k_run_log, m_args.m_runLog), false);
	RUNLOGGER.Log(uh::str_util::format(_T("-%s\t%s\n"), 
		(CString)arg::k_citylist, m_args.m_cityList), false);
	RUNLOGGER.Log(uh::str_util::format(_T("-%s\t%s\n"), 
		(CString)arg::k_extcode_list, m_args.m_extcodeList), false);
	RUNLOGGER.Log(_T("\n"), false);

	writeMessage(
			uh::str_util::format(_T("�f�[�^�x�[�X�ڑ��E�t�B�[�`���N���X�擾:%s\n"),
			m_args.m_edtAddrDb ),
			RunLogMsgStatus::Info,
			true);
		
		// SiNDY�i�Z���E�ҏW�j�ڑ�
	m_edtWorkspace = create_workspace(m_args.m_edtAddrDb);
	if (!m_edtWorkspace)
	{
		writeMessage(
				uh::str_util::format(_T("SiNDY�i�Z���E�ҏW�j�ւ̐ڑ��Ɏ��s�F%s\n"),
				m_args.m_edtAddrDb ),
				RunLogMsgStatus::Error,
				true);
		ret = false;
	}

	// GS�[�i�f�[�^(�Z���j�ڑ�
	auto gsAddrWorkspace = create_workspace(m_args.m_gsAddrDb);
	if (!gsAddrWorkspace)
	{
		writeMessage(
				uh::str_util::format(_T("GS�[�i�f�[�^(�Z���j�ւ̐ڑ��Ɏ��s�F%s\n"),
				m_args.m_gsAddrDb ),
				RunLogMsgStatus::Error,
				true);
		ret = false;
	}

	if( !ret )
	{
		writeMessage(uh::str_util::format(_T("�G���[�I��\n")), Utility::RunLogMsgStatus::Error, false);
		return ret;
	}

	// �e��t�B�[�`���N���X�E�e�[�u���擾

	m_edtLayerInfos.emplace(addr_polygon::kTableName, true);
	if( !m_edtLayerInfos[addr_polygon::kTableName].setFeatureClassInfo(
												m_edtWorkspace,
												addr_polygon::kTableName) )
	{
		writeMessage(
				uh::str_util::format(_T("SiNDY�i�Z���E�ҏW�j��%s�t�B�[�`���N���X�擾�Ɏ��s\n"),
				addr_polygon::kTableName ),
				RunLogMsgStatus::Error,
				true);
		ret = false;
	}

	m_baseLayerInfos.emplace(gs_addr_polygon::kTableName, true);
	if( !m_baseLayerInfos[gs_addr_polygon::kTableName].setFeatureClassInfo(
													gsAddrWorkspace,
													gs_addr_polygon::kTableName) )
	{
		writeMessage(
				uh::str_util::format(_T("GS�[�i�f�[�^(�Z���j��%s�t�B�[�`���N���X�擾�Ɏ��s\n"),
				gs_addr_polygon::kTableName ),
				RunLogMsgStatus::Error,
				true);
		ret = false;
	}

	// �C���|�[�g�^�C�v���uaddr_poly�v�ȊO�̏ꍇ�A�Z���|�C���g�֘A�̃t�B�[�`���N���X���擾
	if(Utility::ExecuteType::AddrPoly != m_args.m_impType)
	{
		m_edtLayerInfos.emplace(addr_point::kTableName, true);
		if( !m_edtLayerInfos[addr_point::kTableName].setFeatureClassInfo(
													m_edtWorkspace,
													addr_point::kTableName) )
		{
			writeMessage(
					uh::str_util::format(_T("SiNDY�i�Z���E�ҏW�j��%s�t�B�[�`���N���X�擾�Ɏ��s\n"),
					addr_point::kTableName ),
					RunLogMsgStatus::Error,
					true);
			ret = false;
		}

		m_baseLayerInfos.emplace(gs_addr_point::kTableName, true);
		if( !m_baseLayerInfos[gs_addr_point::kTableName].setFeatureClassInfo(
													gsAddrWorkspace,
													gs_addr_point::kTableName) )
		{
			writeMessage(
					uh::str_util::format(_T("GS�[�i�f�[�^(�Z���j��%s�t�B�[�`���N���X�擾�Ɏ��s\n"),
					gs_addr_point::kTableName ),
					RunLogMsgStatus::Error,
					true);
			ret = false;
		}
	}

	writeMessage(
			uh::str_util::format(_T("�s�撬���R�[�h���X�g�ǂݍ���:%s\n"),
			m_args.m_cityList ),
			RunLogMsgStatus::Info,
			true);

	// �s�撬���R�[�h���X�g�Ǎ���
	m_citycodeList = uh::text_util::ToTStrList((uh::tstring)m_args.m_cityList);
	if( m_citycodeList.empty() )
	{
		writeMessage(
				_T("�s�撬���R�[�h���X�g����\n"),
				RunLogMsgStatus::Error,
				true);
		ret = false;
	}

	// �g���R�[�h�R�[�h���X�g�Ǎ���
	if( !getExtcodeList(m_args.m_extcodeList))
		ret = false;

	if( !ret )
		writeMessage(uh::str_util::format(_T("�G���[�I��\n")), Utility::RunLogMsgStatus::Error, false);

	return ret;

}

bool AddrDataCreator::setExtcodeInfo(
		const CString& kcode,
		EdtAddrPolygonRec& addrPolygonRec,
		bool isKcode1
	)
{
	CString fourteenCode, compStr;
	// �g���R�[�h��1��2���ŎQ�ƃt�B�[���h���قȂ�
	if( isKcode1 )
		fourteenCode = addrPolygonRec.citycode + addrPolygonRec.addrcode1;
	else
		fourteenCode = addrPolygonRec.citycode + addrPolygonRec.addrcode2;

	// �����l�̏ꍇ�A�����ݒ肵�Ȃ��iEdtAddrPolygonRec�̏����l�̂܂܁j
	// �����l�́A�g���R�[�h1�E�g���R�[�h2�ŋ���
	if( 0 == kcode.CompareNoCase( _T("000") ) )
		return true;

	fourteenCode += kcode;

	// �����l�ȊO�ŁA�g���R�[�h���X�g�ɑ��݂��Ȃ��ꍇ�A�G���[
	auto excodeListRecIt = m_extcodeListMap.find(fourteenCode);
	if( m_extcodeListMap.end() == excodeListRecIt )
		return false;

	// �g���R�[�h���X�g����擾�������́E�ǂ݂����ꂼ��ݒ�
	// �g���R�[�h1��2���ŁA�i�[��̃t�B�[���h���قȂ�
	auto extcodeListRec = (*excodeListRecIt).second;

	if( isKcode1 )
	{
		addrPolygonRec.extName1 = extcodeListRec.extName;
		addrPolygonRec.extYomi1 = extcodeListRec.extYomi;
	}
	else
	{
		addrPolygonRec.extName2 = extcodeListRec.extName;
		addrPolygonRec.extYomi2 = extcodeListRec.extYomi;
	}
	
	return true;
}

bool AddrDataCreator::setEdtAddrPolygonRec(
	const IFeaturePtr& feature,
	EdtAddrPolygonRec& polyRec
	)
{
	bool ret = true;

	auto& baseFieldMap = m_baseLayerInfos[gs_addr_polygon::kTableName].getFieldMap();

	CComVariant citycode, addrcode1, addrcode2, k_code1, k_code2, gaikufugo, addrClass;
	if ( !getValue(feature, baseFieldMap.FindField(gs_addr_polygon::kCityCode), citycode) ||
	     !getValue(feature, baseFieldMap.FindField(gs_addr_polygon::kAddrCode1), addrcode1) || 
	     !getValue(feature, baseFieldMap.FindField(gs_addr_polygon::kAddrCode2), addrcode2) || 
	     !getValue(feature, baseFieldMap.FindField(gs_addr_polygon::kKCode1), k_code1) || 
	     !getValue(feature, baseFieldMap.FindField(gs_addr_polygon::kKCode2), k_code2) || 
	     !getValue(feature, baseFieldMap.FindField(gs_addr_polygon::kGaikuFugo), gaikufugo) || 
	     !getValue(feature, baseFieldMap.FindField(gs_addr_polygon::kAddrClass), addrClass) || 
	     FAILED(feature->get_OID(&polyRec.oid) ) )
	{
			writeErrMsg( gs_addr_polygon::kTableName, polyRec.oid,
						_T("ERROR"), _T("Feature�����get�Ɏ��s"), _T(""));
			return false;
	}
	
	polyRec.citycode = citycode.bstrVal;
	polyRec.addrcode1 = addrcode1.bstrVal;
	polyRec.setAddrcode2(addrcode2.bstrVal);
	polyRec.gaikuFugo = ( 0 == (gaikufugo.lVal) ) ? _T("") : uh::str_util::ToString(gaikufugo.lVal);
	polyRec.addrType = addrClass.lVal;
	polyRec.colorCode = 0;
	polyRec.geo = feature->GetShapeCopy();

	CString kcode1 = k_code1.bstrVal;
	if( !setExtcodeInfo(kcode1, polyRec, true) )
	{
		std::vector<CString> infos = boost::assign::list_of
			( polyRec.citycode )
			( polyRec.addrcode1 )
			( kcode1 );

		writeErrMsg(
				gs_addr_polygon::kTableName,
				polyRec.oid,
				_T("ERROR"), 
				_T("�g���R�[�h1�ɑ΂��閼�́E�ǂ݂̎擾�Ɏ��s"),
				infos
			);

		ret = false;
	}

	// �Z���R�[�h2�����ݒ�̏ꍇ�̂݊g�����́i�ʏ́j�E�g���ǂ݁i�ʏ́j���s��
	CString kcode2 = k_code2.bstrVal;
	if( !polyRec.addrcode2.IsEmpty() && !setExtcodeInfo(kcode2, polyRec, false) )
	{
		std::vector<CString> infos = boost::assign::list_of
			( polyRec.citycode )
			( polyRec.addrcode2 )
			( kcode2 );

		writeErrMsg(
				gs_addr_polygon::kTableName,
				polyRec.oid,
				_T("ERROR"), 
				_T("�g���R�[�h2�ɑ΂��閼�́E�ǂ݂̎擾�Ɏ��s"),
				infos
			);

		ret = false;
	}

	return ret;
}

bool AddrDataCreator::setConfirmC(
	EdtAddrPointRec& addrPointRec,
	const CString& sourceCd
	)
{
	// �Ή��\�ɑ��݂��Ȃ��ꍇ�A�G���[
	auto confirmIt = convertMap.find(sourceCd);
	if( convertMap.end() == confirmIt )
		return false;

	addrPointRec.confirmC = (*confirmIt).second;
	return true;
}

CString AddrDataCreator::toHalfGouNo(
	const CString& str
	)
{
	// AttrGouPointDlg.h���炻�̂܂ܗ��p

	LPTSTR lpszOriginalText = (LPTSTR)((LPCTSTR)str);
	INT nTextLen = lstrlen( lpszOriginalText ) + 1;
	LPTSTR lpszText = (LPTSTR)_alloca( nTextLen * sizeof(TCHAR) * 2 );	// 2�{�̗̈���m��
	// �S�đS�p�ɂ��Ă��܂�
	LCMapString( GetUserDefaultLCID(), LCMAP_FULLWIDTH, lpszOriginalText, nTextLen, lpszText, nTextLen * 2 );

	CString strText( lpszText );
	CString strRet;
	LONG lLen = CString_GetLength2( strText );

	for( LONG i = 0; i < lLen; i++ )
	{
		CString str = CString_GetAt2( strText, i );
		LPSTR lpstr = CT2A( (LPTSTR)((LPCTSTR)str) );
		LPTSTR lptstr = (LPTSTR)((LPCTSTR)str);

		if( ( 0x81 == (unsigned char)((lpstr[0])&0xff) ) && ( 0x94 == (unsigned char)((lpstr[1])&0xff) ) )
			strRet += _T("#");
		else if( ( 0x81 == (unsigned char)((lpstr[0])&0xff) ) && ( 0x96 == (unsigned char)((lpstr[1])&0xff) ) )
			strRet += _T("*");
		else if( ( 0x81 == (unsigned char)((lpstr[0])&0xff) ) && ( 0x7c == (unsigned char)((lpstr[1])&0xff) ) )
			strRet += _T("-");
		else if( ( 0x81 == (unsigned char)((lpstr[0])&0xff) ) && ( 0x40 == (unsigned char)((lpstr[1])&0xff) ) )	// [bug7749]�X�y�[�X�̓��͂͂ł��Ȃ��悤�ɂ���
			strRet += _T("");
		// �����̏ꍇ�ɂ͔��p�ɒ���
		else if( ( 0x82 == (unsigned char)((lpstr[0])&0xff) ) && ( 0x4f <= (unsigned char)((lpstr[1])&0xff) ) && ( (unsigned char)((lpstr[1])&0xff) <= 0x58 ) )
		{
			LPTSTR lpszNumberText = (LPTSTR)_alloca( 2 * sizeof(TCHAR) );
			// ���p�ɂ��Ă��܂�
			LCMapString( GetUserDefaultLCID(), LCMAP_HALFWIDTH, lptstr, 2, lpszNumberText, 2 );

			strRet += lpszNumberText;
		}
		else
		{
			// �擪��#�ł���Ȃ�΁A�������̂܂܂�������
			if( 0x30 <= (unsigned char)((lpstr[0])&0xff) && (unsigned char)(lpstr[0]&0xff) <= 0x39 )
				strRet += lpstr;
			else
			{
				if( strRet.Find( _T("#") ) == 0 )
					strRet += lpstr;
				else
				{
					// �������͂̍ۂ�#�������Ă��Ȃ�������擪��#��t����
					CString strTemp = strRet;
					strRet = _T("#");
					strRet += strTemp;
					strRet += lpstr;
				}
			}
		}
	}
	return strRet;
}

void AddrDataCreator::setGouNo(
	EdtAddrPointRec& addrPointRec,
	const std::vector<CString>& addressList,
	const CString& shinraiF,
	const CString& hyouki
	)
{
	CString gouNo;
	for(const auto& address : addressList)
	{
		// �i�[�l�������l�i���p�X�y�[�X�j�̏ꍇ�A�X�L�b�v
		if( 0 == address.Compare(_T(" ")) )
			continue;

		// ���p�n�C�t���őO����A��
		gouNo += ( gouNo.IsEmpty() ) ? address : ( _T("-") + address);
	}

	// ���̎��_�Œl����Ȃ���ꏈ���A�l������ΘA����������������ԍ��ɐݒ�
	if( gouNo.IsEmpty() )
	{
		// �M���t���O���u2�v�i�}�ʂŊm�F�����������Ɣ��f�����ꍇ�j�܂��́A
		// �\�L���u2�v�i�}�ʂŊm�F���Z���������Ȃ��Ă悢�ƌ`�Ɣ��f�����ꍇ�j�̏ꍇ�A
		// �|�C���g������ʂ��u5�v�i�t�������j�ɂ���B
		// �����łȂ���΁A���ԍ��u*�v�ɂ���
		if( 0 == shinraiF.Compare(_T("2")) || 0 == hyouki.Compare(_T("2")))
		{
			addrPointRec.pointType = 5;
			addrPointRec.gouNo = _T("");
		}
		else
			addrPointRec.gouNo = _T("*");
	}
	else
		addrPointRec.gouNo = toHalfGouNo(gouNo);
}

bool AddrDataCreator::setEdtAddrPointRec(
	const IFeaturePtr& feature,
	const EdtAddrPolygonRec& addrPolygonRec,
	EdtAddrPointRec& addrPointRec
	)
{
	bool ret = true;

	auto& fieldMap = m_baseLayerInfos[gs_addr_point::kTableName].getFieldMap();

	CComVariant placeCd, addr1, addr2, addr3, geoBfId, ipcBjId, sourceCd;
	CComVariant shinraiF, hyouki, adoptF;
	if( !getValue(feature, fieldMap.FindField(gs_addr_point::kPlaceCd), placeCd) || 
	    !getValue(feature, fieldMap.FindField(gs_addr_point::kAddress1), addr1) || 
	    !getValue(feature, fieldMap.FindField(gs_addr_point::kAddress2), addr2) || 
	    !getValue(feature, fieldMap.FindField(gs_addr_point::kAddress3), addr3) || 
	    !getValue(feature, fieldMap.FindField(gs_addr_point::kGeoBFid), geoBfId) || 
	    !getValue(feature, fieldMap.FindField(gs_addr_point::kIpcObjId), ipcBjId) || 
	    !getValue(feature, fieldMap.FindField(gs_addr_point::kSourceCd), sourceCd) || 
	    !getValue(feature, fieldMap.FindField(gs_addr_point::kShinrai), shinraiF) || 
	    !getValue(feature, fieldMap.FindField(gs_addr_point::kHyoki), hyouki) || 
	    !getValue(feature, fieldMap.FindField(gs_addr_point::kAdopt), adoptF) || 
	    FAILED(feature->get_OID(&addrPointRec.oid)) )		
	{
			writeErrMsg( gs_addr_polygon::kTableName, addrPointRec.oid,
						_T("ERROR"), _T("Feature�����get�Ɏ��s"), _T(""));
			ret = false;
	}

	std::vector<CString> addressList = boost::assign::list_of
							(addr1.bstrVal)
							(addr2.bstrVal)
							(addr3.bstrVal);

	// �s�撬���R�[�h�A�Z���R�[�h
	CString citycode = CString(placeCd).Mid(0, 5);
	CString addrcode = CString(placeCd).Mid(5, 6);

	// �Z���|���S���ƏZ���|�C���g�̎s�撬���R�[�h�A�Z���R�[�h1�̂����ꂩ���قȂ�ꍇ
	if( 0 != addrPolygonRec.citycode.Compare(citycode) ||
		0 != addrPolygonRec.addrcode1.Compare(addrcode) )
	{
		// �Z���|�C���g�̎s�撬���R�[�h�A�Z���R�[�h��ݒ�
		addrPointRec.citycode = citycode;
		addrPointRec.addrcode = addrcode;
	}
	else
	{
		// �Z���|���S���ƏZ���|�C���g�̎s�撬���R�[�h�A�Z���R�[�h1��������v����ꍇ

		// ��r�p��ADDRESS1���甼�p�X�y�[�X������
		CString address1 = addr1.bstrVal;
		address1.Replace(_T(" "), _T(""));

		// �Z���|���S���̊X�敄���܂��͊g���R�[�h1�ɑ΂��閼�̂̂����ꂩ���A
		// �Z���|�C���g��ADDRESS1�l�ƈ�v����ꍇ
		if( ( 0 == addrPolygonRec.gaikuFugo.Compare(uh::str_util::ToHalf(address1)) ||
			  0 == addrPolygonRec.extName1.Compare(address1) ) &&
			  !address1.IsEmpty()
			)
		{
			// ���ԍ��ɐݒ肷��l��ADDRESS2��ADDRESS3�̂�
			addressList = boost::assign::list_of
					(addr2.bstrVal)
					(addr3.bstrVal);
		}
		else
		{
			// �X�敄���܂��͊g���R�[�h1�����݂���ꍇ
			if( !address1.IsEmpty() &&
				( !addrPolygonRec.gaikuFugo.IsEmpty() ||
				  !addrPolygonRec.extName1.IsEmpty() ) )
				// �X�敄���������t���O��1�ɂ���
				addrPointRec.ignoreF = 1;
			}
	}

	// ADDRESS�̊i�[�l����
	bool beforeAddressExist = ( 0 != addressList[0].Compare(_T(" ")) );
	for(unsigned int i = 1; i < addressList.size(); ++i)
	{
		bool isAddressExist = ( 0 != addressList[i].Compare(_T(" ")) );
		if( !beforeAddressExist && isAddressExist )
		{
			writeErrMsg(
					gs_addr_point::kTableName,
					addrPointRec.oid,
					_T("ERROR"), 
					_T("ADDRESS�t�B�[���h���������iGS�[�i�f�[�^�j"),
					addressList
				);

			ret = false;
			break;
		}
		beforeAddressExist = isAddressExist;
	}

	// ���ԍ�
	setGouNo(addrPointRec, addressList, CString(shinraiF.bstrVal), CString(hyouki.bstrVal));

	// �D��t���O�i�ꗥ�u0�v�j
	addrPointRec.priorityF = 0;
		
	// Geospace����ID
	addrPointRec.geoBldId = geoBfId.bstrVal;

	// iPC����ID�i�擾����������̐擪2�����͏������Đݒ�j
	CString ipcBldId = ipcBjId.bstrVal;
	addrPointRec.ipcBldId = ( 0 == ipcBldId.Compare(_T(" ")) ) ? _T("") : ipcBldId.Mid(2);

	// �`��
	addrPointRec.geo = feature->GetShapeCopy();

	// �m�F�X�e�[�^�X�R�[�h
	if( !setConfirmC(addrPointRec, CString(sourceCd.bstrVal)) )
	{
		writeErrMsg(
				gs_addr_point::kTableName,
				addrPointRec.oid,
				_T("ERROR"), 
				_T("�o�T�R�[�h�l���s���iGS�[�i�f�[�^�j"),
				CString(sourceCd.bstrVal)
			);
		ret = false;
	}

	// �|�C���g������ʃR�[�h

	// �̗p�t���O��null�̏ꍇ�A�[���I��0�ɂ��Ĕ��肷��
	long adopt = ( adoptF.vt == VT_NULL ) ? 0 : adoptF.lVal;
	if( 0 != adopt ) // �̗p�t���O����ł͂Ȃ��ꍇ
		addrPointRec.pointType = 2;
	else if( 0 == addrPointRec.pointType )
		addrPointRec.pointType = ( addrPointRec.ipcBldId.IsEmpty() ) ? 3 : 1;

	return ret;
}

bool AddrDataCreator::getExtcodeList(
		const CString& extcodeListPath
	)
{
	static std::map<int, CString> extFieldIdxMap = boost::assign::map_list_of
		( 0, _T("14���R�[�h"))
		( 1, _T("����"))
		( 2, _T("�ǂ�"));
	const int EXTCODE_COL_NUM = extFieldIdxMap.size();

	// �t�@�C������1�s���ǂݍ���Ŋi�[
	std::ifstream ifs( extcodeListPath );
	if( !ifs.is_open() )
	{
		writeMessage(
				_T("�g���R�[�h���X�g���ǂݍ��߂܂���\n"),
				RunLogMsgStatus::Error,
				true);
		return false;
	}

	bool ret = true;
	std::string line;
	int lineCount = 0;
	while(std::getline(ifs, line))
	{
		++lineCount;

		// ��s�͎擾�ΏۊO
		if( line.empty() )
			continue;

		auto splitValues = uh::str_util::split(CString(line.c_str()), _T("\t"), true);
		if( EXTCODE_COL_NUM != splitValues.size() )
		{
			writeMessage(
					uh::str_util::format(_T("�g���R�[�h���X�g�̗񐔂�%d��ł͂���܂���i%d�s�� ��:%d�j\n"),
					EXTCODE_COL_NUM, lineCount, splitValues.size() ),
					RunLogMsgStatus::Error,
					true);
			ret = false;
			continue;
		}

		for(unsigned int i = 0; i < splitValues.size(); ++i)
		{
			if( splitValues[i].IsEmpty() )
			{
				writeMessage(
						uh::str_util::format(_T("�g���R�[�h���X�g�̗�l����ł��i%d�s�� ��:%s�j\n"),
						lineCount, extFieldIdxMap[i] ),
						RunLogMsgStatus::Error,
						true);
				ret = false;
			}
		}
		if( !ret )
			continue;

		m_extcodeListMap.emplace(
				std::piecewise_construct,
				std::forward_as_tuple(splitValues[0]),
				std::forward_as_tuple(splitValues[0], splitValues[1], splitValues[2]));
	}

	return ret;
}

bool AddrDataCreator::getAddrPolygon(
	std::vector<EdtAddrPolygonRec>& addrPolygonRecs,
	const CString& citycode,
	CountInfo& countInfo
	)
{
	bool ret = true;

	// �s�撬���R�[�h���w�肵�ĊY���I�u�W�F�N�g���擾
	CString whereClause = uh::str_util::format(_T("%s = '%s'"), gs_addr_polygon::kCityCode, citycode);
	auto& cursor = searchByWhereClause(m_baseLayerInfos[gs_addr_polygon::kTableName], whereClause);

	if( !cursor )
	{
		writeMessage(
				uh::str_util::format(_T("GS�[�i�f�[�^(�Z���F%s�j�̌����Ɏ��s�i�s�撬���R�[�h�F%s�j\n"),
				gs_addr_polygon::kTableName, citycode ),
				RunLogMsgStatus::Error,
				true);
		return false;
	}

	IFeaturePtr feature;
	while (S_OK == cursor->NextFeature(&feature) && feature)
	{
		++(countInfo.totalCount);

		// EdtAddrPolygonRec�ɒl��ݒ�
		EdtAddrPolygonRec polyRec;
		ret &= setEdtAddrPolygonRec(feature, polyRec);

		addrPolygonRecs.push_back(polyRec);
	}
	return ret;
}

bool AddrDataCreator::getAddrPoint(
	const std::vector<EdtAddrPolygonRec>& addrPolygonRecs,
	std::vector<EdtAddrPointRec>& addrPointRecs,
	CountInfo& countInfo
	)
{
	bool ret = true;

	// �擾�����Z���|���S�����A�������s��
	for(auto& polyRec : addrPolygonRecs)
	{
		std::vector<long> touchOIds;

		// �Z���|���S���̋��E��ɑ��݂���Z���|�C���g���Ɏ擾
		if( !getBoundaryTouchPoints(polyRec, touchOIds) )
			return false;

		// �����Ώۂ̏Z���|���S���ɕ�܂����Z���|�C���g���擾
		ISpatialFilterPtr ipSpatialFilter(CLSID_SpatialFilter);
		ipSpatialFilter->putref_Geometry(polyRec.geo);
		ipSpatialFilter->PutSpatialRel(esriSpatialRelEnum::esriSpatialRelIntersects);

		IFeatureCursorPtr cursor;
		if (FAILED(m_baseLayerInfos[gs_addr_point::kTableName].
						getFeatureClass()->Search(ipSpatialFilter, VARIANT_FALSE, &cursor)))
		{
			writeMessage(
					uh::str_util::format(_T("GS�[�i�f�[�^(�Z���F%s�j�̌����Ɏ��s�i�Z���|���S��OID�F%ld�j\n"),
					gs_addr_point::kTableName, polyRec.oid ),
					RunLogMsgStatus::Error,
					true);
			return false;
		}

		IFeaturePtr pointFeature;
		while (S_OK == cursor->NextFeature(&pointFeature) && pointFeature)
		{
			++(countInfo.totalCount);

			CComVariant sourceCd;
			pointFeature->get_Value(
					m_baseLayerInfos[gs_addr_point::kTableName].getFieldMap().FindField(gs_addr_point::kSourceCd), &sourceCd);

			// �o�T�t���O���u9�v�i�ǉ������K�v�j�̏ꍇ�A�C���|�[�g�ΏۊO
			if( 0 == CString(sourceCd.bstrVal).Compare(_T("9")) )
			{
				++(countInfo.exceptCount);
				continue;
			}

			EdtAddrPointRec pointRec;
			ret &= setEdtAddrPointRec(pointFeature, polyRec, pointRec);

			// �擾�����Z���|�C���g���|���S���̋��E��ɑ��݂���ꍇ�A�t���O�𗧂ĂĂ���
			if( touchOIds.end() != std::find(touchOIds.begin(), touchOIds.end(), pointRec.oid ) )
				pointRec.onAddrPolyBoundary = true;

			addrPointRecs.push_back(pointRec);
		}

	}
	return ret;
}

bool AddrDataCreator::getBoundaryTouchPoints(
		const EdtAddrPolygonRec& addrPolygonRec,
		std::vector<long>& touchOIds
	)
{
	IGeometryPtr boundaryGeo = ((ITopologicalOperatorPtr)addrPolygonRec.geo)->GetBoundary();

	// �����Ώۂ̏Z���|���S���̊O���ɐڐG����Z���|�C���g���擾
	ISpatialFilterPtr ipSpatialFilter(CLSID_SpatialFilter);
	ipSpatialFilter->putref_Geometry(boundaryGeo);
	ipSpatialFilter->PutSpatialRel(esriSpatialRelEnum::esriSpatialRelIntersects);

	IFeatureCursorPtr cursor;
	if (FAILED(m_baseLayerInfos[gs_addr_point::kTableName].
					getFeatureClass()->Search(ipSpatialFilter, VARIANT_FALSE, &cursor)))
	{
		writeMessage(
				uh::str_util::format(_T("GS�[�i�f�[�^(�Z���F%s�j�̌����Ɏ��s�i�Z���|���S��OID�F%ld�j\n"),
				gs_addr_point::kTableName, addrPolygonRec.oid ),
				RunLogMsgStatus::Error,
				true);
		return false;
	}

	IFeaturePtr feature;
	while (S_OK == cursor->NextFeature(&feature) && feature)
	{
		long oid(0);
		if( FAILED(feature->get_OID(&oid)) )
			return false;

		touchOIds.push_back(oid);
	}
	return true;
}
bool AddrDataCreator::insertAddrPolygon(
	const std::vector<EdtAddrPolygonRec>& addrPolygonRecs,
	CountInfo& countInfo
	)
{
	bool ret = true;

	IFeatureCursorPtr insertCursor;
	if( FAILED(m_edtLayerInfos[addr_polygon::kTableName].getFeatureClass()->Insert(VARIANT_TRUE, &insertCursor)) )
	{
		writeErrMsg( addr_polygon::kTableName, 0L, _T("ERROR"), _T("Insert�Ɏ��s"), _T(""));
		return false;
	}

	IFeatureBufferPtr featureBuf;
	if( FAILED(m_edtLayerInfos[addr_polygon::kTableName].getFeatureClass()->CreateFeatureBuffer(&featureBuf)) )
	{
		writeErrMsg( addr_polygon::kTableName, 0L, _T("ERROR"), _T("CreateFeatureBuffer�Ɏ��s"), _T(""));
		return false;
	}

	auto& fieldMap = m_edtLayerInfos[addr_polygon::kTableName].getFieldMap();
	for(const auto& addrPolyRec : addrPolygonRecs)
	{
		// ���ʑ����l�ݒ�
		for (const auto& attr : m_commonAttrMap)
		{
			if( !putValue(featureBuf, fieldMap.FindField(attr.first), attr.second) )
			{
				writeErrMsg( gs_addr_polygon::kTableName, addrPolyRec.oid,
							_T("ERROR"), _T("FeatureBuffer�ւ�put�Ɏ��s"), _T(""));
				ret = false;
			}
		}

		// ���C���ʑ����l�ݒ�
		if( !putValue(featureBuf, fieldMap.FindField(addr_polygon::kCityCode), addrPolyRec.citycode) || 
			!putValue(featureBuf, fieldMap.FindField(addr_polygon::kAddrCode1), addrPolyRec.addrcode1) || 
			!putValue(featureBuf, fieldMap.FindField(addr_polygon::kAddrCode2), addrPolyRec.addrcode2) || 
			!putValue(featureBuf, fieldMap.FindField(addr_polygon::kExtName1), addrPolyRec.extName1) || 
			!putValue(featureBuf, fieldMap.FindField(addr_polygon::kExtYomi1), addrPolyRec.extYomi1) || 
			!putValue(featureBuf, fieldMap.FindField(addr_polygon::kExtName2), addrPolyRec.extName2) || 
			!putValue(featureBuf, fieldMap.FindField(addr_polygon::kExtYomi2), addrPolyRec.extYomi2) || 
			!putValue(featureBuf, fieldMap.FindField(addr_polygon::kGaikufugo),_ttol(addrPolyRec.gaikuFugo), 
									( 0 == addrPolyRec.gaikuFugo.Compare(_T("") )) ? true : false ) || 
			!putValue(featureBuf, fieldMap.FindField(addr_polygon::kAddrType), addrPolyRec.addrType) || 
			!putValue(featureBuf, fieldMap.FindField(addr_polygon::kColorCode), 0) || 
			FAILED(featureBuf->putref_Shape(addrPolyRec.geo)) )
		{
			writeErrMsg( gs_addr_polygon::kTableName, addrPolyRec.oid,
						_T("ERROR"), _T("FeatureBuffer�ւ�put�Ɏ��s"), _T(""));
			ret = false;
		}

		CComVariant oid(0);
		if( FAILED(insertCursor->InsertFeature(featureBuf, &oid)) )
		{
			writeErrMsg( addr_polygon::kTableName, addrPolyRec.oid,
						_T("ERROR"), _T("InsertFeature�Ɏ��s"), _T("") );
			ret = false;
			continue;
		}
		++(countInfo.importCount);
	}

	return ret;
}

bool AddrDataCreator::insertAddrPoint(
	const std::vector<EdtAddrPointRec>& addrPointRecs,
	CountInfo& countInfo
	)
{
	bool ret = true;

	IFeatureCursorPtr insertCursor;
	if( FAILED(m_edtLayerInfos[addr_point::kTableName].getFeatureClass()->Insert(VARIANT_TRUE, &insertCursor) ) )
	{
		writeErrMsg( addr_point::kTableName, 0L, _T("ERROR"), _T("Insert�Ɏ��s"), _T(""));
		return false;
	}

	auto fieldMap = m_edtLayerInfos[addr_point::kTableName].getFieldMap();

	IFeatureBufferPtr featureBuf;
	if( FAILED(m_edtLayerInfos[addr_point::kTableName].getFeatureClass()->CreateFeatureBuffer(&featureBuf) ) )
	{
		writeErrMsg( addr_point::kTableName, 0L, _T("ERROR"), _T("CreateFeatureBuffer�Ɏ��s"), _T(""));
		return false;
	}

	for(auto& pointRec : addrPointRecs)
	{
		// ���ʑ����l�ݒ�
		for (const auto& attr : m_commonAttrMap)
		{
			if( !putValue(featureBuf, fieldMap.FindField(attr.first), attr.second) )
			{
				writeErrMsg( gs_addr_point::kTableName, pointRec.oid,
							_T("ERROR"), _T("FeatureBuffer�ւ�put�Ɏ��s"), _T(""));
				ret = false;
			}
		}

		// ���C���ʑ����l�ݒ�
		if ( !putValue(featureBuf, fieldMap.FindField(addr_point::kCityCode), pointRec.citycode) || 
			 !putValue(featureBuf, fieldMap.FindField(addr_point::kAddrCode), pointRec.addrcode) ||
			 !putValue(featureBuf, fieldMap.FindField(addr_point::kGouNo), pointRec.gouNo) ||
			 !putValue(featureBuf, fieldMap.FindField(addr_point::kYomi), pointRec.yomi) ||
			 !putValue(featureBuf, fieldMap.FindField(addr_point::kPriority), pointRec.priorityF) ||
			 !putValue(featureBuf, fieldMap.FindField(addr_point::kIgnore), pointRec.ignoreF) ||
			 !putValue(featureBuf, fieldMap.FindField(addr_point::kPointType), pointRec.pointType) ||
			 !putValue(featureBuf, fieldMap.FindField(addr_point::kConfirm), pointRec.confirmC) ||
			 !putValue(featureBuf, fieldMap.FindField(addr_point::kGeoBldID), pointRec.geoBldId) ||
			 !putValue(featureBuf, fieldMap.FindField(addr_point::kIPCBldID), pointRec.ipcBldId) ||
			 FAILED(featureBuf->putref_Shape(pointRec.geo)) )
		{
			writeErrMsg( gs_addr_point::kTableName, pointRec.oid,
						_T("ERROR"), _T("FeatureBuffer�ւ�put�Ɏ��s"), _T(""));
			ret = false;
		}

		CComVariant oid(0);
		if( FAILED(insertCursor->InsertFeature(featureBuf, &oid)) )
		{
			writeErrMsg( gs_addr_point::kTableName, pointRec.oid,
						_T("ERROR"), _T("InsertFeature�Ɏ��s"), _T("") );
			ret = false;
			continue;
		}
		++(countInfo.importCount);

		// �Y���Z���|�C���g���Z���|���S���̋��E��ɑ��݂���ꍇ�A�G���[���O�ɏ��Ƃ��ďo��
		if( pointRec.onAddrPolyBoundary )
		{
			writeErrMsg(
					addr_point::kTableName,
					(long)oid.lVal,
					_T("WARNING"), 
					_T("�Z���|���S���̋��E��ɑ��݂���i�ҏW�f�[�^�j"),
					CString()
				);
		}

	}
	return ret;
}

bool AddrDataCreator::create()
{
	bool result = true;

	// �����J�E���g���
	std::map<CString, CountInfo> addrPolyCntInfos;
	std::map<CString, CountInfo> addrPointCntInfos;

	// �s�撬���R�[�h�P�̂ŏ��������{
	for(auto& citycode : m_citycodeList)
	{
		// ��̏ꍇ�X�L�b�v
		if( citycode.empty() )
			continue;

		addrPolyCntInfos.emplace(std::piecewise_construct,
									std::make_tuple(citycode.c_str()),
									std::make_tuple(addr_polygon::kTableName));
		addrPointCntInfos.emplace(std::piecewise_construct,
									std::make_tuple(citycode.c_str()),
									std::make_tuple(addr_point::kTableName));

		// �s�撬���R�[�h���s���ȏꍇ
		if( !std::regex_match(CStringW(citycode.c_str()).GetString(), citycode_re) )
		{
			writeMessage(uh::str_util::format(_T("�s�撬���R�[�h���s��:%s\n"), citycode.c_str()),
					RunLogMsgStatus::Error, false);
			result &= false;
			continue;
		}

		writeMessage(
				uh::str_util::format(_T("�s�撬���R�[�h�F%s\n"),
				citycode.c_str() ),
				RunLogMsgStatus::Info,
				true);
	
		// �Z���|���S���擾
		writeMessage(
				_T("GS�[�i���C������f�[�^�擾\n"), RunLogMsgStatus::Info, true);

		std::vector<EdtAddrPolygonRec> addrPolygonRecs;
		if( !getAddrPolygon(addrPolygonRecs,
							citycode.c_str(),
							addrPolyCntInfos[citycode.c_str()]) )
		{
			result &= false;
			continue;
		}

		std::vector<EdtAddrPointRec> addrPointRecs;

		// �C���|�[�g�^�C�v���uaddr_poly�v�ȊO�̏ꍇ�A�Z���|�C���g�������Ώۂɂ���
		if( Utility::ExecuteType::AddrPoly != m_args.m_impType )
		{
			if( !getAddrPoint(addrPolygonRecs, addrPointRecs, addrPointCntInfos[citycode.c_str()]) )
			{
				result &= false;
				continue;
			}
		}

		writeMessage(_T("�C���|�[�g����\n"), RunLogMsgStatus::Info, true);

		// �C���|�[�g����
		if( !startEdit() )
		{
			writeMessage(_T("�ҏW�J�n�Ɏ��s\n"), RunLogMsgStatus::Error, true);
			result &= false;
			continue;
		}

		bool ret = true;

		// �C���|�[�g�^�C�v���uaddr_point�v�ȊO�̏ꍇ�A�Z���|���S���̃C���|�[�g�������s��
		if( Utility::ExecuteType::AddrPoint != m_args.m_impType )
		{
			if( !insertAddrPolygon(addrPolygonRecs, addrPolyCntInfos[citycode.c_str()]) )
				ret = false;
		}

		// �C���|�[�g�^�C�v���uaddr_poly�v�ȊO�̏ꍇ�A�Z���|�C���g�̃C���|�[�g�������s��
		if( Utility::ExecuteType::AddrPoly != m_args.m_impType )
		{
			if( !insertAddrPoint(addrPointRecs, addrPointCntInfos[citycode.c_str()]) )
				ret = false;
		}

		// �C���|�[�g�������ʂɉ����ĕҏW�I��/�j����؂蕪��
		if( ret )
		{
			// �ҏW�I��
			if ( !stopEdit(ret) )
			{
				writeMessage(_T("�ҏW�I���Ɏ��s\n"), RunLogMsgStatus::Error, true);
				addrPolyCntInfos[citycode.c_str()].resetImportCount();
				addrPointCntInfos[citycode.c_str()].resetImportCount();
			}
		}
		else
		{
			// �ҏW�j��
			abort();
			addrPolyCntInfos[citycode.c_str()].resetImportCount();
			addrPointCntInfos[citycode.c_str()].resetImportCount();
		}
		result &= ret;
	}

	// �e�[�u�����v���X�V
	if( Utility::ExecuteType::AddrPoint != m_args.m_impType )
	{
		if( !tableAnalyze(addr_polygon::kTableName) )
		{
			writeMessage(uh::str_util::format(_T("�e�[�u�����v���X�V�Ɏ��s:%s\n"),
							addr_polygon::kTableName), RunLogMsgStatus::Error, true);
			result = false;
		}
	}

	if( Utility::ExecuteType::AddrPoly != m_args.m_impType )
	{
		if( !tableAnalyze(addr_point::kTableName) )
		{
			writeMessage(uh::str_util::format(_T("�e�[�u�����v���X�V�Ɏ��s:%s\n"),
							addr_point::kTableName), RunLogMsgStatus::Error, true);
			result = false;
		}
	}
				
	if( result )
		writeMessage(uh::str_util::format(_T("����I��\n")), RunLogMsgStatus::Info, false);
	else
		writeMessage(uh::str_util::format(_T("�G���[�I��\n")), RunLogMsgStatus::Error, false);

	writeMessage(uh::str_util::format(_T("\n�s�撬���R�[�h\t���C����\t�S����\t�C���|�[�g����\t���O����\n")),
					RunLogMsgStatus::Normal, false);
	for(const auto& citycode : m_citycodeList)
	{
		if( Utility::ExecuteType::AddrPoint != m_args.m_impType )
			writeMessage(uh::str_util::format(_T("%s\t%s"),
								citycode.c_str(),
							addrPolyCntInfos[citycode.c_str()].getLogMsg()),
							RunLogMsgStatus::Normal, false);

		if( Utility::ExecuteType::AddrPoly != m_args.m_impType )
			writeMessage(uh::str_util::format(_T("%s\t%s"),
								citycode.c_str(),
							addrPointCntInfos[citycode.c_str()].getLogMsg()),
							RunLogMsgStatus::Normal, false);
	}

	return result;
}

