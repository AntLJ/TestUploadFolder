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

namespace sindy {
namespace adam2 {
namespace index {

namespace feature {
	static const size_t  LON        = 0;
	static const size_t  LAT        = 1;
	static const size_t  HEIGHT     = 2;
	static const size_t  OBJECTID   = 3;
}

namespace from_to_node {
	static const size_t  FROM_NODE_ID   = 4;
	static const size_t  TO_NODE_ID     = 5;
}

namespace lane_link {
	using namespace feature;
	using namespace from_to_node;

	static const size_t  LANE_GROUP_ID  = 6;
	static const size_t  LANE_GROUP_C   = 7;
	static const size_t  LANE_NO        = 8;
	static const size_t  LANE_COUNT     = 9;

}

}
}
}

