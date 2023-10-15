#pragma once
#include "SDK.hpp"
#include "BackendStuff.h"
#include <fstream>

using namespace SDK;
using namespace Params;
using namespace std;

bool Ready = false;

typedef __int64 _QWORD;

//#define PEVENT

static UNetDriver* (*CreateNetDriver)(void*, void*, FName) = decltype(CreateNetDriver)(__int64(GetModuleHandleW(0)) + 0x4573990);
static __int64* (*GetInventoryOwnerFromWeapon)(void*) = decltype(GetInventoryOwnerFromWeapon)(__int64(GetModuleHandleW(0)) + 0x25EE400);
static bool (*InitListen)(void*, void*, FURL&, bool, FString&) = decltype(InitListen)(__int64(GetModuleHandleW(0)) + 0xD44C40);
static void (*SetWorld)(void*, void*) = decltype(SetWorld)(__int64(GetModuleHandleW(0)) + 0x42C2B20);
static void (*ServerReplicateActors)(void*) = decltype(ServerReplicateActors)(__int64(GetModuleHandleW(0)) + 0x1023F60);
static UObject* (*StaticFindObjectOG)(UClass*, UObject* Package, const wchar_t* OrigInName, bool ExactClass) = decltype(StaticFindObjectOG)(__int64(GetModuleHandleW(0)) + 0x2E1C4B0);
static UObject* (*StaticLoadObjectOG)(UClass* Class, UObject* InOuter, const TCHAR* Name, const TCHAR* Filename, uint32_t LoadFlags, UObject* Sandbox, bool bAllowObjectReconciliation, void*) = decltype(StaticLoadObjectOG)(__int64(GetModuleHandleW(0)) + 0x2E1CD60);
static bool (*InternalTryActivateAbility)(UAbilitySystemComponent* AbilitySystemComp, FGameplayAbilitySpecHandle AbilityToActivate, FPredictionKey InPredictionKey, UGameplayAbility** OutInstancedAbility, void* OnGameplayAbilityEndedDelegate, const FGameplayEventData* TriggerEventData) = decltype(InternalTryActivateAbility)(__int64(GetModuleHandleW(0)) + 0x6B33F0);
static FGameplayAbilitySpecHandle* (__fastcall* GiveAbilityOG)(UAbilitySystemComponent*, FGameplayAbilitySpecHandle*, FGameplayAbilitySpec) = decltype(GiveAbilityOG)(__int64(GetModuleHandleW(0)) + 0x6B19E0);
static void (*FGameplayAbilitySpecCtor)(FGameplayAbilitySpec*, UGameplayAbility*, int, int, UObject*) = decltype(FGameplayAbilitySpecCtor)(__int64(GetModuleHandleW(0)) + 0x6D6DD0);
static __int64 (*CantBuild)(UWorld*, UObject*, FVector, FRotator, char, void*, char*) = decltype(CantBuild)(__int64(GetModuleHandleW(0)) + 0x1E57790);
static ABuildingSMActor* (*ReplaceBuildingActor)(ABuildingSMActor* BuildingSMActor, unsigned int a2, UObject* a3, unsigned int a4, int a5, bool bMirrored, AFortPlayerControllerAthena* PC) = decltype(ReplaceBuildingActor)(__int64(GetModuleHandleW(0)) + 0x1B951B0);
static __int64(__fastcall* GiveAbilityAndActivateOnce)(void*, FGameplayAbilitySpecHandle*, FGameplayAbilitySpec) = decltype(GiveAbilityAndActivateOnce)(__int64(GetModuleHandleW(0)) + 0x6B1B00);
static LPVOID(*ProcessEvent)(void*, void*, void*) = nullptr;
static void(*RemoveFromAlivePlayers)(AFortGameModeAthena*, AFortPlayerControllerAthena*, APlayerState*, AFortPlayerPawn*, UFortWeaponItemDefinition*, EDeathCause, char) = decltype(RemoveFromAlivePlayers)(__int64(GetModuleHandleW(0)) + 0x18ECBB0);
static void(*OnRep_ZiplineState)(AFortPlayerPawn*) = decltype(OnRep_ZiplineState)(__int64(GetModuleHandleW(0)) + 0x2298F10);
static void (*XPCompOnProfileReady)(void*, void*) = decltype(XPCompOnProfileReady)(__int64(GetModuleHandleW(0)) + 0x19A9980);
static bool (*ShowPerkSelect)(AFortAthenaMutator_PerkSystemMutator* Mutator, void* a2) = decltype(ShowPerkSelect)(__int64(GetModuleHandleW(0)) + 0x1A934C0);
static __int64 (*GetAccID1)(__int64, __int64) = decltype(GetAccID1)(__int64(GetModuleHandleW(0)) + 0x22DC390);
static __int64 (*GetAccID2)(__int64, __int64) = decltype(GetAccID2)(__int64(GetModuleHandleW(0)) + 0xAE5CB0);
void (*BotManagerSetupStuffIdk)(__int64 BotManaager, __int64 Pawn, __int64 BehaviorTree, __int64 a4, DWORD* SkillLevel, __int64 idk, __int64 StartupInventory, __int64 BotNameSettings, __int64 idk_1, BYTE* CanRespawnOnDeath, unsigned __int8 BitFieldDataThing, BYTE* CustomSquadId, FFortAthenaAIBotRunTimeCustomizationData InRuntimeBotData) = decltype(BotManagerSetupStuffIdk)(__int64(GetModuleHandleW(0)) + 0x19D93F0);
static void (*SetMatchPlacement)(AFortPlayerStateAthena* PlayerState, int PlacementARg) = nullptr;
static __int64 (*StartAircraftPhase)(AFortGameModeAthena* GameMode, char a2) = nullptr;
static char (*GameSession_KickPlayer)(AGameSession*, AController*) = decltype(GameSession_KickPlayer)(__int64(GetModuleHandleW(0)) + 0x20DFE60);


enum class ELoadoutType : uint8
{
    LateGame = 0,
    CustomLTM = 1,
    MAX = 2
};

template <typename T = UObject>
T* Cast(UObject* Object, bool bCastOnly = false)
{
    if (Object)
    {
        if (!bCastOnly)
        {
            return Object->IsA(T::StaticClass()) ? (T*)Object : nullptr;
        }
        else
        {
            return (T*)Object;
        }
    }
    else
    {
        return nullptr;
    }
}

struct Loadout
{
    map<UFortItemDefinition*, vector<int>> ItemDefinitions{};
    ELoadoutType LoadoutType;
};

vector<Loadout> Loadouts{};
int MaxPlayersPerTeam = 1;
int MaxSquadSize = 1;
int CurrentPlayersOnTeam = 0;
uint8 NextIdx = 3;
uint8 FirstTeam = 3;
uint8 LastTeam = 3;
bool bIsLargeTeamGame = false;
bool bEnableLooting = false;
bool bArsenal = false;
bool bStormKing = false;
bool bSkipAircraft = false;
bool bSolos = true;
// EXPERIMENTAL, DO NOT USE UNLESS YOU KNOW WHAT YOU ARE DOING
//bool bNoTeams = true;
bool bSkipWarmup = false;
bool bBattleLab = false;
bool bLateGame = false;
bool bCreative = false;
bool bTravis = false;
bool bDropZone = false;
bool bTeamRumble = false;
bool bEnableScoringSystem = false;
bool bEnableSiphon = false;
AFortAIPawn* DadBroPawn = nullptr;
bool BotsEnabled = false;

UObject* TestObj = nullptr;
UObject* CountdownTimer = nullptr;

vector<string> AdminNames{"[EON] MagmaDev", "Charles", "Custox", "[EON] Armoon", "Whisper", "Glow"};//real real very secure
vector<string> AdminIps{ "L" };
map<AFortPlayerControllerAthena*, int> ReviveCounts{};

bool RandomPort = false;

FVector BusLocation;

static float XpToGivePerKill = 100.f;
static float XpMultiplierTime = 10.f;

static bool StartedEvent = false;

// 60.0f = 1 min
static float Duration = 120.f;
static int Port = 7777;
static string SessionName = "EonGameSession";

AFortAthenaMutator_Bots* BotMutator = nullptr;
UObject* JerkyLoader = nullptr;
UCompositeDataTable* LootTierData = nullptr;
UCompositeDataTable* LootPackages = nullptr;
UCurveTable* GameData = nullptr;

AActor* BattleBus = nullptr;

void ShowFoundation(ABuildingFoundation* Foundation)
{
    if (!Foundation)
    {
        printf("Invalid Foundation!\n");
        return;
    }

    Foundation->DynamicFoundationType = EDynamicFoundationType::Static;
    Foundation->bServerStreamedInLevel = true;
    Foundation->OnRep_ServerStreamedInLevel();
    Foundation->DynamicFoundationRepData.EnabledState = EDynamicFoundationEnabledState::Enabled;
    Foundation->FoundationEnabledState = EDynamicFoundationEnabledState::Enabled;
    Foundation->OnRep_DynamicFoundationRepData();
    Foundation->SetDynamicFoundationEnabled(true);
}

void sinCos(float* ScalarSin, float* ScalarCos, float Value)
{
    float quotient = (0.31830988618f * 0.5f) * Value;
    if (Value >= 0.0f)
    {
        quotient = (float)((int)(quotient + 0.5f));
    }
    else
    {
        quotient = (float)((int)(quotient - 0.5f));
    }
    float y = Value - (2.0f * 3.1415926535897932f) * quotient;

    float sign;
    if (y > 1.57079632679f)
    {
        y = 3.1415926535897932f - y;
        sign = -1.0f;
    }
    else if (y < -1.57079632679f)
    {
        y = -3.1415926535897932f - y;
        sign = -1.0f;
    }
    else
    {
        sign = +1.0f;
    }

    float y2 = y * y;

    *ScalarSin = (((((-2.3889859e-08f * y2 + 2.7525562e-06f) * y2 - 0.00019840874f) * y2 + 0.0083333310f) * y2 - 0.16666667f) * y2 + 1.0f) * y;

    float p = ((((-2.6051615e-07f * y2 + 2.4760495e-05f) * y2 - 0.0013888378f) * y2 + 0.041666638f) * y2 - 0.5f) * y2 + 1.0f;
    *ScalarCos = sign * p;
}

FQuat FRotToQuat(FRotator Rot)
{
    const float DEG_TO_RAD = 3.1415926535897932f / (180.f);
    const float DIVIDE_BY_2 = DEG_TO_RAD / 2.f;
    float SP, SY, SR;
    float CP, CY, CR;

    sinCos(&SP, &CP, Rot.Pitch * DIVIDE_BY_2);
    sinCos(&SY, &CY, Rot.Yaw * DIVIDE_BY_2);
    sinCos(&SR, &CR, Rot.Roll * DIVIDE_BY_2);

    FQuat RotationQuat;
    RotationQuat.X = CR * SP * SY - SR * CP * CY;
    RotationQuat.Y = -CR * SP * CY - SR * CP * SY;
    RotationQuat.Z = CR * CP * SY - SR * SP * CY;
    RotationQuat.W = CR * CP * CY + SR * SP * SY;

    return RotationQuat;
}

int GetOffset(UObject* Object, std::string Name)
{
    for (UObject* ObjClass = Object->Class; ObjClass; ObjClass = *(UObject**)(__int64(ObjClass) + 0x40))
    {
        void* Property = *(void**)(__int64(ObjClass) + 0x50);

        if (Property)
        {
            FName* PropFName = (FName*)(__int64(Property) + 0x28);
            string PropName = PropFName ? PropFName->ToString() : "";

            while (Property)
            {
                if (PropName == Name)
                {
                    return *(int*)(__int64(Property) + 0x4C);
                }

                Property = *(void**)(__int64(Property) + 0x20);
                PropFName = (FName*)(__int64(Property) + 0x28);
                PropName = Property ? (PropFName ? PropFName->ToString() : "None") : "None";
            }
        }
    }

    return 0;
}

template <typename T>
static T* StaticFindObject(string ObjectName)
{
    auto NameWStr = wstring(ObjectName.begin(), ObjectName.end()).c_str();

    return (T*)StaticFindObjectOG(T::StaticClass(), nullptr, NameWStr, false);
}

template<typename T>
T* StaticLoadObject(string name)
{
    T* Object = StaticFindObject<T>(name);

    if (!Object)
    {
        auto Name = std::wstring(name.begin(), name.end()).c_str();
        Object = (T*)StaticLoadObjectOG(T::StaticClass(), nullptr, Name, nullptr, 0, nullptr, false, nullptr);
    }

    return Object;
}

UFortEngine* GetEngine()
{
    static UFortEngine* Engine = UObject::FindObject<UFortEngine>("FortEngine_");
    return Engine;
}

UWorld* GetWorld()
{
    return GetEngine()->GameViewport->World;
}

AFortGameStateAthena* GetGameState()
{
    return (AFortGameStateAthena*)GetWorld()->GameState;
}

AFortGameModeAthena* GetGameMode()
{
    return (AFortGameModeAthena*)GetWorld()->AuthorityGameMode;
}

UFortKismetLibrary* GetFortKismet()
{
    return (UFortKismetLibrary*)UFortKismetLibrary::StaticClass()->DefaultObject;
}

UGameplayStatics* GetStatics()
{
    return (UGameplayStatics*)UGameplayStatics::StaticClass()->DefaultObject;
}

UKismetMathLibrary* GetMath()
{
    return (UKismetMathLibrary*)UKismetMathLibrary::StaticClass()->DefaultObject;
}

//static ofstream LogStream = ofstream("GSLOG.txt");
void Log(string msg)
{
    cout << msg << endl;
    //LogStream << msg << endl;
}

FString Conv_NameToString(FName name)
{
    return ((UKismetStringLibrary*)UKismetStringLibrary::StaticClass()->DefaultObject)->Conv_NameToString(name);
}

FName Conv_StringToName(FString str)
{
    return ((UKismetStringLibrary*)UKismetStringLibrary::StaticClass()->DefaultObject)->Conv_StringToName(str);
}

template<typename T>
T* SpawnActor(FVector Loc, FRotator Rot = {}, AActor* Owner = nullptr)
{
    static UGameplayStatics* statics = (UGameplayStatics*)UGameplayStatics::StaticClass()->DefaultObject;

    FTransform Transform{};
    Transform.Scale3D = FVector(1, 1, 1);
    Transform.Translation = Loc;

    return (T*)statics->FinishSpawningActor(statics->BeginDeferredActorSpawnFromClass(GetWorld(), T::StaticClass(), Transform, ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn, Owner), Transform);
}

template<typename T>
T* SpawnActor(UClass* Class, FVector Loc, FRotator Rot = {}, AActor* Owner = nullptr)
{
    static UGameplayStatics* statics = (UGameplayStatics*)UGameplayStatics::StaticClass()->DefaultObject;

    FTransform Transform{};
    Transform.Scale3D = FVector(1, 1, 1);
    Transform.Translation = Loc;
    Transform.Rotation = FRotToQuat(Rot);

    return (T*)statics->FinishSpawningActor(statics->BeginDeferredActorSpawnFromClass(GetWorld(), Class, Transform, ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn, Owner), Transform);
}

AFortPickupAthena* SpawnPickup(FVector Loc, UFortItemDefinition* Def, int Count, int LoadedAmmo, EFortPickupSourceTypeFlag SourceTypeFlag = EFortPickupSourceTypeFlag::Tossed, EFortPickupSpawnSource SpawnSource = EFortPickupSpawnSource::Unset, AFortPlayerPawn* Pawn = nullptr, bool Toss = true)
{
    AFortPickupAthena* NewPickup = SpawnActor<AFortPickupAthena>(Loc);
    NewPickup->bRandomRotation = true;
    NewPickup->PrimaryPickupItemEntry.ItemDefinition = Def;
    NewPickup->PrimaryPickupItemEntry.Count = Count;
    NewPickup->PrimaryPickupItemEntry.LoadedAmmo = LoadedAmmo;
    NewPickup->OnRep_PrimaryPickupItemEntry();
    NewPickup->PawnWhoDroppedPickup = Pawn;

    NewPickup->TossPickup(Loc, Pawn, -1, Toss, false, SourceTypeFlag, SpawnSource);

    if (SpawnSource == EFortPickupSpawnSource::Chest || SpawnSource == EFortPickupSpawnSource::AmmoBox)
    {
        NewPickup->bTossedFromContainer = true;
        NewPickup->OnRep_TossedFromContainer();
    }

    return NewPickup;
}

AFortPickupAthena* SpawnPickup(FVector Loc, FFortItemEntry* Entry, EFortPickupSourceTypeFlag SourceTypeFlag = EFortPickupSourceTypeFlag::Tossed, EFortPickupSpawnSource SpawnSource = EFortPickupSpawnSource::Unset, AFortPlayerPawn* Pawn = nullptr, int OverrideCount = 0) //test
{
    if (!Entry)
        return nullptr;

    AFortPickupAthena* NewPickup = SpawnActor<AFortPickupAthena>(Loc);
    NewPickup->bRandomRotation = true;
    //NewPickup->PrimaryPickupItemEntry.Count = OverrideCount != 0 ? OverrideCount : Entry->Count;
    //NewPickup->PrimaryPickupItemEntry.LoadedAmmo = Entry->LoadedAmmo;
    //NewPickup->PrimaryPickupItemEntry.ItemDefinition = Entry->ItemDefinition;
    //NewPickup->PrimaryPickupItemEntry.ItemGuid = Entry->ItemGuid;
    NewPickup->PrimaryPickupItemEntry = *Entry;
    NewPickup->PrimaryPickupItemEntry.Count = OverrideCount != 0 ? OverrideCount : Entry->Count;
    NewPickup->OnRep_PrimaryPickupItemEntry();
    NewPickup->PawnWhoDroppedPickup = Pawn;

    NewPickup->TossPickup(Loc, Pawn, -1, true, false, SourceTypeFlag, SpawnSource);

    if (SpawnSource == EFortPickupSpawnSource::Chest || SpawnSource == EFortPickupSpawnSource::AmmoBox)
    {
        NewPickup->bTossedFromContainer = true;
        NewPickup->OnRep_TossedFromContainer();
    }

    return NewPickup;
}

AActor* GetNearestPawn(AActor* Actor, float MaxDistance = 600.f)
{
    AActor* Last = nullptr;
    float LastDistance = 0;
    for (size_t i = 0; i < GetGameMode()->AlivePlayers.Num(); i++)
    {
        auto PC = GetGameMode()->AlivePlayers[i];
        if (PC && PC->Pawn)
        {
            if (Last)
            {
                if (PC->Pawn->GetDistanceTo(Actor) < LastDistance)
                {
                    LastDistance = PC->Pawn->GetDistanceTo(Actor);
                    Last = PC->Pawn;
                }
            }
            else
            {
                LastDistance = PC->Pawn->GetDistanceTo(Actor);
                Last = PC->Pawn;
            }
        }
    }

    if (LastDistance > MaxDistance)
        Last = nullptr;

    return Last;
}

void GiveXP(AFortPlayerControllerAthena* PC, int CombatXP, int SurvivalXP, int BonusMedalXP, int ChallengeXP, int MatchXP)
{
    UFortPlayerControllerAthenaXPComponent* XPComponent = *(UFortPlayerControllerAthenaXPComponent**)(__int64(PC) + 0x3560);

    if (XPComponent)
    {
        XPComponent->CombatXp += CombatXP;
        XPComponent->SurvivalXp += SurvivalXP;
        XPComponent->MedalBonusXP += BonusMedalXP;
        XPComponent->ChallengeXp += ChallengeXP;
        XPComponent->MatchXp += MatchXP;
        XPComponent->TotalXpEarned += (CombatXP + SurvivalXP + BonusMedalXP + ChallengeXP + MatchXP);
        XPComponent->CachedLevelInfo.LevelXp += XPComponent->TotalXpEarned;
        XPComponent->CachedLevelInfo.BookLevelXp += XPComponent->TotalXpEarned;
        XPComponent->OnXpUpdated(CombatXP, SurvivalXP, BonusMedalXP, ChallengeXP, MatchXP, XPComponent->TotalXpEarned);
    }
}