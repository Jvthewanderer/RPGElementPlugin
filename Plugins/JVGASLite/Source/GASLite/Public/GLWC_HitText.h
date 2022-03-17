// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/WidgetComponent.h"
#include "GLWC_HitText.generated.h"

class UTLDamageTextWidget;

/**
 * 
 */
UCLASS()
class GASLITE_API UGLWC_HitText : public UWidgetComponent
{
	GENERATED_BODY()

	protected:
	UGLWC_HitText();

	UPROPERTY(EditDefaultsOnly, Category = "Popup Widget")
	TSubclassOf<UTLDamageTextWidget> PopupTextWidgetClass;

	public:
	UFUNCTION(BlueprintCallable, Category="Popup Widget")
	void SetDamageText(float Delta);
	
};
