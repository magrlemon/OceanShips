// Fill out your copyright notice in the Description page of Project Settings.


#include "ArmyCoastDefLoader.h"

ArmyCoastDefLoader * ArmyCoastDefLoader::m_Instance = nullptr;

ArmyCoastDefLoader::ArmyCoastDefLoader()
{
}

ArmyCoastDefLoader::~ArmyCoastDefLoader()
{
}

void ArmyCoastDefLoader::Init(FString strFileName) 
{
	ParserXml(strFileName);
}

bool ArmyCoastDefLoader::ParserXml(FString strFileName)
{
	bool bIsSucc=m_XmlFile.LoadFile(strFileName);
	if (!bIsSucc) 
	{
		return false;
	}

	m_RootNode=m_XmlFile.GetRootNode();
	if (!m_RootNode) 
	{
		return false;
	}
	FString strTagName(TEXT("名称"));
	auto nameNode = GetChildNode(m_RootNode,strTagName);

	if (!nameNode) 
	{
		return false;
	}

	auto nodeList = m_RootNode->GetChildrenNodes();
	for(auto node : nodeList)
	{
		if (node->GetTag().Equals(strTagName)) {
			continue;
		}

		ParserEntity(node);
	}
	
	return true;
}


void  ArmyCoastDefLoader::ParserEntity(FXmlNode * parentNode)
{
	auto nodeList = parentNode->GetChildrenNodes();
	for (auto node : nodeList)
	{
		TMap<FString, FString> mapEntityInfo;
		auto chList = node->GetChildrenNodes();
		for (auto chNode : chList)
		{
			auto attList = chNode->GetAttributes();
			for (auto attr : attList)
			{
				mapEntityInfo.Add(attr.GetTag(), attr.GetValue());
			}
		}
		
		if (mapEntityInfo.Num()>0) 
		{
			m_EntityInfos.Add(mapEntityInfo);
		}
	}
}

FXmlNode * ArmyCoastDefLoader::GetChildNode(FXmlNode * parentNode,  FString &ChildTag)
{
	if (!parentNode) 
	{
		return nullptr;
	}

	auto XmlNodeList = parentNode->GetChildrenNodes();

	for (auto node : XmlNodeList) 
	{
		
		if (node->GetTag().Equals(ChildTag))
		{
			
			return node;
		}
	
	}

	return nullptr;

}

