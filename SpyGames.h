#pragma once
#include "ue.h"
#include "Inventory.h"

enum class EPerkSystemSelections : unsigned int
{
	First = 0,
	Second = 1,
	Third = 2,
	CloseError = 500u,
	TimeoutError = 501u,
	ReRoll = 11u,
	StartSafetyTimer = 502u,
};

void (*UnlockPerkForPlayerOG)(AFortAthenaMutator_PerkSystemMutator* PerkSystemMutator, AFortPlayerControllerAthena* a2, int a3, char a4);
void UnlockPerkForPlayer(AFortAthenaMutator_PerkSystemMutator* PerkSystemMutator, AFortPlayerControllerAthena* PC, int PerkAmount, char a4)
{ 
	Log("UnlockPerkForPlayer called!");
	cout << __int64(_ReturnAddress()) - __int64(GetModuleHandleW(0)) << endl;
	
	UFortAbilitySystemComponent* AbilitySystemComponent = *(UFortAbilitySystemComponent**)(__int64(PC->PlayerState) + 0x900);
	UFortControllerComponent_PerkSystem* Comp = (UFortControllerComponent_PerkSystem*)PC->GetComponentByClass(UFortControllerComponent_PerkSystem::StaticClass());

	UnlockPerkForPlayerOG(PerkSystemMutator, PC, PerkAmount, a4);

	for (size_t i = 0; i < Comp->PerkSelection.Num(); i++)
	{
		Comp->PerkSelection[i].Items.FreeArray();
		Comp->PerkSelection[i].Items.Add(Comp->SpyTechArray[i]);
		Comp->PerkSelection[i].Items.Add(Comp->SpyTechArray[i]);
	}
	Comp->RoundStartCache.bDataReady = true;
	Comp->OnRep_PerkSelection();

	if (PC->IsInAircraft())
	{
		Comp->ServerTimeToRelease = GetStatics()->GetTimeSeconds(GetWorld()) + 30;
		Comp->OnRep_ServerTimeToRelease();
	}

	AbilitySystemComponent->BP_ApplyGameplayEffectToSelf(PerkSystemMutator->MutatorData.PerkUnlockedGameplayEffectClass, 1, FGameplayEffectContextHandle());

	if(PC->IsInAircraft())
		ShowPerkSelect(PerkSystemMutator, PC);
}

void (*OnMutatorItemSelectedOG)(AFortAthenaMutator_PerkSystemMutator* Mutator, AFortPlayerControllerAthena* PC, EPerkSystemSelections EnumIdk);
void OnMutatorItemSelected(AFortAthenaMutator_PerkSystemMutator* Mutator, AFortPlayerControllerAthena* PC, EPerkSystemSelections EnumIdk)
{
	cout << "OnMutatorItemSelected called " << (uintptr_t)EnumIdk << endl;
	cout << __int64(_ReturnAddress()) - __int64(GetModuleHandleW(0)) << endl;

	EnumIdk = EPerkSystemSelections::First;

	UFortControllerComponent_PerkSystem* Comp = (UFortControllerComponent_PerkSystem*)PC->GetComponentByClass(UFortControllerComponent_PerkSystem::StaticClass());
	UFortAbilitySystemComponent* AbilitySystemComponent = *(UFortAbilitySystemComponent**)(__int64(PC->PlayerState) + 0x900);

	switch (EnumIdk)
	{
	case EPerkSystemSelections::CloseError:
	case EPerkSystemSelections::TimeoutError:
		OnMutatorItemSelectedOG(Mutator, PC, EnumIdk);
		AbilitySystemComponent->RemoveActiveGameplayEffectBySourceEffect(Mutator->MutatorData.PerkUnlockedGameplayEffectClass, AbilitySystemComponent, 1);
		AbilitySystemComponent->RemoveActiveGameplayEffectBySourceEffect(Mutator->MutatorData.ShowPerkSelectGameplayEffectClass, AbilitySystemComponent, 1);

		Comp->AllSelectedPerks.Add(Comp->PerkSelection[1].Items[0]);
		Comp->OnRep_AllSelectedPerks();
		break;
	case EPerkSystemSelections::ReRoll:
		for (size_t i = 0; i < Comp->PerkSelection.Num(); i++)
		{
			Comp->PerkSelection[i].Items.FreeArray();
			Comp->PerkSelection[i].Items.Add(Comp->SpyTechArray[0]);
			Comp->PerkSelection[i].Items.Add(Comp->SpyTechArray[i]);
		}
		Comp->RoundStartCache.bDataReady = true;
		Comp->OnRep_PerkSelection();
		OnMutatorItemSelectedOG(Mutator, PC, EnumIdk);
		break;
	case EPerkSystemSelections::StartSafetyTimer:
		OnMutatorItemSelectedOG(Mutator, PC, EnumIdk);
		Log("idek");
		break;
	case EPerkSystemSelections::First:
	case EPerkSystemSelections::Second:
	case EPerkSystemSelections::Third:
		OnMutatorItemSelectedOG(Mutator, PC, EnumIdk);
		AbilitySystemComponent->RemoveActiveGameplayEffectBySourceEffect(Mutator->MutatorData.PerkUnlockedGameplayEffectClass, AbilitySystemComponent, 1);
		AbilitySystemComponent->RemoveActiveGameplayEffectBySourceEffect(Mutator->MutatorData.ShowPerkSelectGameplayEffectClass, AbilitySystemComponent, 1);

		Comp->AllSelectedPerks.Add(Comp->PerkSelection[(uintptr_t)EnumIdk].Items[0]);
		Comp->OnRep_AllSelectedPerks();
		Log("Selected perk " + to_string((uintptr_t)EnumIdk));
		break;
	default:
		OnMutatorItemSelectedOG(Mutator, PC, EnumIdk);
		Log("default called");
		break;
	}
}