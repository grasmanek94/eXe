/*
	Copyright (c) 2014 - 2015 Rafał "Gamer_Z" Grasman
		* See LICENSE.md in root directory for license
		* Written by Rafał Grasman <grasmanek94@gmail.com>
		* Development Start Month-Year: October-2014

	________________________________________________________________
	Purpose of this file
		-	Contains all the prototypes, enumerators and structures for the main achievements

	________________________________________________________________	
	Notes

	________________________________________________________________
	Dependencies

	________________________________________________________________
	Project Contributors
		Tom 'IceCube' Hewlett
		Mariusz 'C2A1' K.
		Mateusz 'eider' Cichoń
*/
#pragma once
#include "GameMode.hxx"

/*
	This enumerator contains all possible stats/achievements and whatever
	However we cannot display that much information in one dialog so we have to
	split the information into a few categories:
		- Achievement or stats?
		- How many entreis to display on page?
*/
enum EAchievementsMain
{
	EAM_GivenTotalBounty,//DONE
	EAM_HadTotalBounty,//DONE
	EAM_CollectedTotalBounty,//DONE
	EAM_TotalBountyKills,//DONE
	EAM_LubieDachowac,//DONE
	EAM_SpammerPW,//DONE
	EAM_SpammerChat,//DONE
	EAM_WygraneLotto,
	EAM_SumaWygranaLotto,
	EAM_QuizyDobrze,
	EAM_QuizyZle,
	EAM_Teleporter,//DONE
	EAM_Ramper,//DONE
	EAM_CarSpawner,//DONE
	EAM_ZABAWA,//DONE
	EAM_ZabawyMinigunKills,//DONE
	EAM_ZabawyMinigunDeaths,//DONE
	EAM_ZabawySiankoWygrane,//DONE
	EAM_ZabawySiankoPrzegrane,//DONE
	EAM_ZabawyOneDeKills,//DONE
	EAM_ZabawyOneDeDeaths,//DONE
	EAM_ZabawySniperKills,//DONE
	EAM_ZabawySniperDeaths,//DONE
	EAM_ZabawySawnOffKills,//DONE
	EAM_ZabawySawnOffDeaths,//DONE
	EAM_SpammerMafiaChat,//DONE
	EAM_SpammerVipChat,//DONE
	EAM_SpammerSponsorChat,//DONE
	EAM_SpammerModeratorChat,//DONE
	EAM_SpammerAdminChat,//DONE
	EAM_Reporter,//DONE
	EAM_HELPHELPHELP,//DONE
	EAM_PrivCarSpawned,//DONE
	EAM_MafiaCarSpawned,//DONE
	EAM_NononononoDead,//DONE
	EAM_NononononoArmour,//DONE
	EAM_Czitek,//DONE
	EAM_AnimacjaForLife,//DONE
	EAM_MafiaMnieKochaDollar,//DONE
	EAM_MafiaMnieKochaResp,//DONE
	EAM_LongestKillStreak,//DONE
	EAM_CurrentKillStreak,//DONE
	EAM_HeadShots,//DONE
	EAM_SepiknaExp,//enalast
	EAM_ZabawyGranatyKills,//DONE
	EAM_ZabawyGranatyDeaths,//DONE
	EAM_ZabawyRPGKills,//DONE
	EAM_ZabawyRPGDeaths,//DONE
	EAM_ZabawyOneShotKills,//DONE
	EAM_ZabawyOneShotDeaths,//DONE
	EAM_ZabawyPaintBallKills,//DONE
	EAM_ZabawyPaintBallDeaths,//DONE

	EAM_ZabawyCTFKills,
	EAM_ZabawyCTFDeaths,
	EAM_ZabawyCTFFlagsPickedUp,
	EAM_ZabawyCTFFlagsDelivered,

	EAM_ZabawyStatkiKills,
	EAM_ZabawyStatkiDeaths,

	EAM_ZabawyWWIIIKills,
	EAM_ZabawyWWIIIDeaths,

	EAM_ZabawyHideAndSeekFound,
	EAM_ZabawyHidAndSeekLongestTimeHidden,

	EAM_ZabawyDerbyWins,
	EAM_ZabawyDerbyLosses,

	EAM_ZabawyLabiryntFastestTime,

	EAM_ZabawyTDMKills,
	EAM_ZabawyTDMDeaths,
	EAM_ZabawyTDMWins,
	EAM_ZabawyTDMLosses,

	EAM_ZabawyParkourBMXFastestTime,

	EAM_ZabawyParkourNRGFastestTime,

	EAM_ZabawyParkourFastestTime,

	EAM_ZabawySparinfMafiiKills,
	EAM_ZabawySparingMafiiDeaths,
	EAM_ZabawySparingMaffiWins,
	EAM_ZabawySparingMafiiLosses,

	EAM_ZabawyMMAWins,
	EAM_ZabawyMMALosses,

	EAM_ZabawyWGKills,
	EAM_ZabawyWGDeaths,
	EAM_ZabawyWGWins,
	EAM_ZabawyWGLosses,

	EAM_ZabawyKartFastestTime,

	EAM_ZabawyBAGKills,
	EAM_ZabawyBAGDeaths,
	EAM_ZabawyBAGWins,
	EAM_ZabawyBAGLosses
};

/*
	Here we define the first page of the statistics category (/osg -> additional statistics (1))
*/
static const std::set<int> CategoryStats =
{
	EAM_GivenTotalBounty,//DONE
	EAM_HadTotalBounty,//DONE
	EAM_CollectedTotalBounty,//DONE
	EAM_TotalBountyKills,//DONE
	EAM_WygraneLotto,
	EAM_SumaWygranaLotto,
	EAM_QuizyDobrze,
	EAM_QuizyZle,
	EAM_ZabawyMinigunKills,//DONE
	EAM_ZabawyMinigunDeaths,//DONE
	EAM_ZabawySiankoWygrane,//DONE
	EAM_ZabawySiankoPrzegrane,//DONE
	EAM_ZabawyOneDeKills,//DONE
	EAM_ZabawyOneDeDeaths,//DONE
	EAM_ZabawySniperKills,//DONE
	EAM_ZabawySniperDeaths,//DONE
	EAM_ZabawySawnOffKills,//DONE
	EAM_ZabawySawnOffDeaths,//DONE
	EAM_CurrentKillStreak,//DONE
	EAM_ZabawyGranatyKills,//DONE
	EAM_ZabawyGranatyDeaths,//DONE
	EAM_ZabawyRPGKills,//DONE
	EAM_ZabawyRPGDeaths,//DONE
	EAM_ZabawyOneShotKills,//DONE
	EAM_ZabawyOneShotDeaths,//DONE
	EAM_ZabawyPaintBallKills,//DONE
	EAM_ZabawyPaintBallDeaths,//DONE
	EAM_ZabawyCTFKills,
	EAM_ZabawyCTFDeaths,
	EAM_ZabawyCTFFlagsPickedUp,
	EAM_ZabawyCTFFlagsDelivered
};


/*
	Here we define the second page of the statistics category (/osg -> additional statistics (2))
*/
static const std::set<int> CategoryStats2 =
{
	EAM_ZabawyStatkiKills,
	EAM_ZabawyStatkiDeaths,

	EAM_ZabawyWWIIIKills,
	EAM_ZabawyWWIIIDeaths,

	EAM_ZabawyHideAndSeekFound,
	EAM_ZabawyHidAndSeekLongestTimeHidden,

	EAM_ZabawyDerbyWins,
	EAM_ZabawyDerbyLosses,

	EAM_ZabawyLabiryntFastestTime,

	EAM_ZabawyTDMKills,
	EAM_ZabawyTDMDeaths,
	EAM_ZabawyTDMWins,
	EAM_ZabawyTDMLosses,

	EAM_ZabawyParkourBMXFastestTime,

	EAM_ZabawyParkourNRGFastestTime,

	EAM_ZabawyParkourFastestTime,

	EAM_ZabawySparinfMafiiKills,
	EAM_ZabawySparingMafiiDeaths,
	EAM_ZabawySparingMaffiWins,
	EAM_ZabawySparingMafiiLosses,

	EAM_ZabawyMMAWins,
	EAM_ZabawyMMALosses,

	EAM_ZabawyWGKills,
	EAM_ZabawyWGDeaths,
	EAM_ZabawyWGWins,
	EAM_ZabawyWGLosses,

	EAM_ZabawyKartFastestTime,

	EAM_ZabawyBAGKills,
	EAM_ZabawyBAGDeaths,
	EAM_ZabawyBAGWins,
	EAM_ZabawyBAGLosses
};

/*
	Here we define the first page of the achievements category
*/
static const std::set<int> CategoryAchievements =
{
	EAM_LubieDachowac,//DONE
	EAM_SpammerPW,//DONE
	EAM_SpammerChat,//DONE
	EAM_Teleporter,//DONE
	EAM_Ramper,//DONE
	EAM_CarSpawner,//DONE
	EAM_ZABAWA,//DONE
	EAM_SpammerMafiaChat,//DONE
	EAM_SpammerVipChat,//DONE
	EAM_SpammerSponsorChat,//DONE
	EAM_SpammerModeratorChat,//DONE
	EAM_SpammerAdminChat,//DONE
	EAM_Reporter,//DONE
	EAM_HELPHELPHELP,//DONE
	EAM_PrivCarSpawned,//DONE
	EAM_MafiaCarSpawned,//DONE
	EAM_NononononoDead,//DONE
	EAM_NononononoArmour,//DONE
	EAM_Czitek,//DONE
	EAM_AnimacjaForLife,//DONE
	EAM_MafiaMnieKochaDollar,//DONE
	EAM_MafiaMnieKochaResp,//DONE
	EAM_LongestKillStreak,//DONE
	EAM_HeadShots,//DONE
	EAM_SepiknaExp//DONE
};

typedef  std::pair<unsigned long long, language_string_ids> AchievementInfo;

/*
	Here we store information about the main enumerators' achievements and statistics,
	This information is in the format (required_amount_to_achieve_this, which achievement)
	If 0 it is always displayed, if there is a number then the entry will have to be achieved
	'n' amount of times to be fully added to the account
*/
static const std::vector<AchievementInfo> SingleFormatAchievements =
{
	AchievementInfo(0,		L_stats_totalgivenbounty	), 
	AchievementInfo(0,		L_stats_mytotalbounty		), 
	AchievementInfo(0,		L_stats_collectedbounty		), 
	AchievementInfo(0,		L_stats_bountykillers		), 
	AchievementInfo(100,	L_stats_vehicleflip			), 
	AchievementInfo(100,	L_stats_pmspammer			), 
	AchievementInfo(500,	L_stats_chatspammer			), 
	AchievementInfo(0,		L_stats_lottowins			), 
	AchievementInfo(0,		L_stats_totallottowon		), 
	AchievementInfo(0,		L_stats_quizgood			), 
	AchievementInfo(0,		L_stats_quizbad				), 
	AchievementInfo(1000,	L_stats_amountteleported	), 
	AchievementInfo(100,	L_stats_rampscount			), 
	AchievementInfo(100,	L_stats_vehiclespawncount	), 
	AchievementInfo(100,	L_stats_gamejoiner			), 
	AchievementInfo(0,		L_stats_mng_kills			), 
	AchievementInfo(0,		L_stats_mng_deaths			), 
	AchievementInfo(0,		L_stats_hay_wins			), 
	AchievementInfo(0,		L_stats_hay_loses			), 
	AchievementInfo(0,		L_stats_ode_kills			), 
	AchievementInfo(0,		L_stats_ode_deaths			), 
	AchievementInfo(0,		L_stats_snp_kills			), 
	AchievementInfo(0,		L_stats_snp_deaths			), 
	AchievementInfo(0,		L_stats_sof_kills			), 
	AchievementInfo(0,		L_stats_sof_deaths			), 
	AchievementInfo(100,	L_stats_mafispammer			), 
	AchievementInfo(100,	L_stats_vipspammer			), 
	AchievementInfo(100,	L_stats_sponsorspammer		), 
	AchievementInfo(100,	L_stats_modspammer			), 
	AchievementInfo(100,	L_stats_adminspam			), 
	AchievementInfo(100,	L_stats_cheater				), 
	AchievementInfo(100,	L_stats_helpme				), 
	AchievementInfo(100,	L_stats_privatevehicles		), 
	AchievementInfo(100,	L_stats_mafiavehicles		), 
	AchievementInfo(100,	L_stats_amounthealed		), 
	AchievementInfo(100,	L_stats_mammothtank			), 
	AchievementInfo(100,	L_stats_acsuspect			), 
	AchievementInfo(100,	L_stats_roleplayer			), 
	AchievementInfo(1000000,L_stats_gotmafiascore		), 	
	AchievementInfo(1000,	L_stats_givenmafiascore		), 
	AchievementInfo(10,		L_stats_biggestkillstreak	), 
	AchievementInfo(0,		L_stats_currentkillstreak	), 
	AchievementInfo(50,		L_stats_headshots			), 
	AchievementInfo(10000,	L_stats_waitforxp			), 
	AchievementInfo(0,		L_stats_agr_kills			), 
	AchievementInfo(0,		L_stats_agr_deaths			), 
	AchievementInfo(0,		L_stats_arpg_kills			), 
	AchievementInfo(0,		L_stats_arpg_deaths			), 
	AchievementInfo(0,		L_stats_osh_kills			), 
	AchievementInfo(0,		L_stats_osh_deaths			), 
	AchievementInfo(0,		L_stats_apb_kills			), 
	AchievementInfo(0,		L_stats_apb_deaths			), 
	AchievementInfo(0,		L_stats_ctf_kills			), 
	AchievementInfo(0,		L_stats_ctf_deaths			), 
	AchievementInfo(0,		L_stats_ctf_pickedupflags	), 
	AchievementInfo(0,		L_stats_ctf_capturedflags	), 
	AchievementInfo(0,		L_stats_wns_kills			), 
	AchievementInfo(0,		L_stats_wns_deaths			), 
	AchievementInfo(0,		L_stats_ww3_kills			), 
	AchievementInfo(0,		L_stats_ww3_deaths			), 
	AchievementInfo(0,		L_stats_seeknhide_finds		), 
	AchievementInfo(0,		L_stats_seeknhide_besttime	), 
	AchievementInfo(0,		L_stats_drb_won				), 
	AchievementInfo(0,		L_stats_drb_lost			), 
	AchievementInfo(0,		L_stats_maze_fastesttime	), 
	AchievementInfo(0,		L_stats_tdm_kills			), 
	AchievementInfo(0,		L_stats_tdm_deaths			), 
	AchievementInfo(0,		L_stats_tdm_wins			), 
	AchievementInfo(0,		L_stats_tdm_loses			), 
	AchievementInfo(0,		L_stats_bmxparkourft		), 
	AchievementInfo(0,		L_stats_nrgparkourft		), 
	AchievementInfo(0,		L_stats_parkourft			), 
	AchievementInfo(0,		L_stats_spm_kills			), 
	AchievementInfo(0,		L_stats_spm_deaths			), 
	AchievementInfo(0,		L_stats_spm_wins			), 
	AchievementInfo(0,		L_stats_spm_loses			), 
	AchievementInfo(0,		L_stats_mma_wins			), 
	AchievementInfo(0,		L_stats_mma_loses			), 
	AchievementInfo(0,		L_stats_wjg_kills			), 
	AchievementInfo(0,		L_stats_wjg_deaths			), 
	AchievementInfo(0,		L_stats_wjg_wins			), 
	AchievementInfo(0,		L_stats_wjg_loses			), 
	AchievementInfo(0,		L_stats_racekartft			), 
	AchievementInfo(0,		L_stats_bag_kills			), 
	AchievementInfo(0,		L_stats_bag_deaths			), 
	AchievementInfo(0,		L_stats_bag_wins			), 
	AchievementInfo(0,		L_stats_bag_loses			) 
};

bool GivePlayerAchievement(int playerid, EAchievementsMain which, long long howmuch);