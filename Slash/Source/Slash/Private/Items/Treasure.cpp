// Fill out your copyright notice in the Description page of Project Settings.


#include "Items/Treasure.h"
//#include "Characters/SlashCharacter.h"
#include "Interfaces/PickupInterface.h"
//use sound with hitSound and emitter
#include "Kismet/GameplayStatics.h"

void ATreasure::OnSphereOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	//TODO we have to fully understand how this works the castin
	//TObjectPtr<ASlashCharacter> SlashCharacter = Cast<ASlashCharacter>(OtherActor);
	//if (SlashCharacter)

	//now we are using interface which allows to bring a generic class instead of an specific actor, cause the actor now is "child" of this generic class
	// casting means to bring an object of type "OtherActor" into class specified in this case IPickupInterface
	IPickupInterface* PickupInterface = Cast<IPickupInterface>(OtherActor);
	if (PickupInterface)
	
	{
		SpawnPickupSound();//derived from Items parent class
		Destroy();
	}
}