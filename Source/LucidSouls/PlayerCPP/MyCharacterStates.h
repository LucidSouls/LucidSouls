#pragma once

UENUM(BlueprintType)
enum class ECharacterWeaponState : uint8 {
	State_WeaponUnequipped UMETA(DisplayName = "Unequipped"),
	State_WeaponEquipped UMETA(DisplayName = "Equipped"),
};

UENUM(BlueprintType)
enum class ECharacterActionState : uint8 {
	State_Unocuppied UMETA(DisplayName = "Unoccupied"),
	State_Atttacking UMETA(DisplayName = "Attacking")
};

UENUM(BlueprintType)
enum class ECharacterDeadPose : uint8 {
	Pose_Alive UMETA(DisplayName = "Alive"),
	Pose_Dead1 UMETA(DisplayName = "Dead1"),
	Pose_Dead2 UMETA(DisplayName = "Dead2"),
	Pose_Dead3 UMETA(DisplayName = "Dead3"),
	Pose_Dead4 UMETA(DisplayName = "Dead4"),
	Pose_Dead5 UMETA(DisplayName = "Dead5")
};

UENUM(BlueprintType)
enum class EEnemyState : uint8 {
	State_UnoccupiedPatrol UMETA(DisplayName = "UnoccupiedPatrol"),
	State_FollowPlayer UMETA(DisplayName = "FollowPlayer"),
	State_Attacking UMETA(DisplayName = "AttackingPlayer")
};