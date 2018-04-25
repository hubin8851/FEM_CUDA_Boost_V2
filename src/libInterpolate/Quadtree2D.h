#pragma once
#include <thrust/random.h>
#include <thrust/device_vector.h>
#include <libInterpolate\Bound2D.h>

namespace HBXDef
{
	class Boundary2D;

	class Quadtree2D_node
	{
	private:
		int m_id;

		//边界条件
		Boundary2D m_bound;

		//点的范围
		int m_begin, m_end;

	public:
		__host__ __device__ Quadtree2D_node() : m_id(0), m_begin(0), m_end(0) {};
		
		//
		__host__ __device__ int GetId() const
		{
			return m_id;
		};

		//
		__host__ __device__ void SetId(int _newId)
		{
			m_id = _newId;
		}

		//获取边界条件
		__host__ __device__ __forceinline__ void SetBound(float min_x, float min_y, float max_x, float max_y)
		{
			m_bound.Set(min_x, min_y, max_x, max_y);
		}

		// The number of points in the tree.
		__host__ __device__ __forceinline__ int GetPointNum() const
		{
			return m_end - m_begin;
		}

		// The range of points in the tree.
		__host__ __device__ __forceinline__ int GetBegin() const
		{
			return m_begin;
		}

		__host__ __device__ __forceinline__ int GetEnd() const
		{
			return m_end;
		}

		// Define the range for that node.
		__host__ __device__ __forceinline__ void SetRange(int begin, int end)
		{
			m_begin = begin;
			m_end = end;
		}

	};




}