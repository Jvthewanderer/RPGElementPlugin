// Fill out your copyright notice in the Description page of Project Settings.


#include "GLWC_HitText.h"
#include "Blueprint/UserWidget.h"
#include "UI/TLDamageTextWidget.h"

UGLWC_HitText::UGLWC_HitText()
{
	SetWidgetClass((TSubclassOf<UUserWidget>)PopupTextWidgetClass);
}

void UGLWC_HitText::SetDamageText(float Delta)
{
	if (UTLDamageTextWidget* PUText = Cast<UTLDamageTextWidget>(GetWidget()))
	{
		PUText->SetDamageText(Delta);
	}

}

