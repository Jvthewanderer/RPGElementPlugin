// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "TLDamageTextWidget.generated.h"

class UTextBlock;

/**
 * 
 */
UCLASS()
class GASLITE_API UTLDamageTextWidget : public UUserWidget
{
	GENERATED_BODY()

	protected:
	
		virtual void NativeConstruct() override;

	public:
		UTLDamageTextWidget(const FObjectInitializer& ObjectInitializer);

		UPROPERTY(EditAnywhere,BlueprintReadOnly, Category = "UI")
		float damage;

		UPROPERTY()
			UTextBlock* TextBlock;

		UFUNCTION(BlueprintNativeEvent,BlueprintCallable,Category = "UI")
		void SetDamageText(float damageDelta);
	
};
