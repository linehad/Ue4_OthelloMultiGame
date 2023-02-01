// Fill out your copyright notice in the Description page of Project Settings.


#include "OthelloPlayerController.h"
#include "ServerGameStateBase.h"
#include "GameBoard.h"
#include "OthelloPices_UserWidget.h"

#include <Blueprint/UserWidget.h>
#include <Components/Image.h>

void AOthelloPlayerController::BeginPlay()
{
	Super::BeginPlay();

	bShowMouseCursor = true;
	// SetInputMode�� FInputModeGameAndUI�� �־�θ� ���Ӱ� UI��� �Է��� ������ ���°� �ȴ�.
	SetInputMode(FInputModeGameAndUI());
}

void AOthelloPlayerController::SetOthelloArrIndex_Implementation(int x, int y)
{
	othelloArrIndex = (Cast<AServerGameStateBase>(GetWorld()->GetGameState())->GetBoardSize() * y) + x;
}

void AOthelloPlayerController::SetOthelloNextturn_Implementation()
{
	AOthelloPlayerController* ServerController = Cast<AOthelloPlayerController>(GetWorld()->GetFirstPlayerController());
	UGameBoard* OthelloBoard = Cast<UGameBoard>(ServerController->CurrentWidget);

	for (auto Iter = GetWorld()->GetPlayerControllerIterator(); Iter; ++Iter) {
		AOthelloPlayerController* Controller = Cast<AOthelloPlayerController>(Iter->Get());
		if (Controller) {
			Controller->OthelloNextturn(othelloArrIndex);
		}
	}
}

void AOthelloPlayerController::OthelloNextturn_Implementation(int ButtonIndex)
{
	AServerGameStateBase* OthelloState = Cast<AServerGameStateBase>(GetWorld()->GetGameState());
	UGameBoard* OthelloBoard = Cast<UGameBoard>(CurrentWidget);
	OthelloBoard->SetSeconds();
	OthelloBoard->SetTime(OthelloState->GetLimitTime());

	OthelloState->OthelloNextturn(ButtonIndex);
}

void AOthelloPlayerController::ChangeWidget_Implementation(TSubclassOf<UUserWidget> NewWidgetClass)
{
	if (CurrentWidget != nullptr)
	{
		CurrentWidget->RemoveFromViewport();
		CurrentWidget = nullptr;
	}

	if (NewWidgetClass != nullptr)
	{
		CurrentWidget = CreateWidget(GetWorld(), NewWidgetClass);
		if (CurrentWidget != nullptr)
		{
			CurrentWidget->AddToViewport();
		}
	}

	if (CurrentWidget != nullptr && CurrentWidget == Cast<UGameBoard>(CurrentWidget))
	{
		Cast<UGameBoard>(CurrentWidget)->StartTimer();
	}
}