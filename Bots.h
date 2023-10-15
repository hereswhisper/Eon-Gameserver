#pragma once
#include "ue.h"
#include <intrin.h>

vector<class Bot*> Bots{};

class Bot
{
public:
	ABP_PhoebePlayerController_C* PC;
	AFortPlayerPawnAthena* Pawn;
	bool bTickEnabled = false;
	AActor* CurrentTarget = nullptr;

public:
	Bot(AFortPlayerPawnAthena* Pawn)
	{
		this->Pawn = Pawn;
		PC = (ABP_PhoebePlayerController_C*)Pawn->Controller;
		Bots.push_back(this);
	}

public:
	FGuid GetItem(UFortItemDefinition* Def)
	{
		for (size_t i = 0; i < PC->Inventory->Inventory.ReplicatedEntries.Num(); i++)
		{
			if (PC->Inventory->Inventory.ReplicatedEntries[i].ItemDefinition == Def)
				return PC->Inventory->Inventory.ReplicatedEntries[i].ItemGuid;
		}
		return FGuid();
	}

	void GiveItem(UFortItemDefinition* Def, int Count = 1)
	{
		UFortWorldItem* Item = (UFortWorldItem*)Def->CreateTemporaryItemInstanceBP(Count, 0);
		Item->OwnerInventory = PC->Inventory;

		if (Def->IsA(UFortWeaponRangedItemDefinition::StaticClass()))
		{
			Item->ItemEntry.LoadedAmmo = GetAmmoForDef((UFortWeaponItemDefinition*)Def);
		}

		PC->Inventory->Inventory.ReplicatedEntries.Add(Item->ItemEntry);
		PC->Inventory->Inventory.ItemInstances.Add(Item);
		PC->Inventory->Inventory.MarkItemDirty(Item->ItemEntry);
		PC->Inventory->HandleInventoryLocalUpdate();

		if (Def->IsA(UFortWeaponRangedItemDefinition::StaticClass()))
		{
			Pawn->EquipWeaponDefinition((UFortWeaponItemDefinition*)Def, GetItem(Def));
		}
	}

	// this was a failed attempt.
	/*virtual void OnPerceptionSensed(AActor* SourceActor, FAIStimulus& Stimulus)
	{
		if (Stimulus.bSuccessfullySensed == 1) {
			FHitResult HitResult;
			FVector StartLocation = this->Pawn->K2_GetActorLocation();
			FVector EndLocation = SourceActor->K2_GetActorLocation();

			if (!PC->LineOfSightTo(SourceActor, FVector(), true) && Pawn->GetDistanceTo(SourceActor) < 5000)
			{
				// No obstacles in the way, the AI bot can "see" the source actor
				CurrentTarget = SourceActor;
				cout << "bSuccessfullySensed" << endl;
			}
		}

		if (Stimulus.bSuccessfullySensed == 1 && PC->LineOfSightTo(SourceActor, FVector(), true) && Pawn->GetDistanceTo(SourceActor) < 5000)
		{
			CurrentTarget = SourceActor;
			cout << "bSuccessfullySensed" << endl;
		}
	}*/


	// OLD 
	virtual void OnPerceptionSensed(AActor* SourceActor, FAIStimulus& Stimulus)
	{
		if (Stimulus.bSuccessfullySensed == 1 && PC->LineOfSightTo(SourceActor, FVector(), true) && Pawn->GetDistanceTo(SourceActor) < 5000)
		{
			CurrentTarget = SourceActor;
			cout << "bSuccessfullySensed" << endl;
		}
	}
};
//UWorld
//TArray

// will bot shoot
int RandomNumGen() {
	std::srand(static_cast<unsigned int>(std::time(nullptr)));
	int randomNumber = (std::rand() % 26) + 1;
	//std::cout << "Random number between 1 and 3: " << randomNumber << std::endl;
	return randomNumber;
}

void TickBots()
{
	//AFortGameStateAthena
	for (auto bot : Bots)
	{
		if (bot->bTickEnabled && bot->Pawn && !bot->Pawn->bIsDBNO && bot->CurrentTarget)
		{
			if (bot->Pawn->CurrentWeapon && bot->Pawn->CurrentWeapon->WeaponData->IsA(UFortWeaponRangedItemDefinition::StaticClass()))
			{
				auto BotPos = bot->Pawn->K2_GetActorLocation();
				auto TargetPos = bot->CurrentTarget->K2_GetActorLocation();
				float Distance = bot->Pawn->GetDistanceTo(bot->CurrentTarget);
				//cout << Distance << endl;

				if (Distance > 800)
				{
					bot->PC->MoveToActor(bot->CurrentTarget, 0, true, false, true, nullptr, true);
				}
				else
				{
					bot->PC->StopMovement();
					if (!bot->Pawn->bIsCrouched && GetMath()->RandomBoolWithWeight(0.05f))
					{
						bot->Pawn->Crouch(false);
					}
					else if (bot->Pawn->bIsCrouched)
					{
						bot->Pawn->UnCrouch(false);
					}
				}

				int WillShoot = RandomNumGen();

				if (WillShoot == 1) {
					printf("[EON AI]: Can Hit player!\n");

					float RandomYaw = GetMath()->RandomFloatInRange(-20.0f, 20.0f);
					float RandomPitch = GetMath()->RandomFloatInRange(-10.0f, 10.0f);

					FVector TargetPosTest{ TargetPos.X + RandomPitch, TargetPos.Y + RandomYaw, TargetPos.Z };

					auto TestRot = GetMath()->FindLookAtRotation(BotPos, TargetPosTest);
					bot->PC->SetControlRotation(TestRot);
					bot->Pawn->PawnStartFire(0);

				}
				else {

					float RandomYaw = GetMath()->RandomFloatInRange(-180.0f, 180.0f);
					float RandomPitch = GetMath()->RandomFloatInRange(-90.0f, 90.0f);

					FVector TargetPosFake{ TargetPos.X + RandomPitch, TargetPos.Y + RandomYaw, TargetPos.Z };
					auto TestRot = GetMath()->FindLookAtRotation(BotPos, TargetPosFake);
					bot->PC->SetControlRotation(TestRot);
					bot->Pawn->PawnStartFire(0);
				}
			}
		}
		else if (bot->bTickEnabled && bot->Pawn && bot->Pawn->bIsDBNO)
		{
			bot->CurrentTarget = nullptr;
			bot->PC->StopMovement();
		}
	}
}

wchar_t* (*OnPerceptionSensedOG)(ABP_PhoebePlayerController_C* PC, AActor* SourceActor, FAIStimulus& Stimulus);
wchar_t* OnPerceptionSensed(ABP_PhoebePlayerController_C* PC, AActor* SourceActor, FAIStimulus& Stimulus)
{
	if (SourceActor->IsA(AFortPlayerPawnAthena::StaticClass()) && Cast<AFortPlayerPawnAthena>(SourceActor)->Controller && !Cast<AFortPlayerPawnAthena>(SourceActor)->Controller->IsA(ABP_PhoebePlayerController_C::StaticClass()) /*!Cast<AFortPlayerPawnAthena>(SourceActor)->Controller->IsA(ABP_PhoebePlayerController_C::StaticClass())*/)
	{
		for (auto bot : Bots)
		{
			if (bot->PC == PC)
			{
				bot->OnPerceptionSensed(SourceActor, Stimulus);
			}
		}
	}
	return OnPerceptionSensedOG(PC, SourceActor, Stimulus);
}

void (*OnPossessedPawnDiedOG)(ABP_PhoebePlayerController_C* PC, AActor* DamagedActor, float Damage, AController* InstigatedBy, AActor* DamageCauser, FVector HitLocation, UPrimitiveComponent* HitComp, FName BoneName, FVector Momentum);
void OnPossessedPawnDied(ABP_PhoebePlayerController_C* PC, AActor* DamagedActor, float Damage, AController* InstigatedBy, AActor* DamageCauser, FVector HitLocation, UPrimitiveComponent* HitComp, FName BoneName, FVector Momentum)
{
	for (size_t i = 0; i < PC->Inventory->Inventory.ReplicatedEntries.Num(); i++)
	{
		if (PC->Inventory->Inventory.ReplicatedEntries[i].ItemDefinition->IsA(UFortWeaponMeleeItemDefinition::StaticClass()) || PC->Inventory->Inventory.ReplicatedEntries[i].ItemDefinition->IsA(UFortAmmoItemDefinition::StaticClass()))
			continue;
		SpawnPickup(PC->Pawn->K2_GetActorLocation(), &PC->Inventory->Inventory.ReplicatedEntries[i], EFortPickupSourceTypeFlag::Other, EFortPickupSpawnSource::PlayerElimination);
		if (PC->Inventory->Inventory.ReplicatedEntries[i].ItemDefinition->IsA(UFortWeaponRangedItemDefinition::StaticClass()))
		{
			UFortWeaponRangedItemDefinition* Def = (UFortWeaponRangedItemDefinition*)PC->Inventory->Inventory.ReplicatedEntries[i].ItemDefinition;
			SpawnPickup(PC->Pawn->K2_GetActorLocation(), Def->GetAmmoWorldItemDefinition_BP(), GetAmmoForDef(Def) * 2, GetAmmoForDef(Def), EFortPickupSourceTypeFlag::Other, EFortPickupSpawnSource::PlayerElimination);
		}
	}

	for (size_t i = 0; i < Bots.size(); i++)
	{
		auto bot = Bots[i];
		if (bot && bot->PC == PC)
		{
			Bots.erase(Bots.begin() + i);
			break;
		}
	}

	return OnPossessedPawnDiedOG(PC, DamagedActor, Damage, InstigatedBy, DamageCauser, HitLocation, HitComp, BoneName, Momentum);
}

AFortPlayerPawnAthena* (*SpawnBotOG)(UFortServerBotManagerAthena* BotManager, FVector SpawnLoc, FRotator SpawnRot, UFortAthenaAIBotCustomizationData* BotData, FFortAthenaAIBotRunTimeCustomizationData RuntimeBotData);
AFortPlayerPawnAthena* SpawnBot(UFortServerBotManagerAthena* BotManager, FVector SpawnLoc, FRotator SpawnRot, UFortAthenaAIBotCustomizationData* BotData, FFortAthenaAIBotRunTimeCustomizationData RuntimeBotData)
{
	if (__int64(_ReturnAddress()) - __int64(GetModuleHandleW(0)) == 0x1A4153F)
		return SpawnBotOG(BotManager, SpawnLoc, SpawnRot, BotData, RuntimeBotData);

	cout << BotData->GetFullName() << endl;

	if (BotData->GetFullName().contains("MANG_POI_Yacht"))
	{
		BotData = StaticLoadObject<UFortAthenaAIBotCustomizationData>("/Game/Athena/AI/MANG/BotData/BotData_MANG_POI_HDP.BotData_MANG_POI_HDP");
	}

	if (BotData->CharacterCustomization->CustomizationLoadout.Character->GetName() == "CID_556_Athena_Commando_F_RebirthDefaultA")
	{
		BotData->CharacterCustomization->CustomizationLoadout.Character = StaticLoadObject<UAthenaCharacterItemDefinition>("/Game/Athena/Items/Cosmetics/Characters/CID_NPC_Athena_Commando_M_HenchmanGood.CID_NPC_Athena_Commando_M_HenchmanGood");
	}

	AActor* SpawnLocator = SpawnActor<ADefaultPawn>(SpawnLoc, SpawnRot);
	AFortPlayerPawnAthena* Ret = BotMutator->SpawnBot(BotData->PawnClass, SpawnLocator, SpawnLoc, SpawnRot, true);
	AFortAthenaAIBotController* PC = (AFortAthenaAIBotController*)Ret->Controller;
	PC->CosmeticLoadoutBC = BotData->CharacterCustomization->CustomizationLoadout;
	for (size_t i = 0; i < BotData->CharacterCustomization->CustomizationLoadout.Character->HeroDefinition->Specializations.Num(); i++)
	{
		UFortHeroSpecialization* Spec = StaticLoadObject<UFortHeroSpecialization>(Conv_NameToString(BotData->CharacterCustomization->CustomizationLoadout.Character->HeroDefinition->Specializations[i].ObjectID.AssetPathName).ToString());

		if (Spec)
		{
			for (size_t i = 0; i < Spec->CharacterParts.Num(); i++)
			{
				UCustomCharacterPart* Part = StaticLoadObject<UCustomCharacterPart>(Conv_NameToString(Spec->CharacterParts[i].ObjectID.AssetPathName).ToString());
				Ret->ServerChoosePart(Part->CharacterPartType, Part);
			}
		}
	}

	Ret->CosmeticLoadout = BotData->CharacterCustomization->CustomizationLoadout;
	Ret->OnRep_CosmeticLoadout();

	cout << BotData->CharacterCustomization->CustomizationLoadout.Character->GetName() << endl;

	SpawnLocator->K2_DestroyActor();
	DWORD CustomSquadId = RuntimeBotData.CustomSquadId;
	BYTE TrueByte = 1;
	BYTE FalseByte = 0;
	BotManagerSetupStuffIdk(__int64(BotManager), __int64(Ret), __int64(BotData->BehaviorTree), 0, &CustomSquadId, 0, __int64(BotData->StartupInventory), __int64(BotData->BotNameSettings), 0, &FalseByte, 0, &TrueByte, RuntimeBotData);

	Bot* bot = new Bot(Ret);

	for (size_t i = 0; i < BotData->StartupInventory->Items.Num(); i++)
	{
		bot->GiveItem(BotData->StartupInventory->Items[i]);
		if (auto Data = Cast<UFortWeaponItemDefinition>(BotData->StartupInventory->Items[i]))
		{
			if (Data->GetAmmoWorldItemDefinition_BP() && Data->GetAmmoWorldItemDefinition_BP() != Data)
			{
				bot->GiveItem(Data->GetAmmoWorldItemDefinition_BP(), 99999);
			}
		}
	}
	bot->bTickEnabled = true;
	return Ret;
}
