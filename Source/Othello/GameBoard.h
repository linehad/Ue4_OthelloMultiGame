// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "GameBoard.generated.h"

/**
 *
 */
UCLASS()
class OTHELLO_API UGameBoard : public UUserWidget
{
	GENERATED_BODY()
private:
	int boardSize = 0;
	int limitTime = 0;
	int beforWhiteScore = 0;
	int beforBlackScore = 0;

	int seconds = 0;

	const int DRAW = 0;
	const int BLACK = 1;
	const int WHITE = 2;
public:
	virtual void NativeConstruct() override;
	UPROPERTY()
		TArray <class UOthelloPices_UserWidget*>arrOthelloButton;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		TSubclassOf <class UOthelloPices_UserWidget>OthelloButton;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
		class UUniformGridPanel* Board_UniformGridPanel = nullptr;

	// �浹, �鵹 �̹���
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		UTexture2D* WhiteImage = nullptr;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		UTexture2D* BlackImage = nullptr;

	// �ð� �̹���
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
		class UImage* Time_Image0 = nullptr;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
		class UImage* Time_Image1 = nullptr;

	// �浹 ���� �̹���
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
		class UImage* Black_Image0 = nullptr;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
		class UImage* Black_Image1 = nullptr;

	// �鵹 ���� �̹���
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
		class UImage* White_Image0 = nullptr;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
		class UImage* White_Image1 = nullptr;

	// ���ӿ��� ���� ���ε� �̹���
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
		class UGridPanel* GameOver_GridPanel = nullptr;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
		class UImage* WinBlack_Image = nullptr;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
		class UImage* WinWhite_Image = nullptr;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
		class UButton* Quit_Button = nullptr;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		TArray<UTexture2D*> BigFont;

	// �ִϸ��̼� ���ε�
	UPROPERTY(BlueprintReadWrite, Transient, meta = (BindWidgetAnim))
		class UWidgetAnimation* Time_Image0_Anim = nullptr;
	UPROPERTY(BlueprintReadWrite, Transient, meta = (BindWidgetAnim))
		class UWidgetAnimation* Time_Image1_Anim = nullptr;

	UPROPERTY(BlueprintReadWrite, Transient, meta = (BindWidgetAnim))
		class UWidgetAnimation* Black_Image0_Anim = nullptr;
	UPROPERTY(BlueprintReadWrite, Transient, meta = (BindWidgetAnim))
		class UWidgetAnimation* Black_Image1_Anim = nullptr;

	UPROPERTY(BlueprintReadWrite, Transient, meta = (BindWidgetAnim))
		class UWidgetAnimation* White_Image0_Anim = nullptr;
	UPROPERTY(BlueprintReadWrite, Transient, meta = (BindWidgetAnim))
		class UWidgetAnimation* White_Image1_Anim = nullptr;


	// ������ �帧�� ����ϴ� �Լ���
public:
	// ó�� ������ �� ȣ���� �Լ�
	void StartSet();

	// ������ ���� ������ �Լ�
	UFUNCTION()
		void OthelloChangeturn();

	// 1���� �迭�� 2���� �迭 ó�� ���� ���� �Լ�
	UFUNCTION()
		inline int16 Board_index(int x, int y) { return (boardSize * y) + x; }

	// ������ ����ϴ� �Լ�
	UFUNCTION()
		void SetScore(int BScore, int WScore);
	UFUNCTION()
		void SetSeconds() { seconds = limitTime; }

	// �ð��� ���ϴ� �Լ�
	UFUNCTION()
		void SetTime(int Time);

	UFUNCTION()
		void CountDown();
	UFUNCTION()
		void StartTimer();

	// ������ ������ �� ȣ��� �Լ���
	UFUNCTION()
		void GameOver(int8 gameWinner); // 0 ���º� 1 �浹 �� 2 �鵹 ��
	UFUNCTION()
		void QuitGame();

	UFUNCTION()
		void NextTurn();

	UFUNCTION()
		void SetwhiteScore(int score) { beforWhiteScore = score; }
	UFUNCTION()
		void SetblackScore(int score) { beforBlackScore = score; }

	inline void SetBoardSize(int size) { boardSize = size; }
	inline int GetBoardSize() { return boardSize; }
};