// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/EnemyAIController.h"
#include "Helpers.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Unit/Unit.h"

void AEnemyAIController::BeginPlay()
{
	Super::BeginPlay();
}

void AEnemyAIController::MakeTrueTargets(TArray<AActor*> Targets)
{
	const auto ControlledUnit = Cast<AUnit>(GetPawn());
	const auto EnemyTeam = UHelpers::GetEnemyTeam(ControlledUnit->GetTeam());
	
	TrueTargets = TArray<AActor*>();
	
	for (AActor* Target : Targets)
	{
		const auto Unit = Cast<AUnit>(Target);

		if (Unit == nullptr) continue;
		if (Unit->IsUnitDead()) continue;
		if (Unit->GetTeam() != EnemyTeam) continue;

		TrueTargets.AddUnique(Unit);
	}
}

AActor* AEnemyAIController::GetClosestTarget(TArray<AActor*> Targets) const
{
	if (Targets.Num() <= 0) return nullptr;

	const auto PawnLocation = GetPawn()->GetActorLocation();
	
	auto ClosestTarget = Targets[0];
	auto ClosestDistance = FVector::Dist(PawnLocation, ClosestTarget->GetActorLocation());

	for (const auto Target : Targets)
	{
		const auto Distance = FVector::Dist(PawnLocation, Target->GetActorLocation());
		if (Distance < ClosestDistance)
		{
			ClosestDistance = Distance;
			ClosestTarget = Target;
		}
	}
	return ClosestTarget;
}

void AEnemyAIController::SetBlackboardTarget(AActor* Target, UBlackboardComponent* BlackboardComponent)
{
	TargetActor = Target;
	BlackboardComponent->SetValueAsObject("Target", Target);
}

void AEnemyAIController::OnUpdatePerception(const TArray<AActor*> Targets, UBlackboardComponent* BlackboardComponent)
{
	MakeTrueTargets(Targets);

	if (TrueTargets.Num() <= 0)
	{
		SetBlackboardTarget(nullptr, BlackboardComponent);
		return;
	}

	if (TrueTargets.Contains(TargetActor) && TargetActor.IsValid())
	{
		SetBlackboardTarget(TargetActor.Get(), BlackboardComponent);
		return;
	}

	AActor* ClosestTarget = GetClosestTarget(TrueTargets);
	SetBlackboardTarget(ClosestTarget, BlackboardComponent);
}
