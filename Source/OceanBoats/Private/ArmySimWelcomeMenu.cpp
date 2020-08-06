// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.
#include "ArmySimWelcomeMenu.h"
#include "OceanBoats.h"
#include "ArmySimStyle.h"
#include "SArmySimConfirmationDialog.h"
#include "ArmySimGameViewportClient.h"
#include "ArmySimGameInstance.h"

#define LOCTEXT_NAMESPACE "OcenaBoats.HUD.Menu"

//class SSimWelcomeMenuWidget : public SCompoundWidget
//{
//	/** The menu that owns this widget. */
//	FSimWelcomeMenu* MenuOwner;
//
//	/** Animate the text so that the screen isn't static, for console cert requirements. */
//	FCurveSequence TextAnimation;
//
//	/** The actual curve that animates the text. */
//	FCurveHandle TextColorCurve;
//
//	TSharedPtr<SRichTextBlock> PressPlayText;
//
//	SLATE_BEGIN_ARGS( SSimWelcomeMenuWidget )
//	{}
//
//	SLATE_ARGUMENT(FSimWelcomeMenu*, MenuOwner)
//
//	SLATE_END_ARGS()
//
//	virtual bool SupportsKeyboardFocus() const override
//	{
//		return true;
//	}
//
//	UWorld* GetWorld() const
//	{
//		if (MenuOwner && MenuOwner->GetGameInstance().IsValid())
//		{
//			return MenuOwner->GetGameInstance()->GetWorld();
//		}
//
//		return nullptr;
//	}
//
//	void Construct( const FArguments& InArgs )
//	{
//		MenuOwner = InArgs._MenuOwner;
//		
//		TextAnimation = FCurveSequence();
//		const float AnimDuration = 1.5f;
//		TextColorCurve = TextAnimation.AddCurve(0, AnimDuration, ECurveEaseFunction::QuadInOut);
//
//		ChildSlot
//		[
//			SNew(SBorder)
//			.Padding(30.0f)
//			.VAlign(VAlign_Center)
//			.HAlign(HAlign_Center)
//			[ 
//				SAssignNew( PressPlayText, SRichTextBlock )
//				.Text( LOCTEXT("PressStartXboxOne", "PRESS A TO PLAY" ) )
//				.TextStyle( FArmySimStyle::Get(), "ArmySimGame.WelcomeScreen.WelcomeTextStyle" )
//				.DecoratorStyleSet(&FArmySimStyle::Get())
//				+ SRichTextBlock::ImageDecorator()
//			]
//		];
//	}
//
//	virtual void Tick( const FGeometry& AllottedGeometry, const double InCurrentTime, const float InDeltaTime ) override
//	{ 
//		if(!TextAnimation.IsPlaying())
//		{
//			if(TextAnimation.IsAtEnd())
//			{
//				TextAnimation.PlayReverse(this->AsShared());
//			}
//			else
//			{
//				TextAnimation.Play(this->AsShared());
//			}
//		}
//
//		PressPlayText->SetRenderOpacity(FMath::Lerp(0.5f, 1.0f, TextColorCurve.GetLerp()));
//	}
//
//	virtual FReply OnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override
//	{
//		return FReply::Handled();
//	}
//
//	virtual FReply OnKeyDown(const FGeometry& MyGeometry, const FKeyEvent& InKeyEvent) override
//	{
//		const FKey Key = InKeyEvent.GetKey();
//		if (Key == EKeys::Enter)
//		{
//			TSharedPtr<const FUniqueNetId> UserId;
//			const IOnlineSubsystem* OnlineSub = Online::GetSubsystem(GetWorld());
//			if (OnlineSub)
//			{
//				const IOnlineIdentityPtr IdentityInterface = OnlineSub->GetIdentityInterface();
//				if (IdentityInterface.IsValid())
//				{
//					UserId = IdentityInterface->GetUniquePlayerId(InKeyEvent.GetUserIndex());
//				}
//			}
//			MenuOwner->HandleLoginUIClosed(UserId, InKeyEvent.GetUserIndex());
//		}
//		else if (!MenuOwner->GetControlsLocked() && Key == EKeys::Virtual_Accept)
//		{
//			bool bSkipToMainMenu = true;
//
//			{
//				const IOnlineSubsystem* OnlineSub = Online::GetSubsystem(GetWorld());
//				if (OnlineSub)
//				{
//					const IOnlineIdentityPtr IdentityInterface = OnlineSub->GetIdentityInterface();
//					if (IdentityInterface.IsValid())
//					{
//						TSharedPtr<GenericApplication> GenericApplication = FSlateApplication::Get().GetPlatformApplication();
//						const bool bIsLicensed = GenericApplication->ApplicationLicenseValid();
//
//						const ELoginStatus::Type LoginStatus = IdentityInterface->GetLoginStatus(InKeyEvent.GetUserIndex());
//						if (LoginStatus == ELoginStatus::NotLoggedIn || !bIsLicensed)
//						{
//							// Show the account picker.
//							const IOnlineExternalUIPtr ExternalUI = OnlineSub->GetExternalUIInterface();
//							if (ExternalUI.IsValid())
//							{
//								ExternalUI->ShowLoginUI(InKeyEvent.GetUserIndex(), false, true, FOnLoginUIClosedDelegate::CreateSP(MenuOwner, &FSimWelcomeMenu::HandleLoginUIClosed));
//								bSkipToMainMenu = false;
//							}
//						}
//					}
//				}
//			}
//
//			if (bSkipToMainMenu)
//			{
//				const IOnlineSubsystem* OnlineSub = Online::GetSubsystem(GetWorld());
//				if (OnlineSub)
//				{
//					const IOnlineIdentityPtr IdentityInterface = OnlineSub->GetIdentityInterface();
//					if (IdentityInterface.IsValid())
//					{
//						TSharedPtr<const FUniqueNetId> UserId = IdentityInterface->GetUniquePlayerId(InKeyEvent.GetUserIndex());
//						// If we couldn't show the external login UI for any reason, or if the user is
//						// already logged in, just advance to the main menu immediately.
//						MenuOwner->HandleLoginUIClosed(UserId, InKeyEvent.GetUserIndex());
//					}
//				}
//			}
//
//			return FReply::Handled();
//		}
//
//		return FReply::Unhandled();
//	}
//
//	virtual FReply OnFocusReceived(const FGeometry& MyGeometry, const FFocusEvent& InFocusEvent) override
//	{
//		return FReply::Handled().ReleaseMouseCapture().SetUserFocus(SharedThis( this ), EFocusCause::SetDirectly, true);
//	}
//};
//
//void FSimWelcomeMenu::Construct( TWeakObjectPtr< USimGameInstance > InGameInstance )
//{
//	bControlsLocked = false;
//	GameInstance = InGameInstance;
//	PendingControllerIndex = -1;
//
//	MenuWidget = SNew( SSimWelcomeMenuWidget )
//		.MenuOwner(this);	
//}
//
//void FSimWelcomeMenu::AddToGameViewport()
//{
//	if (GEngine && GEngine->GameViewport)
//	{
//		GEngine->GameViewport->AddViewportWidgetContent(MenuWidget.ToSharedRef());
//		FSlateApplication::Get().SetKeyboardFocus(MenuWidget);
//	}
//}
//
//void FSimWelcomeMenu::RemoveFromGameViewport()
//{
//	if (GEngine && GEngine->GameViewport)
//	{
//		GEngine->GameViewport->RemoveViewportWidgetContent(MenuWidget.ToSharedRef());
//	}
//}
//
//void FSimWelcomeMenu::HandleLoginUIClosed(TSharedPtr<const FUniqueNetId> UniqueId, const int ControllerIndex, const FOnlineError& Error)
//{
//	if ( !ensure( GameInstance.IsValid() ) )
//	{
//		return;
//	}
//
//	UArmySimGameViewportClient* SimViewport = Cast<UArmySimGameViewportClient>( GameInstance->GetGameViewportClient() );
//
//	TSharedPtr<GenericApplication> GenericApplication = FSlateApplication::Get().GetPlatformApplication();
//	const bool bIsLicensed = GenericApplication->ApplicationLicenseValid();
//
//	// If they don't currently have a license, let them know, but don't let them proceed
//	if (!bIsLicensed && SimViewport != NULL)
//	{
//		const FText StopReason	= NSLOCTEXT( "ProfileMessages", "NeedLicense", "" );
//		const FText OKButton	= NSLOCTEXT( "DialogButtons", "OKAY", "OK" );
//
//		SimViewport->ShowDialog( 
//			nullptr,
//			EArmySimDialogType::Generic,
//			StopReason,
//			OKButton,
//			FText::GetEmpty(),
//			FOnClicked::CreateRaw(this, &FSimWelcomeMenu::OnConfirmGeneric),
//			FOnClicked::CreateRaw(this, &FSimWelcomeMenu::OnConfirmGeneric)
//			);
//		return;
//	}
//
//	PendingControllerIndex = ControllerIndex;
//
//	//if (UniqueId.IsValid())
//	//{
//	//	// Next step, check privileges
//	//	const IOnlineSubsystem* OnlineSub = Online::GetSubsystem(GameInstance->GetWorld());
//	//	if (OnlineSub)
//	//	{
//	//		const IOnlineIdentityPtr IdentityInterface = OnlineSub->GetIdentityInterface();
//	//		if (IdentityInterface.IsValid())
//	//		{
//	//			IdentityInterface->GetUserPrivilege(*UniqueId, EUserPrivileges::CanPlay, IOnlineIdentity::FOnGetUserPrivilegeCompleteDelegate::CreateSP(this, &FSimWelcomeMenu::OnUserCanPlay));
//	//		}
//	//	}
//	//}
//	//else
//	//{
//	//	// Show a warning that your progress won't be saved if you continue without logging in. 
//	//	if (SimViewport != NULL)
//	//	{
//	//		SimViewport->ShowDialog( 
//	//			nullptr,
//	//			EArmySimDialogType::Generic,
//	//			NSLOCTEXT("ProfileMessages", "ProgressWillNotBeSaved", "If you continue without signing in, your progress will not be saved."),
//	//			NSLOCTEXT("DialogButtons", "AContinue", "A - Continue"),
//	//			NSLOCTEXT("DialogButtons", "BBack", "B - Back"),
//	//			FOnClicked::CreateRaw(this, &FSimWelcomeMenu::OnContinueWithoutSavingConfirm),
//	//			FOnClicked::CreateRaw(this, &FSimWelcomeMenu::OnConfirmGeneric)
//	//		);
//	//	}
//	//}
//}
//
//void FSimWelcomeMenu::SetControllerAndAdvanceToMainMenu(const int ControllerIndex)
//{
//	if ( !ensure( GameInstance.IsValid() ) )
//	{
//		return;
//	}
//
//	ULocalPlayer * NewPlayerOwner = GameInstance->GetFirstGamePlayer();
//
//	if ( NewPlayerOwner != nullptr && ControllerIndex != -1 )
//	{
//		NewPlayerOwner->SetControllerId(ControllerIndex);
//		NewPlayerOwner->SetCachedUniqueNetId(NewPlayerOwner->GetUniqueNetIdFromCachedControllerId().GetUniqueNetId());
//
//		// tell gameinstance to transition to main menu
//		GameInstance->GotoState(ArmySimGameInstanceState::MainMenu);
//	}	
//}
//
//FReply FSimWelcomeMenu::OnContinueWithoutSavingConfirm()
//{
//	if ( !ensure( GameInstance.IsValid() ) )
//	{
//		return FReply::Handled();
//	}
//
//	UArmySimGameViewportClient * SimViewport = Cast<UArmySimGameViewportClient>( GameInstance->GetGameViewportClient() );
//
//	if (SimViewport != NULL)
//	{
//		SimViewport->HideDialog();
//	}
//
//	SetControllerAndAdvanceToMainMenu(PendingControllerIndex);
//	return FReply::Handled();
//}
//
//FReply FSimWelcomeMenu::OnConfirmGeneric()
//{
//	if ( !ensure( GameInstance.IsValid() ) )
//	{
//		return FReply::Handled();
//	}
//
//	UArmySimGameViewportClient * SimViewport = Cast<UArmySimGameViewportClient>( GameInstance->GetGameViewportClient() );
//
//	if (SimViewport != NULL)
//	{
//		SimViewport->HideDialog();
//	}
//
//	return FReply::Handled();
//}
//
//void FSimWelcomeMenu::OnUserCanPlay(const FUniqueNetId& UserId, EUserPrivileges::Type Privilege, uint32 PrivilegeResults)
//{
//	if (PrivilegeResults == (uint32)IOnlineIdentity::EPrivilegeResults::NoFailures)
//	{
//		SetControllerAndAdvanceToMainMenu(PendingControllerIndex);
//	}
//	else
//	{
//		UArmySimGameViewportClient * SimViewport = Cast<UArmySimGameViewportClient>( GameInstance->GetGameViewportClient() );
//
//		if ( SimViewport != NULL )
//		{
//			const FText ReturnReason = NSLOCTEXT("PrivilegeFailures", "CannotPlayAgeRestriction", "You cannot play this game due to age restrictions.");
//			const FText OKButton = NSLOCTEXT("DialogButtons", "OKAY", "OK");
//
//			SimViewport->ShowDialog( 
//				nullptr,
//				EArmySimDialogType::Generic,
//				ReturnReason,
//				OKButton,
//				FText::GetEmpty(),
//				FOnClicked::CreateRaw(this, &FSimWelcomeMenu::OnConfirmGeneric),
//				FOnClicked::CreateRaw(this, &FSimWelcomeMenu::OnConfirmGeneric)
//			);
//		}
//	}
//}

#undef LOCTEXT_NAMESPACE
