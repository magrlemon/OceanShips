// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.
#pragma once
#include "OceanBoats.h"
#include "SlateBasics.h"
#include "SlateExtras.h"
#include "Widgets/SoldierMenuItem.h"
#include "Widgets/SSoldierMenuWidget.h"

///** supported resolutions */
//const FIntPoint DefaultSoldierResolutions[] = { FIntPoint(1024,768), FIntPoint(1280,720), FIntPoint(1920,1080) };
//
///** supported resolutions count*/
//const int32 DefaultSoldierResCount = ARRAY_COUNT(DefaultSoldierResolutions);

/** delegate called when changes are applied */
DECLARE_DELEGATE(FOnApplyChanges);

class UArmySimGameUserSettings;

class FWeatherOptions : public TSharedFromThis<FWeatherOptions>
{
public:
	/** sets owning player controller */
	void Construct(ULocalPlayer* InPlayerOwner);

	/** get current options values for display */
	void UpdateOptions();

	/** UI callback for applying settings, plays sound */
	void OnApplySettings();

	/** applies changes in game settings */
	void ApplySettings();

	/** needed because we can recreate the subsystem that stores it */
	void TellInputAboutKeybindings();

	/** reverts non-saved changes in game settings */
	void RevertChanges();

	/** holds options menu item */
	TSharedPtr<FSoldierMenuItem> OptionsItem;

	///** holds cheats menu item */
	//TSharedPtr<FSoldierMenuItem> CheatsItem;

	/** called when changes were applied - can be used to close submenu */
	FOnApplyChanges OnApplyChanges;

protected:
	/** User settings pointer */
	UArmySimGameUserSettings* UserSettings;

	/**  */
	void TimeOfDayOptionChanged(TSharedPtr<FSoldierMenuItem> MenuItem, int32 MultiOptionIndex);
	void GloomyOptionChanged(TSharedPtr<FSoldierMenuItem> MenuItem, int32 MultiOptionIndex);
	void RainyOptionChanged(TSharedPtr<FSoldierMenuItem> MenuItem, int32 MultiOptionIndex);
	void DayOptionChanged(TSharedPtr<FSoldierMenuItem> MenuItem, int32 MultiOptionIndex);

	/** Get current mouse option index*/
	int32 GetCurrentMouseGloomyIndex();
	int32 GetCurrentMouseRainyIndex();
	int32 GetCurrentMouseTimeOfDayIndex();
	int32 GetCurrentMouseDayIndex();

	/** get current user index out of PlayerOwner */
	int32 GetOwnerUserIndex() const;

	/** Get the persistence user associated with PlayerOwner*/
	USoldierPersistentUser* GetPersistentUser() const;

	/** Owning player controller */
	ULocalPlayer* PlayerOwner;

	/** holds aim sensitivity option menu item */
	TSharedPtr<FSoldierMenuItem> TimeOfDayOption;

	/** holds gamma correction option menu item */
	TSharedPtr<FSoldierMenuItem> DayOption;

	/** holds full screen option menu item */
	TSharedPtr<FSoldierMenuItem> GloomyOption;

	TSharedPtr<FSoldierMenuItem> RainyOption;

	/** minimum sensitivity index */
	int32 MinSensitivity;

	/** current sensitivity set in options */
	float TimeOfDayOpt;

	/** current gamma correction set in options */
	float DayOpt;

	/** invert mouse setting set in options */
	uint8 bGloomy : 1;

	uint8 bRainy : 1;

	/** style used for the shooter options */
	const struct FSoldierOptionsStyle *OptionsStyle;
};