#pragma once
#include "ue.h"
#include "Inventory.h"

__int64 (*OnReloadOG)(AFortWeapon* Weapon, int RemoveCount);
__int64 OnReload(AFortWeapon* Weapon, int RemoveCount)
{
	auto Ret = OnReloadOG(Weapon, RemoveCount);
	auto WeaponDef = Weapon->WeaponData;
	if (!WeaponDef)
		return Ret;

	auto AmmoDef = WeaponDef->GetAmmoWorldItemDefinition_BP();
	if (!AmmoDef)
		return Ret;
	AFortPlayerPawnAthena* Pawn = (AFortPlayerPawnAthena*)Weapon->GetOwner();
	AFortPlayerControllerAthena* PC = (AFortPlayerControllerAthena*)Pawn->Controller;
	if (!PC->IsA(AFortPlayerControllerAthena::StaticClass()))
		return Ret;

	if (PC->bInfiniteAmmo)
		return Ret;

	Remove(PC, AmmoDef, RemoveCount);
	UpdateLoadedAmmo(PC, Weapon, RemoveCount);

	return Ret;
}