/** Copyright 2019 北京伯睿科技有限公司.
*	Author :	magi
	Date   :	11th July, 2019
	File   :	SimEcs_Archetype.h
	Summary:	Declares the  assest resource.
	All Rights Reserved.
	*http://www.borui.com	(c) Copyright 2019.
*/
// Fill out your copyright notice in the Description page of Project Settings.


#pragma once
#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GameFramework/Info.h"
#include "SimEcs_Core.h"
#include "SimEcs_Archetype.generated.h"




class SimEcs_World;

//base class for pure entity archetypes. This class cant be used directly, but can be spawned from an SimEcs spawner type system
UCLASS(Blueprintable)
class ENTITASFRAME_API ASimEcs_Archetype : public AInfo
{
	GENERATED_BODY()
	
public:	
	
	ASimEcs_Archetype();
	
	virtual void CreateNewEntityFromThis( EntityID  handleID );
	
};
