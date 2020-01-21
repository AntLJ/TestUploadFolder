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
#include <exception>

#include <DRMALayerLoad.h>

int main()
{
	try {
		DRMABaseRoadNodeLoad aLoad;

		// DRMA��{���H�m�[�h�f�[�^��ǂݍ���
		if(! aLoad.load("d:\\drma", 533945))
			return false;

		// �S�Ă̊�{���H�m�[�h�Ń��[�v����
		for(int i = 0; i < (int)aLoad.size(); ++i) {
			const DRMABaseRoadNode& rRoadNode = aLoad[i];
			// �����_���̓ǂ݂����m�[�h��S�ďo�͂���
			if(rRoadNode.eName.eKanaLength) {
				std::cout
					<< "0x" << std::hex << rRoadNode.eId
					<< ":"
					<< rRoadNode.eName.eKanji
					<< "(" << rRoadNode.eName.eKana << ")" 
					<< std::endl;
			}
		}
	}
	catch (std::exception& e) {
		std::cerr << e.what() << std::endl;
		return -1;
	}

	return 0;
}
