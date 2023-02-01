// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameStateBase.h"
#include "ServerGameStateBase.generated.h"

/**
 * 
 */
UCLASS()
class OTHELLO_API AServerGameStateBase : public AGameStateBase
{
	GENERATED_BODY()
private:
	UPROPERTY()
		int limitTime = 0;
	UPROPERTY()
		int boardSize = 0;
	const int ENTRY = 0;

	UPROPERTY()
		int beforeIndex = 0;
	UPROPERTY()
		bool bcheckTurn = false;

	int changeCount = 0;

public:

	UFUNCTION()
		int GetBoardSize() { return boardSize; }

	UFUNCTION()
		int GetLimitTime() { return limitTime; }
	UFUNCTION(NetMulticast, Reliable)
		void SetGameData(int size, int t);
	void SetGameData_Implementation(int size, int t);


	// 다음 턴이 되었을 때 실행 할 함수
	UFUNCTION()
		void OthelloNextturn(int othelloArrIndex);
};
