// Fill out your copyright notice in the Description page of Project Settings.


#include "MeshSelectionScrollBox.h"

TSharedRef<SWidget> UMeshSelectionScrollBox::RebuildWidget()
{
	MyMeshSelectionScrollBox = SNew(SMeshSelectionScrollBox).InMeshAsset(MeshAssetManger).InMeshType(AssetType);
	MyMeshSelectionScrollBox->OnThumbnailSelected.BindUObject(this, &UMeshSelectionScrollBox::HandleSlateSelection);
	MyMeshSelectionScrollBox->OnMaterialThumbnailSelected.BindUObject(this, &UMeshSelectionScrollBox::HandleMaterialSelection);
	MyMeshSelectionScrollBox->OnTextureThumbnailSelected.BindUObject(this, &UMeshSelectionScrollBox::HandleTextureSelection);
	return MyMeshSelectionScrollBox.ToSharedRef();
}
void UMeshSelectionScrollBox::HandleSlateSelection(const FMeshData& MeshData) {
	MeshSelected.ExecuteIfBound(MeshData);
}

void UMeshSelectionScrollBox::HandleMaterialSelection(const FMaterialData& MaterialData) {
	MaterialSelected.ExecuteIfBound(MaterialData);
}

void UMeshSelectionScrollBox::HandleTextureSelection(const FTextureData& TextureData) {
	TextureSelected.ExecuteIfBound(TextureData);
}
void UMeshSelectionScrollBox::ReleaseSlateResources(bool bReleaseChildren)
{
	Super::ReleaseSlateResources(bReleaseChildren);
	MyMeshSelectionScrollBox.Reset();
}

const FText UMeshSelectionScrollBox::GetPaletteCategory()
{
	return FText::FromString("Panel");
}

