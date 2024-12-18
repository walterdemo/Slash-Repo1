// Fill out your copyright notice in the Description page of Project Settings.


#include "HUD/HealthBarComponent.h"
#include "HUD/HealthBar.h"
#include "Components/ProgressBar.h"

void UHealthBarComponent::SetHealthPercent(float Percent)
{
	if (HealthBarWidget == nullptr)//if there's no attribute yet
	{
		//with GetUserWidgetObject we are casting it the object which was set 
		//for passing from a widget pointer to a healthBar pointer we need to cast
		HealthBarWidget = Cast<UHealthBar>(GetUserWidgetObject());//we set the object on the blueprint so now we are casting it to this variable
	}
	if (HealthBarWidget && HealthBarWidget->HealthBar)
	{	
		//HealthBarWidget is the object/widget
		//Health Bar is a progress bar defined on HealthBar.h
		//SetPercent is a function o ProgressBar can be found on documentation
		HealthBarWidget->HealthBar->SetPercent(Percent);
	}

}
