// Copyright 2020-2021 CesiumGS, Inc. and Contributors

#include "ITwinCesiumRuntime.h"
#include "Cesium3DTilesContent/registerAllTileContentTypes.h"
#include "CesiumAsync/CachingAssetAccessor.h"
#include "CesiumAsync/GunzipAssetAccessor.h"
#include "CesiumAsync/SqliteCache.h"
#include "ITwinCesiumRuntimeSettings.h"
#include "CesiumUtility/Tracing.h"
#include "HAL/FileManager.h"
#include "HttpModule.h"
#include "Interfaces/IPluginManager.h"
#include "Misc/Paths.h"
#include "ShaderCore.h"
#include "ITwinSpdlogUnrealLoggerSink.h"
#include "ITwinUnrealAssetAccessor.h"
#include "ITwinUnrealTaskProcessor.h"
#include <CesiumAsync/AsyncSystem.h>
#include <CesiumAsync/IAssetAccessor.h>
#include <Modules/ModuleManager.h>
#include <spdlog/spdlog.h>
#include "CesiumJsonReader/ExtensionsJsonHandler.h"
#include "CesiumGltfReader/ExtensionCesiumRTCReader.h"
#include "CesiumGltfReader/GltfReader.h"


//#include "CesiumGltfReader/generated/src/registerExtensions.h"
#include "../../../../../../Extern/cesium-unreal/extern/cesium-native/CesiumGltfReader/src/NamedObjectJsonHandler.h"
#include "../../../../../../Extern/cesium-unreal/extern/cesium-native/CesiumGltfReader/generated/src/ExtensionCesiumRTCJsonHandler.h"
#include "../../../../../../Extern/cesium-unreal/extern/cesium-native/CesiumGltfReader/generated/src/ExtensionBufferViewExtMeshoptCompressionJsonHandler.h"
#include "../../../../../../Extern/cesium-unreal/extern/cesium-native/CesiumGltfReader/generated/src/ExtensionCesiumTileEdgesJsonHandler.h"
#include "../../../../../../Extern/cesium-unreal/extern/cesium-native/CesiumGltfReader/generated/src/ExtensionExtInstanceFeaturesJsonHandler.h"
#include "../../../../../../Extern/cesium-unreal/extern/cesium-native/CesiumGltfReader/generated/src/ExtensionExtMeshFeaturesJsonHandler.h"
#include "../../../../../../Extern/cesium-unreal/extern/cesium-native/CesiumGltfReader/generated/src/ExtensionExtMeshGpuInstancingJsonHandler.h"
#include "../../../../../../Extern/cesium-unreal/extern/cesium-native/CesiumGltfReader/generated/src/ExtensionKhrDracoMeshCompressionJsonHandler.h"
#include "../../../../../../Extern/cesium-unreal/extern/cesium-native/CesiumGltfReader/generated/src/ExtensionKhrMaterialsUnlitJsonHandler.h"
#include "../../../../../../Extern/cesium-unreal/extern/cesium-native/CesiumGltfReader/generated/src/ExtensionKhrTextureBasisuJsonHandler.h"
#include "../../../../../../Extern/cesium-unreal/extern/cesium-native/CesiumGltfReader/generated/src/ExtensionKhrTextureTransformJsonHandler.h"
#include "../../../../../../Extern/cesium-unreal/extern/cesium-native/CesiumGltfReader/generated/src/ExtensionMeshPrimitiveExtFeatureMetadataJsonHandler.h"
#include "../../../../../../Extern/cesium-unreal/extern/cesium-native/CesiumGltfReader/generated/src/ExtensionMeshPrimitiveExtStructuralMetadataJsonHandler.h"
#include "../../../../../../Extern/cesium-unreal/extern/cesium-native/CesiumGltfReader/generated/src/ExtensionMeshPrimitiveKhrMaterialsVariantsJsonHandler.h"
#include "../../../../../../Extern/cesium-unreal/extern/cesium-native/CesiumGltfReader/generated/src/ExtensionModelExtStructuralMetadataJsonHandler.h"
#include "../../../../../../Extern/cesium-unreal/extern/cesium-native/CesiumGltfReader/generated/src/ExtensionModelKhrMaterialsVariantsJsonHandler.h"
#include "../../../../../../Extern/cesium-unreal/extern/cesium-native/CesiumGltfReader/generated/src/ExtensionModelMaxarMeshVariantsJsonHandler.h"
#include "../../../../../../Extern/cesium-unreal/extern/cesium-native/CesiumGltfReader/generated/src/ExtensionNodeMaxarMeshVariantsJsonHandler.h"
#include "../../../../../../Extern/cesium-unreal/extern/cesium-native/CesiumGltfReader/generated/src/ExtensionTextureWebpJsonHandler.h"
#include "../../../../../../Extern/cesium-unreal/extern/cesium-native/CesiumGltfReader/generated/src/ExtensionModelExtFeatureMetadataJsonHandler.h"
#include "../../../../../../Extern/cesium-unreal/extern/cesium-native/CesiumGltfReader/generated/src/ExtensionBufferExtMeshoptCompressionJsonHandler.h"


using namespace CesiumUtility;
class ExtensionTester
{
public:
    std::shared_ptr<CesiumJsonReader::ExtensionsJsonHandler> handler;
    CesiumGltfReader::GltfReader reader;
    ExtensionTester()
    {
        handler.reset(new CesiumJsonReader::ExtensionsJsonHandler(reader.getOptions()));
    }
    template <typename TExtended, typename TExtensionHandler>
    void registerExtension() {
        TExtended obj;
        handler->reset(nullptr, &obj, TExtended::TypeName);
        auto* extHandler = handler->readObjectKey(TExtensionHandler::ExtensionName);
 /*       auto realext = dynamic_cast<TExtensionHandler*>(extHandler);
        if (!realext)
        {
            throw std::runtime_error(std::string("wrong extension handler for ") + std::string(TExtensionHandler::ExtensionName));
        }*/
    }
};
void TestExtension()
{
    using namespace CesiumGltfReader;
    ExtensionTester options;
    options
        .registerExtension<CesiumGltf::Model, ExtensionCesiumRTCJsonHandler>();
    options.registerExtension<
        CesiumGltf::Model,
        ExtensionModelExtFeatureMetadataJsonHandler>();
    options.registerExtension<
        CesiumGltf::Model,
        ExtensionModelExtStructuralMetadataJsonHandler>();
    options.registerExtension<
        CesiumGltf::Model,
        ExtensionModelKhrMaterialsVariantsJsonHandler>();
    options.registerExtension<
        CesiumGltf::Model,
        ExtensionModelMaxarMeshVariantsJsonHandler>();
    options.registerExtension<
        CesiumGltf::MeshPrimitive,
        ExtensionCesiumTileEdgesJsonHandler>();
    options.registerExtension<
        CesiumGltf::MeshPrimitive,
        ExtensionMeshPrimitiveExtFeatureMetadataJsonHandler>();
    options.registerExtension<
        CesiumGltf::MeshPrimitive,
        ExtensionExtMeshFeaturesJsonHandler>();
    options.registerExtension<
        CesiumGltf::MeshPrimitive,
        ExtensionMeshPrimitiveExtStructuralMetadataJsonHandler>();
    options.registerExtension<
        CesiumGltf::MeshPrimitive,
        ExtensionKhrDracoMeshCompressionJsonHandler>();
    options.registerExtension<
        CesiumGltf::MeshPrimitive,
        ExtensionMeshPrimitiveKhrMaterialsVariantsJsonHandler>();
    options.registerExtension<
        CesiumGltf::Node,
        ExtensionExtInstanceFeaturesJsonHandler>();
    options.registerExtension<
        CesiumGltf::Node,
        ExtensionExtMeshGpuInstancingJsonHandler>();
    options.registerExtension<
        CesiumGltf::Node,
        ExtensionNodeMaxarMeshVariantsJsonHandler>();
    options.registerExtension<
        CesiumGltf::Buffer,
        ExtensionBufferExtMeshoptCompressionJsonHandler>();
    options.registerExtension<
        CesiumGltf::BufferView,
        ExtensionBufferViewExtMeshoptCompressionJsonHandler>();
    options.registerExtension<
        CesiumGltf::Material,
        ExtensionKhrMaterialsUnlitJsonHandler>();
    options.registerExtension<
        CesiumGltf::Texture,
        ExtensionKhrTextureBasisuJsonHandler>();
    options.registerExtension<
        CesiumGltf::Texture,
        ExtensionTextureWebpJsonHandler>();
    options.registerExtension<
        CesiumGltf::TextureInfo,
        ExtensionKhrTextureTransformJsonHandler>();
    options.registerExtension<
        CesiumGltf::MaterialOcclusionTextureInfo,
        ExtensionKhrTextureTransformJsonHandler>();
    options.registerExtension<
        CesiumGltf::MaterialNormalTextureInfo,
        ExtensionKhrTextureTransformJsonHandler>();
}
#if CESIUM_TRACING_ENABLED
#include <chrono>
#endif

#define LOCTEXT_NAMESPACE "FITwinCesiumRuntimeModule"

DEFINE_LOG_CATEGORY(LogITwinCesium);

void FITwinCesiumRuntimeModule::StartupModule() {
  Cesium3DTilesContent::registerAllTileContentTypes();

  std::shared_ptr<spdlog::logger> pLogger = spdlog::default_logger();
  pLogger->sinks() = {std::make_shared<ITwinSpdlogUnrealLoggerSink>()};

  FModuleManager::Get().LoadModuleChecked(TEXT("HTTP"));

  CESIUM_TRACE_INIT(
      "cesium-trace-" +
      std::to_string(std::chrono::time_point_cast<std::chrono::microseconds>(
                         std::chrono::steady_clock::now())
                         .time_since_epoch()
                         .count()) +
      ".json");

  FString PluginShaderDir = FPaths::Combine(
      IPluginManager::Get().FindPlugin(TEXT("ITwinForUnreal"))->GetBaseDir(),
      TEXT("Shaders"));
  AddShaderSourceDirectoryMapping(
      TEXT("/Plugin/ITwinForUnreal"),
      PluginShaderDir);
  TestExtension();
}

void FITwinCesiumRuntimeModule::ShutdownModule() { CESIUM_TRACE_SHUTDOWN(); }

#undef LOCTEXT_NAMESPACE

IMPLEMENT_MODULE(FITwinCesiumRuntimeModule, ITwinCesiumRuntime)

FITwinCesium3DTilesetIonTroubleshooting OnCesium3DTilesetIonTroubleshooting{};
FITwinCesiumRasterOverlayIonTroubleshooting
    OnCesiumRasterOverlayIonTroubleshooting{};

CesiumAsync::AsyncSystem& ITwinCesium::getAsyncSystem() noexcept {
  static CesiumAsync::AsyncSystem asyncSystem(
      std::make_shared<ITwinUnrealTaskProcessor>());
  return asyncSystem;
}

namespace ITwinCesium {

std::string getCacheDatabaseName() {
#if PLATFORM_ANDROID
  FString BaseDirectory = FPaths::ProjectPersistentDownloadDir();
#elif PLATFORM_IOS
  FString BaseDirectory =
      FPaths::Combine(*FPaths::ProjectSavedDir(), TEXT("Cesium"));
  if (!IFileManager::Get().DirectoryExists(*BaseDirectory)) {
    IFileManager::Get().MakeDirectory(*BaseDirectory, true);
  }
#else
  FString BaseDirectory = FPaths::EngineUserDir();
#endif

  FString CesiumDBFile =
      FPaths::Combine(*BaseDirectory, TEXT("cesium-request-cache.sqlite"));
  FString PlatformAbsolutePath =
      IFileManager::Get().ConvertToAbsolutePathForExternalAppForWrite(
          *CesiumDBFile);

  UE_LOG(
      LogITwinCesium,
      Display,
      TEXT("Caching Cesium requests in %s"),
      *PlatformAbsolutePath);

  return TCHAR_TO_UTF8(*PlatformAbsolutePath);
}

std::shared_ptr<CesiumAsync::ICacheDatabase>& getCacheDatabase() {
  static int MaxCacheItems =
      GetDefault<UITwinCesiumRuntimeSettings>()->MaxCacheItems;

  static std::shared_ptr<CesiumAsync::ICacheDatabase> pCacheDatabase =
      std::make_shared<CesiumAsync::SqliteCache>(
          spdlog::default_logger(),
          ITwinCesium::getCacheDatabaseName(),
          MaxCacheItems);

  return pCacheDatabase;
}

const std::shared_ptr<CesiumAsync::IAssetAccessor>& getAssetAccessor() {
  static int RequestsPerCachePrune =
      GetDefault<UITwinCesiumRuntimeSettings>()->RequestsPerCachePrune;
  static std::shared_ptr<CesiumAsync::IAssetAccessor> pAssetAccessor =
      std::make_shared<CesiumAsync::GunzipAssetAccessor>(
          std::make_shared<CesiumAsync::CachingAssetAccessor>(
              spdlog::default_logger(),
              std::make_shared<ITwinUnrealAssetAccessor>(),
              ITwinCesium::getCacheDatabase(),
              RequestsPerCachePrune));
  return pAssetAccessor;
}

} //ITwinCesium
