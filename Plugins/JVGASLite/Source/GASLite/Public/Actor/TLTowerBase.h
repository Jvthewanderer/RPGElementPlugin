// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "Interface/TLGameCompInterface.h"
#include "Interface/TLGameplayInterface.h"
#include "TLTowerBase.generated.h"

class UTLAttributeComponent;
class UTLActionComponent;
class UUserWidget;
class USphereComponent;
class UTLWorldUserWidget;
class UBlackboardComponent;
class USkeletalMeshComponent;
/*class UNiagaraComponent;*/
class UWidgetComponent;
class USceneComponent;
class ATLAIController;

UCLASS()
class GASLITE_API ATLTowerBase : public APawn, public ITLGameCompInterface, public ITLGameplayInterface
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ATLTowerBase();

protected:

	virtual void PostInitializeComponents() override;

	UPROPERTY(EditDefaultsOnly, Category = "Component")
	ATLAIController* TLAIController;

	UPROPERTY(EditDefaultsOnly, Category = "Component")
		UWidgetComponent* HealthBarComp;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Target")
	UTLWorldUserWidget* ActiveHealthBar;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Component")
	USceneComponent* HealthBarPos;

	UPROPERTY(EditAnywhere)
		TSubclassOf<UUserWidget> HealthBarWidgetClass;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Component")
		UTLAttributeComponent* AttributeComp;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Component")
		UTLActionComponent* ActionComp;

		UTLAttributeComponent* OldAttriComp;

		UTLAttributeComponent* NewAttriComp;

// 	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Component")
// 		UNiagaraComponent* BeamNSystem;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Component")
		USphereComponent* TargetRadius;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Target")
		USkeletalMeshComponent* Mesh;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Target")
		USceneComponent* TargetPointComponent;

	UPROPERTY(EditAnywhere, Category = "Effect")
		FName TimeToHitParamName;

	UPROPERTY(EditAnywhere, Category = "Target")
		float TargetDistance;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Target")
		FName SocketName;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Beam")
		FName BeamEnd;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Beam")
		FName BeamColorName;

	UPROPERTY(EditDefaultsOnly, Category = "Beam")
		FLinearColor BeamColor;

	UPROPERTY(BlueprintReadOnly, Category = "Target")
	 AActor* CurrentTarget;

	 UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Target")
	 AActor* MinionNextPoint;

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UFUNCTION(BlueprintCallable, Category = "Target")
	void SetTargetActor(AActor* NewTarget);

	UFUNCTION(BlueprintPure, Category = "Target")
	AActor* GetTargetActor();

	UFUNCTION(NetMulticast, Reliable)
	void OnTargetHealthChange(AActor* InstigatorActor, UTLAttributeComponent* OwningComp, float NewHealth, float Delta);

	/** Returns Mesh subobject **/
	FORCEINLINE class USkeletalMeshComponent* GetMesh() const { return Mesh; }

	UFUNCTION(BlueprintPure, Category = "Blackboard")
	UBlackboardComponent* GetAIControllerBlackboard();

	UFUNCTION(BlueprintCallable, Category = "Target")
	void OnTowerHealthChange(AActor* InstigatorActor, UTLAttributeComponent* OwningComp, float NewHealth, float Delta);

	UFUNCTION( BlueprintCallable, Category = "Target")
	void AddTargetActor(AActor* OtherActor);

	UFUNCTION(BlueprintCallable, Category = "Target")
	void RemoveTargetActor(AActor* OtherActor);

	UFUNCTION(NetMulticast, Reliable)
		void MulticastAddTarget(AActor* OtherActor);

	UFUNCTION(NetMulticast, Reliable)
		void MulticastRemoveTarget(AActor* OtherActor);

	UFUNCTION(NetMulticast, Reliable)
		void MulticastSetTarget(AActor* NewTarget);


public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

public:
	//* INTERFACES *//

	UTLActionComponent* GetActionComponent_Implementation() override;


	UTLAttributeComponent* GetAttriblueComponent_Implementation() override;


	float GetRadius_Implementation() override;


	USkeletalMeshComponent* GetActorMesh_Implementation() override;


	bool PrimaryAttack_Implementation() override;


	USphereComponent* GetRadiusComponent_Implementation() override;


	USceneComponent* GetTargetComponent_Implementation() override;

};
