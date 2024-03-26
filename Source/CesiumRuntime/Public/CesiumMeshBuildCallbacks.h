/*--------------------------------------------------------------------------------------+
|
|     $Source: CesiumMeshBuildCallbacks.cpp $
|
|  $Copyright: (c) 2024 Bentley Systems, Incorporated. All rights reserved. $
|
+--------------------------------------------------------------------------------------*/

#pragma once

#include <Cesium3DTilesSelection/TileID.h>
#include <UObject/WeakObjectPtr.h>
#include <unordered_map>

class UMaterialInstanceDynamic;
class UStaticMeshComponent;
struct FCesiumModelMetadata;
struct FCesiumPrimitiveFeatures;

namespace CesiumGltf {
    struct MeshPrimitive;
} // namespace CesiumGltf
using FCesiumToUnrealTexCoordMap = std::unordered_map<int32_t, uint32_t>;


class CESIUMRUNTIME_API ICesiumMeshBuildCallbacks
{
public:
  ICesiumMeshBuildCallbacks();
  virtual ~ICesiumMeshBuildCallbacks();

  struct FCesiumMeshData
  {
      const CesiumGltf::MeshPrimitive* pMeshPrimitive;
      const FCesiumModelMetadata& Metadata;
      const FCesiumPrimitiveFeatures& Features;
      FCesiumToUnrealTexCoordMap& GltfToUnrealTexCoordMap;
  };

  /**
   * Called at the end of the static mesh component construction.
   */
  virtual void OnMeshConstructed(
      const Cesium3DTilesSelection::TileID& tileId,
      const TWeakObjectPtr<UStaticMeshComponent>& MeshComponent,
      const TWeakObjectPtr<UMaterialInstanceDynamic>& pMaterial,
      const FCesiumMeshData& CesiumMeshData) = 0;

  /**
   * Whether an extra UV layer should be allocated for feature IDs.
   */
  virtual bool ShouldAllocateUVForFeatures() const = 0;

private:
    static TSharedPtr<ICesiumMeshBuildCallbacks> Singleton;
};

