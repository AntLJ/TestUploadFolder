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

#include <iostream>
#include <KKDLayerLoad.h>

int main()
{
	try {
		// KKD��ʋK�����H�m�[�h���C���f�[�^��ǂݍ���
		KKDRegulationRoadNodeLoad aNodeLoad;
		if(! aNodeLoad.load("d:\\kkd", 533945))
			return false;

		// �S�Ẵm�[�h�Ń��[�v����
		for(int i = 0; i < (int)aNodeLoad.size(); i++) {
			const KKDRegulationRoadNode& rNode = aNodeLoad[i];

			// �m�[�hID�ƁA�ڑ������N�����o�͂���
			std::cout
				<< rNode.eId << ":"
				<< rNode.eConnectLinkCount << std::endl;

			// �ڑ����郊���N�Ń��[�v
			for(int j = 0; j < (int)rNode.eConnectLinks.size(); j++) {
				// �ڑ����郊���N�̐ڑ��m�[�h�ԍ��ƌ����_���́A�K�������o�͂���
				std::cout
					<< "\t"
					<< rNode.eConnectLinks[j].eConnectNodeId << ":"
					<< rNode.eConnectLinks[j].eIntersectionName.eKanji
					<< "(" << rNode.eConnectLinks[j].eIntersectionName.eKana << "):"
					<< rNode.eConnectLinks[j].eRegulationCount << std::endl;
			}
		}
	}
    catch (std::exception& e) {
        std::cerr << e.what() << std::endl;
        return -1;
    }

    return 0;
}
