// Copyright 2020-2023 CesiumGS, Inc. and Contributors

#include "ITwinCesiumMetadataPropertyAccess.h"

#include "ITwinCesiumFeatureIdSet.h"
#include "ITwinCesiumModelMetadata.h"
#include "ITwinCesiumPrimitiveFeatures.h"

/*static*/
const FITwinCesiumPropertyTableProperty*
FITwinCesiumMetadataPropertyAccess::FindValidProperty(
    const FITwinCesiumPrimitiveFeatures& Features,
    const FITwinCesiumModelMetadata& Metadata,
    const FString& PropertyName,
    int64 FeatureIDSetIndex) {
  const TArray<FITwinCesiumFeatureIdSet>& featureIDSets =
      UITwinCesiumPrimitiveFeaturesBlueprintLibrary::GetFeatureIDSets(Features);

  if (FeatureIDSetIndex < 0 || FeatureIDSetIndex >= featureIDSets.Num()) {
    return nullptr;
  }

  const FITwinCesiumFeatureIdSet& featureIDSet =
      featureIDSets[FeatureIDSetIndex];
  const int64 propertyTableIndex =
      UITwinCesiumFeatureIdSetBlueprintLibrary::GetPropertyTableIndex(
          featureIDSet);

  const TArray<FITwinCesiumPropertyTable>& propertyTables =
      UITwinCesiumModelMetadataBlueprintLibrary::GetPropertyTables(Metadata);
  if (propertyTableIndex < 0 || propertyTableIndex >= propertyTables.Num()) {
    return nullptr;
  }
  const FITwinCesiumPropertyTableProperty& propWithName =
      UITwinCesiumPropertyTableBlueprintLibrary::FindProperty(
          propertyTables[propertyTableIndex],
          PropertyName);
  const EITwinCesiumPropertyTablePropertyStatus status =
      UITwinCesiumPropertyTablePropertyBlueprintLibrary::
          GetPropertyTablePropertyStatus(propWithName);
  if (status != EITwinCesiumPropertyTablePropertyStatus::Valid) {
    return nullptr;
  }
  return &propWithName;
}
