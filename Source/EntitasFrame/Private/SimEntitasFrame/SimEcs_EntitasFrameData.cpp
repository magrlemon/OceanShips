#include "SimEcs_EntitasFrameData.h"
#include "EntitasFramePCH.h"
#include "SimEcs_ContextDataEnvironment.h"
#include "SimDataStructure.h"
#include "SimApi.h"
#include "SimEcs_BoatArchetype.h"


ASimEcs_EntitasFrameData::ASimEcs_EntitasFrameData(/* const FObjectInitializer& ObjectInitializer*/ ) 
	//: Super( ObjectInitializer )
{
#if WITH_EDITOR
	if (GIsEditor == true) {
		return;
	}
#endif

	int32 index = 0;int32 location = 0;
	for (FString var : bp_type_names) {
		FString BP_ClassType = bp_type_names[index];
		if (bp_type_size[index] == 0) {
			index++;
			continue;
		}
		ConstructorHelpers::FClassFinder<ASimEcs_Archetype> PawnArche( *BP_ClassType );
		if ((PawnArche.Class != NULL) && PawnArche.Succeeded( )) {
			m_MapSubClass_BP.Add( bp_type_size[index], PawnArche.Class );
		}
		index++;

	}
}

void ASimEcs_EntitasFrameData::BeginPlay( ) {
#if WITH_EDITOR
	if (GIsEditor == true) {
		return;
	}
#endif
	m_SimEcsEnginePtr = MakeShareable( new SimEcs_Engine( ) );

}


void ASimEcs_EntitasFrameData::BeginDestroy( ) {
//#if WITH_EDITOR
//	if (GIsEditor == true) {
//		return;
//	}
//#endif
	m_MapSubClass_BP.Reset( );
	m_SimEcsEnginePtr = nullptr;
}


bool ASimEcs_EntitasFrameData::GenerateOBEntitys( int entType, TMapScenario & refScenarioXmlData ) {

	/*find entity type by key*/
	if (refScenarioXmlData.Num( ) <= 0 || entType <= 0)
		return false;

	UWorld* World = GetWorld( );
	if (!World) return false;

	EEntityEquips equipType = static_cast<EEntityEquips>(entType);
	//////////////////////////////////////////////////////////////////////////
	bool beInvalidEnvironment = false;
	switch (equipType) {
#define CASE_TYPE(ETYPE,OCOBJ)\
	case EEntityEquips::EEE_##ETYPE##_TYPE:\
		{\
			beInvalidEnvironment  =  true;\
			if (beInvalidEnvironment)\
			{\
				auto pContextDataIEnvironment  = NewObject<ASimContextData##OCOBJ##Environment>();\
				if(pContextDataIEnvironment){\
					pContextDataIEnvironment->SerializeStructure(refScenarioXmlData);\
					pContextDataIEnvironment->CreateEntity( GetEcsEnginePtr( ) );\
				}\
			}\
		}\
		break;\


		CASE_TYPE( DRONEBOAT, DroneBoat )
			CASE_TYPE( NAVALCRAFT, NavalCraft )
			

			CASE_TYPE( OBSTACLE_HLS, HLS )					//三角锥		 1001
			CASE_TYPE( OBSTACLE_SJZ, SJZ )					//海立石		 1002
			CASE_TYPE( OBSTACLE_GTZ, GTZ )					//轨条砦		 1003
			CASE_TYPE( OBSTACLE_GCW, GCW )					//钢刺猬		 1004
			CASE_TYPE( OBSTACLE_TSW_SF, TSWSF )				//蛇腹铁丝网	 1005
			CASE_TYPE( OBSTACLE_TSW_YL, TSWYL )				//一列铁丝网	 1006
			CASE_TYPE( OBSTACLE_ZGQ, ZGQ )					//组隔墙		 1007
			CASE_TYPE( OBSTACLE_HHLC, HHLC )				//混合雷场		 1008
			CASE_TYPE( OBSTACLE_BLDS, BLDS )				//玻璃刀山		 1009
			CASE_TYPE( OBSTACLE_GTZ_SUIJI, GTZSUIJI )		//水迹轨条砦	 1010
			CASE_TYPE( OBSTACLE_ZJH, ZJH )					//阻绝壕		 1011

#undef  CASE_TYPE
	default:
		break;
	}

	return true;
}


void ASimEcs_EntitasFrameData::GenScenarioObjects( )
{
	using TArrScenario = TArray<TMap<FString, FString>>;
	IScenarioXmlParserModule* pSXmlParserModule = FModuleManager::LoadModulePtr<IScenarioXmlParserModule>( "SimScenarioXmlParser" );
	if (pSXmlParserModule != NULL) {
		pSXmlParserModule->StartScenarioXmlParser( );
		if (GetEcsEnginePtr( ).Pin( )) {
			TArrScenario arryScenarioData;
			pSXmlParserModule->GetScenarioXmlParserInfo( arryScenarioData );
			for (size_t entIndex = 0; entIndex < arryScenarioData.Num( ); entIndex++) {
				 int32 entType =  MakeFStringToInt( arryScenarioData[entIndex].Find( TEXT("类型") ) );
				if (entType <= 0) return;
				
				GenerateOBEntitys( entType, arryScenarioData[entIndex] );
			}
		}
	}
}


void ASimEcs_EntitasFrameData::GenCoastDefBarrierObjects( )
{
	using TArrScenario = TArray<TMap<FString, FString>>;
	ICoastDefXmlParserModule* pSXmlParserModule = FModuleManager::LoadModulePtr<ICoastDefXmlParserModule>( "SimCoastDefXmlParser" );
	if (pSXmlParserModule != NULL) {
		pSXmlParserModule->StartCoastDefXmlParser( );
		if (GetEcsEnginePtr( ).Pin( )) {
			TArrScenario arryScenarioData;
			pSXmlParserModule->GetCoastDefXmlParserInfo( arryScenarioData );
			//TArrScenario& arryScenarioData = ArmyScenarioLoader::GetInstance( ).GetScenarioData( );
			for (size_t entIndex = 0; entIndex < arryScenarioData.Num( ); entIndex++) {
				/*for (TMap<FString, FString>::TIterator it = arryScenarioData[entIndex].CreateIterator( ); it; ++it) {
					GEngine->AddOnScreenDebugMessage( -1, 30.f, FColor::White, it->Key + FString( L"  ===>   " ) );
				}*/

				const int32 entType = MakeFStringToInt( arryScenarioData[entIndex].Find( TEXT( "类型" ) ) );
				if (entType <= 0)
					return;

				GenerateOBEntitys( entType, arryScenarioData[entIndex] );
			}
		}
	}
}
//
TSubclassOf<ASimEcs_Archetype> ASimEcs_EntitasFrameData::GetSubClassByType( int type ) {
	if (type <= 0)
		return nullptr;
	return m_MapSubClass_BP[type];
}

/* Update the character. (Running, health etc).*/
void ASimEcs_EntitasFrameData::Tick( float DeltaSeconds ) {
#if WITH_EDITOR
	if (GIsEditor == true) {
		return;
	}
#endif
	//if (GetEcsEnginePtr().Pin()) {
	//	GetEcsEnginePtr( ).Pin( )->Update( DeltaSeconds );
	//}
}


int32 ASimEcs_EntitasFrameData::MakeFStringToInt( const FString* strType ) {
	if (strType == nullptr || strType->IsEmpty( ) && !strType->IsNumeric( ))
		return -1;
	return FCString::Atoi( **strType );
}
