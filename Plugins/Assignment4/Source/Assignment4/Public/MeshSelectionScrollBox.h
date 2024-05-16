// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/Widget.h"
#include "SMeshSelectionScrollBox.h"
#include "MeshSelectionScrollBox.generated.h"

/**
 * 
 */
DECLARE_DELEGATE_OneParam(FOnMeshSelected,const FMeshData&)
DECLARE_DELEGATE_OneParam(FOnMaterialSelected,const FMaterialData&)
DECLARE_DELEGATE_OneParam(FOnTextureSelected,const FTextureData&)
UCLASS()
class ASSIGNMENT4_API UMeshSelectionScrollBox : public UWidget
{
	GENERATED_BODY()
protected:
	virtual TSharedRef<SWidget> RebuildWidget() override;
	void HandleSlateSelection(const FMeshData& MeshData);
	void HandleMaterialSelection(const FMaterialData& MaterialData);
	void HandleTextureSelection(const FTextureData& TextureData);
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Mesh Asset Manager")
	UMeshAssetManager* MeshAssetManger;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Asset Type")
	TEnumAsByte<EAssetType> AssetType;

	void ReleaseSlateResources(bool bReleaseChildren);
	const FText GetPaletteCategory();
public:
	TSharedPtr<SMeshSelectionScrollBox> MyMeshSelectionScrollBox;
	FOnMeshSelected MeshSelected;
	FOnMaterialSelected MaterialSelected;
	FOnTextureSelected TextureSelected;

};
