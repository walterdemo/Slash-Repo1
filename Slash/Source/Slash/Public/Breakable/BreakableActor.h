// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Interfaces/HitInterface.h" // inheritance from another parent
#include "BreakableActor.generated.h"

class UGeometryCollectionComponent;

UCLASS()
class SLASH_API ABreakableActor : public AActor, public IHitInterface // inheritance from another parent
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ABreakableActor();

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	//virtual void GetHit(const FVector& ImpactPoint) override;
	//since GetHit is a Blueprint native event we should use this new which is internally generated
	virtual void GetHit_Implementation(const FVector& ImpactPoint, AActor* Hitter) override;


protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	TObjectPtr<UGeometryCollectionComponent> GeometryCollection;


	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	TObjectPtr<class UCapsuleComponent> Capsule;


private:	
	

	UPROPERTY(EditAnywhere, Category = "Breakable property")
	TArray<TSubclassOf<class ATreasure>> TreasureClasses;
	//not anymore
	//TSubclassOf<class ATreasure> TreasureClass;// this set the variaable to be only related to any ATreasre BP class
	//TObjectPtr<UClass> TreasureClass;//not anymore

	bool bBroken = false;

};
