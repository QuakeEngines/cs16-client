
#include "hud.h"
#include "parsemsg.h"
#include "cl_util.h"
#include <string.h>

DECLARE_MESSAGE( m_Radio, SendAudio )
DECLARE_MESSAGE( m_Radio, ReloadSound )

int CHudRadio::Init( )
{
	HOOK_MESSAGE( SendAudio );
	HOOK_MESSAGE( ReloadSound );
	gHUD.AddHudElem(this);
	m_iFlags = 0;
	return 1;
}

int CHudRadio::Draw(float flTime)
{
	if( m_sentence[0] == '%' )
		PlaySound( &m_sentence[1], m_iPitch );
	else
		PlaySound( m_sentence, m_iPitch );

	m_iFlags = 0;

	return 1;
}

int CHudRadio::VidInit()
{
	return 1;
}

int CHudRadio::MsgFunc_SendAudio(const char *pszName, int iSize, void *pbuf)
{
	BEGIN_READ( pbuf, iSize );

	m_iSenderID = READ_BYTE( );
	strncpy( m_sentence, READ_STRING( ), sizeof(m_sentence));
	m_iPitch = READ_SHORT( );
	m_iFlags = HUD_ACTIVE;

	g_PlayerExtraInfo[m_iSenderID].radarflashes = 22;
	g_PlayerExtraInfo[m_iSenderID].radarflash = gHUD.m_flTime;
	g_PlayerExtraInfo[m_iSenderID].radarflashon = 1;
	return 1;
}

int CHudRadio::MsgFunc_ReloadSound(const char *pszName, int iSize, void *pbuf)
{
	BEGIN_READ( pbuf, iSize );

	m_iPitch = READ_BYTE();
	int isNotShotgun = READ_BYTE();

	strncpy( m_sentence,  (char*)(isNotShotgun? "weapons/generic_reload.wav" : "weapons/generic_shot_reload.wav"), sizeof(m_sentence));
	m_iFlags = HUD_ACTIVE;

   return 0;
}

void Broadcast( const char *sentence )
{
	if( sentence[0] == '%' )
		PlaySound( (char*)&sentence[1], 100.0 );
	else
		PlaySound( (char*)sentence, 100.0 );
}
