// Fill out your copyright notice in the Description page of Project Settings.


#include "Items/Weapons/Weapon.h"

#include "Characters/SlashCharacter.h"

//sound
#include "Kismet/GameplayStatics.h"

//modyfing sphere colission component from parent class
#include "Components/SphereComponent.h"


void AWeapon::Equip(TObjectPtr<USceneComponent> InParent, FName InScoketName)
{
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

}

void AWeapon::AttachMeshToSocket(TObjectPtr<USceneComponent> InParent, const FName InScoketName)
{
	//this lines were on equip but changed with RMB and refactoring creating anew definition
	//cause we are gonna use to attach to diferent sockets
	FAttachmentTransformRules TransformRules(EAttachmentRule::SnapToTarget, true);
	ItemMesh->AttachToComponent(InParent, TransformRules, InScoketName);
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
