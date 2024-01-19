#pragma once

#include "GridBasedGraph.h"

namespace AI
{
	
	class BFS
	{
	public:
		bool Run(GridBasedGraph& graph, int startX, int startY, int endX, int endY);

		const NodeList& GetClosedList() const { return mClosedList; }
	private:
		NodeList mOpenList;
		NodeList mClosedList;
	};
}