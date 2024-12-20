// Fill out your copyright notice in the Description page of Project Settings.


#include "Items/Weapons/Weapon.h"

#include "Characters/SlashCharacter.h"

//sound
#include "Kismet/GameplayStatics.h"

//modyfing sphere colission component from parent class
#include "Components/SphereComponent.h"

//modyfing sphere colission component from parent class
#include "Components/BoxComponent.h"

//trace by single
#include "Kismet/KismetSystemLibrary.h"


//hit interface
#include "Interfaces/HitInterface.h"

#include "NiagaraComponent.h"

AWeapon::AWeapon()
{
	WeaponBox = CreateDefaultSubobject<UBoxComponent>(TEXT("Weapon Box"));
	WeaponBox->SetupAttachment(GetRootComponent());
	WeaponBox->SetCollisionEnabled(ECollisionEnabled::NoCollision); // we will manage collision with notifying only in the moment of attacking
	//set all the column to overlap
	WeaponBox->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Overlap);
	//set just the pawn option into ignore
	WeaponBox->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Ignore);

	BoxTraceStart = CreateDefaultSubobject<USceneComponent>(TEXT("Box Trace Start"));
	BoxTraceStart->SetupAttachment(GetRootComponent());

	BoxTraceEnd = CreateDefaultSubobject<USceneComponent>(TEXT("Box Trace End"));
	BoxTraceEnd->SetupAttachment(GetRootComponent());

}	

void AWeapon::Equip(TObjectPtr<USceneComponent> InParent, FName InScoketName, AActor* NewOwner, APawn* NewInstigator)
{
	//owner and instigator usually are the same
	SetOwner(NewOwner);
	SetInstigator(NewInstigator);

	AttachMeshToSocket(InParent, InScoketName);
	ItemState = EItemState::EIS_Equiped;
	if (EquipSound) //this should be setted on blueprint
	{
		UGameplayStatics::PlaySoundAtLocation(
			this,
			EquipSound,
			GetActorLocation()
		);
	}
	if (Sphere)//comes from parent class
	{
		Sphere->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	}
	if (EmbersEffect)
	{
		EmbersEffect->Deactivate();
	}

}

void AWeapon::AttachMeshToSocket(TObjectPtr<USceneComponent> InParent, const FName InScoketName)
{
	//this lines were on equip but changed with RMB and refactoring creating anew definition
	//cause we are gonna use to attach to diferent sockets
	FAttachmentTransformRules TransformRules(EAttachmentRule::SnapToTarget, true);
	ItemMesh->AttachToComponent(InParent, TransformRules, InScoketName);
}

void AWeapon::BeginPlay()
{
	Super::BeginPlay();

	WeaponBox->OnComponentBeginOverlap.AddDynamic(this, &AWeapon::OnBoxOverlap);
}

void AWeapon::OnSphereEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	Super::OnSphereEndOverlap(OverlappedComponent, OtherActor, OtherComp, OtherBodyIndex);
	//We will cast an ASlashCharacter which was included up above called OtherActor, 
	//then create a pointer of an ASlashCharacter type into a variable called SlashCharacter
	
	
	//All this part is done in AWeapon now

	/*
	TObjectPtr<ASlashCharacter> SlashCharacter = Cast<ASlashCharacter>(OtherActor); 
	//checking if this variable is getting a valid addres, because its a pointer
	if (SlashCharacter)
	{	
		//this is an structure and need to specify  variables when creaing
		FAttachmentTransformRules TransformRules(EAttachmentRule::SnapToTarget, true);

		//We can acces Item Mesh because is protected and not private in item.h
		ItemMesh->AttachToComponent(SlashCharacter->GetMesh(), TransformRules, FName("RightHandSocket"));

	}*/
}

void AWeapon::OnSphereOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	Super::OnSphereOverlap(OverlappedComponent, OtherActor, OtherComp, OtherBodyIndex, bFromSweep,SweepResult);
}

void AWeapon::OnBoxOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	//Getting the actual position
	const FVector Start = BoxTraceStart->GetComponentLocation();
	const FVector End = BoxTraceEnd->GetComponentLocation();

	//this will be ignore in the trace
	TArray<AActor*> ActorsToIgnore;//this container can store multiple values of the type we specify in the brackets this time classes...
	ActorsToIgnore.Add(this);

	for (AActor* Actor : IgnoreActors) //loops in every component of ignoreActors
	{
		ActorsToIgnore.AddUnique(Actor); // AddUnique check is the same item was already part of the array
	}


	FHitResult BoxHit; // it creates a point on the hit location

	UKismetSystemLibrary::BoxTraceSingle(
		this,
		Start,
		End,
		FVector(5.f, 5.f, 5.f),
		BoxTraceStart->GetComponentRotation(),
		ETraceTypeQuery::TraceTypeQuery1,
		false, //trace by geometry
		ActorsToIgnore,
		EDrawDebugTrace::None,//it was: ForDuration but we dont want to see it anymore
		BoxHit,//this is an out paramater , this value will change,
		true
	);
	
	if (BoxHit.GetActor())
	{	
		//aplying damage first then playing animation
		UGameplayStatics::ApplyDamage(//this is a function from UGameplayStatics
			BoxHit.GetActor(),
			Damage,
			GetInstigator()->GetController(),
			this,
			UDamageType::StaticClass()
		);



		IHitInterface* HitInterface = Cast<IHitInterface>(BoxHit.GetActor());
		if (HitInterface)
		{
			//HitInterface->GetHit(BoxHit.ImpactPoint);
			// since we changes GetHit as blueprint native event
			HitInterface->Execute_GetHit(BoxHit.GetActor(), BoxHit.ImpactPoint);
		}
		IgnoreActors.AddUnique(BoxHit.GetActor()); // Add this actor to ignore  in boxtracesingle up here 
		/*
		TObjectPtr<IHitInterface> HitInterface = Cast<IHitInterface>(BoxHit.GetActor());
		if (HitInterface)
		{
			HitInterface->GetHit(BoxHit.ImpactPoint);
		}
		*/
		CreateFields(BoxHit.ImpactPoint);

		
	}
	
}
