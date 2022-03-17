// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Actor/TLTowerBase.h"
#include "GL_Core.generated.h"

/**
 * 
 */
UCLASS()
class GASLITE_API AGL_Core : public ATLTowerBase
{
	GENERATED_BODY()

	public:

		AGL_Core();
		
		void Tick(float DeltaTime) override;

	protected:
	
		void BeginPlay() override;

};
