// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GL_Fountain.generated.h"

class USphereComponent;

UCLASS()
class GASLITE_API AGL_Fountain : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AGL_Fountain();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere,BlueprintReadOnly, Category = "Common")
	USphereComponent* RadiusComponent;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Common")
	TArray<TEnumAsByte<EObjectTypeQuery>> ObjectTypes;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category ="Team")
	uint8 TeamID;

	UPROPERTY(EditDefaultsOnly, Category = "Team")
	float Percentage;

	UPROPERTY(EditDefaultsOnly, Category = "Team")
	float HealTimer;


	FTimerDelegate TimerDel;

	FTimerHandle TimerHandle;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintCallable, Category = "Common")
	void HealTeamPlayers();

};
