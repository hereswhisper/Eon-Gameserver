#pragma once
#include "ue.h"

FFortUpdatedObjectiveStat& GetStat(AFortPlayerControllerAthena* PC, UFortQuestItemDefinition* QuestDef, bool& NeedsToAdd)
{
    FFortUpdatedObjectiveStat Stat{};
    for (size_t i = 0; i < PC->UpdatedObjectiveStats.Num(); i++)
    {
        if (PC->UpdatedObjectiveStats[i].Quest == QuestDef)
        {
            NeedsToAdd = false;
            return PC->UpdatedObjectiveStats[i];
        }
    }
    NeedsToAdd = true;
    return Stat;
}

void ProgressQuest(AFortPlayerControllerAthena* PC, UFortQuestItemDefinition* QuestDef, FName Primary_BackendName)
{
    auto QuestManager = PC->GetQuestManager(ESubGame::Athena);
    UFortQuestItem* QuestItem = QuestManager->GetQuestWithDefinition(QuestDef);
    cout << QuestDef->GetName() << endl;

    if (!QuestItem)
        return;

    cout << QuestItem->GetNumObjectivesComplete() << endl;
    cout << QuestItem->GetPercentageComplete() << endl;
    cout << QuestItem->CurrentStage << endl;

    QuestManager->SelfCompletedUpdatedQuest(PC, QuestDef, Primary_BackendName, 1, QuestItem->GetNumObjectivesComplete(), nullptr, true, false);
    
    bool FinishedQuest = QuestItem->GetPercentageComplete() >= 100.f;
    if (FinishedQuest)
    {
        QuestManager->ClaimQuestReward(QuestItem);
    }
}