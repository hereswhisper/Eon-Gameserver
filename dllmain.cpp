#include <Windows.h>
#include <cstdio>
#include <string>
#include <iostream>
#include <map>
#include <functional>
#include "PC.h"
#include "GameMode.h"
#include "misc.h"
#include "Pawn.h"
#include "PlayerState.h"
#include "SpyGames.h"
#include "Looting.h"
#include "Weapon.h"
#include "Debug.h"
#include "Bots.h"
#include "minhook/MinHook.h"

#pragma comment(lib, "minhook/minhook.lib")


__int64 NoMcpHook()
{
    return !bUsingApi;
}

DWORD InitThread(LPVOID)
{
    AllocConsole();
    FILE* Console;
    freopen_s(&Console, "CONOUT$", "w", stdout);
    printf("injected gs\n");
    srand(time(0));
    MH_Initialize();
    InitGObjects();
    
    __int64 BaseAddr = __int64(GetModuleHandleW(0));
    
    LPVOID TickFlushAddr = (LPVOID)(BaseAddr + 0x42C3ED0);
    LPVOID KickPlayerAddr = (LPVOID)(BaseAddr + 0x4155600);
    LPVOID WorldNetModeAddr = (LPVOID)(BaseAddr + 0x45C9D90);
    LPVOID QueryPatchAddr = (LPVOID)(BaseAddr + 0x108D740);
    LPVOID NoMcp = (LPVOID)(BaseAddr + 0x1EAA500);
    LPVOID CollectGarbageAddr = (LPVOID)(BaseAddr + 0x2D95E00);
    LPVOID GetMaxTickRateAddrOK = (LPVOID)((*(void***)GetEngine())[86]);
    LPVOID ActorNetModeAddr = (LPVOID)(BaseAddr + 0x3EB6780);
    LPVOID ActorNetModeCrashAddr = (LPVOID)(BaseAddr + 0x1E23840);
    LPVOID PickTeamAddr = (LPVOID)(BaseAddr + 0x18E6B20);
    LPVOID GetPlayerViewpointAddr = (LPVOID)(BaseAddr + 0x22E0D50);
    LPVOID SpawnDefaultPawnForAddr = (LPVOID)(BaseAddr + 0x18F6250);
    LPVOID ReadyToStartMatchAddr = (LPVOID)(BaseAddr + 0x4640A30);
    LPVOID ServerReadyToStartMatchAddr = (LPVOID)((*(void***)AAthena_PlayerController_C::StaticClass()->DefaultObject)[0x269]);
    LPVOID ServerLoadingScreenDroppedAddr = (LPVOID)((*(void***)AAthena_PlayerController_C::StaticClass()->DefaultObject)[0x26B]);
    LPVOID ClientOnPawnDiedAddr = (LPVOID)(BaseAddr + 0x29B5C80);
    LPVOID SpawnLootAddr = (LPVOID)(BaseAddr + 0x1B46D00);
    LPVOID ReloadAddr = (LPVOID)(BaseAddr + 0x260C490);
    LPVOID OnAircraftExitDropZoneAddr = (LPVOID)(BaseAddr + 0x18E07D0);
    LPVOID SpawnBotAddr = (LPVOID)(BaseAddr + 0x19E9B10);
    LPVOID GetCalendarInfoAddr = (LPVOID)(BaseAddr + 0x1E5FC90);
    LPVOID Test3Addr = (LPVOID)(BaseAddr + 0x1637F50);
    LPVOID PreloginAddr = (LPVOID)(BaseAddr + 0x18E8560);
    LPVOID ContextAddr = (LPVOID)(BaseAddr + 0x2DBCBA0);
    LPVOID stormaddr = (LPVOID)(BaseAddr + 0x18FD350);
    LPVOID PEAddr = (LPVOID)(BaseAddr + Offsets::ProcessEvent);
    LPVOID CreativeCrash = (LPVOID)(BaseAddr + 0x21D16C0);
    LPVOID OnCapsuleBeginOverlapAddr = (LPVOID)(BaseAddr + 0x22A08C0);
    LPVOID GameSessionRestartAddr = (LPVOID)(BaseAddr + 0x20E8850);
    LPVOID UnlockPerkForPlayerAddr = (LPVOID)(BaseAddr + 0x1A962E0);
    LPVOID OnMutatorItemSelectedAddr = (LPVOID)(BaseAddr + 0x1A8CAD0);
    LPVOID LogAddr = (LPVOID)(BaseAddr + 0x2B98BE0);
    LPVOID SetMatchPlacementAddr = (LPVOID)(BaseAddr + 0x19E7900);
    LPVOID StartAircraftPhaseAddr = (LPVOID)(BaseAddr + 0x18F9BB0);
    LPVOID SpawnPlayActorAddr = (LPVOID)(BaseAddr + 0x421DF40);
    LPVOID ServerMoveAddr = (LPVOID)(BaseAddr + 0x296F0A0);
    LPVOID OnPossessedPawnDiedAddr = (LPVOID)(BaseAddr + 0x163C760);
    LPVOID ServerAttemptInteractAddr = (LPVOID)(BaseAddr + 0x1C8EA00);
    LPVOID OnDamageServerAddr = (LPVOID)(BaseAddr + 0x2683F80);
    LPVOID DestroyPickup = (LPVOID)(BaseAddr + 0x1F96650);
    LPVOID OnPerceptionSensedAddr = (LPVOID)(BaseAddr + 0x163C300);

    SwapVTable(APlayerPawn_Athena_C::StaticClass()->DefaultObject, 0x1EA, ServerHandlePickup, (void**)&ServerHandlePickupOG);
    SwapVTable(APlayerPawn_Athena_C::StaticClass()->DefaultObject, 0x1E6, ServerHandlePickupWithSwap);
    SwapVTable(APlayerPawn_Athena_C::StaticClass()->DefaultObject, 0x1E8, ServerHandlePickupWithSwap);

    ProcessEvent = decltype(ProcessEvent)(BaseAddr + Offsets::ProcessEvent);

    for (size_t i = 0; i < UObject::GObjects->Num(); i++)
    {
        UObject* Obj = UObject::GObjects->GetByIndex(i);
        if (Obj && Obj->IsA(AFortPhysicsPawn::StaticClass()))
        {
            SwapVTable(Obj->Class->DefaultObject, 0xEE, ServerMove);
        }
    }

    for (size_t i = 0; i < UObject::GObjects->Num(); i++)
    {
        UObject* Obj = UObject::GObjects->GetByIndex(i);
        if (Obj && Obj->IsA(AB_Prj_Athena_Consumable_Thrown_C::StaticClass()))
        {
            SwapVTable(Obj->Class->DefaultObject, 0x44, OnExploded);
        }
    }

    for (size_t i = 0; i < UObject::GObjects->Num(); i++)
    {
        UObject* Obj = UObject::GObjects->GetByIndex(i);
        if (Obj && Obj->IsA(APlayerController::StaticClass()))
        {
            SwapVTable(Obj->Class->DefaultObject, 0x1C5, ServerCheat);
        }
    }

    SpecName = Conv_StringToName(TEXT("Spectating"));
    PickaxeTagName = Conv_StringToName(TEXT("Weapon.Melee.Impact.Pickaxe"));
    
    UFortWeaponItemDefinition* ArDef = StaticLoadObject<UFortWeaponItemDefinition>("/Game/Athena/Items/Weapons/WID_Assault_Auto_Athena_R_Ore_T03.WID_Assault_Auto_Athena_R_Ore_T03");
    UFortWeaponItemDefinition* FlopperDef = StaticLoadObject<UFortWeaponItemDefinition>("/Game/Athena/Items/Consumables/Flopper/WID_Athena_Flopper.WID_Athena_Flopper");
    UFortWeaponItemDefinition* PumpDef = StaticLoadObject<UFortWeaponItemDefinition>("/Game/Athena/Items/Weapons/WID_Shotgun_Standard_Athena_SR_Ore_T03.WID_Shotgun_Standard_Athena_SR_Ore_T03");
    UFortWeaponItemDefinition* SmgDef = StaticLoadObject<UFortWeaponItemDefinition>("/Game/Athena/Items/Weapons/WID_Pistol_AutoHeavyPDW_Athena_R_Ore_T03.WID_Pistol_AutoHeavyPDW_Athena_R_Ore_T03");
    UFortWeaponItemDefinition* DrumgunDef = StaticLoadObject<UFortWeaponItemDefinition>("/Game/Athena/Items/Weapons/Boss/WID_Boss_Midas.WID_Boss_Midas");
    UFortWeaponItemDefinition* SlurpFishDef = UObject::FindObject<UFortWeaponItemDefinition>("WID_Athena_Flopper_Effective.WID_Athena_Flopper_Effective");
    UFortWeaponItemDefinition* SmallShieldDef = UObject::FindObject<UFortWeaponItemDefinition>("Athena_ShieldSmall.Athena_ShieldSmall");

    Loadout Loadout1{};
    Loadout1.ItemDefinitions[PumpDef].push_back(1);
    Loadout1.ItemDefinitions[PumpDef].push_back(5);
    Loadout1.ItemDefinitions[PumpDef].push_back(15);
    Loadout1.ItemDefinitions[SmgDef].push_back(1);
    Loadout1.ItemDefinitions[SmgDef].push_back(30);
    Loadout1.ItemDefinitions[SmgDef].push_back(180);
    Loadout1.ItemDefinitions[ArDef].push_back(1);
    Loadout1.ItemDefinitions[ArDef].push_back(30);
    Loadout1.ItemDefinitions[ArDef].push_back(130);
    Loadout1.ItemDefinitions[SmallShieldDef].push_back(6);
    Loadout1.ItemDefinitions[SmallShieldDef].push_back(0);
    Loadout1.ItemDefinitions[SlurpFishDef].push_back(3);
    Loadout1.ItemDefinitions[SlurpFishDef].push_back(0);
    Loadout1.LoadoutType = ELoadoutType::LateGame;
    Loadouts.push_back(Loadout1);

    auto Loadout2 = Loadout{};
    Loadout2.ItemDefinitions[StaticLoadObject<UFortItemDefinition>("/Game/Athena/Items/Weapons/Boss/WID_Boss_Adventure_AR.WID_Boss_Adventure_AR")].push_back(1);
    Loadout2.ItemDefinitions[StaticLoadObject<UFortItemDefinition>("/Game/Athena/Items/Weapons/Boss/WID_Boss_Adventure_AR.WID_Boss_Adventure_AR")].push_back(30);
    Loadout2.ItemDefinitions[StaticLoadObject<UFortItemDefinition>("/Game/Athena/Items/Weapons/Boss/WID_Boss_Adventure_AR.WID_Boss_Adventure_AR")].push_back(60);
    Loadout2.ItemDefinitions[StaticLoadObject<UFortItemDefinition>("/Game/Athena/Items/Weapons/WID_Shotgun_Standard_Athena_VR_Ore_T03.WID_Shotgun_Standard_Athena_VR_Ore_T03")].push_back(1);
    Loadout2.ItemDefinitions[StaticLoadObject<UFortItemDefinition>("/Game/Athena/Items/Weapons/WID_Shotgun_Standard_Athena_VR_Ore_T03.WID_Shotgun_Standard_Athena_VR_Ore_T03")].push_back(5);
    Loadout2.ItemDefinitions[StaticLoadObject<UFortItemDefinition>("/Game/Athena/Items/Weapons/WID_Shotgun_Standard_Athena_VR_Ore_T03.WID_Shotgun_Standard_Athena_VR_Ore_T03")].push_back(15);
    Loadout2.ItemDefinitions[StaticLoadObject<UFortItemDefinition>("/Game/Athena/Items/Weapons/WID_Pistol_AutoHeavySuppressed_Athena_R_Ore_T03.WID_Pistol_AutoHeavySuppressed_Athena_R_Ore_T03")].push_back(1);
    Loadout2.ItemDefinitions[StaticLoadObject<UFortItemDefinition>("/Game/Athena/Items/Weapons/WID_Pistol_AutoHeavySuppressed_Athena_R_Ore_T03.WID_Pistol_AutoHeavySuppressed_Athena_R_Ore_T03")].push_back(30);
    Loadout2.ItemDefinitions[StaticLoadObject<UFortItemDefinition>("/Game/Athena/Items/Weapons/WID_Pistol_AutoHeavySuppressed_Athena_R_Ore_T03.WID_Pistol_AutoHeavySuppressed_Athena_R_Ore_T03")].push_back(120);
    Loadout2.ItemDefinitions[SlurpFishDef].push_back(3);
    Loadout2.ItemDefinitions[SlurpFishDef].push_back(0);
    Loadout2.ItemDefinitions[FlopperDef].push_back(3);
    Loadout2.ItemDefinitions[FlopperDef].push_back(0);
    Loadout2.LoadoutType = ELoadoutType::LateGame;
    Loadouts.push_back(Loadout2);

    auto Loadout3 = Loadout{};
    Loadout3.ItemDefinitions.clear();
    Loadout3.ItemDefinitions[StaticLoadObject<UFortItemDefinition>("/Game/Athena/Items/Weapons/WID_Assault_Auto_Athena_R_Ore_T03.WID_Assault_Auto_Athena_R_Ore_T03")].push_back(1);
    Loadout3.ItemDefinitions[StaticLoadObject<UFortItemDefinition>("/Game/Athena/Items/Weapons/WID_Assault_Auto_Athena_R_Ore_T03.WID_Assault_Auto_Athena_R_Ore_T03")].push_back(30);
    Loadout3.ItemDefinitions[StaticLoadObject<UFortItemDefinition>("/Game/Athena/Items/Weapons/WID_Assault_Auto_Athena_R_Ore_T03.WID_Assault_Auto_Athena_R_Ore_T03")].push_back(60);
    Loadout3.ItemDefinitions[StaticLoadObject<UFortItemDefinition>("/Game/Athena/Items/Weapons/WID_Shotgun_HighSemiAuto_Athena_VR_Ore_T03.WID_Shotgun_HighSemiAuto_Athena_VR_Ore_T03")].push_back(1);
    Loadout3.ItemDefinitions[StaticLoadObject<UFortItemDefinition>("/Game/Athena/Items/Weapons/WID_Shotgun_HighSemiAuto_Athena_VR_Ore_T03.WID_Shotgun_HighSemiAuto_Athena_VR_Ore_T03")].push_back(8);
    Loadout3.ItemDefinitions[StaticLoadObject<UFortItemDefinition>("/Game/Athena/Items/Weapons/WID_Shotgun_HighSemiAuto_Athena_VR_Ore_T03.WID_Shotgun_HighSemiAuto_Athena_VR_Ore_T03")].push_back(15);
    Loadout3.ItemDefinitions[StaticLoadObject<UFortItemDefinition>("/Game/Athena/Items/Weapons/WID_Pistol_AutoHeavySuppressed_Athena_R_Ore_T03.WID_Pistol_AutoHeavySuppressed_Athena_R_Ore_T03")].push_back(1);
    Loadout3.ItemDefinitions[StaticLoadObject<UFortItemDefinition>("/Game/Athena/Items/Weapons/WID_Pistol_AutoHeavySuppressed_Athena_R_Ore_T03.WID_Pistol_AutoHeavySuppressed_Athena_R_Ore_T03")].push_back(30);
    Loadout3.ItemDefinitions[StaticLoadObject<UFortItemDefinition>("/Game/Athena/Items/Weapons/WID_Pistol_AutoHeavySuppressed_Athena_R_Ore_T03.WID_Pistol_AutoHeavySuppressed_Athena_R_Ore_T03")].push_back(120);
    Loadout3.ItemDefinitions[StaticLoadObject<UFortItemDefinition>("/Game/Athena/Items/Weapons/Boss/WID_Boss_Adventure_GH.WID_Boss_Adventure_GH")].push_back(1);
    Loadout3.ItemDefinitions[StaticLoadObject<UFortItemDefinition>("/Game/Athena/Items/Weapons/Boss/WID_Boss_Adventure_GH.WID_Boss_Adventure_GH")].push_back(999999);
    Loadout3.ItemDefinitions[StaticLoadObject<UFortItemDefinition>("/Game/Athena/Items/Weapons/Boss/WID_Boss_Adventure_GH.WID_Boss_Adventure_GH")].push_back(0);
    Loadout3.ItemDefinitions[SlurpFishDef].push_back(3);
    Loadout3.ItemDefinitions[SlurpFishDef].push_back(0);
    Loadout3.LoadoutType = ELoadoutType::LateGame;
    Loadouts.push_back(Loadout3);

    auto Loadout4 = Loadout{};
    Loadout4.ItemDefinitions.clear();
    Loadout4.ItemDefinitions[StaticLoadObject<UFortItemDefinition>("/Game/Athena/Items/Weapons/Boss/WID_Boss_Midas.WID_Boss_Midas")].push_back(1);
    Loadout4.ItemDefinitions[StaticLoadObject<UFortItemDefinition>("/Game/Athena/Items/Weapons/Boss/WID_Boss_Midas.WID_Boss_Midas")].push_back(40);
    Loadout4.ItemDefinitions[StaticLoadObject<UFortItemDefinition>("/Game/Athena/Items/Weapons/Boss/WID_Boss_Midas.WID_Boss_Midas")].push_back(120);
    Loadout4.ItemDefinitions[StaticLoadObject<UFortItemDefinition>("/Game/Athena/Items/Weapons/WID_Shotgun_SemiAuto_Athena_VR_Ore_T03.WID_Shotgun_SemiAuto_Athena_VR_Ore_T03")].push_back(1);
    Loadout4.ItemDefinitions[StaticLoadObject<UFortItemDefinition>("/Game/Athena/Items/Weapons/WID_Shotgun_SemiAuto_Athena_VR_Ore_T03.WID_Shotgun_SemiAuto_Athena_VR_Ore_T03")].push_back(8);
    Loadout4.ItemDefinitions[StaticLoadObject<UFortItemDefinition>("/Game/Athena/Items/Weapons/WID_Shotgun_SemiAuto_Athena_VR_Ore_T03.WID_Shotgun_SemiAuto_Athena_VR_Ore_T03")].push_back(15);
    Loadout4.ItemDefinitions[StaticLoadObject<UFortItemDefinition>("/Game/Athena/Items/Weapons/WID_Sniper_Heavy_Athena_VR_Ore_T03.WID_Sniper_Heavy_Athena_VR_Ore_T03")].push_back(1);
    Loadout4.ItemDefinitions[StaticLoadObject<UFortItemDefinition>("/Game/Athena/Items/Weapons/WID_Sniper_Heavy_Athena_VR_Ore_T03.WID_Sniper_Heavy_Athena_VR_Ore_T03")].push_back(1);
    Loadout4.ItemDefinitions[StaticLoadObject<UFortItemDefinition>("/Game/Athena/Items/Weapons/WID_Sniper_Heavy_Athena_VR_Ore_T03.WID_Sniper_Heavy_Athena_VR_Ore_T03")].push_back(9);
    Loadout4.ItemDefinitions[SmallShieldDef].push_back(6);
    Loadout4.ItemDefinitions[SmallShieldDef].push_back(0);
    Loadout4.ItemDefinitions[StaticLoadObject<UFortItemDefinition>("/Game/Athena/Items/Consumables/Shields/Athena_Shields.Athena_Shields")].push_back(3);
    Loadout4.ItemDefinitions[StaticLoadObject<UFortItemDefinition>("/Game/Athena/Items/Consumables/Shields/Athena_Shields.Athena_Shields")].push_back(0);
    Loadout4.LoadoutType = ELoadoutType::LateGame;
    Loadouts.push_back(Loadout4);

    auto Loadout5 = Loadout{};
    Loadout5.ItemDefinitions.clear();
    Loadout5.ItemDefinitions[StaticLoadObject<UFortItemDefinition>("/Game/Athena/Items/Weapons/WID_Assault_AutoHigh_Athena_VR_Ore_T03.WID_Assault_AutoHigh_Athena_VR_Ore_T03")].push_back(1);
    Loadout5.ItemDefinitions[StaticLoadObject<UFortItemDefinition>("/Game/Athena/Items/Weapons/WID_Assault_AutoHigh_Athena_VR_Ore_T03.WID_Assault_AutoHigh_Athena_VR_Ore_T03")].push_back(30);
    Loadout5.ItemDefinitions[StaticLoadObject<UFortItemDefinition>("/Game/Athena/Items/Weapons/WID_Assault_AutoHigh_Athena_VR_Ore_T03.WID_Assault_AutoHigh_Athena_VR_Ore_T03")].push_back(60);
    Loadout5.ItemDefinitions[StaticLoadObject<UFortItemDefinition>("/Game/Athena/Items/Weapons/WID_Shotgun_Standard_Athena_UC_Ore_T03.WID_Shotgun_Standard_Athena_UC_Ore_T03")].push_back(1);
    Loadout5.ItemDefinitions[StaticLoadObject<UFortItemDefinition>("/Game/Athena/Items/Weapons/WID_Shotgun_Standard_Athena_UC_Ore_T03.WID_Shotgun_Standard_Athena_UC_Ore_T03")].push_back(5);
    Loadout5.ItemDefinitions[StaticLoadObject<UFortItemDefinition>("/Game/Athena/Items/Weapons/WID_Shotgun_Standard_Athena_UC_Ore_T03.WID_Shotgun_Standard_Athena_UC_Ore_T03")].push_back(15);   Loadout5.ItemDefinitions[StaticLoadObject<UFortItemDefinition>("/Game/Athena/Items/Weapons/WID_Pistol_AutoHeavySuppressed_Athena_R_Ore_T03.WID_Pistol_AutoHeavySuppressed_Athena_R_Ore_T03")].push_back(1);
    Loadout5.ItemDefinitions[StaticLoadObject<UFortItemDefinition>("/Game/Athena/Items/Weapons/WID_Pistol_AutoHeavySuppressed_Athena_R_Ore_T03.WID_Pistol_AutoHeavySuppressed_Athena_R_Ore_T03")].push_back(30);
    Loadout5.ItemDefinitions[StaticLoadObject<UFortItemDefinition>("/Game/Athena/Items/Weapons/WID_Pistol_AutoHeavySuppressed_Athena_R_Ore_T03.WID_Pistol_AutoHeavySuppressed_Athena_R_Ore_T03")].push_back(120);
    Loadout5.ItemDefinitions[StaticLoadObject<UFortItemDefinition>("/Game/Athena/Items/Weapons/Boss/WID_Boss_Adventure_GH.WID_Boss_Adventure_GH")].push_back(1);
    Loadout5.ItemDefinitions[StaticLoadObject<UFortItemDefinition>("/Game/Athena/Items/Weapons/Boss/WID_Boss_Adventure_GH.WID_Boss_Adventure_GH")].push_back(999999);
    Loadout5.ItemDefinitions[StaticLoadObject<UFortItemDefinition>("/Game/Athena/Items/Weapons/Boss/WID_Boss_Adventure_GH.WID_Boss_Adventure_GH")].push_back(0);
    Loadout5.ItemDefinitions[SlurpFishDef].push_back(3);
    Loadout5.ItemDefinitions[SlurpFishDef].push_back(0);
    Loadout5.LoadoutType = ELoadoutType::LateGame;
    Loadouts.push_back(Loadout5);

    Loadout5 = {};
    Loadout5.ItemDefinitions.clear();
    Loadout5.ItemDefinitions[StaticLoadObject<UFortItemDefinition>("/Game/Athena/Items/Weapons/Boss/WID_Boss_Meow_HA.WID_Boss_Meow_HA")].push_back(1);
    Loadout5.ItemDefinitions[StaticLoadObject<UFortItemDefinition>("/Game/Athena/Items/Weapons/Boss/WID_Boss_Meow_HA.WID_Boss_Meow_HA")].push_back(25);
    Loadout5.ItemDefinitions[StaticLoadObject<UFortItemDefinition>("/Game/Athena/Items/Weapons/Boss/WID_Boss_Meow_HA.WID_Boss_Meow_HA")].push_back(75);
    Loadout5.ItemDefinitions[StaticLoadObject<UFortItemDefinition>("/Game/Athena/Items/Weapons/WID_Shotgun_Standard_Athena_SR_Ore_T03.WID_Shotgun_Standard_Athena_SR_Ore_T03")].push_back(1);
    Loadout5.ItemDefinitions[StaticLoadObject<UFortItemDefinition>("/Game/Athena/Items/Weapons/WID_Shotgun_Standard_Athena_SR_Ore_T03.WID_Shotgun_Standard_Athena_SR_Ore_T03")].push_back(5);
    Loadout5.ItemDefinitions[StaticLoadObject<UFortItemDefinition>("/Game/Athena/Items/Weapons/WID_Shotgun_Standard_Athena_SR_Ore_T03.WID_Shotgun_Standard_Athena_SR_Ore_T03")].push_back(15);   Loadout5.ItemDefinitions[StaticLoadObject<UFortItemDefinition>("/Game/Athena/Items/Weapons/WID_Pistol_AutoHeavySuppressed_Athena_R_Ore_T03.WID_Pistol_AutoHeavySuppressed_Athena_R_Ore_T03")].push_back(1);
    Loadout5.ItemDefinitions[StaticLoadObject<UFortItemDefinition>("/Game/Athena/Items/Weapons/WID_Pistol_AutoHeavySuppressed_Athena_R_Ore_T03.WID_Pistol_AutoHeavySuppressed_Athena_R_Ore_T03")].push_back(30);
    Loadout5.ItemDefinitions[StaticLoadObject<UFortItemDefinition>("/Game/Athena/Items/Weapons/WID_Pistol_AutoHeavySuppressed_Athena_R_Ore_T03.WID_Pistol_AutoHeavySuppressed_Athena_R_Ore_T03")].push_back(120);
    Loadout5.ItemDefinitions[StaticLoadObject<UFortItemDefinition>("/Game/Athena/Items/Consumables/Shields/Athena_Shields.Athena_Shields")].push_back(3);
    Loadout5.ItemDefinitions[StaticLoadObject<UFortItemDefinition>("/Game/Athena/Items/Consumables/Shields/Athena_Shields.Athena_Shields")].push_back(0);
    Loadout5.ItemDefinitions[SlurpFishDef].push_back(3);
    Loadout5.ItemDefinitions[SlurpFishDef].push_back(0);
    Loadout5.LoadoutType = ELoadoutType::LateGame;
    Loadouts.push_back(Loadout5);

    auto Loadout6 = Loadout{};
    Loadout6.ItemDefinitions.clear();
    Loadout6.ItemDefinitions[StaticLoadObject<UFortItemDefinition>("/Game/Athena/Items/Weapons/WID_Assault_AutoHigh_Athena_SR_Ore_T03.WID_Assault_AutoHigh_Athena_SR_Ore_T03")].push_back(1);
    Loadout6.ItemDefinitions[StaticLoadObject<UFortItemDefinition>("/Game/Athena/Items/Weapons/WID_Assault_AutoHigh_Athena_SR_Ore_T03.WID_Assault_AutoHigh_Athena_SR_Ore_T03")].push_back(30);
    Loadout6.ItemDefinitions[StaticLoadObject<UFortItemDefinition>("/Game/Athena/Items/Weapons/WID_Assault_AutoHigh_Athena_SR_Ore_T03.WID_Assault_AutoHigh_Athena_SR_Ore_T03")].push_back(60);  Loadout6.ItemDefinitions[StaticLoadObject<UFortItemDefinition>("/Game/Athena/Items/Weapons/WID_Shotgun_SemiAuto_Athena_VR_Ore_T03.WID_Shotgun_SemiAuto_Athena_VR_Ore_T03")].push_back(1);
    Loadout6.ItemDefinitions[StaticLoadObject<UFortItemDefinition>("/Game/Athena/Items/Weapons/WID_Shotgun_SemiAuto_Athena_VR_Ore_T03.WID_Shotgun_SemiAuto_Athena_VR_Ore_T03")].push_back(8);
    Loadout6.ItemDefinitions[StaticLoadObject<UFortItemDefinition>("/Game/Athena/Items/Weapons/WID_Shotgun_SemiAuto_Athena_VR_Ore_T03.WID_Shotgun_SemiAuto_Athena_VR_Ore_T03")].push_back(15);
    Loadout6.ItemDefinitions[StaticLoadObject<UFortItemDefinition>("/Game/Athena/Items/Weapons/Boss/WID_Boss_Hos_MG.WID_Boss_Hos_MG")].push_back(1);
    Loadout6.ItemDefinitions[StaticLoadObject<UFortItemDefinition>("/Game/Athena/Items/Weapons/Boss/WID_Boss_Hos_MG.WID_Boss_Hos_MG")].push_back(0);
    Loadout6.ItemDefinitions[StaticLoadObject<UFortItemDefinition>("/Game/Athena/Items/Weapons/Boss/WID_Boss_Hos_MG.WID_Boss_Hos_MG")].push_back(120);
    Loadout6.ItemDefinitions[StaticLoadObject<UFortItemDefinition>("/Game/Athena/Items/Consumables/Shields/Athena_Shields.Athena_Shields")].push_back(3);
    Loadout6.ItemDefinitions[StaticLoadObject<UFortItemDefinition>("/Game/Athena/Items/Consumables/Shields/Athena_Shields.Athena_Shields")].push_back(0);
    Loadout6.ItemDefinitions[FlopperDef].push_back(3);
    Loadout6.ItemDefinitions[FlopperDef].push_back(0);
    Loadout6.LoadoutType = ELoadoutType::LateGame;
    Loadouts.push_back(Loadout6);
    
#ifdef PEVENT
    MH_CreateHook(PEAddr, ProcessEventHook, (void**)&ProcessEventOG);
    MH_EnableHook(PEAddr);
#endif
    
    MH_CreateHook(ContextAddr, CanCreateContext, nullptr);
    MH_EnableHook(ContextAddr);

    MH_CreateHook(UnlockPerkForPlayerAddr, UnlockPerkForPlayer, (void**)&UnlockPerkForPlayerOG);
    MH_EnableHook(UnlockPerkForPlayerAddr);

    MH_CreateHook(OnPerceptionSensedAddr, OnPerceptionSensed, (void**)&OnPerceptionSensedOG);
    MH_EnableHook(OnPerceptionSensedAddr);

    MH_CreateHook(OnDamageServerAddr, OnDamageServer, (void**)&OnDamageServerOG);
    MH_EnableHook(OnDamageServerAddr);

    MH_CreateHook(OnPossessedPawnDiedAddr, OnPossessedPawnDied, (void**)&OnPossessedPawnDiedOG);
    MH_EnableHook(OnPossessedPawnDiedAddr);

    MH_CreateHook(SpawnPlayActorAddr, SpawnPlayActor, (void**)&SpawnPlayActorOG);
    MH_EnableHook(SpawnPlayActorAddr);

    MH_CreateHook(StartAircraftPhaseAddr, StartAircraftPhaseHook, (void**)&StartAircraftPhase);
    MH_EnableHook(StartAircraftPhaseAddr);

    MH_CreateHook(ServerAttemptInteractAddr, ServerAttemptInteract, (void**)&ServerAttemptInteractOG);
    MH_EnableHook(ServerAttemptInteractAddr);

    MH_CreateHook(SetMatchPlacementAddr, SetMatchPlacementHook, (void**)&SetMatchPlacement);
    MH_EnableHook(SetMatchPlacementAddr);

    MH_CreateHook(GameSessionRestartAddr, GameSessionRestart, nullptr);
    MH_EnableHook(GameSessionRestartAddr);

    MH_CreateHook(stormaddr, stormstuff, (void**)&stormstuffOG);
    MH_EnableHook(stormaddr);

    MH_CreateHook(OnCapsuleBeginOverlapAddr, OnCapsuleBeginOverlap, (void**)&OnCapsuleBeginOverlapOG);
    MH_EnableHook(OnCapsuleBeginOverlapAddr);

    MH_CreateHook(TickFlushAddr, TickFlushHook, (LPVOID*)&TickFlushOG);
    MH_EnableHook(TickFlushAddr);

    MH_CreateHook(PreloginAddr, Prelogin, (LPVOID*)&PreloginOG);
    MH_EnableHook(PreloginAddr);
    
    MH_CreateHook(ReloadAddr, OnReload, (LPVOID*)&OnReloadOG);
    MH_EnableHook(ReloadAddr);

    MH_CreateHook(Test3Addr, test3, (LPVOID*)&test3OG);
    MH_EnableHook(Test3Addr);

    MH_CreateHook(OnAircraftExitDropZoneAddr, OnAircraftExitedDropZone, (LPVOID*)&OnAircraftExitedDropZoneOG);
    MH_EnableHook(OnAircraftExitDropZoneAddr);

    MH_CreateHook(ClientOnPawnDiedAddr, ClientOnPawnDied, (LPVOID*)&ClientOnPawnDiedOG);
    MH_EnableHook(ClientOnPawnDiedAddr);
    
    MH_CreateHook(SpawnLootAddr, SpawnLoot, nullptr);
    MH_EnableHook(SpawnLootAddr);

    MH_CreateHook(SpawnBotAddr, SpawnBot, (void**)&SpawnBotOG);
    MH_EnableHook(SpawnBotAddr);

    MH_CreateHook(QueryPatchAddr, MCP_DispatchRequestHook, (LPVOID*)&MCP_DispatchRequest);
    MH_EnableHook(QueryPatchAddr);

    MH_CreateHook(ServerReadyToStartMatchAddr, ServerReadyToStartMatch, (LPVOID*)&ServerReadyToStartMatchOG);
    MH_EnableHook(ServerReadyToStartMatchAddr);

    MH_CreateHook(ServerLoadingScreenDroppedAddr, ServerLoadingScreenDropped, (LPVOID*)&ServerLoadingScreenDroppedOG);
    MH_EnableHook(ServerLoadingScreenDroppedAddr);

    MH_CreateHook(ReadyToStartMatchAddr, ReadyToStartMatch, (LPVOID*)&ReadyToStartMatchOG);
    MH_EnableHook(ReadyToStartMatchAddr);

    MH_CreateHook(SpawnDefaultPawnForAddr, SpawnDefaultPawnFor, nullptr);
    MH_EnableHook(SpawnDefaultPawnForAddr);
    
    cout << BaseAddr << endl;
    cout << sizeof(TWeakObjectPtr<AFortPlayerStateAthena>) << endl;//omg

    //return 0;

    SwapVTable(AAthena_PlayerController_C::StaticClass()->DefaultObject, 0x20D, ServerExecuteInventoryItem);
    SwapVTable(AAthena_PlayerController_C::StaticClass()->DefaultObject, 0x10D, ServerAcknowledgePossession, (LPVOID*)&ServerAcknowledgePossessionOG);
    SwapVTable(AAthena_PlayerController_C::StaticClass()->DefaultObject, 0x230, ServerCreateBuildingActor, (LPVOID*)&ServerCreateBuildingActorOG);
    SwapVTable(AAthena_PlayerController_C::StaticClass()->DefaultObject, 0x237, ServerBeginEditBuildingActor);
    SwapVTable(AAthena_PlayerController_C::StaticClass()->DefaultObject, 0x232, ServerEditBuildingActor);
    SwapVTable(AAthena_PlayerController_C::StaticClass()->DefaultObject, 0x235, ServerEndEditingBuildingActor);
    SwapVTable(AAthena_PlayerController_C::StaticClass()->DefaultObject, 0x21D, ServerAttemptInventoryDrop);
    SwapVTable(AAthena_PlayerController_C::StaticClass()->DefaultObject, 0x1C7, ServerPlayEmoteItem);
    SwapVTable(AAthena_PlayerController_C::StaticClass()->DefaultObject, 0x265, ServerReturnToMainMenu);
    SwapVTable(AAthena_PlayerController_C::StaticClass()->DefaultObject, 0x4F0, ServerClientIsReadyToRespawn, (void**)&ServerClientIsReadyToRespawnOG);

    SwapVTable(AFortPlayerStateAthena::StaticClass()->DefaultObject, 0xFF, ServerSetInAircraft, (void**)&ServerSetInAircraftOG);
    SwapVTable(APlayerPawn_Athena_C::StaticClass()->DefaultObject, 0x119, NetMulticast_Athena_BatchedDamageCues, (LPVOID*)&NetMulticast_Athena_BatchedDamageCuesOG);
    SwapVTable(APlayerPawn_Athena_C::StaticClass()->DefaultObject, 0x1F5, ServerSendZiplineState);
    SwapVTable(APlayerPawn_Athena_C::StaticClass()->DefaultObject, 0x1D2, ServerReviveFromDBNO);
    SwapVTable(UFortControllerComponent_Aircraft::StaticClass()->DefaultObject, 0x89, ServerAttemptAircraftJump);

    MH_CreateHook(KickPlayerAddr, KickPlayerHook, nullptr);
    MH_EnableHook(KickPlayerAddr);

    MH_CreateHook(PickTeamAddr, PickTeam, nullptr);
    MH_EnableHook(PickTeamAddr);

    MH_CreateHook(WorldNetModeAddr, WorldNetMode, nullptr);
    MH_EnableHook(WorldNetModeAddr);
    
    MH_CreateHook(CollectGarbageAddr, CollectGarbage, nullptr);
    MH_EnableHook(CollectGarbageAddr);

    MH_CreateHook(GetMaxTickRateAddrOK, GetMaxTickRate, nullptr);
    MH_EnableHook(GetMaxTickRateAddrOK);

    MH_CreateHook(ActorNetModeCrashAddr, GameSessionIdThing, nullptr);
    MH_EnableHook(ActorNetModeCrashAddr);
    
    MH_CreateHook(ActorNetModeAddr, ActorNetMode, nullptr);
    MH_EnableHook(ActorNetModeAddr);

    MH_CreateHook(GetPlayerViewpointAddr, GetPlayerViewPoint, (void**)&GetPlayerViewPointOG);
    MH_EnableHook(GetPlayerViewpointAddr);
    
    UFortAbilitySystemComponentAthena* DefObj = (UFortAbilitySystemComponentAthena*)UFortAbilitySystemComponentAthena::StaticClass()->DefaultObject;
    UFortAbilitySystemComponent* DefObj2 = (UFortAbilitySystemComponent*)UFortAbilitySystemComponent::StaticClass()->DefaultObject;
    UAbilitySystemComponent* DefObj3 = (UAbilitySystemComponent*)UAbilitySystemComponent::StaticClass()->DefaultObject;

    SwapVTable(DefObj, 0xFA, InternalServerTryActivateAbilityHook);
    SwapVTable(DefObj2, 0xFA, InternalServerTryActivateAbilityHook);
    SwapVTable(DefObj3, 0xFA, InternalServerTryActivateAbilityHook);
    
    ((UGameplayStatics*)UGameplayStatics::StaticClass()->DefaultObject)->OpenLevel(GetWorld(), ((UKismetStringLibrary*)UKismetStringLibrary::StaticClass()->DefaultObject)->Conv_StringToName(TEXT("Apollo_Terrain")), false, FString());

    GetEngine()->GameInstance->LocalPlayers.FreeArray();
    Ready = true;

    return 0;
}

BOOL APIENTRY DllMain(HMODULE hModule,
    DWORD  ul_reason_for_call,
    LPVOID lpReserved
)
{
    switch (ul_reason_for_call)
    {
    case DLL_PROCESS_ATTACH:
        CreateThread(0, 0, InitThread, 0, 0, 0);
        break;
    case DLL_PROCESS_DETACH:
        Log("Crash or close!");
        EonApi::RemoveSession(Region, SessionName);
        ((UKismetSystemLibrary*)UKismetSystemLibrary::StaticClass()->DefaultObject)->ExecuteConsoleCommand(GetWorld(), TEXT("demostop"), nullptr);
        break;
    default:
        break;
    }
    return TRUE;
}
