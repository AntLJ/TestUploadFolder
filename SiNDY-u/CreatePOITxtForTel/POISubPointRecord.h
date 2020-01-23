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

#pragma once
class POISubPointRecord
{
public:
	POISubPointRecord() : m_poiPointId(0), m_meshCode(0), m_x(0), m_y(0), m_priority(0), m_tollRoadFlag(0){}

	bool operator<( const POISubPointRecord& ob ) const
	{
		if( m_poiPointId == ob.m_poiPointId )
		{
			return _ttol(m_funcSeq) < _ttol(ob.m_funcSeq);
		}
		else
		{
			return m_poiPointId < ob.m_poiPointId;
		}
	}

	long m_poiPointId; //!< POI_POINT_ID
	CString m_funcSeq; //!< �@�\���V�[�P���X
	int m_meshCode; //!< 2�����b�V���R�[�h
	int m_x; //!< MeshX
	int m_y; //!< MeshY
	CString m_pstAttCode; //!< ���W�l��ʃR�[�h
	int m_priority; //!< ���W�l��ʓ��D�揇�ʃR�[�h
	CString m_priorityAttCode; //!< ���W�l��ʓ��D�揇�ʎ�ʃR�[�h
	int m_tollRoadFlag; //!< �L�����H�t���O
};

