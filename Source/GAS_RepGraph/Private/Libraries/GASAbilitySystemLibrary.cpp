// Fill out your copyright notice in the Description page of Project Settings.


#include "Libraries/GASAbilitySystemLibrary.h"

#include "GAS_RepGraph/GAS_RepGraphGameMode.h"
#include "Kismet/GameplayStatics.h"

UCharacterClassInfo* UGASAbilitySystemLibrary::GetCharacterClassDefaultInfo(const UObject* WorldContextObject)
{
	// Получаем текущий режим игры и преобразуем его к нашей пользовательской реализации.
	// Режим игры существует только на сервере, поэтому на клиентах будет возвращен nullptr.
	if (const AGAS_RepGraphGameMode* GASGameMode = Cast<AGAS_RepGraphGameMode>(UGameplayStatics::GetGameMode(WorldContextObject)))
	{
		// Получаем данные конфигурации класса из игрового режима.
		return GASGameMode->GetCharacterClassDefaultInfo();
	}
	
	// Резервный вариант для клиентов или при некорректном контексте мира.
	return nullptr;
}

