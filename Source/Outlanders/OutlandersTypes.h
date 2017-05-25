
//#include "OutlandersTypes.generated.h"
#pragma once
#include "OutlandersTypes.generated.h"

UENUM()
namespace EAbilityAimTypes
{
	enum Type
	{
		AAT_FREE,
		AAT_FIXED,
		AAT_MOVEMENT_BASED,
		AAT_ACTORTARGET,
		AAT_FIXEDCONE,
		AAT_FIXEDSQUARE,
		AAT_INSTANT,
		AAT_NONE
	};
}

UENUM()
namespace EUpgradeTypes
{
	enum Type
	{
		// MaxValue += (Scalar * MaxValue)
		UT_AddPercentOfMax,  
		// MaxValue += (Scalar * BaseValue)
		UT_AddPercentOfBase,	 
		// MaxValue += Scalar
		UT_AddFixedValue,		 
		// MaxValue *= Scalar
		UT_Multiply,			 
		// MaxValue /= Scalar
		UT_Divide,				 
		// MaxValue = BaseValue
		UT_None					 
	};
}

USTRUCT(Blueprintable, Category=UpgradeableParameter)
struct FUpgradeableParameter
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(EditDefaultsOnly, Category = UpgradeableParameter)
		float BaseValue;

	UPROPERTY(EditDefaultsOnly, Category = UpgradeableParameter)
		float Scalar;

	UPROPERTY(EditDefaultsOnly, Category = UpgradeableParameter)
		TEnumAsByte<EUpgradeTypes::Type> UpgradeType;

	UPROPERTY(EditDefaultsOnly, Category = UpgradeableParameter)
		int32 LevelFrequency;

	UPROPERTY(BlueprintReadOnly, Category = UpgradeableParameter)
		float CurrentValue;

	UPROPERTY(BlueprintReadOnly, Category = UpgradeableParameter)
		float MaxValue;

public:
	FUpgradeableParameter() : BaseValue(0), Scalar(0), UpgradeType(EUpgradeTypes::UT_None), LevelFrequency(1), CurrentValue(0), MaxValue(0) {}

	FUpgradeableParameter(float baseValue, float scalar, EUpgradeTypes::Type upgradeType, int32 levelFrequency, float maxValue)
	{
		BaseValue = baseValue;
		Scalar = scalar;
		UpgradeType = upgradeType;
		LevelFrequency = levelFrequency;
		MaxValue = maxValue;
	}

	void Upgrade(int32 level)
	{
		if (level == 1)
		{
			MaxValue = BaseValue;
		}
		else if (LevelFrequency == 0 || (LevelFrequency > 0 && (level % LevelFrequency == 0)))
		{
			switch (UpgradeType)
			{
			case EUpgradeTypes::UT_AddPercentOfMax:
				MaxValue += FMath::FloorToInt(Scalar * MaxValue);
				break;
			case EUpgradeTypes::UT_AddPercentOfBase:
				MaxValue += FMath::FloorToInt(Scalar * BaseValue);
				break;
			case EUpgradeTypes::UT_AddFixedValue:
				MaxValue += Scalar;
				break;
			case EUpgradeTypes::UT_Multiply:
				MaxValue = FMath::FloorToInt(MaxValue * Scalar);
				break;
			case EUpgradeTypes::UT_Divide:
				if (Scalar != 0)
				{
					MaxValue = FMath::FloorToInt(MaxValue / Scalar);
				}
				break;
			case EUpgradeTypes::UT_None:
				MaxValue = BaseValue;
				break;

			default:
				break;					
			}
		}
	}
};