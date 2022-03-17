// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Actor/TLTowerBase.h"
#include "TLTower.generated.h"

UCLASS()
class GASLITE_API ATLTower : public ATLTowerBase
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ATLTower();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	void FindBestTarget();

	UPROPERTY()
	float SencingRange = 15000.f;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
