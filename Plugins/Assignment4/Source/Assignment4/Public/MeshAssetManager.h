// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "MeshAssetManager.generated.h"

/**
 * 
 */
USTRUCT(BlueprintType)
struct FMeshData {
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Mesh Data")
	FString MeshName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Mesh Data")
	UStaticMesh* MeshReference;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Mesh Data")
	UTexture2D* MeshThumbnail;
};

USTRUCT(BlueprintType)
struct FMaterialData {
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Material Data")
	FString MaterialName;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Material Data")
	UMaterialInterface* MaterialReference;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Material Data")
	UTexture2D* MaterialThumbnail;
};

USTRUCT(BlueprintType)
struct FTextureData {
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Texture Data")
	FString TextureName;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Texture Data")
	UTexture* TextureReference;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Texture Data")
	UTexture2D* TextureThumbnail;
};
UCLASS()
class ASSIGNMENT4_API UMeshAssetManager : public UDataAsset
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Mesh Data")
	TArray<FMeshData> Meshes;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Material Data")
	TArray<FMaterialData> Materials;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Texture Data")
	TArray<FTextureData> Textures;

};
