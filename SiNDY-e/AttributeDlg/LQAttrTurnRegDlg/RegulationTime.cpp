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
#include "RegulationTime.h"

EKiseiNum CRegulationTime::CtrlID2KiseiNum( const int nID )
{
	EKiseiNum ERet = KISEIJIKAN1;
	switch(nID)
	{
	case IDC_BUTTON_TUUNEN1:
	case IDC_BUTTON_SYUJITU1:
	case IDC_BUTTON_DELETE1:
	case IDC_CHECK_ZEROGATU1:
	case IDC_EDIT_TURNREGSMON1:
	case IDC_EDIT_TURNREGSDAY1:
	case IDC_EDIT_TURNREGEMON1: 
	case IDC_EDIT_TURNREGEDAY1: 
	case IDC_EDIT_TURNREGSHOUR1:
	case IDC_EDIT_TURNREGSMIN1: 
	case IDC_EDIT_TURNREGEHOUR1:
	case IDC_EDIT_TURNREGEMIN1:
	case IDC_COMBO_TURNREGW1:
		break;

	case IDC_BUTTON_TUUNEN2:
	case IDC_BUTTON_SYUJITU2:
	case IDC_BUTTON_DELETE2:
	case IDC_CHECK_ZEROGATU2:
	case IDC_EDIT_TURNREGSMON2:
	case IDC_EDIT_TURNREGSDAY2:
	case IDC_EDIT_TURNREGEMON2: 
	case IDC_EDIT_TURNREGEDAY2: 
	case IDC_EDIT_TURNREGSHOUR2:
	case IDC_EDIT_TURNREGSMIN2: 
	case IDC_EDIT_TURNREGEHOUR2:
	case IDC_EDIT_TURNREGEMIN2:
	case IDC_COMBO_TURNREGW2:
		ERet = KISEIJIKAN2; break;

	case IDC_BUTTON_TUUNEN3:
	case IDC_BUTTON_SYUJITU3:
	case IDC_BUTTON_DELETE3:
	case IDC_CHECK_ZEROGATU3:
	case IDC_EDIT_TURNREGSMON3:
	case IDC_EDIT_TURNREGSDAY3:
	case IDC_EDIT_TURNREGEMON3: 
	case IDC_EDIT_TURNREGEDAY3: 
	case IDC_EDIT_TURNREGSHOUR3:
	case IDC_EDIT_TURNREGSMIN3: 
	case IDC_EDIT_TURNREGEHOUR3:
	case IDC_EDIT_TURNREGEMIN3:
	case IDC_COMBO_TURNREGW3:
		ERet = KISEIJIKAN3; break;

	case IDC_BUTTON_TUUNEN4:
	case IDC_BUTTON_SYUJITU4:
	case IDC_BUTTON_DELETE4:
	case IDC_CHECK_ZEROGATU4:
	case IDC_EDIT_TURNREGSMON4:
	case IDC_EDIT_TURNREGSDAY4:
	case IDC_EDIT_TURNREGEMON4: 
	case IDC_EDIT_TURNREGEDAY4: 
	case IDC_EDIT_TURNREGSHOUR4:
	case IDC_EDIT_TURNREGSMIN4: 
	case IDC_EDIT_TURNREGEHOUR4:
	case IDC_EDIT_TURNREGEMIN4:
	case IDC_COMBO_TURNREGW4:
		ERet = KISEIJIKAN4; break;

	default: break;
	}
	return ERet;
}

CString CRegulationTime::GetEditStr( CEdit cEdit )
{
	CString retStr;
	cEdit.GetWindowText( retStr );
	return retStr;
}
CRegulationTime::CRegulationTime(void)
{
}

CRegulationTime::~CRegulationTime(void)
{
}

CRegulationTime::CRegulationTime( HWND hDlg, EKiseiNum Enum )
{
	switch( Enum )
	{
	case KISEIJIKAN1:
		m_cEditStartMonth = GetDlgItem( hDlg, IDC_EDIT_TURNREGSMON1 );
		m_cEditStartDay   = GetDlgItem( hDlg, IDC_EDIT_TURNREGSDAY1 );
		m_cEditEndMonth   = GetDlgItem( hDlg, IDC_EDIT_TURNREGEMON1 );
		m_cEditEndDay     = GetDlgItem( hDlg, IDC_EDIT_TURNREGEDAY1 );
		m_cEditStartHour  = GetDlgItem( hDlg, IDC_EDIT_TURNREGSHOUR1 );
		m_cEditStartMin   = GetDlgItem( hDlg, IDC_EDIT_TURNREGSMIN1 );
		m_cEditEndHour    = GetDlgItem( hDlg, IDC_EDIT_TURNREGEHOUR1 );
		m_cEditEndMin     = GetDlgItem( hDlg, IDC_EDIT_TURNREGEMIN1 );
		m_cBtnZeroGatu    = GetDlgItem( hDlg, IDC_CHECK_ZEROGATU1 );
		m_cCmbYobi        = GetDlgItem( hDlg, IDC_COMBO_TURNREGW1 );
		break;
	case KISEIJIKAN2:
		m_cEditStartMonth = GetDlgItem( hDlg, IDC_EDIT_TURNREGSMON2 );
		m_cEditStartDay   = GetDlgItem( hDlg, IDC_EDIT_TURNREGSDAY2 );
		m_cEditEndMonth   = GetDlgItem( hDlg, IDC_EDIT_TURNREGEMON2 );
		m_cEditEndDay     = GetDlgItem( hDlg, IDC_EDIT_TURNREGEDAY2 );
		m_cEditStartHour  = GetDlgItem( hDlg, IDC_EDIT_TURNREGSHOUR2 );
		m_cEditStartMin   = GetDlgItem( hDlg, IDC_EDIT_TURNREGSMIN2 );
		m_cEditEndHour    = GetDlgItem( hDlg, IDC_EDIT_TURNREGEHOUR2 );
		m_cEditEndMin     = GetDlgItem( hDlg, IDC_EDIT_TURNREGEMIN2 );
		m_cBtnZeroGatu    = GetDlgItem( hDlg, IDC_CHECK_ZEROGATU2 );
		m_cCmbYobi        = GetDlgItem( hDlg, IDC_COMBO_TURNREGW2 );
		break;
	case KISEIJIKAN3:
		m_cEditStartMonth = GetDlgItem( hDlg, IDC_EDIT_TURNREGSMON3 );
		m_cEditStartDay   = GetDlgItem( hDlg, IDC_EDIT_TURNREGSDAY3 );
		m_cEditEndMonth   = GetDlgItem( hDlg, IDC_EDIT_TURNREGEMON3 );
		m_cEditEndDay     = GetDlgItem( hDlg, IDC_EDIT_TURNREGEDAY3 );
		m_cEditStartHour  = GetDlgItem( hDlg, IDC_EDIT_TURNREGSHOUR3 );
		m_cEditStartMin   = GetDlgItem( hDlg, IDC_EDIT_TURNREGSMIN3 );
		m_cEditEndHour    = GetDlgItem( hDlg, IDC_EDIT_TURNREGEHOUR3 );
		m_cEditEndMin     = GetDlgItem( hDlg, IDC_EDIT_TURNREGEMIN3 );
		m_cBtnZeroGatu    = GetDlgItem( hDlg, IDC_CHECK_ZEROGATU3 );
		m_cCmbYobi        = GetDlgItem( hDlg, IDC_COMBO_TURNREGW3 );
		break;
	case KISEIJIKAN4:
		m_cEditStartMonth = GetDlgItem( hDlg, IDC_EDIT_TURNREGSMON4 );
		m_cEditStartDay   = GetDlgItem( hDlg, IDC_EDIT_TURNREGSDAY4 );
		m_cEditEndMonth   = GetDlgItem( hDlg, IDC_EDIT_TURNREGEMON4 );
		m_cEditEndDay     = GetDlgItem( hDlg, IDC_EDIT_TURNREGEDAY4 );
		m_cEditStartHour  = GetDlgItem( hDlg, IDC_EDIT_TURNREGSHOUR4 );
		m_cEditStartMin   = GetDlgItem( hDlg, IDC_EDIT_TURNREGSMIN4 );
		m_cEditEndHour    = GetDlgItem( hDlg, IDC_EDIT_TURNREGEHOUR4 );
		m_cEditEndMin     = GetDlgItem( hDlg, IDC_EDIT_TURNREGEMIN4 );
		m_cBtnZeroGatu    = GetDlgItem( hDlg, IDC_CHECK_ZEROGATU4 );
		m_cCmbYobi        = GetDlgItem( hDlg, IDC_COMBO_TURNREGW4 );
		break;
	default: break;
	}
}

// 「0」分だったら「00」分表示に変える。
void CRegulationTime::DisplayZeroZero()
{
	if( GetEditStr(m_cEditStartMin) == _T("0") )
		m_cEditStartMin.SetWindowText( _T("00") );
	
	if( GetEditStr( m_cEditEndMin ) == _T("0") )
		m_cEditEndMin.SetWindowText( _T("00") );
}

// 「毎月」ボタンが押された時の処理用
void CRegulationTime::ToggleZeroGatu( const int nBtnState )
{
	if( nBtnState )
		SetZeroGatu();
	else
		UnsetZeroGatu();
}

// 「毎月」ONの時の処理用
void CRegulationTime::SetZeroGatu( bool bIsSetDay /*=true*/)
{
	m_cBtnZeroGatu.SetCheck(1);

	// 0月1日 ～ 0月31日 をセット
	m_cEditStartMonth.SetWindowText( _T("0") );
	m_cEditEndMonth.SetWindowText( _T("0") );
	if( bIsSetDay )
	{
		m_cEditStartDay.SetWindowText( _T("1") );
		m_cEditEndDay.SetWindowText( _T("31") );
	}

	// 月はDisableに
	m_cEditStartMonth.EnableWindow(FALSE);
	m_cEditEndMonth.EnableWindow(FALSE);
	//m_cEditStartMonth.SetReadOnly( bIsOn ? TRUE : FALSE );
	//m_cEditEndMonth.SetReadOnly( bIsOn ? TRUE : FALSE );

	m_cEditStartDay.SetFocus();

}

// 「毎月」OFFの時の処理用
void CRegulationTime::UnsetZeroGatu()
{
	m_cBtnZeroGatu.SetCheck(0);

	// 月は空欄に
	m_cEditStartMonth.SetWindowText( _T("") );
	m_cEditEndMonth.SetWindowText( _T("") );

	// 月をEnableに戻す
	m_cEditStartMonth.EnableWindow();
	m_cEditEndMonth.EnableWindow();
}

// コントロールに規制期間を入力する関数
void CRegulationTime::SetRegDate( const long lSMon, const long lSDay, const long lEMon, const long lEDay )
{
	CString strTmp;
	strTmp.Format( _T("%d"), lSMon );
	m_cEditStartMonth.SetWindowText(strTmp);
	strTmp.Format( _T("%d"), lSDay );
	m_cEditStartDay.SetWindowText(strTmp);
	strTmp.Format( _T("%d"), lEMon );
	m_cEditEndMonth.SetWindowText(strTmp);
	strTmp.Format( _T("%d"), lEDay );
	m_cEditEndDay.SetWindowText(strTmp);
}

// コントロールに規制時間を入力する関数
void CRegulationTime::SetRegTime( const long lSHour, const long lSMin, const long lEHour, const long lEMin )
{
	CString strTmp;
	strTmp.Format( _T("%d"), lSHour );
	m_cEditStartHour.SetWindowText(strTmp);
	strTmp.Format( (lSMin == 0) ? _T("%02d") : _T("%d"), lSMin );
	m_cEditStartMin.SetWindowText(strTmp);
	strTmp.Format( _T("%d"), lEHour );
	m_cEditEndHour.SetWindowText(strTmp);
	strTmp.Format( (lEMin == 0) ? _T("%02d") : _T("%d"), lEMin );
	m_cEditEndMin.SetWindowText(strTmp);
}

// 規制時間の入力を削除する
void CRegulationTime::Clear()
{
	UnsetZeroGatu();
	m_cEditStartDay.SetWindowText( _T("") );
	m_cEditEndDay.SetWindowText( _T("") );
	m_cEditStartHour.SetWindowText( _T("") );
	m_cEditStartMin.SetWindowText( _T("") );
	m_cEditEndHour.SetWindowText( _T("") );
	m_cEditEndMin.SetWindowText( _T("") );
	m_cCmbYobi.SetCurSel( m_cCmbYobi.FindString( 0, _T("<null>") ) );
	m_cCmbYobi.SendMessage( m_cCmbYobi.GetParent(), WM_COMMAND, MAKEWPARAM(m_cCmbYobi.GetDlgCtrlID(), CBN_SELCHANGE), 0 );
}

// 「月」の入力を監視する
bool CRegulationTime::CheckZeroGatu( HWND hWnd, CString &strErr, bool bFinishInit )
{
	if( m_cBtnZeroGatu.GetCheck() == 0)
	{
		CEdit cEdit(hWnd);
		CString strMonth;
		cEdit.GetWindowText( strMonth );
		if( strMonth.Compare( _T("0") ) == 0 )
		{
			if( !bFinishInit ) // 初回（「0月」のものが選択された場合）用、、、
			{
				SetZeroGatu( false );
			}
			else
			{
				strErr =AheLoadString(IDS_INPUT0MONTH);
				cEdit.SetWindowText( _T("") );
				return false;
			}
		}
	}
	return true;
}

// 規制期間が1月～12月か？
bool CRegulationTime::IsEveryMonth()
{
	if( GetEditStr( m_cEditStartMonth ) == _T("1") &&
		GetEditStr( m_cEditEndMonth ) == _T("12") )
	{
		CString strYobi;
		int nIndex = m_cCmbYobi.GetCurSel();
		m_cCmbYobi.GetLBText( nIndex, strYobi.GetBuffer( m_cCmbYobi.GetLBTextLen(nIndex)+1 )  );
		strYobi.ReleaseBuffer();
		if( strYobi == AheLoadString(IDS_ALLWEEK) )
			return true;
	}
	return false;
}

bool CRegulationTime::IsZeroGatu()
{
	if( GetEditStr( m_cEditStartMonth ) == _T("0") &&
		GetEditStr( m_cEditEndMonth ) == _T("0") )
	{
		return true;
	}
	return false;
}

REGTIME CRegulationTime::GetRegTime()
{
	REGTIME tReg = { GetEditStr(m_cEditStartMonth), GetEditStr(m_cEditEndMonth), GetEditStr(m_cEditStartDay), GetEditStr(m_cEditEndDay),
					 GetEditStr(m_cEditStartHour), GetEditStr(m_cEditEndHour), GetEditStr(m_cEditStartMin), GetEditStr(m_cEditEndMin),
					 m_cCmbYobi.GetCurSel(), m_cBtnZeroGatu.GetCheck() ? true : false
	};
	return tReg;
}
void CRegulationTime::SetRegTime( REGTIME & tRegTime )
{
	if( tRegTime.bIsEveryMonth )
		SetZeroGatu();
	else{
		m_cEditStartMonth.SetWindowText( tRegTime.strSMon );
		m_cEditEndMonth.SetWindowText( tRegTime.strEMon );
	}
	m_cEditStartDay.SetWindowText( tRegTime.strSDay );
	m_cEditEndDay.SetWindowText( tRegTime.strEDay );
	m_cEditStartHour.SetWindowText( tRegTime.strSHour );
	m_cEditEndHour.SetWindowText( tRegTime.strEHour );
	m_cEditStartMin.SetWindowText( tRegTime.strSMin );
	m_cEditEndMin.SetWindowText( tRegTime.strEMin );
	m_cCmbYobi.SetCurSel( tRegTime.nYobiIndex );
	m_cCmbYobi.SendMessage( m_cCmbYobi.GetParent(), WM_COMMAND, MAKEWPARAM(m_cCmbYobi.GetDlgCtrlID(), CBN_SELCHANGE), 0 );
}

bool CRegulationTime::IsContainDays( REGTIME &tReg )
{
	return ( _ttoi( GetEditStr( m_cEditStartDay ) ) <= _ttoi( tReg.strSDay ) &&
			 _ttoi( GetEditStr( m_cEditEndDay ) )   >= _ttoi( tReg.strEDay ) );
}

bool CRegulationTime::IsContainTime( REGTIME &tReg )
{
	float fSrcStime = _ttoi( GetEditStr( m_cEditStartHour ) ) + _ttoi( GetEditStr( m_cEditStartMin ) ) / 60.0,
		  fSrcEtime = _ttoi( GetEditStr( m_cEditEndHour ) )   + _ttoi( GetEditStr( m_cEditEndMin ) ) / 60.0,
		  fDstStime = _ttoi( tReg.strSHour ) + _ttoi( tReg.strSMin ) / 60.0,
		  fDstEtime = _ttoi( tReg.strEHour ) + _ttoi( tReg.strEMin ) / 60.0;

	return ( fSrcStime <= fDstStime &&
			 fSrcEtime >= fDstEtime );
}




//bool CRegulationTime::IsEmpty()
//{
//	if( GetEditStr(m_cEditStartMonth).IsEmpty() &&
//		GetEditStr(m_cEditStartDay).IsEmpty() &&
//		GetEditStr(m_cEditEndMonth).IsEmpty() &&
//		GetEditStr(m_cEditEndDay).IsEmpty() &&
//		GetEditStr(m_cEditStartHour).IsEmpty() &&
//		GetEditStr(m_cEditStartMin).IsEmpty() &&
//		GetEditStr(m_cEditEndHour).IsEmpty() &&
//		GetEditStr(m_cEditEndMin).IsEmpty() )
//	{
//		CString strYobi;
//		int nIndex = m_cCmbYobi.GetCurSel();
//		m_cCmbYobi.GetLBText( nIndex, strYobi.GetBuffer( m_cCmbYobi.GetLBTextLen(nIndex+1) )  );
//		strYobi.ReleaseBuffer();
//		if(strYobi == _T("<null>"))
//			return true;
//	}
//	return false;
//}

//bool CRegulationTime::IsTunen()
//{
//	if( GetEditStr( m_cEditStartMonth ) == _T("1") &&
//		GetEditStr( m_cEditStartDay ) == _T("1") &&
//		GetEditStr( m_cEditEndMonth ) == _T("12") &&
//		GetEditStr( m_cEditEndDay ) == _T("31") )
//	{
//		return true;
//	}
//	return false;
//}
//
//bool CRegulationTime::IsSyujitu()
//{
//	if( GetEditStr(m_cEditStartHour) == _T("0") &&
//		GetEditStr(m_cEditStartMin) == _T("00") &&
//		GetEditStr(m_cEditEndHour) == _T("24") &&
//		GetEditStr(m_cEditEndMin) == _T("0") )
//		return true;
//
//	return false;
//}
//
//bool CRegulationTime::IsEverydayEveryTime()
//{
//	CString strYobi;
//	int nIndex = m_cCmbYobi.GetCurSel();
//	m_cCmbYobi.GetLBText( nIndex, strYobi.GetBuffer( m_cCmbYobi.GetLBTextLen(nIndex+1) )  );
//	strYobi.ReleaseBuffer();
//	return ( IsTunen() && IsSyujitu() && (strYobi == _T("全曜日")) );
//}
