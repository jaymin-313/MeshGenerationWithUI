// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ProceduralMeshComponent.h"
#include "VerticalRailActor.generated.h"


UENUM(BlueprintType)
enum class EMeshType : uint8
{
	Sphere UMETA(DisplayName = "Sphere"),
	Cone UMETA(DisplayName = "Cone"),
	Cylinder UMETA(DisplayName = "Cylinder"),
	Oval UMETA(DisplayName = "Oval"),
	Pyramid UMETA(DisplayName = "Pyramid"),
	SphereWithBase UMETA(DisplayName = "Sphere with Base"),
	CubeWithBase UMETA(DisplayName = "Cube WIth Base")
};
UCLASS()
class ASSIGNMENT_5_6_API AVerticalRailActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AVerticalRailActor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	virtual void OnConstruction(const FTransform& Transform) override;
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	void SetMaterial(int Type, UMaterialInterface* Material);

	UPROPERTY(VisibleAnywhere,BlueprintReadOnly,Category = "Vertical Rail")
	UProceduralMeshComponent* ProceduralMesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Vertical Rail")
	TEnumAsByte<EMeshType> MeshType;

	UFUNCTION(BlueprintCallable, Category = "Vertical Rail")
	void CreateMesh(EMeshType MeshType2);

	void CreateMeshType1();
	void CreateMeshType2();
	void CreateMeshType3();
private:
	


	void CreateMeshType4();

	void CreateMeshType5();

	void CreateMeshType6();

	

	void CreateMeshType7();

	void CreateBox(float Width, float Height, float Depth, TArray<FVector>& Vertices, TArray<int32>& Triangles, TArray<FVector>& Normals, TArray<FVector2D>& UVs, TArray<FProcMeshTangent>& Tangents, FVector Offset);

	void CreateRailMesh(float Width, float Height, float Depth, const FVector& Offset,int SectionIndex);

	void GenerateSphere(float Radius, int32 Segments, int32 Rings, int SectionIndex = 1);

	void CreateCone(float Height, float Radius, int32 NumSegments, TArray<FVector>& Vertices, TArray<int32>& Triangles, TArray<FVector>& Normals, TArray<FVector2D>& UVs, TArray<FProcMeshTangent>& Tangents, FVector Offset);

	void CreateOval(float Height, float Width, float Depth, int32 NumSegments, TArray<FVector>& Vertices, TArray<int32>& Triangles, TArray<FVector>& Normals, TArray<FVector2D>& UVs, TArray<FProcMeshTangent>& Tangents, FVector Offset);

	void CreateCylinder(float Radius, float Height, int32 Segments, TArray<FVector>& Vertices, TArray<int32>& Triangles, TArray<FVector>& Normals, TArray<FVector2D>& UVs, TArray<FProcMeshTangent>& Tangents, FVector Offset);

	void CreatePyramid(float BaseWidth, float Height, TArray<FVector>& Vertices, TArray<int32>& Triangles, TArray<FVector>& Normals, TArray<FVector2D>& UVs, TArray<FProcMeshTangent>& Tangents, FVector Offset);

	void GenerateStand(FVector Offset, float BaseRadius, float Height1, float RimRadius, float CurvatureFactor, int NumSlices, int NumStacks, int SectionIndex);

	
};
