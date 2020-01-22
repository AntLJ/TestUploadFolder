#include "stdafx.h"
#include "CheckDistancePost.h"
#include "CCheckDistancePost.h"
#include "Arguments.h"
#include <TDC/useful_headers/str_util.h>
#include <TDC/useful_headers/directory_util.h>
#include "CFeatureClass.h"
#include "tracer/LinkTracer.h"
#include "StdOutput.h"
#include "CLogger.h"
#include <boost/tuple/tuple.hpp>
#include <ArcHelperEx/AheGeometryOp.h>
#include "ErrorType.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

using namespace std;
using namespace ATL;
using namespace err_type;

int _tmain( int argc, _TCHAR* argv[] )
{
	_CrtSetDbgFlag( _CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF );

	try
	{
		const arctl::coinitializer aCoInitializer;

		// ���P�[���֘A�̕s��h�~
		uh::setJapaneseCharacterTypeFacet();

		try
		{
			// �W���o�͊Ǘ��N���X
			StdOutput stdOut;

			// �R�}���h���C����������
			sindy::CArguments args;
			if( !args.parse( argc, argv ) )
			{
				stdOut.writeErrMsg( _T( "�R�}���h���C�������ɖ�肪����܂�" ) );
				return 1;
			}

			// �W���o�̓w�b�_�[�o��
			CString stdOutHeader = stdOut.CreateLogHeader( args.m_strOpParam );
			cout << CT2A( stdOutHeader ) << endl;

			// �ڑ�
			sindy::CCheckDistancePost cCheckDistancePost;

			if( !cCheckDistancePost.Connect( args.m_strDb.c_str() ) )
			{
				stdOut.writeErrMsg( _T( "ERROR:�w��f�[�^�x�[�X�ɐڑ��ł��܂���B" ) );
				return 0;
			}

			double dbBuff = _tcstod( args.m_strBuf.c_str(), NULL );

			// �R�X�g�t�@�C���ǂݍ���
			cCheckDistancePost.ReadRoadCost( args.m_strRoadCost.c_str() );
			cCheckDistancePost.ReadLinkCost( args.m_strLinkCost.c_str() );

			// DistancePostPoint�t�B�[�`���N���X�擾
			if( !cCheckDistancePost.OpenDistancePostPoint() )
			{
				stdOut.writeErrMsg( _T( "ERROR:�w��f�[�^�x�[�X��[DISTANCE_POST_POINT]�͂���܂���B" ) );
				return 1;
			}

			// ROAD_LINK/ROAD_NODE�t�B�[�`���N���X�擾
			if( !cCheckDistancePost.OpenNetwork() )
			{
				stdOut.writeErrMsg( _T( "ERROR:�w��f�[�^�x�[�X��[ROAD_LINK/ROAD_NODE]�͂���܂���B" ) );
				return 1;
			}

			// ��ƊJ�n���ԏo��
			CString startTime = stdOut.GetNow();
			stdOut.writeConsole( _T( "\nstart " ) + startTime + _T( "\n" ) );

			double dbPitch = _tcstod( args.m_strPitch.c_str(), nullptr );
			double dbRange = _tcstod( args.m_strRange.c_str(), nullptr );
			double dbSeparate = _tcstod( args.m_strSeparate.c_str(), nullptr );

			ISpatialReferenceFactory2Ptr ipSpRefFct( CLSID_SpatialReferenceEnvironment );
			IProjectedCoordinateSystemPtr ipPrjCoordinateSystem;
			ipSpRefFct->CreateProjectedCoordinateSystem( esriSRProjCS_TokyoJapan10, &ipPrjCoordinateSystem );

			// �����W�|�C���g�t�B�[�`���N���X/��ԎQ�Ǝ擾
			IFeatureClassPtr distancePostFC = cCheckDistancePost.getDistancePost();
			ISpatialReferencePtr distancePostSpRef;
			( (IGeoDatasetPtr)distancePostFC )->get_SpatialReference( &distancePostSpRef );

			// ���H�m�[�h�t�B�[�`���N���X/��ԎQ�Ǝ擾
			IFeatureClassPtr roadNodeFC = cCheckDistancePost.getRoadNode();
			ISpatialReferencePtr roadNodeSpRef;
			( (IGeoDatasetPtr)roadNodeFC )->get_SpatialReference( &roadNodeSpRef );

			// ���H�����N�t�B�[�`���N���X�擾
			IFeatureClassPtr roadLinkFC = cCheckDistancePost.getRoadLink();
			long roadCodeIdx = -1;
			IFieldsPtr ipFields;
			if( SUCCEEDED( roadLinkFC->get_Fields( &ipFields ) ) ) {
				ipFields->FindField( CComBSTR( sindy::schema::road_link::kRoadCode ), &roadCodeIdx );
			}
			IGeoDatasetPtr ipGeoDataset = (IGeoDatasetPtr)roadLinkFC;
			ISpatialReferencePtr roadLinkSpRef;
			if( ipGeoDataset )
				ipGeoDataset->get_SpatialReference( &roadLinkSpRef );

			// �H�����X�g�`�F�b�N
			std::map<long, long> mapErr;
			std::map<long, std::pair<std::list<double>, std::map<double, long>>> mapContainer;
			long nRet = cCheckDistancePost.CheckRoadList( args.m_strList.c_str(), dbPitch, mapContainer, mapErr );
			switch( nRet )
			{
			case no_problem:
				break;
			case less_point:
				// TODO:�efor�����֐���
				for( const auto& item : mapErr )
				{
					if( item.second == 0 )	// ����
						continue;

					boost::tuple<long, long, CString> roadInfo = cCheckDistancePost.GetRoadList( item.first );
					CString strTmp;
					strTmp.Format( _T( "ERROR:<%d><%d>�̋����W�|�C���g��2�����ł��B" ), roadInfo.get<0>(), roadInfo.get<1>() );
					stdOut.writeErrMsg( strTmp );
				}
				return 1;
			case short_distance:
				for( const auto& item : mapErr )
				{
					if( item.second == 0 )	// ����
						continue;

					boost::tuple<long, long, CString> roadInfo = cCheckDistancePost.GetRoadList( item.first );
					CString strTmp;
					strTmp.Format( _T( "ERROR:<%d><%d>�̍ő勗���W�l�ƍŏ������W�l�̍���<%.1lf>�ȉ��ł��B" ), roadInfo.get<0>(), roadInfo.get<1>(), dbPitch );
					stdOut.writeErrMsg( strTmp );
				}
				return 1;
			default:
				for( const auto& item : mapErr )
				{
					if( item.second == 0 )	// ����
						continue;

					CString strTmp;
					strTmp.Format( _T( "ERROR:�H�����X�g<%d �s��>���������w�肳��Ă��܂���B" ), item.first );
					stdOut.writeErrMsg( strTmp );
				}
				return 1;
			}

			// 2���ԃ`�F�b�N�p map<�H���R�[�h, map<�����R�[�h, map<�����W�L��, �ŒZ�o�H�`��>>>
			std::map<long, std::map<long, std::map<CString, CComPtr<IPolyline>>>> mapRoadGeoms;
			// 2���ԃ`�F�b�N�p map<�H���R�[�h, map<�����R�[�h, map<�����W�L��, map<�����W�l, pair<OID, �|�C���g�`��>>>>>
			std::map<long, std::map<long, std::map<CString, std::map<double, std::pair<long, CComPtr<IPoint>>>>>> mapPts;

			// �H�����X�g���ɏ���
			for( const auto& cRoadList : mapContainer )
			{
				// �ΏۘH���̏��擾(�H���R�[�h, �����R�[�h, �����W�L��)
				boost::tuple<long, long, CString> roadInfo = cCheckDistancePost.GetRoadList( cRoadList.first );

				// �w�肳�ꂽ�H�����Ƃ̍�ƊJ�n���Ԃ��R���\�[���ɏo��
				CString roadInfoMsg;
				roadInfoMsg.Format( _T( "%d\t%d\t%s" ), roadInfo.get<0>(), roadInfo.get<1>(), roadInfo.get<2>() );
				stdOut.writeRoadInfoMsg( roadInfoMsg );

				bool bNG = false;
				// ���s���O�t�@�C���Ǘ��N���X�I�u�W�F�N�g
				sindy::CSPLogger pLogger;
				pLogger.reset( new sindy::CLogger() );
				pLogger->initLog( args.m_strLogPath.c_str(), roadInfo.get<0>(), roadInfo.get<1>(), roadInfo.get<2>(), args.m_strOpParam, true );

				// �����W�|�C���g����
				cCheckDistancePost.searchDistancePoint( cRoadList.second.second );

				// �����W�|�C���g�Q(�����W�l, CSRowBase)
				std::map<double, sindy::CSPRowBase> mapRows = cCheckDistancePost.findDistancePoints();
				std::list<long> listNotExcludeRoads; // ���O���Ă͂����Ȃ����H�i�����W�|�C���g������Ă��铹�H�j
				std::list<long> listExcludeMesh;
				std::list<long> listRouteIDs; // �ǉ��������H�����N��OID�ێ��p
				std::list<std::pair<sindy::CSPRowBase, CString>> listLenErrs; // �|�C���g�ԋ����̃G���[���b�Z�[�W�i�[�p���X�g list(CSPRowBase, �G���[���b�Z�[�W)
				bool allPointOnRoad = true;	// �S�Ă̋����W�|�C���g���H����ɏ���Ă��邩
				IPolylinePtr allRoutePolyline( CLSID_Polyline ); // �S�̌`��i�[�p
				list<long> allRouteIDs;		// �ŒZ�o�H�ƂȂ�S���H�����N��OID
				list<long> excludeOIDs;		// �`��i�[�ς�OID�i�[�p
				IPointPtr toPointOfStartRoad( CLSID_Point ); // �n�_�E���_������Ă���o�H��To�_

				// �����W�|�C���g���̃��[�v [bug12155]
				for( std::list<double>::const_iterator it = cRoadList.second.first.begin(); it != cRoadList.second.first.end(); ++it )
				{
					// ��_�擾
					sindy::CSPRowBase pFirstRow = mapRows[ ( *it ) ];
					double basePointDistance = ( *it ); // ��_�̋����W�l

					// ��_������Ă��铹�H�擾
					sindy::CFeatureClass::Container firstRoadContainer = cCheckDistancePost.searchRoads( pFirstRow->GetShapeCopy(), dbBuff );
					if( firstRoadContainer.empty() )
					{
						CString strTmp;
						strTmp.Format( _T( "ERROR:<%d>�����H�����N��ɐݒ肳��Ă��܂���B" ), pFirstRow->GetOID() );
						pLogger->AddLog( pFirstRow, 1000, strTmp );
						allPointOnRoad = false;
						continue;
					}

					// ���O���Ă͂����Ȃ����H���X�g�Ɋi�[
					for( const auto& cRoads : firstRoadContainer )
						listNotExcludeRoads.push_back( cRoads.first );

					// ���_�擾
					auto nextIt = std::next( it );
					if( nextIt == cRoadList.second.first.end() ) break;
					sindy::CSPRowBase pSecondRow = mapRows[ ( *nextIt ) ];
					double nextPointDistance = ( *nextIt ); // ���_�̋����W�l
					// ���_������Ă��铹�H�擾
					sindy::CFeatureClass::Container secondRoadContainer = cCheckDistancePost.searchRoads( pSecondRow->GetShapeCopy(), dbBuff );
					if( secondRoadContainer.empty() )
					{
						CString strTmp;
						strTmp.Format( _T( "ERROR:<%d>�����H�����N��ɐݒ肳��Ă��܂���B" ), pSecondRow->GetOID() );
						pLogger->AddLog( pSecondRow, 1000, strTmp );
						allPointOnRoad = false;
						it = nextIt;
						continue;
					}

					std::list<long> listSecondRoads; // ���_��OID�i�[�p
					for( const auto& cRoads : secondRoadContainer )
						listSecondRoads.push_back( cRoads.first );

					// ���O���Ă͂����Ȃ����H���X�g�Ɏ��_������Ă��铹�H���i�[
					for( const auto& cSecond : listSecondRoads )
						listNotExcludeRoads.push_back( cSecond );

					// ��_�A���_������Ă��铹�H���ΏۘH���R�[�h�������Ă��邩
					bool firstRoadOnHWY = cCheckDistancePost.checkOnHWY( firstRoadContainer, roadInfo.get<0>(), roadCodeIdx );
					bool secondRoadOnHWY = cCheckDistancePost.checkOnHWY( secondRoadContainer, roadInfo.get<0>(), roadCodeIdx );

					// �����ΏۂƂȂ铹�H�i�[�p�R���e�i. �܂��͊�_������Ă��铹�H���i�[.
					sindy::CFeatureClass::Container cRoads = firstRoadContainer;

					// ���_������Ă��铹�H�����邩�ǂ���
					bool nextRoadFind = false;
					long nBase = 1;

					// �ŒZ�o�H�̌��Ȃ铹�H�����N���擾
					while( !nextRoadFind )
					{
						sindy::CFeatureClass::Container cRoads1;
						std::list<long> listMesh;
						IGeometryPtr searchGeom = cCheckDistancePost.searchRoadGeoms( cRoads, nBase, listExcludeMesh, listMesh );

						if( firstRoadOnHWY && secondRoadOnHWY ) {
							// �ΏۘH���R�[�h�̓��H�ɁA��_�A���_�̗���������Ă���Ƃ��́A�H���R�[�h�Ō���
							cRoads1 = cCheckDistancePost.searchRoads( searchGeom, dbBuff, roadInfo.get<0>() );
						}
						else {
							// cRoadsTmp���̓��H�����݂��郁�b�V�����̌����Ώۓ��H��S�Ď擾
							cRoads1 = cCheckDistancePost.searchRoads( searchGeom, dbBuff );
						}
						listExcludeMesh.merge( listMesh );
						cRoads.insert( cRoads1.begin(), cRoads1.end() );

						// ���H���O
						cCheckDistancePost.excludeRoad( cRoads, listNotExcludeRoads, listRouteIDs );

						// ���_������Ă��铹�H�����邩�m�F
						for( const auto& cRoad : cRoads1 )
						{
							if( listSecondRoads.end() != std::find( listSecondRoads.begin(), listSecondRoads.end(), cRoad.first ) )
							{
								nextRoadFind = true;
								break;
							}
						}
						if( !nextRoadFind )
						{
							++nBase;
						}
					}

					// �o�H�T���J�n
					sindy::LinkTracer cLinkTracer;
					cLinkTracer.toStartSelectLinkSection();
					cLinkTracer.init_cache( cCheckDistancePost.getRoadLink(), dbBuff );

					// �o�H�T���N���X�ɓ��H��n��
					for( const auto& cRoad : cRoads )
						cLinkTracer.add_cache( cRoad.first, (_IRow*)( *cRoad.second.get() ) );
					cLinkTracer.cache();

					// �o�H�T��
					cLinkTracer.onNormalMode( pFirstRow->GetShapeCopy() );
					cLinkTracer.onNormalMode( pSecondRow->GetShapeCopy() );

					// �o�H�T���`��擾
					IPolylinePtr ipPolyline( CLSID_Polyline );
					ipPolyline = cLinkTracer.getRouteGeom();
					ipPolyline->putref_SpatialReference( distancePostSpRef );
					if( !ipPolyline )
					{
						pLogger->AddLog( pFirstRow, 1001, _T( "ERROR:�o�H�v�Z�Ɏ��s���܂����B���H�����N��ɐݒ肳��Ă��Ȃ��\��������܂��B" ) );
						allPointOnRoad = false;
						break;
					}
					// �N���[���擾
					ipPolyline = cCheckDistancePost.Clone( ipPolyline );

					// ���W�n�ύX 
					ipPolyline->Project( ipPrjCoordinateSystem );

					// ��_�`����擾
					IPointPtr ipFirstP( CLSID_Point );
					ipFirstP = pFirstRow->GetShapeCopy();
					ipFirstP->putref_SpatialReference( distancePostSpRef );
					ipFirstP->Project( ipPrjCoordinateSystem );	// ���W�n��ύX

					// ���_�`����擾
					IPointPtr ipSecondP( CLSID_Point );
					ipSecondP = pSecondRow->GetShapeCopy();
					ipSecondP->putref_SpatialReference( distancePostSpRef );
					ipSecondP->Project( ipPrjCoordinateSystem );	// ���W�n��ύX

					// �o�H�T���������H��1�{�̎��ɁA�`��̌��������H�̌����ɂȂ��Ă��鎖������̂�
					// �����������W�|�C���g���u����Ă�������ɂ��킹��
					double checkPointLen = cCheckDistancePost.getPointLen( ipPolyline, ipFirstP, ipSecondP );

					// ���_�ƃ��C���̎n�_�܂ł̋�������_�ƃ��C���̎n�_�܂ł̋��������߂��Ȃ甽�]
					if( checkPointLen < 0.0f ){
						ipPolyline->ReverseOrientation();
					}

					if( allPointOnRoad )
					{
						// �����W�̏���Ă�������Ɍ��������낦�邽�߁A�n�_�`���_������Ă���o�H�͐�Ɋi�[���Ă���
						if( it == cRoadList.second.first.begin() ){
							allRoutePolyline = ipPolyline;
							allRoutePolyline->Project( ipPrjCoordinateSystem );
						}

						// �ŒZ�o�H�ƂȂ郊���N��OID���i�[
						for( const auto& cRoute : cLinkTracer.getRouteIDs() ){
							if( it == cRoadList.second.first.begin() ){
								// �`����i�[���������N��OID�͏��O���X�g�ɒǉ�
								excludeOIDs.push_back( cRoute );
							}
							else{
								// �d�����Ȃ��悤��OID���i�[����
								if( allRouteIDs.end() == std::find( allRouteIDs.begin(), allRouteIDs.end(), cRoute ) ){
									allRouteIDs.push_back( cRoute );
								}
							}
						}
					}

					// ��_�܂ł̋���
					double toBasePointLen = cCheckDistancePost.split_point_length( ipPolyline, ipFirstP );

					// ���_�܂ł̋���
					double toNextPointLen = cCheckDistancePost.split_point_length( ipPolyline, ipSecondP );

					// �����W�|�C���g�Ԃ̓��H�̋������擾
					double pointLen = toNextPointLen - toBasePointLen;

					// �����W�l�̍����擾
					double distanceDiff = nextPointDistance - basePointDistance;

					// �����W�l�̍��Ƌ����W�|�C���g�Ԃ̋������`�F�b�N
					if( !cCheckDistancePost.checkRoadLen( pointLen, distanceDiff, dbPitch, dbRange ) )
					{
						CString strErr;
						strErr.Format( _T( "ERROR:���_�Ƃ̋����W�|�C���g�ԋ����Ə����s�b�`�̍��فA�܂��͋����W�l�̍��Ə����s�b�`�̍��ق�%.1lf���ȏ゠��܂��B" ), dbRange );
						listLenErrs.push_back( std::make_pair( pFirstRow, strErr ) );
					}
				}

				// �|�C���g�ԋ����Ɋւ���G���[���b�Z�[�W���o��
				if( !listLenErrs.empty() ){
					for( const auto& cErr : listLenErrs )
						pLogger->AddLog( cErr.first, 1002, cErr.second );
				}

				if( !allPointOnRoad ){
					stdOut.writeConsole( _T( "����I��" ) );
					continue;
				}

				// �`��i�[�ς̃����N��OID���폜����
				for(const auto& OID : excludeOIDs){
					if( allRouteIDs.end() != std::find( allRouteIDs.begin(), allRouteIDs.end(), OID ) ){
						allRouteIDs.remove( OID );
					}
				}
				// �S�̂̌o�H�`����擾
				sindy::CFeatureClass::Container allRoads = cCheckDistancePost.searchRoadsByOID( allRouteIDs );
				cCheckDistancePost.getRoutePolyline( allRoutePolyline, allRoads, allRouteIDs, ipPrjCoordinateSystem );
				allRoutePolyline->Project( ipPrjCoordinateSystem );	// ���W�n��ύX

				// 2���ԋ����`�F�b�N�pmap���쐬
				mapRoadGeoms[ roadInfo.get<0>() ][ roadInfo.get<1>() ][ roadInfo.get<2>() ] = allRoutePolyline;

				for( const auto& it : cRoadList.second.first )
				{
					sindy::CSPRowBase pRow = mapRows[ it ];

					IPointPtr ipPoint( CLSID_Point );
					ipPoint->putref_SpatialReference( distancePostSpRef );
					ipPoint = pRow->GetShapeCopy();
					ipPoint->Project( ipPrjCoordinateSystem );	// ���W�n��ύX

					mapPts[ roadInfo.get<0>() ][ roadInfo.get<1>() ][ roadInfo.get<2>() ][ it ] = std::pair<long, CComPtr<IPoint>>( pRow->GetOID(), (IPoint*)ipPoint );
				}

				// 2���ԋ����`�F�b�N
				for( std::map<long, std::map<long, std::map<CString, std::map<double, std::pair<long, CComPtr<IPoint>>>>>>::iterator it = mapPts.begin(); it != mapPts.end(); ++it )
				{
					// it->first	�H���R�[�h
					// it->second	map<�����R�[�h,map<>>

					// �����Ă���H���łȂ��A�܂��͘H���R�[�h�ɂQ�̕����R�[�h���R�Â��Ă��Ȃ�
					if( it->first != roadInfo.get<0>() || it->second.size() != 2 )
						continue;

					std::map<long, std::map<CString, std::map<double, std::pair<long, CComPtr<IPoint>>>>>::iterator itFirst = it->second.begin();
					std::map<long, std::map<CString, std::map<double, std::pair<long, CComPtr<IPoint>>>>>::iterator itSecond = it->second.begin();
					++itSecond;

					// �����W�L�����̃��[�v
					for( const auto& pPts2 : itFirst->second )
					{
						sindy::CSPLogger pSeparateLogger;
						pSeparateLogger.reset( new sindy::CLogger() );
						pSeparateLogger->initLog( args.m_strLogPath.c_str(), it->first, itFirst->first, pPts2.first, args.m_strOpParam, false );

						// ����"�����W�L��"�����邩
						if( itSecond->second.end() == itSecond->second.find( pPts2.first ) )
							break;

						// pPts2.first	�����W�L��
						// pPts2.second	map<�����W�l,pair<OID, �|�C���g�`��>>
						for( const auto& pPt : pPts2.second ) // �����W�l���̃��[�v
						{
							// pPt.first	�����W�l
							// pPt.second	pair<OID,�|�C���g�`��>
							if( itSecond->second[ pPts2.first ].end() == itSecond->second[ pPts2.first ].find( pPt.first ) )	// ����"�����W�l"�����邩
							{
								CString strTmp;
								strTmp.Format( _T( "ERROR:����H���ɓ��ꋗ���W�l<%.1lf>�����݂��܂���B" ), pPt.first );
								pSeparateLogger->AddLog( pPt.second.first, pPt.first, pPts2.first, 1003, strTmp );
								continue;
							}

							// ���ΘH���̍ŋߖT�����N�ւ̋������擾
							IPointPtr point1 = (IPoint*)pPt.second.second;
							IPointPtr point2 = (IPoint*)itSecond->second[ pPts2.first ].find( pPt.first )->second.second;

							double dbDist1 = 0.0f;
							IProximityOperatorPtr ipProxy1 = (IPolyline*)mapRoadGeoms[ it->first ][ itFirst->first ][ pPts2.first ];
							if( FAILED( ipProxy1->ReturnDistance( point2, &dbDist1 ) ) )
							{
								_ASSERT( FALSE );
								continue;
							}

							double dbDist2 = 0.0f;
							IProximityOperatorPtr ipProxy2 = (IPolyline*)mapRoadGeoms[ it->first ][ itSecond->first ][ pPts2.first ];
							if( FAILED( ipProxy2->ReturnDistance( point1, &dbDist2 ) ) )
							{
								_ASSERT( FALSE );
								continue;
							}

							// �����W�|�C���g�Ԃ̋����擾
							IPolylinePtr ipDppPoly( CLSID_Polyline );
							ipDppPoly->putref_SpatialReference( distancePostSpRef );
							ipDppPoly->Project( ipPrjCoordinateSystem );	// ���W�n�ύX
							ipDppPoly->put_FromPoint( pPt.second.second );
							ipDppPoly->put_ToPoint( itSecond->second[ pPts2.first ].find( pPt.first )->second.second );

							double dbLen = 0.0f;
							ipDppPoly->get_Length( &dbLen );

							// ��r
							double dbPointDiff1 = ( abs( dbLen - dbDist1 ) / dbLen ) * 100;
							double dbPointDiff2 = ( abs( dbLen - dbDist2 ) / dbLen ) * 100;

							if( dbPointDiff1 >= dbSeparate || dbPointDiff2 >= dbSeparate )
							{
								CString strTmp;
								strTmp.Format( _T( "ERROR:����H���̓��ꋗ���W�|�C���g�ԋ������A�㉺���̍ŋߖT�����N��<%.1f>���ȏ㗣��Ă���B" ), dbSeparate );
								pSeparateLogger->AddLog( pPt.second.first, pPt.first, pPts2.first, 1004, strTmp );
							}
						}
					}
				}
				stdOut.writeConsole( _T( "����I��" ) );
			}
			// �S�w��H���̍�ƏI�����ԏo��
			CString endTime = stdOut.GetNow();
			stdOut.writeConsole( _T( "\nend " ) + endTime );
			stdOut.writeConsole( _T( "����I��" ) );

			return 0;
		}
		catch( const _com_error& e )
		{
			throw std::runtime_error( uh::toStr( atl2::stringize( e ) ) );
		}
	}
	catch( const std::exception& e )
	{
		cerr << e.what() << endl;
		cerr << "�ُ�I��" << endl;
		return 1;
	}
}