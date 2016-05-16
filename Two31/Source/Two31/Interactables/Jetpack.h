#pragma once
#include "Interactables/Interactable.h"
#include "Jetpack.generated.h"

UCLASS()
class TWO31_API AJetpack : public AInteractable
{
	GENERATED_BODY()
	
	UPROPERTY(VisibleDefaultsOnly, Category = Mesh, meta = (AllowPrivateAccess = "true"))
	class UStaticMeshComponent* JetMesh;

	UPROPERTY(VisibleDefaultsOnly, meta = (AllowPrivateAccess = "true"))
	class UArrowComponent* UpArrow;
	
public:
	AJetpack();

	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = GetFunction)
	USceneComponent* GetOwnerComp();

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay)
	float ImpulsePowah;
	
private:
	bool bIsActive;
};
