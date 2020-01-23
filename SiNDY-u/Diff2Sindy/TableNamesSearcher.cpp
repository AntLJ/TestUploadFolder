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
#include "TableNamesSearcher.h"

TableNamesSearcher::TableNamesSearcher()
{
}

TableNamesSearcher::~TableNamesSearcher()
{
}


// �S�f�[�^�Z�b�g���̃e�[�u�������ċA�I�Ɏ擾
void TableNamesSearcher::getWorkspaceTableNames( IEnumDatasetName* ipDatasetNames, std::vector<uh::tstring>& cTableNames )
{
	if( !ipDatasetNames )
	{
		return;
	}
	IDatasetNamePtr ipDatasetName;
	ipDatasetNames->Reset();

	while( S_OK == ipDatasetNames->Next( &ipDatasetName ) )
	{
		// �f�[�^�Z�b�g��������ċA����
		esriDatasetType emType = esriDTAny;
		ipDatasetName->get_Type( &emType );
		if( isDataset(emType))
		{
			IEnumDatasetNamePtr ipSubDatasetNames;
			ipDatasetName->get_SubsetNames( &ipSubDatasetNames );
			getWorkspaceTableNames( ipSubDatasetNames, cTableNames );
			continue;
		}

		// �e�[�u�������i�[
		CComBSTR bstrName;
		ipDatasetName->get_Name( &bstrName );
		cTableNames.push_back( bstrName.Copy() );
	}
}

// �f�[�^�Z�b�g���ǂ�������
bool TableNamesSearcher::isDataset(esriDatasetType type)
{
	if(
		type == esriDTFeatureDataset || // Feature Dataset
		type == esriDTRasterDataset  || // Raster Dataset
		type == esriDTNetworkDataset || // Network Dataset
		type == esriDTMosaicDataset  || // Mosaic Dataset
		type == esriDTLasDataset        // Las Dataset
		)
	{
		return true;
	}
	return false ;
}
