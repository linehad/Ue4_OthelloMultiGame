// Fill out your copyright notice in the Description page of Project Settings.


#include "ServerGameStateBase.h"
#include "OthelloNativeClass.h"
#include "OthelloPlayerController.h"
#include "OthelloPices_UserWidget.h"
#include "GameBoard.h"

#include <Blueprint/UserWidget.h>
#include <Components/Image.h>

void AServerGameStateBase::OthelloNextturn(int othelloArrIndex)
{
	AOthelloPlayerController* Controller = Cast<AOthelloPlayerController>(GetWorld()->GetFirstPlayerController());
	UGameBoard* OthelloBoard = Cast<UGameBoard>(Controller->CurrentWidget);
	OthelloNativeClass logic;
	int white_score = 0;
	int black_score = 0;
	int PutCount = 0; // ���� �������� ����.
	int gameOver = 0;
	int8 winnner = 0;

	int black_count = 0;
	int white_count = 0;

	const int DRAW = 0;
	const int BLACK = 1;
	const int WHITE = 2;

	OthelloBoard->SetTime(limitTime);

	// ��ư �ʱ�ȭ
	for (int i = 0; i < boardSize * boardSize; i++)
	{
		OthelloBoard->arrOthelloButton[i]->UnPossiblePice();
		black_count = OthelloBoard->arrOthelloButton[i]->GetPice() == BLACK ? ++black_count : black_count;
		white_count = OthelloBoard->arrOthelloButton[i]->GetPice() == WHITE ? ++white_count : white_count;
	}

	if (black_count == 0)
	{
		OthelloBoard->GameOver(2);
	}
	else if (white_count == 0)
	{
		OthelloBoard->GameOver(1);
	}

	// ��ư�� ã�� �����´�.
	OthelloBoard->arrOthelloButton[othelloArrIndex]->OnBtnClick();
	logic.ChangePices(OthelloBoard->arrOthelloButton[othelloArrIndex]->GetX(),
		OthelloBoard->arrOthelloButton[othelloArrIndex]->GetY(), OthelloBoard->arrOthelloButton[othelloArrIndex]->GetPice(), boardSize, OthelloBoard->arrOthelloButton);

	// Ŭ�� ���϶��� �������� �����... ���� ���� hit�� �ʱ�ȭ �ϴ°� �ƴ϶� ������ ������ �ʱ�ȭ �ؾ���...
	if (GetWorld()->GetFirstPlayerController()->GetLocalRole() == ROLE_Authority)
	{
		if (bcheckTurn)
		{
			OthelloBoard->arrOthelloButton[beforeIndex]->bhit = false;
			bcheckTurn = false;
		}
		else
		{
			bcheckTurn = true;
		}
	}
	beforeIndex = othelloArrIndex;
	if (GetWorld()->GetFirstPlayerController()->GetRemoteRole() == ROLE_Authority)
	{
		OthelloBoard->arrOthelloButton[othelloArrIndex]->bhit = false;
	}

	// ������ �ٲٰ�, ���� �� �ִ� ��ġ�� �ٽ� ǥ���ϸ� ������ ����.
	// ���� �� �� �ִ� ��ġ�� �ִ��� üũ�ϰ� ������ ���� �ѱ��.
	// ������ �������� üũ�Ѵ�.
	for (int i = 0; i < boardSize * boardSize; i++)
	{
		white_score = OthelloBoard->arrOthelloButton[i]->GetPice() == WHITE ? ++white_score : white_score;
		black_score = OthelloBoard->arrOthelloButton[i]->GetPice() == BLACK ? ++black_score : black_score;
		gameOver = OthelloBoard->arrOthelloButton[i]->GetPice() == 0 ? ++gameOver : gameOver;

		int32 Px = OthelloBoard->arrOthelloButton[i]->GetX();
		int32 Py = OthelloBoard->arrOthelloButton[i]->GetY();
		OthelloBoard->arrOthelloButton[i]->Changeturn();
		// �ش� ������ �Ѽ� �ִ��� �������� üũ�Ѵ�.
		if (logic.PutOthello(Px, Py, OthelloBoard->arrOthelloButton[i]->GetTurn() + 1, boardSize, OthelloBoard->arrOthelloButton)
			&& OthelloBoard->arrOthelloButton[i]->GetPice() == 0)
		{
			// �ش� ��ġ�� �ξ����� ������ �� �ִ� ���� ������ ��ġ
			OthelloBoard->arrOthelloButton[i]->PossiblePice();
			PutCount++;
		}
	}


	OthelloBoard->SetScore(black_score, white_score);
	OthelloBoard->SetblackScore(black_score);
	OthelloBoard->SetwhiteScore(white_score);

	if (changeCount > 2)
	{
		winnner = black_score == white_score ? DRAW : (black_score > white_score) ? BLACK : WHITE;
		OthelloBoard->GameOver(winnner);
	}

	if (gameOver == 0)
	{
		// ������ 0 �������� �̱�� 1 ����� �̱�� 2
		winnner = black_score == white_score ? DRAW : (black_score > white_score) ? BLACK : WHITE;
		OthelloBoard->GameOver(winnner);
	}
	// ���̻� �������� ���ٸ� ���� �����´�.
	else if (PutCount == 0)
	{
		changeCount++;
		OthelloBoard->OthelloChangeturn();
	}
}

void AServerGameStateBase::SetGameData_Implementation(int size, int t)
{
	limitTime = t;
	boardSize = size;
}