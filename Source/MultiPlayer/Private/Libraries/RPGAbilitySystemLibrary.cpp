#include "Libraries/RPGAbilitySystemLibrary.h"
#include "Game/GameMode/RPGGameMode.h"
#include "Kismet/GameplayStatics.h"

UCharacterClassInfo* URPGAbilitySystemLibrary::GetCharacterClassDefaultInfo(const UObject* WorldContextObject)
{
	if (const ARPGGameMode* RPGGamemode = Cast<ARPGGameMode>(UGameplayStatics::GetGameMode(WorldContextObject)))
	{
		return RPGGamemode->GetCharacterClassDefaultInfo();
	}

	return nullptr;
}
