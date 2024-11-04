#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "EO_RhythmNote.generated.h"


UCLASS()
class BEATSABER_API AEO_RhythmNote : public AActor
{
	GENERATED_BODY()
	
public:	
	AEO_RhythmNote();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;

public:
	UPROPERTY(VisibleAnywhere)
	class UBoxComponent* boxComp;
	UPROPERTY(VisibleAnywhere)
	class UStaticMeshComponent* meshComp;
	UPROPERTY(VisibleAnywhere)
	class UProceduralMeshComponent* proceduralMeshComp;
	UPROPERTY(VisibleAnywhere)
	class UStaticMeshComponent* bombMeshComp;
	UPROPERTY(VisibleAnywhere)
	class USceneComponent* testScene;
	UPROPERTY(VisibleAnywhere)
	class UStaticMeshComponent* wallMeshComp;

	UPROPERTY(EditDefaultsOnly)
	class UMaterialInterface* matInterface;

	UPROPERTY(EditDefaultsOnly)
	float speed = 700;
	int myXPos = 0;
	int myYPos = 0;
	float endPoint = -1;
	int myXPos2 = -1;
	int myYPos2 = -1;
	int colorIndex = 0;
	int typeIndex = 0;

private:
	UPROPERTY(VisibleAnywhere)
	class AEO_Sync* sync;
	UPROPERTY(VisibleAnywhere)
	class AEO_GridController* gridController;
	UPROPERTY(EditDefaultsOnly)
	class USoundBase* tik;

private:
	UMaterialInstanceDynamic* cubeDynamicMaterial;
	
	FVector syncPos;
	bool isTik = false;
	
	UPROPERTY(EditAnywhere)
	float wallXscaleRatio = 0.135f;
	UPROPERTY(EditAnywhere)
	float wallZscaleRatio = 0.32f;

private:
	void PatternTest(float deltaTime);
	void EditorSyncTest();

public:
	UFUNCTION()
	void SetNote(int num);
	UFUNCTION()
	void SetNoteType(int num);

};
