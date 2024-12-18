// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "HealthBar.generated.h"

/**
 * 
 */
UCLASS()
class SLASH_API UHealthBar : public UUserWidget
{
	GENERATED_BODY()
	
public:
	//we created this cpp after created the wpb_healthbar
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UProgressBar> HealthBar;//if we already created the progress bar in the widget nake sure that the name remains the same

};
