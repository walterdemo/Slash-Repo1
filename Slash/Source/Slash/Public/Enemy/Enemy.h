// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Interfaces/HitInterface.h" // inheritance from another parent
#include "Enemy.generated.h"

class UAnimMontage;

UCLASS()
class SLASH_API AEnemy : public ACharacter, public IHitInterface // inheritance from another parent
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AEnemy();
	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;


	virtual void GetHit(const FVector& ImpactPoint) override;
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	/*
	* Play montage functions - this is called refactoring for avoid having to much code in one space
	*/
	void PlayHitReactMontage(const FName& SectionName);

	void DirectionalHitReact(const FVector& ImpactPoint);
private:
	/*
	*Animation montages
	*/
	UPROPERTY(EditDefaultsOnly, Category = Montages)
	TObjectPtr<UAnimMontage> HitReactMontage;

	UPROPERTY(EditAnywhere, Category = Sounds)
	TObjectPtr<USoundBase> HitSound;



public:	
	

};
