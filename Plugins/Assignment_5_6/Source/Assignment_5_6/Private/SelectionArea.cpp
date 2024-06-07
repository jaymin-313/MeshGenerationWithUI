// Fill out your copyright notice in the Description page of Project Settings.


#include "SelectionArea.h"

// Sets default values
ASelectionArea::ASelectionArea()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
    ProceduralMesh = CreateDefaultSubobject<UProceduralMeshComponent>("ProceduralMesh");
    SetRootComponent(ProceduralMesh);

    bIsSphere = false;
    Radius = 400.0f;
    Dimensions = FVector(100.0f, 100.0f, 100.0f);
    // Load translucent material
    static ConstructorHelpers::FObjectFinder<UMaterial> Material(TEXT("/Script/Engine.Material'/Engine/ArtTools/RenderToTexture/Materials/M_RadiusDebugTranslucent.M_RadiusDebugTransLucent'"));
    if (Material.Succeeded())
    {
        TranslucentMaterial = Material.Object;
    }
}

void ASelectionArea::ToggleShape()
{
    if (bIsSphere) {
        bIsSphere = false;
        CreateBox(100.0f);
    }
    else {
        bIsSphere = true;
        GenerateSphere(100, 32, 32, 0);
    }
}

// Called when the game starts or when spawned
void ASelectionArea::BeginPlay()
{
	Super::BeginPlay();
    // Enable cursor
    /*APlayerController* PlayerController = GetWorld()->GetFirstPlayerController();
    if (PlayerController)
    {
        PlayerController->bShowMouseCursor = true;
        PlayerController->bEnableClickEvents = true;
        PlayerController->bEnableMouseOverEvents = true;
    }*/
    //GenerateSphere(Radius,32,32,0);
    CreateBox(100.0f);
    //ToggleShape();
}

// Called every frame
void ASelectionArea::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
   

   
}
float ASelectionArea::GetRadius() {
    return Radius;
}
bool ASelectionArea::IsSphere() {
    return bIsSphere;

}
void ASelectionArea::GenerateSphere(float InRadius, int32 Segments, int32 Rings, int SectionIndex)
{

    TArray<FVector> Vertices;
    TArray<int32> Triangles;
    TArray<FVector> Normals;
    TArray<FLinearColor> Colors;
    TArray<FVector2D> UVs;
    TArray<FProcMeshTangent> Tangents;


    Vertices.Empty();
    Triangles.Empty();
    Normals.Empty();
    UVs.Empty();
    Tangents.Empty();
    FVector Offset = FVector(0, 0, 0);

    for (int32 RingNumber = 0; RingNumber <= Rings; RingNumber++)
    {
        for (int32 SegmentNumber = 0; SegmentNumber <= Segments; SegmentNumber++)
        {
            float Theta = 2.0f * PI * (float)SegmentNumber / (float)Segments;
            float Phi = PI * (float)RingNumber / (float)Rings;

            float X = FMath::Sin(Phi) * FMath::Cos(Theta);
            float Y = FMath::Sin(Phi) * FMath::Sin(Theta);
            float Z = FMath::Cos(Phi);

            FVector Vertex = FVector(X, Y, Z) * InRadius + Offset;
            Vertices.Add(Vertex);

            FVector Normal = Vertex.GetSafeNormal();
            Normals.Add(Normal);

            FVector2D UV = FVector2D((float)SegmentNumber / (float)Segments, (float)RingNumber / (float)Rings);
            UVs.Add(UV);

            FProcMeshTangent Tangent = FProcMeshTangent(1.f, 0.f, 0.f);
            Tangents.Add(Tangent);
        }
    }

    for (int32 RingNumber = 0; RingNumber < Rings; RingNumber++)
    {
        for (int32 SegmentNumber = 0; SegmentNumber < Segments; SegmentNumber++)
        {
            int32 FirstVertex = (RingNumber * (Segments + 1)) + SegmentNumber;
            int32 SecondVertex = FirstVertex + Segments + 1;

            Triangles.Add(FirstVertex);
            Triangles.Add(FirstVertex + 1);
            Triangles.Add(SecondVertex);

            Triangles.Add(SecondVertex);
            Triangles.Add(FirstVertex + 1);
            Triangles.Add(SecondVertex + 1);
        }
    }

    ProceduralMesh->CreateMeshSection_LinearColor(SectionIndex, Vertices, Triangles, Normals, UVs, Colors, Tangents, true);
    ProceduralMesh->SetMaterial(0, TranslucentMaterial);
}

void ASelectionArea::CreateBox(float Dimension)
{
    ProceduralMesh->ClearAllMeshSections();
    TArray<FVector> Vertices;
    TArray<int32> Triangles;
    TArray<FVector> Normals;
    TArray<FVector2D> UVs;
    TArray<FLinearColor> Colors;
    TArray<FProcMeshTangent> Tangents;
    FVector Offset = FVector(0,0,0);
    Vertices.Empty();
    Triangles.Empty();
    Normals.Empty();
    UVs.Empty();
    Tangents.Empty();
    float Width = Dimension;
    float Depth = Dimension;
    float Height = Dimension;
    // Vertices for the box
    Vertices = {
        Offset + FVector(-Width / 2, -Depth / 2, 0), Offset + FVector(-Width / 2, Depth / 2, 0), Offset + FVector(Width / 2, Depth / 2, 0), Offset + FVector(Width / 2, -Depth / 2, 0), // -Z
        Offset + FVector(-Width / 2, -Depth / 2, Height), Offset + FVector(Width / 2, -Depth / 2, Height), Offset + FVector(Width / 2, Depth / 2, Height), Offset + FVector(-Width / 2, Depth / 2, Height), // +Z
        Offset + FVector(-Width / 2, -Depth / 2, 0), Offset + FVector(Width / 2, -Depth / 2, 0), Offset + FVector(Width / 2, -Depth / 2, Height), Offset + FVector(-Width / 2, -Depth / 2, Height), // -Y
        Offset + FVector(-Width / 2, Depth / 2, 0), Offset + FVector(-Width / 2, Depth / 2, Height), Offset + FVector(Width / 2, Depth / 2, Height), Offset + FVector(Width / 2, Depth / 2, 0), // +Y
        Offset + FVector(-Width / 2, -Depth / 2, 0), Offset + FVector(-Width / 2, -Depth / 2, Height), Offset + FVector(-Width / 2, Depth / 2, Height), Offset + FVector(-Width / 2, Depth / 2, 0), // -X
        Offset + FVector(Width / 2, -Depth / 2, 0), Offset + FVector(Width / 2, Depth / 2, 0), Offset + FVector(Width / 2, Depth / 2, Height), Offset + FVector(Width / 2, -Depth / 2, Height) // +X
    };

    Triangles = {
        0, 2, 1, 0, 3, 2, // -Z
        4, 6, 5, 4, 7, 6, // +Z
        8, 10, 9, 8, 11, 10, // -Y
        12, 14, 13, 12, 15, 14, // +Y
        16, 18, 17, 16, 19, 18, // -X
        20, 22, 21, 20, 23, 22 // +X
    };


    Normals = {
        FVector(0, 0, -1), FVector(0, 0, -1), FVector(0, 0, -1), FVector(0, 0, -1), // -Z
        FVector(0, 0, 1), FVector(0, 0, 1), FVector(0, 0, 1), FVector(0, 0, 1), // +Z
        FVector(0, -1, 0), FVector(0, -1, 0), FVector(0, -1, 0), FVector(0, -1, 0), // -Y
        FVector(0, 1, 0), FVector(0, 1, 0), FVector(0, 1, 0), FVector(0, 1, 0), // +Y
        FVector(-1, 0, 0), FVector(-1, 0, 0), FVector(-1, 0, 0), FVector(-1, 0, 0), // -X
        FVector(1, 0, 0), FVector(1, 0, 0), FVector(1, 0, 0), FVector(1, 0, 0) // +X
    };


    UVs = {
        FVector2D(0, 0), FVector2D(0, 1), FVector2D(1, 1), FVector2D(1, 0), // -Z
        FVector2D(0, 0), FVector2D(1, 0), FVector2D(1, 1), FVector2D(0, 1), // +Z
        FVector2D(0, 1), FVector2D(1, 1), FVector2D(1, 0), FVector2D(0, 0), // -Y
        FVector2D(0, 0), FVector2D(0, 1), FVector2D(1, 1), FVector2D(1, 0), // +Y
        FVector2D(1, 0), FVector2D(1, 1), FVector2D(0, 1), FVector2D(0, 0), // -X
        FVector2D(0, 0), FVector2D(1, 0), FVector2D(1, 1), FVector2D(0, 1)  // +X
    };

    Tangents.Init(FProcMeshTangent(1.f, 0.f, 0.f), 24);

    ProceduralMesh->CreateMeshSection_LinearColor(0,Vertices, Triangles, Normals, UVs, Colors, Tangents, true);
    ProceduralMesh->SetMaterial(0, TranslucentMaterial);
}

FVector ASelectionArea::GetDimensions() const
{
    return Dimensions;
}
