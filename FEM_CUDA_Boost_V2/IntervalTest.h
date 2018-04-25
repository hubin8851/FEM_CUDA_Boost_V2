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

extern "C" void InterpolateTest();
extern "C" void QuadTree_Test();
