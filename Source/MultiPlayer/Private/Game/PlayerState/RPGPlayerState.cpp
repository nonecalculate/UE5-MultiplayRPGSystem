#include "Game/PlayerState/RPGPlayerState.h"
#include "AbilitySystem/RPGAbilitySystemComponent.h"
#include "AbilitySystem/Attributes/RPGAttributeSet.h"

ARPGPlayerState::ARPGPlayerState()
{
	NetUpdateFrequency = 100.f; // ��Ƽ �÷��� ���ӿ����� �÷��̾��� �Ӽ� ��ȭ�� ������ �̷������ �ϱ� ������ ���� å��
	MinNetUpdateFrequency = 66.f; // ��� 66.f �ȿ��� �Ӽ� ��ȭ�� ����

	RPGAbilitySystemComp = CreateDefaultSubobject<URPGAbilitySystemComponent>("AbilitySystemComp");
	RPGAbilitySystemComp->SetIsReplicated(true);
	RPGAbilitySystemComp->SetReplicationMode(EGameplayEffectReplicationMode::Mixed); // ��Ƽ �÷��̿����� Replication Mode �� Mixed �� ������ ���� �̱��÷��� ������ �����Ѵٸ� Minimal �� �ξ �ȴ�.

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
