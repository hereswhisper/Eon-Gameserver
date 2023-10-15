#pragma once
#include "ue.h"
#include "Inventory.h"

bool IsPrimaryQuickbar(UFortItemDefinition* Def)
{
	return Def->IsA(UFortConsumableItemDefinition::StaticClass()) || Def->IsA(UFortWeaponRangedItemDefinition::StaticClass()) || Def->IsA(UFortGadgetItemDefinition::StaticClass());
}

void (*ServerHandlePickupOG)(AFortPlayerPawn* Pawn, AFortPickup* Pickup, float InFlyTime, FVector InStartDirection, bool bPlayPickupSound);
void ServerHandlePickup(AFortPlayerPawn* Pawn, AFortPickup* Pickup, float InFlyTime, FVector InStartDirection, bool bPlayPickupSound)
{
	if (Pickup->bPickedUp)
		return ServerHandlePickupOG(Pawn, Pickup, InFlyTime, InStartDirection, bPlayPickupSound);

	AFortPlayerControllerAthena* PC = (AFortPlayerControllerAthena*)Pawn->Controller;
	if (PC->IsA(ABP_PhoebePlayerController_C::StaticClass())) // AAIController
		return;
	UFortItemDefinition* Def = Pickup->PrimaryPickupItemEntry.ItemDefinition;
	int PickupCount = Pickup->PrimaryPickupItemEntry.Count;
	int PickupLoadedAmmo = Pickup->PrimaryPickupItemEntry.LoadedAmmo;
	FFortItemEntry* FoundEntry = nullptr;
	int Items = 0;
	float MaxStackSize = GetMaxStackSize(Def);
	bool Stackable = Def->IsStackable();

	if (!Pawn->CurrentWeapon)
		return;

	for (size_t i = 0; i < PC->WorldInventory->Inventory.ReplicatedEntries.Num(); i++)
	{
		FFortItemEntry& Entry = PC->WorldInventory->Inventory.ReplicatedEntries[i];

		if (IsPrimaryQuickbar(Def) && IsPrimaryQuickbar(Entry.ItemDefinition))
		{
			Items++;
			if (Items > 5)
			{
				Remove(PC, Entry.ItemGuid);
			}
		}

		if (Entry.ItemDefinition == Def && (Entry.Count < MaxStackSize))
		{
			FoundEntry = &PC->WorldInventory->Inventory.ReplicatedEntries[i];
		}
	}

	cout << Items << endl;

	if (Items == 5 && IsPrimaryQuickbar(Pawn->CurrentWeapon->WeaponData))
	{
		FFortItemEntry* SwapEntry = FindEntry(PC, Pawn->CurrentWeapon->WeaponData);
		if (!SwapEntry || !SwapEntry->ItemDefinition)
			return;
		float mMaxStackSize = GetMaxStackSize(SwapEntry->ItemDefinition);

		if (!FoundEntry && !Pawn->CurrentWeapon->WeaponData->IsStackable() || SwapEntry->ItemDefinition != Def || SwapEntry->Count >= mMaxStackSize)
		{
			if (SwapEntry->ItemDefinition->IsA(UFortWeaponItemDefinition::StaticClass()))
			{
				SpawnPickup(Pawn->K2_GetActorLocation(), SwapEntry, EFortPickupSourceTypeFlag::Tossed, EFortPickupSpawnSource::Unset, Pawn);
				Remove(PC, Pawn->CurrentWeapon->GetInventoryGUID());
			}
		}
	}

	if (FoundEntry)
	{
		if (Stackable)
		{
			FoundEntry->Count += PickupCount;
			if (FoundEntry->Count > MaxStackSize)
			{
				int Count = FoundEntry->Count;
				FoundEntry->Count = MaxStackSize;

				if (Def->bAllowMultipleStacks)
				{
					if (Items == 5)
					{
						SpawnPickup(Pawn->K2_GetActorLocation(), FoundEntry, EFortPickupSourceTypeFlag::Tossed, EFortPickupSpawnSource::Unset, Pawn, Count - MaxStackSize);
					}
					else
					{
						GiveItem(PC, Def, Count - MaxStackSize);
					}
				}
				else
				{
					SpawnPickup(Pawn->K2_GetActorLocation(), FoundEntry, EFortPickupSourceTypeFlag::Tossed, EFortPickupSpawnSource::Unset, Pawn, Count - MaxStackSize);
				}
			}

			ModifyEntry(PC, *FoundEntry);
			UpdateInventory(PC, FoundEntry);
		}
		else if (Def->bAllowMultipleStacks)
		{
			GiveItem(PC, Def, PickupCount, PickupLoadedAmmo);
		}
	}
	else
	{
		GiveItem(PC, Def, PickupCount, PickupLoadedAmmo);
	}

	Pickup->PickupLocationData.bPlayPickupSound = bPlayPickupSound;
	Pickup->PickupLocationData.FlyTime = 0.4f;
	Pickup->PickupLocationData.ItemOwner = Pawn;
	Pickup->PickupLocationData.PickupGuid = Pickup->PrimaryPickupItemEntry.ItemGuid;
	Pickup->PickupLocationData.PickupTarget = Pawn;
	Pickup->OnRep_PickupLocationData();

	Pickup->bPickedUp = true;
	Pickup->OnRep_bPickedUp();

	return ServerHandlePickupOG(Pawn, Pickup, InFlyTime, InStartDirection, bPlayPickupSound);
}

void (*NetMulticast_Athena_BatchedDamageCuesOG)(AFortPawn* Pawn, FAthenaBatchedDamageGameplayCues_Shared SharedData, FAthenaBatchedDamageGameplayCues_NonShared NonSharedData);
void NetMulticast_Athena_BatchedDamageCues(AFortPawn* Pawn, FAthenaBatchedDamageGameplayCues_Shared SharedData, FAthenaBatchedDamageGameplayCues_NonShared NonSharedData)
{
	if (!Pawn || Pawn->Controller->IsA(ABP_PhoebePlayerController_C::StaticClass()))
		return;

	if (Pawn->CurrentWeapon)
		UpdateLoadedAmmo((AFortPlayerController*)Pawn->Controller, ((AFortPlayerPawn*)Pawn)->CurrentWeapon);

	return NetMulticast_Athena_BatchedDamageCuesOG(Pawn, SharedData, NonSharedData);
}

void ServerReviveFromDBNO(AFortPlayerPawnAthena* Pawn, AFortPlayerControllerAthena* Instigator)
{
	ReviveCounts[Instigator]++;
	Pawn->bIsDBNO = false;
	Pawn->OnRep_IsDBNO();
	auto PC = ((AFortPlayerControllerAthena*)Pawn->Controller);
	PC->RespawnPlayerAfterDeath(false);
	Pawn->SetHealth(30);
	RemoveAbility(PC, UGAB_AthenaDBNO_C::StaticClass());
}

void ServerSendZiplineState(AFortPlayerPawnAthena* Pawn, FZiplinePawnState State)
{
	if (!Pawn)
		return;

	Pawn->ZiplineState = State;
	OnRep_ZiplineState(Pawn);

	if (State.bJumped)
	{
		Pawn->LaunchCharacter(FVector{ 0,0,1200 }, false, false);
	}
}

void (*OnCapsuleBeginOverlapOG)(AFortPlayerPawn* Pawn, UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, FHitResult SweepResult);
void OnCapsuleBeginOverlap(AFortPlayerPawn* Pawn, UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, FHitResult SweepResult)
{
	if (OtherActor->IsA(AFortPickup::StaticClass()))
	{
		AFortPickup* Pickup = (AFortPickup*)OtherActor;

		if (Pickup->PawnWhoDroppedPickup == Pawn)
			return OnCapsuleBeginOverlapOG(Pawn, OverlappedComp, OtherActor, OtherComp, OtherBodyIndex, bFromSweep, SweepResult);

		UFortItemDefinition* Def = (UFortItemDefinition*)Pickup->PrimaryPickupItemEntry.ItemDefinition;

		if (Def->IsA(UFortAmmoItemDefinition::StaticClass()) || Def->IsA(UFortResourceItemDefinition::StaticClass()))
		{
			Pawn->ServerHandlePickup(Pickup, 0.40f, FVector(), true);
		}
	}

	return OnCapsuleBeginOverlapOG(Pawn, OverlappedComp, OtherActor, OtherComp, OtherBodyIndex, bFromSweep, SweepResult);
}

void ServerHandlePickupWithSwap(AFortPlayerPawnAthena* Pawn, AFortPickup* Pickup, FGuid Swap, float InFlyTime, FVector InStartDirection, bool bPlayPickupSound)
{
	if (!Pawn || !Pickup || Pickup->bPickedUp)
		return;

	AFortPlayerControllerAthena* PC = (AFortPlayerControllerAthena*)Pawn->Controller;

	FFortItemEntry* Entry = FindEntry(PC, Swap);
	if (!Entry || !Entry->ItemDefinition || Entry->ItemDefinition->IsA(UFortWeaponMeleeItemDefinition::StaticClass()))
		return;

	SpawnPickup(Pawn->K2_GetActorLocation(), Entry, EFortPickupSourceTypeFlag::Player, EFortPickupSpawnSource::Unset, Pawn);
	Remove(PC, Swap);
	Pawn->ServerHandlePickup(Pickup, InFlyTime, InStartDirection, bPlayPickupSound);
}