#include "Game/PlayerState/RPGPlayerState.h"
#include "AbilitySystem/RPGAbilitySystemComponent.h"
#include "AbilitySystem/Attributes/RPGAttributeSet.h"

ARPGPlayerState::ARPGPlayerState()
{
	NetUpdateFrequency = 100.f; // 멀티 플레이 게임에서는 플레이어의 속성 변화가 빠르게 이루어져야 하기 때문에 높게 책정
	MinNetUpdateFrequency = 66.f; // 적어도 66.f 안에는 속성 변화가 적용

	RPGAbilitySystemComp = CreateDefaultSubobject<URPGAbilitySystemComponent>("AbilitySystemComp");
	RPGAbilitySystemComp->SetIsReplicated(true);
	RPGAbilitySystemComp->SetReplicationMode(EGameplayEffectReplicationMode::Mixed); // 멀티 플레이에서는 Replication Mode 를 Mixed 로 두지만 만약 싱글플레이 게임을 상정한다면 Minimal 로 두어도 된다.

	RPGAttributes = CreateDefaultSubobject<URPGAttributeSet>("AttributeSet");
}

UAbilitySystemComponent* ARPGPlayerState::GetAbilitySystemComponent() const
{
	return RPGAbilitySystemComp;
}

URPGAbilitySystemComponent* ARPGPlayerState::GetRPGAbilitySystemComponent() const
{
	return RPGAbilitySystemComp;
}

URPGAttributeSet* ARPGPlayerState::GetRPGAttributes() const
{
	return RPGAttributes;
}
