#include "g_input.h"

static PlayerState g_playerState;

void G_InitPlayerState()
{
	
}

static void G_UpdatePlayerLocomotion()
{
	// modify playerState.origin and angles based on inputs
	// Camera must then use the playerstate values to decide where the scene is rendered from
}

void G_UpdatePlayerState()
{
	G_UpdatePlayerLocomotion();
}

PlayerState* G_GetCurrentPlayerState()
{
	return &g_playerState;
}