// Fill out your copyright notice in the Description page of Project Settings.


#include "VerticalRailActor.h"

// Sets default values
AVerticalRailActor::AVerticalRailActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	ProceduralMesh = CreateDefaultSubobject<UProceduralMeshComponent>(TEXT("ProceduralMesh"));
	RootComponent = ProceduralMesh;
}

// Called when the game starts or when spawned
void AVerticalRailActor::BeginPlay()
{
	Super::BeginPlay();
	
}

void AVerticalRailActor::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);
	CreateMesh(MeshType);
}

// Called every frame
void AVerticalRailActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}
void AVerticalRailActor::SetMaterial( int Type, UMaterialInterface* Material)
{
    if (ProceduralMesh)
    {
        ProceduralMesh->SetMaterial(Type, Material);
        ProceduralMesh->SetMaterial(3, Material);
        ProceduralMesh->SetMaterial(4, Material);
      

    }
}
FVector CalculateNormal(const FVector& V1, const FVector& V2, const FVector& V3)
{
    return FVector::CrossProduct(V2 - V1, V3 - V1).GetSafeNormal();
}

FProcMeshTangent CalculateTangent(const FVector& V1, const FVector& V2, const FVector& V3)
{
    FVector Edge1 = V2 - V1;
    FVector Edge2 = V3 - V1;
    FVector Normal = CalculateNormal(V1, V2, V3);

    FVector Tangent = (Edge1 - (Normal * FVector::DotProduct(Normal, Edge1))).GetSafeNormal();
    return FProcMeshTangent(Tangent, true);
}
void AVerticalRailActor::CreateMesh(EMeshType InMeshType)
{
    switch (InMeshType)
    {
    case EMeshType::Sphere:
        CreateMeshType1();
        break;
    case EMeshType::Cone:
        CreateMeshType2();
        break;
    case EMeshType::Cylinder:
        CreateMeshType3();
        break;
    case EMeshType::Oval:
        CreateMeshType4();
        break;
    case EMeshType::Pyramid:
        CreateMeshType5();
        break;
    case EMeshType::SphereWithBase:
        CreateMeshType6();
        break;
    case EMeshType::CubeWithBase:
        CreateMeshType7();
        break;
    default:
        UE_LOG(LogTemp, Warning, TEXT("Invalid MeshType specified"));
        break;
    }
}

void AVerticalRailActor::CreateMeshType1()
{
    ProceduralMesh->ClearAllMeshSections();
    TArray<FVector> Vertices;
    TArray<int32> Triangles;
    TArray<FVector> Normals;
    TArray<FVector2D> UVs;
    TArray<FProcMeshTangent> Tangents;

    FVector Offset(0, 0, 0);

    CreateBox(10, 100, 10, Vertices, Triangles, Normals, UVs, Tangents, Offset);

    Offset.Z += 25;
    CreateRailMesh(100, 10, 12,Offset,1);
    Offset.Z += 40;
    CreateRailMesh(100, 10, 12 ,Offset,2);

    Offset.Z += 35+5;
    //Offset.Z += 105;
    GenerateSphere(5, 16, 16,3);

    // Update the procedural mesh component with the generated data
    ProceduralMesh->CreateMeshSection_LinearColor(0, Vertices, Triangles, Normals, UVs, TArray<FLinearColor>(), Tangents, true);
}

void AVerticalRailActor::CreateMeshType2()
{
    ProceduralMesh->ClearAllMeshSections();
    TArray<FVector> Vertices;
    TArray<int32> Triangles;
    TArray<FVector> Normals;
    TArray<FVector2D> UVs;
    TArray<FProcMeshTangent> Tangents;

    FVector Offset(0, 0, 0);

    CreateBox(10, 100, 10, Vertices, Triangles, Normals, UVs, Tangents, Offset);

    Offset.Z += 25;
    CreateRailMesh(100, 10, 12,Offset,1);
    Offset.Z += 40;
    CreateRailMesh(100, 10, 12, Offset,2);

    Offset.Z += 35;

    int VertexOffset2 = Vertices.Num();
    TArray<FVector> ConeVertices;
    TArray<int32> ConeTriangles;
    TArray<FVector> ConeNormals;
    TArray<FVector2D> ConeUVs;
    TArray<FProcMeshTangent> ConeTangents;

    CreateCone(10, 5, 16, ConeVertices, ConeTriangles, ConeNormals, ConeUVs, ConeTangents, Offset);

    for (int32& Index : ConeTriangles)
    {
        Index += VertexOffset2;
    }

    Vertices.Append(ConeVertices);
    Triangles.Append(ConeTriangles);
    Normals.Append(ConeNormals);
    UVs.Append(ConeUVs);
    Tangents.Append(ConeTangents);

    ProceduralMesh->CreateMeshSection_LinearColor(0, Vertices, Triangles, Normals, UVs, TArray<FLinearColor>(), Tangents, true);
}
void AVerticalRailActor::CreateMeshType3()
{
    ProceduralMesh->ClearAllMeshSections();
    TArray<FVector> Vertices;
    TArray<int32> Triangles;
    TArray<FVector> Normals;
    TArray<FVector2D> UVs;
    TArray<FProcMeshTangent> Tangents;

    FVector Offset(0, 0, 0);

    CreateBox(10, 100, 10, Vertices, Triangles, Normals, UVs, Tangents, Offset);
    Offset.Z += 25;
    CreateRailMesh(100, 10, 12, Offset,1);
    Offset.Z += 40;
    CreateRailMesh(100, 10, 12,Offset,2);

    Offset.Z += 35;

    int32 VertexOffset = Vertices.Num();

    TArray<FVector> CylinderVertices;
    TArray<int32> CylinderTriangles;
    TArray<FVector> CylinderNormals;
    TArray<FVector2D> CylinderUVs;
    TArray<FProcMeshTangent> CylinderTangents;

    CreateCylinder(5, 5, 16, CylinderVertices, CylinderTriangles, CylinderNormals, CylinderUVs, CylinderTangents, Offset);


    for (int32& Index : CylinderTriangles)
    {
        Index += VertexOffset;
    }


    Vertices.Append(CylinderVertices);
    Triangles.Append(CylinderTriangles);
    Normals.Append(CylinderNormals);
    UVs.Append(CylinderUVs);
    Tangents.Append(CylinderTangents);


    ProceduralMesh->CreateMeshSection_LinearColor(0, Vertices, Triangles, Normals, UVs, TArray<FLinearColor>(), Tangents, true);
}

void AVerticalRailActor::CreateMeshType4()
{
    ProceduralMesh->ClearAllMeshSections();
    TArray<FVector> Vertices;
    TArray<int32> Triangles;
    TArray<FVector> Normals;
    TArray<FVector2D> UVs;
    TArray<FProcMeshTangent> Tangents;

    FVector Offset(0, 0, 0);

    CreateBox(10, 100, 10, Vertices, Triangles, Normals, UVs, Tangents, Offset);

    Offset.Z += 25;
    CreateRailMesh(100, 10, 12,Offset,1);
    Offset.Z += 40;
    CreateRailMesh(100, 10, 12, Offset,2);

    Offset.Z += 35 + 10;
    int32 VertexOffset = Vertices.Num();
    TArray<FVector> OvalVertices;
    TArray<int32> OvalTriangles;
    TArray<FVector> OvalNormals;
    TArray<FVector2D> OvalUVs;
    TArray<FProcMeshTangent> OvalTangents;

    CreateOval(20,12, 7, 32, OvalVertices, OvalTriangles, OvalNormals, OvalUVs, OvalTangents, Offset);


    for (int32& Index : OvalTriangles)
    {
        Index += VertexOffset;
    }


    Vertices.Append(OvalVertices);
    Triangles.Append(OvalTriangles);
    Normals.Append(OvalNormals);
    UVs.Append(OvalUVs);
    Tangents.Append(OvalTangents);

    ProceduralMesh->CreateMeshSection_LinearColor(0, Vertices, Triangles, Normals, UVs, TArray<FLinearColor>(), Tangents, true);
}
void AVerticalRailActor::CreateMeshType5()
{
    ProceduralMesh->ClearAllMeshSections();
    TArray<FVector> Vertices;
    TArray<int32> Triangles;
    TArray<FVector> Normals;
    TArray<FVector2D> UVs;
    TArray<FProcMeshTangent> Tangents;

    FVector Offset(0, 0, 0);

    CreateBox(10, 100, 10, Vertices, Triangles, Normals, UVs, Tangents, Offset);

    Offset.Z += 25;
    CreateRailMesh(100, 10, 12, Offset,1);
    Offset.Z += 40;
    CreateRailMesh(100, 10, 12,Offset,2);

    Offset.Z += 35;

    int32 VertexOffset = Vertices.Num();

    TArray<FVector> PyramidVertices;
    TArray<int32> PyramidTriangles;
    TArray<FVector> PyramidNormals;
    TArray<FVector2D> PyramidUVs;
    TArray<FProcMeshTangent> PyramidTangents;

   CreatePyramid(10, 10, PyramidVertices, PyramidTriangles, PyramidNormals, PyramidUVs, PyramidTangents, Offset);


    for (int32& Index : PyramidTriangles)
    {
        Index += VertexOffset;
    }

    Vertices.Append(PyramidVertices);
    Triangles.Append(PyramidTriangles);
    Normals.Append(PyramidNormals);
    UVs.Append(PyramidUVs);
    Tangents.Append(PyramidTangents);

    ProceduralMesh->CreateMeshSection_LinearColor(0, Vertices, Triangles, Normals, UVs, TArray<FLinearColor>(), Tangents, true);
}

void AVerticalRailActor::CreateMeshType6()
{
    ProceduralMesh->ClearAllMeshSections();
    TArray<FVector> Vertices;
    TArray<int32> Triangles;
    TArray<FVector> Normals;
    TArray<FVector2D> UVs;
    TArray<FProcMeshTangent> Tangents;

    FVector Offset(0, 0, 0);

    CreateBox(10, 100, 10, Vertices, Triangles, Normals, UVs, Tangents, Offset);

    Offset.Z += 25;
    CreateRailMesh(100, 10, 12, Offset,1);
    Offset.Z += 40;
    CreateRailMesh(100, 10, 12, Offset ,2);

    Offset.Z += 35;

    ////Offset.Z += 105;
    // 
    GenerateStand(Offset, 1, 2, 1, 5.0f, 10, 20,3);
    Offset.Z += 10;
    GenerateSphere(4, 16, 10,4);


    ProceduralMesh->CreateMeshSection_LinearColor(0, Vertices, Triangles, Normals, UVs, TArray<FLinearColor>(), Tangents, true);
}

void AVerticalRailActor::CreateMeshType7()
{
    ProceduralMesh->ClearAllMeshSections();
    TArray<FVector> Vertices;
    TArray<int32> Triangles;
    TArray<FVector> Normals;
    TArray<FVector2D> UVs;
    TArray<FProcMeshTangent> Tangents;

    FVector Offset(0, 0, 0);


    CreateBox(10, 100, 10, Vertices, Triangles, Normals, UVs, Tangents, Offset);

    Offset.Z += 25;
    CreateRailMesh(100, 10, 12,Offset,1);
    Offset.Z += 40;
    CreateRailMesh(100, 10, 12,Offset,2);

    Offset.Z += 35;

    GenerateStand(Offset, 1, 2, 1, 5.0f, 10, 20, 3);
    Offset.Z += 2;
    CreateRailMesh(10, 10, 10,Offset,4);

    ProceduralMesh->CreateMeshSection_LinearColor(0, Vertices, Triangles, Normals, UVs, TArray<FLinearColor>(), Tangents, true);
}
void AVerticalRailActor::CreateBox(float Width, float Height, float Depth, TArray<FVector>& Vertices, TArray<int32>& Triangles, TArray<FVector>& Normals, TArray<FVector2D>& UVs, TArray<FProcMeshTangent>& Tangents, FVector Offset)
{
    // Vertices for the box
    Vertices = {
        Offset+FVector(-Width/2, -Depth/2, 0), Offset+FVector(-Width/2, Depth/2, 0), Offset+FVector(Width/2, Depth/2, 0), Offset+FVector(Width/2, -Depth/2, 0), // -Z
        Offset+FVector(-Width/2, -Depth/2, Height), Offset+FVector(Width/2, -Depth/2, Height), Offset+FVector(Width/2, Depth/2, Height), Offset+FVector(-Width/2, Depth/2, Height), // +Z
        Offset+FVector(-Width/2, -Depth/2, 0), Offset+FVector(Width/2, -Depth/2, 0), Offset+FVector(Width/2, -Depth/2, Height), Offset+FVector(-Width/2, -Depth/2, Height), // -Y
        Offset+FVector(-Width/2, Depth/2, 0), Offset+FVector(-Width/2, Depth/2, Height), Offset+FVector(Width/2, Depth/2, Height), Offset+FVector(Width/2, Depth/2, 0), // +Y
        Offset+FVector(-Width/2, -Depth/2, 0), Offset+FVector(-Width/2, -Depth/2, Height), Offset+FVector(-Width/2, Depth/2, Height), Offset+FVector(-Width/2, Depth/2, 0), // -X
        Offset+FVector(Width/2, -Depth/2, 0), Offset+FVector(Width/2, Depth/2, 0), Offset+FVector(Width/2, Depth/2, Height), Offset+FVector(Width/2, -Depth/2, Height) // +X
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
}

void AVerticalRailActor::CreateRailMesh(float Width, float Height, float Depth,const FVector& Offset,int SectionIndex)
{

    TArray<FVector> RailVertices;
    TArray<int32> RailTriangles;
    TArray<FVector> RailNormals;
    TArray<FVector2D> RailUVs;
    TArray<FProcMeshTangent> RailTangents;

    RailVertices.Empty();
    RailTriangles.Empty();
    RailNormals.Empty();
    RailUVs.Empty();
    RailTangents.Empty();

    CreateBox(Width, Height, Depth, RailVertices, RailTriangles, RailNormals, RailUVs, RailTangents, Offset);

    ProceduralMesh->CreateMeshSection_LinearColor(SectionIndex, RailVertices, RailTriangles, RailNormals, RailUVs, TArray<FLinearColor>(), RailTangents, true);
}
void AVerticalRailActor::GenerateSphere(float Radius, int32 Segments, int32 Rings,int SectionIndex)
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
    FVector Offset = FVector(0, 0, 105);

    for (int32 RingNumber = 0; RingNumber <= Rings; RingNumber++)
    {
        for (int32 SegmentNumber = 0; SegmentNumber <= Segments; SegmentNumber++)
        {
            float Theta = 2.0f * PI * (float)SegmentNumber / (float)Segments;
            float Phi = PI * (float)RingNumber / (float)Rings;

            float X = FMath::Sin(Phi) * FMath::Cos(Theta);
            float Y = FMath::Sin(Phi) * FMath::Sin(Theta);
            float Z = FMath::Cos(Phi);

            FVector Vertex = FVector(X, Y, Z) * Radius + Offset;
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
}

void AVerticalRailActor::CreateCone(float Height, float Radius, int32 Segments, TArray<FVector>& Vertices, TArray<int32>& Triangles, TArray<FVector>& Normals, TArray<FVector2D>& UVs, TArray<FProcMeshTangent>& Tangents, FVector Offset)
{

    Vertices.Add(Offset + FVector(0.0f, 0.0f, Height));
    Normals.Add(FVector(0.0f, 0.0f, 1.0f));
    UVs.Add(FVector2D(0.5f, 1.0f));
    Tangents.Add(FProcMeshTangent(FVector(1.0f, 0.0f, 0.0f), false));

    for (int32 s = 0; s <= Segments; s++)
    {
        float Angle = static_cast<float>(s) / Segments * 2.0f * PI;
        float X = Radius * FMath::Cos(Angle);
        float Y = Radius * FMath::Sin(Angle);

        Vertices.Add(Offset + FVector(X, Y, 0.0f));
        Normals.Add(FVector(FMath::Cos(Angle), FMath::Sin(Angle), 0.0f));
        UVs.Add(FVector2D(static_cast<float>(s) / Segments, 0.0f));

        FVector TangentX = FVector(-FMath::Sin(Angle), FMath::Cos(Angle), 0.0f);
        FVector TangentY = FVector(FMath::Cos(Angle), FMath::Sin(Angle), 0.0f);
        FVector TangentZ = FVector(0.0f, 0.0f, 1.0f);

        Tangents.Add(FProcMeshTangent(TangentX, false));
    }

    for (int32 s = 0; s < Segments; s++)
    {
        Triangles.Add(0);
        Triangles.Add(s + 2);
        Triangles.Add(s + 1);
    }

}

void AVerticalRailActor::CreateOval(float Height, float Width, float Depth, int32 NumSegments, TArray<FVector>& Vertices, TArray<int32>& Triangles, TArray<FVector>& Normals, TArray<FVector2D>& UVs, TArray<FProcMeshTangent>& Tangents, FVector Offset)
{
    for (int32 i = 0; i <= NumSegments; ++i)
    {
        float Theta = i * PI / NumSegments;
        for (int32 j = 0; j <= NumSegments; ++j)
        {
            float Phi = j * 2 * PI / NumSegments;

            float X = Width * cos(Phi) * sin(Theta) / 2;
            float Y = Depth * sin(Phi) * sin(Theta) / 2;
            float Z = Height * cos(Theta) / 2;

            FVector Vertex = Offset + FVector(X, Y, Z);
            Vertices.Add(Vertex);

            FVector Normal = Vertex.GetSafeNormal();
            Normals.Add(Normal);

            FVector2D UV = FVector2D((float)j / NumSegments, (float)i / NumSegments);
            UVs.Add(UV);

            Tangents.Add(FProcMeshTangent(0.f, 1.f, 0.f));
        }
    }

    for (int32 i = 0; i < NumSegments; ++i)
    {
        for (int32 j = 0; j < NumSegments; ++j)
        {
            int32 First = (i * (NumSegments + 1)) + j;
            int32 Second = First + NumSegments + 1;

            Triangles.Add(First + 1);
            Triangles.Add(Second);
            Triangles.Add(First);

            Triangles.Add(First + 1);
            Triangles.Add(Second + 1);
            Triangles.Add(Second);
        }
    }
}

void AVerticalRailActor::CreateCylinder(float Radius, float Height, int32 Segments, TArray<FVector>& Vertices, TArray<int32>& Triangles, TArray<FVector>& Normals, TArray<FVector2D>& UVs, TArray<FProcMeshTangent>& Tangents, FVector Offset) {

    if (Segments < 3)
    {
        UE_LOG(LogTemp, Warning, TEXT("Number of segments must be at least 3"));
        return;
    }

    float AngleStep = 2 * PI / Segments;


    for (int32 i = 0; i <= Segments; ++i)
    {
        float Angle = i * AngleStep;
        float X = Radius * FMath::Cos(Angle);
        float Y = Radius * FMath::Sin(Angle);

        // Bottom vertices
        FVector BottomVertex = Offset + FVector(X, Y, 0);
        Vertices.Add(BottomVertex);
        Normals.Add(FVector(X, Y, 0).GetSafeNormal());
        UVs.Add(FVector2D((float)i / Segments, 1.0f));

        // Top vertices
        FVector TopVertex = Offset + FVector(X, Y, Height);
        Vertices.Add(TopVertex);
        Normals.Add(FVector(X, Y, 0).GetSafeNormal());
        UVs.Add(FVector2D((float)i / Segments, 0.0f));
    }

    for (int32 i = 0; i < Segments; ++i)
    {
        int32 BottomLeft = i * 2;
        int32 BottomRight = BottomLeft + 2;
        int32 TopLeft = BottomLeft + 1;
        int32 TopRight = BottomRight + 1;

        // Side triangles
        Triangles.Add(BottomLeft);
        Triangles.Add(TopLeft);
        Triangles.Add(BottomRight);

        Triangles.Add(BottomRight);
        Triangles.Add(TopLeft);
        Triangles.Add(TopRight);
    }


    int32 BaseIndex = Vertices.Num();
    Vertices.Add(Offset + FVector(0, 0, 0)); // Center of bottom cap
    Vertices.Add(Offset + FVector(0, 0, Height)); // Center of top cap
    Normals.Add(FVector(0, 0, -1));
    Normals.Add(FVector(0, 0, 1));
    UVs.Add(FVector2D(0.5f, 0.5f));
    UVs.Add(FVector2D(0.5f, 0.5f));

    for (int32 i = 0; i < Segments; ++i)
    {
        int32 Current = i * 2;
        int32 Next = (i + 1) % Segments * 2;

        // Bottom cap triangles
        Triangles.Add(BaseIndex);
        Triangles.Add(Current);
        Triangles.Add(Next);

        // Top cap triangles
        Triangles.Add(BaseIndex + 1);
        Triangles.Add(Next + 1);
        Triangles.Add(Current + 1);

        // Normals for caps
        Normals.Add(FVector(0, 0, -1));
        Normals.Add(FVector(0, 0, 1));
    }

    for (int32 i = 0; i <= Segments; ++i)
    {
        float Angle = i * AngleStep;
        float X = FMath::Cos(Angle);
        float Y = FMath::Sin(Angle);
        FVector TangentDir = FVector(-Y, X, 0).GetSafeNormal();
        FProcMeshTangent Tangent = FProcMeshTangent(TangentDir, false);
        Tangents.Add(Tangent);
        Tangents.Add(Tangent);
    }

    for (int32 i = 0; i < Segments; ++i)
    {
        FProcMeshTangent BottomTangent = FProcMeshTangent(FVector::RightVector, false);
        FProcMeshTangent TopTangent = FProcMeshTangent(FVector::RightVector, false);
        Tangents.Add(BottomTangent);
        Tangents.Add(TopTangent);
    }
}

void AVerticalRailActor::CreatePyramid(float BaseWidth, float Height, TArray<FVector>& Vertices, TArray<int32>& Triangles, TArray<FVector>& Normals, TArray<FVector2D>& UVs, TArray<FProcMeshTangent>& Tangents, FVector Offset)
{

    Vertices.Empty();
    Triangles.Empty();
    Normals.Empty();
    UVs.Empty();
    Tangents.Empty();


    float HalfWidth = BaseWidth / 2.0f;

    // Bottom vertices
    Vertices.Add(FVector(-HalfWidth, -HalfWidth, 0) + Offset); // 0: Bottom left
    Vertices.Add(FVector(HalfWidth, -HalfWidth, 0) + Offset);  // 1: Bottom right
    Vertices.Add(FVector(HalfWidth, HalfWidth, 0) + Offset);   // 2: Top right
    Vertices.Add(FVector(-HalfWidth, HalfWidth, 0) + Offset);  // 3: Top left

    // Top vertex
    Vertices.Add(FVector(0, 0, Height) + Offset);              // 4: Apex

    // Define the triangles (faces) of the pyramid
    // Base
    Triangles.Add(0);
    Triangles.Add(2);
    Triangles.Add(1);

    Triangles.Add(0);
    Triangles.Add(3);
    Triangles.Add(2);

    // Sides
    Triangles.Add(0);
    Triangles.Add(4);
    Triangles.Add(1);

    Triangles.Add(1);
    Triangles.Add(4);
    Triangles.Add(2);

    Triangles.Add(2);
    Triangles.Add(4);
    Triangles.Add(3);

    Triangles.Add(3);
    Triangles.Add(4);
    Triangles.Add(0);

    // Define the normals for each face
    FVector BaseNormal = FVector(0, 0, -1);

    Normals.Add(BaseNormal); // 0: Bottom left
    Normals.Add(BaseNormal); // 1: Bottom right
    Normals.Add(BaseNormal); // 2: Top right
    Normals.Add(BaseNormal); // 3: Top left

    // Side normals
    FVector SideNormal1 = FVector::CrossProduct(Vertices[1] - Vertices[0], Vertices[4] - Vertices[0]).GetSafeNormal();
    FVector SideNormal2 = FVector::CrossProduct(Vertices[2] - Vertices[1], Vertices[4] - Vertices[1]).GetSafeNormal();
    FVector SideNormal3 = FVector::CrossProduct(Vertices[3] - Vertices[2], Vertices[4] - Vertices[2]).GetSafeNormal();
    FVector SideNormal4 = FVector::CrossProduct(Vertices[0] - Vertices[3], Vertices[4] - Vertices[3]).GetSafeNormal();

    Normals.Add(SideNormal1); // 4: Side 1
    Normals.Add(SideNormal1);
    Normals.Add(SideNormal1);

    Normals.Add(SideNormal2); // 5: Side 2
    Normals.Add(SideNormal2);
    Normals.Add(SideNormal2);

    Normals.Add(SideNormal3); // 6: Side 3
    Normals.Add(SideNormal3);
    Normals.Add(SideNormal3);

    Normals.Add(SideNormal4); // 7: Side 4
    Normals.Add(SideNormal4);
    Normals.Add(SideNormal4);

    // Calculate the UVs for texture mapping
    UVs.Add(FVector2D(0, 0)); // 0: Bottom left
    UVs.Add(FVector2D(1, 0)); // 1: Bottom right
    UVs.Add(FVector2D(1, 1)); // 2: Top right
    UVs.Add(FVector2D(0, 1)); // 3: Top left
    UVs.Add(FVector2D(0.5, 0.5)); // 4: Apex

    // Define the tangents for each vertex
    FProcMeshTangent TangentX1(1, 0, 0);
    FProcMeshTangent TangentX2(1, 0, 0);
    FProcMeshTangent TangentX3(1, 0, 0);
    FProcMeshTangent TangentX4(1, 0, 0);

    Tangents.Add(TangentX1); // 0: Bottom left
    Tangents.Add(TangentX1); // 1: Bottom right
    Tangents.Add(TangentX1); // 2: Top right
    Tangents.Add(TangentX1); // 3: Top left
    Tangents.Add(TangentX1); // 4: Apex

    // Set tangents for the sides
    Tangents.Add(TangentX1); // 0: Side 1
    Tangents.Add(TangentX1);
    Tangents.Add(TangentX1);

    Tangents.Add(TangentX2); // 1: Side 2
    Tangents.Add(TangentX2);
    Tangents.Add(TangentX2);

    Tangents.Add(TangentX3); // 2: Side 3
    Tangents.Add(TangentX3);
    Tangents.Add(TangentX3);

    Tangents.Add(TangentX4); // 3: Side 4
    Tangents.Add(TangentX4);
    Tangents.Add(TangentX4);
}
void AVerticalRailActor::GenerateStand(FVector Offset, float BaseRadius, float Height1, float RimRadius, float CurvatureFactor, int NumSlices, int NumStacks,int SectionIndex)
{
    TArray<FVector> Vertices;
    TArray<int32> Triangles;
    TArray<FVector> Normals;
    TArray<FVector2D> UVs;
    TArray<FProcMeshTangent> Tangents;
    TArray<FLinearColor> Colors;

    float Theta, Phi;
    float DeltaTheta = 2.0f * PI / NumSlices;
    float DeltaPhi = PI / (NumStacks - 1);

    // Generate vertices
    for (int i = 0; i < NumStacks; i++)
    {
        Phi = i * DeltaPhi;

        // Bell profile function for radius interpolation with curvature
        float t = Phi / (PI / 2.0f);
        float CurrentRadius = FMath::Lerp(BaseRadius, RimRadius, FMath::Sin(t * CurvatureFactor)) + 2.5;

        for (int j = 0; j < NumSlices; j++)
        {
            Theta = j * DeltaTheta;

            float X = CurrentRadius * FMath::Sin(Theta);
            float Y = CurrentRadius * FMath::Cos(Theta);
            float Z = Height1 * (t - 1.0f); // Z decreases with height for bell shape

            FVector Location = Offset;
            // Apply location offset
            X += Location.X;
            Y += Location.Y;
            Z += Location.Z;

            // Invert Z-coordinate


            Vertices.Add(FVector(X, Y, Z + Height1));

            // Calculate normals for lighting (approximate)
            Normals.Add(FVector(X, Y, Z).GetSafeNormal());

            // UV mapping
            UVs.Add(FVector2D((float)j / (NumSlices - 1), (float)i / (NumStacks - 1)));

            // Tangents (optional)
            Tangents.Add(FProcMeshTangent(-FMath::Sin(Theta), FMath::Cos(Theta), 0.0f));
        }
    }

    // Generate triangles (quads converted to triangles)
    for (int i = 0; i < NumStacks - 1; i++)
    {
        for (int j = 0; j < NumSlices - 1; j++)
        {
            int BottomLeft = i * NumSlices + j;
            int BottomRight = BottomLeft + 1;
            int TopLeft = BottomLeft + NumSlices;
            int TopRight = TopLeft + 1;

            // First triangle (bottom left to top left to top right)
            Triangles.Add(BottomLeft);
            Triangles.Add(TopLeft);
            Triangles.Add(BottomRight);

            Triangles.Add(BottomLeft);
            Triangles.Add(BottomRight);
            Triangles.Add(TopLeft);

            // Second triangle (top left to top right to bottom right)
            Triangles.Add(TopLeft);
            Triangles.Add(TopRight);
            Triangles.Add(BottomRight);

            Triangles.Add(TopLeft);
            Triangles.Add(BottomRight);
            Triangles.Add(TopRight);
        }

        // Handle wrap-around triangle for each stack
        int BottomLeftWrap = i * NumSlices + (NumSlices - 1);
        int BottomRightWrap = i * NumSlices;
        int TopLeftWrap = BottomLeftWrap + NumSlices;
        int TopRightWrap = BottomRightWrap + NumSlices;

        // First triangle (bottom left to top left to top right)
        Triangles.Add(BottomLeftWrap);
        Triangles.Add(TopLeftWrap);
        Triangles.Add(BottomRightWrap);

        Triangles.Add(BottomLeftWrap);
        Triangles.Add(BottomRightWrap);
        Triangles.Add(TopLeftWrap);

        // Second triangle (top left to top right to bottom right)
        Triangles.Add(TopLeftWrap);
        Triangles.Add(TopRightWrap);
        Triangles.Add(BottomRightWrap);

        Triangles.Add(TopLeftWrap);
        Triangles.Add(BottomRightWrap);
        Triangles.Add(TopRightWrap);
    }

    ProceduralMesh->CreateMeshSection_LinearColor(SectionIndex, Vertices, Triangles, Normals, UVs, Colors, Tangents, true);
}
