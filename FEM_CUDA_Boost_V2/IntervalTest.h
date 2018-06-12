#pragma once
#include <CudaPreDef.h>
#include <CuDefStruct.h.h>
#include <libInterpolate\cuInterpolate.h>
#include <libCUFEM\BaseReader.h>
#include <libCUFEM\InputRecord.h>
#include <libReader\AeroCoefReader.h>
#include <HbxXmlDef\AeroTable.h>
#include <libInterpolate\interval_lib.h>
#include <thrust\device_vector.h>
#include <HbxCudaDef\multithreading.h>

extern "C" CUT_THREADPROC InterpolateTest(void* void_arg = nullptr);
CUT_THREADPROC AsynInterpolateTest(void* void_arg = nullptr);
extern "C" void QuadTree_Test();


extern "C" float MultiInterpolateTest(void* void_arg = nullptr);


CUT_THREADPROC postprocess(void *void_arg);
void CUDART_CB myStreamCallback(cudaStream_t stream, cudaError_t status, void *data);
