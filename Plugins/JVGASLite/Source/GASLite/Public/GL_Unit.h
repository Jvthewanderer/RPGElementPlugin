// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Game/GL_PlayerStateBase.h"
#include "Interface/TLGameCompInterface.h"
#include "GL_Unit.generated.h"

UENUM(BlueprintType)
enum ECommand {
	CommandIdle,
	CommandMove,
	CommandAttack
};

UCLASS()
class GASLITE_API AGL_Unit : public ACharacter, public ITLGameCompInterface
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AGL_Unit();


	float GetRadius_Implementation() override;


	USceneComponent* GetTargetComponent_Implementation() override;

protected:

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Unit")
		USceneComponent* TargetPointComponent;

public:	

	UFUNCTION(BlueprintNativeEvent,BlueprintCallable, Category = "Teams")
	void UnitInit(FName _unitKey, uint8 TeamID, AGL_PlayerStateBase* CommanderPlayerState, AGL_Unit* OwnerUnit);

	UFUNCTION(BlueprintCallable, Category = "Commands")
	void SetUnitCommand(TEnumAsByte<ECommand> MicroCommand, FVector Location, AGL_Unit* Unit);

	UFUNCTION(BlueprintCallable, Category ="Unit")
	float GetUnitParam(EUnitParam Param);

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Unit")
	float UnitRadius;

	UPROPERTY(EditAnywhere,BlueprintReadOnly, Category = "Unit")
	uint8 UnitTeamID;

	UPROPERTY(Replicated,EditDefaultsOnly, BlueprintReadOnly, Category = "Unit")
	FName UnitKey;

};
