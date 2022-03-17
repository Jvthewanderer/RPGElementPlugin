// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GL_PopupText.generated.h"

class UProjectileMovementComponent;
class UGLWC_HitText;

UCLASS()
class GASLITE_API AGL_PopupText : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AGL_PopupText();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	UGLWC_HitText* HitTextWidgetComp;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	UProjectileMovementComponent* ProjectileMovementComp;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Default", meta = (ExposeOnSpawn = "true"))
	float GivenDelta;

};
