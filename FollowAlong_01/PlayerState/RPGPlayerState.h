#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemInterface.h"
#include "GameFramework/PlayerState.h"
#include "RPGPlayerState.generated.h"

class URPGAbilitySystemComponent;
class URPGAttributeSet;

UCLASS()
class MULTIPLAYER_API ARPGPlayerState : public APlayerState, public IAbilitySystemInterface // ������ AbilitySystemInterface ������� �߰�
{
	GENERATED_BODY()
	
public:

	ARPGPlayerState();

	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override; // ���?

	UFUNCTION(BlueprintPure)
	URPGAbilitySystemComponent* GetRPGAbilitySystemComponent() const; // �� Ŭ�������� ������

	UFUNCTION(BlueprintPure)
	URPGAttributeSet* GetRPGAttributes() const;


private:

	UPROPERTY(BlueprintReadOnly, meta = (AllowPrivateAccess = true))
	TObjectPtr<URPGAbilitySystemComponent> RPGAbilitySystemComp;

	UPROPERTY(BlueprintReadOnly, meta = (AllowPrivateAccess = true))
	TObjectPtr<URPGAttributeSet> RPGAttributes;
};
