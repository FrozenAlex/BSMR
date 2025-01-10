#include "main.hpp"
#include "_config.hpp"
#include "Unity/XR/Oculus/OculusLoader.hpp"
#include "GlobalNamespace/MainSystemInit.hpp"
#include "GlobalNamespace/ConditionalActivation.hpp"
#include "BeatSaber/PerformancePresets/ObstaclesQuality.hpp"
#include "UnityEngine/Resources.hpp"
#include "UnityEngine/Renderer.hpp"
#include "UnityEngine/Object.hpp"
#include "UnityEngine/Camera.hpp"
#include "UnityEngine/ScriptableObject.hpp"
#include "UnityEngine/XR/XRSettings.hpp"
#include "UnityEngine/QualitySettings.hpp"
#include "UnityEngine/CameraClearFlags.hpp"
#include "UnityEngine/Renderer.hpp"
#include "UnityEngine/Rendering/CommandBuffer.hpp"
#include "UnityEngine/RenderTexture.hpp"
#include "UnityEngine/RenderTextureFormat.hpp"
#include "UnityEngine/RenderTextureReadWrite.hpp"
#include "UnityEngine/VRTextureUsage.hpp"
#include "UnityEngine/ParticleSystem.hpp"
#include "GlobalNamespace/ObservableVariableSO_1.hpp"
#include "GlobalNamespace/MainCamera.hpp"
#include "GlobalNamespace/IFileStorage.hpp"
#include "GlobalNamespace/ConditionalMaterialSwitcher.hpp"
#include "GlobalNamespace/AlwaysVisibleQuad.hpp"
#include "GlobalNamespace/OVRManager.hpp"
#include "GlobalNamespace/OVRPassthroughLayer.hpp"
#include "GlobalNamespace/OVROverlay.hpp"
#include "GlobalNamespace/BloomPrePass.hpp"
#include "GlobalNamespace/BoolSO.hpp"
#include "GlobalNamespace/OVRPlugin.hpp"
#include "GlobalNamespace/ObstacleMaterialSetter.hpp"
#include "GlobalNamespace/ShockwaveEffect.hpp"
#include "GlobalNamespace/MainSystemInit.hpp"
#include "GlobalNamespace/MirrorRendererSO.hpp"
#include "GlobalNamespace/MainEffectController.hpp"
#include "GlobalNamespace/MirrorRendererGraphicsSettingsPresets.hpp"
#include "GlobalNamespace/BloomPrePassEffectContainerSO.hpp"
#include "GlobalNamespace/BloomPrePassEffectSO.hpp"
#include "GlobalNamespace/FakeReflectionDynamicObjectsState.hpp"
#include "GlobalNamespace/PyramidBloomRendererSO.hpp"
#include "GlobalNamespace/FPSCounter.hpp"
#include "GlobalNamespace/VRRenderingParamsSetup.hpp"
#include "GlobalNamespace/FPSCounterUIController.hpp"
#include "GlobalNamespace/GameplayCoreInstaller.hpp"
#include "GlobalNamespace/PerformanceVisualizer.hpp"
#include "GlobalNamespace/FakeMirrorObjectsInstaller.hpp"
#include "GlobalNamespace/MirroredGameNoteController.hpp"
#include "GlobalNamespace/MirroredBombNoteController.hpp"
#include "GlobalNamespace/MirroredObstacleController.hpp"
#include "GlobalNamespace/MirroredSliderController.hpp"
#include "GlobalNamespace/DepthTextureController.hpp"
#include "GlobalNamespace/MirroredBeatmapObjectManager.hpp"
#include "GlobalNamespace/MirrorRendererGraphicsSettingsPresets.hpp"
#include "GlobalNamespace/MainFlowCoordinator.hpp"
#include "GlobalNamespace/PyramidBloomMainEffectSO.hpp"
#include "Zenject/DiContainer.hpp"
#include "Zenject/ScopeConcreteIdArgConditionCopyNonLazyBinder.hpp"
#include "Zenject/FromBinderGeneric_1.hpp"
#include "bsml/shared/BSML/MainThreadScheduler.hpp"
#include "bsml/shared/BSML/SharedCoroutineStarter.hpp"
#include "bsml/shared/BSML.hpp"
#include "UI/BSMRFlowCoordinator.hpp"
#include "GlobalNamespace/BloomPrePass.hpp"
#include "UnityEngine/Camera.hpp"
#include "UnityEngine/GameObject.hpp"
#include "UnityEngine/Shader.hpp"
#include "BeatSaber/PerformancePresets/PerformancePreset.hpp"
#include "BeatSaber/PerformancePresets/QuestPerformanceSettings.hpp"
#include "BeatSaber/GameSettings/GraphicSettingsHandler.hpp"
#include "GlobalNamespace/SaberBurnMarkArea.hpp"
#include "logging.hpp"


inline modloader::ModInfo modInfo = {MOD_ID, VERSION, GIT_COMMIT}; // Stores the ID and version of our mod, and is sent to the modloader upon startup

using namespace BSMR;

// Called at the early stages of game loading
GT_EXPORT_FUNC void setup(CModInfo& info) {
    info.id = MOD_ID;
    info.version = VERSION;
    info.version_long = GIT_COMMIT;
    modInfo.assign(info);
	
    INFO("Setting up BSMR config...");
    getBSMRConfig().Init(modInfo);

    INFO("Completed setup!");
}

MAKE_HOOK_MATCH(
    GameplayCoreInstaller_InstallBindings,
    &GlobalNamespace::GameplayCoreInstaller::InstallBindings,
    void,
    GlobalNamespace::GameplayCoreInstaller* self
) {
    DEBUG("GameplayCoreInstaller_InstallBindings hook called!");
    GameplayCoreInstaller_InstallBindings(self);
}

MAKE_HOOK_MATCH(MainFlowCoordinator_DidActivate, &GlobalNamespace::MainFlowCoordinator::DidActivate, void, GlobalNamespace::MainFlowCoordinator* self, bool firstActivation, bool addedToHierarchy, bool screenSystemEnabling) {
    MainFlowCoordinator_DidActivate(self, firstActivation, addedToHierarchy, screenSystemEnabling);
    DEBUG("MainFlowCoordinator_DidActivate");

}

bool firstTimeInit = false;

MAKE_HOOK_MATCH(AlwaysVisibleQuad_OnEnable, &GlobalNamespace::AlwaysVisibleQuad::OnEnable, void, GlobalNamespace::AlwaysVisibleQuad* self) {
    auto gameObject = self->get_gameObject();
    DEBUG("AlwaysVisibleQuad_OnEnable");
    gameObject->SetActive(!getBSMRConfig().PassthroughEnabled.GetValue());
}

MAKE_HOOK_MATCH(MainCamera_Awake, &GlobalNamespace::MainCamera::Awake, void, GlobalNamespace::MainCamera* self) {
    MainCamera_Awake(self);
    DEBUG("MainCamera_Awake");

    if (!firstTimeInit) {
        auto ovrManagerGO = UnityEngine::GameObject::New_ctor("OVRManager");
        UnityEngine::Object::DontDestroyOnLoad(ovrManagerGO);
        ovrManagerGO->SetActive(false);
        auto ovrManager = ovrManagerGO->AddComponent<GlobalNamespace::OVRManager*>();
        ovrManager->___useRecommendedMSAALevel = false;
        ovrManager->___isInsightPassthroughEnabled = true;
        ovrManager->set_trackingOriginType(GlobalNamespace::__OVRManager__TrackingOrigin::FloorLevel);
        ovrManagerGO->SetActive(true);
        firstTimeInit = true;
        DEBUG("Initialized OVRManager!");
    }

    auto mainCamera = self->get_camera();
    auto mainCameraGO = mainCamera->get_gameObject();

    auto backgroundColor = UnityEngine::Color(
        0.0f,
        0.0f,
        0.0f,
        0.0f
    );

    mainCamera->set_clearFlags(UnityEngine::CameraClearFlags::SolidColor);
    mainCamera->set_backgroundColor(backgroundColor);

    auto ovrPassthroughLayer = mainCameraGO->AddComponent<GlobalNamespace::OVRPassthroughLayer*>();
    ovrPassthroughLayer->___overlayType = GlobalNamespace::OVROverlay::OverlayType::Underlay;
    ovrPassthroughLayer->set_textureOpacity(getBSMRConfig().PassthroughOpacity.GetValue());
}


// Called later on in the game loading - a good time to install function hooks
GT_EXPORT_FUNC void load() {
    INFO("Loading BSMR...");
    il2cpp_functions::Init();
    custom_types::Register::AutoRegister();
    BSML::Init();

    

    INFO("Installing hooks...");
    // INSTALL_HOOK(Logger, GameplayCoreInstaller_InstallBindings);
    // INSTALL_HOOK(Logger, MainFlowCoordinator_DidActivate);
    INSTALL_HOOK(Logger, AlwaysVisibleQuad_OnEnable);
    INSTALL_HOOK(Logger, MainCamera_Awake);

    INFO("Installed all hooks!");
    BSML::Register::RegisterMainMenu<BSMR::UI::BSMRFlowCoordinator*>("<color=#D1ACFF>Mixed Reality", "Mix your reality");
    INFO("BSMR loaded!");
}