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
		// KKD交通規制道路ノードレイヤデータを読み込む
		KKDRegulationRoadNodeLoad aNodeLoad;
		if(! aNodeLoad.load("d:\\kkd", 533945))
			return false;

		// 全てのノードでループする
		for(int i = 0; i < (int)aNodeLoad.size(); i++) {
			const KKDRegulationRoadNode& rNode = aNodeLoad[i];

			// ノードIDと、接続リンク数を出力する
			std::cout
				<< rNode.eId << ":"
				<< rNode.eConnectLinkCount << std::endl;

			// 接続するリンクでループ
			for(int j = 0; j < (int)rNode.eConnectLinks.size(); j++) {
				// 接続するリンクの接続ノード番号と交差点名称、規制数を出力する
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
