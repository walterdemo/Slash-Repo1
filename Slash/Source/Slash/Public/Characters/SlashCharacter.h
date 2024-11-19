// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"

#include "InputActionValue.h" //TODO : remember how and why I  did use this variable
//I think is because is a two dimensional Value and it has a different format
//
#include <AMD/Amf/core/Variant.h>

#include "SlashCharacter.generated.h"




class UInputMappingContext;
class UInputAction;

class UCharMovementComponent;

//camera init
class USpringArmComponent;
class UCameraComponent;


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

	UPROPERTY(EditAnywhere, Category = Input)
	TObjectPtr<UInputMappingContext> SlashContext;
	//UInputMappingContext* SlashContext; //old way of declaring apointer

	UPROPERTY(EditAnywhere, Category = Input)
	TObjectPtr<UInputAction> MovementAction;

	UPROPERTY(EditAnywhere, Category = Input)
	TObjectPtr<UInputAction> LookAction;

	void MoveChar(const FInputActionValue& Value);
	void Look(const FInputActionValue& Value);

	//UPROPERTY(VisibleAnywhere)
	//TObjectPtr<UCharMovementComponent> MovementComponent;

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

private:

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<USpringArmComponent> CameraBoom;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UCameraComponent> ViewCamera;


};
