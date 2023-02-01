// Copyright Epic Games, Inc. All Rights Reserved.


#include "OthelloGameModeBase.h"
#include "OthelloPlayerController.h"
#include "Kismet/GameplayStatics.h"
#include <Blueprint/UserWidget.h>

void AOthelloGameModeBase::BeginPlay()
{
	Super::BeginPlay();
}

void AOthelloGameModeBase::PostLogin(APlayerController* NewPlayer)
{
	Super::PostLogin(NewPlayer);

		AOthelloPlayerController* Controller = Cast<AOthelloPlayerController>(NewPlayer);

		if (Controller)
		{
			Controller->ChangeWidget(StartingWidgetClass);
		}
		if(GetWorld()->GetNumPlayerControllers() == 2)
		{		
			Cast<AOthelloPlayerController>(GetWorld()->GetFirstPlayerController())->ChangeWidget(MainWidgetClass);
		}
}