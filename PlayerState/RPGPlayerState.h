#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemInterface.h"
#include "GameFramework/PlayerState.h"
#include "RPGPlayerState.generated.h"

class URPGAbilitySystemComponent;
class URPGAttributeSet;

UCLASS()
class MULTIPLAYER_API ARPGPlayerState : public APlayerState, public IAbilitySystemInterface // 생성시 AbilitySystemInterface 헤더파일 추가
{
	GENERATED_BODY()
	
public:

	ARPGPlayerState();

	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override; // 상속?

	UFUNCTION(BlueprintPure)
	URPGAbilitySystemComponent* GetRPGAbilitySystemComponent() const; // 이 클래스에서 가져옴

	UFUNCTION(BlueprintPure)
	URPGAttributeSet* GetRPGAttributes() const;


private:

	UPROPERTY(BlueprintReadOnly, meta = (AllowPrivateAccess = true))
	TObjectPtr<URPGAbilitySystemComponent> RPGAbilitySystemComp;

	UPROPERTY(BlueprintReadOnly, meta = (AllowPrivateAccess = true))
	TObjectPtr<URPGAttributeSet> RPGAttributes;
};
