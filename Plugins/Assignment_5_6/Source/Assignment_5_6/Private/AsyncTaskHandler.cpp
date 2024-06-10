// Fill out your copyright notice in the Description page of Project Settings.


#include "AsyncTaskHandler.h"
#include "MeshGenerator.h"

FAsyncTaskHandler::FAsyncTaskHandler(AMeshGenerator* InMeshGenerator)
{
	MeshGenerator = InMeshGenerator;
}


void FAsyncTaskHandler::DoWork()
{
	if (MeshGenerator)
	{
		if (UMeshDataAsset* DataAsset = MeshGenerator->DataAsset)
		{
			TArray<FMeshDataStruct> MeshDataStruct = DataAsset->MeshDataArr;

				for (int iIndex = 0; iIndex < MeshGenerator->NumberOfInstances; iIndex++)
				{

					int RandomIndex = FMath::RandRange(0, MeshDataStruct.Num() - 1);
					UStaticMesh* CurrentMesh = MeshDataStruct[RandomIndex].MeshType;
					float Rotation = FMath::RandRange(MeshDataStruct[RandomIndex].MinRotation, MeshDataStruct[RandomIndex].MaxRotation);
					float Scale = FMath::RandRange(MeshDataStruct[RandomIndex].MinScale, MeshDataStruct[RandomIndex].MaxScale);
					FVector Position;
					if (MeshGenerator->Type == "Box") {
						//Box
						FVector BoundingExtent = (MeshGenerator->Scale) * 50;
						FVector Origin = MeshGenerator->Location + FVector(0, 0, BoundingExtent.Z);
						FBox BoundingBox(Origin - BoundingExtent, Origin + BoundingExtent);

						Position = FMath::RandPointInBox(BoundingBox);
					}
					else {
						//Sphere
						float Radius = MeshGenerator->Scale.Z * 100;
						FVector Origin = MeshGenerator->Location;
						Position = FMath::VRand() * FMath::FRandRange(0.0f, Radius) + Origin;
					}


					TArray<FTransform> InstanceTransforms;
					FTransform transform;
					transform.SetLocation(Position);
					transform.SetScale3D(FVector(Scale));
					FRotator Rotation_(Rotation, Rotation, Rotation);
					FQuat QuatRotation(Rotation_);

					transform.SetRotation(QuatRotation);
					InstanceTransforms.Add(transform);

					MeshGenerator->AddInstances(CurrentMesh, InstanceTransforms);
					FPlatformProcess::Sleep(0.01f);
					float Progress = (float)(iIndex + 1) / (float)(MeshGenerator->NumberOfInstances);

					AsyncTask(ENamedThreads::GameThread, [this, Progress]()
						{
							//MeshGenerator->FinishScatter();
							MeshGenerator->UpdateProgressBar(Progress);

						});
				}



		}

		AsyncTask(ENamedThreads::GameThread, [this]()
			{
				//MeshGenerator->FinishScatter();
			});
	}
}
