#pragma once
#include <cuda_runtime.h>

namespace HBXDef
{
	//二维的边界类，用以判断该点是否在该边界范围内
	//在bound3D中，内含float3
	class Boundary2D
	{
		float2 m_min;
		float2 m_max;
	public:
		__host__ __device__ Boundary2D()
		{
			m_min = make_float2(0.0f, 0.0f);
			m_max = make_float2(1.0f, 1.0f);
		}

		__host__ __device__ void comput_center(float2& center) const
		{
			center.x = 0.5f *(m_min.x + m_max.x);
			center.y = 0.5f *(m_min.y + m_max.y);
		}

		__host__ __device__ __forceinline__ const float2& GetMax() const
		{
			return m_max;
		}

		__host__ __device__ __forceinline__ const float2& GetMin() const
		{
			return m_min;
		}

		//判断是否在边界内
		__host__ __device__ bool ifContains(const float2 &p) const
		{
			return p.x >= m_min.x && p.x < m_max.x && p.y >= m_min.y && p.y < m_max.y;
		}

		// Define the bounding box.
		__host__ __device__ void Set(float min_x, float min_y, float max_x, float max_y)
		{
			m_min.x = min_x;
			m_min.y = min_y;
			m_max.x = max_x;
			m_max.y = max_y;
		}


	};



}
