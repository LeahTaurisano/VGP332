#pragma once

#include <XEngine.h>
#include <AI.h>

using Path = std::vector<X::Math::Vector2>;

struct Tile
{
	X::TextureId textureId;
	bool isWalkable = false;
};

class TileMap
{
public:
	void LoadTiles(const char* fileName);
	void LoadMap(const char* fileName);

	void Render() const;

	bool IsBlocked(int x, int y) const;
	int GetColumns() const { return mColumns; }
	int GetRows() const { return mRows; }
	void SetShowDebug(bool state) { showDebug = state; }

	X::Math::Vector2 GetTilePosition(const X::Math::Vector2& pos) const;
	X::Math::Vector2 GetPixelPosition(int x, int y) const;

	Path FindPathBFS(int startX, int startY, int endX, int endY);
	Path FindPathDFS(int startX, int startY, int endX, int endY);
	Path FindPathDijkstra(int startX, int startY, int endX, int endY);
	Path FindPathAStar(int startX, int startY, int endX, int endY);


private:
	AI::GridBasedGraph mGraph;
	std::vector<int> mMap;
	std::vector<Tile> mTiles;
	int mColumns = 0;
	int mRows = 0;
	int mTileWidth = 0;
	int mTileHeight = 0;

	bool showDebug;
};