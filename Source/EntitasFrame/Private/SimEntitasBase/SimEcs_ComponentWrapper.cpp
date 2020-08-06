// Fill out your copyright notice in the Description page of Project Settings.

#include "SimEcs_ComponentWrapper.h"

 ComponentTypeId IComponentWrapper::STATIC_COMPONENT_TYPE_ID = 0;

IComponentWrapper::IComponentWrapper( ) {

}


void WrapperDataInitializer::GrapData( FString& strValue ) {
	jsonValue = strValue;
}
