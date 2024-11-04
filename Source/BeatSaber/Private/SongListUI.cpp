// Fill out your copyright notice in the Description page of Project Settings.


#include "SongListUI.h"
#include <UMG/Public/Components/ScrollBox.h>
#include <Kismet/KismetMathLibrary.h>
#include "Blueprint/WidgetLayoutLibrary.h"
#include <UMG/Public/Components/CanvasPanelSlot.h>
#include <Engine/DataTable.h>
#include "MusicInfoWidget.h"
#include <UMG/Public/Components/TextBlock.h>
#include <Engine/Texture2D.h>
#include <UMG/Public/Components/Image.h>
#include <Styling/SlateBrush.h>
#include <Styling/SlateColor.h>
#include <Layout/Margin.h>
#include <Math/Color.h>
#include "SB_SelectMusicInfo.h"

USongListUI::USongListUI(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	ConstructorHelpers::FObjectFinder<UDataTable> tempMusicData(TEXT("/Script/Engine.DataTable'/Game/EO/SongData.SongData'"));
	if (tempMusicData.Succeeded())
	{
		listDataTable = tempMusicData.Object;
	}

	static ConstructorHelpers::FClassFinder<UMusicInfoWidget> tempMusicInfoWidget(TEXT("/Script/UMGEditor.WidgetBlueprint'/Game/SB/Blueprints/WB_MusicInfo.WB_MusicInfo_C'"));
	if (tempMusicInfoWidget.Succeeded())
	{
		musicInfoWidgetFactory = tempMusicInfoWidget.Class;
	}
}

void USongListUI::NativeConstruct()
{
	Super::NativeConstruct();

	if (musicInfoWidgetFactory) {
		if (listDataTable) {
			for (int32 i =0; i < listDataTable->GetRowNames().Num(); i++)
			{
				FSongDataTableRow* row = listDataTable->FindRow<FSongDataTableRow>(FName(*(FString::FormatAsNumber(i))), FString(""));

				musicInfoSlot = CreateWidget<UMusicInfoWidget>(GetWorld(), musicInfoWidgetFactory);
			
				if (musicInfoSlot) {
					musicInfoSlot->songSlotData = FSongInfo(row->songName, row->artist, row->imagePath, row->songPath, row->previewSongTime, row->patternPath, row->bpm);
					musicInfoSlot->Subtitle_txt->SetText(FText::FromString(row->songName));
					musicInfoSlot->artist_txt->SetText(FText::FromString(row->artist));

					FString timeFormat = FloatToTimeFormat(row->songDuration);
					FString songDurationTxt = timeFormat;

					musicInfoSlot->musicTime_txt->SetText(FText::FromString(songDurationTxt));
					FString bpmtxt = FString::Printf(TEXT("%.2f"), row->bpm);
					musicInfoSlot->bpm_txt->SetText(FText::FromString(bpmtxt));

					UTexture2D* thumbnail = Cast<UTexture2D>(StaticLoadObject(UTexture2D::StaticClass(), nullptr, *row->imagePath));

					FSlateBrush Brush;
					Brush.DrawAs = ESlateBrushDrawType::Image;
					Brush.Tiling = ESlateBrushTileType::NoTile;
					Brush.Mirroring = ESlateBrushMirrorType::NoMirror;
					Brush.ImageSize = FVector2D(32.0f, 32.0f); 
					Brush.Margin = FMargin(0.0f, 0.0f, 0.0f, 0.0f); 
					Brush.TintColor = FSlateColor(FLinearColor(1.0f, 1.0f, 1.0f, 1.0f));

					Brush.SetResourceObject(thumbnail);
					musicInfoSlot->MusicThumbnail_img->SetBrush(Brush);

					SongList->AddChild(musicInfoSlot);
				}
			}
		}
	}
}

void USongListUI::ScrollDown()
{ 
	if (SongList) {
			scrollVal += 10;
			if (scrollVal >= SongList->GetScrollOffsetOfEnd()) {
				scrollVal = SongList->GetScrollOffsetOfEnd();
			}
			SongList->SetScrollOffset(scrollVal);
	}
} 

void USongListUI::ScrollUp()
{
	if (SongList) {
		scrollVal -= 10;
		if (scrollVal <= 0) {
			scrollVal = 0;
		}

		SongList->SetScrollOffset(scrollVal);

	}
}

FString USongListUI::FloatToTimeFormat(float songDuration)
{
	int32 minute = songDuration / 60;
	int32 second = (int32)songDuration % 60;

	FString timeFormat;
	if (second < 10) {
		timeFormat = FString::FromInt(minute) + TEXT(":0") + FString::FromInt(second);
	}
	else {
		timeFormat = FString::FromInt(minute) + TEXT(":") + FString::FromInt(second);
	}
	return timeFormat;
}