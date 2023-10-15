#pragma once
#include "ue.h"

float GetMaxStackSize(UFortItemDefinition* Def)
{
	if (!Def->MaxStackSize.Curve.CurveTable)
		return Def->MaxStackSize.Value;
	EEvaluateCurveTableResult Result;
	float Ret;
	((UDataTableFunctionLibrary*)UDataTableFunctionLibrary::StaticClass()->DefaultObject)->EvaluateCurveTableRow(Def->MaxStackSize.Curve.CurveTable, Def->MaxStackSize.Curve.RowName, 0, &Result, &Ret, FString());
	return Ret;
}

void ModifyEntry(AFortPlayerControllerAthena* PC, FFortItemEntry& Entry)
{
	for (size_t i = 0; i < PC->WorldInventory->Inventory.ItemInstances.Num(); i++)
	{
		if (PC->WorldInventory->Inventory.ItemInstances[i]->ItemEntry.ItemGuid == Entry.ItemGuid)
		{
			PC->WorldInventory->Inventory.ItemInstances[i]->ItemEntry = Entry;
			break;
		}
	}
}

void UpdateInventory(AFortPlayerControllerAthena* PC, FFortItemEntry* Entry)
{	
	PC->WorldInventory->bRequiresLocalUpdate = true;
	PC->WorldInventory->HandleInventoryLocalUpdate();

	if (Entry)
	{
		PC->WorldInventory->Inventory.MarkItemDirty(*Entry);
	}
	else
	{
		PC->WorldInventory->Inventory.MarkArrayDirty();
	}
}

EFortQuickBars GetQuickBars(UFortItemDefinition* ItemDefinition)
{
	if (!ItemDefinition->IsA(UFortWeaponMeleeItemDefinition::StaticClass()) && !ItemDefinition->IsA(UFortEditToolItemDefinition::StaticClass()) &&
		!ItemDefinition->IsA(UFortBuildingItemDefinition::StaticClass()) && !ItemDefinition->IsA(UFortAmmoItemDefinition::StaticClass()) && !ItemDefinition->IsA(UFortResourceItemDefinition::StaticClass()) && !ItemDefinition->IsA(UFortTrapItemDefinition::StaticClass()))
		return EFortQuickBars::Primary;

	return EFortQuickBars::Secondary;
}

void GiveItem(AFortPlayerControllerAthena* PC, UFortItemDefinition* Def, int Count = 1, int LoadedAmmo = 0, int Level = 0)
{
	UFortWorldItem* Item = (UFortWorldItem*)Def->CreateTemporaryItemInstanceBP(Count, Level);
	Item->SetOwningControllerForTemporaryItem(PC);
	Item->ItemEntry.LoadedAmmo = LoadedAmmo;
	
	PC->WorldInventory->Inventory.ReplicatedEntries.Add(Item->ItemEntry);
	PC->WorldInventory->Inventory.ItemInstances.Add(Item);
	
	UpdateInventory(PC, &Item->ItemEntry);
}

FName PickaxeTagName;

void Remove(AFortPlayerController* PC, FGuid Guid)
{
	for (size_t i = 0; i < PC->WorldInventory->Inventory.ReplicatedEntries.Num(); i++)
	{
		if (PC->WorldInventory->Inventory.ReplicatedEntries[i].ItemGuid == Guid)
		{
			PC->WorldInventory->Inventory.ReplicatedEntries.RemoveSingle(i);
			break;
		}
	}

	for (size_t i = 0; i < PC->WorldInventory->Inventory.ItemInstances.Num(); i++)
	{
		if (!PC->WorldInventory->Inventory.ItemInstances[i])
			continue;
		if (PC->WorldInventory->Inventory.ItemInstances[i]->ItemEntry.ItemGuid == Guid)
		{
			PC->WorldInventory->Inventory.ItemInstances.RemoveSingle(i);
			break;
		}
	}

	UpdateInventory((AFortPlayerControllerAthena*)PC, nullptr);
}

void Remove(AFortPlayerController* PC, UFortItemDefinition* Def, int Count = 1)
{
	for (size_t i = 0; i < PC->WorldInventory->Inventory.ReplicatedEntries.Num(); i++)
	{
		if (PC->WorldInventory->Inventory.ReplicatedEntries[i].ItemDefinition == Def)
		{
			PC->WorldInventory->Inventory.ReplicatedEntries[i].Count -= Count;
			if (PC->WorldInventory->Inventory.ReplicatedEntries[i].Count <= 0)
			{
				Remove(PC, PC->WorldInventory->Inventory.ReplicatedEntries[i].ItemGuid);
				break;
			}
			ModifyEntry((AFortPlayerControllerAthena*)PC, PC->WorldInventory->Inventory.ReplicatedEntries[i]);
			UpdateInventory((AFortPlayerControllerAthena*)PC, &PC->WorldInventory->Inventory.ReplicatedEntries[i]);
			break;
		}
	}
}

void UpdateLoadedAmmo(AFortPlayerController* PC, AFortWeapon* Weapon)
{
	for (size_t i = 0; i < PC->WorldInventory->Inventory.ReplicatedEntries.Num(); i++)
	{
		if (PC->WorldInventory->Inventory.ReplicatedEntries[i].ItemGuid == Weapon->ItemEntryGuid)
		{
			PC->WorldInventory->Inventory.ReplicatedEntries[i].LoadedAmmo = Weapon->AmmoCount;
			ModifyEntry((AFortPlayerControllerAthena*)PC, PC->WorldInventory->Inventory.ReplicatedEntries[i]);
			UpdateInventory((AFortPlayerControllerAthena*)PC, &PC->WorldInventory->Inventory.ReplicatedEntries[i]);
			break;
		}
	}
}

void UpdateLoadedAmmo(AFortPlayerController* PC, AFortWeapon* Weapon, int AmountToAdd)
{
	for (size_t i = 0; i < PC->WorldInventory->Inventory.ReplicatedEntries.Num(); i++)
	{
		if (PC->WorldInventory->Inventory.ReplicatedEntries[i].ItemGuid == Weapon->ItemEntryGuid)
		{
			PC->WorldInventory->Inventory.ReplicatedEntries[i].LoadedAmmo += AmountToAdd;
			ModifyEntry((AFortPlayerControllerAthena*)PC, PC->WorldInventory->Inventory.ReplicatedEntries[i]);
			UpdateInventory((AFortPlayerControllerAthena*)PC, &PC->WorldInventory->Inventory.ReplicatedEntries[i]);
			break;
		}
	}
}

FFortItemEntry* FindEntry(AFortPlayerController* PC, UFortItemDefinition* Def)
{
	for (size_t i = 0; i < PC->WorldInventory->Inventory.ReplicatedEntries.Num(); i++)
	{
		if (PC->WorldInventory->Inventory.ReplicatedEntries[i].ItemDefinition == Def)
			return &PC->WorldInventory->Inventory.ReplicatedEntries[i];
	}
	return nullptr;
}

FFortItemEntry* FindEntry(AFortPlayerController* PC, FGuid& Guid)
{
	for (size_t i = 0; i < PC->WorldInventory->Inventory.ReplicatedEntries.Num(); i++)
	{
		if (PC->WorldInventory->Inventory.ReplicatedEntries[i].ItemGuid == Guid)
			return &PC->WorldInventory->Inventory.ReplicatedEntries[i];
	}
	return nullptr;
}

template<typename T>
FFortItemEntry* FindEntry(AFortPlayerController* PC)
{
	for (size_t i = 0; i < PC->WorldInventory->Inventory.ReplicatedEntries.Num(); i++)
	{
		if (PC->WorldInventory->Inventory.ReplicatedEntries[i].ItemDefinition->IsA(T::StaticClass()))
			return &PC->WorldInventory->Inventory.ReplicatedEntries[i];
	}
	return nullptr;
}