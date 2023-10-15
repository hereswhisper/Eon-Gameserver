#pragma once
#include "Abilities.h"
#include "Inventory.h"
#include "Looting.h"
#include "AntiCheat.h"
#include "misc.h"

void GiveModifier(UFortGameplayModifierItemDefinition* Modifier, AFortPlayerControllerAthena* PC)
{
	static auto AbilityOffset = GetOffset(Modifier, "PersistentAbilitySets");
	static auto EffectsOffset = GetOffset(Modifier, "PersistentGameplayEffects");

	TArray<FFortAbilitySetDeliveryInfo>& Abilities = *(TArray<FFortAbilitySetDeliveryInfo>*)(__int64(Modifier) + AbilityOffset);
	TArray<FFortGameplayEffectDeliveryInfo>& Effects = *(TArray<FFortGameplayEffectDeliveryInfo>*)(__int64(Modifier) + EffectsOffset);

	for (size_t i = 0; i < Abilities.Num(); i++)
	{
		auto& idk = Abilities[i];
		if (idk.DeliveryRequirements.bApplyToPlayerPawns)
		{
			for (size_t j = 0; j < idk.AbilitySets.Num(); j++)
			{
				UFortAbilitySet* Set = StaticLoadObject<UFortAbilitySet>(Conv_NameToString(idk.AbilitySets[j].ObjectID.AssetPathName).ToString());
				Log(Set->GetName());
				GiveAbilitySet(PC, Set);
			}
		}
	}

	for (size_t i = 0; i < Effects.Num(); i++)
	{
		auto& idk = Effects[i];
		if (idk.DeliveryRequirements.bApplyToPlayerPawns)
		{
			for (size_t j = 0; j < idk.GameplayEffects.Num(); j++)
			{
				UClass* Effect = StaticLoadObject<UClass>(Conv_NameToString(idk.GameplayEffects[j].GameplayEffect.ObjectID.AssetPathName).ToString());
				Log(Effect->GetName());
				PC->MyFortPawn->AbilitySystemComponent->BP_ApplyGameplayEffectToSelf(Effect, idk.GameplayEffects[j].Level, FGameplayEffectContextHandle());
			}
		}
	}
}

void (*ServerReadyToStartMatchOG)(AFortPlayerControllerAthena* PC);
void ServerReadyToStartMatch(AFortPlayerControllerAthena* PC)
{
	if (PC->IsA(AFortLiveBroadcastController::StaticClass()))
		return;

	string AccountID = GetAccountID(PC).ToString();

	if (EonApi::IsBanned(AccountID))
	{
		GameSession_KickPlayer(GetGameMode()->GameSession, PC);
		return;
	}

	// epic
	if (bStaffGameOnly) {
		auto Name = PC->PlayerState->GetPlayerName().ToString(); // i hope this works

		if (!VectorContains<string>(Name, AdminNames))
			PC->ClientReturnToMainMenu(L"This EON server has whitelists turned on. Please contact a server administrator if the problem presists.");

		auto IP = PC->PlayerState->SavedNetworkAddress.ToString();
		if (!VectorContains<string>(IP, AdminIps))
			PC->ClientReturnToMainMenu(L"This EON server has whitelists turned on. Please contact a server administrator if the problem presists.");
	}


	string name = PC->PlayerState->GetPlayerName().ToString();

	if (GetGameState()->GamePhase >= EAthenaGamePhase::Aircraft)
	{
		GameSession_KickPlayer(Cast<AFortGameSession>(GetGameMode()->GameSession), PC);
		return;
	}

	ReviveCounts[PC] = 0;

	if (bDropZone)
	{
		AFortAthenaMutator_SpyRumble* Mutator = (AFortAthenaMutator_SpyRumble*)GetGameState()->GetMutatorByClass(GetGameMode(), AFortAthenaMutator_SpyRumble::StaticClass());
		AFortAthenaMutator_PerkSystemMutator* Mutator2 = (AFortAthenaMutator_PerkSystemMutator*)GetGameState()->GetMutatorByClass(GetGameMode(), AFortAthenaMutator_PerkSystemMutator::StaticClass());

		cout << Mutator << endl;
		cout << Mutator2 << endl;

		UFortControllerComponent_PerkSystem* Comp = (UFortControllerComponent_PerkSystem*)PC->GetComponentByClass(UFortControllerComponent_PerkSystem::StaticClass());

		Comp->MutatorData = Mutator2->MutatorData;
		Comp->CachedFactionTag.TagName = Conv_StringToName(TEXT("Athena.Faction.Ego"));
		Log("PerkSelection num " + to_string(Comp->PerkSelection.Num()));
		Comp->RerollCount = 1;

		auto First = StaticLoadObject<UFortSpyTechItemDefinition>("/Game/Athena/Items/SpyTech/ItemDef/STID_PumpShotgun.STID_PumpShotgun");
		auto Second = StaticLoadObject<UFortSpyTechItemDefinition>("/Game/Athena/Items/SpyTech/ItemDef/STID_JetPack.STID_JetPack");

		Comp->SpyTechArray.Add(Comp->DefaultPerks[0]);
		Comp->SpyTechArray.Add(First);
		Comp->SpyTechArray.Add(Second);

		for (size_t i = 0; i < Comp->PerkSelection.Num(); i++)
		{
			Comp->PerkSelection[i].Items.FreeArray();
			Comp->PerkSelection[i].Items.Add(Comp->SpyTechArray[i]);
			Comp->PerkSelection[i].Items.Add(Comp->SpyTechArray[i]);
		}

		Comp->RoundStartCache.bDataReady = true;
		Log("FuturePerks " + to_string(Comp->RoundStartCache.FuturePerks.Num()));
		Log("SpyTechArray  " + to_string(Comp->SpyTechArray.Num()));
		Log("AllSelectedPerks  " + to_string(Comp->AllSelectedPerks.Num()));
		Log("DefaultPerks  " + to_string(Comp->DefaultPerks.Num()));

		Comp->OnRep_PerkSelection();
		Comp->OnRep_SpyTechArray();
		Comp->OnRep_RerollCount();
	}

	AFortPlayerStateAthena* PlayerState = (AFortPlayerStateAthena*)PC->PlayerState;

	//if (!bNoTeams) {
	*(uint8*)(__int64(PlayerState) + 0xFDC) = *(uint8*)(__int64(PlayerState) + 0xE60) - FirstTeam;
	PlayerState->OnRep_SquadId();
	PlayerState->OnRep_TeamIndex(0);
	PlayerState->OnRep_PlayerTeam();
	PlayerState->OnRep_PlayerTeamPrivate();

	FGameMemberInfo Member{ -1,-1,-1 };
	Member.TeamIndex = *(uint8*)(__int64(PlayerState) + 0xE60);
	Member.SquadId = *(uint8*)(__int64(PlayerState) + 0xFDC);
	Member.MemberUniqueId = *(FUniqueNetIdRepl*)(__int64(PlayerState) + 0x248);

	GetGameState()->GameMemberInfoArray.Members.Add(Member);
	GetGameState()->GameMemberInfoArray.MarkItemDirty(Member);
	//}

	return ServerReadyToStartMatchOG(PC);
}

static vector<APlayerState*> PlayersThatJoined{};
void (*ServerLoadingScreenDroppedOG)(AFortPlayerControllerAthena* PC);
void ServerLoadingScreenDropped(AFortPlayerControllerAthena* PC)
{
	if (PC->IsA(AFortLiveBroadcastController::StaticClass()))
		return;

	for (auto PlayerState : PlayersThatJoined)
	{
		if (PlayerState == PC->PlayerState)
		{
			return ServerLoadingScreenDroppedOG(PC);
		}
	}
	PlayersThatJoined.push_back(PC->PlayerState);

	if (GetGameState()->GamePhase >= EAthenaGamePhase::Aircraft)
	{
		GameSession_KickPlayer(Cast<AFortGameSession>(GetGameMode()->GameSession), PC);
		return;
	}

	Log(PC->PlayerState->GetPlayerName().ToString() + " just joined!");

	for (size_t i = 0; i < GetGameMode()->StartingItems.Num(); i++)
	{
		if (GetGameMode()->StartingItems[i].Count <= 0)
			continue;
		GiveItem(PC, GetGameMode()->StartingItems[i].Item, GetGameMode()->StartingItems[i].Count);
	}

	if (bArsenal)
	{
		AFortAthenaMutator_GG* Mutator = (AFortAthenaMutator_GG*)GetGameState()->GetMutatorByClass(GetGameMode(), AFortAthenaMutator_GG::StaticClass());

		GiveItem(PC, Mutator->WeaponEntries[0].Weapon, 1, GetAmmoForDef(Mutator->WeaponEntries[0].Weapon));
	}

	FFortAthenaLoadout& CosmeticLoadoutPC = *(FFortAthenaLoadout*)(__int64(PC) + 0x1810);

	if (bUsingApi) {

		if (!CosmeticLoadoutPC.Pickaxe)
		{
			////GameSession_KickPlayer(Cast<AFortGameSession>(GetGameMode()->GameSession), PC);
			//return;
		}

		GiveItem(PC, CosmeticLoadoutPC.Pickaxe->WeaponDefinition);

		*(FFortAthenaLoadout*)(__int64(PC->Pawn) + 0x18B8) = CosmeticLoadoutPC;
		((AFortPlayerPawnAthena*)PC->MyFortPawn)->OnRep_CosmeticLoadout();
	}
	else {
		static UFortWeaponMeleeItemDefinition* Pickaxe = StaticFindObject<UFortWeaponMeleeItemDefinition>("/Game/Athena/Items/Weapons/WID_Harvest_Pickaxe_Athena_C_T01.WID_Harvest_Pickaxe_Athena_C_T01");
		GiveItem(PC, Pickaxe);
	}

	static UFortAbilitySet* AbilitySet = StaticFindObject<UFortAbilitySet>("/Game/Abilities/Player/Generic/Traits/DefaultPlayer/GAS_AthenaPlayer.GAS_AthenaPlayer");
	GiveAbilitySet(PC, AbilitySet);

	auto PlayerState = (AFortPlayerStateAthena*)PC->PlayerState;
	auto CurrentPlaylist = GetGameState()->CurrentPlaylistInfo.BasePlaylist;
	auto& ModifierList = *(TArray<TSoftObjectPtr<UFortGameplayModifierItemDefinition>>*)(__int64(CurrentPlaylist) + 0x170);
	for (int i = 0; i < ModifierList.Num(); i++)
	{
		auto Current = StaticLoadObject<UFortGameplayModifierItemDefinition>(Conv_NameToString(ModifierList[i].ObjectID.AssetPathName).ToString());
		if (Current)
		{
			GiveModifier(Current, PC);
		}
	}

	GetFortKismet()->UpdatePlayerCustomCharacterPartsVisualization((AFortPlayerState*)PC->PlayerState);
	((AFortPlayerStateAthena*)PC->PlayerState)->OnRep_CharacterData();

	UFortPlayerControllerAthenaXPComponent* XPComponent = *(UFortPlayerControllerAthenaXPComponent**)(__int64(PC) + 0x3560);

	if (XPComponent)
	{
		InitXpComp(XPComponent, PlayerState);
	}

	if (bTeamRumble)
	{
		static UFortWeaponItemDefinition* ArDef = StaticLoadObject<UFortWeaponItemDefinition>("/Game/Athena/Items/Weapons/WID_Assault_Auto_Athena_UC_Ore_T03.WID_Assault_Auto_Athena_UC_Ore_T03");
		static UFortWeaponItemDefinition* PumpDef = StaticLoadObject<UFortWeaponItemDefinition>("/Game/Athena/Items/Weapons/WID_Shotgun_Standard_Athena_C_Ore_T03.WID_Shotgun_Standard_Athena_C_Ore_T03");

		GiveItem(PC, PumpDef, 1, 5);
		GiveItem(PC, ArDef, 1, 30);
		GiveItem(PC, PumpDef->GetAmmoWorldItemDefinition_BP(), 15);
		GiveItem(PC, ArDef->GetAmmoWorldItemDefinition_BP(), 60);
	}

	PC->GetQuestManager(ESubGame::Athena)->InitializeQuestAbilities(PC->Pawn);

	return ServerLoadingScreenDroppedOG(PC);
}

void ServerCheat(AFortPlayerControllerAthena* OPC, FString Msg)
{
	cout << OPC->GetName() << endl;
	AFortPlayerStateAthena* OPlayerState = (AFortPlayerStateAthena*)OPC->PlayerState;
	auto Name = OPlayerState->GetPlayerName().ToString();
	auto IP = OPlayerState->SavedNetworkAddress.ToString();

	bool Contains = !bUsingApi || VectorContains<string>(Name, AdminNames);

	bool Contains2 = !bUsingApi || VectorContains<string>(IP, AdminIps);

	cout << Contains << endl;

	if (!Contains)
		return;

	if (!Contains2)
		return;

	string MsgStr = Msg.ToString();
	cout << MsgStr << endl;

	if (MsgStr.contains("ban "))
	{
		string username = SplitString(true, "ban ", MsgStr);

		for (size_t i = 0; i < GetGameState()->PlayerArray.Num(); i++)
		{
			APlayerState* PlayerState = GetGameState()->PlayerArray[i];
			if (PlayerState && PlayerState->GetPlayerName().ToString() == username)
			{
				string IP = PlayerState->SavedNetworkAddress.ToString();
				AFortPlayerControllerAthena* PC = (AFortPlayerControllerAthena*)PlayerState->Owner;
				cout << "banned " << IP << " " << PlayerState->GetPlayerName().ToString() << endl;
				EonApi::BanPlayer(GetAccountID(PC).ToString());
				GameSession_KickPlayer(Cast<AFortGameSession>(GetGameMode()->GameSession), PC);
				break;
			}
		}
	}

	if (MsgStr == "startaircraft")
	{
		StartAircraftPhase(GetGameMode(), 0);
	}

	if (MsgStr == "simulatedeath")
	{
		auto PC = OPC;//i cba to replace
		if (!PC->IsA(AFortPlayerControllerAthena::StaticClass()))
			return;

		string accid = GetAccountID(PC).ToString();
		EonApi::GiveVbucks("500", GetGameState()->CurrentPlaylistInfo.BasePlaylist->PlaylistName.ToString(), "win", accid);
	}

	if (MsgStr == "endevent")
	{
		for (size_t i = 0; i < GetWorld()->NetDriver->ClientConnections.Num(); i++)
		{
			if (GetWorld()->NetDriver->ClientConnections[i])
				GetWorld()->NetDriver->ClientConnections[i]->PlayerController->ClientReturnToMainMenu(TEXT("Event has ended, thank you for playing!"));
		}
	}

	if (MsgStr == "startevent")
	{
		UFunction* StartEventFunc = JerkyLoader->Class->GetFunction("BP_Jerky_Loader_C", "startevent");
		Log(StartEventFunc->GetName());
		float idk = 0.f;
		ProcessEvent(JerkyLoader, StartEventFunc, &idk);
		Log("Started Astronomical");
	}

	if (MsgStr == "getpos")
	{
		OPC->Pawn->K2_GetActorLocation().Log();
	}

	if (MsgStr.contains("kick "))
	{
		string username = SplitString(true, "kick ", MsgStr);

		for (size_t i = 0; i < GetGameState()->PlayerArray.Num(); i++)
		{
			APlayerState* PlayerState = GetGameState()->PlayerArray[i];
			if (PlayerState && PlayerState->GetPlayerName().ToString() == username)
			{
				string IP = PlayerState->SavedNetworkAddress.ToString();
				AFortPlayerControllerAthena* PC = (AFortPlayerControllerAthena*)PlayerState->Owner;
				cout << "kicked " << IP << " " << PlayerState->GetPlayerName().ToString() << endl;
				GameSession_KickPlayer(Cast<AFortGameSession>(GetGameMode()->GameSession), PC);
				break;
			}
		}
	}

	if (MsgStr.contains("troll1 "))
	{
		string username = SplitString(true, "troll1 ", MsgStr);

		for (size_t i = 0; i < GetGameState()->PlayerArray.Num(); i++)
		{
			APlayerState* PlayerState = GetGameState()->PlayerArray[i];
			if (PlayerState && PlayerState->GetPlayerName().ToString() == username)
			{
				string IP = PlayerState->SavedNetworkAddress.ToString();
				AFortPlayerControllerAthena* PC = (AFortPlayerControllerAthena*)PlayerState->Owner;
				cout << "TROLLED " << IP << " " << PlayerState->GetPlayerName().ToString() << endl;
				AC::Old::HandleTroll(PC, 1);
				break;
			}
		}
	}

	if (MsgStr.contains("tp "))
	{
		string username = SplitString(true, "tp ", MsgStr);

		for (size_t i = 0; i < GetGameState()->PlayerArray.Num(); i++)
		{
			APlayerState* PlayerState = GetGameState()->PlayerArray[i];
			if (PlayerState && PlayerState->GetPlayerName().ToString() == username)
			{
				AFortPlayerControllerAthena* PC = (AFortPlayerControllerAthena*)PlayerState->Owner;

				FTransform Transform = PC->Pawn->GetTransform();
				OPC->Pawn->K2_SetActorTransform(Transform, false, nullptr, true);

				break;
			}
		}
	}

	if (MsgStr.contains("tpme "))
	{
		string username = SplitString(true, "tpme ", MsgStr);

		for (size_t i = 0; i < GetGameState()->PlayerArray.Num(); i++)
		{
			APlayerState* PlayerState = GetGameState()->PlayerArray[i];
			if (PlayerState && PlayerState->GetPlayerName().ToString() == username)
			{
				AFortPlayerControllerAthena* PC = (AFortPlayerControllerAthena*)PlayerState->Owner;

				FTransform Transform = OPC->Pawn->GetTransform();
				PC->Pawn->K2_SetActorTransform(Transform, false, nullptr, true);

				break;
			}
		}
	}

	if (MsgStr == "godmode 1")
	{
		OPC->MyFortPawn->HealthSet->Health.Minimum = 100;
		OPC->MyFortPawn->HealthSet->OnRep_Health();
	}

	if (MsgStr == "godmode 0")
	{
		OPC->MyFortPawn->HealthSet->Health.Minimum = 0;
		OPC->MyFortPawn->HealthSet->OnRep_Health();
	}

	if (MsgStr.contains("giveitem "))
	{
		string item = SplitString(true, "giveitem ", MsgStr);

		UFortItemDefinition* Item = UObject::FindObject<UFortItemDefinition>(item);
		if (Item)
		{
			GiveItem(OPC, Item);
		}
	}

	if (MsgStr.contains("sethealth "))
	{
		string health = SplitString(true, "sethealth ", MsgStr);
		OPC->MyFortPawn->SetMaxHealth(stoi(health));
		OPC->MyFortPawn->SetHealth(stoi(health));
	}

	if (MsgStr == "fly 1")
	{
		OPC->MyFortPawn->CharacterMovement->bCheatFlying = true;
		OPC->MyFortPawn->CharacterMovement->SetMovementMode(EMovementMode::MOVE_Flying, 0);
	}

	if (MsgStr == "fly 0")
	{
		OPC->MyFortPawn->CharacterMovement->bCheatFlying = false;
		OPC->MyFortPawn->CharacterMovement->SetMovementMode(EMovementMode::MOVE_Walking, 0);
	}

	if (MsgStr == "infiniteammo 1")
	{
		OPC->bInfiniteAmmo = true;
	}

	if (MsgStr == "infiniteammo 0")
	{
		OPC->bInfiniteAmmo = false;
	}

	return;
}

inline void ServerExecuteInventoryItem(AFortPlayerControllerAthena* PC, FGuid Guid)
{
	if (!PC->MyFortPawn || !PC->Pawn)
		return;

	for (size_t i = 0; i < PC->WorldInventory->Inventory.ReplicatedEntries.Num(); i++)
	{
		if (PC->WorldInventory->Inventory.ReplicatedEntries[i].ItemGuid == Guid)
		{
			UFortWeaponItemDefinition* DefToEquip = (UFortWeaponItemDefinition*)PC->WorldInventory->Inventory.ReplicatedEntries[i].ItemDefinition;

			if (PC->WorldInventory->Inventory.ReplicatedEntries[i].ItemDefinition->IsA(UFortGadgetItemDefinition::StaticClass()))
			{
				DefToEquip = ((UFortGadgetItemDefinition*)PC->WorldInventory->Inventory.ReplicatedEntries[i].ItemDefinition)->GetWeaponItemDefinition();
			}

			PC->MyFortPawn->EquipWeaponDefinition(DefToEquip, Guid);
			break;
		}
	}
}

void ServerAttemptAircraftJump(UFortControllerComponent_Aircraft* Comp, FRotator ClientRot)
{
	AFortPlayerControllerAthena* PC = (AFortPlayerControllerAthena*)Comp->GetOwner();

	if (bLateGame || bTravis)
	{
		FTransform Transform = BattleBus->GetTransform();
		APawn* Pawn = GetGameMode()->SpawnDefaultPawnAtTransform(PC, Transform);
		PC->Possess(Pawn);
	}
	else
	{
		GetGameMode()->RestartPlayer(PC);
	}

	if (PC->MyFortPawn)
	{
		PC->MyFortPawn->BeginSkydiving(true);
		if (bLateGame)
		{
			PC->MyFortPawn->SetHealth(100);
			PC->MyFortPawn->SetShield(100);
		}

		if (bStormKing)
		{
			PC->MyFortPawn->SetMaxHealth(500);
			PC->MyFortPawn->SetHealth(500);
		}

		if (bTravis)
		{
			PC->MyFortPawn->bCanBeDamaged = false;
			PC->MyFortPawn->HealthSet->Health.Minimum = 100;
			PC->MyFortPawn->HealthSet->OnRep_Health();
		}
	}
}

void (*ServerAcknowledgePossessionOG)(AFortPlayerControllerAthena* PC, APawn* Pawn);
void ServerAcknowledgePossession(AFortPlayerControllerAthena* PC, APawn* Pawn)
{
	PC->AcknowledgedPawn = Pawn;
	return ServerAcknowledgePossessionOG(PC, Pawn);
}

void (*ServerCreateBuildingActorOG)(AFortPlayerControllerAthena* PC, FCreateBuildingActorData CreateBuildingData);
void ServerCreateBuildingActor(AFortPlayerControllerAthena* PC, FCreateBuildingActorData CreateBuildingData)
{
	if (!PC || PC->IsInAircraft())
		return;

	AFortBroadcastRemoteClientInfo* Info = *(AFortBroadcastRemoteClientInfo**)(__int64(PC) + 0x3030);
	UClass* BuildingClass = Info->RemoteBuildableClass.Get();
	char a7;
	TArray<AActor*> BuildingsToRemove;
	if (!CantBuild(GetWorld(), BuildingClass, CreateBuildingData.BuildLoc, CreateBuildingData.BuildRot, CreateBuildingData.bMirrored, &BuildingsToRemove, &a7))
	{
		auto ResDef = GetFortKismet()->K2_GetResourceItemDefinition(((ABuildingSMActor*)BuildingClass->DefaultObject)->ResourceType);
		Remove(PC, ResDef, 10);

		ABuildingSMActor* NewBuilding = SpawnActor<ABuildingSMActor>(BuildingClass, CreateBuildingData.BuildLoc, CreateBuildingData.BuildRot, PC);
		uint8 Team = *(uint8*)(__int64(PC->PlayerState) + 0xE60);

		NewBuilding->bPlayerPlaced = true;
		NewBuilding->InitializeKismetSpawnedBuildingActor(NewBuilding, PC, true);
		NewBuilding->TeamIndex = Team;
		NewBuilding->Team = EFortTeam(Team);

		for (size_t i = 0; i < BuildingsToRemove.Num(); i++)
		{
			BuildingsToRemove[i]->K2_DestroyActor();
		}
		BuildingsToRemove.FreeArray();
	}

	return ServerCreateBuildingActorOG(PC, CreateBuildingData);
}

void ServerBeginEditBuildingActor(AFortPlayerControllerAthena* PC, ABuildingSMActor* ActorToEdit)
{
	if (!PC || !PC->MyFortPawn || !ActorToEdit)
		return;

	AFortPlayerStateAthena* PlayerState = (AFortPlayerStateAthena*)PC->PlayerState;
	if (!PlayerState)
		return;

	FFortItemEntry* EditToolEntry = FindEntry<UFortEditToolItemDefinition>(PC);
	if (!EditToolEntry)
		return;

	PC->ServerExecuteInventoryItem(EditToolEntry->ItemGuid);
}

void ServerEditBuildingActor(AFortPlayerControllerAthena* PC, ABuildingSMActor* ActorToEdit, TSubclassOf<ABuildingSMActor> NewClass, uint8 RotationIterations, bool bMirrored)
{
	if (!PC || !ActorToEdit || !NewClass.Get())
		return;

	ActorToEdit->SetNetDormancy(ENetDormancy::DORM_DormantAll);
	*(void**)(__int64(ActorToEdit) + 0xA20) = nullptr;

	ABuildingSMActor* NewBuild = ReplaceBuildingActor(ActorToEdit, 1, NewClass.Get(), 0, RotationIterations, bMirrored, PC);

	if (NewBuild)
		NewBuild->bPlayerPlaced = true;
}

void ServerEndEditingBuildingActor(AFortPlayerControllerAthena* PC, ABuildingSMActor* ActorToStopEditing)
{
	if (!PC || !PC->MyFortPawn || !ActorToStopEditing || ActorToStopEditing->EditingPlayer != (AFortPlayerStateZone*)PC->PlayerState)
		return;

	ActorToStopEditing->SetNetDormancy(ENetDormancy::DORM_DormantAll);
	*(void**)(__int64(ActorToStopEditing) + 0xA20) = nullptr;

	AFortWeap_EditingTool* EditTool = (AFortWeap_EditingTool*)PC->MyFortPawn->CurrentWeapon; // Cast<>() ??

	if (!EditTool)
		return;

	*(void**)(__int64(EditTool) + 0xBA0) = nullptr;
	EditTool->OnRep_EditActor();
}

void ServerPlayEmoteItem(AFortPlayerControllerAthena* PC, UFortMontageItemDefinitionBase* EmoteAsset, float EmoteRandomNumber)
{
	if (!PC || !EmoteAsset || !PC->MyFortPawn)
		return;

	if (EmoteAsset->IsA(UAthenaDanceItemDefinition::StaticClass()))
	{
		FGameplayAbilitySpec Spec{};
		FGameplayAbilitySpecCtor(&Spec, (UGameplayAbility*)UGAB_Emote_Generic_C::StaticClass()->DefaultObject, 1, -1, EmoteAsset);
		GiveAbilityAndActivateOnce(PC->MyFortPawn->AbilitySystemComponent, &Spec.Handle, Spec);
	}
}

void ServerReturnToMainMenu(AFortPlayerControllerAthena* PC)
{
	PC->ClientReturnToMainMenu(TEXT(""));
}

void (*ClientOnPawnDiedOG)(AFortPlayerControllerAthena* PC, FFortPlayerDeathReport DeathReport);
void ClientOnPawnDied(AFortPlayerControllerAthena* DeadPC, FFortPlayerDeathReport DeathReport)
{
	if (bStormKing)
		return ClientOnPawnDiedOG(DeadPC, DeathReport);

	AFortPlayerStateAthena* DeadState = (AFortPlayerStateAthena*)DeadPC->PlayerState;
	APlayerPawn_Athena_C* KillerPawn = (APlayerPawn_Athena_C*)DeathReport.KillerPawn;
	AFortPlayerStateAthena* KillerState = (AFortPlayerStateAthena*)DeathReport.KillerPlayerState;
	bool Won = false;

	if (!GetGameState()->IsRespawningAllowed(DeadState))//!bLateGameRespawn
	{
		if (DeadPC && DeadPC->WorldInventory)
		{
			for (size_t i = 0; i < DeadPC->WorldInventory->Inventory.ReplicatedEntries.Num(); i++)
			{
				if (!DeadPC->WorldInventory->Inventory.ReplicatedEntries[i].ItemDefinition->IsA(UFortWeaponMeleeItemDefinition::StaticClass()) && (DeadPC->WorldInventory->Inventory.ReplicatedEntries[i].ItemDefinition->IsA(UFortWeaponRangedItemDefinition::StaticClass()) || DeadPC->WorldInventory->Inventory.ReplicatedEntries[i].ItemDefinition->IsA(UFortResourceItemDefinition::StaticClass()) || DeadPC->WorldInventory->Inventory.ReplicatedEntries[i].ItemDefinition->IsA(UFortConsumableItemDefinition::StaticClass()) || DeadPC->WorldInventory->Inventory.ReplicatedEntries[i].ItemDefinition->IsA(UFortAmmoItemDefinition::StaticClass())))
				{
					SpawnPickup(DeadPC->Pawn->K2_GetActorLocation(), &DeadPC->WorldInventory->Inventory.ItemInstances[i]->ItemEntry, EFortPickupSourceTypeFlag::Player, EFortPickupSpawnSource::PlayerElimination, DeadPC->MyFortPawn);
				}
			}
		}
	}
	static int PlaceOffset = 0xE6C;

	if (!Won && DeadPC && DeadState)
	{
		if (KillerState && KillerState != DeadState)
		{
			static auto KillScoreOffset = GetOffset(KillerState, "KillScore");
			static auto TeamKillScoreOffset = GetOffset(KillerState, "TeamKillScore");

			++*(int*)(__int64(KillerState) + KillScoreOffset);
			++*(int*)(__int64(KillerState) + TeamKillScoreOffset);
			KillerState->ClientReportKill(DeadState);
			KillerState->OnRep_Kills();
			KillerState->OnRep_TeamKillScore();



			static auto DeathLocationOffset = GetOffset(DeadState, "PawnDeathLocation");
			static auto DeathInfoOffset = GetOffset(DeadState, "DeathInfo");

			*(FVector*)(__int64(DeadState) + DeathLocationOffset) = DeadPC->Pawn->K2_GetActorLocation();
			FDeathInfo& DeathInfo = *(FDeathInfo*)(__int64(DeadState) + DeathInfoOffset);

			DeathInfo.bDBNO = DeadPC->MyFortPawn->bWasDBNOOnDeath;
			DeathInfo.bInitialized = true;
			DeathInfo.DeathLocation = DeadPC->Pawn->K2_GetActorLocation();
			DeathInfo.DeathTags = DeathReport.Tags;
			DeathInfo.Downer = KillerState;
			DeathInfo.Distance = (KillerPawn ? KillerPawn->GetDistanceTo(DeadPC->Pawn) : ((AFortPlayerPawnAthena*)DeadPC->Pawn)->LastFallDistance);
			DeathInfo.FinisherOrDowner = KillerState;
			DeathInfo.DeathCause = DeadState->ToDeathCause(DeathInfo.DeathTags, DeathInfo.bDBNO);
			DeadState->OnRep_DeathInfo();

			if (bEnableSiphon)
			{
				if (KillerPawn)
				{
					float Health = KillerPawn->GetHealth();
					float NewHealthAmount = Health + 50;

					KillerPawn->SetHealth(NewHealthAmount);

					if (NewHealthAmount > 100)
					{
						float ShieldToGive = (NewHealthAmount - 100) + KillerPawn->GetShield();

						KillerPawn->SetHealth(100);
						KillerPawn->SetShield(ShieldToGive);

						if (KillerPawn->GetShield() > 100) { KillerPawn->SetShield(100); }
					}
				}
			}

			int Kills = *(int*)(__int64(KillerState) + KillScoreOffset);
			if (bEnableScoringSystem)
			{
				static auto ScoreOffset = GetOffset(KillerState, "Score");
				static auto TeamScoreOffset = GetOffset(KillerState, "TeamScore");
				static auto TeamScorePlacementOffset = GetOffset(KillerState, "TeamScorePlacement");
				static auto OldTotalScoreStatOffset = GetOffset(KillerState, "OldTotalScoreStat");
				static auto TotalPlayerScoreOffset = GetOffset(KillerState, "TotalPlayerScore");

				*(float*)(__int64(KillerState) + ScoreOffset) = Kills;
				*(int32*)(__int64(KillerState) + TeamScoreOffset) = Kills;
				int32& KillerStatePlacement = *(int32*)(__int64(KillerState) + TeamScorePlacementOffset);

				KillerStatePlacement = 1;
				*(int32*)(__int64(KillerState) + OldTotalScoreStatOffset) = Kills;
				*(int32*)(__int64(KillerState) + TotalPlayerScoreOffset) = Kills;
				GetGameState()->CurrentHighScoreTeam = 3;
				GetGameState()->CurrentHighScore = Kills;
				GetGameState()->OnRep_CurrentHighScore();
				GetGameState()->WinningScore = Kills;
				GetGameState()->WinningTeam = 3;
				GetGameState()->OnRep_WinningTeam();
				GetGameState()->OnRep_WinningScore();
				KillerState->OnRep_Score();
				KillerState->OnRep_TeamScore();
				KillerState->OnRep_TeamScorePlacement();
				KillerState->OnRep_TotalPlayerScore();
				KillerState->UpdateScoreStatChanged();
			}

			if (bArsenal)
			{
				AFortAthenaMutator_GG* Mutator = (AFortAthenaMutator_GG*)GetGameState()->GetMutatorByClass(GetGameMode(), AFortAthenaMutator_GG::StaticClass());
				AFortPlayerControllerAthena* KillerPC = (AFortPlayerControllerAthena*)KillerState->GetOwner();

				if (KillerPC->WorldInventory)
				{
					for (size_t i = 0; i < KillerPC->WorldInventory->Inventory.ReplicatedEntries.Num(); i++)
					{
						auto Mutator = (AFortAthenaMutator_GG*)GetGameState()->GetMutatorByClass(GetGameMode(), AFortAthenaMutator_GG::StaticClass());
						if (Mutator->WeaponEntries.IsValidIndex(Kills - 1))
						{
							Remove(KillerPC, Mutator->WeaponEntries[Kills - 1].Weapon);
						}
					}

					if (Mutator->WeaponEntries.IsValidIndex(Kills))
					{
						GiveItem(KillerPC, Mutator->WeaponEntries[Kills].Weapon, 1, GetAmmoForDef(Mutator->WeaponEntries[Kills].Weapon));
					}
				}

				if (Kills == Mutator->ScoreToWin)
				{
					Log("Won");
					*(int32*)(__int64(KillerState) + PlaceOffset) = 1;
					KillerState->OnRep_Place();

					for (size_t i = 0; i < GetGameMode()->AlivePlayers.Num(); i++)
					{
						AFortPlayerStateAthena* PlayerState = ((AFortPlayerStateAthena*)GetGameMode()->AlivePlayers[i]->PlayerState);
						int PlayerKills = *(int*)(__int64(PlayerState) + 0xE74);

						if (GetGameMode()->AlivePlayers[i] != KillerPC)
						{
							*(int32*)(__int64(PlayerState) + PlaceOffset) = PlayerKills;
							PlayerState->OnRep_Place();
						}
					}

					GetGameState()->WinningPlayerState = KillerState;
					GetGameState()->WinningScore = 1;
					GetGameState()->WinningTeam = *(uint8*)(__int64(KillerState) + 0xE60);

					GetGameState()->OnRep_WinningPlayerState();
					GetGameState()->OnRep_WinningScore();
					GetGameState()->OnRep_WinningTeam();
					GetGameMode()->EndMatch();
					Won = true;
				}
			}
		}

		if (Won || !GetGameState()->IsRespawningAllowed(DeadState))//!bLateGameRespawn
		{
			GiveXP(DeadPC, 0, GetStatics()->GetTimeSeconds(GetWorld()) * XpMultiplierTime, 0, 0, 100);
			FAthenaRewardResult Result;
			UFortPlayerControllerAthenaXPComponent* XPComponent = *(UFortPlayerControllerAthenaXPComponent**)(__int64(DeadPC) + 0x3560);
			Result.TotalBookXpGained = XPComponent->TotalXpEarned;
			Result.TotalSeasonXpGained = XPComponent->TotalXpEarned;
			DeadPC->ClientSendEndBattleRoyaleMatchForPlayer(true, Result);

			FAthenaMatchStats Stats;
			FAthenaMatchTeamStats TeamStats;

			if (DeadState)
			{
				*(int32*)(__int64(DeadState) + PlaceOffset) = GetGameState()->PlayersLeft;
				DeadState->OnRep_Place();
			}

			for (size_t i = 0; i < 20; i++)
			{
				Stats.Stats[i] = 0;
			}

			Stats.Stats[3] = *(int32*)(__int64(DeadState) + 0xE74);
			Stats.Stats[7] = ReviveCounts[DeadPC];
			Stats.Stats[8] = *(uint16*)(__int64(DeadPC->PlayerState) + 0xFDA);

			TeamStats.Place = GetGameState()->PlayersLeft;
			TeamStats.TotalPlayers = GetGameState()->TotalPlayers;

			DeadPC->ClientSendMatchStatsForPlayer(Stats);
			DeadPC->ClientSendTeamStatsForPlayer(TeamStats);
			FDeathInfo* DeathInfo = (FDeathInfo*)(__int64(DeadState) + 0x12C8);

			RemoveFromAlivePlayers(GetGameMode(), DeadPC, (KillerState == DeadState ? nullptr : KillerState), KillerPawn, DeathReport.KillerWeapon ? DeathReport.KillerWeapon : nullptr, DeadState ? DeathInfo->DeathCause : EDeathCause::Rifle, 0);

			if (DeadState)
			{
				DeadState->OnRep_DeathInfo();
				*(int32*)(__int64(DeadState) + PlaceOffset) = GetGameState()->PlayersLeft + 1;
				DeadState->OnRep_Place();
			}

			if (KillerState)
			{
				//AFortPlayerControllerAthena* PC = (AFortPlayerControllerAthena*)KillerState->Owner;
				//GiveXP(PC, 150, 1000, 0, 0, 500);

				if (*(int32*)(__int64(KillerState) + PlaceOffset) == 1)
				{
					//EonApi::GiveVbucks("500", GetGameState()->CurrentPlaylistInfo.BasePlaylist->PlaylistName.ToString(), "win", GetAccountID(PC).ToString());
					EonApi::RemoveSession(Region, SessionName);
					if (DeathReport.KillerWeapon)
					{
						((AFortPlayerControllerAthena*)KillerState->Owner)->PlayWinEffects(KillerPawn, DeathReport.KillerWeapon, EDeathCause::Rifle, false);
						((AFortPlayerControllerAthena*)KillerState->Owner)->ClientNotifyWon(KillerPawn, DeathReport.KillerWeapon, EDeathCause::Rifle);
					}

					FAthenaRewardResult Result;
					AFortPlayerControllerAthena* KillerPC = (AFortPlayerControllerAthena*)KillerState->GetOwner();
					KillerPC->ClientSendEndBattleRoyaleMatchForPlayer(true, Result);

					FAthenaMatchStats Stats;
					FAthenaMatchTeamStats TeamStats;

					for (size_t i = 0; i < 20; i++)
					{
						Stats.Stats[i] = 0;
					}

					Stats.Stats[3] = *(int32*)(__int64(KillerState) + 0xE74);
					Stats.Stats[7] = ReviveCounts[KillerPC];
					Stats.Stats[8] = *(uint16*)(__int64(KillerState) + 0xFDA);

					TeamStats.Place = 1;
					TeamStats.TotalPlayers = GetGameState()->TotalPlayers;

					KillerPC->ClientSendMatchStatsForPlayer(Stats);
					KillerPC->ClientSendTeamStatsForPlayer(TeamStats);

					GetGameState()->WinningPlayerState = KillerState;
					GetGameState()->WinningTeam = *(uint8*)(__int64(KillerState) + 0xE60);
					GetGameState()->OnRep_WinningPlayerState();
					GetGameState()->OnRep_WinningTeam();
				}
			}
		}
	}
	return ClientOnPawnDiedOG(DeadPC, DeathReport);
}

void ServerAttemptInventoryDrop(AFortPlayerControllerAthena* PC, FGuid ItemGuid, int32 Count, bool bTrash)
{
	if (bArsenal)
		return;

	for (size_t i = 0; i < PC->WorldInventory->Inventory.ReplicatedEntries.Num(); i++)
	{
		FFortItemEntry& Entry = PC->WorldInventory->Inventory.ReplicatedEntries[i];

		if (Entry.ItemGuid == ItemGuid)
		{
			Entry.Count -= Count;
			if (Entry.Count <= 0)
			{
				SpawnPickup(PC->Pawn->K2_GetActorLocation(), &Entry, EFortPickupSourceTypeFlag::Player, EFortPickupSpawnSource::Unset, PC->MyFortPawn, Count);
				Remove(PC, ItemGuid);
				break;
			}
			SpawnPickup(PC->Pawn->K2_GetActorLocation(), &Entry, EFortPickupSourceTypeFlag::Player, EFortPickupSpawnSource::Unset, PC->MyFortPawn, Count);
			UpdateInventory(PC, &Entry);
			ModifyEntry(PC, Entry);
			break;
		}
	}
}

void (*ServerClientIsReadyToRespawnOG)(AFortPlayerControllerAthena* PC);
void ServerClientIsReadyToRespawn(AFortPlayerControllerAthena* PC)
{
	AFortPlayerStateAthena* PlayerState = (AFortPlayerStateAthena*)PC->PlayerState;

	FFortRespawnData& RespawnData = *(FFortRespawnData*)(__int64(PlayerState) + 0xD30);

	if (RespawnData.bRespawnDataAvailable && RespawnData.bServerIsReady)
	{
		RespawnData.bClientIsReady = true;

		FTransform Transform{};
		Transform.Scale3D = FVector{ 1,1,1 };
		Transform.Translation = RespawnData.RespawnLocation;

		AFortPlayerPawnAthena* NewPawn = (AFortPlayerPawnAthena*)GetGameMode()->SpawnDefaultPawnAtTransform(PC, Transform);
		PC->Possess(NewPawn);

		PC->MyFortPawn->SetMaxHealth(100);
		PC->MyFortPawn->SetMaxShield(100);
		PC->MyFortPawn->SetHealth(100);
		PC->MyFortPawn->SetShield(0);

		PC->MyFortPawn->bCanBeDamaged = true; // nah no need for this

		PC->RespawnPlayerAfterDeath(true);
	}

	return ServerClientIsReadyToRespawnOG(PC);
}

void SetMatchPlacementHook(AFortPlayerStateAthena* PlayerState, int Placement)
{
	return SetMatchPlacement(PlayerState, Placement);
}