// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ComboWidget.generated.h"

/**
 * 
 */
UCLASS()
class BEATSABER_API UComboWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditAnywhere, meta=(BindWidget))
	class UTextBlock* currCombo_txt;
};
