#pragma once

// CUDA includes

#include <cuda.h>
#include <cuda_runtime.h>

#include <stdio.h>
#include <thrust/sort.h>
#include <thrust/execution_policy.h>
#include <thrust/random.h>
#include <thrust/device_vector.h>
#include <thrust/host_vector.h>
#include <device_launch_parameters.h>
#include <cmath>
#include <vector>
#include <mutex>

// Marching Cube includes
#include "SETTINGS.h"
#include "mesh.h"
#include "field.h"

void checkCUDAErrorFn(const char* msg, const char* file = NULL, int line = -1);


namespace CudaMC {

    
    inline Real mc_internalLength2(const VEC3F& v)
    {
        return v.x() * v.x() + v.y() * v.y() + v.z() * v.z();
    }
    inline Real mc_internalLength(const VEC3F& v)
    {
        return std::sqrt(mc_internalLength2(v));
    }
    inline VEC3F mc_internalNormalize(const VEC3F& v)
    {
        Real vv = mc_internalLength(v);
        return VEC3F(v.x() / vv, v.y() / vv, v.z() / vv);
    }
    inline VEC3F mc_internalCross(const VEC3F& v1, const VEC3F& v2)
    {
        return VEC3F(v1.y() * v2.z() - v1.z() * v2.y(), v1.z() * v2.x() - v1.x() * v2.z(), v1.x() * v2.y() - v1.y() * v2.x());
    }
    inline VEC3F operator-(const VEC3F& l, const VEC3F r)
    {
        return VEC3F(l.x() - r.x(), l.y() - r.y(), l.z() - r.z());
    }

    // ------------------------------------------

    void setDefaultArraySizes(uint vertSize, uint normSize, uint triSize);
    
    inline uint mc_internalToIndex1D(uint i, uint j, uint k, const VEC3I& size)
    {
        return (k * size.y() + j) * size.x() + i;
    }

    inline uint mc_internalToIndex1DSlab(uint i, uint j, uint k, const VEC3I& size)
    {
        return size.x() * size.y() * (k % 2) + j * size.x() + i;
    }

    // ------------------------------------------

}