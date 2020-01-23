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

// LinkQueue.cpp: CLinkQueue クラスのインプリメンテーション
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "LinkQueue.h"
#include "ErrorCode.h"
#include "TableTypeUtil.h"
#include "Geometry.h"
#include "Table.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

namespace sindy {
using namespace errorcode;

sindyErrCode CopyCLinkQueue( CLinkQueue& dest, const CLinkQueue& src )
{
	// キャッシュのコピーは各クラスのオペレータに任せる
	src.CopyTo( dest );		// INFをコピー
	dest.m_listLQ = src.m_listLQ;	// LQ列をコピー

	return sindyErr_NoErr;
}

// メンバをクリアする
void CLinkQueue::clear()
{
	// メモリ破棄が含まれるので、インラインにするとDLLでの動作時に
	// まずいため、cppの方に書くこと！！
	m_listLQ.clear();
}

errorcode::sindyErrCode CLinkQueue::Materialization(ITable* ipInfTable, ITable* ipLqTable)
{
	errorcode::sindyErrCode emErr = errorcode::sindyErr_NoErr; // 戻り値

	if ( ! IsDeleted() ) {
		if ( errorcode::sindyErr_NoErr == (emErr = CRow::Materialization( ipInfTable, ipLqTable )) ) {
			long lOID = GetOID(); // 実体化後のOBJECTID

			// 関連レコードを実体化
			for ( iterator itLQ = begin(), itLQEnd = end(); itLQ != itLQEnd; ++itLQ ) {
				emErr = CRow::Materialization( ipLqTable );	// LQがCContainer使うようになったらいらない
				emErr = (*itLQ).SetInfId( lOID );
			}
		}
	}

	return emErr;
}

// LQをセットする
sindyErrCode CLinkQueue::push_back( sindyTableType::ECode emType, _IRow* lp, CSPFieldMap& cFields, CSPTableNameString& cNames )
{
	sindyErrCode emErr = sindyErr_NoErr;	// 返り値

	link_queue::CLQ cLQ( lp, emType, false, cFields, cNames );

/*	// コンテナの中身が0で、テーブルタイプが定義されていない場合は
	// emTypeの値から判断する
	if( m_listLQ.empty() && sindyTableType::unknown == m_emTableType )
		m_emTableType = AheTableName2TableType( _T("INF_") + CString(AheTableType2TableName( emType ) + 3) ); <- とりあえず、INF が先にセットされていることを前提条件とする*/

	// テーブル名の比較
	LPCTSTR lpcszInfName = GetTableName();
	LPCTSTR lpcszLQName  = cLQ.GetTableName();
	LOGASSERTEERR_IF( lpcszInfName && lpcszLQName, sindyErr_AlgorithmFailed )
	{
		LOGASSERTEERR_IF( lstrcmpi( lpcszInfName + 4, lpcszLQName + 3 ) == 0, sindyErr_InfTableIsNotMatchLQTable )	// INF_、LQ_以下が同じかどうか
		{
/*			// LQがセットされた後でINFがセットされることがあるが、
			// その場合 GetOID() が失敗するので INF があるときのみ
			// 以下のチェックをする <- とりあえず INF が先にセットされていることを前提にする
			if( p )
			{*/
				// 関連IDをチェック
				LOGASSERTEERR_IF( GetOID() == cLQ.GetInfId(), sindyErr_InfIDInvalid )
					m_listLQ.push_back( cLQ );
//			}
		}
	}

	return emErr;
}

bool CLinkQueue::HasRoad(const CRow& cRow) const
{
	for ( const_iterator itLQ = begin(); itLQ != end(); ++itLQ ) {
		if ( cRow.GetOID() == (*itLQ).GetLinkId() )
			return true;
	}

	return false;
}

errorcode::sindyErrCode CLinkQueue::Merge( const CRow& cSource, const CRow& cDest, bool bIsReverse, sindyTableType::ECode emTableType, const CContainer& cRoads, CErrorObjects& cErrObjects )
{
	// マージ元先のLQを取得
	link_queue::CLQ* pSourceLQ = NULL;
	link_queue::CLQ* pDestLQ = NULL;

	for ( CLinkQueue::const_iterator itLQ = begin(); itLQ != end(); ++itLQ ) {
		if ( (*itLQ).GetLinkId() == cSource.GetOID() )	// マージ元？
			pSourceLQ = const_cast<link_queue::CLQ*>(&(*itLQ));
		else if ( (*itLQ).GetLinkId() == cDest.GetOID() )	// マージ先？
			pDestLQ = const_cast<link_queue::CLQ*>(&(*itLQ));
	}

	if ( pDestLQ ) {
		if ( pSourceLQ && pDestLQ ) {	// マージ元先両方にリンク列がある？
			if ( pDestLQ->GetLastLink() == 1 ) {	// マージ先のLQが終LQ？
				// マージ元のLQのLAST_LINKFを1にする。
				if ( sindyErr_NoErr != pSourceLQ->SetLastLink( 1 ) )
					cErrObjects.push_back( CErrorObject( (*pSourceLQ) ) );
			} else {
				// マージ先のLQのSEQUENCE以降をずらす。
				for ( CLinkQueue::iterator itLQ = begin(); itLQ != end(); ++itLQ ) {
					if ( (*itLQ).GetSequence() > pDestLQ->GetSequence() ) {
						if ( sindyErr_NoErr != (*itLQ).SetSequence( (*itLQ).GetSequence() - 1 ) )
							cErrObjects.push_back( CErrorObject( (*itLQ) ) );
					}
				}
			}

			// マージ先のLQを削除
			pDestLQ->Delete();
		} else if ( !pSourceLQ && pDestLQ ) {	// マージ先にしかリンク列がない？
			// LINK_IDをマージ元の道路に変更
			if ( sindyErr_NoErr != pDestLQ->SetLinkId( cSource.GetOID() ) )
				cErrObjects.push_back( CErrorObject( (*pDestLQ) ) );

			if ( bIsReverse ) {	// マージ元先の道路の向きが違う？
				// LINKDIR_Cを変更
				if ( pDestLQ->GetLinkDir() == 1 ) {
					if ( sindyErr_NoErr != pDestLQ->SetLinkDir( 2 ) )
						cErrObjects.push_back( CErrorObject( (*pDestLQ) ) );
				} else {
					if ( sindyErr_NoErr != pDestLQ->SetLinkDir( 1 ) )
						cErrObjects.push_back( CErrorObject( (*pDestLQ) ) );
				}
			}
		}
	}

	// リンク列が分断されたかチェック
	for ( CLinkQueue::const_iterator itLQ = begin(); itLQ != end(); ++itLQ ) {
		if ( (*itLQ).GetLinkId() == cSource.GetOID() ) {
			bool bFast = false, bLast = false;

			if ( (*itLQ).GetLastLink() == 1 )	// 終端？
				bLast = true;

			if ( (*itLQ).GetSequence() == 1 )	// 始？
				bFast = true;

			if ( bFast && bLast )	// 両方あるなら分断ではないので処理しない
				break;

			if ( bLast ) {
				// マージしたLQの前のLQ取得
				for ( CLinkQueue::const_iterator itLQ2 = itLQ; itLQ2 != begin(); --itLQ2 ) {
					if ( ! (*itLQ2).IsDeleted() ) {
						itLQ = itLQ2;
						break;
					}
				}
			} else {
				// マージしたLQの次のLQ取得
				for ( CLinkQueue::const_iterator itLQ2 = itLQ; itLQ2 != end(); ++itLQ2 ) {
					if ( ! (*itLQ2).IsDeleted() ) {
						itLQ = itLQ2;
						break;
					}
				}
			}

			bool bDeleteLinkq = false;

			// 道路のFROM/TO取得
			CGeometry cFrom = cSource.GetShape()->GetPoint( 0 );
			CGeometry cTo = cSource.GetShape()->GetPoint( cSource.GetShape()->GetPointCount() - 1 );

			// マージしたLQの前のLQ or マージしたLQの次のLQの道路のFROM/TOと比較
			for ( CContainer::const_rows_iterator itRoad = cRoads.begin(emTableType), itRoadEnd = cRoads.end(emTableType); itRoad != itRoadEnd; ++itRoad ) {
				if ( (*itRoad)->GetOID() == (*itLQ).GetLinkId() ) {
					CGeometry cPt;

					// FROM/TO取得
					if ( bLast ) {
						if ( (*itLQ).GetLinkDir() == 2 )
							cPt = (*itRoad)->GetShape()->GetPoint( 0 );
						else
							cPt = (*itRoad)->GetShape()->GetPoint( (*itRoad)->GetShape()->GetPointCount() - 1 );
					} else {
						if ( (*itLQ).GetLinkDir() == 2 )
							cPt = (*itRoad)->GetShape()->GetPoint( (*itRoad)->GetShape()->GetPointCount() - 1 );
						else
							cPt = (*itRoad)->GetShape()->GetPoint( 0 );
					}

					// マージした道路のFROM/TOに一致しなければ分断されたと判断してリンク列を削除する。
					if ( ! cFrom.Equals( cPt ) && ! cTo.Equals( cPt ) )
						bDeleteLinkq = true;

					break;
				}
			}

			if ( bDeleteLinkq )
				Delete( cErrObjects );

			break;
		}
	}

	return cErrObjects.GetDefaultErrCode();
}

errorcode::sindyErrCode CLinkQueue::Delete( CErrorObjects& cErrObjects )
{
	// LQ群削除
	for ( iterator itLQ = begin(); itLQ != end(); ++itLQ )
		(*itLQ).Delete();

	// INF削除
	CRow::Delete();

	return cErrObjects.GetDefaultErrCode();
}

errorcode::sindyErrCode CLinkQueue::DeleteLQ( const CRow& cRow, CLinkQueue& cNewLinkq, CErrorObjects& cErrObjects )
{
	schema::ipc_table::update_type::ECode emUpdateType = schema::ipc_table::update_type::kDefault;

	// 削除済みではない？
	if ( !Changed( &emUpdateType ) || emUpdateType != schema::ipc_table::update_type::kDeleted ) {
		// LQに道路がある？
		if ( HasRoad(cRow) ) {
			std::map<long, std::list<link_queue::CLQ*>> mapSeqs; // map<インデックス,list<LQ>>

			for ( iterator itLQ = begin(); itLQ != end(); ++itLQ ) {
				if ( cRow.GetOID() == (*itLQ).GetLinkId() )
					(*itLQ).Delete();
				else {
					schema::ipc_table::update_type::ECode emUpdateType = schema::ipc_table::update_type::kDefault;

					if ( !(*itLQ).Changed( &emUpdateType ) || emUpdateType != schema::ipc_table::update_type::kDeleted ) {
						long nSeq = (*itLQ).GetSequence();
						long Idx = 0;

						for ( std::map<long, std::list<link_queue::CLQ*>>::const_iterator itSeq = mapSeqs.begin(), itSeqEnd = mapSeqs.end(); itSeq != itSeqEnd; ++itSeq, ++Idx )
						{
							if ( itSeq->second.back()->GetSequence() + 1 == nSeq )	// 今のシーケンスと前のシーケンスが続いている？
								break;
						}

						mapSeqs[Idx].push_back( &(*itLQ) );
					}
				}
			}

			if ( mapSeqs.empty() ) {	// ないってことは削除
				CLinkQueue::Delete( cErrObjects );
			} else {
				// 分割したリンク列の整合処理
				for ( std::map<long, std::list<link_queue::CLQ*>>::iterator itIdx = mapSeqs.begin(), itIdxEnd = mapSeqs.end(); itIdx != itIdxEnd; ++itIdx ) {
					long nSeq = 1;

					for ( std::list<link_queue::CLQ*>::iterator itLQ = itIdx->second.begin(); itLQ != itIdx->second.end(); ++itLQ, ++nSeq ) {
						// シーケンスつけなおし
						if ( sindyErr_NoErr != (*itLQ)->SetSequence( nSeq ) )
							cErrObjects.push_back( CErrorObject( (*(*itLQ)) ) );

						// 終端フラグつけなおし
						if ( sindyErr_NoErr != (*itLQ)->SetLastLink( 0 ) )
							cErrObjects.push_back( CErrorObject( (*(*itLQ)) ) );
					}

					// 終端フラグつけなおし
					if ( sindyErr_NoErr != itIdx->second.back()->SetLastLink( 1 ) )
						cErrObjects.push_back( CErrorObject( (*itIdx->second.back()) ) );

					if ( itIdx != mapSeqs.begin() ) {
						CTable cInfTable;
						CTable cRoadTable( cRow.GetTable() );

						if ( ! (_IRow*)(*this) )
							cInfTable.SetObject( GetTableType(), cRoadTable.OpenSameOwnerTable( GetTableType() ) );
						else
							cInfTable.SetObject( GetTable() );

						// ダミー作成
						cInfTable.CreateDummyRow( cNewLinkq );

						// 属性をコピー
						CopyTo( cNewLinkq, true, false, false );

						// LQ群コピー
						for ( std::list<link_queue::CLQ*>::iterator itLQ = itIdx->second.begin(); itLQ != itIdx->second.end(); ++itLQ ) {
							link_queue::CLQ cLq( *(*itLQ) );

							cNewLinkq.push_back_lq( cLq );
							remove_lq( *(*itLQ) );
						}
					}
				}

				CLinkQueue::sort();
				cNewLinkq.sort();
			}
		}
	}

	return cErrObjects.GetDefaultErrCode();
}

void CLinkQueue::swap( CLinkQueue& cLinkq, bool bIsSwapLQ, bool bIsCopyCache/*=true*/, bool bIsCopyRow/*=true*/, bool bIsCopyFlag/*=true*/, bool bIsCopyContainer/*=true*/ )
{
	// INFを交換
	CRow::swap( cLinkq, bIsCopyCache, bIsCopyRow, bIsCopyFlag, bIsCopyContainer );

	if ( bIsSwapLQ ) {	// LQ群を交換する？
		std::list<link_queue::CLQ> listswap;	// 一時用

		// 一時用に退避
		for ( const_iterator itLQ = cLinkq.begin(), itLQEnd = cLinkq.end(); itLQ != itLQEnd; ++itLQ )
			listswap.push_back( (*itLQ) );

		// cLinkqのLQ群を削除
		for ( std::list<link_queue::CLQ>::iterator itLQ = listswap.begin(); itLQ != listswap.end(); ++itLQ ) 
			cLinkq.remove_lq( (*itLQ) );

		// 一時用と交換
		m_listLQ.swap( listswap );

		// 一時用と交換
		for ( std::list<link_queue::CLQ>::iterator itLQ = listswap.begin(); itLQ != listswap.end(); ++itLQ ) 
			cLinkq.push_back_lq( (*itLQ) );
	}
}

IGeometryPtr CLinkQueue::CreateLQGeometry(bool bUseEditUpdate, sindyTableType::ECode emTableType, const CContainer& cRoads)
{
	// 選択形状作成
	IGeometryPtr ipLQGeom;

	for ( const_iterator itLQ = begin(), itLQEnd = end(); itLQ != itLQEnd; ++itLQ ) {
		bool bAdd = false;

		if ( bUseEditUpdate ) {
			schema::ipc_table::update_type::ECode emUpdateType = schema::ipc_table::update_type::kDefault;

			if ( !(*itLQ).Changed( &emUpdateType ) || emUpdateType != schema::ipc_table::update_type::kDeleted )
				bAdd = true;
		} else
			bAdd = true;

		if ( bAdd ) {
			IGeometryPtr ipRoadGeom;

			// 形状取得
			for ( CContainer::const_rows_iterator itRoad = cRoads.begin(emTableType), itRoadEnd = cRoads.end(emTableType); itRoad != itRoadEnd; ++itRoad ) {
				if ( (*itRoad)->GetOID() == (*itLQ).GetLinkId() ) {
					ipRoadGeom = (*itRoad)->GetShapeCopy();
					break;
				}
			}

			if ( ipRoadGeom ) {
				// 反転
				if ( (*itLQ).GetLinkDir() == 2 )
					((IPolylinePtr)ipRoadGeom)->ReverseOrientation();

				if ( ipLQGeom ) {
					//ITopologicalOperatorPtr ipTopo = ipLQGeom;
					//IGeometryPtr ipResultGeom;

					long lBaseCount=0, lAddCount=0;

					lBaseCount = CGeometry( ipLQGeom ).GetPointCount();
					lAddCount = CGeometry( ipRoadGeom ).GetPointCount();

					WKSPoint* point_wks = new WKSPoint[lBaseCount+lAddCount];

					((IPointCollectionPtr)ipLQGeom)->QueryWKSPoints( 0, lBaseCount, point_wks );
					((IPointCollectionPtr)ipRoadGeom)->QueryWKSPoints( 0, lAddCount, &(point_wks[lBaseCount - 1]) );

					((IPointCollectionPtr)ipLQGeom)->SetWKSPoints( lBaseCount + lAddCount - 1, point_wks );

					delete [] point_wks;
					point_wks = NULL;
					//if ( ipTopo )
					//	ipTopo->Union( ipRoadGeom, &ipResultGeom );

					//ipLQGeom = ipResultGeom;
				} else
					ipLQGeom = ipRoadGeom;
			}
		}
	}

	return ipLQGeom;
}

errorcode::sindyErrCode CLinkQueue::Split( const CRow& cOrgRoadRow, sindyTableType::ECode emTableType, const CContainer& cNewRoads, CErrorObjects& cErrObjects )
{
	schema::ipc_table::update_type::ECode emUpdateType = schema::ipc_table::update_type::kDefault;

	// 削除済みではない？
	if ( !Changed( &emUpdateType ) || emUpdateType != schema::ipc_table::update_type::kDeleted ) {
		// LQに道路がある？
		if ( HasRoad(cOrgRoadRow) ) {
			bool bLast = false, bDir = false;
			std::list<link_queue::CLQ> listNewLQs;	// 新規LQ群
			iterator itPosition = end();	// 挿入位置
			int nSeq = 0; 	// シーケンス

			for ( iterator itLQ = begin(); itLQ != end(); ++itLQ ) {
				if ( cOrgRoadRow.GetOID() == (*itLQ).GetLinkId() ) {
					if ( (*itLQ).GetLinkDir() == 2 )
						bDir = true;

					if ( (*itLQ).GetLastLink() == 1 ) {
						// 終端フラグつけなおし
						if ( sindyErr_NoErr != (*itLQ).SetLastLink( 0 ) )
							cErrObjects.push_back( CErrorObject( (*itLQ) ) );

						bLast = true;
					}

					std::list<CSPRow> listTmps;
					for ( CContainer::const_rows_iterator itRoad = cNewRoads.begin(emTableType), itRoadEnd = cNewRoads.end(emTableType); itRoad != itRoadEnd; ++itRoad ) {
						if ( bDir )
							listTmps.push_front( (*itRoad) );
						else
							listTmps.push_back( (*itRoad) );
					}

					nSeq = (*itLQ).GetSequence(); 	// シーケンス
					int nNewSeq = nSeq + (bDir ? 0 : 1);

					CTable cRoadTable( cOrgRoadRow.GetTable() );

					for ( std::list<CSPRow>::const_iterator itRoad = listTmps.begin(), itRoadEnd = listTmps.end(); itRoad != itRoadEnd; ++itRoad, ++nNewSeq ) {
						// LQ作成
						link_queue::CLQ cNewLQ;

						CTable cLQTable;

						if ( ! (_IRow*)(*itLQ) )
							cLQTable.SetObject( (*itLQ).GetTableType(), cRoadTable.OpenSameOwnerTable( (*itLQ).GetTableType() ) );
						else
							cLQTable.SetObject( (*itLQ).GetTable() );

						cLQTable.CreateRow( cNewLQ );

						// 属性コピー
						itLQ->CopyTo( cNewLQ, true, false, false, false );

						// LinkIDセット
						if ( sindyErr_NoErr != cNewLQ.SetLinkId( (*itRoad)->GetOID() ) )
							cErrObjects.push_back( CErrorObject( cNewLQ ) );

						// シーケンスセット
						if ( sindyErr_NoErr != cNewLQ.SetSequence( nNewSeq ) )
							cErrObjects.push_back( CErrorObject( cNewLQ ) );

						listNewLQs.push_back( cNewLQ );
					}

					if ( bDir ) {
						// シーケンスつけなおし
						if ( sindyErr_NoErr != (*itLQ).SetSequence( (*itLQ).GetSequence() + listNewLQs.size() ) )
							cErrObjects.push_back( CErrorObject( (*itLQ) ) );

						if ( bLast ) {	// 終端フラグあり？
							// 終端フラグつけなおし
							if ( sindyErr_NoErr != (*itLQ).SetLastLink( 1 ) )
								cErrObjects.push_back( CErrorObject( (*itLQ) ) );
						}
					} else {
						if ( bLast ) {	// 終端フラグあり？
							// 一番けつのLQに終端フラグをつける
							if(listNewLQs.empty())
								break;
							if ( sindyErr_NoErr != listNewLQs.back().SetLastLink( 1 ) )
								cErrObjects.push_back( CErrorObject( listNewLQs.back() ) );
						}
					}

					itPosition = itLQ;	// 挿入位置取得
					break;
				}
			}

			std::list<link_queue::CLQ> listLQ;	// LQ群入れ替え用

			for ( iterator itLQ = begin(); itLQ != end(); ++itLQ ) {
				if ( itLQ == itPosition ) {
					if ( bDir ) {
						// 新規LQ群を追加
						for ( std::list<link_queue::CLQ>::const_iterator itNew = listNewLQs.begin(), itNewEnd = listNewLQs.end(); itNew != itNewEnd; ++itNew )
							listLQ.push_back( *itNew );

						listLQ.push_back( *itLQ );
					} else {
						listLQ.push_back( *itLQ );

						// 新規LQ群を追加
						for ( std::list<link_queue::CLQ>::const_iterator itNew = listNewLQs.begin(), itNewEnd = listNewLQs.end(); itNew != itNewEnd; ++itNew )
							listLQ.push_back( *itNew );
					}
				} else {
					if ( (*itLQ).GetSequence() > nSeq ) {
						// シーケンスつけなおし
						if ( sindyErr_NoErr != (*itLQ).SetSequence( (*itLQ).GetSequence() + listNewLQs.size() ) )
							cErrObjects.push_back( CErrorObject( (*itLQ) ) );
					}

					listLQ.push_back( *itLQ );
				}
			}

			m_listLQ.swap( listLQ );	// 入れ替え
			m_listLQ.sort();
		}
	}

	return cErrObjects.GetDefaultErrCode();
}

// 編集内容を保存する
sindyErrCode CLinkQueue::Store( CErrorObjects& cErrObjects )
{
	// INFの保存
	if( Changed() )
		CInfo::Store( cErrObjects );

	// LQの保存
	for( iterator it = begin(); it != end(); ++it )
	{
		if( it->Changed() )
			it->Store( cErrObjects );
	}
	return cErrObjects.GetDefaultErrCode();
}

// 既に編集されていないかどうか確認する
sindyErrCode CLinkQueue::CheckModify( CErrorObjects& cErrObjects ) const
{
	if( Changed() )
		CRow::CheckModify( cErrObjects );
		
	// 現状はやらない方向で･･･。
	//for( const_iterator it = begin(); it != end(); ++it )
	//{
	//	if( it->Changed() )
	//	{
	//		CErrorObject cErrObj( *it );
	//		if( sindyErr_NoErr != it->CheckModify( cErrObj.push_back( CErrorInfos() ) ) )
	//			cErrObjects.push_back( cErrObj );
	//	}
	//}
	return cErrObjects.GetDefaultErrCode();
}

// 整合性をチェックする
errorcode::sindyErrCode CLinkQueue::CheckLogic( const CLogicRule& /*cRule*/, CErrorObjects& cErrObjects ) const
{
	schema::ipc_table::update_type::ECode emUpdateType = schema::ipc_table::update_type::kDefault;

	// 編集したもの＆削除されていないもののみチェック
	if ( Changed( &emUpdateType ) && emUpdateType != schema::ipc_table::update_type::kDeleted ) {
		long iSeq = 1;
		bool bLastF = false;

		for ( const_iterator itLQ = begin(); itLQ != end(); ++itLQ ) {
			(*itLQ).Changed( &emUpdateType );

			// 削除済みではない？
			if ( emUpdateType != schema::ipc_table::update_type::kDeleted ) {
				if ( iSeq != (*itLQ).GetSequence() )	// 順序がおかしい？
					cErrObjects.push_back( CErrorObject( (*itLQ), CErrorInfo( sindyErr_LinkQueueBadSequence, sindyErrLevel_ERR, NULL, 0, -1, schema::link_queue::kSequence ) ) );

				if ( (*itLQ).GetLastLink() == 1 ) {
					if ( bLastF )	// 終端フラグがほかにあった？
						cErrObjects.push_back( CErrorObject( (*itLQ), CErrorInfo( sindyErr_LinkQueueLastLinkFs, sindyErrLevel_ERR, NULL, 0, -1, schema::link_queue::kLastLink ) ) );
					else
						bLastF = true;
				}

				++iSeq;
			}
		}

		if ( ! bLastF )	// 終端フラグがなかった？
			cErrObjects.push_back( CErrorObject( *this, CErrorInfo( sindyErr_LinkQueueNotLastLinkF, sindyErrLevel_ERR, NULL, 0, -1, schema::link_queue::kLastLink ) ) );
	}

	return cErrObjects.GetDefaultErrCode();
}

bool CLinkQueue::Changed( schema::ipc_table::update_type::ECode* emUpdateType/* = NULL*/ ) const
{
	bool bChanged = false;

	// INFの変更をチェック
	bChanged = CInfo::Changed( emUpdateType );

	if ( !bChanged ) {
		// INFに変更がなくてもLQには変更があるかもしれないので、LQが変更されているかチェック
		for ( const_iterator it = begin(); it != end(); ++it ) {
			if ( it->Changed( emUpdateType ) ) {
				// 変更があった時は属性変更にしておく
				if ( emUpdateType )
					*emUpdateType = schema::ipc_table::update_type::kAttrUpdated;

				bChanged = true;
				break;
			}
		}
	}

	return bChanged;
}

} // sindy
