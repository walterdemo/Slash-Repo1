// Fill out your copyright notice in the Description page of Project Settings.

#pragma once


#include "CoreMinimal.h"
#include "Items/Items.h"
#include "Weapon.generated.h"

 /*CommentSomething here
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
	virtual void BeginPlay() override;

	//virtual if we are planning to override it on child class
	virtual void OnSphereOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) override;
	virtual void OnSphereEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex) override;
	
	
	UFUNCTION() // the ones above already are declared as UFUNCTION in parent class
	void OnBoxOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

private:
	//EditAnywhere when its something that you can set , and we want to change them
	UPROPERTY(EditAnywhere, Category ="Weapon Properties")
	TObjectPtr<USoundBase> EquipSound;
	
	//VisibleAnywhere when its something to set
	UPROPERTY(VisibleAnywhere, Category = "Weapon Properties")
	TObjectPtr<UBoxComponent> WeaponBox;

	//empty pointers for tracing
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<USceneComponent> BoxTraceStart;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<USceneComponent> BoxTraceEnd;

public:
	//getter for accesing this variable from slashCharacter
	FORCEINLINE TObjectPtr<UBoxComponent> GetWeaponBox() const { return WeaponBox; }


};
