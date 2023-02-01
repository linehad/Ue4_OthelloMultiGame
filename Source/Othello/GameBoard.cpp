// Fill out your copyright notice in the Description page of Project Settings.


#include "GameBoard.h"

#include "ServerGameStateBase.h"
#include "OthelloPlayerController.h"
#include "OthelloPices_UserWidget.h"
#include "OthelloNativeClass.h"

#include "Kismet/GameplayStatics.h"

#include <Components/Button.h>
#include <Components/Image.h>
#include <Components/GridPanel.h>
#include <Components/UniformGridPanel.h>
#include "Animation/WidgetAnimation.h"

void UGameBoard::NativeConstruct()
{
	Super::NativeConstruct();

	// 텍스쳐 매핑
	BigFont.Empty();
	for (int i = 0; i < 10; i++)
	{
		FString a = ".";
		FString BigFontAddress = TEXT("/Game/Font/BigFont/") + FString::FromInt(i) + a + FString::FromInt(i);
		BigFont.Add(LoadObject<UTexture2D>(NULL, *BigFontAddress, NULL, LOAD_None, NULL));
	}

	Time_Image0->SetBrushFromTexture(BigFont[0], true);
	Time_Image1->SetBrushFromTexture(BigFont[0], true);

	White_Image0->SetBrushFromTexture(BigFont[2], true);
	White_Image1->SetBrushFromTexture(BigFont[0], true);

	Black_Image0->SetBrushFromTexture(BigFont[2], true);
	Black_Image1->SetBrushFromTexture(BigFont[0], true);


	boardSize = Cast<AServerGameStateBase>(GetWorld()->GetGameState())->GetBoardSize();
	limitTime = Cast<AServerGameStateBase>(GetWorld()->GetGameState())->GetLimitTime();

	seconds = limitTime;
	// 버튼에 정보 넘겨주고 함수와 묶기
	if (OthelloButton)
	{
		int arr_index = 0;

		arrOthelloButton.Init(nullptr, boardSize * boardSize);
		for (int i = 0; i < boardSize; i++)
		{
			for (int j = 0; j < boardSize; j++)
			{
				UUserWidget* widget = CreateWidget(this, OthelloButton);
				Board_UniformGridPanel->AddChildToUniformGrid(widget, i, j);
				arrOthelloButton[arr_index] = Cast<UOthelloPices_UserWidget>(widget);
				arrOthelloButton[arr_index]->SetData(i, j);
				arrOthelloButton[arr_index]->OthlloPices_Button->OnClicked.AddDynamic(this, &UGameBoard::NextTurn);
				arr_index++;
			}
		}
		// 시작 돌 놓기
		StartSet();
	}
}

void UGameBoard::SetTime(int time)
{
	Time_Image0->SetBrushFromTexture(BigFont[time % 10], true);
	Time_Image1->SetBrushFromTexture(BigFont[time / 10], true);
}

void UGameBoard::GameOver(int8 GameWinner)
{
	Quit_Button->OnClicked.AddDynamic(this, &UGameBoard::QuitGame);

	GameOver_GridPanel->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
	if (GameWinner == DRAW)
	{
		WinBlack_Image->SetVisibility(ESlateVisibility::HitTestInvisible);
		WinWhite_Image->SetVisibility(ESlateVisibility::HitTestInvisible);
	}
	else if (GameWinner == BLACK)
	{
		WinBlack_Image->SetVisibility(ESlateVisibility::HitTestInvisible);
	}
	else if (GameWinner == WHITE)
	{
		WinWhite_Image->SetVisibility(ESlateVisibility::HitTestInvisible);
	}

}

void UGameBoard::StartTimer()
{
	// 타이머 핸들러를 통해 시간을 업데이트한다.
	FTimerHandle timerHandle;
	GetWorld()->GetTimerManager().SetTimer(timerHandle, this, &UGameBoard::CountDown, 1.f, true, 0.0);
	seconds = limitTime;
}

void UGameBoard::CountDown()
{
	bool flag = false;
	if (seconds != 0)
	{
		seconds--;
		SetTime(seconds);
		PlayAnimation(Time_Image0_Anim);
	}
	if (seconds % 10 == 0)
	{
		if (seconds == 0)
		{
			OthelloChangeturn();
		}
		else
		{
			PlayAnimation(Time_Image1_Anim);
		}

	}
}

void UGameBoard::QuitGame()
{
	UKismetSystemLibrary::QuitGame(GetWorld(), GetWorld()->GetFirstPlayerController(), EQuitPreference::Quit, true);
}


void UGameBoard::NextTurn()
{

	Cast<AOthelloPlayerController>(GetWorld()->GetFirstPlayerController())->SetOthelloNextturn();
}

void UGameBoard::SetScore(int black_score, int white_score)
{
	// 점수 출력 부
	if (beforBlackScore % 10 != black_score % 10)
	{
		PlayAnimation(Black_Image0_Anim);
		Black_Image0->SetBrushFromTexture(BigFont[black_score % 10], true);
	}

	if (beforBlackScore / 10 != black_score / 10)
	{
		PlayAnimation(Black_Image1_Anim);
		Black_Image1->SetBrushFromTexture(BigFont[black_score / 10], true);
	}

	if (beforWhiteScore % 10 != white_score % 10)
	{
		PlayAnimation(White_Image0_Anim);
		White_Image0->SetBrushFromTexture(BigFont[white_score % 10], true);
	}
	if (beforWhiteScore / 10 != white_score / 10)
	{
		PlayAnimation(White_Image1_Anim);
		White_Image1->SetBrushFromTexture(BigFont[white_score / 10], true);
	}
}

void UGameBoard::StartSet()
{
	int y = boardSize / 2 - 1;
	int index = 0;
	for (int i = 0; i < 2; i++)
	{
		int x = (boardSize / 2) - 1;
		for (int j = 0; j < 2; j++)
		{
			index = Board_index(x, y);
			arrOthelloButton[index]->ReversePice();

			OthelloChangeturn();
			x++;
		}
		y++;
		OthelloChangeturn();
	}
	OthelloChangeturn();
}

void UGameBoard::OthelloChangeturn()
{
	int white_score = 0;
	int black_score = 0;
	OthelloNativeClass logic;
	seconds = limitTime;
	SetTime(seconds);

	// 버튼 초기화
	for (int i = 0; i < boardSize * boardSize; i++)
	{
		arrOthelloButton[i]->UnPossiblePice();
	}

	for (int i = 0; i < boardSize * boardSize; i++)
	{
		white_score = arrOthelloButton[i]->GetPice() == 2 ? ++white_score : white_score;
		black_score = arrOthelloButton[i]->GetPice() == 1 ? ++black_score : black_score;

		int32 pX = arrOthelloButton[i]->GetX();
		int32 pY = arrOthelloButton[i]->GetY();
		arrOthelloButton[i]->Changeturn();
		if (logic.PutOthello(pX, pY, arrOthelloButton[i]->GetTurn() + 1, boardSize, arrOthelloButton) && arrOthelloButton[i]->GetPice() == 0)
		{
			arrOthelloButton[i]->PossiblePice();
		}
	}
}