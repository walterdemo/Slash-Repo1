// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"

#include "InputActionValue.h" //TODO : remember how and why I  did use this variable
//I think is because is a two dimensional Value and it has a different format
//
//bringing values like the Enum
#include "CharacterTypes.h"


#include <AMD/Amf/core/Variant.h>

#include "SlashCharacter.generated.h"




class UInputMappingContext;
class UInputAction;

class UCharMovementComponent;

//camera init
class USpringArmComponent;
class UCameraComponent;
class UGroomComponent;

class AItems;
class UAnimMontage;

class AWeapon;
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

UCLASS()
class SLASH_API ASlashCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ASlashCharacter();
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

protected:
	//void MoveForward(floatValue);

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


	//UPROPERTY(VisibleAnywhere)
	//TObjectPtr<UCharMovementComponent> MovementComponent;

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	/*
	Input action properties and variables
	*/
	void MoveChar(const FInputActionValue& Value);
	void Look(const FInputActionValue& Value);
	virtual void Jump() override;
	void EKeyPressed();
	void Attack();
	/*
	void Dodge();
	virtual void Attack() override;
	CommentSomething here
	*/

	/*
	* Play montage functions - this is called refactoring for avoid having to much code in one space
	*/
	void PlayAttackMontage();
	UFUNCTION(BlueprintCallable)
	void AttackEnd();
	bool CanAttack();

	void PlayEquipMontage(FName SectionName);
	bool CanDisarm();
	bool CanArm();
private:

	ECharacterState CharacterState = ECharacterState::ECS_Unequipped;
	
	UPROPERTY(BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	EActionState ActionState = EActionState::EAS_Unoccupied;

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

	/*
	*Animation montages
	*/
	UPROPERTY(EditDefaultsOnly, Category = Montages)
	TObjectPtr<UAnimMontage> AttackMontage;

	UPROPERTY(EditDefaultsOnly, Category = Montages)
	TObjectPtr<UAnimMontage> EquipMontage;

	UPROPERTY(VisibleAnywhere, Category = Weapon)
	TObjectPtr<AWeapon> EquippedWeapon;

public:
	//optimize code of a getter or setter in one line
	FORCEINLINE void SetOverlappingItem(AItems* Item) { OverlappingItem = Item; }

	//the const means that this function will not be able to change anything in it, maybe the Enum
	FORCEINLINE ECharacterState GetCharacterState() const { return CharacterState; } //Getting the value for being able to know it when it is called in SlashAnimInstance



};
