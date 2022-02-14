#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "CustomWidget.generated.h"

/**
 * 
 */
UCLASS()
class TESTGAME_API UCustomWidget : public UUserWidget
{
	GENERATED_BODY()
public:	
	void SetTextAmmo(int val);
	void SetTextMag(int val);
	void SetHealth(float Health);

	void HideUI();
	void ShowUI();

	UPROPERTY(meta = (BindWidget))
		class UProgressBar* HealthBar;
private:

	UPROPERTY(meta = (BindWidgt))
		class UTextBlock* TextAmmo;
	UPROPERTY(meta = (BindWidgt))
		UTextBlock* TextMag;
	bool Initialize() override;
};
