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
#include "CreateDistancePost.h"
#include "CCreateDistancePost.h"
#include "Arguments.h"
#include <TDC/useful_headers/str_util.h>
#include <TDC/useful_headers/directory_util.h>
#include "CFeatureClass.h"
#include "CStdOutput.h"
#include "CLogger.h"
#include <ArcHelperEx/AheGeometryOp.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

using namespace std;
using namespace ATL;

int _tmain(int argc, _TCHAR* argv[])
{
	_CrtSetDbgFlag( _CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF );

	try
	{
		const arctl::coinitializer aCoInitializer;

		// ���P�[���֘A�̕s��h�~
		uh::setJapaneseCharacterTypeFacet();

		try
		{
			// �W���o�͊Ǘ��N���X�I�u�W�F�N�g
			CStdOutput stdOut;

			// �R�}���h���C����������
			sindy::CArguments args;
			if(! args.parse(argc, argv))
			{
				stdOut.writeErrMsg(_T("�R�}���h���C�������ɖ�肪����܂�"));
				return 1;
			}

			// �W���o�̓w�b�_�[�o��
			CString stdOutHeader = stdOut.CreateLogHeader(args.m_strOpParam);
			cout << CT2A(stdOutHeader) << endl;

			// �ڑ�
			sindy::CCreateDistancePost cCreateDistancePost;

			if ( ! cCreateDistancePost.Connect(args.m_strDb.c_str()) )
			{
				stdOut.writeErrMsg(_T("ERROR:�w��f�[�^�x�[�X�ɐڑ��ł��܂���B"));
				return 1;
			}

			// �R�X�g�t�@�C���ǂݍ���
			cCreateDistancePost.ReadRoadCost(args.m_strRoadCost.c_str());
			cCreateDistancePost.ReadLinkCost(args.m_strLinkCost.c_str());

			// DistancePostPoint�t�B�[�`���N���X�擾
			if ( ! cCreateDistancePost.OpenDistancePostPoint() )
			{
				stdOut.writeErrMsg(_T("ERROR:�w��f�[�^�x�[�X��[DISTANCE_POST_POINT]�͂���܂���B"));
				return 1;
			}

			// ROAD_LINK/ROAD_NODE�t�B�[�`���N���X�擾
			if ( ! cCreateDistancePost.OpenNetwork() )
			{
				stdOut.writeErrMsg(_T("ERROR:�w��f�[�^�x�[�X��[ROAD_LINK/ROAD_NODE]�͂���܂���B"));
				return 1;
			}

			// ��ƊJ�n���ԏo��
			CString startTime = stdOut.GetNow();
			stdOut.writeConsole(_T("\nstart ") + startTime + _T("\n"));

			double dbPitch = _tcstod(args.m_strPitch.c_str(), nullptr);	// �s�b�`
			double dbBuff = _tcstod(args.m_strBuf.c_str(), nullptr);	// �o�b�t�@

			// �����s�b�`����␳�_�Ԃ̋������擾
			double correctionPointLen = (dbPitch) * 10;

			// �H�����X�g�`�F�b�N
			std::map<long,long> mapErr;
			std::map<long, std::pair<std::list<double>, std::map<double, long>>> mapContainer;
			long nRet = cCreateDistancePost.CheckRoadList(args.m_strList.c_str(), dbPitch, mapContainer, mapErr);
			if ( 0 != nRet )
			{
				if ( -6 == nRet )
				{
					for ( const auto& item : mapErr )
					{
						if ( item.second == 0 )	// ����
							continue;

						boost::tuple<long,long,CString> pair = cCreateDistancePost.GetRoadList(item.first);
						CString strTmp;
						strTmp.Format(_T("ERROR:<%ld><%ld><%s>�̋����W�|�C���g��2�����ł��B"), pair.get<0>(), pair.get<1>(), pair.get<2>() );
						stdOut.writeErrMsg(strTmp);
					}
				}
				else if ( -7 == nRet )
				{
					for ( const auto& item : mapErr )
					{
						if ( item.second == 0 )	// ����
							continue;

						boost::tuple<long,long,CString> pair = cCreateDistancePost.GetRoadList(item.first);
						CString strTmp;
						strTmp.Format(_T("ERROR:<%ld><%ld><%s>�̍ő勗���W�l�ƍŏ������W�l�̍���<%.1f>�ȉ��ł��B"), pair.get<0>(), pair.get<1>(), pair.get<2>(), dbPitch );
						stdOut.writeErrMsg(strTmp);
					}
				}
				else
				{
					for ( const auto& item : mapErr )
					{
						if ( item.second == 0 )	// ����
							continue;

						CString strTmp;
						strTmp.Format(_T("ERROR:�H�����X�g<%ld �s��>���������w�肳��Ă��܂���B"), item.first );
						stdOut.writeErrMsg(strTmp);
					}
				}

				return 1;
			}

			long nMIdx = cCreateDistancePost.GetFieldIndex( sindy::schema::distance_post_point::kDistanceMark );

			// ���W�n�ύX
			ISpatialReferenceFactory2Ptr ipSpRefFct(CLSID_SpatialReferenceEnvironment);
			IProjectedCoordinateSystemPtr ipPrjCoordinateSystem;
			ipSpRefFct->CreateProjectedCoordinateSystem(esriSRProjCS_TokyoJapan10, &ipPrjCoordinateSystem);

			// �H�����X�g���ɏ���
			for ( const auto& cRoadList : mapContainer )
			{
				boost::tuple<long,long,CString> pair = cCreateDistancePost.GetRoadList(cRoadList.first);

				// �w�肳�ꂽ�H�����Ƃ̍�ƊJ�n���Ԃ��R���\�[���ɏo��
				CString roadInfoMsg;
				roadInfoMsg.Format(_T("%ld\t%ld\t%s"), pair.get<0>(), pair.get<1>(), pair.get<2>());
				stdOut.writeRoadInfoMsg(roadInfoMsg);

				// ���s���O�t�@�C���Ǘ��N���X�I�u�W�F�N�g
				sindy::CSPLogger pLogger;
				pLogger.reset( new sindy::CLogger() );
				pLogger->initLog(args.m_strLogPath.c_str(), pair, args.m_strOpParam);

				// �����W�|�C���g����
				cCreateDistancePost.searchDistancePoint(cRoadList.second.second);

				std::map<double, sindy::CSPRowBase> mapRows = cCreateDistancePost.findDistancePoints();

				// �H���`��i�[�p
				IPolylinePtr ipAllPolyline;
				
				// �������鋗���W�|�C���g�i�[�p
				std::map<double, IPointPtr> mapPoints;

				// �G���[���b�Z�[�W�i�[�p
				CString errMsg;

				// �������鋗���W�|�C���g���擾
				if ( !cCreateDistancePost.getCreateDistancePost(cRoadList.second.first, mapRows, 
					ipPrjCoordinateSystem, ipAllPolyline, mapPoints, dbPitch, dbBuff, correctionPointLen, errMsg) )
				{
					stdOut.writeErrMsg(errMsg);
					continue;
				}

				if ( ipAllPolyline )
				{
					// �o�H�T�����H�`��̋����擾
					double dbLen = 0.0f;
					ipAllPolyline->get_Length(&dbLen);

					if ( dbLen <= dbPitch )
					{
						CString strTmp;
						strTmp.Format(_T("ERROR:<%ld><%ld><%s>�̏����Ώۓ��H�����N������<%.1lf>��<%.1lf>�ȉ��ł��B"), pair.get<0>(), pair.get<1>(), pair.get<2>(), dbLen, dbPitch );
						stdOut.writeErrMsg(strTmp);
						continue;
					}

					cCreateDistancePost.startEdit();

					// ���������W�l������΁A���̃|�C���g�͍쐬���Ȃ�
					long nEditCnt = 0;
					bool bEdit = 0;
					for ( const auto& pPt : mapPoints )
					{
						CString strTmp( cCreateDistancePost.convertStr(cCreateDistancePost.convert( pPt.first, esriMeters, esriKilometers )) );
						// �����W�l�Ō���
						// ���������W�l�̃|�C���g�͍쐬���Ȃ�
						sindy::CSPRowBase pRow = mapRows[_tcstod(strTmp, nullptr)];

						if ( ! pRow )
						{
							// �쐬
							pRow = cCreateDistancePost.createDistancePoint(pPt.second, _tcstod(strTmp, nullptr), cRoadList.first );

							CComVariant vaVal( pRow->GetValue(nMIdx) );

							pLogger->Log(pRow->GetOID(), CString(vaVal.bstrVal), _tcstod(strTmp, nullptr));

							++nEditCnt;
							bEdit = true;
						}

						if ( nEditCnt >= 1000 )
						{
							cCreateDistancePost.stopEdit();
							cCreateDistancePost.startEdit();
							nEditCnt = 0;
						}
					}

					if (bEdit)
						cCreateDistancePost.stopEdit();
					else
						cCreateDistancePost.abortEdit();

					stdOut.writeConsole(_T("����I��"));
				}
			}
			// �S�w��H���̍�ƏI�����ԏo��
			CString endTime = stdOut.GetNow();
			stdOut.writeConsole(_T("\nend ") + endTime);
			stdOut.writeConsole(_T("����I��"));
			return 0;
		}
		catch(const _com_error& e)
		{
			throw std::runtime_error(uh::toStr(atl2::stringize(e)));
		}
	}
	catch(const std::exception& e)
	{
		cerr << e.what() << endl;
		cerr << "�ُ�I��" << endl;
		return 1;
	}
}

