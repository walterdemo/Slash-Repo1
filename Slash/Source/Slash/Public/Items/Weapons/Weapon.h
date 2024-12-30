// Fill out your copyright notice in the Description page of Project Settings.

#pragma once


#include "CoreMinimal.h"
#include "Items/Items.h"
#include "Weapon.generated.h"

 /*CommentSomething here
 */
class USoundBase;

class UBoxComponent;

UENUM()
enum class EWeaponType : uint8
{
	EWT_OneHanded UMETA(DisplayName = "OneHanded"),
	EWT_TwoHanded UMETA(DisplayName = "TwoHanded")
};


UCLASS()
class SLASH_API AWeapon : public AItems
{
	GENERATED_BODY()
public:
	AWeapon();

	void Equip(TObjectPtr<USceneComponent> InParent, FName InScoketName, AActor* NewOwner, APawn* NewInstigator);

	void DisableSphereCollision();

	void DeactivateEmbers();

	void PlayEquipSound();

	void AttachMeshToSocket(TObjectPtr<USceneComponent> InParent, const FName InScoketName);

	//public to use this variable in SlashCharacters when collision ends
	TArray<AActor*> IgnoreActors;

protected:
	virtual void BeginPlay() override;

		
	UFUNCTION() // the ones above already are declared as UFUNCTION in parent class
	void OnBoxOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	bool ActorIsSameType(AActor* OtherActor);

	void ExecuteGetHit(FHitResult& BoxHit);


	UFUNCTION(BlueprintImplementableEvent)
	void CreateFields(const FVector& FieldLocation);


private:
	void BoxTrace(FHitResult& BoxHit);

	UPROPERTY(EditAnywhere, Category = "Weapon Properties")
	FVector BoxTraveExtent = FVector(5.f);

	UPROPERTY(EditAnywhere, Category = "Weapon Properties")
	bool bShowBoxDebug = false;

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

	UPROPERTY(EditAnywhere, Category = "Weapon Properties")
	float Damage = 20.f; 

	UPROPERTY(EditAnywhere, Category = "Weapon Properties")
	EWeaponType WeaponType = EWeaponType::EWT_OneHanded;

public:
	//getter for accesing this variable from slashCharacter
	FORCEINLINE TObjectPtr<UBoxComponent> GetWeaponBox() const { return WeaponBox; }

	FORCEINLINE EWeaponType GetWeaponType() const { return WeaponType; }
};
