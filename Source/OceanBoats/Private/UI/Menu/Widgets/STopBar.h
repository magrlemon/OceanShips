#pragma once
#include "OceanBoats.h"
#include "SoldierScoreboardWidgetStyle.h"

//class declare
class STopBar : public SCompoundWidget
{
public:
	SLATE_BEGIN_ARGS(STopBar)
	{}

	SLATE_ARGUMENT(TWeakObjectPtr<ULocalPlayer >, PlayerOwner)
		//SLATE_ARGUMENT(TSharedPtr<AHUD>, OwnerWidget)

		SLATE_END_ARGS()

	/** needed for every widget */
	void Construct(const FArguments& InArgs);

	EVisibility GetCancelButtonVisibility() const;
	/** Called when the user clicks the "Expand All" button; Expands the entire tag tree */
	FReply OnStartClicked();
	FReply OnPauseClicked();
	FReply OnStopClicked();

protected:
	const FSoldierScoreboardStyle* ScoreboardStyle;
};