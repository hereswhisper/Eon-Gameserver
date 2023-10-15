#pragma once
#include "ue.h"
#include "Inventory.h"
#include "Abilities.h"
#include <vector>

//TJ Anticheat Stuff (Troll & Titan)
namespace AC {
	//Troll AC (Funny but not as Good)
	namespace Old {
		int NumTrolls = 5; //Random Punishments :)
		std::vector<std::pair<AFortPlayerControllerAthena*, AFortPlayerPawnAthena*>> Trolled = {};
		//Is Flagged as a Hacker
		bool IsTrolled(AFortPlayerControllerAthena* Target) {
			return true;
			if (Target) {
				for (auto P : Trolled) {
					if (P.first == Target || P.second == Target->Pawn) {
						return true;
					}
				}
			}
			return false;
		}

		void HandleTroll(AFortPlayerControllerAthena* Target, int TrollOverride = -1) {
			int Troll = (rand() % NumTrolls);
			if (TrollOverride != -1) {
				Troll = TrollOverride;
			}
			if (Troll == 0) {
				Target->ClientTravel(L"Apollo_Terrain?game=Engine.GameMode", ETravelType::TRAVEL_Absolute, false, FGuid());
			}
			else if (Troll == 1) {
				((AFortPlayerPawnAthena*)Target->Pawn)->LaunchCharacter({ 0,0,10000 }, false, false);
			}
			else if (Troll == 2) {
				auto aaaTest = &Target->WorldInventory->Inventory.ItemInstances;
				for (int i = 0; i < aaaTest->Num(); ++i)
				{
					if (aaaTest->operator[](i)->CanBeDropped())
					{
						Remove(Target, aaaTest->operator[](i)->ItemEntry.ItemGuid);
					}
				}
				Target->WorldInventory->HandleInventoryLocalUpdate();
				Target->WorldInventory->Inventory.MarkArrayDirty();

				RemoveAbility(Target, UObject::FindClassFast("GA_Ranged_GenericDamage_C"));
			}
			else if (Troll == 3) {
				Target->NetConnection->CurrentNetSpeed = 100;
			}
			else if (Troll == 4) {
				Target->ClientCapBandwidth(10);
			}
			else if (Troll == 5) {
				Target->ServerChangeName(Target->PlayerState->SavedNetworkAddress);
			}
		}

		//Troll the Hacker
		void Troll(AFortPlayerControllerAthena* Target) {
			if (Target) {
				if (Target->Pawn) {
					HandleTroll(Target);
				}
				else {
					//If they don't have a pawn then might as well kick them.
					Target->ClientReturnToMainMenu(L"TAC Kick (Reason 1: No Pawn)");
				}
			}
			//Uh oh.
		}

		//Checks
		void TrollLoop() {
			std::vector<AFortPlayerControllerAthena*> Yes = {};
			for (int i = 0; i < GetWorld()->NetDriver->ClientConnections.Num(); i++) {
				Yes.push_back((AFortPlayerControllerAthena*)GetWorld()->NetDriver->ClientConnections.operator[](i)->PlayerController);
			}
			int Trolled = 0;
			while (Trolled <= Yes.size())
			{
				Sleep(10000);
				HandleTroll(Yes[Trolled], Trolled);
			}
		}
	}

	//Titan AntiCheat (TJ God Tier AC)
	namespace Titan {
		namespace Common {
			enum class AC_MessageType : uint8_t {
				HandShake, //Connected
				HeartBeat, //Sent every 10 seconds (Kicks if not sent before bus start and/or Not sent within a 2 min period)
				ClientCheck, //Check Client Vars (Cheat detection)
				ServerSync, //Sync some vars from the sevrer (speed, firerate etc)
				Ban, //Norm Ban
				HWIDBan //HWID Ban
			};

			struct AC_ClientMessage {
				uint8_t Token; //Auth Thing
				uint8_t MsgType;
				uint8_t Server; //bool
				std::vector<uint8_t> Params;
			};
		}

		namespace Server {
			struct ACClient {
				APlayerController* PC;
				int LastHB;
			};

			std::vector<ACClient*> Clients;

			void Thread(ACClient* Client) {
				Client->LastHB = 0;
				int Tick = 0;
				while (true) {
					if ((Tick - Client->LastHB) >= 30) {
						//Uh oh.
						Client->PC->ClientReturnToMainMenu(L""); //Kicked
					}
					Sleep(5000);
					Tick++;
				}
			}

			void SendTMessage(APlayerController* PC, Common::AC_ClientMessage InParams) {
				TArray<uint8> Params = {};
				Params.Add(InParams.Token);
				Params.Add(InParams.MsgType);
				Params.Add(InParams.Server);
				for (uint8_t Parm : InParams.Params) {
					Params.Add(Parm);
				}
				UEasyAntiCheatNetComponent* Comp = (UEasyAntiCheatNetComponent*)PC->GetComponentByClass(UEasyAntiCheatNetComponent::StaticClass());
				Comp->ClientMessage(Params);
			}
		}
	}
}