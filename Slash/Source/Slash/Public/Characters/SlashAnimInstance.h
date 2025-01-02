// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "CharacterTypes.h" //included for being ablo to use the enum ECharacterState

#include "SlashAnimInstance.generated.h"

/**
 * 
 */

//forwardDeclare
class ASlashCharacter;
class UCharacterMovementComponent;

UCLASS()
class SLASH_API USlashAnimInstance : public UAnimInstance
{
	GENERATED_BODY()
	
public:
	virtual void NativeInitializeAnimation() override;
	virtual void NativeUpdateAnimation(float DeltaTime) override;

	UPROPERTY(BlueprintReadOnly)
	TObjectPtr<ASlashCharacter> SlashCharacter; //It also can be: class ASlashCharacter* SlashCharacter;
	
	UPROPERTY(BlueprintReadOnly, Category = Movement)
	TObjectPtr<UCharacterMovementComponent> SlashCharacterMovement;

	UPROPERTY(BlueprintReadOnly, Category = Movement)
	float GroundSpeed;

	UPROPERTY(BlueprintReadOnly, Category = Movement)
	bool IsFalling;

	UPROPERTY(BlueprintReadOnly, Category = Movement)
	ECharacterState CharacterState;

	UPROPERTY(BlueprintReadOnly, Category = Movement)
	EActionState ActionState;

	UPROPERTY(BlueprintReadOnly, Category = Movement)
	TEnumAsByte<EDeathPose> DeathPose;
};
