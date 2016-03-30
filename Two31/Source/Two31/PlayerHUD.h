#pragma once

#include "GameFramework/HUD.h"
#include "PlayerHUD.generated.h"


UCLASS()
class TWO31_API APlayerHUD : public AHUD
{
	GENERATED_BODY()
	
public:
	APlayerHUD();

	virtual void DrawHUD() override;

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Visual)
	UTexture2D* CrosshairTexture;

};
