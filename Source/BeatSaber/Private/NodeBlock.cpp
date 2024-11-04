// Fill out your copyright notice in the Description page of Project Settings.


#include "NodeBlock.h"
#include "ProceduralMeshComponent.h"
#include <Components/StaticMeshComponent.h>
#include <Kismet/KismetMathLibrary.h>
#include <Materials/MaterialInstanceDynamic.h>
#include <Kismet/KismetMaterialLibrary.h>
#include <Kismet/GameplayStatics.h>
#include "EO_Sync.h"
#include "InGameMode.h"

// Sets default values
ANodeBlock::ANodeBlock()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	rootComp = CreateDefaultSubobject<USceneComponent>(TEXT("rootComp"));
	SetRootComponent(rootComp);

	proceduralMesh = CreateDefaultSubobject<UProceduralMeshComponent>(TEXT("proceduralMesh"));
	proceduralMesh->SetupAttachment(rootComp);
	proceduralMesh->SetRelativeLocation(FVector(0));
	proceduralMesh->SetRelativeRotation(FRotator(0, 180, 180));
	proceduralMesh->SetCollisionProfileName(TEXT("NodeBlock"));
	proceduralMesh->bUseComplexAsSimpleCollision = false;

	sm_nodeBlock = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("sm_nodeBlock"));
	sm_nodeBlock->SetupAttachment(rootComp);
	sm_nodeBlock->SetRelativeLocation(FVector(0));
	sm_nodeBlock->SetRelativeRotation(FRotator(0, 180, 180));
	sm_nodeBlock->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	sm_nodeBlock->bHiddenInGame = true;

	static ConstructorHelpers::FObjectFinder<UStaticMesh> tempNodeBlockMesh(TEXT("/Script/Engine.StaticMesh'/Game/SB/Meshes/SM_BS_Cube.SM_BS_Cube'"));
	if (tempNodeBlockMesh.Succeeded()) {
		sm_nodeBlock->SetStaticMesh(tempNodeBlockMesh.Object);
	}

	static ConstructorHelpers::FObjectFinder<UMaterialInterface> tempBlockMat(TEXT("/Script/Engine.MaterialInstanceConstant'/Game/SB/Materials/M_Cube_Inst.M_Cube_Inst'"));

	if (tempBlockMat.Succeeded())
	{
		blockMat = tempBlockMat.Object;
	}

	sync = Cast<AEO_Sync>(UGameplayStatics::GetActorOfClass(GetWorld(), AEO_Sync::StaticClass()));
}

// Called when the game starts or when spawned
void ANodeBlock::BeginPlay()
{
	Super::BeginPlay();

	//sm_nodeBlock->SetMaterial(0, blockMat);
	CubeDynamicMaterial = UMaterialInstanceDynamic::Create(sm_nodeBlock->GetMaterial(0), this);
	sm_nodeBlock->SetMaterial(0, CubeDynamicMaterial);
	proceduralMesh->SetMaterial(0, CubeDynamicMaterial);

	gm = Cast<AInGameMode>(GetWorld()->GetAuthGameMode());

	//blockColor = UKismetMathLibrary::RandomIntegerInRange(0, 1);
	//SwitchColor(blockColor);
	//
	//blockRotate = UKismetMathLibrary::RandomInteger64InRange(0, 7);

	//switch (blockRotate)
	//{
	//	case 0: // 0�� �Ʒ�
	//	{
	//		SetActorRotation(FRotator(0, 0, 0));
	//		break;
	//	}
	//	case 1: // 45�� �Ʒ� ���� �밢��
	//	{
	//		SetActorRotation(FRotator(0, 0, 45));
	//		break;
	//	}
	//	case 2: // 90�� 
	//	{
	//		SetActorRotation(FRotator(0, 0, 90));
	//		break;
	//	}
	//	case 3: // 135�� 
	//	{
	//		SetActorRotation(FRotator(0, 0, 135));
	//		break;
	//	}
	//	case 4: // 180�� 
	//	{
	//		SetActorRotation(FRotator(0, 0, 180));
	//		break;
	//	}
	//	case 5: // 225�� 
	//	{
	//		SetActorRotation(FRotator(0, 0, 225));
	//		break;
	//	}
	//	case 6: // 270��
	//	{
	//		SetActorRotation(FRotator(0, 0, 270));
	//		break;
	//	}
	//	case 7: // 315�� 
	//	{
	//		SetActorRotation(FRotator(0, 0, 315));
	//		break;
	//	}
	//}

}

// Called every frame
void ANodeBlock::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (gm) {
		if (gm->bGameEnd) {
			FVector p0 = GetActorLocation();
			FVector vt = FVector::UpVector * -1 * speed * 0.7f * DeltaTime;
			SetActorLocation(p0 + vt);
			return;
		}
	}

	if (sync != nullptr)
	{
		if (!bSlice && sync->isGenerate && sync->bGameStart)
		{
			FVector p0 = GetActorLocation();
			FVector vt = FVector::ForwardVector * -1 * speed * DeltaTime;
			SetActorLocation(p0 + vt);
		}

		// ��ũ �׽�Ʈ �ڵ�, ��ũ�� ��Ȯ�� ��ġ�� �������� �� ������ �׽�Ʈ�ϱ� ���� �ڵ�
		/*if (GetActorLocation().X <= sync->GetActorLocation().X)
		{
			UGameplayStatics::PlaySound2D(GetWorld(), tik);
			Destroy();
		}*/
	}
}

void ANodeBlock::SwitchColor(int num)
{
	switch (num)
	{
	case 0: // ����
		if (CubeDynamicMaterial) {
			CubeDynamicMaterial->SetScalarParameterValue(TEXT("ColorChoice"), 1);
			blockColor = 0;
		}
		break;
	case 1: // �Ķ�
		if (CubeDynamicMaterial) {
			CubeDynamicMaterial->SetScalarParameterValue(TEXT("ColorChoice"), 0);
			blockColor = 1;
		}
		break;
	}
}

void ANodeBlock::SwitchType(int num)
{
	switch (num)
	{
	case 0: // ȭ��ǥ
		blockType = 0;
		CubeDynamicMaterial->SetScalarParameterValue(TEXT("TypeChoice"), 0);
		break;
	case 1: // ȭ��ǥ ����
		blockType = 1;
		CubeDynamicMaterial->SetScalarParameterValue(TEXT("TypeChoice"), 1);
		break;
	}
}

void ANodeBlock::DelayDestroy()
{
	FTimerHandle destroyDelay;
	GetWorld()->GetTimerManager().SetTimer(destroyDelay, FTimerDelegate::CreateLambda([&]() {
		Destroy();
	}), 3, false);
}

