// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#include "OceanBoats.h"
#include "ArmySimStyle.h"
#include "SArmySimConfirmationDialog.h"
#include "SoldierMessageMenu.h"
#include "ArmySimGameViewportClient.h"
#include "ArmySimGameInstance.h"

#define LOCTEXT_NAMESPACE "OceanBoats.HUD.Menu"

void FSoldierMessageMenu::Construct(TWeakObjectPtr<UArmySimGameInstance> InGameInstance, TWeakObjectPtr<ULocalPlayer> InPlayerOwner, const FText& Message, const FText& OKButtonText, const FText& CancelButtonText, const FName& InPendingNextState)
{
	GameInstance			= InGameInstance;
	PlayerOwner				= InPlayerOwner;
	PendingNextState		= InPendingNextState;

	if ( ensure( GameInstance.IsValid() ) )
	{
		UArmySimGameViewportClient* SoldierViewport = Cast<UArmySimGameViewportClient>( GameInstance->GetGameViewportClient() );

		if ( SoldierViewport )
		{
			// Hide the previous dialog
			SoldierViewport->HideDialog();

			// Show the new one
			SoldierViewport->ShowDialog( 
				PlayerOwner,
				EArmySimDialogType::Generic,
				Message, 
				OKButtonText, 
				CancelButtonText, 
				FOnClicked::CreateRaw(this, &FSoldierMessageMenu::OnClickedOK),
				FOnClicked::CreateRaw(this, &FSoldierMessageMenu::OnClickedCancel)
			);
		}
	}
}

void FSoldierMessageMenu::RemoveFromGameViewport()
{
	if ( ensure( GameInstance.IsValid() ) )
	{
		UArmySimGameViewportClient * SoldierViewport = Cast<UArmySimGameViewportClient>( GameInstance->GetGameViewportClient() );

		if ( SoldierViewport )
		{
			// Hide the previous dialog
			SoldierViewport->HideDialog();
		}
	}
}

void FSoldierMessageMenu::HideDialogAndGotoNextState()
{
	RemoveFromGameViewport();

	if ( ensure( GameInstance.IsValid() ) )
	{
		GameInstance->GotoState( PendingNextState );
	}
};

FReply FSoldierMessageMenu::OnClickedOK()
{
	OKButtonDelegate.ExecuteIfBound();
	HideDialogAndGotoNextState();
	return FReply::Handled();
}

FReply FSoldierMessageMenu::OnClickedCancel()
{
	CancelButtonDelegate.ExecuteIfBound();
	HideDialogAndGotoNextState();
	return FReply::Handled();
}

void FSoldierMessageMenu::SetOKClickedDelegate(FMessageMenuButtonClicked InButtonDelegate)
{
	OKButtonDelegate = InButtonDelegate;
}

void FSoldierMessageMenu::SetCancelClickedDelegate(FMessageMenuButtonClicked InButtonDelegate)
{
	CancelButtonDelegate = InButtonDelegate;
}


#undef LOCTEXT_NAMESPACE
