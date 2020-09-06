// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "OceanBoats.h"
#include "Engine/GameInstance.h"
#include "ArmySimGameInstance.generated.h"

class FDcxVehicleManager;
class FVariantData;
class FSoldierMainMenu;
class FSoldierWelcomeMenu;
class FSoldierMessageMenu;
class ASoldierGameSession;
class FDcxVehicleManager;
class IVehicle;

namespace ArmySimGameInstanceState
{
	extern const FName None;
	extern const FName PendingInvite;
	extern const FName WelcomeScreen;
	extern const FName MainMenu;
	extern const FName MessageMenu;
	extern const FName Playing;
}

/** This class holds the value of what message to display when we are in the "MessageMenu" state */
class FArmySimPendingMessage {
public:
	FText	DisplayString;				// This is the display message in the main message body
	FText	OKButtonString;				// This is the ok button text
	FText	CancelButtonString;			// If this is not empty, it will be the cancel button text
	FName	NextState;					// Final destination state once message is discarded

	TWeakObjectPtr< ULocalPlayer > PlayerOwner;		// Owner of dialog who will have focus (can be NULL)
};

class FArmySimPendingInvite {
public:
	FArmySimPendingInvite( ) : ControllerId( -1 ), UserId( nullptr ), bPrivilegesCheckedAndAllowed( false ) {}

	int32							 ControllerId;
	TSharedPtr< const FUniqueNetId > UserId;
	FOnlineSessionSearchResult 		 InviteResult;
	bool							 bPrivilegesCheckedAndAllowed;
};

struct FSoldierPlayTogetherInfo {
	FSoldierPlayTogetherInfo( ) : UserIndex( -1 ) {}
	FSoldierPlayTogetherInfo( int32 InUserIndex, const TArray<TSharedPtr<const FUniqueNetId>>& InUserIdList )
		: UserIndex( InUserIndex )
	{
		UserIdList.Append( InUserIdList );
	}

	int32 UserIndex;
	TArray<TSharedPtr<const FUniqueNetId>> UserIdList;
};

class SArmySimWaitDialog : public SCompoundWidget
{
public:
	SLATE_BEGIN_ARGS( SArmySimWaitDialog )
	{}
	SLATE_ARGUMENT(FText, MessageText)
		SLATE_END_ARGS()

		void Construct(const FArguments& InArgs);

private:

	/** our curve sequence and the related handles */
	FCurveSequence WidgetAnimation;

	/** used for animating the text color. */
	FCurveHandle TextColorCurve;

	/** Gets the animated text color */
	FSlateColor GetTextColor() const;
};


UENUM( )
enum class EOnlineMode : uint8 {
	Offline,
	LAN,
	Online
};

/**
 * 
 */
UCLASS( config = Game )
class  UArmySimGameInstance : public UGameInstance
{
	GENERATED_UCLASS_BODY()
public:
	  
	bool Tick(float DeltaSeconds);

	virtual void Init() override;
	virtual void Shutdown() override;
	virtual void StartGameInstance() override;


	/** Sends the game to the specified state. */
	void GotoState(FName NewState);

	/** Obtains the initial welcome state, which can be different based on platform */
	FName GetInitialState();

	/** Sends the game to the initial startup/frontend state  */
	void GotoInitialState();

	/** Gets the current state of the GameInstance */
	const FName GetCurrentState() const;


	void ShowLoadingScreen( );

	void AddNetworkFailureHandlers( );
	void RemoveNetworkFailureHandlers( );
	/**
	* Creates the message menu, clears other menus and sets the KingState to Message.
	*
	* @param	Message				Main message body
	* @param	OKButtonString		String to use for 'OK' button
	* @param	CancelButtonString	String to use for 'Cancel' button
	* @param	NewState			Final state to go to when message is discarded
	*/
	void ShowMessageThenGotoState(const FText& Message, const FText& OKButtonString, const FText& CancelButtonString, const FName& NewState, const bool OverrideExisting = true, TWeakObjectPtr< ULocalPlayer > PlayerOwner = nullptr);

	void RemoveExistingLocalPlayer(ULocalPlayer* ExistingPlayer);

	void RemoveSplitScreenPlayers();


	// Generic confirmation handling (just hide the dialog)
	FReply OnConfirmGeneric();
	bool HasLicense() const { return bIsLicensed; }


	/** Returns true if the game is in online mode */
	EOnlineMode GetOnlineMode( ) const { return OnlineMode; }

	/** Sets the online mode of the game */
	void SetOnlineMode( EOnlineMode InOnlineMode );


	/** Show approved dialogs for various privileges failures */
	void DisplayOnlinePrivilegeFailureDialogs( const FUniqueNetId& UserId, EUserPrivileges::Type Privilege, uint32 PrivilegeResults );

	bool HostGame( ULocalPlayer* LocalPlayer, const FString& GameType, const FString& InTravelURL );


	bool LoadFrontEndMap( const FString& MapName );

	/** Updates the status of using multiplayer features */
	void UpdateUsingMultiplayerFeatures( bool bIsUsingMultiplayerFeatures );

	/** Returns true if the passed in local player is signed in*/
	bool IsLocalPlayerSignedIn( ULocalPlayer* LocalPlayer );

	/** Returns true if owning player is online. Displays proper messaging if the user can't play */
	bool ValidatePlayerForOnlinePlay( ULocalPlayer* LocalPlayer );

	bool IsLocalPlayerOnline( ULocalPlayer* LocalPlayer );

	/** Returns true if owning player is signed in. Displays proper messaging if the user can't play */
	bool ValidatePlayerIsSignedIn( ULocalPlayer* LocalPlayer );


	/** Flag the local player when they quit the game */
	void LabelPlayerAsQuitter( ULocalPlayer* LocalPlayer ) const;

	void OnPreLoadMap( const FString& MapName );
	void OnPostLoadMap( UWorld* );
	void OnPostDemoPlay( );


	/** Sets the controller to ignore for pairing changes. Useful when we are showing external UI for manual profile switching. */
	void SetIgnorePairingChangeForControllerId( const int32 ControllerId );

	/** Sets a rich presence string for all local players. */
	void SetPresenceForLocalPlayers( const FString& StatusStr, const FVariantData& PresenceData );

	void ToggleMainMenu( );

	void ToggleSceneObjectsList( );
	/** Show messaging and punt to welcome screen */
	void HandleSignInChangeMessaging( );

	// Callback to pause the game when the OS has constrained our app.
	void HandleAppWillDeactivate( );

	// Callback occurs when game being suspended
	void HandleAppSuspend( );

	// Callback occurs when game resuming
	void HandleAppResume( );

	// Callback to process game licensing change notifications.
	void HandleAppLicenseUpdate( );

	// Callback to handle safe frame size changes.
	void HandleSafeFrameChanged( );

	TSharedPtr< const FUniqueNetId > GetUniqueNetIdFromControllerId( const int ControllerId );

public:

	UFUNCTION( BlueprintCallable )
		AActor* GetActiveVehicle( ) { return ActiveVehicle; }
	UFUNCTION( BlueprintCallable )
		void SetActiveVehicle( AActor* vehicle );
	UFUNCTION( BlueprintCallable )
		void VehicleDetect( bool enter, AActor* vehicle, AActor* enterActor );
	UFUNCTION( BlueprintCallable )
		bool IsLoginVehicle( ) { return bLoginVehicle; }
	UFUNCTION( BlueprintCallable )
		bool IsEnterVehicle( ) { return bEnterVehicle; }
	UFUNCTION( BlueprintCallable )
		void SetLoginVehicle( bool login ) { bLoginVehicle = login; }
private:
	void MaybeChangeState();
	void EndCurrentState(FName NextState);
	void BeginNewState(FName NewState, FName PrevState);

	void BeginPendingInviteState();
	void BeginWelcomeScreenState();
	void BeginMainMenuState();
	void BeginMessageMenuState();
	void BeginPlayingState();


	void EndPendingInviteState();
	void EndWelcomeScreenState();
	void EndMainMenuState();
	void EndMessageMenuState();
	void EndPlayingState();

private:

	UPROPERTY(config)
	FString WelcomeScreenMap;

	UPROPERTY(config)
	FString MainMenuMap;


	/** Controller to ignore for pairing changes. -1 to skip ignore. */
	int32 IgnorePairingChangeForControllerId;

	FName CurrentState;
	FName PendingState;

	/** If true, enable splitscreen when map starts loading */
	bool bPendingEnableSplitscreen;

	/** Local player login status when the system is suspended */
	TArray<ELoginStatus::Type> LocalPlayerOnlineStatus;

	/** Delegate for callbacks to Tick */
	FTickerDelegate TickDelegate;
	/** Handle to various registered delegates */
	FDelegateHandle TickDelegateHandle;
	FDelegateHandle TravelLocalSessionFailureDelegateHandle;
	FDelegateHandle OnJoinSessionCompleteDelegateHandle;
	FDelegateHandle OnSearchSessionsCompleteDelegateHandle;
	FDelegateHandle OnStartSessionCompleteDelegateHandle;
	FDelegateHandle OnEndSessionCompleteDelegateHandle;
	FDelegateHandle OnDestroySessionCompleteDelegateHandle;
	FDelegateHandle OnCreatePresenceSessionCompleteDelegateHandle;


	/** Play Together on PS4 system event info */
	FSoldierPlayTogetherInfo PlayTogetherInfo;

	/** URL to travel to after pending network operations */
	FString TravelURL;

	/** Current online mode of the game (offline, LAN, or online) */
	EOnlineMode OnlineMode;

	/** Whether the user has an active license to play the game */
	bool bIsLicensed;

private:
	AActor* ActiveVehicle;
	bool bLoginVehicle;
	bool bEnterVehicle;



	FArmySimPendingMessage PendingMessage;

	FArmySimPendingInvite PendingInvite;

	/** Main menu UI */
	TSharedPtr<FSoldierMainMenu> MainMenuUI;

	/** Message menu (Shown in the even of errors - unable to connect etc) */
	TSharedPtr<FSoldierMessageMenu> MessageMenuUI;

	/** Welcome menu UI (for consoles) */
	TSharedPtr<FSoldierWelcomeMenu> WelcomeMenuUI;

	/** Dialog widget to show non-interactive waiting messages for network timeouts and such. */
	TSharedPtr<SArmySimWaitDialog> WaitMessageWidget;



};
