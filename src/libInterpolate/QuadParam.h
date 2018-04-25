#pragma once
#include <cuda_runtime.h>


namespace HBXDef
{
	struct QuadParam 
	{
		// ѡȡ�Ҷ˼�����Ϊ����/���
		int point_selector;
		// �����㼶�Ľڵ���4^�㼶��
		int num_nodes_at_this_level;
		// ���
		int depth;
		// ������
		const int max_depth;
		// ֹͣ�ݹ����С�ڵ���
		const int min_points_per_node;

		// Ĭ�Ϲ��캯��
		__host__ __device__ QuadParam(int max_depth, int min_points_per_node) :
			point_selector(0),
			num_nodes_at_this_level(1),
			depth(0),
			max_depth(max_depth),
			min_points_per_node(min_points_per_node)
		{}

		// �������캯��
		__host__ __device__ QuadParam(const QuadParam &params, bool) :
			point_selector((params.point_selector + 1) % 2),
			num_nodes_at_this_level(4 * params.num_nodes_at_this_level),
			depth(params.depth + 1),
			max_depth(params.max_depth),
			min_points_per_node(params.min_points_per_node)
		{}

	};


}