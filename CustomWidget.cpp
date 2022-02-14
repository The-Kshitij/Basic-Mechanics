#include "CustomWidget.h"
#include "Components/TextBlock.h"
#include "Components/ProgressBar.h"

bool UCustomWidget::Initialize()
{
	bool bSuccess = Super::Initialize();
	if (bSuccess)
	{
		if (TextAmmo == nullptr || TextMag == nullptr)
		{
			bSuccess = false;
		}
		UE_LOG(LogTemp,Warning,TEXT("(CustomWidget)Initialization of CustomWidget successfull"));
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("(CustomWidget)Initialization of CustomWidget failed"));
	}
	return bSuccess;
}

void UCustomWidget::SetTextAmmo(int val)
{
	if (TextAmmo == nullptr) return;

	TextAmmo->SetText(FText::FromString(FString::FromInt(val)));
}

void UCustomWidget::SetTextMag(int val)
{
	if (TextMag == nullptr) return;

	TextMag->SetText(FText::FromString(FString::FromInt(val)));
}

void UCustomWidget::SetHealth(float Health)
{
	if (HealthBar==nullptr) return;

	HealthBar->SetPercent(Health / 100.0f);
}

void UCustomWidget::HideUI()
{
	if (TextMag != nullptr && HealthBar != nullptr && TextAmmo != nullptr) return;

	TextMag->SetVisibility(ESlateVisibility::Collapsed);
	TextAmmo->SetVisibility(ESlateVisibility::Collapsed);
}

void UCustomWidget::ShowUI()
{
	if (TextMag != nullptr && HealthBar != nullptr && TextAmmo != nullptr) return;

	TextMag->SetVisibility(ESlateVisibility::Visible);
	TextAmmo->SetVisibility(ESlateVisibility::Visible);
}
