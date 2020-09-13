// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#include "OceanBoats.h"
#include "WeatherOptions.h"
#include "ArmySimTypes.h"
#include "ArmySimStyle.h"
#include "SoldierOptionsWidgetStyle.h"
#include "ArmySimGameUserSettings.h"
#include "Player/SoldierPersistentUser.h"
#include "Player/SoldierLocalPlayer.h"

#define LOCTEXT_NAMESPACE "OceanBoats.HUD.Menu"

void FWeatherOptions::Construct(ULocalPlayer* InPlayerOwner)
{
	OptionsStyle = &FArmySimStyle::Get().GetWidgetStyle<FSoldierOptionsStyle>("DefaultSoldierOptionsStyle");

	PlayerOwner = InPlayerOwner;
	MinSensitivity = 1;
	
	//TArray<FText> ResolutionList;
	TArray<FText> OnOffList;
	TArray<FText> SensitivityList;
	TArray<FText> LowHighList;

	OnOffList.Add(LOCTEXT("Off","OFF"));
	OnOffList.Add(LOCTEXT("On","ON"));

	LowHighList.Add(LOCTEXT("Low","LOW"));
	LowHighList.Add(LOCTEXT("High","HIGH"));

	//Mouse sensitivity 0-50
	for (int32 i = 0; i < 51; i++)
	{
		SensitivityList.Add(FText::AsNumber(i));
	}

	/** Options menu root item */
	TSharedPtr<FSoldierMenuItem> OptionsRoot = FSoldierMenuItem::CreateRoot();


	OptionsItem = MenuHelper::AddMenuItem(OptionsRoot,LOCTEXT("WeatherSettings", "WeatherSETTINGS"));

	DayOption = MenuHelper::AddMenuOptionSP(OptionsItem,LOCTEXT("Day", "DAY SPEED"), SensitivityList, this, &FWeatherOptions::DayOptionChanged);
	TimeOfDayOption = MenuHelper::AddMenuOptionSP(OptionsItem,LOCTEXT("TimeOfDay", "TIME OF DAY"),SensitivityList, this, &FWeatherOptions::TimeOfDayOptionChanged);
	GloomyOption = MenuHelper::AddMenuOptionSP(OptionsItem,LOCTEXT("Gloomy", "Gloomy"),OnOffList, this, &FWeatherOptions::GloomyOptionChanged);
	RainyOption = MenuHelper::AddMenuOptionSP(OptionsItem, LOCTEXT("Rainy", "Rainy"), OnOffList, this, &FWeatherOptions::GloomyOptionChanged);

	
	MenuHelper::AddMenuItemSP(OptionsItem,LOCTEXT("ApplyChanges", "APPLY CHANGES"), this, &FWeatherOptions::OnApplySettings);

	//Do not allow to set aim sensitivity to 0
	TimeOfDayOption->MinMultiChoiceIndex = MinSensitivity; 

	//UserSettings = CastChecked<UArmySimGameUserSettings>(GEngine->GetGameUserSettings());
	//ResolutionOpt = UserSettings->GetScreenResolution();
	//bFullScreenOpt = UserSettings->GetFullscreenMode();
	//GraphicsQualityOpt = UserSettings->GetGraphicsQuality();

	USoldierPersistentUser* PersistentUser = GetPersistentUser();
	if(PersistentUser)
	{
		//bInvertYAxisOpt = PersistentUser->GetInvertedYAxis();
		//SensitivityOpt = PersistentUser->GetAimSensitivity();
		//GammaOpt = PersistentUser->GetGamma();
		//bVibrationOpt = PersistentUser->GetVibration();
	}
	else
	{
		//bVibrationOpt = true;
		//bInvertYAxisOpt = false;
		//SensitivityOpt = 1.0f;
		//GammaOpt = 2.2f;
	}

}

void FWeatherOptions::OnApplySettings()
{
	FSlateApplication::Get().PlaySound(OptionsStyle->AcceptChangesSound, GetOwnerUserIndex());
	ApplySettings();
}

void FWeatherOptions::ApplySettings()
{
	//USoldierPersistentUser* PersistentUser = GetPersistentUser();
	//if(PersistentUser)
	//{
	//	PersistentUser->SetAimSensitivity(SensitivityOpt);
	//	PersistentUser->SetInvertedYAxis(bInvertYAxisOpt);
	//	PersistentUser->SetGamma(GammaOpt);
	//	PersistentUser->SetVibration(bVibrationOpt);
	//	PersistentUser->TellInputAboutKeybindings();

	//	PersistentUser->SaveIfDirty();
	//}

	//UserSettings->SetScreenResolution(ResolutionOpt);
	//UserSettings->SetFullscreenMode(bFullScreenOpt);
	//UserSettings->SetGraphicsQuality(GraphicsQualityOpt);
	//UserSettings->ApplySettings(false);

	//OnApplyChanges.ExecuteIfBound();
}

void FWeatherOptions::TellInputAboutKeybindings()
{
	USoldierPersistentUser* PersistentUser = GetPersistentUser();
	if(PersistentUser)
	{
		PersistentUser->TellInputAboutKeybindings();
	}
}

void FWeatherOptions::RevertChanges()
{
	FSlateApplication::Get().PlaySound(OptionsStyle->DiscardChangesSound, GetOwnerUserIndex());
	UpdateOptions();
	//GEngine->DisplayGamma =  2.2f + 2.0f * (-0.5f + GammaOption->SelectedMultiChoice / 100.0f);
}

int32 FWeatherOptions::GetCurrentMouseGloomyIndex()
{
	USoldierPersistentUser* PersistentUser = GetPersistentUser();
	if(PersistentUser)
	{
		return GloomyOption->SelectedMultiChoice = PersistentUser->GetInvertedYAxis() ? 1 : 0;
	}
	else
	{
		return 0;
	}
}

int32 FWeatherOptions::GetCurrentMouseRainyIndex()
{
	USoldierPersistentUser* PersistentUser = GetPersistentUser();
	if (PersistentUser)
	{
		return RainyOption->SelectedMultiChoice = PersistentUser->GetInvertedYAxis() ? 1 : 0;
	}
	else
	{
		return 0;
	}
}

int32 FWeatherOptions::GetCurrentMouseTimeOfDayIndex()
{
	USoldierPersistentUser* PersistentUser = GetPersistentUser();
	if(PersistentUser)
	{
		//mouse sensitivity is a floating point value ranged from 0.0f to 1.0f
		int32 timeofday = FMath::RoundToInt((PersistentUser->GetAimSensitivity() - 0.5f) * 10.0f);
		//Clamp to valid index range
		return FMath::Clamp(timeofday, MinSensitivity, 100);
	}

	return FMath::RoundToInt((1.0f - 0.5f) * 10.0f);
}

int32 FWeatherOptions::GetCurrentMouseDayIndex()
{
	USoldierPersistentUser* PersistentUser = GetPersistentUser();
	if (PersistentUser)
	{
		//mouse sensitivity is a floating point value ranged from 0.0f to 1.0f
		int32 day = FMath::RoundToInt((PersistentUser->GetAimSensitivity() - 0.5f) * 10.0f);
		//Clamp to valid index range
		return FMath::Clamp(day, MinSensitivity, 100);
	}

	return FMath::RoundToInt((1.0f - 0.5f) * 10.0f);
}


int32 FWeatherOptions::GetOwnerUserIndex() const
{
	return PlayerOwner ? PlayerOwner->GetControllerId() : 0;
}

USoldierPersistentUser* FWeatherOptions::GetPersistentUser() const
{
	USoldierLocalPlayer* const SLP = Cast<USoldierLocalPlayer>(PlayerOwner);
	if (SLP)
	{
		return SLP->GetPersistentUser();
	}

	return nullptr;
}

void FWeatherOptions::UpdateOptions()
{
	//grab the user settings
	USoldierPersistentUser* const PersistentUser = GetPersistentUser();
	if (PersistentUser)
	{
		//TimeOfDayOpt = PersistentUser->GetTimeOfDay();
		//DayOpt = PersistentUser->GetDay();
		//bGloomy = PersistentUser->GetGloomy();
		//bRainy = PersistentUser->GetRainy();	
	} 

	GloomyOption->SelectedMultiChoice =  GetCurrentMouseGloomyIndex();
	RainyOption->SelectedMultiChoice = GetCurrentMouseRainyIndex();
	TimeOfDayOption->SelectedMultiChoice = GetCurrentMouseTimeOfDayIndex();
	DayOption->SelectedMultiChoice = GetCurrentMouseDayIndex();

	//DayOptionChanged(DayOption, DayOption->SelectedMultiChoice);
}


void FWeatherOptions::TimeOfDayOptionChanged(TSharedPtr<FSoldierMenuItem> MenuItem, int32 MultiOptionIndex)
{
	TimeOfDayOpt = 0.5f + (MultiOptionIndex / 10.0f);
}

void FWeatherOptions::DayOptionChanged(TSharedPtr<FSoldierMenuItem> MenuItem, int32 MultiOptionIndex)
{
	DayOpt = 2.2f + 2.0f * (-0.5f + MultiOptionIndex / 100.0f);
	//GEngine->DisplayGamma = GammaOpt;
}



void FWeatherOptions::GloomyOptionChanged(TSharedPtr<FSoldierMenuItem> MenuItem, int32 MultiOptionIndex)
{
	bGloomy = MultiOptionIndex > 0  ? true : false;
}

void FWeatherOptions::RainyOptionChanged(TSharedPtr<FSoldierMenuItem> MenuItem, int32 MultiOptionIndex)
{
	bRainy = MultiOptionIndex > 0 ? true : false;
}

#undef LOCTEXT_NAMESPACE
