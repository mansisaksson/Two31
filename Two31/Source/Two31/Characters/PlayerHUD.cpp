#include "Two31.h"
#include "PlayerHUD.h"
#include "Engine/Canvas.h"
#include "TextureResource.h"
#include "CanvasItem.h"

APlayerHUD::APlayerHUD()
{

}


void APlayerHUD::DrawHUD()
{
	Super::DrawHUD();

	//if (CrosshairTexture != NULL)
	//{
	//	const FVector2D Center(Canvas->ClipX * 0.5f, Canvas->ClipY * 0.5f);

	//	const FVector2D CrosshairDrawPosition((Center.X - (CrosshairTexture->GetSurfaceWidth() * 0.5)), (Center.Y - (CrosshairTexture->GetSurfaceHeight() * 0.5f)));

	//	// draw the crosshair
	//	FCanvasTileItem TileItem(CrosshairDrawPosition, CrosshairTexture->Resource, FLinearColor::White);
	//	TileItem.BlendMode = SE_BLEND_Translucent;
	//	Canvas->DrawItem(TileItem);
	//}
}

