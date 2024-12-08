// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Items/Items.h"
#include "Weapon.generated.h"

/**
 *CommentSomething here
 */
class USoundBase;

class UBoxComponent;

UCLASS()
class SLASH_API AWeapon : public AItems
{
	GENERATED_BODY()
public:
	AWeapon();

	void Equip(TObjectPtr<USceneComponent> InParent, FName InScoketName);

	void AttachMeshToSocket(TObjectPtr<USceneComponent> InParent, const FName InScoketName);

protected:
	virtual void OnSphereEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex) override;

	virtual void OnSphereOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) override;
private:
	UPROPERTY(EditAnywhere, Category ="Weapon Properties")
	TObjectPtr<USoundBase> EquipSound;
	
	UPROPERTY(VisibleAnywhere, Category = "Weapon Properties")
	TObjectPtr<UBoxComponent> WeaponBox;

};
