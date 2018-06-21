#include "cuFluid.h"
#include "..\libFluid2D\Fluid_kernel.cuh"
#include <helper_cuda.h>

// Œ∆¿Ì∂‘∆Î≥ﬂ¥Á
size_t g_TexturePitch = 0;
cudaArray* g_cuArray = nullptr;

void updateTexture(HBXDef::HbxCuDef::UserCalPrec2 *data, size_t wib, size_t h, size_t pitch)
{
	cudaMemcpy2DToArray(g_cuArray, 0, 0, data, pitch, wib, h, cudaMemcpyDeviceToDevice);
	getLastCudaError("cudaMemcpy failed");
}


void addForces(	HBXDef::HbxCuDef::UserCalPrec2 * v, 
				int dx, int dy, 
				int spx, int spy,
				float fx, float fy, int r)
{
	dim3 tids(2*r+1, 2*r+1);
	addForces_k <<<1, tids >>> (v, dx, dy, spx, spy, fx, fy, r, g_TexturePitch);
	getLastCudaError("Func addForces_k failed");
}

void advectVelocity(HBXDef::HbxCuDef::UserCalPrec2 * v, 
					HBXDef::UserCalPrec * vx, HBXDef::UserCalPrec * vy,
					int dx, int pdx, int dy, HBXDef::UserCalPrec dt,
					int xTile, int yTile, 
					int xTidsPerTile, int yTidsPerTile,
					int _xDim, int _yDim)
{

	dim3 grids( (dx/xTile) + ((dx%xTile)?1:0) , (dy/yTile) + ((dy%yTile)?1:0) );
	dim3 blocks(xTidsPerTile, yTidsPerTile);

	updateTexture(v, _xDim * sizeof(HBXDef::HbxCuDef::UserCalPrec2), _yDim, g_TexturePitch);
	advectVelocity_k( v, vx, vy, dx, pdx, dy, dt, yTile/ yTidsPerTile);

	getLastCudaError("Func advectVelocity failed");
}

void diffuseProject(HBXDef::HbxCuDef::UserCalPrec2 * vx, HBXDef::HbxCuDef::UserCalPrec2 * vy, int dx, int dy, float dt, float visc)
{




	getLastCudaError("Func diffuseProject failed");
}
