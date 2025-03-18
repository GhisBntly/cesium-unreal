// Copyright 2020-2023 CesiumGS, Inc. and Contributors

#pragma once

#include "Containers/UnrealString.h"

struct FITwinCesiumPrimitiveFeatures;
struct FITwinCesiumModelMetadata;
struct FITwinCesiumPropertyTableProperty;


class ITWINCESIUMRUNTIME_API FITwinCesiumMetadataPropertyAccess {

public:
  /**
   * Retrieves a property by name.
   * If the specified feature ID set does not exist or if the property table
   * does not contain a property with that name, this function returns nullptr.
   */
  static const FITwinCesiumPropertyTableProperty* FindValidProperty(
      const FITwinCesiumPrimitiveFeatures& Features,
      const FITwinCesiumModelMetadata& Metadata,
      const FString& PropertyName,
      int64 FeatureIDSetIndex = 0);
};
