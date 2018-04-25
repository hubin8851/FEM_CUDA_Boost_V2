#pragma once
#include <cuda_runtime.h>
#include <cooperative_groups.h>
#include <HbxCudaDef\Points.h>
#include <libInterpolate\Quadtree2D.h>
#include <libInterpolate\QuadParam.h>

namespace HBXDef
{
	namespace cg = cooperative_groups;

//��̬�����Ĳ�����ʹ��CUDA��̬���С��㷨���£�
//��������һ���߳̿��ڵ���NUM_THREAD_PER_BLOCK���̣߳��������£�
//1. �������Ŀ�����
//2. ����ÿ���ӽڵ�Ľڵ���
//3. ����߳̽���Ա��֪�ڵ�����Ŀ
//4. �ƶ��ڵ�
//5. �����µ��߳̿�
	template< int NUM_THREADS_PER_BLOCK >
	__global__	Quadtree_kernel(Quadtree2D_node *nodes, Points2D *points, QuadParam params)
	{
		// �洢�ڵ���Ŀ�Ĺ����ڴ�ռ�
		extern __shared__ int smem[];

		// s_num_pts[4][NUM_WARPS_PER_BLOCK];
		volatile int *s_num_pts[4];

		for (int i = 0; i < 4; ++i)
			s_num_pts[i] = (volatile int *)&smem[i*NUM_WARPS_PER_BLOCK];

		// �����߳̿��ڸ��̵߳�����
		const int warp_id = threadIdx.x / warpSize;//�߳�����
		const int lane_id = threadIdx.x % warpSize;//�߳����ڱ��

		// Mask for compaction.
		int lane_mask_lt = (1 << lane_id) - 1; // Same as: asm( "mov.u32 %0, %%lanemask_lt;" : "=r"(lane_mask_lt) );

		//��ǰ�ڵ�
		Quadtree2D_node &node = nodes[blockIdx.x];
		node.set_id(node.id() + blockIdx.x);

		// ��ǰ�ڵ���ӵ�еĵ����Ŀ
		int num_points = node.num_points();

		float2 center;
		int range_begin, range_end;
		int warp_cnts[4] = { 0, 0, 0, 0 };

		//����1���������Ŀ�������


	}


}