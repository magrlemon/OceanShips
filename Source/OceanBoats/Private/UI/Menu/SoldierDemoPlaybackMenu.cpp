// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#include "OceanBoats.h"
#include "SoldierDemoPlaybackMenu.h"
#include "ArmySimStyle.h"
#include "ArmySimMenuSoundsWidgetStyle.h"
#include "ArmySimGameInstance.h"

#define LOCTEXT_NAMESPACE "OceanBoats.HUD.Menu"

void FSoldierDemoPlaybackMenu::Construct( ULocalPlayer* _PlayerOwner )
{
	PlayerOwner = _PlayerOwner;
	bIsAddedToViewport = false;

	if ( !GEngine || !GEngine->GameViewport )
	{
		return;
	}
	
	if ( !GameMenuWidget.IsValid() )
	{
		SAssignNew( GameMenuWidget, SSoldierMenuWidget )
			.PlayerOwner( MakeWeakObjectPtr( PlayerOwner ) )
			.Cursor( EMouseCursor::Default )
			.IsGameMenu( true );			

		TSharedPtr<FSoldierMenuItem> MainMenuRoot = FSoldierMenuItem::CreateRoot();
		MainMenuItem = MenuHelper::AddMenuItem(MainMenuRoot,LOCTEXT( "Main Menu", "MAIN MENU" ) );

		MenuHelper::AddMenuItemSP( MainMenuItem, LOCTEXT( "No", "NO" ), this, &FSoldierDemoPlaybackMenu::OnCancelExitToMain );
		MenuHelper::AddMenuItemSP( MainMenuItem, LOCTEXT( "Yes", "YES" ), this, &FSoldierDemoPlaybackMenu::OnConfirmExitToMain );

		MenuHelper::AddExistingMenuItem( RootMenuItem, MainMenuItem.ToSharedRef() );
				
#if !SHOOTER_CONSOLE_UI
		MenuHelper::AddMenuItemSP( RootMenuItem, LOCTEXT("Quit", "QUIT"), this, &FSoldierDemoPlaybackMenu::OnUIQuit );
#endif

		GameMenuWidget->MainMenu = GameMenuWidget->CurrentMenu = RootMenuItem->SubMenu;
		GameMenuWidget->OnMenuHidden.BindSP( this, &FSoldierDemoPlaybackMenu::DetachGameMenu );
		GameMenuWidget->OnToggleMenu.BindSP( this, &FSoldierDemoPlaybackMenu::ToggleGameMenu );
		GameMenuWidget->OnGoBack.BindSP( this, &FSoldierDemoPlaybackMenu::OnMenuGoBack );
	}
}

void FSoldierDemoPlaybackMenu::CloseSubMenu()
{
	GameMenuWidget->MenuGoBack();
}

void FSoldierDemoPlaybackMenu::OnMenuGoBack(MenuPtr Menu)
{
}

void FSoldierDemoPlaybackMenu::DetachGameMenu()
{
	if ( GEngine && GEngine->GameViewport )
	{
		GEngine->GameViewport->RemoveViewportWidgetContent( GameMenuContainer.ToSharedRef() );
	}

	bIsAddedToViewport = false;
}

void FSoldierDemoPlaybackMenu::ToggleGameMenu()
{
	if ( !GameMenuWidget.IsValid( ))
	{
		return;
	}

	if ( bIsAddedToViewport && GameMenuWidget->CurrentMenu != RootMenuItem->SubMenu )
	{
		GameMenuWidget->MenuGoBack();
		return;
	}
	
	if ( !bIsAddedToViewport )
	{
		GEngine->GameViewport->AddViewportWidgetContent( SAssignNew( GameMenuContainer, SWeakWidget ).PossiblyNullContent( GameMenuWidget.ToSharedRef() ) );

		GameMenuWidget->BuildAndShowMenu();

		bIsAddedToViewport = true;
	} 
	else
	{
		// Start hiding animation
		GameMenuWidget->HideMenu();

		ASoldierPlayerController* const PCOwner = PlayerOwner ? Cast<ASoldierPlayerController>(PlayerOwner->PlayerController) : nullptr;

		if ( PCOwner )
		{
			// Make sure viewport has focus
			FSlateApplication::Get().SetAllUserFocusToGameViewport();
		}
	}
}

void FSoldierDemoPlaybackMenu::OnCancelExitToMain()
{
	CloseSubMenu();
}

void FSoldierDemoPlaybackMenu::OnConfirmExitToMain()
{
	UArmySimGameInstance* const GameInstance = Cast<UArmySimGameInstance>( PlayerOwner->GetGameInstance() );

	if ( GameInstance )
	{
		// tell game instance to go back to main menu state
		GameInstance->GotoState( ArmySimGameInstanceState::MainMenu );
	}
}

void FSoldierDemoPlaybackMenu::OnUIQuit()
{
	UArmySimGameInstance* const GameInstance = Cast<UArmySimGameInstance>( PlayerOwner->GetGameInstance() );

	GameMenuWidget->LockControls( true );
	GameMenuWidget->HideMenu();

	UWorld* const World = PlayerOwner ? PlayerOwner->GetWorld() : nullptr;
	if ( World )
	{
		const FSimMenuSoundsStyle& MenuSounds = FArmySimStyle::Get().GetWidgetStyle< FSimMenuSoundsStyle >( "DefaultSoldierMenuSoundsStyle" );
		MenuHelper::PlaySoundAndCall( World, MenuSounds.ExitGameSound, GetOwnerUserIndex(), this, &FSoldierDemoPlaybackMenu::Quit );
	}
}

void FSoldierDemoPlaybackMenu::Quit()
{
	APlayerController* const PCOwner = PlayerOwner ? PlayerOwner->PlayerController : nullptr;

	if ( PCOwner )
	{
		PCOwner->ConsoleCommand( "quit" );
	}
}

int32 FSoldierDemoPlaybackMenu::GetOwnerUserIndex() const
{
	return PlayerOwner ? PlayerOwner->GetControllerId() : 0;
}
#undef LOCTEXT_NAMESPACE
