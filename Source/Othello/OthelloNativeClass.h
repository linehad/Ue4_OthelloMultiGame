// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

/**
 * 
 */
class OTHELLO_API OthelloNativeClass
{
private:
	UPROPERTY()
		TArray<int8> arr_dX = { -1, -1, -1, 0, 0 , 1, 1, 1 };
	UPROPERTY()
		TArray<int8> arr_dY = { -1, 0, 1, -1, 1 , -1, 0, 1 };
	const int DIRECTUIN = 8;
public:
	// ���� ���� ������ �Լ�
	UFUNCTION()
		void ChangePices(int x, int y, int8 piceColor, int boardSize, TArray <class UOthelloPices_UserWidget*>arrOthelloButton);
	// �Ѽ� �ִ� ��ġ�� ǥ���ϴ� �Լ�
	UFUNCTION()
		bool PutOthello(int x, int y, int8 piceColor, int boardSize, TArray <class UOthelloPices_UserWidget*>arrOthelloButton);
	// 1���� �迭�� 2���� �迭 ó�� ���� ���� �Լ�
	UFUNCTION()
		inline int16 Board_index(int x, int y, int boardSize) { return (boardSize * y) + x; }
};
