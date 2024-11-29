#pragma once

UENUM(BlueprintType) //a type that can be used in blueprint
enum class ECharacterState : uint8 //u because it's unsigned no negative values
{
	// the macro is tha name that will be shown in blueprint
	ECS_Unequipped UMETA(DisplayName = "Unequipped"),
	ECS_EquippedOneHandedWeapon UMETA(DisplayName = "Equipped One-Handed Weapon"),
	ECS_EquippedTwoHandedWeapon UMETA(DisplayName = "Equipped Two-Handed Weapon")
};

UENUM(BlueprintType) //we can cal it from the blueprints
enum class EActionState : uint8
{
	EAS_Unoccupied UMETA(DisplayName = "Unoccupied"),
	EAS_Attacking UMETA(DisplayName = "Attacking")
};