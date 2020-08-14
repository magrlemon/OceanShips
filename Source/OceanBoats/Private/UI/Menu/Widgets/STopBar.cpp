#include "OceanBoats.h"
#include "STopbar.h"

#define LOCTEXT_NAMESPACE "TopBar"

void STopBar::Construct(const FArguments& InArgs)
{
	//PlayerOwner = InArgs._PlayerOwner;
	TSharedRef<SWrapBox> ButtonBox = SNew(SWrapBox).UseAllottedWidth(true);
	const float MinButtonSize = 120.0f;
	const FMargin ButtonPadding(100.0f, 30.0f, 2.0f, 0.0f);
	ScoreboardStyle = &FArmySimStyle::Get().GetWidgetStyle<FSoldierScoreboardStyle>("DefaultSoldierScoreboardStyle");
	const FButtonStyle* PlayButtonStyle = &FArmySimStyle::Get().GetWidgetStyle<FButtonStyle>("PlayButtonStyle");
	const FButtonStyle* PauseButtonStyle = &FArmySimStyle::Get().GetWidgetStyle<FButtonStyle>("PauseButtonStyle");
	const FButtonStyle* StopButtonStyle = &FArmySimStyle::Get().GetWidgetStyle<FButtonStyle>("StopButtonStyle");

	ButtonBox->AddSlot()
		.Padding(ButtonPadding)
		[
			SNew(SBox)
			.MinDesiredWidth(MinButtonSize)
			.WidthOverride(70)
			.HeightOverride(70)
			[
				SNew(SButton)
				.VAlign(VAlign_Center)
				.HAlign(HAlign_Center)
				.OnClicked(this, &STopBar::OnStartClicked)
				.Visibility(this, &STopBar::GetCancelButtonVisibility)
				//.Text(LOCTEXT("StartMission", "Start Mission"))
				.ButtonStyle(PlayButtonStyle)
			]
		];

	ButtonBox->AddSlot()
		.Padding(ButtonPadding)
		[
			SNew(SBox)
			//.MinDesiredWidth(MinButtonSize)
			.WidthOverride(70)
			.HeightOverride(70)
			[
				SNew(SButton)
				.VAlign(VAlign_Center)
				.HAlign(HAlign_Center)
				.OnClicked(this, &STopBar::OnPauseClicked)
				.Visibility(this, &STopBar::GetCancelButtonVisibility)
				//.Text(LOCTEXT("StartMission", "Start Mission"))
				.ButtonStyle(PauseButtonStyle)
			]
		];

	ButtonBox->AddSlot()
		.Padding(ButtonPadding)
		[
			SNew(SBox)
			.MinDesiredWidth(MinButtonSize)
			.WidthOverride(70)
			.HeightOverride(70)
			[
				SNew(SButton)
				.VAlign(VAlign_Center)
				.HAlign(HAlign_Center)
				.OnClicked(this, &STopBar::OnStopClicked)
				.Visibility(this, &STopBar::GetCancelButtonVisibility)
				//.Text(LOCTEXT("StartMission", "Start Mission"))
				.ButtonStyle(StopButtonStyle)
			]
		];

	ChildSlot
		.VAlign(VAlign_Fill)
		.HAlign(HAlign_Fill)		
		[
			SNew(SHorizontalBox)
			+ SHorizontalBox::Slot()
			.FillWidth(1.0f)
			[
				ButtonBox
			]
		];
}

EVisibility STopBar::GetCancelButtonVisibility() const
{
	return EVisibility::Visible;
}

/** Called when the user clicks the "Expand All" button; Expands the entire tag tree */
FReply STopBar::OnStartClicked()
{
	return FReply::Handled();
}
FReply STopBar::OnPauseClicked()
{
	return FReply::Handled();
}
FReply STopBar::OnStopClicked()
{
	return FReply::Handled();
}

#undef LOCTEXT_NAMESPACE