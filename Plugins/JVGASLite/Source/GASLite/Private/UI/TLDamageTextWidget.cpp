// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/TLDamageTextWidget.h"
#include "Components/CanvasPanel.h"
#include "Components/TextBlock.h"
#include "Blueprint/UserWidget.h"
#include "Blueprint/WidgetTree.h"

UTLDamageTextWidget::UTLDamageTextWidget(const FObjectInitializer& ObjectInitializer): Super(ObjectInitializer)
{

}


void UTLDamageTextWidget::SetDamageText_Implementation(float damageDelta)
{
// 	FString damageStr = FString::SanitizeFloat(FMath::Abs(damageDelta));
// 
// 	TextBlock->SetText(FText::FromString(damageStr));
}

void UTLDamageTextWidget::NativeConstruct()
{
	Super::NativeConstruct();

	UCanvasPanel* Root = WidgetTree->ConstructWidget<UCanvasPanel>(UCanvasPanel::StaticClass(), FName("Canvas"));

	WidgetTree->RootWidget = Root;
	UWidget* root = GetRootWidget();

	TextBlock = WidgetTree->ConstructWidget<UTextBlock>(UTextBlock::StaticClass(),FName("DamageText"));
	
}

