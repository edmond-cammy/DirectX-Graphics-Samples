//*********************************************************
//
// Copyright (c) Microsoft. All rights reserved.
// This code is licensed under the MIT License (MIT).
// THIS CODE IS PROVIDED *AS IS* WITHOUT WARRANTY OF
// ANY KIND, EITHER EXPRESS OR IMPLIED, INCLUDING ANY
// IMPLIED WARRANTIES OF FITNESS FOR A PARTICULAR
// PURPOSE, MERCHANTABILITY, OR NON-INFRINGEMENT.
//
//*********************************************************

#pragma once

#include "RayTracingHlslCompat.h"

// ToDo standardize use of CS suffix
namespace ComputeShader {
	namespace Type {
		enum Enum {
			HemisphereSampleSetVisualization = 0,
			ReduceSum,
			ComposeRenderPassesCS,
            AoBlurCS,
            AoBlurAndUpsampleCS,
			Count
		};
	}
	namespace RootSignature {
		namespace HemisphereSampleSetVisualization {
			namespace Slot {
				enum Enum {
					Output = 0,
					SampleBuffers,
					SceneConstant,
					Count
				};
			}
		}		
		namespace ReduceSum {
			namespace Slot {
				enum Enum {
					Output = 0,
					Input,
					Count
				};
			}
		}

		namespace ComposeRenderPassesCS {
			namespace Slot {
				enum Enum {
					Output = 0,
					GBufferResources,
					AO,
					Visibility,
					MaterialBuffer,
					ConstantBuffer,
                    FilterWeightSum,
					Count
				};
			}
		}
		
		namespace AoBlurCS {
			namespace Slot {
				enum Enum {
					Output = 0,
					Normal,
                    Distance,
                    InputAO,
					ConstantBuffer,
					Count
				};
			}
		}
		
	}
	namespace RS = RootSignature;
}
namespace CSType = ComputeShader::Type;
namespace CSRootSignature = ComputeShader::RootSignature;


namespace DownsampleFilter {
	enum Enum {
		None = 0,
		BoxFilter2x2,
		GaussianFilter9Tap,
		GaussianFilter25Tap,
		Count
	};
}

namespace GlobalRootSignature {
    namespace Slot {
        enum Enum {
            Output = 0,
			GBufferResources,
			GBufferResourcesIn,
			AOResourcesOut,	// ToDo cleanup, move to local root sigs 
			VisibilityResource,	// ToDo cleanup, move to local root sigs 
            AccelerationStructure,
            SceneConstant,
			MaterialBuffer,
            SampleBuffers,
            EnvironmentMap,
            FilterWeightSum,
            GBufferDepth,   // ToDo move to the above slot for GBufferResources ?
            GbufferNormalRGB,
            Count
        };
    }
}

namespace LocalRootSignature {
    namespace Type {
        enum Enum {
            Triangle = 0,
            AABB,
            Count
        };
    }
}

namespace LocalRootSignature {
    namespace Triangle {
        namespace Slot {
            enum Enum {
                ConstantBuffer = 0,
                VertexBuffers,
                DiffuseTexture,
                NormalTexture,
                Count
            };
        }
        struct RootArguments {
            PrimitiveConstantBuffer cb;
			//XMUINT3 padding;		// ToDo remove/explain why - CB needs to be aligned to 16B
            D3D12_GPU_DESCRIPTOR_HANDLE vertexBufferGPUHandle;
            D3D12_GPU_DESCRIPTOR_HANDLE diffuseTextureGPUHandle;
            D3D12_GPU_DESCRIPTOR_HANDLE normalTextureGPUHandle;
        };
    }
}

namespace LocalRootSignature {
    inline UINT MaxRootArgumentsSize()
    {
        return sizeof(Triangle::RootArguments);
    }
}

// Todo rename
namespace ReduceSumCalculations {
	enum Enum {
		CameraRayHits = 0,
		AORayHits,
		Count
	};
}

namespace GeometryType {
    enum Enum {
        Plane = 0,
        Sphere,
		SquidRoom,
		PBRT,
        Count
    };
}

// ToDo fix names
// ToDo Gputimers bug out changing enums around
namespace GpuTimers {
	enum Enum {
		Raytracing_GBuffer = 0,
        Raytracing_Variance,
		Raytracing_AO,
        Denoising,
		Raytracing_BlurAO,
        Raytracing_FilterWeightSum,
        Raytracing_VarianceSmoothing,
		Raytracing_Visibility,
        DownsampleToBackbuffer,
        DownsampleGBufferBilateral,
        UpsampleAOBilateral,
        ComposeRenderPassesCS,
        UpdateBLAS,
        ReduceSum,
        UpdateTLAS,
		Count
	};
}

namespace UIParameters {
	enum Enum {
		RaytracingAPI = 0,
		BuildQuality,
		UpdateAlgorithm,
		TesselationQuality,
		NumberOfObjects,
		Count
	};
}

// ToDo update descriptions, prune redundant.
namespace GBufferResource {
	enum Enum {
		Hit = 0,		// Geometry hit or not.
		Material,		// Material of the object hit ~ {MaterialID, texCoord}.
		HitPosition,	// 3D position of hit.
		SurfaceNormal,	// 3D normal at a hit and dot(normal, rayDir) in W,
        Distance,       // Length along ray of hit.
        Depth,          // Non-linear depth of the hit.
        SurfaceNormalRGB, // 3D normal at a hit. // ToDo deduplicate
		Count
	};
}

namespace AOResource {
	enum Enum {
		Coefficient = 0,
        Smoothed,
		HitCount,
        FilterWeightSum,
		Count
	};
}

namespace Scene {
	namespace Type {
		enum Enum {
			SingleObject = 0,
			GeometricForest,
			SquidRoom,
			PBRT,	// Rename
			Count
		};
		extern const WCHAR* Names[Count];
	}


	struct Camera
	{
		struct CameraPosition
		{
			XMVECTOR eye, at, up;
		};

		struct CameraBoundaries
		{
			XMVECTOR min, max;
		};

		CameraPosition position;
		CameraBoundaries boundaries;
	};

	struct Params {
		Camera camera;
	};

	class Initialize
	{
	public:
		Initialize();
	};
	extern Params args[Scene::Type::Count];
}

namespace SceneEnums
{
	namespace VertexBuffer {
		enum Value { SceneGeometry = 0, Count };
	}
}




// Bottom-level acceleration structures (BottomLevelASType).
// This sample uses two BottomLevelASType, one for AABB and one for Triangle geometry.
// Mixing of geometry types within a BLAS is not supported.
namespace BottomLevelASType = GeometryType;