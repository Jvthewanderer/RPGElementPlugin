// Fill out your copyright notice in the Description page of Project Settings.


#include "UnitFactory/GL_LaneWrapper.h"
#include "UnitFactory/RelayPointWrapper.h"

void UGL_LaneWrapper::Init()
{
	for (ELanes Val : TEnumRange<ELanes>())
	{
		URelayPointWrapper* MyObj = NewObject<URelayPointWrapper>(URelayPointWrapper::StaticClass());
		
		Lanes.Add(Val,MyObj);
	}
}
