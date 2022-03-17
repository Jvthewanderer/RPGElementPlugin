// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "RLInteractionComponent.generated.h"

class UTLWorldUserWidget;


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class GASLITE_API URLInteractionComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	URLInteractionComponent();

	void PrimaryInteract();

	UPROPERTY(EditAnywhere)
	float eyeDistance;

protected:
	UFUNCTION(Server,Reliable)
	void ServerInteract(AActor* InFocus);

	void FindBestInteractable();

	// Called when the game starts
	virtual void BeginPlay() override;

	UPROPERTY()
	AActor* FocusActor;

	UPROPERTY(EditDefaultsOnly, Category = "Trace")
	float TraceRadius;
	
	UPROPERTY(EditDefaultsOnly, Category = "Trace")
	TEnumAsByte<ECollisionChannel> CollisionChannel;

	UPROPERTY(EditDefaultsOnly, Category = "UI")
	TSubclassOf<UTLWorldUserWidget> DefaultWidgetClass;

	UPROPERTY()
	UTLWorldUserWidget* DefaultWidgetInstance;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

		
};
