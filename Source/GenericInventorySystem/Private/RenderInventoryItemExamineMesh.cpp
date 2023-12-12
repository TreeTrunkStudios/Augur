// TreeTrunkStudios (c) 2023


// 
#include "RenderInventoryItemExamineMesh.h"
#include <Kismet/KismetMathLibrary.h>
#include <Components/ArrowComponent.h>


// Sets default values
ARenderInventoryItemExamineMesh::ARenderInventoryItemExamineMesh() {
	
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
	PrimaryActorTick.bRunOnAnyThread = true;
	PrimaryActorTick.bTickEvenWhenPaused = false;
	PrimaryActorTick.bStartWithTickEnabled = false;

	// Ensure that our root component is a valid arrow component to allow all other assets to be disjointed
	SetRootComponent(CreateDefaultSubobject<UArrowComponent>(TEXT("RootArrowComponent")));

	// Create a default static mesh component for base functionality
	VisualMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("VisualMesh"));
	//VisualMesh->Mobility = EComponentMobility::Stationary;
	VisualMesh->SetSimulatePhysics(false);
	VisualMesh->SetupAttachment(RootComponent);
	//VisualMesh->AttachToComponent(RootComponent, FAttachmentTransformRules::SnapToTargetNotIncludingScale);
	//VisualMesh->SetRelativeTransform(FTransform::Identity);

	//
	MeshLighting = CreateDefaultSubobject<UPointLightComponent>(TEXT("MeshLighting"));
	MeshLighting->SetupAttachment(RootComponent);

	//
	MeshLighting->IntensityUnits = ELightUnits::Lumens;
	MeshLighting->Intensity = 20.0f;
	MeshLighting->AttenuationRadius = 1024.0f;
	MeshLighting->SourceRadius = 32.0f;
	MeshLighting->SoftSourceRadius = 32.0f;
	MeshLighting->bUseTemperature = true;
	MeshLighting->Temperature = 6500.0f;
	
	// Create a default static mesh component for base functionality
	VisualMeshCapture = CreateDefaultSubobject<USceneCaptureComponent2D>(TEXT("VisualMeshCapture"));
	VisualMeshCapture->SetupAttachment(RootComponent);

	// Initialize our new scene capture component to only update when we tell it to, and to only render our static mesh component
	VisualMeshCapture->PrimitiveRenderMode = ESceneCapturePrimitiveRenderMode::PRM_UseShowOnlyList;
	VisualMeshCapture->bCaptureEveryFrame = true;
	VisualMeshCapture->bCaptureOnMovement = true;
	VisualMeshCapture->bUseRayTracingIfEnabled = true;
	VisualMeshCapture->ClearShowOnlyComponents();
	VisualMeshCapture->ShowOnlyComponents.AddUnique(VisualMesh);
	VisualMeshCapture->CompositeMode = SCCM_Overwrite;

	// Finally, set our mesh capture information for the source, as well as the render target to utilize
	VisualMeshCapture->CaptureSource = SCS_SceneColorHDR;
	VisualMeshCapture->bConsiderUnrenderedOpaquePixelAsFullyTranslucent = true;

	//
	FEngineShowFlags VisualMeshShowFlags{ESFIM_Game}; //ESFIM_All0

	//
	VisualMeshShowFlags.Atmosphere = false;
	VisualMeshShowFlags.AmbientOcclusion = true;
	VisualMeshShowFlags.AntiAliasing = true;
	VisualMeshShowFlags.AmbientCubemap = false;
	VisualMeshShowFlags.AudioRadius = false;
	VisualMeshShowFlags.Bounds = false;
	VisualMeshShowFlags.Bloom = false;
	VisualMeshShowFlags.Bones = false;
	VisualMeshShowFlags.Brushes = false;
	VisualMeshShowFlags.BillboardSprites = false;
	VisualMeshShowFlags.BuilderBrush = false;
	VisualMeshShowFlags.BSP = false;
	VisualMeshShowFlags.BSPSplit = false;
	VisualMeshShowFlags.BSPTriangles = false;
	VisualMeshShowFlags.CameraFrustums = false;
	VisualMeshShowFlags.CameraImperfections = false;
	VisualMeshShowFlags.ColorGrading = true;
	VisualMeshShowFlags.ContactShadows = true;
	VisualMeshShowFlags.CameraSafeFrames = false;
	VisualMeshShowFlags.Cloud = false;
	VisualMeshShowFlags.Collision = false;
	VisualMeshShowFlags.Constraints = false;
	VisualMeshShowFlags.Cover = false;
	VisualMeshShowFlags.CameraInterpolation = false;
	VisualMeshShowFlags.CapsuleShadows = true;
	VisualMeshShowFlags.CameraAspectRatioBars = false;
	VisualMeshShowFlags.CollisionPawn = false;
	VisualMeshShowFlags.CameraAspectRatioBars = false;
	VisualMeshShowFlags.CollisionVisibility = false;
	//VisualMeshShowFlags.CustomShowFlags = false;
	VisualMeshShowFlags.DeferredLighting = true;
	VisualMeshShowFlags.DirectLighting = true;
	VisualMeshShowFlags.DynamicShadows = true;
	VisualMeshShowFlags.DistanceFieldAO = true;
	VisualMeshShowFlags.Decals = false;
	VisualMeshShowFlags.Diffuse = true;
	VisualMeshShowFlags.DirectionalLights = false;
	VisualMeshShowFlags.DepthOfField = false;
	VisualMeshShowFlags.DistanceCulledPrimitives = false;
	VisualMeshShowFlags.EyeAdaptation = false;
	VisualMeshShowFlags.Fog = false;
	VisualMeshShowFlags.ForceFeedbackRadius = false;
	VisualMeshShowFlags.GlobalIllumination = true;
	VisualMeshShowFlags.Game = true;
	VisualMeshShowFlags.Grain = false;
	VisualMeshShowFlags.Grid = false;
	VisualMeshShowFlags.HeterogeneousVolumes = true;
	VisualMeshShowFlags.HitProxies = false;
	VisualMeshShowFlags.HighResScreenshotMask = false;

	// 
	VisualMeshCapture->ShowFlags = VisualMeshShowFlags;
}


// 
UTextureRenderTarget2D * ARenderInventoryItemExamineMesh::Setup(UStaticMesh * GivenStaticMesh) {

	//
	if (IsValid(VisualMeshRenderTarget) == false) {

		//
		VisualMeshRenderTarget = NewObject<UTextureRenderTarget2D>();

		//
		FViewport::ViewportResizedEvent.RemoveAll(this);
		FViewport::ViewportResizedEvent.AddUObject(this, &ARenderInventoryItemExamineMesh::HandleViewportResolutionChange);

		// 
		FVector2D ViewportResolution;
		GEngine->GameViewport->GetViewportSize(ViewportResolution);

		// Ensure that our aspect ratio stays within the 16:9 or less aspect ratio
		ViewportResolution.X = FMath::Min(ViewportResolution.X, (ViewportResolution.Y * 1.7777777777777777777777777777778));

		// Initialize our new render target's information and update it accordingly
		VisualMeshRenderTarget->InitCustomFormat(ViewportResolution.X, ViewportResolution.Y, EPixelFormat::PF_A2B10G10R10, false);
		VisualMeshRenderTarget->MipLoadOptions = ETextureMipLoadOptions::OnlyFirstMip;
		VisualMeshRenderTarget->MipsSamplerFilter = TextureFilter::TF_Trilinear;
		VisualMeshRenderTarget->ClearColor = FLinearColor::Transparent;
		VisualMeshRenderTarget->bAutoGenerateMips = false;
		VisualMeshRenderTarget->RenderTargetFormat = ETextureRenderTargetFormat::RTF_RGB10A2;
		VisualMeshRenderTarget->UpdateResourceImmediate();

		//
		VisualMeshCapture->TextureTarget = VisualMeshRenderTarget;
	}

	// Set the static mesh to the given static mesh
	VisualMesh->SetStaticMesh(GivenStaticMesh);

	//
	VisualMeshCapture->ShowOnlyComponents.AddUnique(VisualMesh);

	//
	const double MaxExtent = (GivenStaticMesh->GetExtendedBounds().SphereRadius * VisualMesh->BoundsScale);

	// 
	VisualMeshCapture->SetRelativeLocation(FVector(MaxExtent * 4.0, 0.0, 0.0));
	VisualMeshCapture->SetRelativeRotation(UKismetMathLibrary::MakeRotFromX(FVector(MaxExtent * -4.0, 0.0, 0.0)));

	//
	MeshLighting->SetSourceRadius(VisualMeshCapture->GetRelativeLocation().X);
	MeshLighting->SetSoftSourceRadius(VisualMeshCapture->GetRelativeLocation().X);
	MeshLighting->SetIntensity((20.0f * FMath::Square(VisualMeshCapture->GetRelativeLocation().X / (MaxExtent * 4.0))));
	MeshLighting->SetRelativeLocation(VisualMeshCapture->GetRelativeLocation());

	// Capture the current scene to render it to the render target
	VisualMeshCapture->CaptureScene();

	// Finally, return a pointer to our 2D render target for use within the UI
	return VisualMeshRenderTarget;
}


// 
void ARenderInventoryItemExamineMesh::EndPlay(const EEndPlayReason::Type EndPlayReason) {
	Super::EndPlay(EndPlayReason);
	FViewport::ViewportResizedEvent.RemoveAll(this);
}


// 
void ARenderInventoryItemExamineMesh::AddRotation(const FRotator & GivenRotationAdditive) {

	// Add the given rotation to our current static mesh's relative rotation
	VisualMesh->AddRelativeRotation(GivenRotationAdditive);

	// Re-render the scene to update the render target with the new rotation
	VisualMeshCapture->CaptureScene();
}


// 
void ARenderInventoryItemExamineMesh::AddZoom(const float & GivenZoomAdditive) {

	//
	const double MaxExtent = (VisualMesh->GetStaticMesh()->GetExtendedBounds().SphereRadius * VisualMesh->BoundsScale);

	//
	const FVector LocalVector = VisualMeshCapture->GetRelativeLocation();

	//
	VisualMeshCapture->SetRelativeLocation(FVector(FMath::Max(MaxExtent * 2.0, FMath::Min(MaxExtent * 8.0, LocalVector.X + GivenZoomAdditive)), LocalVector.Y, LocalVector.Z));

	//
	MeshLighting->SetSourceRadius(VisualMeshCapture->GetRelativeLocation().X);
	MeshLighting->SetSoftSourceRadius(VisualMeshCapture->GetRelativeLocation().X);
	MeshLighting->SetIntensity((20.0f * FMath::Square(VisualMeshCapture->GetRelativeLocation().X / (MaxExtent * 4.0))));
	MeshLighting->SetRelativeLocation(VisualMeshCapture->GetRelativeLocation());

	// Re-render the scene to update the render target with the new rotation
	VisualMeshCapture->CaptureScene();
}


// 
void ARenderInventoryItemExamineMesh::HandleViewportResolutionChange(FViewport * ChangingViewport, uint32 Unknown) {

	// 
	FVector2D ViewportResolution;
	GEngine->GameViewport->GetViewportSize(ViewportResolution);

	// Ensure that our aspect ratio stays within the 16:9 or less aspect ratio
	ViewportResolution.X = FMath::Min(ViewportResolution.X, (ViewportResolution.Y * 1.7777777777777777777777777777778));

	// Initialize our new render target's information and update it accordingly
	VisualMeshRenderTarget->InitCustomFormat(ViewportResolution.X, ViewportResolution.Y, EPixelFormat::PF_A2B10G10R10, false);
	VisualMeshRenderTarget->MipLoadOptions = ETextureMipLoadOptions::OnlyFirstMip;
	VisualMeshRenderTarget->MipsSamplerFilter = TextureFilter::TF_Trilinear;
	VisualMeshRenderTarget->ClearColor = FLinearColor::Transparent;
	VisualMeshRenderTarget->bAutoGenerateMips = false;
	VisualMeshRenderTarget->RenderTargetFormat = ETextureRenderTargetFormat::RTF_RGB10A2;
	VisualMeshRenderTarget->UpdateResourceImmediate();

	// Recapture the current scene to render it to the render target at the new resolution
	VisualMeshCapture->CaptureScene();
}
