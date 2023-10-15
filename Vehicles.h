#pragma once
#include "ue.h"

void SpawnVehicles()
{
	if (bLateGame || bArsenal)
		return;

	TArray<AActor*> Spawners;
	GetStatics()->GetAllActorsOfClass(GetWorld(), AFortAthenaVehicleSpawner::StaticClass(), &Spawners);

	for (size_t i = 0; i < Spawners.Num(); i++)
	{
		AFortAthenaVehicleSpawner* Spawner = (AFortAthenaVehicleSpawner*)Spawners[i];
		SpawnActor<AFortAthenaVehicle>(Spawner->GetVehicleClass(), Spawner->K2_GetActorLocation());
	}

	Spawners.FreeArray();
}

void (*ServerMoveOG)(AFortPhysicsPawn* Pawn, FReplicatedPhysicsPawnState InState);
void ServerMove(AFortPhysicsPawn* Pawn, FReplicatedPhysicsPawnState InState)
{
	UPrimitiveComponent* Mesh = (UPrimitiveComponent*)Pawn->RootComponent;

	InState.Rotation.X -= 2.5;
	InState.Rotation.Y /= 0.3;
	InState.Rotation.Z -= -2.0;
	InState.Rotation.W /= -1.2;

	FTransform Transform{};
	Transform.Translation = InState.Translation;
	Transform.Rotation = InState.Rotation;
	Transform.Scale3D = FVector{ 1, 1, 1 };

	Mesh->K2_SetWorldTransform(Transform, false, nullptr, true);
	Mesh->bComponentToWorldUpdated = true;
	Mesh->SetPhysicsLinearVelocity(InState.LinearVelocity, 0, FName());
	Mesh->SetPhysicsAngularVelocity(InState.AngularVelocity, 0, FName());
}