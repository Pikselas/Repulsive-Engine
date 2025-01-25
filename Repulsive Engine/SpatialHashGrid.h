#pragma once
#include <string>
#include <unordered_set>
#include <unordered_map>

struct Box
{
	int left;
	int right;
	int top;
	int bottom;
	bool operator== (const Box& other) const
	{
		return left == other.left && right == other.right && top == other.top && bottom == other.bottom;
	}
};

struct BoxHash
{
	std::size_t operator()(const Box& b) const 
	{
		return std::hash<int>()(b.left) ^ std::hash<int>()(b.right) ^ std::hash<int>()(b.top) ^ std::hash<int>()(b.bottom);
	}
};

class SpatialHashGrid
{
private:
	unsigned int cell_side;
	std::unordered_map<std::string, std::unordered_set<Box , BoxHash>> cells;
public:
	SpatialHashGrid(unsigned int cell_side) : cell_side(cell_side)
	{}
private:
	std::pair<int, int> getCellIndex(int x, int y) const
	{
		return { x / cell_side , y / cell_side};
	}
	std::string getKey(int x, int y) const
	{
		return std::to_string(x) + "." + std::to_string(y);
	}
public:
	void Insert(Box box)
	{
		auto [x1, y1] = getCellIndex(box.left , box.top);
		auto [x2, y2] = getCellIndex(box.right, box.bottom);

		for (int x = x1; x <= x2; ++x)
		{
			for (int y = y1; y <= y2; ++y)
			{
				auto k = getKey(x, y);
				cells[k].insert(box);
			}
		}
	}
	void Remove(Box box)
	{
		auto [x1, y1] = getCellIndex(box.left, box.top);
		auto [x2, y2] = getCellIndex(box.right, box.bottom);

		for (int x = x1; x <= x2; ++x)
		{
			for (int y = y1; y <= y2; ++y)
			{
				auto k = getKey(x, y);
				cells[k].erase(box);
			}
		}
	}
	std::vector<Box> FindNear(Box box) const
	{
		std::vector <Box> boxes;

		auto [x1, y1] = getCellIndex(box.left, box.top);
		auto [x2, y2] = getCellIndex(box.right, box.bottom);

		for (int x = x1; x <= x2; ++x)
		{
			for (int y = y1; y <= y2; ++y)
			{
				auto k = getKey(x, y);
				if (cells.contains(k))
				{
					for (auto b : cells.at(k))
					{
						boxes.push_back(b);
					}
				}
			}
		}
		return boxes;
	}
};