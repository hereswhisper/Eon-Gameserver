#pragma once
#include "ue.h"
#include "Looting.h"

void SpawnFloorLoot()
{
	TArray<AActor*> FloorLootSpawners;
	UClass* SpawnerClass = StaticLoadObject<UClass>("/Game/Athena/Environments/Blueprints/Tiered_Athena_FloorLoot_Warmup.Tiered_Athena_FloorLoot_Warmup_C");
	GetStatics()->GetAllActorsOfClass(GetWorld(), SpawnerClass, &FloorLootSpawners);

	for (size_t i = 0; i < FloorLootSpawners.Num(); i++)
	{
		FloorLootSpawners[i]->K2_DestroyActor();
	}

	FloorLootSpawners.FreeArray();

	SpawnerClass = StaticLoadObject<UClass>("/Game/Athena/Environments/Blueprints/Tiered_Athena_FloorLoot_01.Tiered_Athena_FloorLoot_01_C");
	GetStatics()->GetAllActorsOfClass(GetWorld(), SpawnerClass, &FloorLootSpawners);

	for (size_t i = 0; i < FloorLootSpawners.Num(); i++)
	{
		FloorLootSpawners[i]->K2_DestroyActor();
	}

	FloorLootSpawners.FreeArray();
}