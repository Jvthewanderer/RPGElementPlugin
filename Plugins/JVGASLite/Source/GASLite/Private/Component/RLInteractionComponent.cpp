// Fill out your copyright notice in the Description page of Project Settings.


#include "Component/RLInteractionComponent.h"
#include "Interface/TLGameplayInterface.h"
#include <DrawDebugHelpers.h>
#include "UI/TLWorldUserWidget.h"

static TAutoConsoleVariable<bool> CVarDebugDrawInteractor(TEXT("tl.InteractionDebugDraw"),false,TEXT("Draw Debug for Interactor Component"), ECVF_Cheat);

// Sets default values for this component's properties
URLInteractionComponent::URLInteractionComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	eyeDistance = 1000.0f;

	TraceRadius = 30.0f;

	CollisionChannel = ECC_WorldDynamic;


	// ...
}


// Called when the game starts
void URLInteractionComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}

// Called every frame
void URLInteractionComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...

	APawn* MyPawn = Cast<APawn>(GetOwner());
	if (MyPawn->IsLocallyControlled())
	{
		FindBestInteractable();
	}
}

void URLInteractionComponent::FindBestInteractable()
{
	bool bDebugDraw = CVarDebugDrawInteractor.GetValueOnGameThread();

	FCollisionObjectQueryParams ObjectQueryParams;
	ObjectQueryParams.AddObjectTypesToQuery(CollisionChannel);

	AActor* MyOwner = GetOwner();

	FVector EyeLocation;
	FRotator EyeRotation;

	MyOwner->GetActorEyesViewPoint(EyeLocation, EyeRotation);

	FVector Start = EyeLocation;
	FVector End = EyeLocation + (EyeRotation.Vector() * eyeDistance);

	//FHitResult Hit;
	//bool bBlockingHit = GetWorld()->LineTraceSingleByObjectType(Hit, Start, End, ObjectQueryParams);

	TArray<FHitResult> Hits;

	float Radius = TraceRadius;

	FCollisionShape Shape;
	Shape.SetSphere(Radius);

	bool bBlockingHit = GetWorld()->SweepMultiByObjectType(Hits, Start, End, FQuat::Identity, ObjectQueryParams, Shape);

	FColor LineColor = bBlockingHit ? FColor::Green : FColor::Red;

	//Clear ref before Find focus
	FocusActor = nullptr;
	for (FHitResult Hit : Hits)
	{
		if (bDebugDraw)
		{
			DrawDebugSphere(GetWorld(), Hit.ImpactPoint, Radius, 32, LineColor, false, 2.0f);
		}

		AActor* HitActor = Hit.GetActor();
		if (HitActor)
		{
			if (HitActor->Implements<UTLGameplayInterface>())
			{
				FocusActor = HitActor;
				break;
			}
		}
	}


	if (FocusActor)
	{
		if (DefaultWidgetInstance == nullptr && DefaultWidgetClass)
		{
			DefaultWidgetInstance = CreateWidget<UTLWorldUserWidget>(GetWorld(), DefaultWidgetClass);
		}

		if (DefaultWidgetInstance)
		{
			DefaultWidgetInstance->AttachedActor = FocusActor;
			if (!DefaultWidgetInstance->IsInViewport())
			{
				DefaultWidgetInstance->AddToViewport();
			}
		}

	}
	else
	{
		if (DefaultWidgetInstance)
		{
			DefaultWidgetInstance->RemoveFromParent();
		}
	}

	if (bDebugDraw)
	{
		DrawDebugLine(GetWorld(), EyeLocation, End, LineColor, false, 2.0f, 0, 2.0f);
	}

}

void URLInteractionComponent::PrimaryInteract()
{
	ServerInteract(FocusActor);

}

void URLInteractionComponent::ServerInteract_Implementation(AActor* InFocus)
{
	if (InFocus == nullptr)
	{
		GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Red, "No Focus Actor to interact.");
		return;
	}

	APawn* MyPawn = Cast<APawn>(GetOwner());

	ITLGameplayInterface::Execute_Interact(InFocus, MyPawn);
}