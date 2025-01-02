// Fill out your copyright notice in the Description page of Project Settings.


#include "Items/Items.h"
#include "DrawDebugHelpers.h"

#include "Slash/Slash.h"
#include "Slash/DebugMacros.h"

//Sphere component
#include "Components/SphereComponent.h"
//#include "Characters/SlashCharacter.h"
#include "Interfaces/PickupInterface.h"
#include "NiagaraComponent.h"

#include "NiagaraFunctionLibrary.h"//using objects of type niagara (input objects)
#include "Kismet/GameplayStatics.h"// 

// Sets default values
AItems::AItems()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	ItemMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ItemMeshComponent"));
	ItemMesh->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
	ItemMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);


	RootComponent = ItemMesh;

	Sphere = CreateDefaultSubobject<USphereComponent>(TEXT("Sphere"));
	Sphere->SetupAttachment(GetRootComponent());

	ItemEffect = CreateDefaultSubobject<UNiagaraComponent>(TEXT("Embers"));
	ItemEffect->SetupAttachment(GetRootComponent());
}

// Called when the game starts or when spawned
void AItems::BeginPlay()
{
	Super::BeginPlay();

	//accesing its delegate, passing the value to the function OnSphereOverlap of this code
	Sphere->OnComponentBeginOverlap.AddDynamic(this, &AItems::OnSphereOverlap);
	Sphere->OnComponentEndOverlap.AddDynamic(this, &AItems::OnSphereEndOverlap);

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

	//UWorld* World = GetWorld();
	

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

void AItems::OnSphereOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	//TODO we have to fully understand how this works he castin
	
	/*before using interfaces
	TObjectPtr<ASlashCharacter> SlashCharacter = Cast<ASlashCharacter>(OtherActor);
	if (SlashCharacter)
	{
		SlashCharacter->SetOverlappingItem(this);
	}
	*/
	/*
	const FString OtherActorName = OtherActor->GetName();
	if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(1, 1.f, FColor::Red, OtherActorName);
	}
	*/

	//IMPORTANCE OF INTERFACE
	//if there are so many actor that need to be casted FOR using a function or propertie
	//we can use the same interface to cast them so it gets into a better way and more generic way of doing it instead of casting different actors (which would need to be included on the top)
	IPickupInterface* PickupInterface = Cast<IPickupInterface>(OtherActor);//the actor SlashCharacter now is child of I pickupinterface so it can be casted as this
	if (PickupInterface)
	{
		PickupInterface->SetOverlappingItem(this);
	}
}

void AItems::OnSphereEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	/*
	TObjectPtr<ASlashCharacter> SlashCharacter = Cast<ASlashCharacter>(OtherActor);
	if (SlashCharacter)
	{
		SlashCharacter->SetOverlappingItem(nullptr);
	}
	*/

	//const FString OtherActorNameEnd = OtherActor->GetName();
	/*
	if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(1, 1.f, FColor::Green, OtherActorNameEnd);
	}
	*/

	//IMPORTANCE OF INTERFACE
	//if there are so many actor that need to be casted FOR using a function or propertie
	//we can use the same interface to cast them so it gets into a better way and more generic way of doing it instead of casting different actors (which would need to be included on the top)


	IPickupInterface* PickupInterface = Cast<IPickupInterface>(OtherActor);
	if (PickupInterface)
	{
		PickupInterface->SetOverlappingItem(nullptr);
	}
}

void AItems::SpawnPickupSystem()
{
	if (PickupEffect)
	{
		UNiagaraFunctionLibrary::SpawnSystemAtLocation(
			this,
			PickupEffect,
			GetActorLocation()
		);
	}
}
void AItems::SpawnPickupSound()
{
	if (PickupSound)
	{
		UGameplayStatics::SpawnSoundAtLocation(
			this,
			PickupSound,
			GetActorLocation()
		);
	}
}

// Called every frame
void AItems::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (ItemState == EItemState::EIS_Hovering)
	{
		AddActorWorldOffset(FVector(0.f, 0.f, TransformedSin()));
	}


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
	//AddActorWorldRotation(FRotator(0.f, RotationSpeed * DeltaTime, 0.f));


}

