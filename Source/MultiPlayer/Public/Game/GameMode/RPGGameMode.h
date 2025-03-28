#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameMode.h"
#include "RPGGameMode.generated.h"

class UCharacterClassInfo;

UCLASS()
class MULTIPLAYER_API ARPGGameMode : public AGameMode
{
	GENERATED_BODY()
	
public:

	UCharacterClassInfo* GetCharacterClassDefaultInfo() const;

private:

	UPROPERTY(EditDefaultsOnly, Category= "Custom Values|Class Defaults")
	TObjectPtr<UCharacterClassInfo> ClassDefaults;
};
