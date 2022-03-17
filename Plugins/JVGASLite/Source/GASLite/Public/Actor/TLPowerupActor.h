// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Interface/TLGameplayInterface.h"
#include "TLPowerupActor.generated.h"

class USphereComponent;

UCLASS()
class GASLITE_API ATLPowerupActor : public AActor, public ITLGameplayInterface
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ATLPowerupActor();

protected:
	UPROPERTY(ReplicatedUsing = "OnRep_IsActive")
		bool bIsActive;

	UFUNCTION()
		void OnRep_IsActive();

	UPROPERTY(EditAnywhere, Category = "Powerup")
	float RespawnTime;

	UFUNCTION(BlueprintCallable, Category = "Powerup")
	void ShowPowerup();

	UFUNCTION(BlueprintCallable, Category = "Powerup")
	void HideAndCooldownPowerup();

	UFUNCTION(BlueprintCallable, Category = "Powerup")
	void SetPowerupState(bool bNewIsActive);

	UPROPERTY(VisibleAnywhere, Category = "Components")
		USphereComponent* SphereComp;

public:
	void Interact_Implementation(APawn* InstigatorPawn) override;


	FText GetInteractText_Implementation(APawn* InstigatorPawn) override;

};
