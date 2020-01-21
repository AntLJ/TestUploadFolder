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
#include "GouPoint.h"
#include "CodeConverter.h"
#include "AreaCodeHelper.h"
#include "AddrGlobals.h"

#include <ArcHelperEx/GlobalFunctions.h>

namespace addr
{
using namespace sindy;
using namespace sindy::schema;

// 20���Z���R�[�h�擾
CString CGouPoint::GetAddrCode( bool bChar /* = false */, bool bCodeDBSaved /* = false */ )
{
	// ���ԍ��̐擪��[*]��������ϊ��ΏۊO
	if( _T("*") == GetGouNo() )
		return _T("");

	// �Z���R�[�h��������L���b�V������
	if( m_strAddrCode.IsEmpty() )
	{
		// �s���E�E���|�C���g�ŕ����t���Z��������(�����t���l�����Ȃ��ꍇ�́A�����I�ɒʏ�Z��)
		addrStatus eStatus = bChar? IsCharAddr() : kNormal;
		if( kNormal == eStatus )	// �ʏ�Z��
			m_strAddrCode = GetAddrCode_NoChar();
		else						// �����t���Z��
			m_strAddrCode = GetAddrCode_Char( bCodeDBSaved );
	}
	
	return m_strAddrCode;
}

// �Z�����̎擾
CString CGouPoint::GetAddrName( bool bChar /* = false */ )
{
	if( m_strAddrName.IsEmpty() )
	{
		const CString& strAddrCode = GetAddrCode(bChar);
		if( strAddrCode.GetLength() == 20 )
		{
			m_strAddrName = g_cAreaCodeHelper.IsConnected()? g_cAreaCodeHelper.GetAddrKanji(strAddrCode) : _T("");
			if( !m_strAddrName.IsEmpty() )
			{
				CString strPntCode = strAddrCode.Mid(11, 5);
				CString strGouCode = strAddrCode.Mid(16);
				if( !bChar )
				{
					long lPnt = _ttol( strPntCode );
					long lGou = _ttol( strGouCode );
					if( lPnt > 0 && lGou > 0 )
						m_strAddrName.AppendFormat( _T("%d-%d"), lPnt, lGou );
					else if( lPnt > 0 && lGou == 0 )
						m_strAddrName.AppendFormat( _T("%d"), lPnt );					
				}
				else
				{
					bool bPntExist = strPntCode != _T("00000");
					bool bGouExist = strGouCode != _T("0000");

					if( bPntExist && bGouExist )
					{
						m_strAddrName.AppendFormat( _T("%s-%s"), 
													IsNormalPnt(strPntCode)? strPntCode.Trim('0') : CCodeConverter::GetCodeConverter().GetPntName(strAddrCode.Left(11), strPntCode),
													IsNormalGou(strGouCode)? strGouCode.Trim('0') : CCodeConverter::GetCodeConverter().GetGouName(strAddrCode.Left(11), strGouCode) );
					}
					else if( bPntExist && !bGouExist )
					{
						m_strAddrName.AppendFormat( _T("%s"), IsNormalPnt(strPntCode)? strPntCode.Trim('0') : CCodeConverter::GetCodeConverter().GetPntName(strAddrCode.Left(11), strPntCode) );
					}
				}
				m_strAddrName = GetZenkakuData( m_strAddrName );
			}
		}
	}
	return m_strAddrName;
}

// ���|�C���g�������錚���擾
CBuilding* CGouPoint::GetBuilding( IFeatureClassPtr ipBldClass /* = NULL */ )
{
	iterator itr = find(sindyTableType::building);
	if( itr != end() )
		return static_cast<CBuilding*>(&**itr->second.begin());
	else
	{
		// �����e�[�u�����w�莞�́A���ꃏ�[�N�X�y�[�X����擾
		if( !ipBldClass )
			((IFeatureWorkspacePtr)GetWorkspace())->OpenFeatureClass( _bstr_t(building::kTableName), &ipBldClass );

		if( ipBldClass )
		{
			ISpatialFilterPtr ipSpFilter( AheInitSpatialFilter(NULL, GetShapeCopy()) );
			if( ipSpFilter )
			{
				// ��ɍi��Ȃ��Ƃ��́ANULL�ɂ���
				long lCount = 0;
				ipBldClass->FeatureCount( ipSpFilter, &lCount );
				if( lCount != 1 )
					return NULL;

				IFeatureCursorPtr ipCursor;
				ipBldClass->Search( ipSpFilter, VARIANT_FALSE, &ipCursor );
				IFeaturePtr ipFeature;
				if( ipCursor && ipCursor->NextFeature(&ipFeature) == S_OK )
				{
					// ��x�T������i�[���Ă���
					CSPBuilding cFeat( new CBuilding(ipFeature) );
					push_back( cFeat );
					return &*cFeat;
				}
			}
		}
	}

	return NULL;
}

///////////////////////////
// ����J�֐�
///////////////////////////

// �g�[�N�������֐�
void CGouPoint::Tokenize( const CString& strData, const wchar_t* lpcszSep, GOUTOKENLIST& rList )
{
	rList.clear();
	CStringW strDataW( strData );
	int iCurPos = 0;
	CString strToken( strDataW.Tokenize( lpcszSep, iCurPos ) );
	while( !strToken.IsEmpty() )
	{
		rList.push_back( strToken );
		strToken = CW2CT(strDataW.Tokenize(lpcszSep, iCurPos));
	}
}

// �g�[�N���擾�֐�
CString CGouPoint::GetToken( const GOUTOKENLIST& listToken, int iStart, int iEnd /* = -1 */ ) const
{
	if( iStart >= 0 && iStart < GetTokenCount() )
	{
		GOUTOKENLIST::const_iterator itrStart = listToken.begin();
		std::advance( itrStart, iStart );

		GOUTOKENLIST::const_iterator itrEnd = listToken.begin();
		if( iStart < iEnd && iEnd < GetTokenCount() )
			std::advance( itrEnd, iEnd );
		else
			itrEnd = listToken.end();

		CString strGouNo( *itrStart );
		++itrStart;
		while( itrStart != itrEnd )
		{
			strGouNo += _T("-") + *itrStart;
			++itrStart;
		}
		return strGouNo;
	}
	else
		return _T("");
}

// �����t���Z���H
addrStatus CGouPoint::IsCharAddr()
{
	// ���ԍ��̐擪[#]�Ȃ當���t��
	if( GetGouNo().Find(_T('#')) == 0 )
		return !IsExGouNo() && !GetCityAdmin()->GetExGaikuFugo().IsEmpty()? kMojiGou_ExtGaiku : kMojiGou;

	if( !IsExGouNo() && !GetCityAdmin()->GetExGaikuFugo().IsEmpty() )	// �g���X�敄������
	{
		// ��؂肪2�ȏ�Ȃ當���t����
		if( GetTokenCount() > 1 )
			return kSepOver_ExtGaiku;

		// ����10,000�ȏ�Ȃ當���t����
		return (_tstol(GetGouNo(0, 1)) > 9999)? kNoOver_ExtGaiku : kExtGaiku;
	}
	else														// �g���X�敄���Ȃ�
	{
		// �g�����{�X�敄������ŋ�؂肪2�ȏ�Ȃ當���t���Ƃ��Ĉ���
		// �g�����{�ŋ�؂肪2�ȏ�Ȃ當���t���Ƃ��Ĉ���
		if( (!IsExGouNo() && GetCityAdmin()->GetGaikuFugo() > 0 && GetTokenCount() > 1) || (IsExGouNo() && GetTokenCount() > 2) )
			return kSepOver;

		// �X�敄���Ȃ��ŋ�؂肪3�ȏ�Ȃ當���t���Ƃ��Ĉ���
		if( GetCityAdmin()->GetGaikuFugo() == 0 && GetTokenCount() > 2 )
			return kSepOver;

		// �Ō�ɒn�Ԃ�100,000�ȏ� ����10,000�ȏ�Ȃ當���t���Ƃ��Ĉ���
		if( !IsExGouNo() && GetCityAdmin()->GetGaikuFugo() > 0 )	// �X�敄������
		{
			return (_tstol(GetGouNo(0, 1)) > 9999)? kNoOver : kNormal;
		}
		else												// �X�敄���Ȃ�
		{
			return (_tstol(GetGouNo(0, 1)) > 99999) 
					|| (GetTokenCount() > 1 && _tstol(GetGouNo(1, 2)) > 9999) ?
					kNoOver : kNormal;
		}
	}
}

// 20���R�[�h����(�����l�����Ȃ�)
CString CGouPoint::GetAddrCode_NoChar()
{
	// ���ԍ��𕪗�(��ʂQ��)
	long lGouNo[2] = {0};
	CString strTmpGouNo( GetGouNo() );
	if( !strTmpGouNo.IsEmpty() )
	{		
		int nPos = strTmpGouNo.FindOneOf( _T("0123456789-") );
		if( nPos > -1 )
		{
			strTmpGouNo = strTmpGouNo.Mid( nPos );
			int nHaihunPos = strTmpGouNo.Find( '-' );
			if( 0 != nHaihunPos )
			{
				lGouNo[0] = _tstol( strTmpGouNo );
				if( nHaihunPos > 0 )
				{
					strTmpGouNo = strTmpGouNo.Mid( nHaihunPos );
					nPos = strTmpGouNo.FindOneOf( _T("0123456789") );
					if( nPos >= 0 )
						lGouNo[1] = _tstol( strTmpGouNo.Mid(nPos) );
				}
			}
			else
				return _T("");	// �u�`-1�v�Ƃ������P�[�X�̓R�[�h�����Ȃ�
		}
	}
	else
		return _T("");

	// �s���E�R�[�h�擾
	CString strAddrCode( GetCityAdmin()->GetAddrCode(false) );

	// �s���E����擾�ł���Z���R�[�h�����ɂ���ďꍇ����
	switch( strAddrCode.GetLength() )
	{
	case 11:
		if( lGouNo[0] > 0 )
			strAddrCode.AppendFormat( _T("%05d%04d"), lGouNo[0], lGouNo[1] );
		break;
	case 16:
		// �g���X�敄���ŃR�[�h�����@�قȂ�
		if( !IsExGouNo() )
			strAddrCode.AppendFormat( _T("%04d"), lGouNo[0] );
		else if( lGouNo[0] > 0 )
			strAddrCode.Format( _T("%s%05d%04d"), strAddrCode.Left(11), lGouNo[0], lGouNo[1] );	// �s���E�R�[�h��16����11���ɂ��ăR�[�h��
		break;
	default:
		break;
	}
	return strAddrCode.GetLength() == 20? strAddrCode : _T("");
}

// 20���R�[�h����(�����l��)
CString CGouPoint::GetAddrCode_Char( bool bCodeDBSaved /* = false */ )
{
	CString strAddrCode( GetCityAdmin()->GetAddrCode(true) );

	// �s���E����擾�ł���Z���R�[�h�����ɂ���ďꍇ����
	CString strPntName, strGouName;
	switch( strAddrCode.GetLength() )
	{
	case 11:
		strPntName = GetGouNo(0, 1);	strPntName.Replace( _T("#"), _T("") );
		strGouName = GetTokenCount() > 1? GetGouNo(1) : _T("");	strGouName.Replace( _T("#"), _T("") );

		strAddrCode.AppendFormat( _T("%05s%04s"), 
								  !IsNormalPnt(strPntName)? CCodeConverter::GetCodeConverter().GetPntCode(strAddrCode.Left(11), strPntName, bCodeDBSaved) : strPntName,
								  !IsNormalGou(strGouName)? CCodeConverter::GetCodeConverter().GetGouCode(strAddrCode.Left(11), strGouName, bCodeDBSaved) : strGouName );
		break;
	case 16:
		// �g���X�敄���ŃR�[�h�����@�قȂ�
		if( !IsExGouNo() )
		{
			strGouName = GetGouNo();	strGouName.Replace( _T("#"), _T("") );
			strAddrCode.AppendFormat( _T("%04s"), 
									  !IsNormalGou(strGouName)? CCodeConverter::GetCodeConverter().GetGouCode(strAddrCode.Left(11), strGouName, bCodeDBSaved) : strGouName );
		}
		else
		{
			strPntName = GetGouNo(0, 1);	strPntName.Replace( _T("#"), _T("") );
			strGouName = GetTokenCount() > 1? GetGouNo(1) : _T("");	strGouName.Replace( _T("#"), _T("") );
			strAddrCode.Format( _T("%s%05s%04s"), strAddrCode.Left(11), 
								!IsNormalPnt(strPntName)? CCodeConverter::GetCodeConverter().GetPntCode(strAddrCode.Left(11), strPntName, bCodeDBSaved) : strPntName,
								  !IsNormalGou(strGouName)? CCodeConverter::GetCodeConverter().GetGouCode(strAddrCode.Left(11), strGouName, bCodeDBSaved) : strGouName );
		}
		break;
	default:
		strAddrCode = _T("");
		break;
	}

	return strAddrCode;
}

}	// namespace addr
