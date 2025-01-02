// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
//#include "GameFramework/Character.h" // changing to BaseCharacter 
#include "BaseCharacter.h"
#include "InputActionValue.h" //TODO : remember how and why I  did use this variable
//I think is because is a two dimensional Value and it has a different format
//
//bringing values like the Enum
#include "CharacterTypes.h"


#include <AMD/Amf/core/Variant.h>
#include "Interfaces/PickupInterface.h"
#include "SlashCharacter.generated.h"




class UInputMappingContext;
class UInputAction;

class UCharMovementComponent;

//camera init
class USpringArmComponent;
class UCameraComponent;
class UGroomComponent;

class ATreasure;
class ASoul;
class AItems;
class UAnimMontage;

class USlashOverlay;

/*
//We will declare it on CharacterTypesHeader.h
UENUM(BlueprintType) //a type that can be used in blueprint
enum class ECharacterState : uint8 //u because it's unsigned no negative values
{
	// the macro is tha name that will be shown in blueprint
	ECS_Unequipped UMETA(DisplayName = "Unequipped"),
	ECS_EquippedOneHandedWeapon UMETA(DisplayName = "Equipped One-Handed Weapon"),
	ECS_EquippedTwoHandedWeapon UMETA(DisplayName = "Equipped Two-Handed Weapon")
};*/


//IMPORTANCE OF INTERFACE
//if there are so many actor that need to be casted FOR using a function or propertie
//we can use the same interface to cast them so it gets into a better way and more generic way of doing it instead of casting different actors (which would need to be included on the top)

UCLASS()
class SLASH_API ASlashCharacter : public ABaseCharacter, public IPickupInterface // changed from ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ASlashCharacter();
	// Called every frame
	virtual void Tick(float DeltaTime) override;// nod needed but I will leave this here

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	virtual void GetHit_Implementation(const FVector& ImpactPoint, AActor* Hitter) override;
	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser) override;

	virtual void SetOverlappingItem(AItems* Item) override;
	virtual void AddSouls(ASoul* Soul) override;
	virtual void AddGold(ATreasure* Treasure) override;

protected:
	//void MoveForward(floatValue);

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;


	//UPROPERTY(VisibleAnywhere)
	//TObjectPtr<UCharMovementComponent> MovementComponent;

	
	/*
	Input action properties and variables
	*/
	void MoveChar(const FInputActionValue& Value);
	void Look(const FInputActionValue& Value);
	virtual void Jump() override;
	void EKeyPressed();
	virtual void Attack() override;
	
	/** Combat */
	void EquipWeapon(TObjectPtr<AWeapon>& OverlappingWeapon);
	//UFUNCTION(BlueprintCallable)// UFUNCTION only needed on BaseCharacter
	virtual void AttackEnd() override;
	virtual bool CanAttack() override;
	bool CanDisarm();
	bool CanArm();
	void Arm();
	void Disarm();
	void PlayEquipMontage(const FName& SectionName);
	
	virtual void Die() override;

	/*
	void Dodge();
	virtual void Attack() override;
	CommentSomething here
	*/

	/*
	* Play montage functions - this is called refactoring for avoid having to much code in one space
	*/



	//two handed weapon
	bool isWeapon1 = false;
	bool isWeapon2 = false;



	
	

	UFUNCTION(BlueprintCallable)
	void AttachWeaponToBack();

	UFUNCTION(BlueprintCallable)
	void AttachWeaponToHand();

	UFUNCTION(BlueprintCallable)
	void FinishEquipping();

	//ENHANCED INPUT
	/*
	Input action properties and variables
	*/
	UPROPERTY(EditAnywhere, Category = Input)
	TObjectPtr<UInputMappingContext> SlashContext;
	//UInputMappingContext* SlashContext; //old way of declaring apointer

	UPROPERTY(EditAnywhere, Category = Input)
	TObjectPtr<UInputAction> MovementAction;

	UPROPERTY(EditAnywhere, Category = Input)
	TObjectPtr<UInputAction> LookAction;

	UPROPERTY(EditAnywhere, Category = Input)
	TObjectPtr<UInputAction> JumpAction;

	UPROPERTY(EditAnywhere, Category = Input)
	TObjectPtr<UInputAction> EquipAction;

	UPROPERTY(EditAnywhere, Category = Input)
	TObjectPtr<UInputAction> AttackAction;

	UFUNCTION(BlueprintCallable)
	void HitReactEnd();



private:
	bool IsUnoccupied();
	void InitializeSlashOverlay();
	void SetHUDHealth();

	/** Character components */

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<USpringArmComponent> CameraBoom;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UCameraComponent> ViewCamera;
	
	
	UPROPERTY(VisibleAnywhere, Category = Hair)
	TObjectPtr<UGroomComponent> Hair;

	UPROPERTY(VisibleAnywhere, Category = Hair)
	TObjectPtr<UGroomComponent> Eyebrows;
	
	UPROPERTY(VisibleInstanceOnly)
	TObjectPtr<AItems> OverlappingItem;

	

	UPROPERTY(EditDefaultsOnly, Category = Montages)
	TObjectPtr<UAnimMontage> EquipMontage;


	ECharacterState CharacterState = ECharacterState::ECS_Unequipped;

	UPROPERTY(BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	EActionState ActionState = EActionState::EAS_Unoccupied;


	//Animation Montage fot two handed weapon
	UPROPERTY(EditDefaultsOnly, Category = Montages)
	TObjectPtr<UAnimMontage> AttackMontage2;

	UPROPERTY(EditDefaultsOnly, Category = Montages)
	TObjectPtr<UAnimMontage> EquipMontage2;

	UPROPERTY()
	USlashOverlay* SlashOverlay;

public://GETTER AND SETTER
	//optimize code of a getter or setter in one line
	//FORCEINLINE void SetOverlappingItem(AItems* Item) { OverlappingItem = Item; } //we are using interface now

	//the const means that this function will not be able to change anything in it, maybe the Enum
	FORCEINLINE ECharacterState GetCharacterState() const { return CharacterState; } //Getting the value for being able to know it when it is called in SlashAnimInstance

	FORCEINLINE EActionState GetActionState() const { return ActionState; }

};
