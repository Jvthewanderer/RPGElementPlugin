// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Interface/TLGameCompInterface.h"
#include "GL_Unit.h"
#include "RLCharacter.generated.h"

class UCameraComponent;
class USpringArmComponent;
class URLInteractionComponent;
class UAnimMontage;
class UTLAttributeComponent;
class UTLActionComponent;
class USceneComponent;

UCLASS()
class GASLITE_API ARLCharacter : public AGL_Unit
{
	GENERATED_BODY()

protected:
	UPROPERTY(EditAnywhere, Category = "Attack")
	TSubclassOf<AActor> ProjectileClass;

	UPROPERTY(EditAnywhere, Category ="Attack")
	UAnimMontage* AttackAnim;

	UPROPERTY(EditAnywhere, Category = "Attack")
	UAnimMontage* SpecialAnim;

	UPROPERTY(EditAnywhere, Category = "Attack")
	UAnimMontage* DashAnim;

	UPROPERTY(EditAnywhere, Category = "Attack")
	FName ProjectileSocketName;

	UPROPERTY(EditAnywhere, Category = "Attack")
	FName TimeToHitParamName;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Component")
	UTLAttributeComponent* AttributeComp;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Component")
	UTLActionComponent* ActionComp;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Attack")
	bool bisTargeting;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Team")
	int32 Team;

	FTimerHandle TimerHandle_PrimaryAttack;

	FTimerHandle TimerHandle_SpecialAttack;

	FTimerHandle TimerHandle_DashAttack;


public:
	// Sets default values for this character's properties
	ARLCharacter();

	UCameraComponent* GetPlayerCamera() {
		return CameraComp;
	}

	UTLActionComponent* GetActionComponent_Implementation() override;


	UTLAttributeComponent* GetAttriblueComponent_Implementation() override;


	USkeletalMeshComponent* GetActorMesh_Implementation() override;

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	USpringArmComponent* SpringArmComp;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	UCameraComponent* CameraComp;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	URLInteractionComponent* InteractionComp;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Character")
	bool bIsDead;

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UFUNCTION()
	void MoveForward(float Value);

	UFUNCTION()
	void MoveRight(float Value);

	void PrimaryAbility();

	void SecondaryAbility();

	void PrimaryAttack();

	void SpecialAttack();

	void PrimaryInteract();

	void TertiaryAbility();
	

	UFUNCTION()
	void OnHealthChange(AActor* InstigatorActor, UTLAttributeComponent* OwningComp, float NewHealth, float Delta);



	virtual void PostInitializeComponents();

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	
	/** Returns	Pawn's eye location */
	virtual FVector GetPawnViewLocation() const override;

	virtual void Jump() override;

	UFUNCTION(Exec)
	void HealSelf(float Amount = 100);
	
	UFUNCTION(BlueprintPure, Category = "Teams")
		int32 GetTeamID(){return Team;};

	UFUNCTION(BlueprintCallable, Category = "Teams")
		void SetTeamID(int32 TeamId) { Team = TeamId; };

	UFUNCTION(BlueprintPure, Category = "Camera")
	FVector GetMouseLocation();

};
