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

/**
 * @file DispLine.cpp
 * @brief CDispLineクラス実装ファイル
 * @author 地図ＤＢ制作部開発グループ 古川貴宏
 *
 * $Id$
 */
#include "stdafx.h"
#include "DispLine.h"
#include "util.h"
#include "AnnoGlobals.h"
#include "GlobalFunctions.h"
#include "AheWKSFunctions.h"
#include "../../WinLib/math2.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

namespace sindy {

#ifdef SINDYLIB_CALC_TIME
double g_dCalcStringBoundsTime = 0.0;
double g_dCalcFlagStringBoundsTime = 0.0;
double g_dCalcPositionTime = 0.0;
double g_dCalcParameterTime = 0.0;
double g_dQueryPointsTime = 0.0;

void CDispLine::TraceTime()
{
	TRACEMESSAGE( _T("                          ======================================================\n"));
	TRACEMESSAGE( _T("CDispLine               : 計測時間結果 :\n") );
	TRACEMESSAGE( _T("                        : CalcStringBounds     総時間             : %.5lf秒\n"), g_dCalcFlagStringBoundsTime/1000.0 );
	TRACEMESSAGE( _T("                        : CalcPosition         総時間             : %.5lf秒\n"), g_dCalcPositionTime/1000.0 );
	TRACEMESSAGE( _T("                        : CalcParameter        総時間             : %.5lf秒\n"), g_dCalcParameterTime/1000.0 );
	TRACEMESSAGE( _T("                          ------------------------------------------------------\n"));
	TRACEMESSAGE( _T("                        :                      QueryWKSPoints     : %.5lf秒\n"), g_dQueryPointsTime/1000.0 );
	TRACEMESSAGE( _T("                        :                      AheCalcStringBounds: %.5lf秒\n"), g_dCalcStringBoundsTime/1000.0 );
	TRACEMESSAGE( _T("                          ======================================================\n"));
}
#endif // ifdef SINDYLIB_CALC_TIME

using namespace errorcode;

void CDispLine::Init()
{
	CRow::Init();
	m_lSecondLineStartIndex = -1;
	m_nStringRectsSize = -1;
	m_pStringRects = NULL;
	m_cRect.XMin = m_cRect.YMin = m_cRect.XMax = m_cRect.YMax = -1;
	m_emLineType = (sindyAnnotationLineType)-1;
	m_bIsFlag = false;
	m_bCanCopy = false;
}

void CDispLine::DeleteCache( bool bDeleteContainerCache/* = false*/ )
{
	CRow::DeleteCache(bDeleteContainerCache);
	ClearParameterCache();
}

void CDispLine::ClearParameterCache()
{
	if( m_pStringRects )
	{
		delete [] m_pStringRects;
		m_pStringRects = NULL;
	}
}

// パラメータキャッシュをセットする
void CDispLine::SetParameterCache( int nStringRectSize, long lSecondLineStartIndex, WKSPoint* pStringRects, const WKSEnvelope& cRect, IGeometry* ipGeom, sindyAnnotationLineType emLineType, bool bIsFlag, bool bCanCopy )
{
	m_nStringRectsSize = nStringRectSize;
	m_pStringRects = pStringRects;
	m_cRect = cRect;
	m_ipGeom = ipGeom;
	m_emLineType = emLineType;
	m_bIsFlag = bIsFlag;
	m_bCanCopy = bCanCopy;
	m_lSecondLineStartIndex = lSecondLineStartIndex;
}

// コピーする
sindyErrCode CDispLine::CopyTo( CRow& dest, bool bIsCopyCache/*=true*/, bool bIsCopyRow/*=true*/, bool bIsCopyFlag/*=true*/, bool bIsCopyContainer/*=true*/ ) const
{
	// CDispLine以外は通さない
	CDispLine* pDest = dynamic_cast<CDispLine*>( &dest );
	if( ! pDest )
		return sindyErr_ArgIsConsistency;

	sindyErrCode emErr = CRow::CopyTo( dest, bIsCopyCache, bIsCopyRow, bIsCopyFlag, bIsCopyContainer );
	if( sindyErr_NoErr == emErr )
	{
		pDest->m_nStringRectsSize = m_nStringRectsSize;
		if( pDest->m_pStringRects )
			delete [] pDest->m_pStringRects;
		if( m_pStringRects )
		{
			pDest->m_pStringRects = new WKSPoint[m_nStringRectsSize];
			CopyMemory( pDest->m_pStringRects, m_pStringRects, sizeof(WKSPoint) );
		}
		else
			pDest->m_pStringRects = NULL;
		pDest->m_lSecondLineStartIndex = m_lSecondLineStartIndex;
		pDest->m_cRect = m_cRect;
		pDest->m_ipGeom = AheCreateClone( m_ipGeom );
		pDest->m_emLineType = m_emLineType;
		pDest->m_bIsFlag = m_bIsFlag;
		pDest->m_bCanCopy = m_bCanCopy;
	}
	return emErr;
}
///////////////////////////////////////////////////////////////////////////////////////////////////
//
// チェック関数
//
///////////////////////////////////////////////////////////////////////////////////////////////////
// フォントサイズ値をチェックする
sindyErrCode CDispLine::CheckFontSize( LPCTSTR lpcszNameString ) const
{
	sindyErrCode emErr = sindyErr_NoErr;	// 返り値

	switch( GetFontSize() )
	{
		// [bug 3768] フォントサイズが10の時は、漢字が使用されていない場合に限りエラー、漢字が使用されているなら警告とする
		case schema::disp_line::font_size::k10:
		{
			bool bHasKanji = false;							// 漢字が使われているかどうか

			USES_CONVERSION;

			// SJISで比較するので、コード変換
			LPCSTR lpcszNameStringA = T2CA(lpcszNameString);
			size_t nLen = strlen(lpcszNameStringA);
			for( size_t i = 0; i < nLen; ++i )
			{
				bool bIsWide = false;
				if( issjiswide( lpcszNameStringA[i], lpcszNameStringA[i+1] ) )
				{
					bIsWide = true;
					if( issjiskanji( lpcszNameStringA[i], lpcszNameStringA[i+1] ) )
					{
						bHasKanji = true;
						break;
					}
				}
				if( bIsWide ) ++i;
			}
			emErr = ( bHasKanji ) ? sindyErr_AnnoDispFontSizeInvalidKanji : sindyErr_AnnoDispFontSizeInvalidWarn;
		}
		break;
		case schema::disp_line::font_size::k12:
		case schema::disp_line::font_size::k14:
		case schema::disp_line::font_size::k16:
		case schema::disp_line::font_size::k20:
		case schema::disp_line::font_size::k24: break;
		default: emErr = sindyErr_AnnoDispFontSizeInvalid; break;
	}
	return emErr;
}

// 関連注記真位置IDをチェックする
sindyErrCode CDispLine::CheckAnnoID() const
{
	return ( GetAnnoID() < 0 ) ? sindyErr_AnnoDispRelIDInvalid : sindyErr_NoErr;
}

// 参照文字列コードをチェックする
sindyErrCode CDispLine::CheckTargetStr() const
{
	sindyErrCode emErr = sindyErr_NoErr;	// 返り値

	switch( GetTargetStr() )
	{
		case schema::disp_line::target_str::kStr1:
		case schema::disp_line::target_str::kStr2: break;
		case schema::disp_line::target_str::kStr3: emErr = ( GetTableType() == sindyTableType::city_annotation ) ? sindyErr_AnnoDispTargetStrInvalid : sindyErr_NoErr; break;
		default: emErr = sindyErr_AnnoDispTargetStrInvalid; break;
	}
	return emErr;
}

// 表示ラインの角度をチェックする
sindyErrCode CDispLine::CheckAngle( sindyAnnotationLineType emLineType, double* dErrAngle ) const // TODO: 第一引数いらないよね
{
	sindyErrCode emErr = sindyErr_NoErr;	// 返り値

	if( emLineType & sindyAnnoLine_Linear )	// チェックするのは線状注記のみ
	{
		LOGASSERTEERR_IF( GetShape(), sindyErr_GeometryNotCached )
		{
			IPointCollectionPtr ipCol;
			LOGASSERTEERR_IF( SUCCEEDED( ((IGeometry*)(*GetShape()))->QueryInterface( IID_IPointCollection, (void**)&ipCol ) ), sindyErr_COMQueryInterfaceFailed )
			{
				long lCount = 0;
				ipCol->get_PointCount( &lCount );
				WKSPoint* pPoints = new WKSPoint[lCount];
				LOGASSERTEERR_IF( SUCCEEDED( ipCol->QueryWKSPoints( 0, lCount, pPoints ) ), sindyErr_GeometryFunctionFailed )
				{
					for( long i = 0; i < lCount - 3; ++i )	// 3点の角度をチェックするのは4点（文字数は3）以上
					{
						double angle = rad2angle( WKS3PAngle( pPoints[i], pPoints[i+1], pPoints[i+2] ) );
						if( ! ( (double)135.0 <= angle && angle <= (double)225.0 ) )
						{
							emErr = sindyErr_AnnoStrAngleInvalid;
							if( dErrAngle ) *dErrAngle = angle;
							break;
						}
					}

				}
				delete [] pPoints;
			}
		}
	}

	return emErr;
}

// 表示ライン形状の整合性をチェックする
sindyErrCode CDispLine::CheckLine( schema::annotation_class::disptype_rule::ECode emDispTypeRule ) const
{
	sindyErrCode emErr = sindyErr_NoErr; // 戻り値

	// ラインタイプルールとラインタイプの整合チェック（bug 6537）
	switch( emDispTypeRule )
	{
		case schema::annotation_class::disptype_rule::kLinear:
			if( sindyAnnoLine_Linear != m_emLineType )
				emErr = sindyErr_AnnoDispLineTypeInvalid; // ルールが「線状」なのにラインタイプが「線状」ではない
			break;
		default:
			if( sindyAnnoLine_Linear == m_emLineType )
					emErr = sindyErr_AnnoDispLineTypeInvalid; // ルールが「線状」じゃないのにラインタイプが「線状」
			break;
	}
	// 2行注記の場合に一文字しか存在しない行があるかどうかのチェック（bug 6537）
	// 折り返し時のX差分が負であるかどうかのチェック（bug 6537）
	if( (sindyAnnotationLineType)(sindyAnnoLine_Double|sindyAnnoLine_Horizonal) == m_emLineType )
	{
		if( m_lSecondLineStartIndex == 1 ||                     // 一行目が一文字しかない
			m_nStringRectsSize - m_lSecondLineStartIndex == 1 ) // 二行目が一文字しかない
			emErr = sindyErr_AnnoDispLineIllegalDouble; // 一文字しか存在しない行がある
		else {
			if( GetShape()->GetPoint(m_lSecondLineStartIndex).GetX() - 
				GetShape()->GetPoint( m_lSecondLineStartIndex - 1).GetX() >= 0 )
				emErr = sindyErr_AnnoDispLineIllegalDouble; // 折り返し差分が0以上
		}
	}

	return emErr;
}
///////////////////////////////////////////////////////////////////////////////////////////////////
//
// その他の関数
//
///////////////////////////////////////////////////////////////////////////////////////////////////
// 1pt当たりの長さを取得する
double CDispLine::Calc1PTLen( bool bCalcX ) const
{
	switch( GetTableType() )
	{
	case sindyTableType::city_disp_line: return ( bCalcX ) ? ANNOTATION_1PT_CITY_X : ANNOTATION_1PT_CITY_Y; break;
	case sindyTableType::c_sc1disp_line: return ( bCalcX ) ? ANNOTATION_1PT_CSC1_X : ANNOTATION_1PT_CSC1_Y; break;
	case sindyTableType::b_sc4disp_line: return ( bCalcX ) ? ANNOTATION_1PT_BSC4_X : ANNOTATION_1PT_BSC4_Y; break;
	case sindyTableType::b_sc3disp_line: return ( bCalcX ) ? ANNOTATION_1PT_BSC3_X : ANNOTATION_1PT_BSC3_Y; break;
	case sindyTableType::b_sc2disp_line: return ( bCalcX ) ? ANNOTATION_1PT_BSC2_X : ANNOTATION_1PT_BSC2_Y; break;
	case sindyTableType::b_sc1disp_line: return ( bCalcX ) ? ANNOTATION_1PT_BSC1_X : ANNOTATION_1PT_BSC1_Y; break;
	case sindyTableType::m_sc4disp_line: return ( bCalcX ) ? ANNOTATION_1PT_MSC4_X : ANNOTATION_1PT_MSC4_Y; break;
	case sindyTableType::m_sc3disp_line: return ( bCalcX ) ? ANNOTATION_1PT_MSC3_X : ANNOTATION_1PT_MSC3_Y; break;
	case sindyTableType::m_sc2disp_line: return ( bCalcX ) ? ANNOTATION_1PT_MSC2_X : ANNOTATION_1PT_MSC2_Y; break;
	case sindyTableType::m_sc1disp_line: return ( bCalcX ) ? ANNOTATION_1PT_MSC1_X : ANNOTATION_1PT_MSC1_Y; break;
	case sindyTableType::t_sc4disp_line: return ( bCalcX ) ? ANNOTATION_1PT_TSC4_X : ANNOTATION_1PT_TSC4_Y; break;
	case sindyTableType::t_sc3disp_line: return ( bCalcX ) ? ANNOTATION_1PT_TSC3_X : ANNOTATION_1PT_TSC3_Y; break;
	case sindyTableType::t_sc2disp_line: return ( bCalcX ) ? ANNOTATION_1PT_TSC2_X : ANNOTATION_1PT_TSC2_Y; break;
	case sindyTableType::t_sc1disp_line: return ( bCalcX ) ? ANNOTATION_1PT_TSC1_X : ANNOTATION_1PT_TSC1_Y; break;
	default:
		SASSERT(false);
		ERRORLOG(sindyErr_InvalidTypeInfo);	// 型情報が一致しない
	}
	return 0.0;
}

// 表示文字外形を計算する
errorcode::sindyErrCode CDispLine::CalcStringBounds( IPoint* pAnnoPoint, IGeometry* ipBound ) const
{
#ifdef SINDYLIB_CALC_TIME
	double dStartFunc = GetTickCount();
#endif // ifdef SINDYLIB_CALC_TIME

	sindyErrCode emErr = sindyErr_NoErr;	// 返り値

	IPointCollectionPtr ipCol( ipBound );

	if( NULL != ipCol && NULL != pAnnoPoint )
	{
		VARIANT_BOOL vbDisjoint = VARIANT_FALSE;
		if( SUCCEEDED( ((IRelationalOperatorPtr)ipBound)->Disjoint( pAnnoPoint, &vbDisjoint ) ) )
		{
			if( vbDisjoint )
			{
				const CGeometry* pGeom = GetShape();
				if( pGeom )
				{
					// 真位置座標取得
					WKSPoint pb;
					pAnnoPoint->QueryCoords( &pb.X, &pb.Y );

					// 点列を取得
					long lCount = 0;
					ipCol->get_PointCount( &lCount );
					WKSPoint* pPoints = new WKSPoint[lCount];
					ipCol->QueryWKSPoints( 0, lCount, pPoints );


					// 吹き出し線を作成するのに適したセグメントを見つける
					double dAngleMax = 0.0; // 各セグメントのFrom、Toと真位置の角度の差の最大値
					long lIndexMax = -1;	// 上記を記録した時のiの値

					for( long i = 0; i < lCount - 1; ++i )
					{
						bool bIsCross = false;

						long j = i + 1;
						while( j != i )
						{
							long jn = ( j < lCount - 1 ) ? j+1 : 0; // jの次の値

							// 各セグメントのFrom、Toと真位置を結ぶ線分がipBoundと交差しないもののみ処理
							if( WKSIsCross( pb, pPoints[i],   pPoints[j], pPoints[jn], false ) ||
								WKSIsCross( pb, pPoints[i+1], pPoints[j], pPoints[jn], false ) )
							{
								bIsCross = true;
								break;
							}
							// カウンタ処理
							j = ( j < lCount - 1 ) ? ++j : j=0;
						}
						if( ! bIsCross )
						{
							// 各セグメントのFrom、Toと真位置の角度の差を取り、一番大きなものを採用する
							double dAngle = WKS3PAngle( pPoints[i], pb, pPoints[i+1] );
							if( dAngleMax < dAngle ) 
							{
								lIndexMax = i;
								dAngleMax = dAngle;
							}
						}
					}

					if( lIndexMax < 0 )
					{
						emErr = sindyErr_AlgorithmFailed; // アルゴリズムエラー
						SASSERT( lIndexMax >= 0 );
						lIndexMax = 0;	// エラーの場合はしょうがないので0番目を使用
					}
					
					// 吹き出しのための2点を作成する
					// 採択セグメントの中点から等距離にある2点を計算
					WKSPoint pc, pc1, pc2;
					pc.X = (pPoints[lIndexMax].X+pPoints[lIndexMax+1].X)/(double)2.0;
					pc.Y = (pPoints[lIndexMax].Y+pPoints[lIndexMax+1].Y)/(double)2.0;
					
					// 採択セグメントの距離の半分と、求める点と中点の距離の比率を出す
					double dRatio = (Calc1PTLen(true)*(double)3.0)/(WKS2PLength(pPoints[lIndexMax],pPoints[lIndexMax+1])/(double)2.0);

					pc1.X = dRatio*pPoints[lIndexMax+1].X + ((double)1.0-dRatio)*pc.X;
					pc1.Y = dRatio*pPoints[lIndexMax+1].Y + ((double)1.0-dRatio)*pc.Y;
					pc2.X = dRatio*pPoints[lIndexMax].X + ((double)1.0-dRatio)*pc.X;
					pc2.Y = dRatio*pPoints[lIndexMax].Y + ((double)1.0-dRatio)*pc.Y;
					
					// 構成点を再作成する
		/*			WKSPoint pPointsNew[3];
					pPointsNew[0] = pc2; pPointsNew[1] = pb; pPointsNew[2] = pc1;
					ipCol->InsertWKSPoints( lIndexMax, 3, pPointsNew );
					((IPolygonPtr)ipCol)->SimplifyPreserveFromTo();*/
					WKSPoint* pPointsNew = new WKSPoint[lCount*3];
					for( long i = 0; i < lCount+3; ++i )
					{
						if( i <= lIndexMax )
							pPointsNew[i] = pPoints[i];
						else if( i == lIndexMax + 1 )
							pPointsNew[i] = pc2;
						else if( i == lIndexMax + 2 )
							pPointsNew[i] = pb;
						else if( i == lIndexMax + 3 )
							pPointsNew[i] = pc1;
						else
							pPointsNew[i] = pPoints[i-3];
					}
					ipCol->RemovePoints( 0, lCount );
					ipCol->SetWKSPoints( lCount+3, pPointsNew );
					((IPolygonPtr)ipCol)->SimplifyPreserveFromTo();
					// 後始末
					delete [] pPointsNew;
					delete [] pPoints;
				}
				else {
					emErr = sindyErr_GeometryNotCached;
					ERRORLOG(emErr);
					SASSERT(pGeom);
				}
			}
		}
		else {
			emErr = sindyErr_GeometryFunctionFailed;
			ERRORLOG(emErr);
			SASSERT(false);
		}
	}
	else {
		emErr = sindyErr_COMInterfaceIsNull;
		ERRORLOG(emErr);
		SASSERT(NULL != ipCol && NULL != pAnnoPoint);
	}

#ifdef SINDYLIB_CALC_TIME
	g_dCalcFlagStringBoundsTime += (double)GetTickCount() - dStartFunc;
#endif // ifdef SINDYLIB_CALC_TIME

	return emErr;
}

// 渡されたポイントに対して表示位置が縦横右左のどの位置にあるかを判定する
sindyAnnotationPosition CDispLine::CalcPosition( const CPoint& cPoint, sindyAnnotationLineType emLineType, const WKSEnvelope& cRect ) const
{
#ifdef SINDYLIB_CALC_TIME
	double dStartFunc = GetTickCount();
#endif // ifdef SINDYLIB_CALC_TIME

	sindyAnnotationPosition emPos = sindyAnnoPos_Other;	// 返り値

	if( emLineType != -1 )
	{
		// 右横、左横の判定
		if( cRect.XMin > cPoint.GetX() )
		{
			if( emLineType | sindyAnnoLine_Horizonal )
				emPos = sindyAnnoPos_RightHorizonal;
		}
		else if( cRect.XMax < cPoint.GetX() )
		{
			if( emLineType | sindyAnnoLine_Horizonal )
				emPos = sindyAnnoPos_LeftHorizonal;
		}
		// 上縦横、下縦横の判定（アルゴリズム上こちらが優先になる）
		if( cRect.YMin > cPoint.GetY() )
		{
			if( emLineType | sindyAnnoLine_Horizonal )
				emPos = sindyAnnoPos_UpperHorizonal;
			else if( emLineType | sindyAnnoLine_Vertical )
				emPos = sindyAnnoPos_UpperVertical;
		}
		else if( cRect.YMax < cPoint.GetY() )
		{
			if( emLineType | sindyAnnoLine_Horizonal )
				emPos = sindyAnnoPos_LowerHorizonal;
			else if( emLineType | sindyAnnoLine_Vertical )
				emPos = sindyAnnoPos_LowerVertical;
		}
	}

#ifdef SINDYLIB_CALC_TIME
	g_dCalcPositionTime += (double)GetTickCount() - dStartFunc;
#endif // ifdef SINDYLIB_CALC_TIME

	return emPos;
}

/*// 注記文字列の外形ポリゴンを取得する
IGeometryPtr CDispLine::CalcStringBounds( long nLen, double** dEnvs ) const
{
	IPointCollectionPtr ipGeomCol( CLSID_Polygon );
	//IPointCollectionPtr ipEnv( CLSID_Ring );
	WKSPoint* pEnv = new WKSPoint[nLen*5];
	for( long i = 0; i < nLen; ++i )
	{
		pEnv[i*5].X   = dEnvs[i][0]; pEnv[i*5].Y   = dEnvs[i][1];
		pEnv[i*5+1].X = dEnvs[i][0]; pEnv[i*5+1].Y = dEnvs[i][3];
		pEnv[i*5+2].X = dEnvs[i][2]; pEnv[i*5+2].Y = dEnvs[i][1];
		pEnv[i*5+3].X = dEnvs[i][2]; pEnv[i*5+3].Y = dEnvs[i][3];
		pEnv[i*5+4].X = dEnvs[i][0]; pEnv[i*5+4].Y = dEnvs[i][1];
	}
	ipGeomCol->SetWKSPoints( nLen*5, pEnv );
	delete [] pEnv;

	// ConvexHullを外形とする
	IGeometryPtr ipOutGeom;
	if( SUCCEEDED( ((IPolygonPtr)ipGeomCol)->SimplifyPreserveFromTo() ) )
	{
		if( FAILED( ((ITopologicalOperatorPtr)ipGeomCol)->ConvexHull( &ipOutGeom ) ) )
			ERRORLOG(sindyErr_GeometryFunctionFailed);
	}
	else
		ERRORLOG(sindyErr_GeometryFunctionFailed);

	return ipOutGeom;
}*/

// 各種パラメータを計算する
sindyAnnotationLineType CDispLine::CalcParameter( bool bIsHalfString, long* pSecondLineStartIndex/* = NULL*/, WKSPoint* pStringRects/* = NULL*/, WKSEnvelope* pRect/* = NULL*/, IGeometry** ipStringBounds/* = NULL*/ ) const
{
#ifdef SINDYLIB_CALC_TIME
	double dStartFunc = GetTickCount();
#endif // ifdef SINDYLIB_CALC_TIME

	sindyAnnotationLineType emLineType = (sindyAnnotationLineType)-1;	// 返り値

	IGeometryPtr ipShape = (IGeometryPtr)(IGeometry*)(*GetShape());
	IPointCollectionPtr ipCol( ipShape );
	WKSPoint* pPoints = NULL;					// ポイント列格納用（高速化の為）
	long lStringCount = 0;						// ポイント数格納用（最終点は含まない）
	WKSPoint* pStringBounds = NULL;				// 個々の文字矩形格納用
	long lSecondLineStartIndex = 0;				// 二行注記の場合の開始インデックス格納用

/*	// 初期化
	if( pRect ) *pRect = WKSRect{ -1, -1, -1, -1 }; <- 開始点で初期化するからいらないんじゃん？*/

#ifdef SINDYLIB_CALC_TIME
	double dStartQueryPoints = GetTickCount();
#endif // ifdef SINDYLIB_CALC_TIME

	if( ipCol != NULL )
	{
		ipCol->get_PointCount( &lStringCount );
		lStringCount--;	// 最終点は含めない
		pPoints = new WKSPoint[lStringCount];
		ipCol->QueryWKSPoints( 0, lStringCount, pPoints );

		if( ipStringBounds != NULL )
			pStringBounds = new WKSPoint[lStringCount*5];

	}

#ifdef SINDYLIB_CALC_TIME
	g_dQueryPointsTime += (double)GetTickCount() - dStartQueryPoints;
#endif // ifdef SINDYLIB_CALC_TIME

	if( pPoints )
	{
		double dStrLenX = ( GetFontSize() * Calc1PTLen( true ) )/(double)( ( bIsHalfString ) ? 2 : 1 );	// 一文字あたりの幅
		double dStrLenY = GetFontSize() * Calc1PTLen( false );											// 一文字あたりの高さ
		double dX = 0, dY = 0, dXPrev = 0, dYPrev = 0;
		int nChangeX = 0, nChangeY = 0;				// 変更チェック用
//		int nRow = 1;							// 行数

		// 最終点の前の点までの座標をチェック
		for( long i = 0; i < lStringCount; ++i )
		{
			// 個々の文字列矩形作成
			if( ipStringBounds != NULL )
			{
				pStringBounds[i*5].X   = pPoints[i].X;            pStringBounds[i*5].Y   = pPoints[i].Y;
				pStringBounds[i*5+1].X = pPoints[i].X;            pStringBounds[i*5+1].Y = pPoints[i].Y + dStrLenY;
				pStringBounds[i*5+2].X = pPoints[i].X + dStrLenX; pStringBounds[i*5+2].Y = pPoints[i].Y + dStrLenY;
				pStringBounds[i*5+3].X = pPoints[i].X + dStrLenX; pStringBounds[i*5+3].Y = pPoints[i].Y;
				pStringBounds[i*5+4].X = pPoints[i].X;            pStringBounds[i*5+4].Y = pPoints[i].Y;
			}

			// 開始点を格納
			if( 0 == i )
			{
				dX = dXPrev = pPoints[i].X;
				dY = dYPrev = pPoints[i].Y;

				if( pRect )
				{
					pRect->XMin = pPoints[i].X;
					pRect->YMin = pPoints[i].Y;
					pRect->XMax = pPoints[i].X;
					pRect->YMax = pPoints[i].Y;
				}
			}
			else {
				// 最大・最小判定
				if( pRect )
				{
					pRect->XMin = min( pRect->XMin, dX );
					pRect->YMin = min( pRect->YMin, dY );
					pRect->XMax = max( pRect->XMax, dX );
					pRect->YMax = max( pRect->YMax, dY );
				}

				if( dXPrev != pPoints[i].X )
					++nChangeX;
				if( dYPrev != pPoints[i].Y )
				{
					++nChangeY;
					lSecondLineStartIndex = i; // 二行注記と判断された場合はこれがpSecondLineStartIndexに反映される
				}
				dXPrev = pPoints[i].X;
				dYPrev = pPoints[i].Y;
			}
		}
		// 一行二行、縦横をチェック
		// 一文字しかない場合は一行横
		switch( lStringCount )
		{
			// 一文字しかない場合は一行横
			case 1: emLineType = (sindyAnnotationLineType)(sindyAnnoLine_Horizonal|sindyAnnoLine_Single); break;
			// 二文字しかない場合は一行縦横、線状
			case 2:
				if( 2 == nChangeX && 2 == nChangeY )
					emLineType = sindyAnnoLine_Linear;
				else if( 1 == nChangeY )
					emLineType = (sindyAnnotationLineType)(sindyAnnoLine_Horizonal|sindyAnnoLine_Single);
				else
					emLineType = (sindyAnnotationLineType)(sindyAnnoLine_Vertical|sindyAnnoLine_Single);
				break;
			// 三文字以上
			default:
				// Y方向に2回以上且つX方向に1回以上変わっていたら線状
				if( 1 < nChangeY && 0 < nChangeX )
					emLineType = sindyAnnoLine_Linear;
				else {
					if( 0 == nChangeY )
						emLineType = (sindyAnnotationLineType)(sindyAnnoLine_Horizonal|sindyAnnoLine_Single);
					else if( 1 == nChangeY )
						emLineType = (sindyAnnotationLineType)(sindyAnnoLine_Horizonal|sindyAnnoLine_Double);
					else
						emLineType = (sindyAnnotationLineType)(sindyAnnoLine_Vertical|sindyAnnoLine_Single);
				}
				break;
		}
		// 点列はもういらないので削除
		delete [] pPoints;
		pPoints = NULL;

		if( pStringRects )
			CopyMemory( pStringRects, pStringBounds, sizeof(WKSPoint)*(lStringCount*5) );

		if( pSecondLineStartIndex )
			*pSecondLineStartIndex = ( emLineType & sindyAnnoLine_Double ) ? lSecondLineStartIndex : 0;

#ifdef SINDYLIB_CALC_TIME
		double dStartStringBounds = GetTickCount();
#endif // SINDYLIB_CALC_TIME

		// バウンディングボックス作成
		if( ipStringBounds )
		{
			SASSERT( *ipStringBounds == NULL );

			IGeometryPtr ipStringBoundsTmp = AheCalcStringBounds( pStringBounds, lStringCount, emLineType, lSecondLineStartIndex, GetShape()->GetSpatialReference() );
			if( ipStringBoundsTmp != NULL ) { *ipStringBounds = ipStringBoundsTmp; (*ipStringBounds)->AddRef(); }
		}

#ifdef SINDYLIB_CALC_TIME
		g_dCalcStringBoundsTime += (double)GetTickCount() - dStartStringBounds;
#endif // ifdef SINDYLIB_CALC_TIME

	}
	// 後始末
	delete [] pStringBounds;
	pStringBounds = NULL;

#ifdef SINDYLIB_CALC_TIME
	g_dCalcParameterTime += (double)GetTickCount() - dStartFunc;
#endif // ifdef SINDYLIB_CALC_TIME

	return emLineType;
}

// 表示できるスケールの最大値を取得する
sindyAnnotationScale CDispLine::GetMaxScale() const
{
	return AheTableType2AnnotationScale( GetTableType() );
}

} // sindy
