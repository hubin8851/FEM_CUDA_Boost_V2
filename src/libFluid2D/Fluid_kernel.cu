#include <CudaPreDef.h>
#include <cufft.h>
#include <helper_gl.h>
#include "..\libFluid2D\Fluid_kernel.cuh"

// Texture reference for reading velocity field
texture<float2, 2> g_texVeloc;//纹理内存在做插值计算时只支持float型

__global__ void addForces_k(HBXDef::HbxCuDef::UserCalPrec2 * v, int dx, int dy, int spx, int spy, float fx, float fy, int r, size_t pitch)
{
	int idx = threadIdx.x;
	int idy = threadIdx.y;

	//二维索引当前线程对应的速度域
	HBXDef::HbxCuDef::UserCalPrec2* fj = (HBXDef::HbxCuDef::UserCalPrec2*)((char*)v + (idy + spy)*pitch) + idx + spx;

	HBXDef::HbxCuDef::UserCalPrec2 tmpV = *fj;
	idx -= r;
	idy -= r;

	HBXDef::UserCalPrec s = 1.f / (1.f + idx * idx*idx*idx + idy * idy*idy*idy);
	tmpV.x += s * fx;
	tmpV.y += s * fy;
	*fj = tmpV;

}

__global__ void advectVelocity_k(	HBXDef::HbxCuDef::UserCalPrec2 * v, HBXDef::UserCalPrec * vx, HBXDef::UserCalPrec * vy, 
									int dx, int pdx, int dy, float dt, int lb)
{
	int gtidx = blockIdx.x * blockDim.x + threadIdx.x;
	int gtidy = blockIdx.y * (lb * blockDim.y) + threadIdx.y * lb;
	int p;

	float2 tmpV;	//临时速度域变量
	float2 pLoc;	//地址对齐后的索引
	HBXDef::UserCalPrec vxterm, vyterm;

	//gtidx是该线程x位置所在的速度域
	if (gtidx < dx)
	{
		for (p = 0; p < lb; p++)
		{
			// fi是该线程y位置所在的速度域
			int fi = gtidy + p;
			if (fi < dy)
			{
				int fj = fi * pdx + gtidx;//线性化后的索引
				tmpV = tex2D(g_texVeloc, (float)gtidx, (float)fi);
				pLoc.x = (gtidx + 0.5f) - (dt * tmpV.x * dx);
				pLoc.y = (fi + 0.5f) - (dt * tmpV.y * dy);
				tmpV = tex2D(g_texVeloc, pLoc.x, pLoc.y);
				vxterm = (HBXDef::UserCalPrec)tmpV.x;
				vyterm = (HBXDef::UserCalPrec)tmpV.y;
				vx[fj] = vxterm;
				vy[fj] = vyterm;
			}
		}
	}

}
