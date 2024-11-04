#include "EO_Sync.h"
#include <Components/AudioComponent.h>
#include <Engine/DataTable.h>
#include "EO_RhythmNote.h"
#include "EO_PatternDataTable.h"
#include "NodeBlock.h"
#include <Kismet/GameplayStatics.h>
#include "VR_Player.h"
#include "InGameMode.h"
#include <Engine/StreamableManager.h>
#include "WallObstacle.h"
#include "SphereObstacle.h"
#include <Components/TimelineComponent.h>


AEO_Sync::AEO_Sync()
{
	PrimaryActorTick.bCanEverTick = true;

	ConstructorHelpers::FClassFinder<ANodeBlock> noteTemp(TEXT("'/Game/SB/Blueprints/BP_NodeBlock.BP_NodeBlock_C'"));
	if (noteTemp.Succeeded())
	{
		noteFactory = noteTemp.Class;
	}

	ConstructorHelpers::FClassFinder<ASphereObstacle> bomTemp(TEXT("'/Game/SB/Blueprints/BP_SphereObstacle.BP_SphereObstacle_C'"));
	if (bomTemp.Succeeded())
	{
		bombFactory = bomTemp.Class;
	}

	ConstructorHelpers::FClassFinder<AWallObstacle> wallTemp(TEXT("'/Game/SB/Blueprints/BP_WallObstacle.BP_WallObstacle_C'"));
	if (wallTemp.Succeeded())
	{
		wallFactory = wallTemp.Class;
	}

	ConstructorHelpers::FObjectFinder<UDataTable> patternTemp(TEXT("/Script/Engine.DataTable'/Game/EO/Resources/test.test'"));
	if (patternTemp.Succeeded())
	{
		patternData = patternTemp.Object;
	}

	ConstructorHelpers::FObjectFinder<UCurveFloat> fadeoutTemp(TEXT("'/Game/EO/Blueprints/MusicFader.MusicFader'"));
	if (fadeoutTemp.Succeeded())
	{
		fadeoutTimeline = fadeoutTemp.Object;
	}

	audioComp = CreateDefaultSubobject<UAudioComponent>(TEXT("AudioComponent"));
	RootComponent = audioComp;

	timelineComp = CreateDefaultSubobject<UTimelineComponent>(TEXT("Timeline Component"));
}

void AEO_Sync::BeginPlay()
{
	Super::BeginPlay();

	if (AInGameMode* gmbTemp = Cast<AInGameMode>(GetWorld()->GetAuthGameMode()))
	{
		gameModeBase = gmbTemp;
	}
	else
	{
		bGameStart = true;
	}

	if (fadeoutTimeline != nullptr)
	{
		float min = 0, max = 0;
		fadeoutTimeline->GetTimeRange(min, max);
		timelineComp->SetTimelineLength(max);

		FOnTimelineFloat timelineUpdateDelegate;
		timelineUpdateDelegate.BindUFunction(this, FName("FadeoutMusic"));

		FOnTimelineEvent timelineFinishedEvent;
		timelineFinishedEvent.BindUFunction(this, FName("TimelineFinished"));

		timelineComp->AddInterpFloat(fadeoutTimeline, timelineUpdateDelegate);
		timelineComp->SetTimelineFinishedFunc(timelineFinishedEvent);
	}

	player = Cast<AVR_Player>(UGameplayStatics::GetActorOfClass(GetWorld(), AVR_Player::StaticClass()));
	if (player != nullptr)
		SetActorLocation(FVector(player->GetActorLocation().X + 140, player->GetActorLocation().Y, GetActorLocation().Z + 200));

	/*GenerateNote(TEXT("'/Game/EO/Sounds/BTS-Dynamite__Short_Version_.BTS-Dynamite__Short_Version_'"), TEXT("'/Game/EO/Resources/Dynamite.Dynamite'"), 148.f);
	GameStart();*/
}

void AEO_Sync::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (gameModeBase != nullptr)
		bGameStart = gameModeBase->bGameStart;
}

void AEO_Sync::MusicPlay()
{
	audioComp->Play();
}

void AEO_Sync::GenerateNote(FString songPath, FString patternPath, float bpm)
{
	FStreamableManager assetLoader;

	musicBPM = bpm;
	frequeny = 44100.0f;

	sampleOffset = frequeny * offset;
	oneBeatTime = (60.0f / musicBPM);
	nextSample += sampleOffset;

	barPerSec = oneBeatTime * 4;

	audioComp->SetSound(Cast<USoundBase>(assetLoader.LoadSynchronous(FPaths::ConvertRelativePathToFull(songPath))));

	startPos = 700 * 3;

	patternData = Cast<UDataTable>(assetLoader.LoadSynchronous(FPaths::ConvertRelativePathToFull(patternPath)));

	if (patternData != nullptr)
	{
		for (int i = 0; i < patternData->GetRowNames().Num(); i++)
		{
			FPatternDataTableRow* row = patternData->FindRow<FPatternDataTableRow>(FName(*(FString::FormatAsNumber(i))), FString(""));

			if (row->color == 0 || row->color == 1)
			{
				ANodeBlock* tempNote = GetWorld()->SpawnActor<ANodeBlock>(noteFactory, FVector(GetActorLocation().X + (startPos + offset + 700 * (row->ms * 0.001f)), YGeneratePos(row->y), XGeneratePos(row->x)), FRotator(0, 0, row->rot));
				tempNote->SwitchColor(row->color);
				tempNote->SwitchType(row->type);

				noteCount++;
			}
			else if (row->color == 2)
			{
				ASphereObstacle* bomb = GetWorld()->SpawnActor<ASphereObstacle>(bombFactory, FVector(GetActorLocation().X + (startPos + offset + 700 * (row->ms * 0.001f)), YGeneratePos(row->y), XGeneratePos(row->x)), FRotator(0));
			}
			else if (row->color == 3)
			{
				UE_LOG(LogTemp, Warning, TEXT("Spawn wall"));
				FVector vecFirst = FVector(GetActorLocation().X + (startPos + 700 * (row->ms * 0.001f)), YGeneratePos(row->y), XGeneratePos(row->x));
				FVector vecEnd = FVector(GetActorLocation().X + (startPos + 700 * (row->wEndms * 0.001f)), YGeneratePos(row->y2), XGeneratePos(row->x2));

				FVector dist = FVector(vecEnd.X - vecFirst.X, vecEnd.Y + vecFirst.Y, vecEnd.Z - vecFirst.Z);
				float xRef = (700 * (oneBeatTime / 4 * 1000) * 0.001f);

				AWallObstacle* wallObj = GetWorld()->SpawnActor<AWallObstacle>(wallFactory, FVector(vecFirst.X, dist.Y / 2, GetActorLocation().Z + dist.Z / 2), FRotator(0, 180, 0));
				wallObj->SetActorRelativeScale3D(FVector((FMath::RoundToInt(dist.X / xRef) + 1) * (xRef / 100), (FMath::Abs(vecEnd.Y - vecFirst.Y) / 70 + 1), (FMath::Abs(vecEnd.Z - vecFirst.Z) / 60 + 1)));
			}
		}
	}

	//UE_LOG(LogTemp, Warning, TEXT("All Generated"));
	isGenerate = true;
}

void AEO_Sync::GameStart()
{
	if (isGenerate)
	{
		FTimerHandle handle;
		GetWorldTimerManager().SetTimer(handle, this, &AEO_Sync::MusicPlay, 3.0f, false);
	}
}

void AEO_Sync::FadeoutMusic(float output)
{
	UE_LOG(LogTemp, Warning, TEXT("Timeline in"));
	UE_LOG(LogTemp, Warning, TEXT("%f"), output);
	audioComp->SetPitchMultiplier(output);
}

void AEO_Sync::TimelineFinished()
{
	UE_LOG(LogTemp, Warning, TEXT("Timeline finished"));
}

void AEO_Sync::GameOverFadeout()
{
	timelineComp->PlayFromStart();
	audioComp->FadeOut(2.3f, 0);
}

float AEO_Sync::XGeneratePos(int rowX)
{
	if (rowX == 0)
	{
		return GetActorLocation().Z + 120;
	}
	else if (rowX == 1)
	{
		return GetActorLocation().Z + 60;
	}
	else if (rowX == 2)
	{
		return GetActorLocation().Z + 0;
	}

	return -1;
}

float AEO_Sync::YGeneratePos(int rowY)
{
	if (rowY == 0)
	{
		return GetActorLocation().Y - 105;
	}
	else if (rowY == 1)
	{
		return GetActorLocation().Y - 35;
	}
	else if (rowY == 2)
	{
		return GetActorLocation().Y + 35;
	}
	else if (rowY == 3)
	{
		return GetActorLocation().Y + 105;
	}

	return -1;
}

