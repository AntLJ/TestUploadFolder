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
#include "Output.h"

#include <strcode/INCLUDE/SJIS2EUC.H>

Output::~Output()
{
	if( m_ofs.is_open() )
		m_ofs.close();
}

void Output::Open( const uh::tstring& outputFilePath )
{
	m_ofs.imbue( std::locale("") );
	m_ofs.open(outputFilePath, std::ios::out | std::ios::binary);
}

bool Output::IsOpen()
{
	return m_ofs.is_open();
}

void Output::POIPointOutput( const POIPointRecordSet& recordSet )
{
	POIPointRecordSetItr& it = recordSet.begin();
	while( it != recordSet.end() )
	{
		// �d�b�ԍ��Ɩ��̃Z�b�g�Ō��āA�d�����Ă���ꍇ�Ƃ����łȂ��ꍇ�ŏꍇ����
		CString strData;
		CString buildingID = _T("");
		int nCount = recordSet.count( *it );
		if( nCount == 1 )
		{
			if( it->m_buildingID > 0 )
				buildingID.Format( _T("%ld"), it->m_buildingID );

			strData.Format( _T("%ld\t%06d\t%d\t%d\t%ld\t%ld\t%s\t%d\t%s\t%s\t%s\t%s\t%d\t%s"),
				it->m_lOID,
				it->m_nMeshCode, it->m_nX, it->m_nY,								// 2�����b�V��XY
				it->m_lGrpCode, it->m_lChainCode, it->m_strSeq, it->m_deleteCode,	// ���ށE�X�܁E�V�[�P���X�E�폜�R�[�h
				it->m_strTel, it->m_strName, it->m_strAddrCode1, it->m_strAddrCode2,// �d�b�ԍ��E���́E�Z���R�[�h
				it->m_nAddXYC, buildingID );										// ��ʃR�[�h�E�ƌ`ID
			++it;
		}
		else
		{
			std::pair<POIPointRecordSetItr, POIPointRecordSetItr>& itPair = recordSet.equal_range( *it );
			POIPointRecord cBasePOI( *it );

			do
			{
				// ��ʂɂ��D��x(�d�v�R���e���c > �ʒu�i������ > �s���|�C���g����)
				// ���x�ɂ��D��x( 1S > 3A > 1A > UK )
				int nBasePrior = cBasePOI.GetPriorValueFromAcc();
				int nCompPrior = itPair.first->GetPriorValueFromAcc();

				if( (cBasePOI.m_nAddXYC > itPair.first->m_nAddXYC)
					|| (cBasePOI.m_nAddXYC == itPair.first->m_nAddXYC && nBasePrior > nCompPrior)
					|| (cBasePOI.m_nAddXYC == itPair.first->m_nAddXYC && nBasePrior == nCompPrior && cBasePOI.m_lOID > itPair.first->m_lOID) )
				{
					cBasePOI = *itPair.first;
				}
				++itPair.first;
			}
			while( itPair.first != itPair.second );

			if( cBasePOI.m_buildingID > 0 )
				buildingID.Format( _T("%d"), cBasePOI.m_buildingID );

			strData.Format( _T("%ld\t%06d\t%d\t%d\t%ld\t%ld\t%s\t%d\t%s\t%s\t%s\t%s\t%d\t%s"),
				cBasePOI.m_lOID,
				cBasePOI.m_nMeshCode, cBasePOI.m_nX, cBasePOI.m_nY,										// 2�����b�V��XY
				cBasePOI.m_lGrpCode, cBasePOI.m_lChainCode, cBasePOI.m_strSeq, cBasePOI.m_deleteCode,	// ���ށE�X�܁E�V�[�P���X�E�폜�R�[�h
				cBasePOI.m_strTel, cBasePOI.m_strName, cBasePOI.m_strAddrCode1, cBasePOI.m_strAddrCode2,// �d�b�ԍ��E���́E�Z���R�[�h
				cBasePOI.m_nAddXYC, buildingID );														// ��ʃR�[�h�E�ƌ`ID

			advance( it, nCount );
		}

		// EUC�ɕϊ�
		char szConvData[1024] = "";
		SJIStoEUCString( CT2CA(strData), szConvData, sizeof(szConvData) );
		
		m_ofs << szConvData << "\n";
		m_ofs.flush();
	}
}

void Output::POISubPointOutput( const POISubPointRecordSet& recordSet )
{
	for( const auto& data : recordSet )
	{
		CString strData;
		strData.Format( _T("%ld\t%s\t%06d\t%ld\t%ld\t%s\t%d\t%s\t%d"), 
			data.m_poiPointId, // POI_POINT_ID
			data.m_funcSeq, // FuncSeq
			data.m_meshCode, // 2�����b�V���R�[�h
			data.m_x, // X
			data.m_y, // Y
			data.m_pstAttCode, // ���W�l��ʃR�[�h
			data.m_priority,// ���W�l��ʓ��D�揇��
			data.m_priorityAttCode, // ���W�l��ʓ��D�揇�ʎ�ʃR�[�h
			data.m_tollRoadFlag ); // �L�����H�t���O

		// EUC�ɕϊ�
		char szConvData[1024] = "";
		SJIStoEUCString( CT2CA(strData), szConvData, sizeof(szConvData) );

		m_ofs << szConvData << "\n";
		m_ofs.flush();
	}
}
