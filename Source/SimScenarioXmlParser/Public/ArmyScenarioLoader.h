// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "XmlParser.h"

/**
 * 
 */
class  SIMSCENARIOXMLPARSER_API ArmyScenarioLoader
{
private:
	ArmyScenarioLoader();
	ArmyScenarioLoader(const ArmyScenarioLoader& other) {};
	ArmyScenarioLoader & operator=(const ArmyScenarioLoader& other) { return *this; };
public:
	~ArmyScenarioLoader();
public:
	static ArmyScenarioLoader & GetInstance() { if (!m_Instance) m_Instance = new ArmyScenarioLoader(); return *m_Instance; }
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
	static ArmyScenarioLoader * m_Instance;
};
