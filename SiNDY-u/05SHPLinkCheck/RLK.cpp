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
#include "RLK.h"
#include <atl2/io.h>

CRLK::CRLK(IFeatureClassPtr ipFeatureClass)
{
    m_ipFeatureClass = ipFeatureClass;

}

CRLK::~CRLK(void)
{
}

bool CRLK::GetInfo(std::vector<LINK> & rLinkSet, std::wstring rLinkFileName, bool bWalkOpt)
{
	HRESULT hRetObjct, hRetFrom, hRetTo;

	//OBJECTID̨���ށAFROMNODEID̨���ށATONODEID̨���ނ̲��ޯ���擾
	long lObjectIDIndex, lFromNodeIDIndex, lToNodeIDIndex;
	hRetObjct = m_ipFeatureClass->FindField(_T("OBJECTID"), &lObjectIDIndex);
	hRetFrom = m_ipFeatureClass->FindField(_T("FROMNODEID"), &lFromNodeIDIndex);
	hRetTo = m_ipFeatureClass->FindField(_T("ToNodeID"), &lToNodeIDIndex);
	if(FAILED(hRetObjct) || (-1 == lObjectIDIndex) ||
	   FAILED(hRetFrom) || (-1 == lFromNodeIDIndex) ||
	   FAILED(hRetTo) || (-1 == lToNodeIDIndex)){
		std::cerr << rLinkFileName << ":�t�B�[���h�C���f�b�N�X�̎擾�Ɏ��s���܂���"<<std::endl;
		return false;
	}

	//FROMDIVID̨���ށATODIVID̨���ނ̲��ޯ���擾
	long lFromDivIDIndex, lToDivIDIndex;
	if(bWalkOpt){
		hRetFrom = m_ipFeatureClass->FindField(_T("FROMDIVID"), &lFromDivIDIndex);
		hRetTo = m_ipFeatureClass->FindField(_T("TODIVID"), &lToDivIDIndex);
		if(FAILED(hRetFrom) || (-1 == lFromDivIDIndex) ||
		   FAILED(hRetTo) || (-1 == lToDivIDIndex)){
			std::cerr << rLinkFileName << ":�t�B�[���h�C���f�b�N�X�̎擾�Ɏ��s���܂���"<<std::endl;
			return false;
		}
	}

	//̨�������ق̎擾
	IFeatureCursorPtr ipFeatureCursor;
	if( FAILED(m_ipFeatureClass->Search(NULL,VARIANT_FALSE, &ipFeatureCursor))){

		std::cerr << rLinkFileName <<":̨�������ق̎擾�Ɏ��s���܂����B"<<std::endl;
		return false;
	}

	//̨�������ق��s����܂�̨����������
	IFeaturePtr ipFeature;
	while (ipFeatureCursor->NextFeature(&ipFeature)==S_OK && (ipFeature!=NULL)){
		long lOID = ipFeature->GetOID();
		LINK LinkInfo;

		LinkInfo.FID = lOID;

		CComVariant vaObjectID,vaFromNodeID,vaToNodeID;
		//OBJECTID̨���ނ̒l���擾
		if(FAILED(ipFeature->get_Value(lObjectIDIndex, &vaObjectID))){
			std::cerr<< rLinkFileName << "\tFID:" << lOID <<"\tOBJECTID�̎擾�Ɏ��s"<<std::endl;
			continue;
		}
		else{
			LinkInfo.ObjectID = vaObjectID.lVal;
		}
		//FROMNODEID̨���ނ̒l�̎擾
		if(FAILED(ipFeature->get_Value(lFromNodeIDIndex, &vaFromNodeID))){
			std::cerr<< rLinkFileName << "\tFID:" << lOID<<"\tFROMNODEID�̎擾�Ɏ��s"<<std::endl;
			continue;
		}
		else{
			LinkInfo.FromNodeID = vaFromNodeID.lVal;
		}
		//TONODEID̨���ނ̒l�̎擾
		if(FAILED(ipFeature->get_Value(lToNodeIDIndex, &vaToNodeID))){
			std::cerr<< rLinkFileName << "\tFID:" << lOID << "\tTONODEID�̎擾�Ɏ��s"<<std::endl;
			continue;
		}
		else{
			LinkInfo.ToNodeID = vaToNodeID.lVal;
		}

		LinkInfo.FromDivID = -1;		//�����l�ݒ�(RLK�̏ꍇ��-1�Œ�Ƃ���)
		LinkInfo.ToDivID = -1;			//�����l�ݒ�(RLK�̏ꍇ��-1�Œ�Ƃ���)
		if(bWalkOpt){
			CComVariant vaFromDivID,vaToDivID;
			//FROMDIVID̨���ނ̒l�̎擾
			if(FAILED(ipFeature->get_Value(lFromDivIDIndex, &vaFromDivID))){
				std::cerr<< rLinkFileName << "\tFID:" << lOID<<"\tFROMDIVID�̎擾�Ɏ��s"<<std::endl;
				continue;
			}
			else{
				LinkInfo.FromDivID = vaFromDivID.lVal;
			}
			//TODIVID̨���ނ̒l�̎擾
			if(FAILED(ipFeature->get_Value(lToDivIDIndex, &vaToDivID))){
				std::cerr<< rLinkFileName << "\tFID:" << lOID << "\tTODIVID�̎擾�Ɏ��s"<<std::endl;
				continue;
			}
			else{
				LinkInfo.ToDivID = vaToDivID.lVal;
			}
		}

		//�`����擾
		IGeometryPtr ipGeometry;
		if(FAILED(ipFeature->get_ShapeCopy(&ipGeometry))){
			std::cerr<< rLinkFileName << "\tFID:" << lOID<<"\t�`��̎擾�Ɏ��s"<<std::endl;
			continue;
		}
		VARIANT_BOOL check;
		ipGeometry->get_IsEmpty(&check);
		if(check){
			std::cout << "error\t" << rLinkFileName << "\tFID:" << lOID <<"\t�`�󂪑��݂��܂���B" << std::endl;
			continue;
		}

		//�`���_��ɕϊ�
		IPointCollectionPtr ipPointCollection(ipGeometry);
		long lPointCount;
		if(FAILED(ipPointCollection->get_PointCount(& lPointCount))){
			std::cerr << rLinkFileName << "\tFID:" << lOID <<"\t�\���_���擾�Ɏ��s"<<std::endl;
			continue;
		}

		//�\���_����2�����ł����
		if(lPointCount < 2){
			std::cout << "error" << rLinkFileName << "\tFID:" << lOID << "\t�\���_����2�����ł�."<<std::endl;
			continue;
		}

		//�n�_���W�擾
		IPointPtr ipPoint;
		if(FAILED(ipPointCollection->get_Point(0,&ipPoint)
			||ipPoint->get_X(&LinkInfo.FirstPoint.X)
			||ipPoint->get_Y(&LinkInfo.FirstPoint.Y))){
			std::cerr << rLinkFileName << "\tFID:" << lOID << "\t�n�_�̍��W�擾�Ɏ��s"<<std::endl;
			continue;
		}

		//�I�_���W�擾
		if(FAILED(ipPointCollection->get_Point(lPointCount-1,&ipPoint)
			||ipPoint->get_X(&LinkInfo.EndPoint.X)
			||ipPoint->get_Y(&LinkInfo.EndPoint.Y))){
				std::cerr <<  rLinkFileName << "\tFID:" << lOID <<"\t�I�_�̍��W�擾�Ɏ��s"<<std::endl;
				continue;
		}

		//LinkInfo��vector�R���e�i�Ɋi�[
		rLinkSet.push_back(LinkInfo);
	}
	return true;
  }


