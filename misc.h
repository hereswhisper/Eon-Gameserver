#pragma once
#include "ue.h"
#include "Bots.h"
#include "Looting.h"
#include <intrin.h>
#include "Quests.h"

void (*TickFlushOG)(UNetDriver* Driver, float a2);
void TickFlushHook(UNetDriver* Driver, float a2)
{
    static bool Done = false;
    static bool Done2 = false;
    if (!Driver)
        return;

    if (!Done)
    {
        if (GetGameState()->WarmupCountdownEndTime - GetStatics()->GetTimeSeconds(GetWorld()) <= 10)
        {
            Done = true;

            EonApi::LockGameSession(Region, SessionName);

            if (!bDontNotifyMMS || !bStaffGameOnly) {
                EonMMSAPI::SetServerStatus("offline");
            }
            
        }
    }

    if (BotsEnabled)
    {
        if (!Done2)
        {
            if (GetGameState()->WarmupCountdownEndTime - GetStatics()->GetTimeSeconds(GetWorld()) <= 0)
            {
                Done2 = true;

                StartAircraftPhase(GetGameMode(), 0);
            }
        }

        TickBots();
    }

    if (Driver && Driver->ReplicationDriver && Driver->ClientConnections.Num() > 0 && !Driver->ClientConnections[0]->InternalAck)
        ServerReplicateActors(Driver->ReplicationDriver);

    return TickFlushOG(Driver, a2);
}

char __fastcall KickPlayerHook(__int64, __int64, __int64)
{
    return 0;
}

__int64 WorldNetMode(UWorld* World)
{
    return 1;
}

__int64(__fastcall* MCP_DispatchRequest)(void* McpProfile, void* Context, int a3);
__int64 __fastcall MCP_DispatchRequestHook(void* McpProfile, void* Context, int a3)
{
    return MCP_DispatchRequest(McpProfile, Context, bMcp ? 3 : a3);
}
void __fastcall CollectGarbage(unsigned int a1, unsigned __int8 a2)
{
    return;
}

Loadout& GetRandomLoadout()
{
    static int Wow = 0;
    static bool Once = false;
    if (!Once)
    {
        Once = true;
        Wow = GetMath()->RandomIntegerInRange(0, Loadouts.size() - 1);
    }
    Loadout& loadout = Loadouts[Wow];
    Wow++;
    if (Wow == Loadouts.size())
        Wow = 0;
    return loadout;
}

void SwapVTable(void* base, int Idx, void* Detour, void** OG = nullptr)
{
    if (!base)
        return;

    void** VTable = *(void***)base;
    if (!VTable || !VTable[Idx])
        return;
    if (OG)
    {
        *OG = VTable[Idx];
    }

    DWORD oldProtection;

    VirtualProtect(&VTable[Idx], sizeof(void*), PAGE_EXECUTE_READWRITE, &oldProtection);

    VTable[Idx] = Detour;

    VirtualProtect(&VTable[Idx], sizeof(void*), oldProtection, NULL);
}

float GetMaxTickRate(UGameEngine* Engine, float a2, bool a3)
{
    return 30.f;
}

BYTE* __fastcall GameSessionIdThing(__int64, __int64)
{
    return nullptr;
}

__int64 ActorNetMode(AActor* Actor)
{
    return 1;
}

EFortTeam PickTeam(AFortGameModeAthena* Gamemode, uint8 PreferredTeam, AFortPlayerControllerAthena* PC)
{
    if (PC->IsA(AFortLiveBroadcastController::StaticClass()))
        return EFortTeam::Spectator;

    uint8 RetVal = NextIdx;

    if (!bIsLargeTeamGame)
    {
        CurrentPlayersOnTeam++;

        if (CurrentPlayersOnTeam >= MaxPlayersPerTeam)
        {
            cout << "MAX\n";
            CurrentPlayersOnTeam = 0;
            NextIdx++;
        }
    }
    else
    {
        RetVal = NextIdx;
        NextIdx++;
        cout << "NextIdx " << to_string(NextIdx) << endl;
        if (NextIdx > FirstTeam + 1)
        {
            Log("NextIdx > FirstTeam + 1");
            NextIdx = FirstTeam;
        }
    }

    cout << "returning " << to_string(RetVal) << endl;
    return EFortTeam(RetVal);
}

FName SpecName;
void (*GetPlayerViewPointOG)(AFortPlayerController* PC, FVector& out_Location, FRotator& out_Rotation);
void GetPlayerViewPoint(AFortPlayerController* PC, FVector& out_Location, FRotator& out_Rotation)
{
    if (PC->IsA(AFortLiveBroadcastController::StaticClass()))
        return GetPlayerViewPointOG(PC, out_Location, out_Rotation);

    if (PC->StateName.ComparisonIndex == SpecName.ComparisonIndex)
    {
        out_Location = PC->LastSpectatorSyncLocation;
        out_Rotation = PC->LastSpectatorSyncRotation;
    }
    else if (PC->GetViewTarget())
    {
        out_Location = PC->GetViewTarget()->K2_GetActorLocation();
        out_Rotation = PC->GetControlRotation();
    }
    else
    {
        return GetPlayerViewPointOG(PC, out_Location, out_Rotation);
    }
}

__int64 __fastcall context(__int64 a1, __int64 a2, __int64* a3, __int64 a4)
{
    return 1;
}

void TESTETSETSTS()
{
    return;
}

__int64(__fastcall* test3OG)(AActor* a1, __int64 a2);
__int64 __fastcall test3(AActor* a1, __int64 a2)
{
    AFortInventory* NewInv = SpawnActor<AFortInventory>({});
    NewInv->SetOwner(a1);

    *(unsigned long long*)(__int64(a1) + 1352) = __int64(NewInv);
    return test3OG(a1, a2);
}

__int64 CanCreateContext(__int64 a1)
{
    return 1;
}

int TimesCalled = 0;

int GetStormDuration()
{
    int Ret = 30;

    if (bStormKing)
    {
        switch (TimesCalled)
        {
        case 0:
            Ret = 30;
            break;

        case 1:
            Ret = 180;
            break;

        case 2:
            Ret = 300;
            break;

        case 3:
            Ret = 360;
            break;

        default:
            break;
        }
    }
    else
    {
        switch (TimesCalled)
        {
        case 0:
            Ret = 0.f;
            break;

        case 1:
            Ret = 60;
            break;

        case 2:
            Ret = 30;
            break;

        case 3:
            Ret = 15;
            break;

        default:
            break;
        }
    }

    return Ret;
}

float GetZoneStartDuration()
{
    if (bLateGame)
    {
        switch (TimesCalled)
        {
        case 0:
        {
            return 0.f;
        }

        case 1:
        {
            return 60;
        }

        case 2:
            return 60;

        case 3:
            return 60;

        default:
            return 0.f;
        }
    }
}

void (__fastcall* stormstuffOG)(__int64 a1, int a2);
void __fastcall stormstuff(__int64 a1, int a2)
{
    if (bTravis && !StartedEvent)
        return;

    stormstuffOG(a1, a2);

    if (bLateGame)
    {
        GetGameState()->SafeZoneIndicator->NextCenter = (FVector_NetQuantize100)BusLocation;
        GetGameState()->SafeZoneIndicator->NextNextCenter = (FVector_NetQuantize100)BusLocation;
        
        switch (TimesCalled)
        {
        case 0:
        {
            GetGameState()->SafeZoneIndicator->NextRadius = 10000;
            break;
        }

        case 1:
        {
            GetGameState()->SafeZoneIndicator->NextRadius = 7500;
            break;
        }

        case 2:
            GetGameState()->SafeZoneIndicator->NextRadius = 5000;
            break;

        case 3:
            GetGameState()->SafeZoneIndicator->NextRadius = 3500;
            break;

        default:
            GetGameState()->SafeZoneIndicator->NextRadius = 0;
            break;
        }

        FVector Center = GetGameState()->SafeZoneIndicator->GetSafeZoneCenter();
        FVector NextCenter = GetGameState()->SafeZoneIndicator->GetSafeZoneCenter();

        if (TimesCalled)
        {
            int Amount = GetMath()->RandomIntegerInRange(-25000, 25000);
            NextCenter.X += Amount;
            NextCenter.Y += Amount;
            GetGameState()->SafeZoneIndicator->NextCenter = (FVector_NetQuantize100)NextCenter;
        }

        int StormDuration = GetStormDuration();
        float Distance = GetMath()->Vector_Distance2D(Center, NextCenter);

        if (Distance >= 10000)
            StormDuration += 85;

        cout << "Distance: " << Distance << endl;

        GetGameState()->SafeZoneIndicator->SafeZoneStartShrinkTime = GetStatics()->GetTimeSeconds(GetWorld()) + GetZoneStartDuration();
        GetGameState()->SafeZoneIndicator->SafeZoneFinishShrinkTime = GetGameState()->SafeZoneIndicator->SafeZoneStartShrinkTime + StormDuration;
        GetGameState()->SafeZonePhase = 5;
        GetGameMode()->SafeZonePhase = 5;
        GetGameState()->OnRep_SafeZonePhase();
        TimesCalled++;
    }

    if (bStormKing && GetGameState()->GamePhase > EAthenaGamePhase::Aircraft)
    {
        AFortAthenaMutator_DadBro* Mutator = (AFortAthenaMutator_DadBro*)GetGameState()->GetMutatorByClass(GetGameMode(), AFortAthenaMutator_DadBro::StaticClass());
        GetGameState()->SafeZoneIndicator->NextCenter = (FVector_NetQuantize100)Mutator->DadBroSpawnLocation;
        GetGameState()->SafeZoneIndicator->NextNextCenter = (FVector_NetQuantize100)Mutator->DadBroSpawnLocation;
        
        switch (TimesCalled)
        {
        case 0:
            GetGameState()->SafeZoneIndicator->NextRadius = 10000;
            break;

        case 1:
            GetGameState()->SafeZoneIndicator->NextRadius = 7500;
            break;

        case 2:
            GetGameState()->SafeZoneIndicator->NextRadius = 5000;
            break;

        case 3:
            GetGameState()->SafeZoneIndicator->NextRadius = 4750;
            break;

        default:
            GetGameState()->SafeZoneIndicator->NextRadius = 4350;
            break;
        }

        GetGameState()->SafeZoneIndicator->SafeZoneStartShrinkTime = GetStatics()->GetTimeSeconds(GetWorld());
        GetGameState()->SafeZoneIndicator->SafeZoneFinishShrinkTime = GetStatics()->GetTimeSeconds(GetWorld()) + GetStormDuration();
        GetGameState()->SafeZonePhase = 5;
        GetGameMode()->SafeZonePhase = 5;
        GetGameState()->OnRep_SafeZonePhase();
        TimesCalled++;
    }
}

void GameSessionRestart(AFortGameSessionDedicated* Session)
{
    EonApi::RemoveSession(Region, SessionName);
    ((UKismetSystemLibrary*)UKismetSystemLibrary::StaticClass()->DefaultObject)->ExecuteConsoleCommand(GetWorld(), TEXT("demostop"), nullptr);
    exit(0);
}

__int64 uelog(__int64, unsigned int, __int64, unsigned __int8, wchar_t* Format, ...)
{
    printf("%ls\n", Format);
    return 0;
}

void InitXpComp(UFortPlayerControllerAthenaXPComponent* XPComponent, AFortPlayerStateAthena* PlayerState)
{
    XPComponent->bRegisteredWithQuestManager = true;
    XPComponent->OnRep_bRegisteredWithQuestManager();
    *(int32*)(__int64(PlayerState) + 0xE78) = XPComponent->CurrentLevel;
    PlayerState->OnRep_SeasonLevelUIDisplay();

    // the function is called OnProfileReady but magma don't we need to call the ORigianl First??
}

//literally the most 1:1 thing ever
APlayerController* (*SpawnPlayActorOG)(UWorld* World, UPlayer* Player, ENetRole RemoteRole, FURL& InURL, void* UniqueID, FString& Error, uint8 InNetPlayerIndex);
APlayerController* SpawnPlayActor(UWorld* World, UPlayer* Player, ENetRole RemoteRole, FURL& InURL, void* UniqueID, FString& Error, uint8 InNetPlayerIndex)
{
    FString& JoinURL = *(FString*)(__int64(Player) + 0x1B8);
    if (JoinURL.ToString().contains("livespectator"))
    {
        GetGameMode()->PlayerControllerClass = GetGameMode()->LiveBroadcastPlayerControllerClass.Get();
    }
    else
    {
        GetGameMode()->PlayerControllerClass = AAthena_PlayerController_C::StaticClass();
    }
    auto Ret = SpawnPlayActorOG(World, Player, RemoteRole, InURL, UniqueID, Error, InNetPlayerIndex);

    if (GetGameMode()->PlayerControllerClass == AAthena_PlayerController_C::StaticClass() && GetGameState()->GamePhase >= EAthenaGamePhase::Aircraft)
    {
        Ret->ClientReturnToMainMenu(TEXT(""));
        return Ret;
    }

    return Ret;
}

__int64 (*OnExplodedOG)(AB_Prj_Athena_Consumable_Thrown_C* a1, unsigned __int64* a2, char* a3);
__int64 OnExploded(AB_Prj_Athena_Consumable_Thrown_C* a1, unsigned __int64* a2, char* a3)
{
    if (!a1)
        return 0;
    auto Def = *(UFortItemDefinition**)(__int64(a1) + 0x888);
    if (!Def)
        return 0;
    SpawnPickup(a1->K2_GetActorLocation(), Def, 1, 0, EFortPickupSourceTypeFlag::Tossed, EFortPickupSpawnSource::Unset, nullptr, false);
    a1->K2_DestroyActor();
    return 0;
}

string SplitString(bool SecondString, string delim, string strtosplit)
{
    auto start = 0U;
    auto end = strtosplit.find(delim);
    if (SecondString)
    {
        while (end != std::string::npos)
        {
            start = end + delim.length();
            end = strtosplit.find(delim, start);
        }
    }

    return strtosplit.substr(start, end);
}

void (*ServerAttemptInteractOG)(UFortControllerComponent_Interaction* Comp, AActor* ReceivingActor, UPrimitiveComponent* InteractionComponent, ETInteractionType InteractType, UObject* OptionalData, EInteractionBeingAttempted InteractionBeingAttempted, int32 RequestId);
void ServerAttemptInteract(UFortControllerComponent_Interaction* Comp, AActor* ReceivingActor, UPrimitiveComponent* InteractionComponent, ETInteractionType InteractType, UObject* OptionalData, EInteractionBeingAttempted InteractionBeingAttempted, int32 RequestId)
{
    cout << __int64(_ReturnAddress()) - __int64(GetModuleHandleW(0)) << endl;
    cout << __int64((*(void***)ReceivingActor)) - __int64(GetModuleHandleW(0)) << endl;
    cout << __int64((*(void***)Comp)) - __int64(GetModuleHandleW(0)) << endl;
    cout << OptionalData->GetName() << endl;
    cout << ReceivingActor->GetFullName() << endl;
    cout << ReceivingActor->Class->GetFullName() << endl;

    static class UClass* Clss = UObject::FindClassFast("BGA_Athena_Lock_Parent_C");
    static UClass* AthenaQuestBGAClass = StaticLoadObject<UClass>("/Game/Athena/Items/QuestInteractablesV2/Parents/AthenaQuest_BGA.AthenaQuest_BGA_C");
    ServerAttemptInteractOG(Comp, ReceivingActor, InteractionComponent, InteractType, OptionalData, InteractionBeingAttempted, RequestId);
    AFortPlayerControllerAthena* PC = (AFortPlayerControllerAthena*)Comp->GetOwner();

    if (ReceivingActor->IsA(Clss))
    {
        UFortItemDefinition* Def = *(UFortItemDefinition**)(__int64(ReceivingActor) + 0x8F0);
        Remove(PC, Def);
    }
    else if (ReceivingActor->IsA(AthenaQuestBGAClass))
    {
        Log("Quest!");
        ReceivingActor->ProcessEvent(ReceivingActor->Class->GetFunction("AthenaQuest_BGA_C", "BindToQuestManagerForQuestUpdate"), &PC);
        
        static auto QuestsRequiredOnProfileOffset = GetOffset(ReceivingActor, "QuestsRequiredOnProfile");
        static auto Primary_BackendNameOffset = GetOffset(ReceivingActor, "Primary_BackendName");
        TArray<UFortQuestItemDefinition*>& QuestsRequiredOnProfile = *(TArray<UFortQuestItemDefinition*>*)(__int64(ReceivingActor) + QuestsRequiredOnProfileOffset);
        FName& Primary_BackendName = *(FName*)(__int64(ReceivingActor) + Primary_BackendNameOffset);
        
        ProgressQuest(PC, QuestsRequiredOnProfile[0], Primary_BackendName);
    }
    else if (ReceivingActor->GetName().contains("QuestInteractable"))
    {
        Log("Old quest so bad code wjasfhuaeguj");

        static auto QuestInteractable_GEN_VARIABLEOffset = GetOffset(ReceivingActor, "QuestInteractable");
        static auto PCsOnQuestOffset = GetOffset(ReceivingActor, "PCsOnQuest");
        static auto PCsThatCompletedQuest_ServerOffset = GetOffset(ReceivingActor, "PCsThatCompletedQuest_Server");
        UQuestInteractableComponent* QuestComp = *(UQuestInteractableComponent**)(__int64(ReceivingActor) + QuestInteractable_GEN_VARIABLEOffset);
        TArray<AFortPlayerControllerAthena*>& PCsOnQuest = *(TArray<AFortPlayerControllerAthena*>*)(__int64(ReceivingActor) + PCsThatCompletedQuest_ServerOffset);
        TArray<AFortPlayerControllerAthena*>& PCsThatCompletedQuest_Server = *(TArray<AFortPlayerControllerAthena*>*)(__int64(ReceivingActor) + PCsThatCompletedQuest_ServerOffset);
        QuestComp->bReady = true;
        QuestComp->OnRep_Ready();
        auto QuestManager = PC->GetQuestManager(ESubGame::Athena);
        
        PCsOnQuest.Add(PC);
        PCsThatCompletedQuest_Server.Add(PC);
        QuestComp->OnPlaylistDataReady(GetGameState(), GetGameState()->CurrentPlaylistInfo.BasePlaylist, *(FGameplayTagContainer*)(__int64(GetGameState()->CurrentPlaylistInfo.BasePlaylist) + GetOffset(GetGameState()->CurrentPlaylistInfo.BasePlaylist, "GameplayTagContainer")));
        
        ProgressQuest(PC, QuestComp->QuestItemDefinition, QuestComp->ObjectiveBackendName);

        QuestComp->OnCalendarUpdated();
    }
    else if (ReceivingActor->IsA(AFortAthenaSupplyDrop::StaticClass()))
    {
        Log("Supply Drop");
        ItemRow* Consumable = GetRandomItem("Airdrop", EFortItemType::Consumable);
        ItemRow* Weapon = GetRandomItem("Airdrop", EFortItemType::WeaponRanged);

        static auto WoodDef = StaticLoadObject<UFortItemDefinition>("/Game/Items/ResourcePickups/WoodItemData.WoodItemData");
        static auto StoneDef = StaticLoadObject<UFortItemDefinition>("/Game/Items/ResourcePickups/StoneItemData.StoneItemData");
        static auto MetalDef = StaticLoadObject<UFortItemDefinition>("/Game/Items/ResourcePickups/MetalItemData.MetalItemData");

        SpawnPickup(ReceivingActor->K2_GetActorLocation(), Consumable->Def, Consumable->DropCount, Consumable->LoadedAmmo, EFortPickupSourceTypeFlag::Container, EFortPickupSpawnSource::Unset);
        SpawnPickup(ReceivingActor->K2_GetActorLocation(), Weapon->Def, Weapon->DropCount, Weapon->LoadedAmmo, EFortPickupSourceTypeFlag::Container, EFortPickupSpawnSource::Unset);
        SpawnPickup(ReceivingActor->K2_GetActorLocation(), WoodDef, 30, 0, EFortPickupSourceTypeFlag::Container, EFortPickupSpawnSource::Unset);
        SpawnPickup(ReceivingActor->K2_GetActorLocation(), StoneDef, 30, 0, EFortPickupSourceTypeFlag::Container, EFortPickupSpawnSource::Unset);
        SpawnPickup(ReceivingActor->K2_GetActorLocation(), MetalDef, 30, 0, EFortPickupSourceTypeFlag::Container, EFortPickupSpawnSource::Unset);
    }
    //else if (ReceivingActor->IsA(AFortAthenaVehicle::StaticClass()))
    //{
    //    if (PC->MyFortPawn->IsInVehicle())
    //    {
    //        auto Vehicle = PC->MyFortPawn->GetVehicle();
    //        if (Vehicle)
    //        {
    //            int SeatIdx = PC->MyFortPawn->GetVehicleSeatIndex();
    //            auto WeaponComp = Vehicle->GetSeatWeaponComponent(SeatIdx);
    //            if (WeaponComp)
    //            {
    //                if (WeaponComp->WeaponSeatDefinitions.IsValidIndex(SeatIdx))
    //                {
    //                    auto& Def = WeaponComp->WeaponSeatDefinitions[SeatIdx];
    //                    if (Def.VehicleWeapon && Def.SeatIndex == SeatIdx)
    //                    {
    //                        GiveItem(PC, Def.VehicleWeapon, 1, 99999);
    //                        FFortItemEntry* Entry = FindEntry(PC, Def.VehicleWeapon); // BAD
    //                        if (Entry)
    //                        {
    //                            //PC->ServerExecuteInventoryItem(Entry->ItemGuid); // ClientEquipItem or else crash thanks
// 
    //                        }
    //                    }
    //                }
    //            }
    //        }
    //    }
    //}
}

void (*OnDamageServerOG)(ABuildingSMActor*, float, FGameplayTagContainer, FVector, FHitResult, AFortPlayerControllerAthena*, AActor*, FGameplayEffectContextHandle);
void OnDamageServer(ABuildingSMActor* Actor, float Damage, FGameplayTagContainer DamageTags, FVector Momentum, FHitResult HitInfo, AFortPlayerControllerAthena* InstigatedBy, AActor* DamageCauser, FGameplayEffectContextHandle EffectContext)
{
    bool FoundTag = false;
    if (!InstigatedBy || Actor->bPlayerPlaced || Actor->GetHealth() == 1)
        return OnDamageServerOG(Actor, Damage, DamageTags, Momentum, HitInfo, InstigatedBy, DamageCauser, EffectContext);

    for (size_t i = 0; i < DamageTags.GameplayTags.Num(); i++)
    {
        if (DamageTags.GameplayTags[i].TagName.ComparisonIndex == PickaxeTagName.ComparisonIndex)
        {
            FoundTag = true;
            break;
        }
    }

    if (!FoundTag)
        return OnDamageServerOG(Actor, Damage, DamageTags, Momentum, HitInfo, InstigatedBy, DamageCauser, EffectContext);

    auto ResourceItemDef = GetFortKismet()->K2_GetResourceItemDefinition(Actor->ResourceType);

    if (!ResourceItemDef)
        return OnDamageServerOG(Actor, Damage, DamageTags, Momentum, HitInfo, InstigatedBy, DamageCauser, EffectContext);
    bool Found = false;

    bool Weakspot = Damage == 100.f;
    int ResCount = GetMath()->RandomIntegerInRange(3, 5);

    if (Weakspot)
    {
        ResCount = GetMath()->RandomIntegerInRange(7, 12);
    }

    for (size_t i = 0; i < InstigatedBy->WorldInventory->Inventory.ReplicatedEntries.Num(); i++)
    {
        if (InstigatedBy->WorldInventory->Inventory.ReplicatedEntries[i].ItemDefinition == ResourceItemDef)
        {
            Found = true;
            InstigatedBy->WorldInventory->Inventory.ReplicatedEntries[i].Count += ResCount;
            if (InstigatedBy->WorldInventory->Inventory.ReplicatedEntries[i].Count > 999)
            {
                SpawnPickup(InstigatedBy->Pawn->K2_GetActorLocation(), InstigatedBy->WorldInventory->Inventory.ReplicatedEntries[i].ItemDefinition, InstigatedBy->WorldInventory->Inventory.ReplicatedEntries[i].Count - 999, 0, EFortPickupSourceTypeFlag::Tossed, EFortPickupSpawnSource::Unset, InstigatedBy->MyFortPawn);
                InstigatedBy->WorldInventory->Inventory.ReplicatedEntries[i].Count = 999;
            }
            ModifyEntry(InstigatedBy, InstigatedBy->WorldInventory->Inventory.ReplicatedEntries[i]);
            UpdateInventory(InstigatedBy, &InstigatedBy->WorldInventory->Inventory.ReplicatedEntries[i]);
            break;
        }
    }

    if (!Found)
        GiveItem(InstigatedBy, ResourceItemDef, ResCount);

    InstigatedBy->ClientReportDamagedResourceBuilding(Actor, Actor->ResourceType, ResCount, false, Weakspot);

    return OnDamageServerOG(Actor, Damage, DamageTags, Momentum, HitInfo, InstigatedBy, DamageCauser, EffectContext);
}

//imagine skidding pseudocode
__int64 GetAccID(__int64 Smth, __int64 a2)
{
    static  __int64 (*sub_7FF7C019DC30)(__int64, __int64) = decltype(sub_7FF7C019DC30)(__int64(GetModuleHandleW(0)) + 0x2C2DC30);
    __int64 v4 = 0; // rcx
    __int64 v5 = 0; // r14
    __int64 v6 = 0; // rax
    wchar_t* v7 = 0; // rbx
    wchar_t* v8 = 0; // rsi
    __int64 v9 = 0; // rax
    __int64 v10 = 0; // rax
    __int64* v11 = 0; // rcx
    char v12 = 0; // bl
    int v13 = 0; // eax
    __int64 v15 = 0; // [rsp+20h] [rbp-58h] BYREF
    __int64 v16 = 0; // [rsp+28h] [rbp-50h]
    __int64 v17[2]; // [rsp+30h] [rbp-48h] BYREF
    __int64 v18 = 0; // [rsp+40h] [rbp-38h] BYREF
    __int64 v19 = 0; // [rsp+50h] [rbp-28h] BYREF
    int v20 = 0; // [rsp+80h] [rbp+8h] BYREF

    v20 = 0;
    v4 = *(_QWORD*)(Smth + 8);
    if (v4 && (*(unsigned __int8(__fastcall**)(__int64))(*(_QWORD*)v4 + 40i64))(v4))
    {
        v5 = *(_QWORD*)(Smth + 8);
        v6 = (*(__int64(__fastcall**)(__int64, __int64*))(*(_QWORD*)v5 + 56i64))(v5, &v19);
        cout << __int64(*(_QWORD*)v5 + 56i64) - __int64(GetModuleHandleW(0)) << endl;
        wchar_t* TestChar = *(wchar_t**)(v5 + 24);
        printf("testchar %ls \n", TestChar);
        if (*(DWORD*)(v6 + 8))
            v8 = *(wchar_t**)v6;
        v9 = (*(__int64(__fastcall**)(__int64, int*))(*(_QWORD*)v5 + 16i64))(v5, &v20);
        v10 = sub_7FF7C019DC30(v9, (__int64)&v18);
        if (*(DWORD*)(v10 + 8))
            v7 = *(wchar_t**)v10;
        printf("test1 %ls\n", v8);
        printf("test2 %ls\n", v7);
    }

    *(_QWORD*)a2 = *v11;
    *v11 = 0i64;
    *(DWORD*)(a2 + 8) = *((DWORD*)v11 + 2);
    v13 = *((DWORD*)v11 + 3);
    v11[1] = 0i64;
    *(DWORD*)(a2 + 12) = v13;

    return a2;
}

FString GetAccountID(AFortPlayerControllerAthena* PC)
{
    __int64 Smth = 0;
    __int64 Smth4 = 0;
    __int64 Smth2 = GetAccID1(__int64(PC), (__int64)&Smth);
    __int64 Smth3 = GetAccID2(Smth2, (__int64)&Smth4);
    __int64 v5 = *(_QWORD*)(Smth2 + 8);
    return *(wchar_t**)(v5 + 24);
}

bool CanBeDropped(UFortItemDefinition* Def)
{
    return !Def->IsA(UFortWeaponMeleeItemDefinition::StaticClass()) && (Def->IsA(UFortWeaponRangedItemDefinition::StaticClass()) || Def->IsA(UFortResourceItemDefinition::StaticClass()) || Def->IsA(UFortConsumableItemDefinition::StaticClass()) || Def->IsA(UFortAmmoItemDefinition::StaticClass()));
}