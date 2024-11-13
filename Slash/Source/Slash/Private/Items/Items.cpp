// Fill out your copyright notice in the Description page of Project Settings.


#include "Items/Items.h"
#include "DrawDebugHelpers.h"

#include "Slash/Slash.h"
#include "Slash/DebugMacros.h"

// Sets default values
AItems::AItems()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	ItemMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ItemMeshComponent"));
	RootComponent = ItemMesh;

}

// Called when the game starts or when spawned
void AItems::BeginPlay()
{
	Super::BeginPlay();


	/*
	UE_LOG(LogTemp, Warning, TEXT("Begin play called!"));

	if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(2, 60.f, FColor::Cyan, FString("Items on Screen Message!"));
	}

	
	UWorld* World = GetWorld();
	if (World)
	{
		FVector Location = GetActorLocation();
		DrawDebugSphere(World, Location, 25.f, 30, FColor::Red, false, 30.f);
	}
	*/

	UWorld* World = GetWorld();
	

	/*
	if (World)
	{		
		DrawDebugLine(World, Location, Location + Forward * 100.f, FColor::Red, true, -1.f, 0, 1.f);
	}
	*/

	
	
	//DRAW_LINE(Location, Location + Forward * 100.f);

	/*
	if (World)
	{
		DrawDebugPoint(World, Location + Forward * 100.f, 15.f, FColor::Red, true);
	}
	*/

	//DRAW_POINT(Location + Forward * 100.f);

	/*
	SetActorLocation(FVector(0.f, 0.f, 50.f));
	SetActorRotation(FRotator(0.f, 45.f, 0.f));
	*/

	/*
	FVector Location = GetActorLocation();
	FVector Forward = GetActorForwardVector();

	DRAW_SPHERE(Location);
	DRAW_VECTOR(Location, Location + Forward * 100.f);
	*/



	/*
	DRAW_SPHERE_COLOR(Location, FColor::Yellow);
	DRAW_ARROW(Location + Forward * 100.f, Location + Forward * 110)
	*/
}

float AItems::TransformedSin()
{
	return Amplitud * FMath::Sin(RunningTime * TimeConstant);
}

float AItems::TransformedCos()
{
	return Amplitud * FMath::Cos(RunningTime * TimeConstant);
}

// Called every frame
void AItems::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	/*
	//Movement rate in units of cms
	float MovementRate = 50.f;
	float RotationRate = 45.f;
		
	//MovementRace * DeltaTime(cm/s) * (s/frame) = (cm/frame)
	AddActorWorldOffset(FVector(MovementRate*DeltaTime, 0.f, 0.f));
	AddActorWorldRotation(FRotator(0.f,RotationRate * DeltaTime, 0.f));
	*/
	
	RunningTime += DeltaTime;

	//float DeltaZ = Amplitud * FMath::Sin(RunningTime * TimeConstant); //period = 2*pi/K

	//AddActorWorldOffset(FVector(0.f, 0.f, DeltaZ));
	
	/*
	DRAW_SPHERE_SingleFrame(GetActorLocation());
	DRAW_LINE_SingleFrame(GetActorLocation(), GetActorLocation() + GetActorForwardVector() * 100.f);
	DRAW_POINT_SingleFrame(GetActorLocation() + GetActorForwardVector() * 100.f);

	FVector AvgVector = Avg<FVector>(GetActorLocation(), FVector::ZeroVector);
	DRAW_POINT_SingleFrame(AvgVector);
	*/

	/*
	UE_LOG(LogTemp, Warning, TEXT("DeltaTime: %f"), DeltaTime);

	if (GEngine)
	{
		FString Name = GetName();
		FString Message = FString::Printf(TEXT("Item Name: %s"), *Name);
		GEngine->AddOnScreenDebugMessage(1, 60.f, FColor::Cyan, Message);

		UE_LOG(LogTemp, Warning, TEXT("Item Name: %s"), *Name);
	}
	*/

	//Rotation Challenge
	AddActorWorldRotation(FRotator(0.f, RotationSpeed * DeltaTime, 0.f));


}

