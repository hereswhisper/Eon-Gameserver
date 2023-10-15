#pragma once
#include "ue.h"

void GiveAbility(AFortPlayerControllerAthena* PC, UClass* Ability)
{
    if (!PC || !PC->PlayerState || !PC->MyFortPawn) { return; }
    auto AbilitySystemComponent = PC->MyFortPawn->AbilitySystemComponent;

    if (!AbilitySystemComponent)
        return;

    if (!Ability)
        return;

    FGameplayAbilitySpec Spec{};
    FGameplayAbilitySpecCtor(&Spec, (UGameplayAbility*)Ability->DefaultObject, 1, -1, nullptr);
    GiveAbilityOG(AbilitySystemComponent, &Spec.Handle, Spec);
}

FGameplayAbilitySpec* FindAbilitySpecFromHandle(UAbilitySystemComponent* Component, FGameplayAbilitySpecHandle& Handle)
{
    for (size_t i = 0; i < Component->ActivatableAbilities.Items.Num(); i++)
    {
        if (Component->ActivatableAbilities.Items[i].Handle.Handle == Handle.Handle) { return &Component->ActivatableAbilities.Items[i]; }
    }

    return nullptr;
}

void InternalServerTryActivateAbilityHook(UFortAbilitySystemComponentAthena* AbilitySystemComponent, FGameplayAbilitySpecHandle Handle, bool InputPressed, FPredictionKey& PredictionKey, FGameplayEventData* TriggerEventData)
{
    FGameplayAbilitySpec* Spec = FindAbilitySpecFromHandle(AbilitySystemComponent, Handle);
    if (!Spec)
        return AbilitySystemComponent->ClientActivateAbilityFailed(Handle, PredictionKey.Current);

    // Consume any pending target info, to clear out cancels from old executions
    //ConsumeAllReplicatedData(Handle, PredictionKey);

    UGameplayAbility* AbilityToActivate = Spec->Ability;

    UGameplayAbility* InstancedAbility = nullptr;
    Spec->InputPressed = true;

    // Attempt to activate the ability (server side) and tell the client if it succeeded or failed.
    if (!InternalTryActivateAbility(AbilitySystemComponent, Handle, PredictionKey, &InstancedAbility, nullptr, TriggerEventData))
    {
        AbilitySystemComponent->ClientActivateAbilityFailed(Handle, PredictionKey.Current);
        Spec->InputPressed = false;
    }
    AbilitySystemComponent->ActivatableAbilities.MarkItemDirty(*Spec);
    //MarkAbilitySpecDirty(*Spec);
}

void GiveAbilitySet(AFortPlayerControllerAthena* PC, UFortAbilitySet* Set)
{
    if (Set)
    {
        for (size_t i = 0; i < Set->GameplayAbilities.Num(); i++)
        {
            UClass* Ability = Set->GameplayAbilities[i].Get();
            if (Ability) { GiveAbility(PC, Ability); }
        }
    }
}

void RemoveAbility(AFortPlayerController* PC, UClass* AbilityClass)
{
    for (size_t i = 0; i < PC->MyFortPawn->AbilitySystemComponent->ActivatableAbilities.Items.Num(); i++)
    {
        if (PC->MyFortPawn->AbilitySystemComponent->ActivatableAbilities.Items[i].Ability->Class == AbilityClass)
        {
            auto& Item = PC->MyFortPawn->AbilitySystemComponent->ActivatableAbilities.Items[i];
            PC->MyFortPawn->AbilitySystemComponent->ServerEndAbility(Item.Handle, Item.ActivationInfo, Item.ActivationInfo.PredictionKeyWhenActivated);
            PC->MyFortPawn->AbilitySystemComponent->ServerCancelAbility(Item.Handle, Item.ActivationInfo);
            PC->MyFortPawn->AbilitySystemComponent->ClientCancelAbility(Item.Handle, Item.ActivationInfo);
            break;
        }
    }
}