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
#include "AheGlobalsCoord.h"
#include "AheGlobalsMisc.h"
#include "AheLayerFunctions.h"
#include "AheMeshUtil.h"	// �����include�����crd_cnv���K�v�ɂȂ�B

/////////////////////////////////////////////////////////////////////////////
//
// URI ��͗p�֐��E���W�ϊ��Ȃ�
//
/////////////////////////////////////////////////////////////////////////////

// TODO:crd_cnv::GetMeshCode()�Ƌ@�\���d�����邽�ߐ�������B
//      ���̊֐��̓��b�V���R�[�h��XY�v�f��10�i���Ōv�Z���Ă���(�{����8�i���Ōv�Z���ׂ�)�B
//      INC_MESHXY��DEC_MESHXY�Œ��K�������悤�Ɍv�Z����Ζ��Ȃ����A
//      ����ł�2���b�V���ȏ㗣�ꂽ���̌v�Z�͂ł��Ȃ����߁A��肠��B
//      ������crd_cnv::GetMeshCode()�𐄏�����(ta_suga)
void AheMeshcode2MeshXY( int& nX, int& nY, unsigned int nMeshCode)
{
	nY = ( ( nMeshCode / 10000) * 10) + ( ( nMeshCode % 100) / 10);
	nX = ( ( ( nMeshCode / 100) % 100) * 10) + ( nMeshCode % 10);
}

// TODO:crd_cnv::GetMeshCode()�Ƌ@�\���d�����邽�ߐ�������B(ta_suga)
unsigned int AheMeshXY2Meshcode( int nX, int nY)
{
	unsigned int nMesh;
	nMesh = ( ( nY / 10) * 10000) + ( ( nY % 10) * 10);
	nMesh += ( ( nX / 10) * 100) + ( nX % 10);
	return nMesh;
}
DOUBLE AheDegMinSec2Realdegree( INT iDeg, INT iMin, DOUBLE dSec )
{
	DOUBLE ret = (DOUBLE)iDeg + (DOUBLE)iMin / 60.0 + (DOUBLE)dSec / 3600.0;
	return ret;
}
BOOL AheStr2DegMinSec( CString& str, INT& iDeg, INT& iMin, DOUBLE& dSec )
{
	LPTSTR lpszTmp; // ���݁[
	INT iCount;
	CString strTmp;
	CString strHead = str.Left( 1 );	// W,E,N,S,w,e,n,s ����菜��

	if ( -1 != strHead.FindOneOf(_T("WENSwens")) ) {
		// ���p�������������Ƃ�
		str = str.Right( str.GetLength() - 1 );
	} else {
		return FALSE;
	}

	iCount = str.Find( _T(".") );
	strTmp = str.Left( iCount );
	iDeg = _ttoi( (LPCTSTR)strTmp );
	str = str.Right( str.GetLength() - iCount - 1 );

	if (strHead.CompareNoCase(_T("W")) == 0)
		iDeg = 180 + 180 - iDeg;

	if (strHead.CompareNoCase(_T("S")) == 0)
		iDeg = -iDeg;

	iCount = str.Find( _T(".") );
	strTmp = str.Left( iCount );
	iMin = _ttoi( (LPCTSTR)strTmp );
	str = str.Right( str.GetLength() - iCount - 1 );

	iCount = str.Find( _T(".") );
	strTmp = str.Left( iCount );
	CString strRight = str.Right( str.GetLength() - iCount - 1 );
	
	if( strRight.Find( _T(".") ) > 0 )
		return FALSE;	// �Z�p���[�^�i.�j���܂�����Ȃ�ϊ����Ȃ�
	
	dSec = _tcstod( (LPCTSTR)str, &lpszTmp );

	return TRUE;
}
BOOL AheURIMFV2LonLat(CString strURI, LonLat& lTmp)
{
	// �߂�l������
	lTmp.Latitude = lTmp.Longitude = lTmp.Scale = 0;

	// �����`�F�b�N
	if( 20 >= strURI.GetLength() )
		return FALSE;

	CString strTmp, strCompWord, strValue;
	int iDeg, iMin;
	double dSec;

	///  mf://mapfan/map/pos? ��؂�̂Ă�
	strURI = strURI.Right( strURI.GetLength() - 20 );	

	while( strURI.CompareNoCase( _T("") ) != 0 ) {
		INT iCount = strURI.Find(_T("&"));
		if( iCount < 0 ) {	/// �v�f���Ȃ�
			strTmp = strURI;
		} else {
			strTmp = strURI.Left( iCount );
		}
		INT iValue = strTmp.Find(_T("="));
		if( iValue < 0 ) {	/// �l���Ȃ��ꍇ�͔�΂�
			if( iCount < 0 ) 
				break;	/// �v�f���Ȃ��ꍇ�͏I��
			else {
				strURI = strURI.Right( strURI.GetLength() - iCount - 1 );
				continue;
			}
		}

		strCompWord = strTmp.Left( iValue );
		strValue = strTmp.Right( strTmp.GetLength() - iValue - 1 );
		
		if( strCompWord.CompareNoCase( _T("longitude") ) == 0 ) 
		{
			if( AheStr2DegMinSec( strValue, iDeg, iMin, dSec ) == FALSE )
				break;

			lTmp.Longitude = AheDegMinSec2Realdegree( iDeg, iMin, dSec );
		}
		else if( strCompWord.CompareNoCase( _T("latitude") ) == 0 )
		{
			if( AheStr2DegMinSec( strValue, iDeg, iMin, dSec ) == FALSE )
				break;

			lTmp.Latitude = AheDegMinSec2Realdegree( iDeg, iMin, dSec );
		}
		else if( strCompWord.CompareNoCase( _T("realscale") ) == 0 )
		{
			LPTSTR lpszTmp;
			lTmp.Scale = _tcstod( (LPCTSTR)strValue, &lpszTmp );
		}
		else if( strCompWord.CompareNoCase( _T("") ) == 0 )
		{
			break;
		}

		if( iCount < 0 )
			break;

		strURI = strURI.Right( strURI.GetLength() - iCount - 1 );
	}

	return TRUE;
}

BOOL AheURIMFV2MeshXY(CString strURI, MeshXY& MeshXY)
{
	USES_CONVERSION;

	CString 	strTmp, strCompWord, strValue;
	INT			nLonLatFlg = 0, iCnt = 0;;
	int 		iDeg = 0, iMin = 0;
	double 		dSec = 0;
	LonLat		lTmp = {0,0,0};
	TCHAR 		*tStop = NULL;

	// URI��؂�̂Ă�
	strURI = strURI.Right( strURI.GetLength() - 20 );
	while( strURI.CompareNoCase( _T("") ) != 0 ) {
		INT 		iCount = 0, iValue = 0;
		CString		szTmp;

		// &�ʒu�擾
		iCount = strURI.Find( _T("&") );
		if( iCount < 0 )
			strTmp = strURI;
		else
			strTmp = strURI.Left( iCount );

		// =�ʒu�擾
		iValue = strTmp.Find( _T("=") );
		if( iValue < 0 )
			return FALSE;

		// ���͂��ꂽ������擾
		strCompWord = strTmp.Left( iValue );
		// ���͂��ꂽ�l�擾
		strValue = strTmp.Right( strTmp.GetLength() - iValue - 1 );

		if( strCompWord.CompareNoCase( _T("longitude") ) == 0 ) {
			if ( iCnt != 0 )
				return FALSE;
			iCnt++;

			if ( strValue.Left(1) == _T("E") )
				nLonLatFlg = 0;
			else if ( strValue.Left(1) == _T("W") )
				nLonLatFlg = 1;
			else
				return FALSE;

			if ( AheChkLonLat(strValue) == FALSE )
				return FALSE;

			if ( AheGetKeywordValue( strTmp, strValue, _T("longitude=EW0123456789."), szTmp, 1 ) == FALSE )
				return FALSE;

			if( AheStr2DegMinSec( szTmp, iDeg, iMin, dSec ) == FALSE )
				return FALSE;

			lTmp.Longitude = AheDegMinSec2Realdegree( iDeg, iMin, dSec );
		} else if( strCompWord.CompareNoCase( _T("latitude") ) == 0 ) {
			if ( iCnt != 1 )
				return FALSE;
			iCnt++;

			if ( nLonLatFlg == 0 ) {
				if ( strValue.Left(1) != _T("N") )
					return FALSE;
			} else if ( nLonLatFlg == 1 ){
				if ( strValue.Left(1) != _T("S") )
					return FALSE;
			}

			if ( AheChkLonLat(strValue) == FALSE )
				return FALSE;

			if ( AheGetKeywordValue( strTmp, strValue, _T("latitude=NS0123456789."), szTmp, 1 ) == FALSE )
				return FALSE;

			if( AheStr2DegMinSec( szTmp, iDeg, iMin, dSec ) == FALSE )
				return FALSE;

			lTmp.Latitude = AheDegMinSec2Realdegree( iDeg, iMin, dSec );
		} else if( strCompWord.CompareNoCase( _T("realscale") ) == 0 ) {
			if ( iCnt != 2 )
				return FALSE;

			if ( AheGetKeywordValue( strTmp, strValue, _T("realscale=0123456789."), szTmp, 0 ) == FALSE )
				return FALSE;

			lTmp.Scale = _tcstod((LPCTSTR)szTmp, &tStop);
		} else if( strCompWord.CompareNoCase( _T("") ) == 0 ) {
			return FALSE;
		} else {
			return FALSE;
		}

		if( iCount < 0 )
			break;
		strURI = strURI.Right( strURI.GetLength() - iCount - 1 );
	}

	// ���b�V���R�[�h�擾
	MeshXY.MeshCode = AheLL2MESH( lTmp.Longitude, lTmp.Latitude, 2, MeshXY.X, MeshXY.Y );
	MeshXY.Scale = lTmp.Scale;

	return TRUE;
}

BOOL AheChkLonLat(CString strTemp)
{
	USES_CONVERSION;

	CString		strCmp;
	CComBSTR	sTmp = 0;
	TCHAR 		*token;
	int 		iCnt = 0;

	sTmp = strTemp;
	token = _tcstok(OLE2T(sTmp.m_str), _T("."));
	while ( token != NULL) {
		iCnt++;
		switch (iCnt) {
			case 1:
				strCmp = OLE2T(sTmp.m_str);
				if ( strCmp.Left(1) != _T("E") && strCmp.Left(1) != _T("W") && strCmp.Left(1) != _T("N") && strCmp.Left(1) != _T("S") )
					return FALSE;
				break;
			case 2:	case 3: case 4:
				break;
			default:
				return FALSE;
		}
		token = _tcstok(NULL, _T("."));
	}
	return TRUE;
}

BOOL AheURIMFW2LonLat(CString strURI, LonLat& lTmp)
{
	CString strTmp, strTmp2;
	int iCnt=0, iPos = -1;
	int iDeg, iMin;
	double dSec;
	int iTmp=0;

	///  http://kokomail.mapfan.com/receive.cgi?MAP ��؂�̂Ă�
	strURI = strURI.Right( strURI.GetLength() - 42 );

	/// URI ����Y�����鐔�l�𒊏o
	while (strURI.CompareNoCase( _T("") ) != 0) {
		iCnt++;
		iPos = strURI.Find(_T("="));

		if (iPos == -1) {
			break;
		} else {
			strURI = strURI.Right(strURI.GetLength() - iPos - 1);
		}

		switch (iCnt){
		case 1:
			strTmp = strURI.SpanIncluding(_T("EW"));

			if (strTmp.Compare(_T("E")) == 0)
			{
				/// �o�x
				strURI = strURI.Right(strURI.GetLength() - iPos - 1);
				strTmp = _T("E") +  strURI.SpanIncluding(_T("0123456789."));
				
				if ( AheStr2DegMinSec(strTmp, iDeg, iMin, dSec) == FALSE ) {
					return FALSE;
				}

				// �o�x�����߂�
				lTmp.Longitude = AheDegMinSec2Realdegree( iDeg, iMin, dSec );

				strTmp = strURI.SpanIncluding(_T("0123456789."));
				strURI = strURI.Right(strURI.GetLength() - strTmp.GetLength());

				/// �����Ĉܓx

				strTmp = strURI.SpanIncluding(_T("NS"));

				if (strTmp.Compare(_T("N")) == 0)
				{
					strURI = strURI.Right(strURI.GetLength() - iPos - 1);
					strTmp = _T("N") + strURI.SpanIncluding(_T("0123456789."));
					
					if ( AheStr2DegMinSec(strTmp, iDeg, iMin, dSec) == FALSE ) {
						return FALSE;
					}

					// �ܓx�����߂�
					lTmp.Latitude = AheDegMinSec2Realdegree( iDeg, iMin, dSec );

				}
			} else if (strTmp.Compare(_T("W"))) {
				/// ���o
			}

			break;
		case 2:
			/// Zoom �i1/640 �����O�F1/320�����P�F1/160�����Q�E�E�E�j
			iTmp = _ttoi(( LPCTSTR)strURI);

			if (iTmp == 0)
				lTmp.Scale = 6400000;
			else
				lTmp.Scale = 3200000 / (AheFact( 2, iTmp - 1));

			break;
		default:
			break;
		}
	}

	return TRUE;
}

BOOL AheURIMFW2MeshXY(CString strURI, MeshXY& MeshXY)
{
	USES_CONVERSION;

	CString 	strTmp, strCompWord, strValue;
	INT 		iCnt=0, iPos = -1;
	int 		iDeg = 0, iMin = 0;
	double 		dSec = 0;
	LonLat		lTmp = {0,0,0};
	int 		iTmp = 0;

	//  http://kokomail.mapfan.com/receive.cgi?MAP ��؂�̂Ă�
	if ( strURI.Left( 42 ) != _T("http://kokomail.mapfan.com/receive.cgi?MAP") )
		return FALSE;

	strURI = strURI.Right( strURI.GetLength() - 42 );
	if ( strURI.GetLength() <= 23 )
		return FALSE;

	// URI ����Y�����鐔�l�𒊏o
	while (strURI.CompareNoCase( _T("") ) != 0) {
		iCnt++;
		if( iCnt == 2 ) {
			INT 		iCount = 0, iValue = 0;

			iCount = strURI.Find(_T("&"));
			if( iCount < 0 )
				return FALSE;
			strTmp = strURI.Right( strURI.GetLength() - iCount - 1);

			iValue = strTmp.Find(_T("="));
			if( iValue < 0 )
				return FALSE;

			strCompWord = strTmp.Left( iValue );
			strValue = strTmp.Right( strTmp.GetLength() - iValue - 1 );
			if( strCompWord.CompareNoCase( _T("ZM") ) != 0 )
				return FALSE;
		}

		iPos = strURI.Find(_T("="));
		if (iPos == -1)
			break;
		else
			strURI = strURI.Right(strURI.GetLength() - iPos - 1);

		switch (iCnt){
			case 1:
				strTmp = strURI.SpanIncluding(_T("EW"));
				if ( strURI.Left(1) != _T("E") && strURI.Left(1) != _T("W") )
					return FALSE;
				if ( strTmp.Compare(_T("E")) == 0 ) {
					// �o�x
					strURI = strURI.Right(strURI.GetLength() - iPos - 1);
					strTmp = _T("E") +  strURI.SpanIncluding(_T("0123456789."));
					if( AheChkLonLat( strTmp ) == FALSE )
						return FALSE;

					if( AheStr2DegMinSec( strTmp, iDeg, iMin, dSec ) == FALSE )
						return FALSE;

					lTmp.Longitude = AheDegMinSec2Realdegree( iDeg, iMin, dSec );

					strTmp = strURI.SpanIncluding(_T("0123456789."));
					strURI = strURI.Right(strURI.GetLength() - strTmp.GetLength());
					
					// �����Ĉܓx
					strTmp = strURI.SpanIncluding(_T("NS"));
					if (strTmp.Compare(_T("N")) == 0) {
						strURI = strURI.Right(strURI.GetLength() - iPos - 1);
						strTmp = _T("N") + strURI.SpanIncluding(_T("0123456789."));
						if( AheChkLonLat( strTmp ) == FALSE )
							return FALSE;

						if( AheStr2DegMinSec( strTmp, iDeg, iMin, dSec ) == FALSE )
							return FALSE;

						lTmp.Latitude = AheDegMinSec2Realdegree( iDeg, iMin, dSec );
					} else {
						return FALSE;
					}
				} else if ( strTmp.Compare(_T("W")) == 0 ) {
					// ���o
					return FALSE;
				} else if ( strTmp.Compare(_T("")) == 0 ) {
					return FALSE;
				} else {
					return FALSE;
				}
				break;
			case 2:
				if ( strURI != strURI.SpanIncluding(_T("0123456789")) || _ttoi(( LPCTSTR)strURI) < 0 || _ttoi(( LPCTSTR)strURI) > 12 || strURI == _T("") )
					return FALSE;
				/// Zoom �i1/640 �����O�F1/320�����P�F1/160�����Q�E�E�E�j
				iTmp = _ttoi(( LPCTSTR)strURI);

				if (iTmp == 0)
					lTmp.Scale = 6400000;
				else
					lTmp.Scale = 3200000 / (AheFact( 2, iTmp - 1));
				break;
			default:
				return FALSE;
		}
	}

	// ���b�V���R�[�h�擾
	MeshXY.MeshCode = AheLL2MESH( lTmp.Longitude, lTmp.Latitude, 2, MeshXY.X, MeshXY.Y );
	MeshXY.Scale = lTmp.Scale;

	return TRUE;
}

BOOL AheURIMFP2LonLat(CString strURI, LonLat& lTmp)
{
	/// ������

	return FALSE;
}

BOOL AheURISiNDYMesh2LonLat(CString strURI, LonLat& lTmp)
{
	USES_CONVERSION;

	CString strTmp;
	unsigned int iMesh = 0;
	int iX=-1, iY=-1, iCnt=0, iPos=-1;
	
	///  sindy://sindy/map/pos? ��؂�̂Ă�
	strURI = strURI.Right( strURI.GetLength() - 22 );	

	/// URI ����Y�����鐔�l�𒊏o
	while (strURI.CompareNoCase( _T("") ) != 0) {
		iCnt++;
		iPos = strURI.Find(_T("="));

		if (iPos == -1) {
			break;
		} else {
			strURI = strURI.Right(strURI.GetLength() - iPos - 1);
		}

		strTmp = strURI.SpanIncluding(_T("0123456789"));

		switch (iCnt){
		case 1:
		{
			TCHAR* stopstring = NULL;
			iMesh = _tcstoul(strTmp, &stopstring, 10); // UINT�Ή�
			break;
		}
		case 2:
			iX = _ttoi((LPCTSTR)strTmp);
			break;
		case 3:
			iY = _ttoi((LPCTSTR)strTmp);
			break;
		default:
			break;
		}
	}

	/// iX iY ���ȗ����ꂽ�ꍇ�́A�����ɂ���
	if (( iX == -1 ) && ( iY == -1)) lTmp.Scale = 100000; /// �S�̂�����悤��
	if ( iX == -1 ) iX = 500000;
	if ( iY == -1 ) iY = 500000;

	/// MeshXY ����ܓx�o�x�ɕϊ�
	// TODO:���̊֐��͍��ۓs�s�n�}�Ή����Ă��Ȃ��̂Œ���
	AheMESH2LL( iMesh, iX, iY, lTmp.Longitude, lTmp.Latitude );

	return TRUE;
}

BOOL AheURISiNDYMesh2MeshXY(CString strURI, MeshXY& MeshXY)
{
	USES_CONVERSION;

	CString 	strTmp, strCompWord, strValue;
	INT			iCnt = 0;
	LPTSTR 		lpszTmp;

	// URI��؂�̂Ă�
	strURI = strURI.Right( strURI.GetLength() - 22 );
	while( strURI.CompareNoCase( _T("") ) != 0 ) {
		INT 		iCount = 0, iValue = 0;
		CString		szTmp;

		// &�ʒu�擾
		iCount = strURI.Find( _T("&") );
		if( iCount < 0 )
			strTmp = strURI;
		else
			strTmp = strURI.Left( iCount );

		// =�ʒu�擾
		iValue = strTmp.Find( _T("=") );
		if( iValue < 0 )
			return FALSE;

		// ���͂��ꂽ������擾
		strCompWord = strTmp.Left( iValue );
		// ���͂��ꂽ�l�擾
		strValue = strTmp.Right( strTmp.GetLength() - iValue - 1 );

		if( strCompWord.CompareNoCase( _T("meshcode") ) == 0 ) {
			if ( iCnt != 0 )
				return FALSE;
			iCnt++;

			if ( AheGetKeywordValue( strTmp, strValue, _T("meshcode=0123456789"), szTmp, 0 ) == FALSE )
				return FALSE;

			TCHAR* stopstring = NULL;
			MeshXY.MeshCode = _tcstoul(szTmp, &stopstring, 10);
		} else if( strCompWord.CompareNoCase( _T("x") ) == 0 ) {
			if ( iCnt != 1 )
				return FALSE;
			iCnt++;

			if ( AheGetKeywordValue( strTmp, strValue, _T("x=0123456789"), szTmp, 0 ) == FALSE )
				return FALSE;

			TCHAR* stopstring = NULL;
			MeshXY.X = _tcstoul(szTmp, &stopstring, 10);
		} else if( strCompWord.CompareNoCase( _T("y") ) == 0 ) {
			if ( iCnt != 2 )
				return FALSE;
			iCnt++;

			if ( AheGetKeywordValue( strTmp, strValue, _T("y=0123456789"), szTmp, 0 ) == FALSE )
				return FALSE;

			TCHAR* stopstring = NULL;
			MeshXY.Y = _tcstoul(szTmp, &stopstring, 10);
		} else if( strCompWord.CompareNoCase( _T("scale") ) == 0 ) {
			if ( iCnt != 3 )
				return FALSE;

			if ( AheGetKeywordValue( strTmp, strValue, _T("scale=0123456789."), szTmp, 0 ) == FALSE )
				return FALSE;

				MeshXY.Scale = _tcstod( (LPCTSTR)szTmp, &lpszTmp );
		} else if( strCompWord.CompareNoCase( _T("") ) == 0 ) {
			return FALSE;
		} else {
			return FALSE;
		}

		if( iCount < 0 )
			break;
		strURI = strURI.Right( strURI.GetLength() - iCount - 1 );
	}
	return TRUE;
}

ISpatialReferencePtr AheGetSpatialReference( IUnknownPtr ipUnk )
{
	ISpatialReferencePtr ipSpRef;
	IGeometryPtr ipGeometry( ipUnk );
	IEnvelopePtr ipEnv( ipUnk );
	IFeaturePtr ipFeature( ipUnk );
	if( ipFeature )
		ipFeature->get_Shape( &ipGeometry );

	if( ipGeometry )
		ipGeometry->get_SpatialReference( &ipSpRef );
	else if( ipEnv )
		ipEnv->get_SpatialReference( &ipSpRef );
	else {		
		IGeoDatasetPtr ipDataset = AheGetFeatureClass( ipUnk );
		if( ipDataset == NULL ) return NULL;

		ipDataset->get_SpatialReference( &ipSpRef );
	}

	return ipSpRef;
}
