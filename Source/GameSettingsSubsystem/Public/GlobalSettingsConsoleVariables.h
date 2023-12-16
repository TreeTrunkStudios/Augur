// TreeTrunkStudios (c) 2023


// 
#pragma once


//
static TAutoConsoleVariable<int32> CVarGlobalSettingsLevel{
	TEXT("Global.SettingsLevel"),
	0,
	TEXT("Setting which handles all settings combined into one, allowing changing every single setting (other than the subjective ones) all at once to a specified level of quality."),
	ECVF_Default
};


//
static TAutoConsoleVariable<int32> CVarAudioSettingsLevel{
	TEXT("Global.Audio.SettingsLevel"),
	0,
	TEXT("Setting which handles all audio settings combined into one, allowing changing every single setting (other than the subjective ones) all at once to a specified level of quality."),
	ECVF_Default
};


//
static TAutoConsoleVariable<int32> CVarDisplaySettingsLevel{
	TEXT("Global.Display.SettingsLevel"),
	0,
	TEXT("Setting which handles all audio settings combined into one, allowing changing every single setting (other than the subjective ones) all at once to a specified level of quality."),
	ECVF_Default
};


//
static TAutoConsoleVariable<int32> CVarGraphicsSettingsLevel{
	TEXT("Global.Graphics.SettingsLevel"),
	0,
	TEXT("Setting which handles all graphics settings combined into one, allowing changing every single setting (other than the subjective ones) all at once to a specified level of quality."),
	ECVF_Default
};


//
static TAutoConsoleVariable<int32> CVarGraphicsShadowsSettingsLevel{
	TEXT("Global.Graphics.Shadows.SettingsLevel"),
	0,
	TEXT("Setting which handles all shadow graphics settings combined into one, allowing changing every single setting (other than the subjective ones) all at once to a specified level of quality."),
	ECVF_Default
};


//
static TAutoConsoleVariable<int32> CVarGraphicsMaterialsSettingsLevel{
	TEXT("Global.Graphics.Materials.SettingsLevel"),
	0,
	TEXT("Setting which handles all shadow graphics settings combined into one, allowing changing every single setting (other than the subjective ones) all at once to a specified level of quality."),
	ECVF_Default
};


//
static TAutoConsoleVariable<int32> CVarGraphicsEnvironmentSettingsLevel{
	TEXT("Global.Graphics.Environment.SettingsLevel"),
	0,
	TEXT("Setting which handles all shadow graphics settings combined into one, allowing changing every single setting (other than the subjective ones) all at once to a specified level of quality."),
	ECVF_Default
};


//
static TAutoConsoleVariable<int32> CVarGraphicsSkeletalSettingsLevel{
	TEXT("Global.Graphics.Skeletal.SettingsLevel"),
	0,
	TEXT("Setting which handles all shadow graphics settings combined into one, allowing changing every single setting (other than the subjective ones) all at once to a specified level of quality."),
	ECVF_Default
};


//
static TAutoConsoleVariable<int32> CVarGraphicsShadingSettingsLevel{
	TEXT("Global.Graphics.Shading.SettingsLevel"),
	0,
	TEXT("Setting which handles all shadow graphics settings combined into one, allowing changing every single setting (other than the subjective ones) all at once to a specified level of quality."),
	ECVF_Default
};


//
static TAutoConsoleVariable<int32> CVarGraphicsFoliageSettingsLevel{
	TEXT("Global.Graphics.Foliage.SettingsLevel"),
	0,
	TEXT("Setting which handles all shadow graphics settings combined into one, allowing changing every single setting (other than the subjective ones) all at once to a specified level of quality."),
	ECVF_Default
};


//
static TAutoConsoleVariable<int32> CVarGraphicsEffectsSettingsLevel{
	TEXT("Global.Graphics.Effects.SettingsLevel"),
	0,
	TEXT("Setting which handles all shadow graphics settings combined into one, allowing changing every single setting (other than the subjective ones) all at once to a specified level of quality."),
	ECVF_Default
};


//
static TAutoConsoleVariable<int32> CVarGraphicsPostProcessingSettingsLevel{
	TEXT("Global.Graphics.PostProcessing.SettingsLevel"),
	0,
	TEXT("Setting which handles all shadow graphics settings combined into one, allowing changing every single setting (other than the subjective ones) all at once to a specified level of quality."),
	ECVF_Default
};


//
static TAutoConsoleVariable<int32> CVarGraphicsReflectionsSettingsLevel{
	TEXT("Global.Graphics.Reflections.SettingsLevel"),
	0,
	TEXT("Setting which handles all shadow graphics settings combined into one, allowing changing every single setting (other than the subjective ones) all at once to a specified level of quality."),
	ECVF_Default
};


//
static TAutoConsoleVariable<int32> CVarGraphicsTranslucencySettingsLevel{
	TEXT("Global.Graphics.Translucency.SettingsLevel"),
	0,
	TEXT("Setting which handles all shadow graphics settings combined into one, allowing changing every single setting (other than the subjective ones) all at once to a specified level of quality."),
	ECVF_Default
};


//
static TAutoConsoleVariable<int32> CVarGraphicsGlobalIlluminationSettingsLevel{
	TEXT("Global.Graphics.GlobalIllumination.SettingsLevel"),
	0,
	TEXT("Setting which handles all shadow graphics settings combined into one, allowing changing every single setting (other than the subjective ones) all at once to a specified level of quality."),
	ECVF_Default
};


//
static TAutoConsoleVariable<int32> CVarGraphicsAmbientOcclusionSettingsLevel{
	TEXT("Global.Graphics.AmbientOcclusion.SettingsLevel"),
	0,
	TEXT("Setting which handles all shadow graphics settings combined into one, allowing changing every single setting (other than the subjective ones) all at once to a specified level of quality."),
	ECVF_Default
};

/*
//
static TAutoConsoleVariable<int32> CVarLumenSettingsLevel{
	TEXT("Global.Lumen.SettingsLevel"),
	0,
	TEXT("Setting which handles all graphics settings combined into one, allowing changing every single setting (other than the subjective ones) all at once to a specified level of quality."),
	ECVF_Default
};


//// TODO: Potentially unnecessary and should be removed
//
static TAutoConsoleVariable<int32> CVarRayTracingSettingsLevel{
	TEXT("Global.RayTracing.SettingsLevel"),
	0,
	TEXT("Setting which handles all graphics settings combined into one, allowing changing every single setting (other than the subjective ones) all at once to a specified level of quality."),
	ECVF_Default
};


//
static TAutoConsoleVariable<int32> CVarRayTracingShadowsSettingsLevel{
	TEXT("Global.RayTracing.Shadows.SettingsLevel"),
	0,
	TEXT("Setting which handles all graphics settings combined into one, allowing changing every single setting (other than the subjective ones) all at once to a specified level of quality."),
	ECVF_Default
};


//
static TAutoConsoleVariable<int32> CVarRayTracingReflectionsSettingsLevel{
	TEXT("Global.RayTracing.Reflections.SettingsLevel"),
	0,
	TEXT("Setting which handles all graphics settings combined into one, allowing changing every single setting (other than the subjective ones) all at once to a specified level of quality."),
	ECVF_Default
};


//
static TAutoConsoleVariable<int32> CVarRayTracingTranslucencySettingsLevel{
	TEXT("Global.RayTracing.Translucency.SettingsLevel"),
	0,
	TEXT("Setting which handles all graphics settings combined into one, allowing changing every single setting (other than the subjective ones) all at once to a specified level of quality."),
	ECVF_Default
};


//
static TAutoConsoleVariable<int32> CVarRayTracingGlobalIlluminationSettingsLevel{
	TEXT("Global.RayTracing.GlobalIllumination.SettingsLevel"),
	0,
	TEXT("Setting which handles all graphics settings combined into one, allowing changing every single setting (other than the subjective ones) all at once to a specified level of quality."),
	ECVF_Default
};


//
static TAutoConsoleVariable<int32> CVarRayTracingAmbientOcclusionSettingsLevel{
	TEXT("Global.RayTracing.AmbientOcclusion.SettingsLevel"),
	0,
	TEXT("Setting which handles all graphics settings combined into one, allowing changing every single setting (other than the subjective ones) all at once to a specified level of quality."),
	ECVF_Default
};*/
