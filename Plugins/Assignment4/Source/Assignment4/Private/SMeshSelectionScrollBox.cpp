// Fill out your copyright notice in the Description page of Project Settings.


#include "SMeshSelectionScrollBox.h"
#include "SlateOptMacros.h"

BEGIN_SLATE_FUNCTION_BUILD_OPTIMIZATION
void SMeshSelectionScrollBox::Construct(const FArguments& InArgs)
{
	
	FSlateBrush* BackgroundBrush = new FSlateBrush();
	BackgroundBrush->TintColor = FLinearColor(0.117647f, 0.505882f, 0.690196f, 1.0f); // Set your desired background color
	ScrollBox = SNew(SScrollBox).Orientation(EOrientation::Orient_Horizontal);
		

	MeshAsset = InArgs._InMeshAsset;
	MeshType = InArgs._InMeshType;
	ChildSlot
		[
				SNew(SBorder)
					.BorderImage(BackgroundBrush)[
						SNew(SVerticalBox)
							+SVerticalBox::Slot().AutoHeight().HAlign(HAlign_Center)
							[
								SNew(STextBlock)
									.Text(GetAssetTypeName())
									.ColorAndOpacity(FColor::Blue)
									.Font(FSlateFontInfo(FPaths::EngineContentDir() / TEXT("Slate/Fonts/Roboto-Bold.ttf"), 20))
							]
							+ SVerticalBox::Slot()[

								ScrollBox.ToSharedRef()
							]
							
				]
		];
	if (MeshType == EAssetType::Mesh)
		RefreshAssetMeshThubnail();
	else if (MeshType == EAssetType::Material)
		RefreshAssetMaterialThubnail();
	else if (MeshType == EAssetType::Texture)
		RefreshAssetTextureThubnail();
}
END_SLATE_FUNCTION_BUILD_OPTIMIZATION
void SMeshSelectionScrollBox::RefreshAssetMeshThubnail()
{

	ScrollBox->ClearChildren();
	if (MeshAsset.IsValid()) {

		for (const auto& MeshData : MeshAsset->Meshes) {
			TSharedPtr<SVerticalBox> VerticalBox = SNew(SVerticalBox);
			TSharedPtr<SHorizontalBox> HorizontalBox = SNew(SHorizontalBox);
			FSlateBrush* ThumbnailBrush = new FSlateBrush();
			ThumbnailBrush->SetResourceObject(MeshData.MeshThumbnail);
			ThumbnailBrush->SetImageSize(FVector2D(150));
			TSharedPtr<STextBlock> TextBlock = SNew(STextBlock).ColorAndOpacity(FColor::Blue).Font(FSlateFontInfo(FPaths::EngineContentDir() / TEXT("Slate/Fonts/Roboto-Bold.ttf"), 20));
			TextBlock->SetText(FText::FromString(MeshData.MeshName));
			TSharedPtr<SImage> ThumbnailImage = SNew(SImage).Image(ThumbnailBrush).Cursor(EMouseCursor::Hand).OnMouseButtonDown_Lambda([this, MeshData](const FGeometry& Geometry, const FPointerEvent& MouseEvent) {
				if (MouseEvent.GetEffectingButton() == EKeys::LeftMouseButton) {
					OnThumbnailSelected.ExecuteIfBound(MeshData);
					return FReply::Handled();
				}
				return FReply::Unhandled();
				});
			VerticalBox->AddSlot()[
				ThumbnailImage.ToSharedRef()
			];
			VerticalBox->AddSlot().HAlign(EHorizontalAlignment::HAlign_Center).AutoHeight()
				[
					TextBlock.ToSharedRef()
				];

			ScrollBox->AddSlot().VAlign(EVerticalAlignment::VAlign_Center)
				[
					VerticalBox.ToSharedRef()
				];
		}
	}
}

void SMeshSelectionScrollBox::RefreshAssetMaterialThubnail()
{
	ScrollBox->ClearChildren();
	if (MeshAsset.IsValid()) {
		for (const auto& MeshData : MeshAsset->Materials) {
			TSharedPtr<SVerticalBox> VerticalBox = SNew(SVerticalBox);
			FSlateBrush* ThumbnailBrush = new FSlateBrush();
			ThumbnailBrush->SetResourceObject(MeshData.MaterialThumbnail);
			ThumbnailBrush->SetImageSize(FVector2D(150));
			TSharedPtr<STextBlock> TextBlock = SNew(STextBlock).ColorAndOpacity(FColor::Blue).Font(FSlateFontInfo(FPaths::EngineContentDir() / TEXT("Slate/Fonts/Roboto-Bold.ttf"), 20));
			TextBlock->SetText(FText::FromString(MeshData.MaterialName));
			TSharedPtr<SImage> ThumbnailImage = SNew(SImage).Image(ThumbnailBrush).Cursor(EMouseCursor::Hand).OnMouseButtonDown_Lambda([this, MeshData](const FGeometry& Geometry, const FPointerEvent& MouseEvent) {
				if (MouseEvent.GetEffectingButton() == EKeys::LeftMouseButton) {
					OnMaterialThumbnailSelected.ExecuteIfBound(MeshData);
					return FReply::Handled();
				}
				return FReply::Unhandled();
				});

			VerticalBox->AddSlot()[
				ThumbnailImage.ToSharedRef()
			];
			VerticalBox->AddSlot().HAlign(EHorizontalAlignment::HAlign_Center).AutoHeight()
				[
					TextBlock.ToSharedRef()
				];

			ScrollBox->AddSlot().VAlign(EVerticalAlignment::VAlign_Center)
				[
					VerticalBox.ToSharedRef()
				];
		}
	}
}

void SMeshSelectionScrollBox::RefreshAssetTextureThubnail()
{
	ScrollBox->ClearChildren();
	if (MeshAsset.IsValid()) {
		for (const auto& MeshData : MeshAsset->Textures) {
			TSharedPtr<SVerticalBox> VerticalBox = SNew(SVerticalBox);
			FSlateBrush* ThumbnailBrush = new FSlateBrush();
			ThumbnailBrush->SetResourceObject(MeshData.TextureThumbnail);
			ThumbnailBrush->SetImageSize(FVector2D(150));
			TSharedPtr<STextBlock> TextBlock = SNew(STextBlock).ColorAndOpacity(FColor::Blue).Font(FSlateFontInfo(FPaths::EngineContentDir() / TEXT("Slate/Fonts/Roboto-Bold.ttf"), 20));
			TextBlock->SetText(FText::FromString(MeshData.TextureName));
			TSharedPtr<SImage> ThumbnailImage = SNew(SImage).Image(ThumbnailBrush).Cursor(EMouseCursor::Hand).OnMouseButtonDown_Lambda([this, MeshData](const FGeometry& Geometry, const FPointerEvent& MouseEvent) {
				if (MouseEvent.GetEffectingButton() == EKeys::LeftMouseButton) {
					OnTextureThumbnailSelected.ExecuteIfBound(MeshData);
					return FReply::Handled();
				}
				return FReply::Unhandled();
				});

			VerticalBox->AddSlot()[
				ThumbnailImage.ToSharedRef()
			];
			VerticalBox->AddSlot().HAlign(EHorizontalAlignment::HAlign_Center).AutoHeight()
				[
					TextBlock.ToSharedRef()
				];

			ScrollBox->AddSlot().VAlign(EVerticalAlignment::VAlign_Center)
				[
					VerticalBox.ToSharedRef()
				];
		}
	}
}
FText SMeshSelectionScrollBox::GetAssetTypeName() const
{
	switch (MeshType)
	{
	case EAssetType::Mesh:
		return FText::FromString(TEXT("Mesh")); // Adjust the string as needed
	case EAssetType::Material:
		return FText::FromString(TEXT("Material")); // Adjust the string as needed
	case EAssetType::Texture:
		return FText::FromString(TEXT("Texture")); // Adjust the string as needed
	default:
		return FText::FromString(TEXT("Unknown")); // Default case for unknown types
	}
}