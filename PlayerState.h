#pragma once
#include "ue.h"
#include "BackendStuff.h"

void (*ServerSetInAircraftOG)(AFortPlayerStateAthena* PlayerState, bool aircragft);
static void ServerSetInAircraft(AFortPlayerStateAthena* PlayerState, bool aircragft)
{
	AFortPlayerControllerAthena* PC = (AFortPlayerControllerAthena*)PlayerState->Owner;

	if (!PC || !PC->WorldInventory)
		return ServerSetInAircraftOG(PlayerState, aircragft);
	PlayersOnBus = GetGameState()->PlayersLeft;
	PC->WorldInventory->Inventory.ReplicatedEntries.FreeArray();
	PC->WorldInventory->Inventory.ItemInstances.FreeArray();
	BattleBus = GetGameState()->GetAircraft(0);
	for (size_t i = 0; i < GetGameMode()->StartingItems.Num(); i++)
	{
		if (GetGameMode()->StartingItems[i].Count <= 0)
			continue;
		GiveItem(PC, GetGameMode()->StartingItems[i].Item, GetGameMode()->StartingItems[i].Count);
	}

	if (!PC->CosmeticLoadoutPC.Pickaxe || !PC->CosmeticLoadoutPC.Pickaxe->WeaponDefinition)
	{
		PC->ClientReturnToMainMenu(TEXT("Invalid pickaxe, please report tyhis to a dev"));
		return;
	}
	GiveItem(PC, PC->CosmeticLoadoutPC.Pickaxe->WeaponDefinition);

	if (bTeamRumble)
	{
		static UFortWeaponItemDefinition* ArDef = StaticLoadObject<UFortWeaponItemDefinition>("/Game/Athena/Items/Weapons/WID_Assault_Auto_Athena_UC_Ore_T03.WID_Assault_Auto_Athena_UC_Ore_T03");
		static UFortWeaponItemDefinition* PumpDef = StaticLoadObject<UFortWeaponItemDefinition>("/Game/Athena/Items/Weapons/WID_Shotgun_Standard_Athena_C_Ore_T03.WID_Shotgun_Standard_Athena_C_Ore_T03");

		GiveItem(PC, PumpDef, 1, 5);
		GiveItem(PC, ArDef, 1, 30);
		GiveItem(PC, PumpDef->GetAmmoWorldItemDefinition_BP(), 15);
		GiveItem(PC, ArDef->GetAmmoWorldItemDefinition_BP(), 60);
	}

	if (bArsenal)
	{
		static UFortItemDefinition* WoodDef = StaticLoadObject<UFortItemDefinition>("/Game/Items/ResourcePickups/WoodItemData.WoodItemData");
		static UFortItemDefinition* StoneDef = StaticLoadObject<UFortItemDefinition>("/Game/Items/ResourcePickups/StoneItemData.StoneItemData");
		static UFortItemDefinition* MetalDef = StaticLoadObject<UFortItemDefinition>("/Game/Items/ResourcePickups/MetalItemData.MetalItemData");

		GiveItem(PC, WoodDef, 9999);
		GiveItem(PC, StoneDef, 9999);
		GiveItem(PC, MetalDef, 9999);

		AFortAthenaMutator_GG* Mutator = (AFortAthenaMutator_GG*)GetGameState()->GetMutatorByClass(GetGameMode(), AFortAthenaMutator_GG::StaticClass());

		if (Mutator && Mutator->WeaponEntries[0].Weapon)
		{
			GiveItem(PC, Mutator->WeaponEntries[0].Weapon, 1, GetAmmoForDef(Mutator->WeaponEntries[0].Weapon));
		}
	}

	if (bStormKing)
	{
		static UFortItemDefinition* WoodDef = StaticLoadObject<UFortItemDefinition>("/Game/Items/ResourcePickups/WoodItemData.WoodItemData");
		static UFortItemDefinition* StoneDef = StaticLoadObject<UFortItemDefinition>("/Game/Items/ResourcePickups/StoneItemData.StoneItemData");
		static UFortItemDefinition* MetalDef = StaticLoadObject<UFortItemDefinition>("/Game/Items/ResourcePickups/MetalItemData.MetalItemData");
		static UFortWeaponItemDefinition* ScarDef = StaticLoadObject<UFortWeaponItemDefinition>("/Game/Athena/Items/Weapons/Boss/WID_Boss_Adventure_AR.WID_Boss_Adventure_AR");
		static UFortWeaponItemDefinition* DrumgunDef = StaticLoadObject<UFortWeaponItemDefinition>("/Game/Athena/Items/Weapons/Boss/WID_Boss_Midas.WID_Boss_Midas");
		static UFortWeaponItemDefinition* SlurpFishDef = UObject::FindObject<UFortWeaponItemDefinition>("WID_Athena_Flopper_Effective.WID_Athena_Flopper_Effective");
		static UFortWeaponItemDefinition* SmallShieldDef = UObject::FindObject<UFortWeaponItemDefinition>("Athena_ShieldSmall.Athena_ShieldSmall");

		static UFortWeaponItemDefinition* GraveDiggerDef = UObject::FindObject<UFortWeaponItemDefinition>("WID_Assault_Auto_Halloween_SR_Ore_T01.WID_Assault_Auto_Halloween_SR_Ore_T01");
		static UFortWeaponItemDefinition* NocturnoDef = UObject::FindObject<UFortWeaponItemDefinition>("WID_Assault_Auto_Founders_SR_Ore_T05.WID_Assault_Auto_Founders_SR_Ore_T05");
		static UFortWeaponItemDefinition* MeleeDef1 = UObject::FindObject<UFortWeaponItemDefinition>("WID_Edged_Sword_NeonGlow_SR_Ore_T05.WID_Edged_Sword_NeonGlow_SR_Ore_T05");
		static UFortWeaponItemDefinition* MeleeDef2 = UObject::FindObject<UFortWeaponItemDefinition>("WID_Edged_Sword_Medium_Laser_Founders_SR_Crystal_T05.WID_Edged_Sword_Medium_Laser_Founders_SR_Crystal_T05");

		GiveItem(PC, WoodDef, 9999);
		GiveItem(PC, StoneDef, 9999);
		GiveItem(PC, MetalDef, 9999);
		//GiveItem(PC, ScarDef, 1, 30);
		//GiveItem(PC, GraveDiggerDef, 1, 0, 144);
		//GiveItem(PC, NocturnoDef, 1, 0, 86);
		GiveItem(PC, SlurpFishDef, 3);
		GiveItem(PC, SmallShieldDef, 6);
		//GiveItem(PC, MeleeDef1, 1);
		//GiveItem(PC, MeleeDef2, 1);
		GiveItem(PC, ScarDef, 1, 30);
		GiveItem(PC, DrumgunDef, 1, 40);
		//GiveItem(PC, NocturnoDef->GetAmmoWorldItemDefinition_BP(), 9999);
		//GiveItem(PC, GraveDiggerDef->GetAmmoWorldItemDefinition_BP(), 9999);
		GiveItem(PC, ScarDef->GetAmmoWorldItemDefinition_BP(), 9999);

		auto Aircraft = GetGameState()->GetAircraft(0);
		auto PoiManager = GetGameState()->PoiManager;

		if (PoiManager)
		{
			Aircraft->FlightInfo.FlightSpeed = 1500.f;
			FVector Loc = (GetGameState()->PoiManager->AllPoiVolumes[0]->K2_GetActorLocation() - (GetGameState()->PoiManager->AllPoiVolumes[0]->GetActorForwardVector() * 5000));
			Loc.Z = 15000;
			Aircraft->FlightInfo.FlightStartLocation = (FVector_NetQuantize100)Loc;
			BusLocation = GetGameState()->PoiManager->AllPoiVolumes[0]->K2_GetActorLocation();
			BusLocation.Z = 0;
		}
	}

	if (bTravis)
	{
		//X: 62590.7 Y : -75501.8Z : 13982.4
		auto Aircraft = GetGameState()->GetAircraft(0);
		FVector Loc = FVector(62590, -75501, 13982);
		Aircraft->FlightInfo.FlightStartLocation = (FVector_NetQuantize100)Loc;
		Aircraft->FlightInfo.FlightSpeed = 1000;
		Aircraft->FlightInfo.TimeTillDropStart = 1;
		Aircraft->DropStartTime = GetStatics()->GetTimeSeconds(GetWorld()) + 1;
		GetGameState()->bAircraftIsLocked = false;
		BusLocation = Loc;
	}

	if (bLateGame)
	{
		static bool Done = false;
		auto Aircraft = GetGameState()->GetAircraft(0);
		auto PoiManager = GetGameState()->PoiManager;

		if (!Done)
		{
			Done = true;
			BattleBus = Aircraft;
			GetGameState()->GamePhase = EAthenaGamePhase::SafeZones;
			GetGameState()->GamePhaseStep = EAthenaGamePhaseStep::StormHolding;
			GetGameState()->OnRep_GamePhase(EAthenaGamePhase::Aircraft);
			GetGameState()->SafeZonesStartTime = GetStatics()->GetTimeSeconds(GetWorld());

			if (PoiManager)
			{
				Aircraft->FlightInfo.FlightSpeed = 1000.f;

				TArray<AActor*> Test;
				GetStatics()->GetAllActorsOfClass(GetWorld(), ABuildingContainer::StaticClass(), &Test);
				// FVector Proper = GetGameMode()->SafeZoneLocations[4]; // TEST
				int Idx = GetMath()->RandomIntegerInRange(0, Test.Num()); // BAD 
				FVector Loc = (Test[Idx]->K2_GetActorLocation() - (Test[Idx]->GetActorForwardVector() * 1000)); // bro what the fuck is wrong with u
				Loc.Z = 15000;
				Aircraft->FlightInfo.FlightStartLocation = (FVector_NetQuantize100)Loc;

				Aircraft->FlightInfo.TimeTillFlightEnd = 9;
				Aircraft->FlightInfo.TimeTillDropEnd = 9;
				Aircraft->FlightInfo.TimeTillDropStart = 1;
				Aircraft->DropStartTime = GetStatics()->GetTimeSeconds(GetWorld()) + 1;
				Aircraft->DropEndTime = GetStatics()->GetTimeSeconds(GetWorld()) + 9;
				GetGameState()->bAircraftIsLocked = false;
				BusLocation = Loc;
				BusLocation.Z = 0;
				Test.FreeArray();
			}
		}

		static UFortItemDefinition* WoodDef = StaticLoadObject<UFortItemDefinition>("/Game/Items/ResourcePickups/WoodItemData.WoodItemData");
		static UFortItemDefinition* StoneDef = StaticLoadObject<UFortItemDefinition>("/Game/Items/ResourcePickups/StoneItemData.StoneItemData");
		static UFortItemDefinition* MetalDef = StaticLoadObject<UFortItemDefinition>("/Game/Items/ResourcePickups/MetalItemData.MetalItemData");

		GiveItem(PC, WoodDef, 500);
		GiveItem(PC, StoneDef, 400);
		GiveItem(PC, MetalDef, 350);

		auto& LoadoutToGive = GetRandomLoadout();

		for (auto& Pair : LoadoutToGive.ItemDefinitions)
		{
			GiveItem(PC, Pair.first, Pair.second[0], Pair.second[1]);
			UFortItemDefinition* Def = ((UFortWeaponRangedItemDefinition*)Pair.first)->GetAmmoWorldItemDefinition_BP();
			if(Def && Def != Pair.first)
				GiveItem(PC, Def, Pair.second[2]);
		}
	}

	return ServerSetInAircraftOG(PlayerState, aircragft);
}
