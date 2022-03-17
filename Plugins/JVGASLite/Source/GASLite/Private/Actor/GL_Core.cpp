// Fill out your copyright notice in the Description page of Project Settings.


#include "Actor/GL_Core.h"

AGL_Core::AGL_Core()
{
	BeamColor = FLinearColor::Black;
}

void AGL_Core::Tick(float DeltaTime)
{
	//Super::Tick(DeltaTime);
}

void AGL_Core::BeginPlay()
{
	Super::BeginPlay();
}
