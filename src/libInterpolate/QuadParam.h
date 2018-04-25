#pragma once
#include <cuda_runtime.h>


namespace HBXDef
{
	struct QuadParam 
	{
		// 选取右端集合作为输入/输出
		int point_selector;
		// 给定层级的节点数4^层级数
		int num_nodes_at_this_level;
		// 深度
		int depth;
		// 最大深度
		const int max_depth;
		// 停止递归的最小节点数
		const int min_points_per_node;

		// 默认构造函数
		__host__ __device__ QuadParam(int max_depth, int min_points_per_node) :
			point_selector(0),
			num_nodes_at_this_level(1),
			depth(0),
			max_depth(max_depth),
			min_points_per_node(min_points_per_node)
		{}

		// 拷贝构造函数
		__host__ __device__ QuadParam(const QuadParam &params, bool) :
			point_selector((params.point_selector + 1) % 2),
			num_nodes_at_this_level(4 * params.num_nodes_at_this_level),
			depth(params.depth + 1),
			max_depth(params.max_depth),
			min_points_per_node(params.min_points_per_node)
		{}

	};


}