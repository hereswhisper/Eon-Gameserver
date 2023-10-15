#pragma once
#include "ue.h"
#include <vector>
#include <map>
#include <algorithm>
#include <random>

struct ItemRow
{
	UFortItemDefinition* Def;
	int DropCount;
	float Weight;
	int LoadedAmmo = 0;
};

map<string, vector<ItemRow>> LootForTiers{};

int GetAmmoForDef(UFortWeaponItemDefinition* Def)
{
	if (!Def)
	{
		Log("Invalid Def in GetAmmoForDef");
		return 0;
	}

	UDataTable* Table = ((UFortWeaponItemDefinition*)Def)->WeaponStatHandle.DataTable;
	FName RowName = ((UFortWeaponItemDefinition*)Def)->WeaponStatHandle.RowName;

	MTMap<FName, void*>& RowMap = Table->RowMap;

	for (size_t i = 0; i < RowMap.Pairs.Elements.Data.Num(); i++)
	{
		auto& Pair = RowMap.Pairs.Elements.Data[i].ElementData.Value;
		FName rowName = Pair.First;
		if (!RowName.ComparisonIndex || !Pair.Second)
			continue;

		if (RowName.ComparisonIndex == rowName.ComparisonIndex)
		{
			return ((FFortRangedWeaponStats*)Pair.Second)->ClipSize;
		}
	}

	Log("Invalid Def in GetAmmoForDef 2");
	return 0;
}

void InitLooting()
{
	//AR
	ItemRow Row1{};
	Row1.Def = StaticLoadObject<UFortItemDefinition>("/Game/Athena/Items/Weapons/WID_Assault_Auto_Athena_C_Ore_T02.WID_Assault_Auto_Athena_C_Ore_T02");
	Row1.DropCount = 1;
	Row1.LoadedAmmo = GetAmmoForDef((UFortWeaponItemDefinition*)Row1.Def);
	Row1.Weight = 0.15f;
	LootForTiers["Loot_AthenaFloorLoot_Warmup"].push_back(Row1);
	LootForTiers["Loot_AthenaFloorLoot"].push_back(Row1);
	LootForTiers["Loot_AthenaTreasure"].push_back(Row1);

	Row1.Def = StaticLoadObject<UFortItemDefinition>("/Game/Athena/Items/Weapons/WID_Assault_Auto_Athena_UC_Ore_T03.WID_Assault_Auto_Athena_UC_Ore_T03");
	Row1.DropCount = 1;
	Row1.LoadedAmmo = GetAmmoForDef((UFortWeaponItemDefinition*)Row1.Def);
	Row1.Weight = 0.30f;
	LootForTiers["Loot_AthenaFloorLoot_Warmup"].push_back(Row1);
	LootForTiers["Loot_AthenaFloorLoot"].push_back(Row1);
	Row1.Weight = 0.15f;
	LootForTiers["Loot_AthenaTreasure"].push_back(Row1);

	Row1.Def = StaticLoadObject<UFortItemDefinition>("/Game/Athena/Items/Weapons/WID_Assault_Auto_Athena_R_Ore_T03.WID_Assault_Auto_Athena_R_Ore_T03");
	Row1.DropCount = 1;
	Row1.LoadedAmmo = GetAmmoForDef((UFortWeaponItemDefinition*)Row1.Def);
	Row1.Weight = 0.10f;
	LootForTiers["Loot_AthenaFloorLoot_Warmup"].push_back(Row1);
	LootForTiers["Loot_AthenaFloorLoot"].push_back(Row1);
	Row1.Weight = 0.35f;
	LootForTiers["Loot_AthenaTreasure"].push_back(Row1);

	Row1.Def = StaticLoadObject<UFortItemDefinition>("/Game/Athena/Items/Weapons/WID_Assault_AutoHigh_Athena_VR_Ore_T03.WID_Assault_AutoHigh_Athena_VR_Ore_T03");
	Row1.DropCount = 1;
	Row1.LoadedAmmo = GetAmmoForDef((UFortWeaponItemDefinition*)Row1.Def);
	Row1.Weight = 0.05f;
	LootForTiers["Loot_AthenaTreasure"].push_back(Row1);

	Row1.Def = StaticLoadObject<UFortItemDefinition>("/Game/Athena/Items/Weapons/WID_Assault_AutoHigh_Athena_SR_Ore_T03.WID_Assault_AutoHigh_Athena_SR_Ore_T03");
	Row1.DropCount = 1;
	Row1.LoadedAmmo = GetAmmoForDef((UFortWeaponItemDefinition*)Row1.Def);
	Row1.Weight = 0.01f;
	LootForTiers["Loot_AthenaTreasure"].push_back(Row1);
	//End AR

	//Burst Rifle
	Row1.Def = StaticLoadObject<UFortItemDefinition>("/Game/Athena/Items/Weapons/WID_Assault_SemiAuto_Athena_C_Ore_T02.WID_Assault_SemiAuto_Athena_C_Ore_T02");
	Row1.DropCount = 1;
	Row1.LoadedAmmo = GetAmmoForDef((UFortWeaponItemDefinition*)Row1.Def);
	Row1.Weight = 0.15f;
	LootForTiers["Loot_AthenaFloorLoot_Warmup"].push_back(Row1);
	LootForTiers["Loot_AthenaFloorLoot"].push_back(Row1);
	LootForTiers["Loot_AthenaTreasure"].push_back(Row1);

	Row1.Def = StaticLoadObject<UFortItemDefinition>("/Game/Athena/Items/Weapons/WID_Assault_SemiAuto_Athena_UC_Ore_T03.WID_Assault_SemiAuto_Athena_UC_Ore_T03");
	Row1.DropCount = 1;
	Row1.LoadedAmmo = GetAmmoForDef((UFortWeaponItemDefinition*)Row1.Def);
	Row1.Weight = 0.30f;
	LootForTiers["Loot_AthenaFloorLoot_Warmup"].push_back(Row1);
	LootForTiers["Loot_AthenaFloorLoot"].push_back(Row1);
	Row1.Weight = 0.15f;
	LootForTiers["Loot_AthenaTreasure"].push_back(Row1);

	Row1.Def = StaticLoadObject<UFortItemDefinition>("/Game/Athena/Items/Weapons/WID_Assault_SemiAuto_Athena_R_Ore_T03.WID_Assault_SemiAuto_Athena_R_Ore_T03");
	Row1.DropCount = 1;
	Row1.LoadedAmmo = GetAmmoForDef((UFortWeaponItemDefinition*)Row1.Def);
	Row1.Weight = 0.10f;
	LootForTiers["Loot_AthenaFloorLoot_Warmup"].push_back(Row1);
	LootForTiers["Loot_AthenaFloorLoot"].push_back(Row1);
	Row1.Weight = 0.35f;
	LootForTiers["Loot_AthenaTreasure"].push_back(Row1);

	Row1.Def = StaticLoadObject<UFortItemDefinition>("/Game/Athena/Items/Weapons/WID_Assault_SemiAuto_Athena_VR_Ore_T03.WID_Assault_SemiAuto_Athena_VR_Ore_T03");
	Row1.DropCount = 1;
	Row1.LoadedAmmo = GetAmmoForDef((UFortWeaponItemDefinition*)Row1.Def);
	Row1.Weight = 0.05f;
	LootForTiers["Loot_AthenaTreasure"].push_back(Row1);

	Row1.Def = StaticLoadObject<UFortItemDefinition>("/Game/Athena/Items/Weapons/WID_Assault_SemiAuto_Athena_SR_Ore_T03.WID_Assault_SemiAuto_Athena_SR_Ore_T03");
	Row1.DropCount = 1;
	Row1.LoadedAmmo = GetAmmoForDef((UFortWeaponItemDefinition*)Row1.Def);
	Row1.Weight = 0.01f;
	LootForTiers["Loot_AthenaTreasure"].push_back(Row1);
	//End Burst Rifle

	//AK
	Row1.Def = StaticLoadObject<UFortItemDefinition>("/Game/Athena/Items/Weapons/WID_Assault_Heavy_Athena_C_Ore_T03.WID_Assault_Heavy_Athena_C_Ore_T03");
	Row1.DropCount = 1;
	Row1.LoadedAmmo = GetAmmoForDef((UFortWeaponItemDefinition*)Row1.Def);
	Row1.Weight = 0.12f;
	LootForTiers["Loot_AthenaFloorLoot_Warmup"].push_back(Row1);
	LootForTiers["Loot_AthenaFloorLoot"].push_back(Row1);
	LootForTiers["Loot_AthenaTreasure"].push_back(Row1);

	Row1.Def = StaticLoadObject<UFortItemDefinition>("/Game/Athena/Items/Weapons/WID_Assault_Heavy_Athena_UC_Ore_T03.WID_Assault_Heavy_Athena_UC_Ore_T03");
	Row1.DropCount = 1;
	Row1.LoadedAmmo = GetAmmoForDef((UFortWeaponItemDefinition*)Row1.Def);
	Row1.Weight = 0.28f;
	LootForTiers["Loot_AthenaFloorLoot_Warmup"].push_back(Row1);
	LootForTiers["Loot_AthenaFloorLoot"].push_back(Row1);
	Row1.Weight = 0.13f;
	LootForTiers["Loot_AthenaTreasure"].push_back(Row1);

	Row1.Def = StaticLoadObject<UFortItemDefinition>("/Game/Athena/Items/Weapons/WID_Assault_Heavy_Athena_R_Ore_T03.WID_Assault_Heavy_Athena_R_Ore_T03");
	Row1.DropCount = 1;
	Row1.LoadedAmmo = GetAmmoForDef((UFortWeaponItemDefinition*)Row1.Def);
	Row1.Weight = 0.07f;
	LootForTiers["Loot_AthenaFloorLoot_Warmup"].push_back(Row1);
	LootForTiers["Loot_AthenaFloorLoot"].push_back(Row1);
	Row1.Weight = 0.34f;
	LootForTiers["Loot_AthenaTreasure"].push_back(Row1);

	Row1.Def = StaticLoadObject<UFortItemDefinition>("/Game/Athena/Items/Weapons/WID_Assault_Heavy_Athena_VR_Ore_T03.WID_Assault_Heavy_Athena_VR_Ore_T03");
	Row1.DropCount = 1;
	Row1.LoadedAmmo = GetAmmoForDef((UFortWeaponItemDefinition*)Row1.Def);
	Row1.Weight = 0.05f;
	LootForTiers["Loot_AthenaTreasure"].push_back(Row1);

	Row1.Def = StaticLoadObject<UFortItemDefinition>("/Game/Athena/Items/Weapons/WID_Assault_Heavy_Athena_SR_Ore_T03.WID_Assault_Heavy_Athena_SR_Ore_T03");
	Row1.DropCount = 1;
	Row1.LoadedAmmo = GetAmmoForDef((UFortWeaponItemDefinition*)Row1.Def);
	Row1.Weight = 0.01f;
	LootForTiers["Loot_AthenaTreasure"].push_back(Row1);
	//End AK

	//Pump
	Row1.Def = StaticLoadObject<UFortItemDefinition>("/Game/Athena/Items/Weapons/WID_Shotgun_Standard_Athena_C_Ore_T03.WID_Shotgun_Standard_Athena_C_Ore_T03");
	Row1.DropCount = 1;
	Row1.LoadedAmmo = GetAmmoForDef((UFortWeaponItemDefinition*)Row1.Def);
	Row1.Weight = 0.12f;
	LootForTiers["Loot_AthenaFloorLoot_Warmup"].push_back(Row1);
	LootForTiers["Loot_AthenaFloorLoot"].push_back(Row1);
	LootForTiers["Loot_AthenaTreasure"].push_back(Row1);

	Row1.Def = StaticLoadObject<UFortItemDefinition>("/Game/Athena/Items/Weapons/WID_Shotgun_Standard_Athena_C_Ore_T03.WID_Shotgun_Standard_Athena_C_Ore_T03");
	Row1.DropCount = 1;
	Row1.LoadedAmmo = GetAmmoForDef((UFortWeaponItemDefinition*)Row1.Def);
	Row1.Weight = 0.28f;
	LootForTiers["Loot_AthenaFloorLoot_Warmup"].push_back(Row1);
	LootForTiers["Loot_AthenaFloorLoot"].push_back(Row1);
	Row1.Weight = 0.13f;
	LootForTiers["Loot_AthenaTreasure"].push_back(Row1);

	Row1.Def = StaticLoadObject<UFortItemDefinition>("/Game/Athena/Items/Weapons/WID_Shotgun_Standard_Athena_UC_Ore_T03.WID_Shotgun_Standard_Athena_UC_Ore_T03");
	Row1.DropCount = 1;
	Row1.LoadedAmmo = GetAmmoForDef((UFortWeaponItemDefinition*)Row1.Def);
	Row1.Weight = 0.07f;
	LootForTiers["Loot_AthenaFloorLoot_Warmup"].push_back(Row1);
	LootForTiers["Loot_AthenaFloorLoot"].push_back(Row1);
	Row1.Weight = 0.28f;
	LootForTiers["Loot_AthenaTreasure"].push_back(Row1);

	Row1.Def = StaticLoadObject<UFortItemDefinition>("/Game/Athena/Items/Weapons/WID_Shotgun_Standard_Athena_VR_Ore_T03.WID_Shotgun_Standard_Athena_VR_Ore_T03");
	Row1.DropCount = 1;
	Row1.LoadedAmmo = GetAmmoForDef((UFortWeaponItemDefinition*)Row1.Def);
	Row1.Weight = 0.05f;
	LootForTiers["Loot_AthenaTreasure"].push_back(Row1);

	Row1.Def = StaticLoadObject<UFortItemDefinition>("/Game/Athena/Items/Weapons/WID_Shotgun_Standard_Athena_SR_Ore_T03.WID_Shotgun_Standard_Athena_SR_Ore_T03");
	Row1.DropCount = 1;
	Row1.LoadedAmmo = GetAmmoForDef((UFortWeaponItemDefinition*)Row1.Def);
	Row1.Weight = 0.01f;
	LootForTiers["Loot_AthenaTreasure"].push_back(Row1);
	//End Pump

	//TAC
	Row1.Def = StaticLoadObject<UFortItemDefinition>("/Game/Athena/Items/Weapons/WID_Shotgun_SemiAuto_Athena_UC_Ore_T03.WID_Shotgun_SemiAuto_Athena_UC_Ore_T03");
	Row1.DropCount = 1;
	Row1.LoadedAmmo = GetAmmoForDef((UFortWeaponItemDefinition*)Row1.Def);
	Row1.Weight = 0.12f;
	LootForTiers["Loot_AthenaFloorLoot_Warmup"].push_back(Row1);
	LootForTiers["Loot_AthenaFloorLoot"].push_back(Row1);
	LootForTiers["Loot_AthenaTreasure"].push_back(Row1);

	Row1.Def = StaticLoadObject<UFortItemDefinition>("/Game/Athena/Items/Weapons/WID_Shotgun_SemiAuto_Athena_R_Ore_T03.WID_Shotgun_SemiAuto_Athena_R_Ore_T03");
	Row1.DropCount = 1;
	Row1.LoadedAmmo = GetAmmoForDef((UFortWeaponItemDefinition*)Row1.Def);
	Row1.Weight = 0.28f;
	LootForTiers["Loot_AthenaFloorLoot_Warmup"].push_back(Row1);
	LootForTiers["Loot_AthenaFloorLoot"].push_back(Row1);
	Row1.Weight = 0.13f;
	LootForTiers["Loot_AthenaTreasure"].push_back(Row1);

	Row1.Def = StaticLoadObject<UFortItemDefinition>("/Game/Athena/Items/Weapons/WID_Shotgun_SemiAuto_Athena_VR_Ore_T03.WID_Shotgun_SemiAuto_Athena_VR_Ore_T03");
	Row1.DropCount = 1;
	Row1.LoadedAmmo = GetAmmoForDef((UFortWeaponItemDefinition*)Row1.Def);
	Row1.Weight = 0.07f;
	LootForTiers["Loot_AthenaFloorLoot_Warmup"].push_back(Row1);
	LootForTiers["Loot_AthenaFloorLoot"].push_back(Row1);
	Row1.Weight = 0.28f;
	LootForTiers["Loot_AthenaTreasure"].push_back(Row1);

	Row1.Def = StaticLoadObject<UFortItemDefinition>("/Game/Athena/Items/Weapons/WID_Shotgun_HighSemiAuto_Athena_VR_Ore_T03.WID_Shotgun_HighSemiAuto_Athena_VR_Ore_T03");
	Row1.DropCount = 1;
	Row1.LoadedAmmo = GetAmmoForDef((UFortWeaponItemDefinition*)Row1.Def);
	Row1.Weight = 0.2f;
	LootForTiers["Loot_AthenaTreasure"].push_back(Row1);

	Row1.Def = StaticLoadObject<UFortItemDefinition>("/Game/Athena/Items/Weapons/WID_Shotgun_HighSemiAuto_Athena_SR_Ore_T03.WID_Shotgun_HighSemiAuto_Athena_SR_Ore_T03");
	Row1.DropCount = 1;
	Row1.LoadedAmmo = GetAmmoForDef((UFortWeaponItemDefinition*)Row1.Def);
	Row1.Weight = 0.05f;
	LootForTiers["Loot_AthenaTreasure"].push_back(Row1);
	//End TAC

	//Start supprcessed smg
	Row1.Def = StaticLoadObject<UFortItemDefinition>("/Game/Athena/Items/Weapons/WID_Pistol_AutoHeavySuppressed_Athena_C_Ore_T02.WID_Pistol_AutoHeavySuppressed_Athena_C_Ore_T02");
	Row1.DropCount = 1;
	Row1.LoadedAmmo = GetAmmoForDef((UFortWeaponItemDefinition*)Row1.Def);
	Row1.Weight = 0.10f;
	LootForTiers["Loot_AthenaFloorLoot_Warmup"].push_back(Row1);
	LootForTiers["Loot_AthenaFloorLoot"].push_back(Row1);
	LootForTiers["Loot_AthenaTreasure"].push_back(Row1);

	Row1.Def = StaticLoadObject<UFortItemDefinition>("/Game/Athena/Items/Weapons/WID_Pistol_AutoHeavySuppressed_Athena_UC_Ore_T03.WID_Pistol_AutoHeavySuppressed_Athena_UC_Ore_T03");
	Row1.DropCount = 1;
	Row1.LoadedAmmo = GetAmmoForDef((UFortWeaponItemDefinition*)Row1.Def);
	Row1.Weight = 0.15f;
	LootForTiers["Loot_AthenaFloorLoot_Warmup"].push_back(Row1);
	LootForTiers["Loot_AthenaFloorLoot"].push_back(Row1);
	LootForTiers["Loot_AthenaTreasure"].push_back(Row1);

	Row1.Def = StaticLoadObject<UFortItemDefinition>("/Game/Athena/Items/Weapons/WID_Pistol_AutoHeavySuppressed_Athena_R_Ore_T03.WID_Pistol_AutoHeavySuppressed_Athena_R_Ore_T03");
	Row1.DropCount = 1;
	Row1.LoadedAmmo = GetAmmoForDef((UFortWeaponItemDefinition*)Row1.Def);
	Row1.Weight = 0.07f;
	LootForTiers["Loot_AthenaFloorLoot_Warmup"].push_back(Row1);
	LootForTiers["Loot_AthenaFloorLoot"].push_back(Row1);
	Row1.Weight = 0.25f;
	LootForTiers["Loot_AthenaTreasure"].push_back(Row1);
	//End Suppressed smg

	//SMG
	Row1.Def = StaticLoadObject<UFortItemDefinition>("/Game/Athena/Items/Weapons/WID_Pistol_AutoHeavyPDW_Athena_C_Ore_T03.WID_Pistol_AutoHeavyPDW_Athena_C_Ore_T03");
	Row1.DropCount = 1;
	Row1.LoadedAmmo = GetAmmoForDef((UFortWeaponItemDefinition*)Row1.Def);
	Row1.Weight = 0.7f;
	LootForTiers["Loot_AthenaFloorLoot_Warmup"].push_back(Row1);
	LootForTiers["Loot_AthenaFloorLoot"].push_back(Row1);
	LootForTiers["Loot_AthenaTreasure"].push_back(Row1);

	Row1.Def = StaticLoadObject<UFortItemDefinition>("/Game/Athena/Items/Weapons/WID_Pistol_AutoHeavyPDW_Athena_UC_Ore_T03.WID_Pistol_AutoHeavyPDW_Athena_UC_Ore_T03");
	Row1.DropCount = 1;
	Row1.LoadedAmmo = GetAmmoForDef((UFortWeaponItemDefinition*)Row1.Def);
	Row1.Weight = 0.20f;
	LootForTiers["Loot_AthenaFloorLoot_Warmup"].push_back(Row1);
	LootForTiers["Loot_AthenaFloorLoot"].push_back(Row1);
	Row1.Weight = 0.13f;
	LootForTiers["Loot_AthenaTreasure"].push_back(Row1);

	Row1.Def = StaticLoadObject<UFortItemDefinition>("/Game/Athena/Items/Weapons/WID_Pistol_AutoHeavyPDW_Athena_R_Ore_T03.WID_Pistol_AutoHeavyPDW_Athena_R_Ore_T03");
	Row1.DropCount = 1;
	Row1.LoadedAmmo = GetAmmoForDef((UFortWeaponItemDefinition*)Row1.Def);
	Row1.Weight = 0.07f;
	LootForTiers["Loot_AthenaFloorLoot_Warmup"].push_back(Row1);
	LootForTiers["Loot_AthenaFloorLoot"].push_back(Row1);
	Row1.Weight = 0.35f;
	LootForTiers["Loot_AthenaTreasure"].push_back(Row1);

	Row1.Def = StaticLoadObject<UFortItemDefinition>("/Game/Athena/Items/Weapons/WID_Pistol_AutoHeavyPDW_Athena_VR_Ore_T03.WID_Pistol_AutoHeavyPDW_Athena_VR_Ore_T03");
	Row1.DropCount = 1;
	Row1.LoadedAmmo = GetAmmoForDef((UFortWeaponItemDefinition*)Row1.Def);
	Row1.Weight = 0.05f;
	LootForTiers["Loot_AthenaTreasure"].push_back(Row1);

	Row1.Def = StaticLoadObject<UFortItemDefinition>("/Game/Athena/Items/Weapons/WID_Pistol_AutoHeavyPDW_Athena_SR_Ore_T03.WID_Pistol_AutoHeavyPDW_Athena_SR_Ore_T03");
	Row1.DropCount = 1;
	Row1.LoadedAmmo = GetAmmoForDef((UFortWeaponItemDefinition*)Row1.Def);
	Row1.Weight = 0.01f;
	LootForTiers["Loot_AthenaTreasure"].push_back(Row1);
	//End SMG

	//Bolt Action Sniper
	Row1.Def = StaticLoadObject<UFortItemDefinition>("/Game/Athena/Items/Weapons/WID_Sniper_BoltAction_Scope_Athena_R_Ore_T03.WID_Sniper_BoltAction_Scope_Athena_R_Ore_T03");
	Row1.DropCount = 1;
	Row1.LoadedAmmo = GetAmmoForDef((UFortWeaponItemDefinition*)Row1.Def);
	Row1.Weight = 0.25;
	LootForTiers["Loot_AthenaFloorLoot_Warmup"].push_back(Row1);
	LootForTiers["Loot_AthenaFloorLoot"].push_back(Row1);
	LootForTiers["Loot_AthenaTreasure"].push_back(Row1);

	Row1.Def = StaticLoadObject<UFortItemDefinition>("/Game/Athena/Items/Weapons/WID_Sniper_BoltAction_Scope_Athena_VR_Ore_T03.WID_Sniper_BoltAction_Scope_Athena_VR_Ore_T03");
	Row1.DropCount = 1;
	Row1.LoadedAmmo = GetAmmoForDef((UFortWeaponItemDefinition*)Row1.Def);
	Row1.Weight = 0.1f;
	LootForTiers["Loot_AthenaTreasure"].push_back(Row1);

	Row1.Def = StaticLoadObject<UFortItemDefinition>("/Game/Athena/Items/Weapons/WID_Sniper_BoltAction_Scope_Athena_SR_Ore_T03.WID_Sniper_BoltAction_Scope_Athena_SR_Ore_T03");
	Row1.DropCount = 1;
	Row1.LoadedAmmo = GetAmmoForDef((UFortWeaponItemDefinition*)Row1.Def);
	Row1.Weight = 0.05f;
	LootForTiers["Loot_AthenaTreasure"].push_back(Row1);
	//End Bolt Action Sniper

	//Heavy Sniper
	Row1.Def = StaticLoadObject<UFortItemDefinition>("/Game/Athena/Items/Weapons/WID_Sniper_Heavy_Athena_VR_Ore_T03.WID_Sniper_Heavy_Athena_VR_Ore_T03");
	Row1.DropCount = 1;
	Row1.LoadedAmmo = GetAmmoForDef((UFortWeaponItemDefinition*)Row1.Def);
	Row1.Weight = 0.07f;
	LootForTiers["Loot_AthenaTreasure"].push_back(Row1);

	Row1.Def = StaticLoadObject<UFortItemDefinition>("/Game/Athena/Items/Weapons/WID_Sniper_Heavy_Athena_VR_Ore_T03.WID_Sniper_Heavy_Athena_VR_Ore_T03");
	Row1.DropCount = 1;
	Row1.LoadedAmmo = GetAmmoForDef((UFortWeaponItemDefinition*)Row1.Def);
	Row1.Weight = 0.04f;
	LootForTiers["Loot_AthenaTreasure"].push_back(Row1);
	//End Heavy Sniper

	//Consumables
	Row1.Def = StaticLoadObject<UFortItemDefinition>("/Game/Athena/Items/Consumables/Grenade/Athena_Grenade.Athena_Grenade");
	Row1.DropCount = 3;
	Row1.LoadedAmmo = 0;
	Row1.Weight = 0.25f;
	LootForTiers["Loot_AthenaFloorLoot_Warmup"].push_back(Row1);
	LootForTiers["Loot_AthenaFloorLoot"].push_back(Row1);
	LootForTiers["Loot_AthenaTreasure"].push_back(Row1);
	LootForTiers["Loot_AthenaTreasure_Blue"].push_back(Row1);
	LootForTiers["Loot_AthenaTreasure_Red"].push_back(Row1);
	LootForTiers["Loot_AthenaTreasure_White"].push_back(Row1);
	LootForTiers["Airdrop"].push_back(Row1);

	Row1.Def = StaticLoadObject<UFortItemDefinition>("/Game/Athena/Items/Consumables/Shields/Athena_Shields.Athena_Shields");
	Row1.DropCount = 1;
	Row1.LoadedAmmo = 0;
	Row1.Weight = 0.25f;
	LootForTiers["Loot_AthenaFloorLoot_Warmup"].push_back(Row1);
	LootForTiers["Loot_AthenaFloorLoot"].push_back(Row1);
	LootForTiers["Loot_AthenaTreasure"].push_back(Row1);
	LootForTiers["Loot_AthenaTreasure_Blue"].push_back(Row1);
	LootForTiers["Loot_AthenaTreasure_Red"].push_back(Row1);
	LootForTiers["Loot_AthenaTreasure_White"].push_back(Row1);
	LootForTiers["Airdrop"].push_back(Row1);

	Row1.Def = StaticLoadObject<UFortItemDefinition>("/Game/Athena/Items/Consumables/ShieldSmall/Athena_ShieldSmall.Athena_ShieldSmall");
	Row1.DropCount = 3;
	Row1.LoadedAmmo = 0;
	Row1.Weight = 0.25f;
	LootForTiers["Loot_AthenaFloorLoot_Warmup"].push_back(Row1);
	LootForTiers["Loot_AthenaFloorLoot"].push_back(Row1);
	LootForTiers["Loot_AthenaTreasure"].push_back(Row1);
	LootForTiers["Loot_AthenaTreasure_Blue"].push_back(Row1);
	LootForTiers["Loot_AthenaTreasure_Red"].push_back(Row1);
	LootForTiers["Loot_AthenaTreasure_White"].push_back(Row1);
	LootForTiers["Airdrop"].push_back(Row1);

	Row1.Def = StaticLoadObject<UFortItemDefinition>("/Game/Athena/Items/Consumables/Bandage/Athena_Bandage.Athena_Bandage");
	Row1.DropCount = 5;
	Row1.LoadedAmmo = GetAmmoForDef((UFortWeaponItemDefinition*)Row1.Def);
	Row1.Weight = 0.25f;
	LootForTiers["Loot_AthenaFloorLoot_Warmup"].push_back(Row1);
	LootForTiers["Loot_AthenaFloorLoot"].push_back(Row1);
	LootForTiers["Loot_AthenaTreasure"].push_back(Row1);
	LootForTiers["Loot_AthenaTreasure_Blue"].push_back(Row1);
	LootForTiers["Loot_AthenaTreasure_Red"].push_back(Row1);
	LootForTiers["Loot_AthenaTreasure_White"].push_back(Row1);
	LootForTiers["Airdrop"].push_back(Row1);

	Row1.Def = StaticLoadObject<UFortItemDefinition>("/Game/Athena/Items/Consumables/Medkit/Athena_Medkit.Athena_Medkit");
	Row1.DropCount = 1;
	Row1.LoadedAmmo = 0;
	Row1.Weight = 0.30f;
	LootForTiers["Loot_AthenaFloorLoot_Warmup"].push_back(Row1);
	LootForTiers["Loot_AthenaFloorLoot"].push_back(Row1);
	LootForTiers["Loot_AthenaTreasure"].push_back(Row1);
	LootForTiers["Loot_AthenaTreasure_Blue"].push_back(Row1);
	LootForTiers["Loot_AthenaTreasure_Red"].push_back(Row1);
	LootForTiers["Loot_AthenaTreasure_White"].push_back(Row1);
	LootForTiers["Airdrop"].push_back(Row1);
	//End Consumables

	//Ammo
	Row1.Def = StaticLoadObject<UFortItemDefinition>("/Game/Athena/Items/Ammo/AthenaAmmoDataBulletsHeavy.AthenaAmmoDataBulletsHeavy");
	Row1.DropCount = ((UFortAmmoItemDefinition*)Row1.Def)->DropCount;
	Row1.LoadedAmmo = 0;
	Row1.Weight = 0.25f;
	LootForTiers["Loot_AthenaAmmoLarge"].push_back(Row1);

	Row1.Def = StaticLoadObject<UFortItemDefinition>("/Game/Athena/Items/Ammo/AthenaAmmoDataBulletsLight.AthenaAmmoDataBulletsLight");
	Row1.DropCount = ((UFortAmmoItemDefinition*)Row1.Def)->DropCount;
	Row1.LoadedAmmo = 0;
	Row1.Weight = 0.25f;
	LootForTiers["Loot_AthenaAmmoLarge"].push_back(Row1);

	Row1.Def = StaticLoadObject<UFortItemDefinition>("/Game/Athena/Items/Ammo/AthenaAmmoDataBulletsMedium.AthenaAmmoDataBulletsMedium");
	Row1.DropCount = ((UFortAmmoItemDefinition*)Row1.Def)->DropCount;
	Row1.LoadedAmmo = 0;
	Row1.Weight = 0.25f;
	LootForTiers["Loot_AthenaAmmoLarge"].push_back(Row1);

	Row1.Def = StaticLoadObject<UFortItemDefinition>("/Game/Athena/Items/Ammo/AthenaAmmoDataShells.AthenaAmmoDataShells");
	Row1.DropCount = ((UFortAmmoItemDefinition*)Row1.Def)->DropCount;
	Row1.LoadedAmmo = 0;
	Row1.Weight = 0.25f;
	LootForTiers["Loot_AthenaAmmoLarge"].push_back(Row1);

	Row1.Def = StaticLoadObject<UFortItemDefinition>("/Game/Athena/Items/Ammo/AmmoDataRockets.AmmoDataRockets");
	Row1.DropCount = ((UFortAmmoItemDefinition*)Row1.Def)->DropCount;
	Row1.LoadedAmmo = 0;
	Row1.Weight = 0.10f;
	LootForTiers["Loot_AthenaAmmoLarge"].push_back(Row1);
	//End Ammo

	//Faction Chests
	Row1.Def = StaticLoadObject<UFortItemDefinition>("/Game/Athena/Items/Weapons/WID_Sniper_Heavy_Athena_VR_Ore_T03.WID_Sniper_Heavy_Athena_VR_Ore_T03");
	Row1.DropCount = 1;
	Row1.LoadedAmmo = GetAmmoForDef((UFortWeaponItemDefinition*)Row1.Def);
	Row1.Weight = 0.20f;
	LootForTiers["Loot_AthenaTreasure_Blue"].push_back(Row1);
	LootForTiers["Loot_AthenaTreasure_Red"].push_back(Row1);
	LootForTiers["Loot_AthenaTreasure_White"].push_back(Row1);

	Row1.Def = StaticLoadObject<UFortItemDefinition>("/Game/Athena/Items/Weapons/WID_Sniper_Heavy_Athena_VR_Ore_T03.WID_Sniper_Heavy_Athena_VR_Ore_T03");
	Row1.DropCount = 1;
	Row1.LoadedAmmo = GetAmmoForDef((UFortWeaponItemDefinition*)Row1.Def);
	Row1.Weight = 0.15f;
	LootForTiers["Loot_AthenaTreasure_Blue"].push_back(Row1);
	LootForTiers["Loot_AthenaTreasure_Red"].push_back(Row1);
	LootForTiers["Loot_AthenaTreasure_White"].push_back(Row1);

	Row1.Def = StaticLoadObject<UFortItemDefinition>("/Game/Athena/Items/Weapons/WID_Assault_Heavy_Athena_VR_Ore_T03.WID_Assault_Heavy_Athena_VR_Ore_T03");
	Row1.DropCount = 1;
	Row1.LoadedAmmo = GetAmmoForDef((UFortWeaponItemDefinition*)Row1.Def);
	Row1.Weight = 0.30f;
	LootForTiers["Loot_AthenaTreasure_Blue"].push_back(Row1);
	LootForTiers["Loot_AthenaTreasure_Red"].push_back(Row1);
	LootForTiers["Loot_AthenaTreasure_White"].push_back(Row1);

	Row1.Def = StaticLoadObject<UFortItemDefinition>("/Game/Athena/Items/Weapons/WID_Assault_Heavy_Athena_R_Ore_T03.WID_Assault_Heavy_Athena_R_Ore_T03");
	Row1.DropCount = 1;
	Row1.LoadedAmmo = GetAmmoForDef((UFortWeaponItemDefinition*)Row1.Def);
	Row1.Weight = 0.65f;
	LootForTiers["Loot_AthenaTreasure_Blue"].push_back(Row1);
	LootForTiers["Loot_AthenaTreasure_Red"].push_back(Row1);
	LootForTiers["Loot_AthenaTreasure_White"].push_back(Row1);

	Row1.Def = StaticLoadObject<UFortItemDefinition>("/Game/Athena/Items/Weapons/WID_Shotgun_Standard_Athena_SR_Ore_T03.WID_Shotgun_Standard_Athena_SR_Ore_T03");
	Row1.DropCount = 1;
	Row1.LoadedAmmo = GetAmmoForDef((UFortWeaponItemDefinition*)Row1.Def);
	Row1.Weight = 0.25f;
	LootForTiers["Loot_AthenaTreasure_Blue"].push_back(Row1);
	LootForTiers["Loot_AthenaTreasure_Red"].push_back(Row1);
	LootForTiers["Loot_AthenaTreasure_White"].push_back(Row1);
	//End Faction Chests

	//RodBoxes
	Row1.Def = StaticLoadObject<UFortItemDefinition>("/Game/Athena/Items/Consumables/FloppingRabbit/WID_Athena_FloppingRabbit.WID_Athena_FloppingRabbit");
	Row1.DropCount = 1;
	Row1.Weight = 0.75f;
	LootForTiers["Loot_ApolloRodBox"].push_back(Row1);

	Row1.Def = StaticLoadObject<UFortItemDefinition>("/Game/Athena/Items/Consumables/HappyGhost/WID_Athena_HappyGhost.WID_Athena_HappyGhost");
	Row1.DropCount = 1;
	Row1.LoadedAmmo = 10;
	Row1.Weight = 0.25f;
	LootForTiers["Loot_ApolloRodBox"].push_back(Row1);
	//End RodBoxes

	//Airdrops
	Row1.Def = StaticLoadObject<UFortItemDefinition>("/Game/Items/ResourcePickups/WoodItemData.WoodItemData");
	Row1.DropCount = 30;
	Row1.Weight = 0.50f;
	LootForTiers["Airdrop"].push_back(Row1);

	Row1.Def = StaticLoadObject<UFortItemDefinition>("/Game/Items/ResourcePickups/StoneItemData.StoneItemData");
	Row1.DropCount = 30;
	Row1.Weight = 0.50f;
	LootForTiers["Airdrop"].push_back(Row1);

	Row1.Def = StaticLoadObject<UFortItemDefinition>("/Game/Items/ResourcePickups/MetalItemData.MetalItemData");
	Row1.DropCount = 30;
	Row1.Weight = 0.50f;
	LootForTiers["Airdrop"].push_back(Row1);

	Row1.Def = StaticLoadObject<UFortItemDefinition>("/Game/Athena/Items/Weapons/WID_Sniper_BoltAction_Scope_Athena_VR_Ore_T03.WID_Sniper_BoltAction_Scope_Athena_VR_Ore_T03");
	Row1.DropCount = 1;
	Row1.LoadedAmmo = GetAmmoForDef((UFortWeaponItemDefinition*)Row1.Def);
	Row1.Weight = 0.35f;
	LootForTiers["Airdrop"].push_back(Row1);

	Row1.Def = StaticLoadObject<UFortItemDefinition>("/Game/Athena/Items/Weapons/WID_Sniper_Heavy_Athena_VR_Ore_T03.WID_Sniper_Heavy_Athena_VR_Ore_T03");
	Row1.DropCount = 1;
	Row1.LoadedAmmo = GetAmmoForDef((UFortWeaponItemDefinition*)Row1.Def);
	Row1.Weight = 0.30f;
	LootForTiers["Airdrop"].push_back(Row1);

	Row1.Def = StaticLoadObject<UFortItemDefinition>("/Game/Athena/Items/Weapons/WID_Pistol_AutoHeavyPDW_Athena_VR_Ore_T03.WID_Pistol_AutoHeavyPDW_Athena_VR_Ore_T03");
	Row1.DropCount = 1;
	Row1.LoadedAmmo = GetAmmoForDef((UFortWeaponItemDefinition*)Row1.Def);
	Row1.Weight = 0.35f;
	LootForTiers["Airdrop"].push_back(Row1);

	Row1.Def = StaticLoadObject<UFortItemDefinition>("/Game/Athena/Items/Weapons/WID_Shotgun_HighSemiAuto_Athena_VR_Ore_T03.WID_Shotgun_HighSemiAuto_Athena_VR_Ore_T03");
	Row1.DropCount = 1;
	Row1.LoadedAmmo = GetAmmoForDef((UFortWeaponItemDefinition*)Row1.Def);
	Row1.Weight = 0.40f;
	LootForTiers["Airdrop"].push_back(Row1);

	Row1.Def = StaticLoadObject<UFortItemDefinition>("/Game/Athena/Items/Weapons/WID_Shotgun_Standard_Athena_VR_Ore_T03.WID_Shotgun_Standard_Athena_VR_Ore_T03");
	Row1.DropCount = 1;
	Row1.LoadedAmmo = GetAmmoForDef((UFortWeaponItemDefinition*)Row1.Def);
	Row1.Weight = 0.30f;
	LootForTiers["Airdrop"].push_back(Row1);

	Row1.Def = StaticLoadObject<UFortItemDefinition>("/Game/Athena/Items/Weapons/WID_Assault_Heavy_Athena_VR_Ore_T03.WID_Assault_Heavy_Athena_VR_Ore_T03");
	Row1.DropCount = 1;
	Row1.LoadedAmmo = GetAmmoForDef((UFortWeaponItemDefinition*)Row1.Def);
	Row1.Weight = 0.32f;
	LootForTiers["Airdrop"].push_back(Row1);
	//End Airdrops
	
	//Refrigerator thing
	Row1.Def = StaticLoadObject<UFortItemDefinition>("/Game/Athena/Items/Consumables/Flopper/Small/WID_Athena_FlopperSmall.WID_Athena_FlopperSmall");
	Row1.DropCount = 3;
	Row1.LoadedAmmo = 0;
	Row1.Weight = .25f;
	LootForTiers["Loot_AthenaIceBox"].push_back(Row1);

	Row1.Def = StaticLoadObject<UFortItemDefinition>("/Game/Athena/Items/Consumables/Flopper/Effective/WID_Athena_Flopper_Effective.WID_Athena_Flopper_Effective");
	Row1.DropCount = 1;
	Row1.LoadedAmmo = 0;
	Row1.Weight = .30f;
	LootForTiers["Loot_AthenaIceBox"].push_back(Row1);

	Row1.Def = StaticLoadObject<UFortItemDefinition>("/Game/Athena/Items/Consumables/Flopper/WID_Athena_Flopper.WID_Athena_Flopper");
	Row1.DropCount = 1;
	Row1.LoadedAmmo = 0;
	Row1.Weight = .20f;
	LootForTiers["Loot_AthenaIceBox"].push_back(Row1);

	Row1.Def = StaticLoadObject<UFortItemDefinition>("/Game/Athena/Items/Consumables/Bucket/Nice/WID_Athena_Bucket_Nice.WID_Athena_Bucket_Nice");
	Row1.DropCount = 1;
	Row1.LoadedAmmo = 0;
	Row1.Weight = 0.001f;
	LootForTiers["Loot_AthenaIceBox"].push_back(Row1);
	//End Refrigerator thing
}

template<typename T>
bool VectorContains(T Item, vector<T>& Vector)
{
	for (auto& Def : Vector)
	{
		if (Def == Item)
			return true;
	}

	return false;
}

ItemRow* GetRandomItem(string LootTier, EFortItemType ItemType = EFortItemType::EFortItemType_MAX, bool EnableDefs = false)
{
	static vector<UFortItemDefinition*> LastDefs{};

	if (!LootForTiers.contains(LootTier))
		return nullptr;

	auto& Vector = LootForTiers[LootTier];

	if (Vector.size() <= 0)
		return nullptr;
	static auto rng = default_random_engine((unsigned int)time(0));
	shuffle(Vector.begin(), Vector.end(), rng);

	ItemRow* Ret = &Vector[GetMath()->RandomIntegerInRange(0, Vector.size() - 1)];

	if (EnableDefs)
	{
		bool contains = VectorContains<UFortItemDefinition*>(Ret->Def, LastDefs);

		if (contains)
			return GetRandomItem(LootTier, ItemType);
	}

	if (ItemType != EFortItemType::EFortItemType_MAX && Ret->Def->GetItemType() != ItemType)
		return GetRandomItem(LootTier, ItemType);

	if (!GetMath()->RandomBoolWithWeight(Ret->Weight))
		return GetRandomItem(LootTier, ItemType);

	if (EnableDefs)
	{
		LastDefs.push_back(Ret->Def);

		if (LastDefs.size() > 3)
			LastDefs.erase(LastDefs.begin());
	}

	return Ret;
}

char __fastcall SpawnLoot(ABuildingContainer* Object)
{
	if (!bEnableLooting)
		return 0;

	string TierGroupStr = Object->SearchLootTierGroup.ToString();

	if (!LootForTiers.contains(TierGroupStr))
	{
		Log("Invalid TierGroup " + TierGroupStr);
		return 0;
	}

	cout << LootForTiers[TierGroupStr].size() << endl;

	EFortPickupSpawnSource Source = EFortPickupSpawnSource::Unset;
	EFortPickupSourceTypeFlag Flag = EFortPickupSourceTypeFlag::Container;
	bool IsChest = false;
	bool IsFactionChest = false;

	if (TierGroupStr == "Loot_AthenaTreasure")
	{
		Source = EFortPickupSpawnSource::Chest;
		IsChest = true;
	}
	else if (TierGroupStr == "Loot_AthenaAmmoLarge")
	{
		Source = EFortPickupSpawnSource::AmmoBox;
	}
	else if (TierGroupStr.contains("FloorLoot"))
	{
		Flag = EFortPickupSourceTypeFlag::FloorLoot;
	}

	IsFactionChest = TierGroupStr == "Loot_AthenaTreasure_Blue" || TierGroupStr == "Loot_AthenaTreasure_Red";

	if (TierGroupStr == "Loot_AthenaIceBox")
	{
		Object->LootSpawnLocation_Athena.Y += 45;
	}

	FVector Loc = Object->K2_GetActorLocation() + (Object->GetActorForwardVector() * Object->LootSpawnLocation_Athena.X) + (Object->GetActorRightVector() * Object->LootSpawnLocation_Athena.Y) + (Object->GetActorUpVector() * Object->LootSpawnLocation_Athena.Z);

	ItemRow* Item = GetRandomItem(TierGroupStr, (IsChest ? EFortItemType::WeaponRanged : EFortItemType::EFortItemType_MAX));
	if (!Item || !Item->Def)
		return 0;

	SpawnPickup(Loc, Item->Def, Item->DropCount, Item->LoadedAmmo, Flag, Source);

	if (TierGroupStr == "Loot_AthenaIceBox")
	{
		for (size_t i = 0; i < 2; i++)
		{
			ItemRow* Item = GetRandomItem(TierGroupStr, EFortItemType::EFortItemType_MAX, true);
			if (!Item || !Item->Def)
				return 0;
			SpawnPickup(Loc, Item->Def, Item->DropCount, Item->LoadedAmmo, Flag, Source);
		}
	}
	
	UFortAmmoItemDefinition* AmmoDef = (UFortAmmoItemDefinition*)((UFortWeaponRangedItemDefinition*)Item->Def)->GetAmmoWorldItemDefinition_BP();
	if (AmmoDef && Item->Def != AmmoDef && AmmoDef->DropCount > 0)
	{
		SpawnPickup(Loc, AmmoDef, AmmoDef->DropCount, 0, Flag, Source);
	}

	if (IsChest || IsFactionChest)
	{
		ItemRow* ConsItem = GetRandomItem(TierGroupStr, EFortItemType::Consumable);
		if (!ConsItem)
			return 0;
		SpawnPickup(Loc, ConsItem->Def, ConsItem->DropCount, ConsItem->LoadedAmmo, Flag, Source);

		static auto Wood = StaticLoadObject<UFortItemDefinition>("/Game/Items/ResourcePickups/WoodItemData.WoodItemData");
		static auto Metal = StaticLoadObject<UFortItemDefinition>("/Game/Items/ResourcePickups/MetalItemData.MetalItemData");
		static auto Stone = StaticLoadObject<UFortItemDefinition>("/Game/Items/ResourcePickups/StoneItemData.StoneItemData");
		UFortItemDefinition* Def = nullptr;
		if (rand() % 40 > 20)
		{
			Def = rand() % 20 > 10 ? Wood : Stone;
		}
		else
		{
			Def = Metal;
		}
		SpawnPickup(Loc, Def, 30, 0, Flag, Source);

	}

	if (IsFactionChest)
	{
		for (size_t i = 0; i < 3; i++)
		{
			ItemRow* Item = GetRandomItem(TierGroupStr, (IsChest ? EFortItemType::WeaponRanged : EFortItemType::EFortItemType_MAX));
			if (!Item || !Item->Def)
				return 0;

			SpawnPickup(Loc, Item->Def, Item->DropCount, Item->LoadedAmmo, Flag, Source);
			UFortAmmoItemDefinition* AmmoDef = (UFortAmmoItemDefinition*)((UFortWeaponRangedItemDefinition*)Item->Def)->GetAmmoWorldItemDefinition_BP();
			if (AmmoDef && Item->Def != AmmoDef && AmmoDef->DropCount > 0)
			{
				SpawnPickup(Loc, AmmoDef, AmmoDef->DropCount, 0, Flag, Source);
			}
		}
	}

	Object->bAlreadySearched = true;
	Object->SearchBounceData.SearchAnimationCount++;
	Object->BounceContainer();
	Object->OnRep_bAlreadySearched();

	return 1;
}