/*--------------------------------------------------------------------------------------+
|
|     $Source: CesiumMeshBuildCallbacks.cpp $
|
|  $Copyright: (c) 2024 Bentley Systems, Incorporated. All rights reserved. $
|
+--------------------------------------------------------------------------------------*/

#pragma once

#include <Cesium3DTilesSelection/TileID.h>

class UMaterialInstanceDynamic;
class UStaticMeshComponent;
struct FCesiumModelMetadata;
struct FCesiumPrimitiveFeatures;


class CESIUMRUNTIME_API ICesiumMeshBuildCallbacks {
public:
  ICesiumMeshBuildCallbacks();
  virtual ~ICesiumMeshBuildCallbacks();

  /**
   * Called at the end of the static mesh component construction.
   */
  virtual void OnMeshConstructed(
      const Cesium3DTilesSelection::TileID& tileId,
      const TWeakObjectPtr<UStaticMeshComponent>& MeshComponent,
      const TWeakObjectPtr<UMaterialInstanceDynamic>& pMaterial,
      const FCesiumModelMetadata& Metadata,
      const FCesiumPrimitiveFeatures& Features) = 0;

private:
    static TSharedPtr<ICesiumMeshBuildCallbacks> Singleton;
};

