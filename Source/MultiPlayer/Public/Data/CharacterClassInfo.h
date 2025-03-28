#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "GameplayTagContainer.h"
#include "CharacterClassInfo.generated.h"

//게임 플레이 태그를 적극적으로 사용하기 위함. 아직은 모르겠지만 아주 중요하고 유용하다고 함.

class UGameplayEffect;
class UGameplayAbility;

USTRUCT()
struct FCharacterClassDefaultInfo //캐릭터 클래스의 기본 정보 (속성, 능력, 패시브)
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UGameplayEffect> DefaultAttribute;

	UPROPERTY(EditDefaultsOnly)
	TArray<TSubclassOf<UGameplayAbility>> StartingAbilities;

	UPROPERTY(EditDefaultsOnly)
	TArray<TSubclassOf<UGameplayAbility>> StartingPassive;
};

UCLASS()
class MULTIPLAYER_API UCharacterClassInfo : public UDataAsset
{
	GENERATED_BODY()
	
public:
	
	UPROPERTY(EditDefaultsOnly)
	TMap<FGameplayTag, FCharacterClassDefaultInfo> ClassDefaultInfoMap;
};
