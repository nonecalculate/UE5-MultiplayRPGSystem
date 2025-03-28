#include "AbilitySystem/RPGAbilitySystemComponent.h"

void URPGAbilitySystemComponent::AddCharacterAbilities(const TArray<TSubclassOf<UGameplayAbility>>& AbilitiesToGrant)
{
	for (const TSubclassOf<UGameplayAbility>& Ability : AbilitiesToGrant)
	{
		FGameplayAbilitySpec AbilitySpec = FGameplayAbilitySpec(Ability, 1.f); // 1.f 는 레벨 1이라는 의미라는데. 무엇인지는 더 알아야할듯
			GiveAbility(AbilitySpec);
	}
}

void URPGAbilitySystemComponent::AddCharacterPassiveAbilities(const TArray<TSubclassOf<UGameplayAbility>>& PassiveToGrant)
{
	for (const TSubclassOf<UGameplayAbility>& Ability : PassiveToGrant)
	{
		FGameplayAbilitySpec AbilitySpec = FGameplayAbilitySpec(Ability, 1.f); // 1.f 는 레벨 1이라는 의미라는데. 무엇인지는 더 알아야할듯
		GiveAbilityAndActivateOnce(AbilitySpec);
	}
}

void URPGAbilitySystemComponent::InitializeDefaultAttribute(const TSubclassOf<UGameplayEffect> AttributeEffect)
{
	checkf(AttributeEffect, TEXT("No valid default attribute for this Character %s"), *GetAvatarActor()->GetName());
	const FGameplayEffectContextHandle ContextHandle = MakeEffectContext();
	FGameplayEffectSpecHandle SpecHandle = MakeOutgoingSpec(AttributeEffect, 1.f, ContextHandle);
	ApplyGameplayEffectSpecToSelf(*SpecHandle.Data.Get());
}
