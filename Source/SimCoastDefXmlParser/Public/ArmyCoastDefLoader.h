// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "XmlParser.h"

/**
 * 
 */ 

class  SIMCOASTDEFXMLPARSER_API ArmyCoastDefLoader
{
private:
	ArmyCoastDefLoader();
	ArmyCoastDefLoader(const ArmyCoastDefLoader& other) {};
	ArmyCoastDefLoader & operator=(const ArmyCoastDefLoader& other) { return *this; };
public:
	~ArmyCoastDefLoader();
public:
	static ArmyCoastDefLoader & GetInstance() { if (!m_Instance) m_Instance = new ArmyCoastDefLoader(); return *m_Instance; }
public:
	void Init(FString strFileName);
	bool ParserXml(FString strFileName);
	FXmlNode * GetChildNode(FXmlNode * parentNode,FString &ChildTag);
	void	   ParserEntity(FXmlNode * parentNode);
	const TArray<TMap<FString, FString>> & GetScenarioData() { return m_EntityInfos; }
private:
	FXmlFile  m_XmlFile;
	FXmlNode* m_RootNode=nullptr;
	TArray<TMap<FString,FString>>	  m_EntityInfos;
private:
	static ArmyCoastDefLoader * m_Instance;
};
