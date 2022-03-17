// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "TLPopUpText.generated.h"

class USphereComponent;
class UProjectileMovementComponent;
class UWidgetComponent;
class UUserWidget;

UCLASS()
class GASLITE_API ATLPopUpText : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ATLPopUpText();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Damage")
		float damageDelta;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Widget")
		UWidgetComponent* TextWidget;

protected:

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
		USphereComponent* SphereComp;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
		UProjectileMovementComponent* MoveComp;



	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Widget")
		TSubclassOf<UUserWidget> TextUIClass;

		virtual void BeginPlay() override;


};
