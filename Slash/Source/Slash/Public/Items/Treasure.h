// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Items/Items.h"
#include "Treasure.generated.h"

/**
 * 
 */
UCLASS()
class SLASH_API ATreasure : public AItems
{
	GENERATED_BODY()
	
protected:
	virtual void OnSphereOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) override;

private:
	//UPROPERTY(EditAnywhere, Category = Sounds)
	//TObjectPtr<USoundBase> PickUpSound;//set on Items

	UPROPERTY(EditAnywhere, Category = "Treasure Properties")
	int32 Gold;

public:
	FORCEINLINE int32 GetGold() const { return Gold; }

};
