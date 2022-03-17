// Fill out your copyright notice in the Description page of Project Settings.


#include "Actor/TLTower.h"
#include <Kismet/KismetMathLibrary.h>

// Sets default values
ATLTower::ATLTower()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ATLTower::BeginPlay()
{
	Super::BeginPlay();
	
}
void ATLTower::FindBestTarget()
{


}


// Called every frame
void ATLTower::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

