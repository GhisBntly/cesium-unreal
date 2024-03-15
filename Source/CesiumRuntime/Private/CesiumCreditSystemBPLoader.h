// Copyright 2020-2021 CesiumGS, Inc. and Contributors

#pragma once

#include "CesiumCreditSystemBPLoader.generated.h"

UCLASS()
class UCesiumCreditSystemBPLoader : public UObject {
  GENERATED_BODY()

public:
  UCesiumCreditSystemBPLoader();

  UPROPERTY()
  TSoftObjectPtr<UObject> CesiumCreditSystemBP = TSoftObjectPtr<
      UObject>(FSoftObjectPath(TEXT(
      "Class'/ITwinForUnreal/CesiumCreditSystemBP.CesiumCreditSystemBP_C'")));
};
