// TreeTrunkStudios (c) 2023


// 
#pragma once


//
#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Engine/TextureRenderTarget2D.h"
#include "Components/SceneCaptureComponent2D.h"
#include "Components/PointLightComponent.h"
#include "UnrealClient.h"
#include "RenderInventoryItemExamineMesh.generated.h"


// 
UCLASS()
class GENERICINVENTORYSYSTEM_API ARenderInventoryItemExamineMesh : public AActor {
	GENERATED_BODY()

//
protected:

	//
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	UStaticMeshComponent * VisualMesh;

	//
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	USceneCaptureComponent2D * VisualMeshCapture;

	//
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	UPointLightComponent * MeshLighting;

	//
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	UTextureRenderTarget2D * VisualMeshRenderTarget;

		
// 
public:
	
	// Sets default values for this actor's properties
	ARenderInventoryItemExamineMesh();

	//
	UTextureRenderTarget2D * Setup(UStaticMesh * GivenStaticMesh);

	//
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;


//
public:

	//
	UFUNCTION(BlueprintCallable)
	void AddRotation(const FRotator & GivenRotationAdditive);

	//
	UFUNCTION(BlueprintCallable)
	void AddZoom(const float & GivenZoomAdditive);

	//
	void HandleViewportResolutionChange(FViewport * ChangingViewport, uint32 Unknown);
};
