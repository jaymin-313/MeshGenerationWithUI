// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MeshAssetManager.h"
#include "InputCoreTypes.h"
#include "Widgets/SCompoundWidget.h"
#include <Widgets/Layout/SScrollBox.h>

/**
 * 
 */
DECLARE_DELEGATE_OneParam(FOnThumbnailSelected, const FMeshData&)
DECLARE_DELEGATE_OneParam(FOnMaterialThumbnailSelected, const FMaterialData&)
DECLARE_DELEGATE_OneParam(FOnTextureThumbnailSelected, const FTextureData&)

UENUM()
enum class EAssetType {
	Mesh,
	Material,
	Texture
};
class ASSIGNMENT4_API SMeshSelectionScrollBox : public SCompoundWidget
{
public:
	SLATE_BEGIN_ARGS(SMeshSelectionScrollBox)
	{}
		SLATE_ARGUMENT(TWeakObjectPtr<UMeshAssetManager>, InMeshAsset)
		SLATE_ARGUMENT(EAssetType, InMeshType)
	SLATE_END_ARGS()

	/** Constructs this widget with InArgs */
	void Construct(const FArguments& InArgs);

	FOnThumbnailSelected OnThumbnailSelected;
	FOnMaterialThumbnailSelected OnMaterialThumbnailSelected;
	FOnTextureThumbnailSelected OnTextureThumbnailSelected;
private:
	void RefreshAssetMeshThubnail();
	void RefreshAssetMaterialThubnail();
	void RefreshAssetTextureThubnail();

	FText GetAssetTypeName() const;

	TWeakObjectPtr<UMeshAssetManager> MeshAsset;
	EAssetType MeshType;
	TSharedPtr<SScrollBox> ScrollBox;
};
