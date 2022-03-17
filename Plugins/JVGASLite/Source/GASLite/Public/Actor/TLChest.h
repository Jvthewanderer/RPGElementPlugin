// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Interface/TLGameplayInterface.h"
#include "TLChest.generated.h"

class UStaticMeshComponent;

UCLASS()
class GASLITE_API ATLChest : public AActor, public ITLGameplayInterface
{
	GENERATED_BODY()
	
public:	
	
	UPROPERTY(EditAnywhere)
	float TargetPitch;

	// Sets default values for this actor's properties
	ATLChest();

protected:

	UPROPERTY(ReplicatedUsing="OnRep_LidOpened", BlueprintReadOnly, SaveGame)// RepNotify
	bool bLidOpened;

	UFUNCTION()
	void OnRep_LidOpened();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	UStaticMeshComponent* BaseMesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	UStaticMeshComponent* LidMesh;

public:
	void Interact_Implementation(APawn* InstigatorPawn) override;


	void OnActorLoaded_Implementation();

};
