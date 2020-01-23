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

// LinkQueue.cpp: CLinkQueue �N���X�̃C���v�������e�[�V����
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
	// �L���b�V���̃R�s�[�͊e�N���X�̃I�y���[�^�ɔC����
	src.CopyTo( dest );		// INF���R�s�[
	dest.m_listLQ = src.m_listLQ;	// LQ����R�s�[

	return sindyErr_NoErr;
}

// �����o���N���A����
void CLinkQueue::clear()
{
	// �������j�����܂܂��̂ŁA�C�����C���ɂ����DLL�ł̓��쎞��
	// �܂������߁Acpp�̕��ɏ������ƁI�I
	m_listLQ.clear();
}

errorcode::sindyErrCode CLinkQueue::Materialization(ITable* ipInfTable, ITable* ipLqTable)
{
	errorcode::sindyErrCode emErr = errorcode::sindyErr_NoErr; // �߂�l

	if ( ! IsDeleted() ) {
		if ( errorcode::sindyErr_NoErr == (emErr = CRow::Materialization( ipInfTable, ipLqTable )) ) {
			long lOID = GetOID(); // ���̉����OBJECTID

			// �֘A���R�[�h�����̉�
			for ( iterator itLQ = begin(), itLQEnd = end(); itLQ != itLQEnd; ++itLQ ) {
				emErr = CRow::Materialization( ipLqTable );	// LQ��CContainer�g���悤�ɂȂ����炢��Ȃ�
				emErr = (*itLQ).SetInfId( lOID );
			}
		}
	}

	return emErr;
}

// LQ���Z�b�g����
sindyErrCode CLinkQueue::push_back( sindyTableType::ECode emType, _IRow* lp, CSPFieldMap& cFields, CSPTableNameString& cNames )
{
	sindyErrCode emErr = sindyErr_NoErr;	// �Ԃ�l

	link_queue::CLQ cLQ( lp, emType, false, cFields, cNames );

/*	// �R���e�i�̒��g��0�ŁA�e�[�u���^�C�v����`����Ă��Ȃ��ꍇ��
	// emType�̒l���画�f����
	if( m_listLQ.empty() && sindyTableType::unknown == m_emTableType )
		m_emTableType = AheTableName2TableType( _T("INF_") + CString(AheTableType2TableName( emType ) + 3) ); <- �Ƃ肠�����AINF ����ɃZ�b�g����Ă��邱�Ƃ�O������Ƃ���*/

	// �e�[�u�����̔�r
	LPCTSTR lpcszInfName = GetTableName();
	LPCTSTR lpcszLQName  = cLQ.GetTableName();
	LOGASSERTEERR_IF( lpcszInfName && lpcszLQName, sindyErr_AlgorithmFailed )
	{
		LOGASSERTEERR_IF( lstrcmpi( lpcszInfName + 4, lpcszLQName + 3 ) == 0, sindyErr_InfTableIsNotMatchLQTable )	// INF_�ALQ_�ȉ����������ǂ���
		{
/*			// LQ���Z�b�g���ꂽ���INF���Z�b�g����邱�Ƃ����邪�A
			// ���̏ꍇ GetOID() �����s����̂� INF ������Ƃ��̂�
			// �ȉ��̃`�F�b�N������ <- �Ƃ肠���� INF ����ɃZ�b�g����Ă��邱�Ƃ�O��ɂ���
			if( p )
			{*/
				// �֘AID���`�F�b�N
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
	// �}�[�W�����LQ���擾
	link_queue::CLQ* pSourceLQ = NULL;
	link_queue::CLQ* pDestLQ = NULL;

	for ( CLinkQueue::const_iterator itLQ = begin(); itLQ != end(); ++itLQ ) {
		if ( (*itLQ).GetLinkId() == cSource.GetOID() )	// �}�[�W���H
			pSourceLQ = const_cast<link_queue::CLQ*>(&(*itLQ));
		else if ( (*itLQ).GetLinkId() == cDest.GetOID() )	// �}�[�W��H
			pDestLQ = const_cast<link_queue::CLQ*>(&(*itLQ));
	}

	if ( pDestLQ ) {
		if ( pSourceLQ && pDestLQ ) {	// �}�[�W���旼���Ƀ����N�񂪂���H
			if ( pDestLQ->GetLastLink() == 1 ) {	// �}�[�W���LQ���ILQ�H
				// �}�[�W����LQ��LAST_LINKF��1�ɂ���B
				if ( sindyErr_NoErr != pSourceLQ->SetLastLink( 1 ) )
					cErrObjects.push_back( CErrorObject( (*pSourceLQ) ) );
			} else {
				// �}�[�W���LQ��SEQUENCE�ȍ~�����炷�B
				for ( CLinkQueue::iterator itLQ = begin(); itLQ != end(); ++itLQ ) {
					if ( (*itLQ).GetSequence() > pDestLQ->GetSequence() ) {
						if ( sindyErr_NoErr != (*itLQ).SetSequence( (*itLQ).GetSequence() - 1 ) )
							cErrObjects.push_back( CErrorObject( (*itLQ) ) );
					}
				}
			}

			// �}�[�W���LQ���폜
			pDestLQ->Delete();
		} else if ( !pSourceLQ && pDestLQ ) {	// �}�[�W��ɂ��������N�񂪂Ȃ��H
			// LINK_ID���}�[�W���̓��H�ɕύX
			if ( sindyErr_NoErr != pDestLQ->SetLinkId( cSource.GetOID() ) )
				cErrObjects.push_back( CErrorObject( (*pDestLQ) ) );

			if ( bIsReverse ) {	// �}�[�W����̓��H�̌������Ⴄ�H
				// LINKDIR_C��ύX
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

	// �����N�񂪕��f���ꂽ���`�F�b�N
	for ( CLinkQueue::const_iterator itLQ = begin(); itLQ != end(); ++itLQ ) {
		if ( (*itLQ).GetLinkId() == cSource.GetOID() ) {
			bool bFast = false, bLast = false;

			if ( (*itLQ).GetLastLink() == 1 )	// �I�[�H
				bLast = true;

			if ( (*itLQ).GetSequence() == 1 )	// �n�H
				bFast = true;

			if ( bFast && bLast )	// ��������Ȃ番�f�ł͂Ȃ��̂ŏ������Ȃ�
				break;

			if ( bLast ) {
				// �}�[�W����LQ�̑O��LQ�擾
				for ( CLinkQueue::const_iterator itLQ2 = itLQ; itLQ2 != begin(); --itLQ2 ) {
					if ( ! (*itLQ2).IsDeleted() ) {
						itLQ = itLQ2;
						break;
					}
				}
			} else {
				// �}�[�W����LQ�̎���LQ�擾
				for ( CLinkQueue::const_iterator itLQ2 = itLQ; itLQ2 != end(); ++itLQ2 ) {
					if ( ! (*itLQ2).IsDeleted() ) {
						itLQ = itLQ2;
						break;
					}
				}
			}

			bool bDeleteLinkq = false;

			// ���H��FROM/TO�擾
			CGeometry cFrom = cSource.GetShape()->GetPoint( 0 );
			CGeometry cTo = cSource.GetShape()->GetPoint( cSource.GetShape()->GetPointCount() - 1 );

			// �}�[�W����LQ�̑O��LQ or �}�[�W����LQ�̎���LQ�̓��H��FROM/TO�Ɣ�r
			for ( CContainer::const_rows_iterator itRoad = cRoads.begin(emTableType), itRoadEnd = cRoads.end(emTableType); itRoad != itRoadEnd; ++itRoad ) {
				if ( (*itRoad)->GetOID() == (*itLQ).GetLinkId() ) {
					CGeometry cPt;

					// FROM/TO�擾
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

					// �}�[�W�������H��FROM/TO�Ɉ�v���Ȃ���Ε��f���ꂽ�Ɣ��f���ă����N����폜����B
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
	// LQ�Q�폜
	for ( iterator itLQ = begin(); itLQ != end(); ++itLQ )
		(*itLQ).Delete();

	// INF�폜
	CRow::Delete();

	return cErrObjects.GetDefaultErrCode();
}

errorcode::sindyErrCode CLinkQueue::DeleteLQ( const CRow& cRow, CLinkQueue& cNewLinkq, CErrorObjects& cErrObjects )
{
	schema::ipc_table::update_type::ECode emUpdateType = schema::ipc_table::update_type::kDefault;

	// �폜�ς݂ł͂Ȃ��H
	if ( !Changed( &emUpdateType ) || emUpdateType != schema::ipc_table::update_type::kDeleted ) {
		// LQ�ɓ��H������H
		if ( HasRoad(cRow) ) {
			std::map<long, std::list<link_queue::CLQ*>> mapSeqs; // map<�C���f�b�N�X,list<LQ>>

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
							if ( itSeq->second.back()->GetSequence() + 1 == nSeq )	// ���̃V�[�P���X�ƑO�̃V�[�P���X�������Ă���H
								break;
						}

						mapSeqs[Idx].push_back( &(*itLQ) );
					}
				}
			}

			if ( mapSeqs.empty() ) {	// �Ȃ����Ă��Ƃ͍폜
				CLinkQueue::Delete( cErrObjects );
			} else {
				// �������������N��̐�������
				for ( std::map<long, std::list<link_queue::CLQ*>>::iterator itIdx = mapSeqs.begin(), itIdxEnd = mapSeqs.end(); itIdx != itIdxEnd; ++itIdx ) {
					long nSeq = 1;

					for ( std::list<link_queue::CLQ*>::iterator itLQ = itIdx->second.begin(); itLQ != itIdx->second.end(); ++itLQ, ++nSeq ) {
						// �V�[�P���X���Ȃ���
						if ( sindyErr_NoErr != (*itLQ)->SetSequence( nSeq ) )
							cErrObjects.push_back( CErrorObject( (*(*itLQ)) ) );

						// �I�[�t���O���Ȃ���
						if ( sindyErr_NoErr != (*itLQ)->SetLastLink( 0 ) )
							cErrObjects.push_back( CErrorObject( (*(*itLQ)) ) );
					}

					// �I�[�t���O���Ȃ���
					if ( sindyErr_NoErr != itIdx->second.back()->SetLastLink( 1 ) )
						cErrObjects.push_back( CErrorObject( (*itIdx->second.back()) ) );

					if ( itIdx != mapSeqs.begin() ) {
						CTable cInfTable;
						CTable cRoadTable( cRow.GetTable() );

						if ( ! (_IRow*)(*this) )
							cInfTable.SetObject( GetTableType(), cRoadTable.OpenSameOwnerTable( GetTableType() ) );
						else
							cInfTable.SetObject( GetTable() );

						// �_�~�[�쐬
						cInfTable.CreateDummyRow( cNewLinkq );

						// �������R�s�[
						CopyTo( cNewLinkq, true, false, false );

						// LQ�Q�R�s�[
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
	// INF������
	CRow::swap( cLinkq, bIsCopyCache, bIsCopyRow, bIsCopyFlag, bIsCopyContainer );

	if ( bIsSwapLQ ) {	// LQ�Q����������H
		std::list<link_queue::CLQ> listswap;	// �ꎞ�p

		// �ꎞ�p�ɑޔ�
		for ( const_iterator itLQ = cLinkq.begin(), itLQEnd = cLinkq.end(); itLQ != itLQEnd; ++itLQ )
			listswap.push_back( (*itLQ) );

		// cLinkq��LQ�Q���폜
		for ( std::list<link_queue::CLQ>::iterator itLQ = listswap.begin(); itLQ != listswap.end(); ++itLQ ) 
			cLinkq.remove_lq( (*itLQ) );

		// �ꎞ�p�ƌ���
		m_listLQ.swap( listswap );

		// �ꎞ�p�ƌ���
		for ( std::list<link_queue::CLQ>::iterator itLQ = listswap.begin(); itLQ != listswap.end(); ++itLQ ) 
			cLinkq.push_back_lq( (*itLQ) );
	}
}

IGeometryPtr CLinkQueue::CreateLQGeometry(bool bUseEditUpdate, sindyTableType::ECode emTableType, const CContainer& cRoads)
{
	// �I���`��쐬
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

			// �`��擾
			for ( CContainer::const_rows_iterator itRoad = cRoads.begin(emTableType), itRoadEnd = cRoads.end(emTableType); itRoad != itRoadEnd; ++itRoad ) {
				if ( (*itRoad)->GetOID() == (*itLQ).GetLinkId() ) {
					ipRoadGeom = (*itRoad)->GetShapeCopy();
					break;
				}
			}

			if ( ipRoadGeom ) {
				// ���]
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

	// �폜�ς݂ł͂Ȃ��H
	if ( !Changed( &emUpdateType ) || emUpdateType != schema::ipc_table::update_type::kDeleted ) {
		// LQ�ɓ��H������H
		if ( HasRoad(cOrgRoadRow) ) {
			bool bLast = false, bDir = false;
			std::list<link_queue::CLQ> listNewLQs;	// �V�KLQ�Q
			iterator itPosition = end();	// �}���ʒu
			int nSeq = 0; 	// �V�[�P���X

			for ( iterator itLQ = begin(); itLQ != end(); ++itLQ ) {
				if ( cOrgRoadRow.GetOID() == (*itLQ).GetLinkId() ) {
					if ( (*itLQ).GetLinkDir() == 2 )
						bDir = true;

					if ( (*itLQ).GetLastLink() == 1 ) {
						// �I�[�t���O���Ȃ���
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

					nSeq = (*itLQ).GetSequence(); 	// �V�[�P���X
					int nNewSeq = nSeq + (bDir ? 0 : 1);

					CTable cRoadTable( cOrgRoadRow.GetTable() );

					for ( std::list<CSPRow>::const_iterator itRoad = listTmps.begin(), itRoadEnd = listTmps.end(); itRoad != itRoadEnd; ++itRoad, ++nNewSeq ) {
						// LQ�쐬
						link_queue::CLQ cNewLQ;

						CTable cLQTable;

						if ( ! (_IRow*)(*itLQ) )
							cLQTable.SetObject( (*itLQ).GetTableType(), cRoadTable.OpenSameOwnerTable( (*itLQ).GetTableType() ) );
						else
							cLQTable.SetObject( (*itLQ).GetTable() );

						cLQTable.CreateRow( cNewLQ );

						// �����R�s�[
						itLQ->CopyTo( cNewLQ, true, false, false, false );

						// LinkID�Z�b�g
						if ( sindyErr_NoErr != cNewLQ.SetLinkId( (*itRoad)->GetOID() ) )
							cErrObjects.push_back( CErrorObject( cNewLQ ) );

						// �V�[�P���X�Z�b�g
						if ( sindyErr_NoErr != cNewLQ.SetSequence( nNewSeq ) )
							cErrObjects.push_back( CErrorObject( cNewLQ ) );

						listNewLQs.push_back( cNewLQ );
					}

					if ( bDir ) {
						// �V�[�P���X���Ȃ���
						if ( sindyErr_NoErr != (*itLQ).SetSequence( (*itLQ).GetSequence() + listNewLQs.size() ) )
							cErrObjects.push_back( CErrorObject( (*itLQ) ) );

						if ( bLast ) {	// �I�[�t���O����H
							// �I�[�t���O���Ȃ���
							if ( sindyErr_NoErr != (*itLQ).SetLastLink( 1 ) )
								cErrObjects.push_back( CErrorObject( (*itLQ) ) );
						}
					} else {
						if ( bLast ) {	// �I�[�t���O����H
							// ��Ԃ���LQ�ɏI�[�t���O������
							if(listNewLQs.empty())
								break;
							if ( sindyErr_NoErr != listNewLQs.back().SetLastLink( 1 ) )
								cErrObjects.push_back( CErrorObject( listNewLQs.back() ) );
						}
					}

					itPosition = itLQ;	// �}���ʒu�擾
					break;
				}
			}

			std::list<link_queue::CLQ> listLQ;	// LQ�Q����ւ��p

			for ( iterator itLQ = begin(); itLQ != end(); ++itLQ ) {
				if ( itLQ == itPosition ) {
					if ( bDir ) {
						// �V�KLQ�Q��ǉ�
						for ( std::list<link_queue::CLQ>::const_iterator itNew = listNewLQs.begin(), itNewEnd = listNewLQs.end(); itNew != itNewEnd; ++itNew )
							listLQ.push_back( *itNew );

						listLQ.push_back( *itLQ );
					} else {
						listLQ.push_back( *itLQ );

						// �V�KLQ�Q��ǉ�
						for ( std::list<link_queue::CLQ>::const_iterator itNew = listNewLQs.begin(), itNewEnd = listNewLQs.end(); itNew != itNewEnd; ++itNew )
							listLQ.push_back( *itNew );
					}
				} else {
					if ( (*itLQ).GetSequence() > nSeq ) {
						// �V�[�P���X���Ȃ���
						if ( sindyErr_NoErr != (*itLQ).SetSequence( (*itLQ).GetSequence() + listNewLQs.size() ) )
							cErrObjects.push_back( CErrorObject( (*itLQ) ) );
					}

					listLQ.push_back( *itLQ );
				}
			}

			m_listLQ.swap( listLQ );	// ����ւ�
			m_listLQ.sort();
		}
	}

	return cErrObjects.GetDefaultErrCode();
}

// �ҏW���e��ۑ�����
sindyErrCode CLinkQueue::Store( CErrorObjects& cErrObjects )
{
	// INF�̕ۑ�
	if( Changed() )
		CInfo::Store( cErrObjects );

	// LQ�̕ۑ�
	for( iterator it = begin(); it != end(); ++it )
	{
		if( it->Changed() )
			it->Store( cErrObjects );
	}
	return cErrObjects.GetDefaultErrCode();
}

// ���ɕҏW����Ă��Ȃ����ǂ����m�F����
sindyErrCode CLinkQueue::CheckModify( CErrorObjects& cErrObjects ) const
{
	if( Changed() )
		CRow::CheckModify( cErrObjects );
		
	// ����͂��Ȃ������ť���B
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

// ���������`�F�b�N����
errorcode::sindyErrCode CLinkQueue::CheckLogic( const CLogicRule& /*cRule*/, CErrorObjects& cErrObjects ) const
{
	schema::ipc_table::update_type::ECode emUpdateType = schema::ipc_table::update_type::kDefault;

	// �ҏW�������́��폜����Ă��Ȃ����̂̂݃`�F�b�N
	if ( Changed( &emUpdateType ) && emUpdateType != schema::ipc_table::update_type::kDeleted ) {
		long iSeq = 1;
		bool bLastF = false;

		for ( const_iterator itLQ = begin(); itLQ != end(); ++itLQ ) {
			(*itLQ).Changed( &emUpdateType );

			// �폜�ς݂ł͂Ȃ��H
			if ( emUpdateType != schema::ipc_table::update_type::kDeleted ) {
				if ( iSeq != (*itLQ).GetSequence() )	// ���������������H
					cErrObjects.push_back( CErrorObject( (*itLQ), CErrorInfo( sindyErr_LinkQueueBadSequence, sindyErrLevel_ERR, NULL, 0, -1, schema::link_queue::kSequence ) ) );

				if ( (*itLQ).GetLastLink() == 1 ) {
					if ( bLastF )	// �I�[�t���O���ق��ɂ������H
						cErrObjects.push_back( CErrorObject( (*itLQ), CErrorInfo( sindyErr_LinkQueueLastLinkFs, sindyErrLevel_ERR, NULL, 0, -1, schema::link_queue::kLastLink ) ) );
					else
						bLastF = true;
				}

				++iSeq;
			}
		}

		if ( ! bLastF )	// �I�[�t���O���Ȃ������H
			cErrObjects.push_back( CErrorObject( *this, CErrorInfo( sindyErr_LinkQueueNotLastLinkF, sindyErrLevel_ERR, NULL, 0, -1, schema::link_queue::kLastLink ) ) );
	}

	return cErrObjects.GetDefaultErrCode();
}

bool CLinkQueue::Changed( schema::ipc_table::update_type::ECode* emUpdateType/* = NULL*/ ) const
{
	bool bChanged = false;

	// INF�̕ύX���`�F�b�N
	bChanged = CInfo::Changed( emUpdateType );

	if ( !bChanged ) {
		// INF�ɕύX���Ȃ��Ă�LQ�ɂ͕ύX�����邩������Ȃ��̂ŁALQ���ύX����Ă��邩�`�F�b�N
		for ( const_iterator it = begin(); it != end(); ++it ) {
			if ( it->Changed( emUpdateType ) ) {
				// �ύX�����������͑����ύX�ɂ��Ă���
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
