// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Interface/TLGameCompInterface.h"
#include "Interface/TLGameplayInterface.h"
#include "GL_Unit.h"
#include "TLAICharacter.generated.h"

class UPawnSensingComponent;
class UTLAttributeComponent;
class UUserWidget;
class UTLWorldUserWidget;
class UTLActionComponent;
class USphereComponent;
class UWidgetComponent;
class USceneComponent;

UCLASS()
class GASLITE_API ATLAICharacter : public AGL_Unit, public ITLGameplayInterface
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ATLAICharacter();

	UFUNCTION(BlueprintPure, Category = "Component")
	bool IsAlive();

	UFUNCTION()
	UTLActionComponent* GetActionComponent_Implementation() override;
	
	UFUNCTION()
	UTLAttributeComponent* GetAttriblueComponent_Implementation() override;
	
	UFUNCTION()
	float GetRadius_Implementation() override;
	
	UFUNCTION()
	USkeletalMeshComponent* GetActorMesh_Implementation() override;


	void BeginPlay() override;


	bool PrimaryAttack_Implementation() override;


	bool PrimaryAbility_Implementation() override;


	bool SecondaryAbility_Implementation() override;


	bool TertiaryAbility_Implementation() override;


	bool SpecialAttack_Implementation() override;


	void EndPlay(const EEndPlayReason::Type EndPlayReason) override;


	USphereComponent* GetRadiusComponent_Implementation() override;

protected:

	virtual void PostInitializeComponents() override;
	UFUNCTION()
	void OnAIRadiusBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	void OnAIRadiusEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	UTLWorldUserWidget* ActiveHealthBar;

	UTLAttributeComponent* OldAttriComp;
	UTLAttributeComponent* NewAttriComp;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Component")
		USphereComponent* TargetRadius;

	/* Widget to display when bot first sees a player. */
	UPROPERTY(EditDefaultsOnly, Category = "UI")
		TSubclassOf<UUserWidget> SpottedWidgetClass;

	UFUNCTION()
	void SetTargetActor(AActor* NewTarget);

	/* Key for AI Blackboard 'TargetActor' */
	UPROPERTY(EditDefaultsOnly, Category = "Effects")
		FName TargetActorKey;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Component")
		USceneComponent* HealthBarPos;

	UPROPERTY(EditAnywhere)
	TSubclassOf<UUserWidget> HealthBarWidgetClass;

	UPROPERTY(VisibleAnywhere, Category = "Componenet")
	UPawnSensingComponent* PawnSenseComp;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Component")
	UTLAttributeComponent* AttributeComp;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Component")
	UTLActionComponent* ActionComp;

	UPROPERTY(BlueprintReadOnly, Category = "Target")
	AActor* OriginPoint;

	UPROPERTY(BlueprintReadOnly, Category = "Target")
		AActor* CurrentTarget;

	UFUNCTION()
	void OnPawnSeen(APawn* Pawn);

	UFUNCTION(NetMulticast, Unreliable)
		void MulticastPawnSeen();

	UFUNCTION()
	void OnAIHealthChange(AActor* InstigatorActor, UTLAttributeComponent* OwningComp, float NewHealth, float Delta);

	UFUNCTION(NetMulticast, Reliable)
		void OnTargetHealthChange(AActor* InstigatorActor, UTLAttributeComponent* OwningComp, float NewHealth, float Delta);

	UFUNCTION(BlueprintPure, Category = "Blackboard")
	UBlackboardComponent* GetAIControllerBlackboard();

	UPROPERTY(EditAnywhere, Category = "Effect")
	FName TimeToHitParamName;

public:	

	UFUNCTION(BlueprintCallable, Category = "AI")
		void AssignTarget(AActor* NewTarget);

	UFUNCTION(BlueprintCallable, Category = "AI")
		void FindNearestTarget();

	UFUNCTION(BlueprintCallable, Category = "AI")
		AActor* GetTargetActor() const;

	UFUNCTION(BlueprintPure, Category = "AI")
	ATLAIController* GetTLAIController();

	UFUNCTION(BlueprintPure, Category = "Target")
		AActor* GetOriginTarget() const {return OriginPoint; };

};
