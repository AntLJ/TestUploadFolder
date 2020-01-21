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
		// KKDデータを読み込む
		KKDLoad aKkdLoad;
		if(! aKkdLoad.load("d:\\kkd", 533945))
			return false;

		// 交通規制道路ノードレイヤを取得する
		const KKDRegulationRoadNodeLayer& rLayer= aKkdLoad.regulationRoadNodeLayer();

		// 全てのノードでループする
		for(int i = 0; i < (int)rLayer.size(); i++) {
			const KKDRegulationRoadNode& rNode = rLayer[i];

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
