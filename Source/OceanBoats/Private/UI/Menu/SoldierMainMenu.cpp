// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#include "OceanBoats.h"
#include "SoldierMainMenu.h"
#include "ArmyGameLoadingScreen.h"
#include "ArmySimStyle.h"
#include "ArmySimMenuSoundsWidgetStyle.h"
#include "ArmySimGameInstance.h"
#include "SlateBasics.h"
#include "SlateExtras.h"
#include "GenericPlatformChunkInstall.h"
#include "OnlineSubsystemSessionSettings.h"
#include "SArmySimConfirmationDialog.h"
#include "ArmySimMenuItemWidgetStyle.h"
#include "ArmySimGameUserSettings.h"
#include "ArmySimGameViewportClient.h"
#include "SoldierPersistentUser.h"
#include "Player/SoldierLocalPlayer.h"
#include "OnlineSubsystemUtils.h"
#include "SoldierHUD.h"
#include "WeatherOptions.h"
#include "Widgets/WeatherSettingWidget.h"

#define LOCTEXT_NAMESPACE "OceanBoats.HUD.Menu"

#define MAX_BOT_COUNT 8

static const FString MapNames[] = { TEXT("BreakBoats-20200408"), TEXT("Demonstration") };
static const FString JoinMapNames[] = { TEXT("Any"), TEXT("Demonstration"), TEXT("BreakBoats-20200408") };
static const FName PackageNames[] = { TEXT("BreakBoats-20200408.umap"), TEXT("Demonstration.umap") };
static const int DefaultTDMMap = 1;
static const int DefaultFFAMap = 0; 
static const float QuickmatchUIAnimationTimeDuration = 30.f;

//use an EMap index, get back the ChunkIndex that map should be part of.
//Instead of this mapping we should really use the AssetRegistry to query for chunk mappings, but maps aren't members of the AssetRegistry yet.
static const int ChunkMapping[] = { 1, 2 };

#if PLATFORM_SWITCH
#	define LOGIN_REQUIRED_FOR_ONLINE_PLAY 1
#else
#	define LOGIN_REQUIRED_FOR_ONLINE_PLAY 0
#endif

#if PLATFORM_SWITCH
#	define CONSOLE_LAN_SUPPORTED 1
#else
#	define CONSOLE_LAN_SUPPORTED 0
#endif

FSoldierMainMenu::~FSoldierMainMenu()
{
	CleanupOnlinePrivilegeTask();
}

void FSoldierMainMenu::Construct(TWeakObjectPtr<UArmySimGameInstance> _GameInstance, TWeakObjectPtr<ULocalPlayer> _PlayerOwner)
{
	bShowingDownloadPct = false;
	bAnimateQuickmatchSearchingUI = false;
	bUsedInputToCancelQuickmatchSearch = false;
	bQuickmatchSearchRequestCanceled = false;
	bIncQuickMAlpha = false;
	bIsGameMainMenuUp = false;
	PlayerOwner = _PlayerOwner;
	MatchType = EMatchType::Custom;

	check(_GameInstance.IsValid());

	GameInstance = _GameInstance;
	PlayerOwner = _PlayerOwner;

	OnCancelMatchmakingCompleteDelegate = FOnCancelMatchmakingCompleteDelegate::CreateSP(this, &FSoldierMainMenu::OnCancelMatchmakingComplete);
	OnMatchmakingCompleteDelegate = FOnMatchmakingCompleteDelegate::CreateSP(this, &FSoldierMainMenu::OnMatchmakingComplete);
	
	// read user settings
#if SHOOTER_CONSOLE_UI
	bIsLanMatch = FParse::Param(FCommandLine::Get(), TEXT("forcelan"));
#else
	UArmySimGameUserSettings* const UserSettings = CastChecked<UArmySimGameUserSettings>(GEngine->GetGameUserSettings());
	bIsLanMatch = UserSettings->IsLanMatch();
	bIsDedicatedServer = UserSettings->IsDedicatedServer();
#endif

	BotsCountOpt = 1;
	bIsRecordingDemo = false;

	if(GetPersistentUser())
	{
		BotsCountOpt = GetPersistentUser()->GetBotsCount();
		bIsRecordingDemo = GetPersistentUser()->IsRecordingDemos();
	}		

	// number entries 0 up to MAX_BOX_COUNT
	TArray<FText> BotsCountList;
	for (int32 i = 0; i <= MAX_BOT_COUNT; i++)
	{
		BotsCountList.Add(FText::AsNumber(i));
	}
	
	TArray<FText> MapList;
	for (int32 i = 0; i < ARRAY_COUNT(MapNames); ++i)
	{
		MapList.Add(FText::FromString(MapNames[i]));		
	}

	TArray<FText> JoinMapList;
	for (int32 i = 0; i < ARRAY_COUNT(JoinMapNames); ++i)
	{
		JoinMapList.Add(FText::FromString(JoinMapNames[i]));
	}

	TArray<FText> OnOffList;
	OnOffList.Add( LOCTEXT("Off","OFF") );
	OnOffList.Add( LOCTEXT("On","ON") );

	TArray<FText> StateList;
	StateList.Add(LOCTEXT("Start", "START"));
	StateList.Add(LOCTEXT("Pause", "PAUSE"));
	StateList.Add(LOCTEXT("Stop", "STOP"));

	SoldierOptions = MakeShareable(new FSoldierOptions()); 
	SoldierOptions->Construct(GetPlayerOwner());
	SoldierOptions->TellInputAboutKeybindings();
	SoldierOptions->OnApplyChanges.BindSP(this, &FSoldierMainMenu::CloseSubMenu);

	//Now that we are here, build our menu 
	MenuWidget.Reset();
	MenuWidgetContainer.Reset();

	TArray<FString> Keys;
	GConfig->GetSingleLineArray(TEXT("/Script/SwitchRuntimeSettings.SwitchRuntimeSettings"), TEXT("LeaderboardMap"), Keys, GEngineIni);

	if (GEngine && GEngine->GameViewport)
	{		
		SAssignNew(MenuWidget, SSoldierMenuWidget)
			.Cursor(EMouseCursor::Default)
			.PlayerOwner(GetPlayerOwner())
			.IsGameMenu(false);

		SAssignNew(MenuWidgetContainer, SWeakWidget)
			.PossiblyNullContent(MenuWidget);	
				
		SAssignNew(SplitScreenLobbyWidget, SSoldierSplitScreenLobby)
			.PlayerOwner(GetPlayerOwner())
			.OnCancelClicked(FOnClicked::CreateSP(this, &FSoldierMainMenu::OnSplitScreenBackedOut)) 
			.OnPlayClicked(FOnClicked::CreateSP(this, &FSoldierMainMenu::OnSplitScreenPlay));

		FText Msg = LOCTEXT("No matches could be found", "No matches could be found");
		FText OKButtonString = NSLOCTEXT("DialogButtons", "OKAY", "OK");
		QuickMatchFailureWidget = SNew(SArmySimConfirmationDialog).PlayerOwner(PlayerOwner)			
			.MessageText(Msg)
			.ConfirmText(OKButtonString)
			.CancelText(FText())
			.OnConfirmClicked(FOnClicked::CreateRaw(this, &FSoldierMainMenu::OnQuickMatchFailureUICancel))
			.OnCancelClicked(FOnClicked::CreateRaw(this, &FSoldierMainMenu::OnQuickMatchFailureUICancel));

		Msg = LOCTEXT("Searching for Match...", "SEARCHING FOR MATCH...");
		OKButtonString = LOCTEXT("Stop", "STOP");
		QuickMatchSearchingWidget = SNew(SArmySimConfirmationDialog).PlayerOwner(PlayerOwner)			
			.MessageText(Msg)
			.ConfirmText(OKButtonString)
			.CancelText(FText())
			.OnConfirmClicked(FOnClicked::CreateRaw(this, &FSoldierMainMenu::OnQuickMatchSearchingUICancel))
			.OnCancelClicked(FOnClicked::CreateRaw(this, &FSoldierMainMenu::OnQuickMatchSearchingUICancel));

		SAssignNew(SplitScreenLobbyWidgetContainer, SWeakWidget)
			.PossiblyNullContent(SplitScreenLobbyWidget);		

		SAssignNew(QuickMatchFailureWidgetContainer, SWeakWidget)
			.PossiblyNullContent(QuickMatchFailureWidget);	

		SAssignNew(QuickMatchSearchingWidgetContainer, SWeakWidget)
			.PossiblyNullContent(QuickMatchSearchingWidget);

		FText StoppingOKButtonString = LOCTEXT("Stopping", "STOPPING...");
		QuickMatchStoppingWidget = SNew(SArmySimConfirmationDialog).PlayerOwner(PlayerOwner)			
			.MessageText(Msg)
			.ConfirmText(StoppingOKButtonString)
			.CancelText(FText())
			.OnConfirmClicked(FOnClicked())
			.OnCancelClicked(FOnClicked());

		SAssignNew(QuickMatchStoppingWidgetContainer, SWeakWidget)
			.PossiblyNullContent(QuickMatchStoppingWidget);

#if PLATFORM_XBOXONE
		TSharedPtr<FSoldierMenuItem> MenuItem;

		// HOST ONLINE menu option
		{
			MenuItem = MenuHelper::AddMenuItemSP(RootMenuItem, LOCTEXT("HostCustom", "HOST CUSTOM"), this, &FSoldierMainMenu::OnHostOnlineSelected);

			// submenu under "HOST ONLINE"
			MenuHelper::AddMenuItemSP(MenuItem, LOCTEXT("TDMLong", "TEAM DEATHMATCH"), this, &FSoldierMainMenu::OnSplitScreenSelected);

			TSharedPtr<FSoldierMenuItem> NumberOfBotsOption = MenuHelper::AddMenuOptionSP(MenuItem, LOCTEXT("NumberOfBots", "NUMBER OF BOTS"), BotsCountList, this, &FSoldierMainMenu::BotCountOptionChanged);				
			NumberOfBotsOption->SelectedMultiChoice = BotsCountOpt;																

			HostOnlineMapOption = MenuHelper::AddMenuOption(MenuItem, LOCTEXT("SELECTED_LEVEL", "Map"), MapList);
		}

		// JOIN menu option
		{
			// JOIN menu option
			MenuHelper::AddMenuItemSP(RootMenuItem, LOCTEXT("FindCustom", "FIND CUSTOM"), this, &FSoldierMainMenu::OnJoinServer);

			// Server list widget that will be called up if appropriate
			MenuHelper::AddCustomMenuItem(JoinServerItem,SAssignNew(//ServerListWidget,SSoldierServerList).OwnerWidget(MenuWidget).PlayerOwner(GetPlayerOwner()));
		}

		// QUICK MATCH menu option
		{
			MenuHelper::AddMenuItemSP(RootMenuItem, LOCTEXT("QuickMatch", "QUICK MATCH"), this, &FSoldierMainMenu::OnQuickMatchSelected);
		}

		// HOST OFFLINE menu option
		{
			MenuItem = MenuHelper::AddMenuItemSP(RootMenuItem, LOCTEXT("PlayOffline", "PLAY OFFLINE"),this, &FSoldierMainMenu::OnHostOfflineSelected);

			// submenu under "HOST OFFLINE"
			MenuHelper::AddMenuItemSP(MenuItem, LOCTEXT("TDMLong", "TEAM DEATHMATCH"), this, &FSoldierMainMenu::OnSplitScreenSelected);

			TSharedPtr<FSoldierMenuItem> NumberOfBotsOption = MenuHelper::AddMenuOptionSP(MenuItem, LOCTEXT("NumberOfBots", "NUMBER OF BOTS"), BotsCountList, this, &FSoldierMainMenu::BotCountOptionChanged);				
			NumberOfBotsOption->SelectedMultiChoice = BotsCountOpt;																

			HostOfflineMapOption = MenuHelper::AddMenuOption(MenuItem, LOCTEXT("SELECTED_LEVEL", "Map"), MapList);
		}
#elif SHOOTER_CONSOLE_UI
		TSharedPtr<FSoldierMenuItem> MenuItem;

		// HOST ONLINE menu option
		{
			HostOnlineMenuItem = MenuHelper::AddMenuItemSP(RootMenuItem, LOCTEXT("HostOnline", "HOST ONLINE"), this, &FSoldierMainMenu::OnHostOnlineSelected);

			// submenu under "HOST ONLINE"
#if LOGIN_REQUIRED_FOR_ONLINE_PLAY
			MenuHelper::AddMenuItemSP(HostOnlineMenuItem, LOCTEXT("TDMLong", "TEAM DEATHMATCH"), this, &FSoldierMainMenu::OnSplitScreenSelectedHostOnlineLoginRequired);
#else
			MenuHelper::AddMenuItemSP(HostOnlineMenuItem, LOCTEXT("TDMLong", "TEAM DEATHMATCH"), this, &FSoldierMainMenu::OnSplitScreenSelectedHostOnline);
#endif

			TSharedPtr<FSoldierMenuItem> NumberOfBotsOption = MenuHelper::AddMenuOptionSP(HostOnlineMenuItem, LOCTEXT("NumberOfBots", "NUMBER OF BOTS"), BotsCountList, this, &FSoldierMainMenu::BotCountOptionChanged);
			NumberOfBotsOption->SelectedMultiChoice = BotsCountOpt;																

			HostOnlineMapOption = MenuHelper::AddMenuOption(HostOnlineMenuItem, LOCTEXT("SELECTED_LEVEL", "Map"), MapList);
#if CONSOLE_LAN_SUPPORTED
			HostLANItem = MenuHelper::AddMenuOptionSP(HostOnlineMenuItem, LOCTEXT("LanMatch", "LAN"), OnOffList, this, &FSoldierMainMenu::LanMatchChanged);
			HostLANItem->SelectedMultiChoice = bIsLanMatch;
#endif
		}

		// HOST OFFLINE menu option
		{
			MenuItem = MenuHelper::AddMenuItemSP(RootMenuItem, LOCTEXT("HostOffline", "HOST OFFLINE"),this, &FSoldierMainMenu::OnHostOfflineSelected);

			// submenu under "HOST OFFLINE"
			MenuHelper::AddMenuItemSP(MenuItem, LOCTEXT("TDMLong", "TEAM DEATHMATCH"), this, &FSoldierMainMenu::OnSplitScreenSelected);

			TSharedPtr<FSoldierMenuItem> NumberOfBotsOption = MenuHelper::AddMenuOptionSP(MenuItem, LOCTEXT("NumberOfBots", "NUMBER OF BOTS"), BotsCountList, this, &FSoldierMainMenu::BotCountOptionChanged);				
			NumberOfBotsOption->SelectedMultiChoice = BotsCountOpt;																

			HostOfflineMapOption = MenuHelper::AddMenuOption(MenuItem, LOCTEXT("SELECTED_LEVEL", "Map"), MapList);
		}

		// QUICK MATCH menu option
		{
#if LOGIN_REQUIRED_FOR_ONLINE_PLAY
			MenuHelper::AddMenuItemSP(RootMenuItem, LOCTEXT("QuickMatch", "QUICK MATCH"), this, &FSoldierMainMenu::OnQuickMatchSelectedLoginRequired);
#else
			MenuHelper::AddMenuItemSP(RootMenuItem, LOCTEXT("QuickMatch", "QUICK MATCH"), this, &FSoldierMainMenu::OnQuickMatchSelected);
#endif
		}

		// JOIN menu option
		{
			// JOIN menu option
			MenuItem = MenuHelper::AddMenuItemSP(RootMenuItem, LOCTEXT("Join", "JOIN"), this, &FSoldierMainMenu::OnJoinSelected);

			// submenu under "join"
#if LOGIN_REQUIRED_FOR_ONLINE_PLAY
			MenuHelper::AddMenuItemSP(MenuItem, LOCTEXT("Server", "SERVER"), this, &FSoldierMainMenu::OnJoinServerLoginRequired);
#else
			MenuHelper::AddMenuItemSP(MenuItem, LOCTEXT("Server", "SERVER"), this, &FSoldierMainMenu::OnJoinServer);
#endif
			JoinMapOption = MenuHelper::AddMenuOption(MenuItem, LOCTEXT("SELECTED_LEVEL", "Map"), JoinMapList);

			// Server list widget that will be called up if appropriate
			MenuHelper::AddCustomMenuItem(JoinServerItem,SAssignNew(//ServerListWidget,SSoldierServerList).OwnerWidget(MenuWidget).PlayerOwner(GetPlayerOwner()));

#if CONSOLE_LAN_SUPPORTED
			JoinLANItem = MenuHelper::AddMenuOptionSP(MenuItem, LOCTEXT("LanMatch", "LAN"), OnOffList, this, &FSoldierMainMenu::LanMatchChanged);
			JoinLANItem->SelectedMultiChoice = bIsLanMatch;
#endif
		}

#else
		TSharedPtr<FSoldierMenuItem> MenuItem;
		// HOST menu option
		MenuItem = MenuHelper::AddMenuItem(RootMenuItem, LOCTEXT("Host", "HOST"));

		// submenu under "host"
		MenuHelper::AddMenuItemSP(MenuItem, LOCTEXT("FFALong", "ReStart"), this, &FSoldierMainMenu::OnUIHostFreeForAll);
		//MenuHelper::AddMenuItemSP(MenuItem, LOCTEXT("TDMLong", "TEAM DEATHMATCH"), this, &FSoldierMainMenu::OnUIHostTeamDeathMatch);

		TSharedPtr<FSoldierMenuItem> NumberOfBotsOption = MenuHelper::AddMenuOptionSP(MenuItem, LOCTEXT("NumberOfBots", "NUMBER OF BOTS"), BotsCountList, this, &FSoldierMainMenu::BotCountOptionChanged);
		NumberOfBotsOption->SelectedMultiChoice = BotsCountOpt;

		HostOnlineMapOption = MenuHelper::AddMenuOption(MenuItem, LOCTEXT("SELECTED_LEVEL", "Map"), MapList);

		HostLANItem = MenuHelper::AddMenuOptionSP(MenuItem, LOCTEXT("LanMatch", "LAN"), OnOffList, this, &FSoldierMainMenu::LanMatchChanged);
		HostLANItem->SelectedMultiChoice = bIsLanMatch;

		RecordDemoItem = MenuHelper::AddMenuOptionSP(MenuItem, LOCTEXT("RecordDemo", "Record Demo"), OnOffList, this, &FSoldierMainMenu::RecordDemoChanged);
		RecordDemoItem->SelectedMultiChoice = bIsRecordingDemo;

		MenuStateItem = MenuHelper::AddMenuOptionSP(MenuItem, LOCTEXT("State", "State"), StateList, this, &FSoldierMainMenu::ProcessStateChanged);
		MenuStateItem->SelectedMultiChoice = bIsRecordingDemo;

		LightItem = MenuHelper::AddMenuOptionSP(MenuItem, LOCTEXT("Light", "Light"), OnOffList, this, &FSoldierMainMenu::LightChanged);

		// JOIN menu option
		//MenuItem = MenuHelper::AddMenuItem(RootMenuItem, LOCTEXT("Join", "JOIN"));

		// submenu under "join"
		/*MenuHelper::AddMenuItemSP(MenuItem, LOCTEXT("Server", "SERVER"), this, &FSoldierMainMenu::OnJoinServer);
		JoinLANItem = MenuHelper::AddMenuOptionSP(MenuItem, LOCTEXT("LanMatch", "LAN"), OnOffList, this, &FSoldierMainMenu::LanMatchChanged);
		JoinLANItem->SelectedMultiChoice = bIsLanMatch;

		DedicatedItem = MenuHelper::AddMenuOptionSP(MenuItem, LOCTEXT("Dedicated", "Dedicated"), OnOffList, this, &FSoldierMainMenu::DedicatedServerChanged);
		DedicatedItem->SelectedMultiChoice = bIsDedicatedServer;*/

		// Server list widget that will be called up if appropriate
	//	MenuHelper::AddCustomMenuItem(JoinServerItem,SAssignNew(//ServerListWidget,SSoldierServerList).OwnerWidget(MenuWidget).PlayerOwner(GetPlayerOwner()));
#endif

		// Leaderboards
		//MenuHelper::AddMenuItemSP(RootMenuItem, LOCTEXT("Leaderboards", "LEADERBOARDS"), this, &FSoldierMainMenu::OnShowLeaderboard);
		//MenuHelper::AddCustomMenuItem(LeaderboardItem,SAssignNew(LeaderboardWidget,SSoldierLeaderboard).OwnerWidget(MenuWidget).PlayerOwner(GetPlayerOwner()));

		// Purchases
		//MenuHelper::AddMenuItemSP(RootMenuItem, LOCTEXT("Store", "ONLINE STORE"), this, &FSoldierMainMenu::OnShowOnlineStore);
		//MenuHelper::AddCustomMenuItem(OnlineStoreItem, SAssignNew(OnlineStoreWidget, SSoldierOnlineStore).OwnerWidget(MenuWidget).PlayerOwner(GetPlayerOwner()));

#if !SHOOTER_CONSOLE_UI

		//SceneObjects UI
		{
			MenuHelper::AddMenuItemSP( RootMenuItem, LOCTEXT( "Objects", "Scene Objects" ), this, &FSoldierMainMenu::OnShowSceneObjectsBrowser );
			MenuHelper::AddCustomMenuItem( SceneObjectsBrowserItem,SAssignNew( SceneObjectsListWidget, SSceneObjectsList ).OwnerWidget( MenuWidget ).PlayerOwner( GetPlayerOwner( ) ) );

			MenuHelper::AddMenuItemSP(RootMenuItem, LOCTEXT("Weather", "WeathSettings"), this, &FSoldierMainMenu::OnShowWeatherSettings);
			MenuHelper::AddCustomMenuItem(WeatherSettingsItem, SAssignNew(WeatherSettingWidget, SWeatherSettingWidget).OwnerWidget(MenuWidget).PlayerOwner(GetPlayerOwner()));
		}
#endif

		// Options
		MenuHelper::AddExistingMenuItem(RootMenuItem, SoldierOptions->OptionsItem.ToSharedRef());

		if(FSlateApplication::Get().SupportsSystemHelp())
		{
			TSharedPtr<FSoldierMenuItem> HelpSubMenu = MenuHelper::AddMenuItem(RootMenuItem, LOCTEXT("Help", "HELP"));
			HelpSubMenu->OnConfirmMenuItem.BindStatic([](){ FSlateApplication::Get().ShowSystemHelp(); });
		}

		// QUIT option (for PC)
#if !SHOOTER_CONSOLE_UI
		MenuHelper::AddMenuItemSP(RootMenuItem, LOCTEXT("Quit", "QUIT"), this, &FSoldierMainMenu::OnUIQuit);
#endif

		MenuWidget->CurrentMenuTitle = LOCTEXT("MainMenu","MAIN MENU");
		MenuWidget->OnGoBack.BindSP(this, &FSoldierMainMenu::OnMenuGoBack);
		MenuWidget->MainMenu = MenuWidget->CurrentMenu = RootMenuItem->SubMenu;
		MenuWidget->OnMenuHidden.BindSP(this, &FSoldierMainMenu::OnMenuHidden);

		SoldierOptions->UpdateOptions();
		MenuWidget->BuildAndShowMenu();
	}
}

void FSoldierMainMenu::AddMenuToGameViewport()
{
	if (GEngine && GEngine->GameViewport)
	{
		UGameViewportClient* GVC = GEngine->GameViewport;
		GVC->AddViewportWidgetContent(MenuWidgetContainer.ToSharedRef());
		GVC->SetCaptureMouseOnClick(EMouseCaptureMode::NoCapture);
		bIsGameMainMenuUp = true;
	}
}

void FSoldierMainMenu::RemoveMenuFromGameViewport()
{
	if (GEngine && GEngine->GameViewport)
	{
		GEngine->GameViewport->RemoveViewportWidgetContent(MenuWidgetContainer.ToSharedRef());
	}
}

void FSoldierMainMenu::Tick(float DeltaSeconds)
{
	if (bAnimateQuickmatchSearchingUI)
	{
		FLinearColor QuickMColor = QuickMatchSearchingWidget->GetColorAndOpacity();
		if (bIncQuickMAlpha)
		{
			if (QuickMColor.A >= 1.f)
			{
				bIncQuickMAlpha = false;
			}
			else
			{
				QuickMColor.A += DeltaSeconds;
			}
		}
		else
		{
			if (QuickMColor.A <= .1f)
			{
				bIncQuickMAlpha = true;
			}
			else
			{
				QuickMColor.A -= DeltaSeconds;
			}
		}
		QuickMatchSearchingWidget->SetColorAndOpacity(QuickMColor);
		QuickMatchStoppingWidget->SetColorAndOpacity(QuickMColor);
	}

	IPlatformChunkInstall* ChunkInstaller = FPlatformMisc::GetPlatformChunkInstall();
	if (ChunkInstaller)
	{
		EMap SelectedMap = GetSelectedMap();
		// use assetregistry when maps are added to it.
		int32 MapChunk = ChunkMapping[(int)SelectedMap];
		EChunkLocation::Type ChunkLocation = ChunkInstaller->GetChunkLocation(MapChunk);

		FText UpdatedText;
		bool bUpdateText = false;
		if (ChunkLocation == EChunkLocation::NotAvailable)
		{			
			float PercentComplete = FMath::Min(ChunkInstaller->GetChunkProgress(MapChunk, EChunkProgressReportingType::PercentageComplete), 100.0f);									
			UpdatedText = FText::FromString(FString::Printf(TEXT("%s %4.0f%%"),*LOCTEXT("SELECTED_LEVEL", "Map").ToString(), PercentComplete));
			bUpdateText = true;
			bShowingDownloadPct = true;
		}
		else if (bShowingDownloadPct)
		{
			UpdatedText = LOCTEXT("SELECTED_LEVEL", "Map");			
			bUpdateText = true;
			bShowingDownloadPct = false;			
		}

		if (bUpdateText)
		{
			if (HostOfflineMapOption.IsValid())
			{
				HostOfflineMapOption->SetText(UpdatedText);
			}
		}
	}
}

TStatId FSoldierMainMenu::GetStatId() const
{
	RETURN_QUICK_DECLARE_CYCLE_STAT(FSoldierMainMenu, STATGROUP_Tickables);
}

void FSoldierMainMenu::OnMenuHidden()
{	
#if SHOOTER_CONSOLE_UI
	// Menu was hidden from the top-level main menu, on consoles show the welcome screen again.
	if ( ensure(GameInstance.IsValid()))
	{
		GameInstance->GotoState(ArmySimGameInstanceState::WelcomeScreen);
	}
#else
	RemoveMenuFromGameViewport();
#endif
}

void FSoldierMainMenu::OnQuickMatchSelectedLoginRequired()
{
	IOnlineIdentityPtr Identity = Online::GetIdentityInterface(GetTickableGameObjectWorld());
	if (Identity.IsValid())
	{
		int32 ControllerId = GetPlayerOwner()->GetControllerId();

		OnLoginCompleteDelegateHandle = Identity->AddOnLoginCompleteDelegate_Handle(ControllerId, FOnLoginCompleteDelegate::CreateRaw(this, &FSoldierMainMenu::OnLoginCompleteQuickmatch));
		Identity->Login(ControllerId, FOnlineAccountCredentials());
	}
}

void FSoldierMainMenu::OnLoginCompleteQuickmatch(int32 LocalUserNum, bool bWasSuccessful, const FUniqueNetId& UserId, const FString& Error)
{
	IOnlineIdentityPtr Identity = Online::GetIdentityInterface(GetTickableGameObjectWorld());
	if (Identity.IsValid())
	{
		Identity->ClearOnLoginCompleteDelegate_Handle(LocalUserNum, OnLoginCompleteDelegateHandle);
	}

	OnQuickMatchSelected();
}

void FSoldierMainMenu::OnQuickMatchSelected()
{
	bQuickmatchSearchRequestCanceled = false;
#if SHOOTER_CONSOLE_UI
	if ( !ValidatePlayerForOnlinePlay(GetPlayerOwner()) )
	{
		return;
	}
#endif

	StartOnlinePrivilegeTask(IOnlineIdentity::FOnGetUserPrivilegeCompleteDelegate::CreateSP(this, &FSoldierMainMenu::OnUserCanPlayOnlineQuickMatch));
}

void FSoldierMainMenu::OnUserCanPlayOnlineQuickMatch(const FUniqueNetId& UserId, EUserPrivileges::Type Privilege, uint32 PrivilegeResults)
{
	CleanupOnlinePrivilegeTask();
	MenuWidget->LockControls(false);
	if (PrivilegeResults == (uint32)IOnlineIdentity::EPrivilegeResults::NoFailures)
	{
		if (GameInstance.IsValid())
		{
			//GameInstance->SetOnlineMode(EOnlineMode::Online);
		}

		MatchType = EMatchType::Quick;

		SplitScreenLobbyWidget->SetIsJoining(false);

		// Skip splitscreen for PS4
#if PLATFORM_PS4 || MAX_LOCAL_PLAYERS == 1
		BeginQuickMatchSearch();
#else
		UGameViewportClient* const GVC = GEngine->GameViewport;

		RemoveMenuFromGameViewport();
		GVC->AddViewportWidgetContent(SplitScreenLobbyWidgetContainer.ToSharedRef());

		SplitScreenLobbyWidget->Clear();
		FSlateApplication::Get().SetKeyboardFocus(SplitScreenLobbyWidget);
#endif
	}
	else if (GameInstance.IsValid())
	{

		GameInstance->DisplayOnlinePrivilegeFailureDialogs(UserId, Privilege, PrivilegeResults);
	}
}

FReply FSoldierMainMenu::OnConfirmGeneric()
{
	UArmySimGameViewportClient* SoldierViewport = Cast<UArmySimGameViewportClient>(GameInstance->GetGameViewportClient());
	if (SoldierViewport)
	{
		SoldierViewport->HideDialog();
	}

	return FReply::Handled();
}

void FSoldierMainMenu::BeginQuickMatchSearch()
{
	
}


void FSoldierMainMenu::OnSplitScreenSelectedHostOnlineLoginRequired()
{
	IOnlineIdentityPtr Identity = Online::GetIdentityInterface(GetTickableGameObjectWorld());
	if (Identity.IsValid())
	{
		int32 ControllerId = GetPlayerOwner()->GetControllerId();

		if (bIsLanMatch)
		{
			Identity->Logout(ControllerId);
			OnSplitScreenSelected();
		}
		else
		{
			OnLoginCompleteDelegateHandle = Identity->AddOnLoginCompleteDelegate_Handle(ControllerId, FOnLoginCompleteDelegate::CreateRaw(this, &FSoldierMainMenu::OnLoginCompleteHostOnline));
			Identity->Login(ControllerId, FOnlineAccountCredentials());
		}
	}
}

void FSoldierMainMenu::OnSplitScreenSelected()
{
	if (!IsMapReady())
	{
		return;
	}

	RemoveMenuFromGameViewport();

#if PLATFORM_PS4 || MAX_LOCAL_PLAYERS == 1
	if (GameInstance.IsValid())
	{
		OnUIHostTeamDeathMatch();
	}
	else
#endif
	{
		UGameViewportClient* const GVC = GEngine->GameViewport;
		GVC->AddViewportWidgetContent(SplitScreenLobbyWidgetContainer.ToSharedRef());

		SplitScreenLobbyWidget->Clear();
		FSlateApplication::Get().SetKeyboardFocus(SplitScreenLobbyWidget);
	}
}

void FSoldierMainMenu::OnHostOnlineSelected()
{
#if SHOOTER_CONSOLE_UI
	if (!ValidatePlayerIsSignedIn(GetPlayerOwner()))
	{
		return;
	}
#endif

	MatchType = EMatchType::Custom;

	/*EOnlineMode NewOnlineMode = bIsLanMatch ? EOnlineMode::LAN : EOnlineMode::Online;
	if (GameInstance.IsValid())
	{
		//GameInstance->SetOnlineMode(NewOnlineMode);
	}*/
	SplitScreenLobbyWidget->SetIsJoining(false);
	MenuWidget->EnterSubMenu();
}

void FSoldierMainMenu::OnUserCanPlayHostOnline(const FUniqueNetId& UserId, EUserPrivileges::Type Privilege, uint32 PrivilegeResults)
{
	CleanupOnlinePrivilegeTask();
	MenuWidget->LockControls(false);
	if (PrivilegeResults == (uint32)IOnlineIdentity::EPrivilegeResults::NoFailures)	
	{
		OnSplitScreenSelected();
	}
	else if (GameInstance.IsValid())
	{
		GameInstance->DisplayOnlinePrivilegeFailureDialogs(UserId, Privilege, PrivilegeResults);
	}
}

void FSoldierMainMenu::OnLoginCompleteHostOnline(int32 LocalUserNum, bool bWasSuccessful, const FUniqueNetId& UserId, const FString& Error)
{
	Online::GetIdentityInterface(GetTickableGameObjectWorld())->ClearOnLoginCompleteDelegate_Handle(LocalUserNum, OnLoginCompleteDelegateHandle);

	OnSplitScreenSelectedHostOnline();
}

void FSoldierMainMenu::OnSplitScreenSelectedHostOnline()
{
#if SHOOTER_CONSOLE_UI
	if (!ValidatePlayerForOnlinePlay(GetPlayerOwner()))
	{
		return;
	}
#endif

	StartOnlinePrivilegeTask(IOnlineIdentity::FOnGetUserPrivilegeCompleteDelegate::CreateSP(this, &FSoldierMainMenu::OnUserCanPlayHostOnline));
}
void FSoldierMainMenu::StartOnlinePrivilegeTask(const IOnlineIdentity::FOnGetUserPrivilegeCompleteDelegate& Delegate)
{
	if (GameInstance.IsValid())
	{
		// Lock controls for the duration of the async task
		MenuWidget->LockControls(true);
		FUniqueNetIdRepl UserId;
		if (PlayerOwner.IsValid())
		{
			UserId = PlayerOwner->GetPreferredUniqueNetId();
		}
		//GameInstance->StartOnlinePrivilegeTask(Delegate, EUserPrivileges::CanPlayOnline, UserId.GetUniqueNetId());
	}	
}

void FSoldierMainMenu::CleanupOnlinePrivilegeTask()
{
	if (GameInstance.IsValid())
	{
		//GameInstance->CleanupOnlinePrivilegeTask();
	}
}

void FSoldierMainMenu::OnHostOfflineSelected()
{
	MatchType = EMatchType::Custom;

#if LOGIN_REQUIRED_FOR_ONLINE_PLAY
	Online::GetIdentityInterface(GetTickableGameObjectWorld())->Logout(GetPlayerOwner()->GetControllerId());
#endif

	//if (GameInstance.IsValid())
	//{
	//	//GameInstance->SetOnlineMode(EOnlineMode::Offline);
	//}
	SplitScreenLobbyWidget->SetIsJoining( false );

	MenuWidget->EnterSubMenu();
}

FReply FSoldierMainMenu::OnSplitScreenBackedOut()
{	
	SplitScreenLobbyWidget->Clear();
	SplitScreenBackedOut();
	return FReply::Handled();
}

FReply FSoldierMainMenu::OnSplitScreenPlay()
{
	switch ( MatchType )
	{
		case EMatchType::Custom:
		{
#if SHOOTER_CONSOLE_UI
			if ( SplitScreenLobbyWidget->GetIsJoining() )
			{
#if 1
				// Until we can make split-screen menu support sub-menus, we need to do it this way
				if (GEngine && GEngine->GameViewport)
				{
					GEngine->GameViewport->RemoveViewportWidgetContent(SplitScreenLobbyWidgetContainer.ToSharedRef());
				}
				AddMenuToGameViewport();

				FSlateApplication::Get().SetKeyboardFocus(MenuWidget);	

				// Grab the map filter if there is one
				FString SelectedMapFilterName = TEXT("ANY");
				if (JoinMapOption.IsValid())
				{
					int32 FilterChoice = JoinMapOption->SelectedMultiChoice;
					if (FilterChoice != INDEX_NONE)
					{
						SelectedMapFilterName = JoinMapOption->MultiChoice[FilterChoice].ToString();
					}
				}


				MenuWidget->NextMenu = JoinServerItem->SubMenu;
				//ServerListWidget->BeginServerSearch(bIsLanMatch, bIsDedicatedServer, SelectedMapFilterName);
				//ServerListWidget->UpdateServerList();
				MenuWidget->EnterSubMenu();
#else
				SplitScreenLobbyWidget->NextMenu = JoinServerItem->SubMenu;
				//ServerListWidget->BeginServerSearch(bIsLanMatch, bIsDedicatedServer, SelectedMapFilterName);
				//ServerListWidget->UpdateServerList();
				SplitScreenLobbyWidget->EnterSubMenu();
#endif
			}
			else
#endif
			{
				if (GEngine && GEngine->GameViewport)
				{
					GEngine->GameViewport->RemoveViewportWidgetContent(SplitScreenLobbyWidgetContainer.ToSharedRef());
				}
				OnUIHostTeamDeathMatch();
			}
			break;
		}

		case EMatchType::Quick:
		{
			if (GEngine && GEngine->GameViewport)
			{
				GEngine->GameViewport->RemoveViewportWidgetContent(SplitScreenLobbyWidgetContainer.ToSharedRef());
			}
			BeginQuickMatchSearch();
			break;
		}
	}

	return FReply::Handled();
}

void FSoldierMainMenu::SplitScreenBackedOut()
{
	if (GEngine && GEngine->GameViewport)
	{
		GEngine->GameViewport->RemoveViewportWidgetContent(SplitScreenLobbyWidgetContainer.ToSharedRef());	
	}
	AddMenuToGameViewport();

	FSlateApplication::Get().SetKeyboardFocus(MenuWidget);	
}

void FSoldierMainMenu::HelperQuickMatchSearchingUICancel(bool bShouldRemoveSession)
{
	IOnlineSessionPtr Sessions = Online::GetSessionInterface(GetTickableGameObjectWorld());
	if (bShouldRemoveSession && Sessions.IsValid())
	{
		if (PlayerOwner.IsValid() && PlayerOwner->GetPreferredUniqueNetId().IsValid())
		{
			UGameViewportClient* const GVC = GEngine->GameViewport;
			GVC->RemoveViewportWidgetContent(QuickMatchSearchingWidgetContainer.ToSharedRef());
			GVC->AddViewportWidgetContent(QuickMatchStoppingWidgetContainer.ToSharedRef());
			FSlateApplication::Get().SetKeyboardFocus(QuickMatchStoppingWidgetContainer);
			
			OnCancelMatchmakingCompleteDelegateHandle = Sessions->AddOnCancelMatchmakingCompleteDelegate_Handle(OnCancelMatchmakingCompleteDelegate);
			Sessions->CancelMatchmaking(*PlayerOwner->GetPreferredUniqueNetId(), NAME_GameSession);
		}
	}
	else
	{
		UGameViewportClient* const GVC = GEngine->GameViewport;
		GVC->RemoveViewportWidgetContent(QuickMatchSearchingWidgetContainer.ToSharedRef());
		AddMenuToGameViewport();
		FSlateApplication::Get().SetKeyboardFocus(MenuWidget);
	}
}

FReply FSoldierMainMenu::OnQuickMatchSearchingUICancel()
{
	HelperQuickMatchSearchingUICancel(true);
	bUsedInputToCancelQuickmatchSearch = true;
	bQuickmatchSearchRequestCanceled = true;
	return FReply::Handled();
}

FReply FSoldierMainMenu::OnQuickMatchFailureUICancel()
{
	if (GEngine && GEngine->GameViewport)
	{
		GEngine->GameViewport->RemoveViewportWidgetContent(QuickMatchFailureWidgetContainer.ToSharedRef());
	}
	AddMenuToGameViewport();
	FSlateApplication::Get().SetKeyboardFocus(MenuWidget);
	return FReply::Handled();
}

void FSoldierMainMenu::DisplayQuickmatchFailureUI()
{
	UGameViewportClient* const GVC = GEngine->GameViewport;
	RemoveMenuFromGameViewport();
	GVC->AddViewportWidgetContent(QuickMatchFailureWidgetContainer.ToSharedRef());
	FSlateApplication::Get().SetKeyboardFocus(QuickMatchFailureWidget);
}

void FSoldierMainMenu::DisplayQuickmatchSearchingUI()
{
	UGameViewportClient* const GVC = GEngine->GameViewport;
	RemoveMenuFromGameViewport();
	GVC->AddViewportWidgetContent(QuickMatchSearchingWidgetContainer.ToSharedRef());
	FSlateApplication::Get().SetKeyboardFocus(QuickMatchSearchingWidget);
	bAnimateQuickmatchSearchingUI = true;
}

void FSoldierMainMenu::OnMatchmakingComplete(FName SessionName, bool bWasSuccessful)
{
	IOnlineSessionPtr SessionInterface = Online::GetSessionInterface(GetTickableGameObjectWorld());
	if (!SessionInterface.IsValid())
	{
		UE_LOG(LogOnline, Warning, TEXT("OnMatchmakingComplete: Couldn't find session interface."));
		return;
	}

	SessionInterface->ClearOnMatchmakingCompleteDelegate_Handle(OnMatchmakingCompleteDelegateHandle);

	if (bQuickmatchSearchRequestCanceled && bUsedInputToCancelQuickmatchSearch)
	{
		bQuickmatchSearchRequestCanceled = false;
		// Clean up the session in case we get this event after canceling
		if (bWasSuccessful)
		{
			if (PlayerOwner.IsValid() && PlayerOwner->GetPreferredUniqueNetId().IsValid())
			{
				SessionInterface->DestroySession(NAME_GameSession);
			}
		}
		return;
	}

	if (bAnimateQuickmatchSearchingUI)
	{
		bAnimateQuickmatchSearchingUI = false;
		HelperQuickMatchSearchingUICancel(false);
		bUsedInputToCancelQuickmatchSearch = false;
	}
	else
	{
		return;
	}

	if (!bWasSuccessful)
	{
		UE_LOG(LogOnline, Warning, TEXT("Matchmaking was unsuccessful."));
		DisplayQuickmatchFailureUI();
		return;
	}

	UE_LOG(LogOnline, Log, TEXT("Matchmaking successful! Session name is %s."), *SessionName.ToString());

	if (GetPlayerOwner() == NULL)
	{
		UE_LOG(LogOnline, Warning, TEXT("OnMatchmakingComplete: No owner."));
		return;
	}

	FNamedOnlineSession* MatchmadeSession = SessionInterface->GetNamedSession(SessionName);

	if (!MatchmadeSession)
	{
		UE_LOG(LogOnline, Warning, TEXT("OnMatchmakingComplete: No session."));
		return;
	}

	if(!MatchmadeSession->OwningUserId.IsValid())
	{
		UE_LOG(LogOnline, Warning, TEXT("OnMatchmakingComplete: No session owner/host."));
		return;
	}

	if (GEngine && GEngine->GameViewport)
	{
		GEngine->GameViewport->RemoveViewportWidgetContent(QuickMatchSearchingWidgetContainer.ToSharedRef());
	}
	bAnimateQuickmatchSearchingUI = false;

	UE_LOG(LogOnline, Log, TEXT("OnMatchmakingComplete: Session host is %d."), *MatchmadeSession->OwningUserId->ToString());

	if (ensure(GameInstance.IsValid()))
	{
		MenuWidget->LockControls(true);

		IOnlineSubsystem* Subsystem = Online::GetSubsystem(GetTickableGameObjectWorld());
		if (Subsystem != nullptr && Subsystem->IsLocalPlayer(*MatchmadeSession->OwningUserId))
		{
			// This console is the host, start the map.
			//GameInstance->BeginHostingQuickMatch();
		}
		else
		{
			// We are the client, join the host.
		//	GameInstance->TravelToSession(SessionName);
		}
	}
}

FSoldierMainMenu::EMap FSoldierMainMenu::GetSelectedMap() const
{
   if (HostOfflineMapOption.IsValid())
	{
		return (EMap)HostOfflineMapOption->SelectedMultiChoice;
	}

	return EMap::ESancturary;	// Need to return something (we can hit this path in cooking)
}

void FSoldierMainMenu::CloseSubMenu()
{
	MenuWidget->MenuGoBack(true);
}

void FSoldierMainMenu::OnMenuGoBack(MenuPtr Menu)
{
	// if we are going back from options menu
	if (SoldierOptions->OptionsItem->SubMenu == Menu)
	{
		SoldierOptions->RevertChanges();
	}

	// In case a Play Together event was received, don't act on it
	// if the player changes their mind.
	if (HostOnlineMenuItem.IsValid() && HostOnlineMenuItem->SubMenu == Menu)
	{
	//	GameInstance->ResetPlayTogetherInfo();
	}

	// if we've backed all the way out we need to make sure online is false.
	/*if (MenuWidget->GetMenuLevel() == 1)
	{
		//GameInstance->SetOnlineMode(EOnlineMode::Offline);
	}*/
}

void FSoldierMainMenu::BotCountOptionChanged(TSharedPtr<FSoldierMenuItem> MenuItem, int32 MultiOptionIndex)
{
	BotsCountOpt = MultiOptionIndex;

	if(GetPersistentUser())
	{
		GetPersistentUser()->SetBotsCount(BotsCountOpt);
	}
}

void FSoldierMainMenu::LanMatchChanged(TSharedPtr<FSoldierMenuItem> MenuItem, int32 MultiOptionIndex)
{
	if (HostLANItem.IsValid())
	{
		HostLANItem->SelectedMultiChoice = MultiOptionIndex;
	}

	check(JoinLANItem.IsValid());
	JoinLANItem->SelectedMultiChoice = MultiOptionIndex;
	bIsLanMatch = true;// MultiOptionIndex > 0;
	UArmySimGameUserSettings* UserSettings = CastChecked<UArmySimGameUserSettings>(GEngine->GetGameUserSettings());
	UserSettings->SetLanMatch(bIsLanMatch);

	/*EOnlineMode NewOnlineMode = bIsLanMatch ? EOnlineMode::LAN : EOnlineMode::Online;
	if (GameInstance.IsValid())
	{
		//GameInstance->SetOnlineMode(NewOnlineMode);
	}*/
}

//开关灯
void FSoldierMainMenu::LightChanged(TSharedPtr<FSoldierMenuItem> MenuItem, int32 MultiOptionIndex)
{
	IGameModeInterface::Execute_SwitchSceneLight(UGameplayStatics::GetGameMode(GWorld), MultiOptionIndex>0?true:false);
}

//开始，暂停，结束
void FSoldierMainMenu::ProcessStateChanged(TSharedPtr<FSoldierMenuItem> MenuItem, int32 MultiOptionIndex)
{
	if (MultiOptionIndex == 0)
	{
		IGameModeInterface::Execute_ReStartMission(UGameplayStatics::GetGameMode(GWorld));
	}
	else if (MultiOptionIndex == 1)
	{
		IGameModeInterface::Execute_PauseMission(UGameplayStatics::GetGameMode(GWorld));
	}
	else if (MultiOptionIndex == 2)
	{
		IGameModeInterface::Execute_StopMission(UGameplayStatics::GetGameMode(GWorld));
	}
}

void FSoldierMainMenu::DedicatedServerChanged(TSharedPtr<FSoldierMenuItem> MenuItem, int32 MultiOptionIndex)
{
	check(DedicatedItem.IsValid());
	DedicatedItem->SelectedMultiChoice = MultiOptionIndex;
	bIsDedicatedServer = MultiOptionIndex > 0;
	UArmySimGameUserSettings* UserSettings = CastChecked<UArmySimGameUserSettings>(GEngine->GetGameUserSettings());
	UserSettings->SetDedicatedServer(bIsDedicatedServer);
}

void FSoldierMainMenu::RecordDemoChanged(TSharedPtr<FSoldierMenuItem> MenuItem, int32 MultiOptionIndex)
{
	if (RecordDemoItem.IsValid())
	{
		RecordDemoItem->SelectedMultiChoice = MultiOptionIndex;
	}

	bIsRecordingDemo = MultiOptionIndex > 0;

	if(GetPersistentUser())
	{
		GetPersistentUser()->SetIsRecordingDemos(bIsRecordingDemo);
		GetPersistentUser()->SaveIfDirty();
	}
}

void FSoldierMainMenu::OnUIHostFreeForAll()
{
#if WITH_EDITOR
	if (GIsEditor == true)
	{
		return;
	}
#endif
	if (!IsMapReady())
	{
		return;
	}

#if !SHOOTER_CONSOLE_UI
	if (GameInstance.IsValid())
	{
		bIsLanMatch = true;
		GameInstance->SetOnlineMode(bIsLanMatch ? EOnlineMode::LAN : EOnlineMode::Online);
	}
#endif

	MenuWidget->LockControls(true);
	MenuWidget->HideMenu();

	UWorld* World = GetTickableGameObjectWorld();
	const int32 ControllerId = GetPlayerOwnerControllerId();

	if (World && ControllerId != -1)
	{
		const FSimMenuSoundsStyle& MenuSounds = FArmySimStyle::Get().GetWidgetStyle<FSimMenuSoundsStyle>("DefaultSoldierMenuSoundsStyle");
		MenuHelper::PlaySoundAndCall(World, MenuSounds.StartGameSound, ControllerId, this, &FSoldierMainMenu::HostFreeForAll);
	}
}

void FSoldierMainMenu::OnUIHostTeamDeathMatch()
{
#if WITH_EDITOR
	if (GIsEditor == true)
	{
		return;
	}
#endif
	if (!IsMapReady())
	{
		return;
	}

#if !SHOOTER_CONSOLE_UI
	if (GameInstance.IsValid())
	{
		//GameInstance->SetOnlineMode(bIsLanMatch ? EOnlineMode::LAN : EOnlineMode::Online);
	}
#endif

	MenuWidget->LockControls(true);
	MenuWidget->HideMenu();

	if (GetTickableGameObjectWorld() && GetPlayerOwnerControllerId() != -1)
	{
		const FSimMenuSoundsStyle& MenuSounds = FArmySimStyle::Get().GetWidgetStyle<FSimMenuSoundsStyle>("DefaultSoldierMenuSoundsStyle");
			MenuHelper::PlaySoundAndCall(GetTickableGameObjectWorld(), MenuSounds.StartGameSound, GetPlayerOwnerControllerId(), this, &FSoldierMainMenu::HostTeamDeathMatch);
	}
}

void FSoldierMainMenu::HostGame(const FString& GameType)
{	
	if (ensure(GameInstance.IsValid()) && GetPlayerOwner() != NULL)
	{
		FString const StartURL = FString::Printf(TEXT("/Game/TropicalIsland/Maps/%s?game=%s%s%s?%s=%d%s"), *GetMapName(), *GameType, GameInstance->GetOnlineMode() != EOnlineMode::Offline ? TEXT("?listen") : TEXT(""), GameInstance->GetOnlineMode() == EOnlineMode::LAN ? TEXT("?bIsLanMatch") : TEXT(""), *AOceanBoatsGameMode::GetBotsCountOptionName(), BotsCountOpt, bIsRecordingDemo ? TEXT("?DemoRec") : TEXT("") );
		//GEngine->AddOnScreenDebugMessage( -1, 18.f, FColor::Red, StartURL );
		// Game instance will handle success, failure and dialogs
		GameInstance->HostGame(GetPlayerOwner(), GameType, StartURL);
	}
}

void FSoldierMainMenu::HostFreeForAll()
{
	HostGame(TEXT("FFA"));
}

void FSoldierMainMenu::HostTeamDeathMatch()
{
	HostGame(TEXT("TDM"));
}

FReply FSoldierMainMenu::OnConfirm()
{
	if (GEngine && GEngine->GameViewport)
	{
		UArmySimGameViewportClient * SoldierViewport = Cast<UArmySimGameViewportClient>(GEngine->GameViewport);

		if (SoldierViewport)
		{
			// Hide the previous dialog
			SoldierViewport->HideDialog();
		}
	}

	return FReply::Handled();
}

bool FSoldierMainMenu::ValidatePlayerForOnlinePlay(ULocalPlayer* LocalPlayer)
{
	if (!ensure(GameInstance.IsValid()))
	{
		return false;
	}

	return false;
}

bool FSoldierMainMenu::ValidatePlayerIsSignedIn(ULocalPlayer* LocalPlayer)
{
	if (!ensure(GameInstance.IsValid()))
	{
		return false;
	}

	return GameInstance->ValidatePlayerIsSignedIn(LocalPlayer);
}

void FSoldierMainMenu::OnJoinServerLoginRequired()
{
	IOnlineIdentityPtr Identity = Online::GetIdentityInterface(GetTickableGameObjectWorld());
	if (Identity.IsValid())
	{
		int32 ControllerId = GetPlayerOwner()->GetControllerId();
	
		if (bIsLanMatch)
		{
			Identity->Logout(ControllerId);
			OnUserCanPlayOnlineJoin(*GetPlayerOwner()->GetCachedUniqueNetId(), EUserPrivileges::CanPlayOnline, (uint32)IOnlineIdentity::EPrivilegeResults::NoFailures);
		}
		else
		{
			OnLoginCompleteDelegateHandle = Identity->AddOnLoginCompleteDelegate_Handle(ControllerId, FOnLoginCompleteDelegate::CreateRaw(this, &FSoldierMainMenu::OnLoginCompleteJoin));
			Identity->Login(ControllerId, FOnlineAccountCredentials());
		}
	}
}

void FSoldierMainMenu::OnLoginCompleteJoin(int32 LocalUserNum, bool bWasSuccessful, const FUniqueNetId& UserId, const FString& Error)
{
	IOnlineIdentityPtr Identity = Online::GetIdentityInterface(GetTickableGameObjectWorld());
	if (Identity.IsValid())
	{
		Identity->ClearOnLoginCompleteDelegate_Handle(LocalUserNum, OnLoginCompleteDelegateHandle);
	}

	OnJoinServer();
}

void FSoldierMainMenu::OnJoinSelected()
{
#if SHOOTER_CONSOLE_UI
	if (!ValidatePlayerIsSignedIn(GetPlayerOwner()))
	{
		return;
	}
#endif

	MenuWidget->EnterSubMenu();
}

void FSoldierMainMenu::OnJoinServer()
{
#if SHOOTER_CONSOLE_UI
	if ( !ValidatePlayerForOnlinePlay(GetPlayerOwner()) )
	{
		return;
	}
#endif

	StartOnlinePrivilegeTask(IOnlineIdentity::FOnGetUserPrivilegeCompleteDelegate::CreateSP(this, &FSoldierMainMenu::OnUserCanPlayOnlineJoin));
}

void FSoldierMainMenu::OnUserCanPlayOnlineJoin(const FUniqueNetId& UserId, EUserPrivileges::Type Privilege, uint32 PrivilegeResults)
{
	CleanupOnlinePrivilegeTask();
	MenuWidget->LockControls(false);

	if (PrivilegeResults == (uint32)IOnlineIdentity::EPrivilegeResults::NoFailures)
	{

		//make sure to switch to custom match type so we don't instead use Quick type
		MatchType = EMatchType::Custom;

		if (GameInstance.IsValid())
		{
			//GameInstance->SetOnlineMode(bIsLanMatch ? EOnlineMode::LAN : EOnlineMode::Online);
		}

		MatchType = EMatchType::Custom;
		// Grab the map filter if there is one
		FString SelectedMapFilterName("Any");
		if( JoinMapOption.IsValid())
		{
			int32 FilterChoice = JoinMapOption->SelectedMultiChoice;
			if( FilterChoice != INDEX_NONE )
			{
				SelectedMapFilterName = JoinMapOption->MultiChoice[FilterChoice].ToString();
			}
		}

#if SHOOTER_CONSOLE_UI
		UGameViewportClient* const GVC = GEngine->GameViewport;
#if PLATFORM_PS4 || MAX_LOCAL_PLAYERS == 1
		// Show server menu (skip splitscreen)
		AddMenuToGameViewport();
		FSlateApplication::Get().SetKeyboardFocus(MenuWidget);

		MenuWidget->NextMenu = JoinServerItem->SubMenu;

		MenuWidget->EnterSubMenu();
#else
		// Show splitscreen menu
		RemoveMenuFromGameViewport();	
		GVC->AddViewportWidgetContent(SplitScreenLobbyWidgetContainer.ToSharedRef());

		SplitScreenLobbyWidget->Clear();
		FSlateApplication::Get().SetKeyboardFocus(SplitScreenLobbyWidget);

		SplitScreenLobbyWidget->SetIsJoining( true );
#endif
#else
		MenuWidget->NextMenu = JoinServerItem->SubMenu;
		//FString SelectedMapFilterName = JoinMapOption->MultiChoice[JoinMapOption->SelectedMultiChoice].ToString();

		MenuWidget->EnterSubMenu();
#endif
	}
	else if (GameInstance.IsValid())
	{
		//GameInstance->DisplayOnlinePrivilegeFailureDialogs(UserId, Privilege, PrivilegeResults);
	}
}

void FSoldierMainMenu::OnShowLeaderboard()
{
	MenuWidget->NextMenu = LeaderboardItem->SubMenu;

	//LeaderboardWidget->ReadStats();

	MenuWidget->EnterSubMenu();
}

void FSoldierMainMenu::OnShowOnlineStore()
{
	MenuWidget->NextMenu = OnlineStoreItem->SubMenu;
#if LOGIN_REQUIRED_FOR_ONLINE_PLAY
	UE_LOG(LogOnline, Warning, TEXT("You need to be logged in before using the store"));
#endif
	//OnlineStoreWidget->BeginGettingOffers();
	MenuWidget->EnterSubMenu();
}

void FSoldierMainMenu::OnShowDemoBrowser()
{
	MenuWidget->NextMenu = DemoBrowserItem->SubMenu;
	MenuWidget->EnterSubMenu();
}

void FSoldierMainMenu::OnShowSceneObjectsBrowser( )
{
	MenuWidget->NextMenu = SceneObjectsBrowserItem->SubMenu;
	SceneObjectsListWidget->BuildSceneObjectsList( );
	MenuWidget->EnterSubMenu( );
}
void FSoldierMainMenu::OnShowWeatherSettings()
{
	MenuWidget->NextMenu = WeatherSettingsItem->SubMenu;
	//SceneObjectsListWidget->BuildSceneObjectsList();
	MenuWidget->EnterSubMenu();
}
void FSoldierMainMenu::OnUIQuit()
{
	LockAndHideMenu();

	const FSimMenuSoundsStyle& MenuSounds = FArmySimStyle::Get().GetWidgetStyle<FSimMenuSoundsStyle>("DefaultSoldierMenuSoundsStyle");

	if (GetTickableGameObjectWorld() != NULL && GetPlayerOwnerControllerId() != -1)
	{
		FSlateApplication::Get().PlaySound(MenuSounds.ExitGameSound, GetPlayerOwnerControllerId());
		MenuHelper::PlaySoundAndCall(GetTickableGameObjectWorld(), MenuSounds.ExitGameSound, GetPlayerOwnerControllerId(), this, &FSoldierMainMenu::Quit);
	}
}

void FSoldierMainMenu::Quit()
{
	if (ensure(GameInstance.IsValid()))
	{
		UGameViewportClient* const Viewport = GameInstance->GetGameViewportClient();
		if (ensure(Viewport)) 
		{
			Viewport->ConsoleCommand("quit");
		}
	}
}
void FSoldierMainMenu::UnLockAndHideMenu( ) {
	MenuWidget->LockControls( false );
	MenuWidget->HideMenu( );
}

void FSoldierMainMenu::LockAndHideMenu()
{
	MenuWidget->LockControls(true);
	MenuWidget->HideMenu();
}

void FSoldierMainMenu::DisplayLoadingScreen()
{
	IArmyGameLoadingScreenModule* LoadingScreenModule = FModuleManager::LoadModulePtr<IArmyGameLoadingScreenModule>("ArmyGameLoadingScreen");
	if( LoadingScreenModule != NULL )
	{
		LoadingScreenModule->StartInGameLoadingScreen();
	}
}

bool FSoldierMainMenu::IsMapReady() const
{
	bool bReady = true;
	IPlatformChunkInstall* ChunkInstaller = FPlatformMisc::GetPlatformChunkInstall();
	if (ChunkInstaller)
	{
		EMap SelectedMap = GetSelectedMap();
		// should use the AssetRegistry as soon as maps are added to the AssetRegistry
		int32 MapChunk = ChunkMapping[(int)SelectedMap];
		EChunkLocation::Type ChunkLocation = ChunkInstaller->GetChunkLocation(MapChunk);
		if (ChunkLocation == EChunkLocation::NotAvailable)
		{			
			bReady = false;
		}
	}
	return bReady;
}

USoldierPersistentUser* FSoldierMainMenu::GetPersistentUser() const
{
	USoldierLocalPlayer* const SoldierLocalPlayer = Cast<USoldierLocalPlayer>(GetPlayerOwner());
	return SoldierLocalPlayer ? SoldierLocalPlayer->GetPersistentUser() : nullptr;
}

UWorld* FSoldierMainMenu::GetTickableGameObjectWorld() const
{
	ULocalPlayer* LocalPlayerOwner = GetPlayerOwner();
	return (LocalPlayerOwner ? LocalPlayerOwner->GetWorld() : nullptr);
}

ULocalPlayer* FSoldierMainMenu::GetPlayerOwner() const
{
	return PlayerOwner.Get();
}

int32 FSoldierMainMenu::GetPlayerOwnerControllerId() const
{
	return ( PlayerOwner.IsValid() ) ? PlayerOwner->GetControllerId() : -1;
}

FString FSoldierMainMenu::GetMapName() const
{
	return MapNames[(int)GetSelectedMap()];
}

void FSoldierMainMenu::OnCancelMatchmakingComplete(FName SessionName, bool bWasSuccessful)
{
	IOnlineSessionPtr Sessions = Online::GetSessionInterface(GetTickableGameObjectWorld());
	if (Sessions.IsValid())
	{
		Sessions->ClearOnCancelMatchmakingCompleteDelegate_Handle(OnCancelMatchmakingCompleteDelegateHandle);
	}

	bAnimateQuickmatchSearchingUI = false;
	UGameViewportClient* const GVC = GEngine->GameViewport;
	GVC->RemoveViewportWidgetContent(QuickMatchStoppingWidgetContainer.ToSharedRef());
	AddMenuToGameViewport();
	FSlateApplication::Get().SetKeyboardFocus(MenuWidget);
}

void FSoldierMainMenu::OnPlayTogetherEventReceived()
{
	HostOnlineMenuItem->Widget->SetMenuItemActive(true);
	MenuWidget->ConfirmMenuItem();
}


void FSoldierMainMenu::ToggleGameMainMenu( ) {

	if (!MenuWidget.IsValid( )) {
		return;
	}

	if (bIsGameMainMenuUp && MenuWidget->CurrentMenu != RootMenuItem->SubMenu)
	{
		MenuWidget->MenuGoBack();
		return;
	}

	ASoldierPlayerController* const PCOwner = PlayerOwner.Get() ? Cast<ASoldierPlayerController>( PlayerOwner->PlayerController ) : nullptr;
	if (!bIsGameMainMenuUp) {
		// Hide the scoreboard
		if (PCOwner) {
			ASoldierHUD* const SoldierHUD = PCOwner->GetSoldierHUD( );
			if (SoldierHUD != NULL) {
				SoldierHUD->ShowScoreboard( false );
			}
		}
		AddMenuToGameViewport( );
		
		MenuWidget->BuildAndShowMenu( );
		bIsGameMainMenuUp = true;

		if (PCOwner) {
			// Disable controls while paused
			PCOwner->SetCinematicMode( true, false, false, true, true );

			PCOwner->SetPause( false );
			FInputModeGameAndUI InputMode;
			PCOwner->SetInputMode( InputMode );
		}
	}
	else {
		MenuWidget->MenuGoBack();
		//Start hiding animation
		MenuWidget->HideMenu( );
		if (PCOwner) {
			// Make sure viewport has focus
			FSlateApplication::Get( ).SetAllUserFocusToGameViewport( );

			// Don't renable controls if the match is over
			ASoldierHUD* const SoldierHUD = PCOwner->GetSoldierHUD( );
			if (SoldierHUD != NULL) {
				PCOwner->SetCinematicMode( false, false, false, true, true );

				FInputModeGameOnly InputMode;
				PCOwner->SetInputMode( InputMode );
				bIsGameMainMenuUp = false;
			}
		}
	}
}

#undef LOCTEXT_NAMESPACE
