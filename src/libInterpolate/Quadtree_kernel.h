#pragma once
#include <cuda_runtime.h>
#include <cooperative_groups.h>
#include <HbxCudaDef\Points.h>
#include <libInterpolate\Quadtree2D.h>
#include <libInterpolate\QuadParam.h>

namespace HBXDef
{
	namespace cg = cooperative_groups;

//动态构建四叉树，使用CUDA动态并行。算法如下：
//主机端在一个线程块内调用NUM_THREAD_PER_BLOCK个线程，步骤如下：
//1. 检查点的数目和深度
//2. 计算每个子节点的节点数
//3. 浏览线程结果以便得知节点总数目
//4. 移动节点
//5. 开启新的线程块
	template< int NUM_THREADS_PER_BLOCK >
	__global__	Quadtree_kernel(Quadtree2D_node *nodes, Points2D *points, QuadParam params)
	{
		// 存储节点数目的共享内存空间
		extern __shared__ int smem[];

		// s_num_pts[4][NUM_WARPS_PER_BLOCK];
		volatile int *s_num_pts[4];

		for (int i = 0; i < 4; ++i)
			s_num_pts[i] = (volatile int *)&smem[i*NUM_WARPS_PER_BLOCK];

		// 计算线程块内该线程的坐标
		const int warp_id = threadIdx.x / warpSize;//线程束号
		const int lane_id = threadIdx.x % warpSize;//线程束内编号

		// Mask for compaction.
		int lane_mask_lt = (1 << lane_id) - 1; // Same as: asm( "mov.u32 %0, %%lanemask_lt;" : "=r"(lane_mask_lt) );

		//当前节点
		Quadtree2D_node &node = nodes[blockIdx.x];
		node.set_id(node.id() + blockIdx.x);

		// 当前节点所拥有的点的数目
		int num_points = node.num_points();

		float2 center;
		int range_begin, range_end;
		int warp_cnts[4] = { 0, 0, 0, 0 };

		//步骤1，检查点的数目和其深度


	}


}