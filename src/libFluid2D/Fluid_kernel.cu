#include <CudaPreDef.h>
#include <cufft.h>
#include <helper_gl.h>
#include "..\libFluid2D\Fluid_kernel.cuh"

// Texture reference for reading velocity field
texture<float2, 2> g_texVeloc;//�����ڴ�������ֵ����ʱֻ֧��float��

__global__ void addForces_k(HBXDef::HbxCuDef::UserCalPrec2 * v, int dx, int dy, int spx, int spy, float fx, float fy, int r, size_t pitch)
{
	int idx = threadIdx.x;
	int idy = threadIdx.y;

	//��ά������ǰ�̶߳�Ӧ���ٶ���
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

	float2 tmpV;	//��ʱ�ٶ������
	float2 pLoc;	//��ַ����������
	HBXDef::UserCalPrec vxterm, vyterm;

	//gtidx�Ǹ��߳�xλ�����ڵ��ٶ���
	if (gtidx < dx)
	{
		for (p = 0; p < lb; p++)
		{
			// fi�Ǹ��߳�yλ�����ڵ��ٶ���
			int fi = gtidy + p;
			if (fi < dy)
			{
				int fj = fi * pdx + gtidx;//���Ի��������
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
