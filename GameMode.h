#pragma once
#include "misc.h"
#include "Floorloot.h"
#include "Vehicles.h"
#include "AntiCheat.h"

bool InitListenFunc(void* Driver, void* World, FURL& url, bool reuse, FString& error)
{
	if (!InitListen(Driver, World, url, reuse, error))
	{
		if(RandomPort)
			Port = GetMath()->RandomIntegerInRange(1000, 9999);
		url.Port = Port;
		return InitListenFunc(Driver, World, url, reuse, error);
	}
}

void DadBroHealthTest()
{
	Sleep(10000);
	
	while (true)
	{
		if (!DadBroPawn || DadBroPawn->IsDead() || DadBroPawn->GetHealth() <= 0)
		{
			Log("Won!");

			for (size_t i = 0; i < GetGameState()->PlayerArray.Num(); i++)
			{
				AFortPlayerStateAthena* PlayerState = (AFortPlayerStateAthena*)GetGameState()->PlayerArray[i];
				if (PlayerState)
				{
					*(int32*)(__int64(PlayerState) + 0xE6C) = 1;
					PlayerState->OnRep_Place();
				}
			}
			break;
		}
	}
}

void StartDadBro()
{
	UClass* DadBroPawnClass = StaticLoadObject<UClass>("/Game/Athena/DADBRO/DADBRO_Pawn.DADBRO_Pawn_C");
	AFortAthenaMutator_DadBro* Mutator = (AFortAthenaMutator_DadBro*)GetGameState()->GetMutatorByClass(GetGameMode(), AFortAthenaMutator_DadBro::StaticClass());
	Mutator->DadBroSpawnLocation.Z = -193.048096f;
	DadBroPawn = SpawnActor<AFortAIPawn>(DadBroPawnClass, Mutator->DadBroSpawnLocation);

	Mutator->DadBroPawn = DadBroPawn;
	Mutator->DadBroCodeState = EDadBroState::Active;
	Mutator->OnRep_DadBroPawn();
	Mutator->OnRep_DadBroCodeState();

	new thread(DadBroHealthTest);
}

bool (*ReadyToStartMatchOG)(AFortGameModeAthena* GameMode);
bool ReadyToStartMatch(AFortGameModeAthena* GameMode)
{
	ReadyToStartMatchOG(GameMode);
	static bool InitPlaylist = false;
	static bool Listening = false;
	static string CustomCode = "DevGame";

	if (!Ready)
		return false;

	if (!InitPlaylist)
	{
		InitPlaylist = true;
		if (RandomPort)
			Port = GetMath()->RandomIntegerInRange(1000, 9999);
		SessionName += to_string(GetMath()->RandomInteger(2147483646));
		EonApi::SetupAPI();
		//Playlist_Cobalt_Squads_P_Respawn
		//Playlist_Respawn_Op
		//AFortAthenaMutator_Infiltration
		//UFortPlaylistAthena* Playlist = UObject::FindObject<UFortPlaylistAthena>("Playlist_Cobalt_Squads_P_Respawn.Playlist_Cobalt_Squads_P_Respawn");
		//UFortPlaylistAthena* Playlist = UObject::FindObject<UFortPlaylistAthena>("Playlist_Respawn_Op.Playlist_Respawn_Op");
		//UFortPlaylistAthena* Playlist = UObject::FindObject<UFortPlaylistAthena>("Playlist_Mash_Squads.Playlist_Mash_Squads");
		//UFortPlaylistAthena* Playlist = UObject::FindObject<UFortPlaylistAthena>("Playlist_DADBRO_Squads.Playlist_DADBRO_Squads");
		//UFortPlaylistAthena* Playlist = UObject::FindObject<UFortPlaylistAthena>("Playlist_DefaultSolo.Playlist_DefaultSolo");
		//UFortPlaylistAthena* Playlist = UObject::FindObject<UFortPlaylistAthena>("Playlist_ShowdownAlt_Solo.Playlist_ShowdownAlt_Solo");
		//UFortPlaylistAthena* Playlist = UObject::FindObject<UFortPlaylistAthena>("Playlist_Bots_DefaultSolo.Playlist_Bots_DefaultSolo");
		UFortPlaylistAthena* Playlist = UObject::FindObject<UFortPlaylistAthena>("Playlist_DefaultSolo.Playlist_DefaultSolo");
		//UFortPlaylistAthena* Playlist = UObject::FindObject<UFortPlaylistAthena>("Playlist_DefaultSquad.Playlist_DefaultSquad");
		//UFortPlaylistAthena* Playlist = UObject::FindObject<UFortPlaylistAthena>("Playlist_BattleLab.Playlist_BattleLab");
		 //UFortPlaylistAthena* Playlist = UObject::FindObject<UFortPlaylistAthena>("Playlist_Gg_Reverse.Playlist_Gg_Reverse");
		//UFortPlaylistAthena* Playlist = UObject::FindObject<UFortPlaylistAthena>("Playlist_Respawn_24.Playlist_Respawn_24");
		//UFortPlaylistAthena* Playlist = UObject::FindObject<UFortPlaylistAthena>("Playlist_Music_High.Playlist_Music_High");
		// UFortPlaylistAthena* Playlist = UObject::FindObject<UFortPlaylistAthena>("Playlist_PlaygroundV2.Playlist_PlaygroundV2");
		GetGameState()->CurrentPlaylistInfo.BasePlaylist = Playlist;
		GetGameState()->CurrentPlaylistInfo.PlaylistReplicationKey++;
		GetGameState()->CurrentPlaylistInfo.MarkArrayDirty();
		GetGameState()->CurrentPlaylistId = Playlist->PlaylistId;
		
		//MaxPlayersPerTeam = 64;

		//MaxSquadSize = *(int32*)(__int64(Playlist) + GetOffset(Playlist, "MaxSquadSize"));
		//MaxPlayersPerTeam = *(int32*)(__int64(Playlist) + GetOffset(Playlist, "MaxTeamSize"));
		MaxPlayersPerTeam = *(int32*)(__int64(Playlist) + GetOffset(Playlist, "MaxSquadSize"));

		if (bLateGame || bSolos)
			MaxPlayersPerTeam = 1;

		NextIdx = *(uint8*)(__int64(Playlist) + GetOffset(Playlist, "DefaultFirstTeam"));
		
		bIsLargeTeamGame = *(bool*)(__int64(Playlist) + GetOffset(Playlist, "bIsLargeTeamGame"));
		FirstTeam = NextIdx;
		LastTeam = *(uint8*)(__int64(Playlist) + GetOffset(Playlist, "DefaultLastTeam"));
		bSkipWarmup = *(bool*)(__int64(Playlist) + GetOffset(Playlist, "bSkipWarmup"));
		bSkipAircraft = *(bool*)(__int64(Playlist) + GetOffset(Playlist, "bSkipAircraft"));

		GetGameState()->bGameModeWillSkipAircraft = bSkipAircraft;

		if (MaxPlayersPerTeam > 1)
		{
			GameMode->bDBNOEnabled = true;
			GameMode->bAlwaysDBNO = true;
			GetGameState()->bDBNODeathEnabled = true;
			GetGameState()->SetIsDBNODeathEnabled(true);
		}

		TSoftObjectPtr<UCompositeDataTable>& LootTierDataPtr = *(TSoftObjectPtr<UCompositeDataTable>*)(__int64(Playlist) + GetOffset(Playlist, "LootTierData"));
		TSoftObjectPtr<UCurveTable>& GameDataPtr = *(TSoftObjectPtr<UCurveTable>*)(__int64(Playlist) + GetOffset(Playlist, "GameData"));
		TSoftObjectPtr<UCompositeDataTable>& LootPackagesPtr = *(TSoftObjectPtr<UCompositeDataTable>*)(__int64(Playlist) + GetOffset(Playlist, "LootPackages"));
		bTravis = Playlist->GetName() == "Playlist_Music_High";
		LootTierData = StaticLoadObject<UCompositeDataTable>(Conv_NameToString(LootTierDataPtr.ObjectID.AssetPathName).ToString());
		GameData = StaticLoadObject<UCurveTable>(Conv_NameToString(GameDataPtr.ObjectID.AssetPathName).ToString());
		LootPackages = StaticLoadObject<UCompositeDataTable>(Conv_NameToString(LootPackagesPtr.ObjectID.AssetPathName).ToString());
		
		cout << "MaxPlayersPerTeam " << MaxPlayersPerTeam << " NextIdx " << to_string(NextIdx) << "FirstTeam " << to_string(FirstTeam) << "LastTeam " << to_string(LastTeam) << endl;
		cout << "LootPackages " << LootPackages << " GameData " << GameData << "LootTierData " << LootTierData << endl;
		Sleep(1000);
		wstring SessionNameWStr = wstring(SessionName.begin(), SessionName.end());
		AFortGameSessionDedicatedAthena* GameSession = SpawnActor<AFortGameSessionDedicatedAthena>({});
		*(int32*)(__int64(GameSession) + GetOffset(GameSession, "MaxPlayers")) = *(int32*)(__int64(Playlist) + GetOffset(Playlist, "MaxPlayers"));
		if (bLateGame)
			*(int32*)(__int64(GameSession) + GetOffset(GameSession, "MaxPlayers")) = 40;
		if (bTravis)
			*(int32*)(__int64(GameSession) + GetOffset(GameSession, "MaxPlayers")) = 100;
		GameSession->SessionName = Conv_StringToName(SessionNameWStr.c_str());
		GameMode->GameSession = GameSession;
		GameMode->FortGameSession = GameSession;
		GameMode->GameModeSessionString = SessionNameWStr.c_str();

		float TimeSeconds = GetStatics()->GetTimeSeconds(GetWorld());

		GetGameState()->WarmupCountdownEndTime = TimeSeconds + Duration;
		GameMode->WarmupCountdownDuration = Duration;
		GetGameState()->WarmupCountdownStartTime = TimeSeconds;
		GameMode->WarmupEarlyCountdownDuration = Duration;
		
		if (bTravis)
		{
			UClass* ClassTest = StaticLoadObject<UClass>("/CycloneJerky/Gameplay/BP_Jerky_Scripting.BP_Jerky_Scripting_C");
			cout << ClassTest->GetName() << endl;

			TSoftObjectPtr<UObject>& Test = *(TSoftObjectPtr<UObject>*)(__int64(ClassTest->DefaultObject) + GetOffset(ClassTest->DefaultObject, "CountdownTimer"));
			cout << Conv_NameToString(Test.ObjectID.AssetPathName).ToString() << endl;
			cout << Test.ObjectID.SubPathString.ToString() << endl;
			TestObj = ((UKismetSystemLibrary*)UKismetSystemLibrary::StaticClass()->DefaultObject)->Conv_SoftObjectReferenceToObject(Test);

			cout << TestObj->GetName() << endl;

			auto foundation = StaticFindObject<ABuildingFoundation>("/Game/Athena/Apollo/Maps/Apollo_POI_Foundations.Apollo_POI_Foundations.PersistentLevel.LF_Athena_POI_19x19_2");
			CountdownTimer = StaticFindObject<UObject>("/Game/Athena/Apollo/Maps/Apollo_POI_Foundations.Apollo_POI_Foundations.PersistentLevel.BP_Countdown_Child_Jerky_2");

			cout << CountdownTimer->GetName() << endl;

			ShowFoundation(foundation);

			TArray<AActor*> BuildingFoundations;
			GetStatics()->GetAllActorsOfClass(GetWorld(), ABuildingFoundation::StaticClass(), &BuildingFoundations);

			for (size_t i = 0; i < BuildingFoundations.Num(); i++)
			{
				ABuildingFoundation* Foundation = (ABuildingFoundation*)BuildingFoundations[i];

				if (Foundation && Foundation->GetFullName().contains("Jerky_Head"))
				{
					ShowFoundation(Foundation);
				}
			}
			BuildingFoundations.FreeArray();
		}
	}

	if (!GetGameState() || !GetGameState()->MapInfo)
	{
		if (!bCreative)
			return false;
	}

	if (!Listening)
	{
		Listening = true;
		JerkyLoader = UObject::FindObject<UObject>("BP_Jerky_Loader_C JerkyLoaderLevel.JerkyLoaderLevel.PersistentLevel.BP_Jerky_Loader_2");

		FName GameNetDriver = Conv_StringToName(TEXT("GameNetDriver"));
		UNetDriver* Driver = CreateNetDriver(GetEngine(), GetWorld(), GameNetDriver);
		cout << Driver->GetName() << endl;
		Driver->World = GetWorld();
		Driver->NetDriverName = GameNetDriver;

		FString Error;
		FURL InURL = FURL();
		InURL.Port = Port;

		InitListen(Driver, GetWorld(), InURL, false, Error);
		SetWorld(Driver, GetWorld());

		GetWorld()->NetDriver = Driver;
		GetWorld()->LevelCollections[0].NetDriver = Driver;
		GetWorld()->LevelCollections[1].NetDriver = Driver;

		GameMode->bWorldIsReady = true;
		GetGameState()->AirCraftBehavior = GetGameState()->CurrentPlaylistInfo.BasePlaylist->AirCraftBehavior;
		GetGameState()->CachedSafeZoneStartUp = GetGameState()->CurrentPlaylistInfo.BasePlaylist->SafeZoneStartUp;
		GetGameState()->OnRep_CurrentPlaylistId();
		GetGameState()->OnRep_CurrentPlaylistInfo();
		UFortPlaylistAthena* Playlist = GetGameState()->CurrentPlaylistInfo.BasePlaylist;
		TArray<TSoftObjectPtr<UWorld>>& AdditionalLevels = *(TArray<TSoftObjectPtr<UWorld>>*)(__int64(Playlist) + GetOffset(Playlist, "AdditionalLevels"));
		TArray<TSoftObjectPtr<UWorld>>& AdditionalLevelsServerOnly = *(TArray<TSoftObjectPtr<UWorld>>*)(__int64(Playlist) + GetOffset(Playlist, "AdditionalLevelsServerOnly"));

		for (size_t i = 0; i < GetGameState()->MutatorListComponent->Mutators.Num(); i++)
		{
			cout << GetGameState()->MutatorListComponent->Mutators[i]->GetFullName() << endl;
		}

		UFortServerBotManagerAthena* BotManager = (UFortServerBotManagerAthena*)GetStatics()->SpawnObject(UFortServerBotManagerAthena::StaticClass(), GameMode);
		GameMode->ServerBotManager = BotManager;
		BotManager->CachedGameState = GetGameState();
		BotManager->CachedGameMode = GameMode;

		BotMutator = SpawnActor<AFortAthenaMutator_Bots>({});
		BotManager->CachedBotMutator = BotMutator;
		BotMutator->CachedGameMode = GameMode;
		BotMutator->CachedGameState = GetGameState();

		AAthenaAIDirector* Director = SpawnActor<AAthenaAIDirector>({});
		GameMode->AIDirector = Director;
		Director->Activate();

		AFortAIGoalManager* GoalManager = SpawnActor<AFortAIGoalManager>({});
		GameMode->AIGoalManager = GoalManager;

		BotsEnabled = BotMutator;
		Log("Streaming Playlist AdditionalLevels");
		for (size_t i = 0; i < AdditionalLevelsServerOnly.Num(); i++)
		{
			FVector Loc{};
			FRotator Rot{};
			bool Success = false;
			((ULevelStreamingDynamic*)ULevelStreamingDynamic::StaticClass()->DefaultObject)->LoadLevelInstanceBySoftObjectPtr(GetWorld(), AdditionalLevelsServerOnly[i], Loc, Rot, &Success, FString());
			FAdditionalLevelStreamed NewLevel{};
			NewLevel.bIsServerOnly = true;
			NewLevel.LevelName = AdditionalLevelsServerOnly[i].ObjectID.AssetPathName;
			GetGameState()->AdditionalPlaylistLevelsStreamed.Add(NewLevel);
		}

		for (size_t i = 0; i < AdditionalLevels.Num(); i++)
		{
			FVector Loc{};
			FRotator Rot{};
			bool Success = false;
			((ULevelStreamingDynamic*)ULevelStreamingDynamic::StaticClass()->DefaultObject)->LoadLevelInstanceBySoftObjectPtr(GetWorld(), AdditionalLevels[i], Loc, Rot, &Success, FString());
			FAdditionalLevelStreamed NewLevel{};
			NewLevel.bIsServerOnly = false;
			NewLevel.LevelName = AdditionalLevels[i].ObjectID.AssetPathName;
			GetGameState()->AdditionalPlaylistLevelsStreamed.Add(NewLevel);
		}

		GetGameState()->OnRep_AdditionalPlaylistLevelsStreamed();
		GetGameState()->OnFinishedStreamingAdditionalPlaylistLevel();
		Log("Finished streaming additional levels");

		bArsenal = Playlist->GetName() == "Playlist_Gg_Reverse";
		bStormKing = Playlist->GetName() == "Playlist_DADBRO_Squads";
		bBattleLab = Playlist->GetName() == "Playlist_BattleLab";
		bDropZone = Playlist->GetName() == "Playlist_Respawn_Op";
		bTeamRumble = Playlist->GetName() == "Playlist_Respawn_24";

		bEnableLooting = !bArsenal && !bStormKing && !bLateGame && !bDropZone && !bTravis;
		bEnableScoringSystem = bArsenal || bTeamRumble || bDropZone;

		bEnableSiphon = bLateGame || bArsenal;

		InitLooting();
		SpawnFloorLoot();
		SpawnVehicles();

		string cmd = "demorec " + SessionName;
		((UKismetSystemLibrary*)UKismetSystemLibrary::StaticClass()->DefaultObject)->ExecuteConsoleCommand(GetWorld(), wstring(cmd.begin(), cmd.end()).c_str(), nullptr);

		TArray<AActor*> BuildingContainers;

		GetStatics()->GetAllActorsOfClass(GetWorld(), ABuildingContainer::StaticClass(), &BuildingContainers);

		for (size_t i = 0; i < BuildingContainers.Num(); i++)
		{
			ABuildingContainer* Container = (ABuildingContainer*)BuildingContainers[i];

			if (Container)
			{
				string LootTierGroup = Conv_NameToString(Container->SearchLootTierGroup).ToString();
				
				Log(LootTierGroup);

				if (LootTierGroup == "Loot_Treasure")
				{
					LootTierGroup = "Loot_AthenaTreasure";
				}
				else if (LootTierGroup == "Loot_Ammo")
				{
					LootTierGroup = "Loot_AthenaAmmoLarge";
				}

				Container->SearchLootTierGroup = Conv_StringToName(wstring(LootTierGroup.begin(), LootTierGroup.end()).c_str());
			}
		}

		TArray<AActor*> VendingMachinesArray;

		GetStatics()->GetAllActorsOfClass(GetWorld(), UObject::FindObject<UClass>("BlueprintGeneratedClass B_Athena_VendingMachine.B_Athena_VendingMachine_C"), &VendingMachinesArray);

		for (size_t i = 0; i < VendingMachinesArray.Num(); i++)
		{
			VendingMachinesArray[i]->K2_DestroyActor();
		}

		VendingMachinesArray.FreeArray();

		Log("Destroyed Vending Machines");

		Log("Finished setting LootTierGroups, freeing array");
		BuildingContainers.FreeArray();
		
		//GetGameState()->EventTournamentRound = EEventTournamentRound::Arena;
		//GetGameState()->OnRep_EventTournamentRound();
		// 
		cout << Port << endl;
		
		SetConsoleTitleA(("Port: " + to_string(Port)).c_str());

		//EonApi::MarkServerOnline(Region, to_string(GameMode->GameSession->MaxPlayers), to_string(Port), SessionName, "Playlist_DADBRO_Squads");
		if (!CustomCode.contains("DevGame") && bLateGame) {
			//Region == "EU" ? CustomCode = "LateGame" : CustomCode = "LateGame2";
			CustomCode = "LateGame"; // Forced This But Should Be LateGame and LateGame2 on both (if wanting 3 games then LateGame3 ofc)

			/* Devers
			DevGame
			DevGame2
			DevGame3
			*/
		}
		//EonApi::MarkServerOnline(Region, to_string(GameMode->GameSession->MaxPlayers), to_string(Port), SessionName, "Playlist_DefaultSolo", CustomCode);
		
		// test new MMS


		// if notify mms AND it's NOT a staff game
		if (!bDontNotifyMMS || !bStaffGameOnly) {
			EonApi::SetupAPI();
			bool as = EonMMSAPI::MarkServerOnlinev2(Region, to_string(GameMode->GameSession->MaxPlayers), to_string(Port), SessionName, "Playlist_DefaultSolo", CustomCode);

			if (as == true) {
				printf("Server Marked 1\n");
			}

			bool ab = EonMMSAPI::SetServerStatus("online");

			if (ab == true) {
				printf("Server Marked 2\n");
			}
		}

		
		//EonApi::MarkServerOnlinev2(Region, to_string(GameMode->GameSession->MaxPlayers), to_string(Port), SessionName, "Playlist_DefaultSolo", CustomCode);
		//EonApi::SetServerStatus("online");
		//EonApi::MarkServerOnline(Region, to_string(GameMode->GameSession->MaxPlayers), to_string(Port), SessionName, GetGameState()->CurrentPlaylistInfo.BasePlaylist->PlaylistName.ToString(), CustomCode);
	}

	if (GetGameState()->PlayersLeft > 0)
	{
		return true;
	}
	else
	{
		float TimeSeconds = GetStatics()->GetTimeSeconds(GetWorld());

		GetGameState()->WarmupCountdownEndTime = TimeSeconds + Duration;
		GameMode->WarmupCountdownDuration = Duration;
		GetGameState()->WarmupCountdownStartTime = TimeSeconds;
		GameMode->WarmupEarlyCountdownDuration = Duration;
	}

	return false;
}

APawn* SpawnDefaultPawnFor(AFortGameMode* GM, AFortPlayerController* Player, AActor* StartSpot)
{
	FTransform transform = StartSpot->GetTransform();
	AFortPlayerPawnAthena* Ret = (AFortPlayerPawnAthena*)GM->SpawnDefaultPawnAtTransform(Player, transform);
	return Ret;
}

void (*OnAircraftExitedDropZoneOG)(AFortGameModeAthena* GM, AFortAthenaAircraft* Aircraft);
void OnAircraftExitedDropZone(AFortGameModeAthena* GM, AFortAthenaAircraft* Aircraft)
{
	if (bLateGame || bTravis)
	{
		for (size_t i = 0; i < GM->AlivePlayers.Num(); i++)
		{
			if (GM->AlivePlayers[i] && GM->AlivePlayers[i]->IsInAircraft())
			{
				GM->AlivePlayers[i]->GetAircraftComponent()->ServerAttemptAircraftJump(FRotator());
			}
		}
	}

	OnAircraftExitedDropZoneOG(GM, Aircraft);

	if (/*bTravis*/false)
	{
		UFunction* StartEventFunc = JerkyLoader->Class->GetFunction("BP_Jerky_Loader_C", "startevent");
		Log(StartEventFunc->GetName());
		float idk = 0.f;
		ProcessEvent(JerkyLoader, StartEventFunc, &idk);
		Log("Started Astronomical");
	}

	if (bStormKing)
	{
		StartDadBro();
	}
}

void(__fastcall* PreloginOG)(AFortGameModeAthena* GameMode, FString& Options, FString& Address, FUniqueNetIdRepl& UniqueId, FString& ErrorMessage);
void __fastcall Prelogin(AFortGameModeAthena* GameMode, FString& Options, FString& Address, FUniqueNetIdRepl& UniqueId, FString& ErrorMessage)
{
	return;
	if (bBattleLab)
		return;

	return PreloginOG(GameMode, Options, Address, UniqueId, ErrorMessage);
}

__int64 StartAircraftPhaseHook(AFortGameModeAthena* GM, char a2)
{
	cout << to_string(a2) << endl;
	//auto ret = StartAircraftPhase(GM, a2);
	//if (BotsEnabled)
	//	((UKismetSystemLibrary*)UKismetSystemLibrary::StaticClass()->DefaultObject)->ExecuteConsoleCommand(GetWorld(), TEXT("startaircraft"), nullptr);//1:1
	//return ret;
	return StartAircraftPhase(GM, a2);
}