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
#include <KKDLoad.h>

int main()
{
	try {
		// KKD�f�[�^��ǂݍ���
		KKDLoad aKkdLoad;
		if(! aKkdLoad.load("d:\\kkd", 533945))
			return false;

		// ��ʋK�����H�m�[�h���C�����擾����
		const KKDRegulationRoadNodeLayer& rLayer= aKkdLoad.regulationRoadNodeLayer();

		// �S�Ẵm�[�h�Ń��[�v����
		for(int i = 0; i < (int)rLayer.size(); i++) {
			const KKDRegulationRoadNode& rNode = rLayer[i];

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
