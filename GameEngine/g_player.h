struct PlayerLocomotionState
{
	float origin[4];
	float angles[4];
};

struct PlayerCommands
{
	int buttonInputs;
	int prevButtonInputs;
};

struct PlayerState
{
	PlayerLocomotionState locomotionState;
	PlayerCommands commands;
};


PlayerState* G_GetCurrentPlayerState();
void G_InitPlayerState();
void G_UpdatePlayerState();