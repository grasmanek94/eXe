	#include <a_samp>
	
	main(){}
	
	public OnRconCommand(cmd[])
	{
		return 0;
	}
	
	forward OnClientCheckResponse(playerid, type, arg, response);
	public OnClientCheckResponse(playerid, type, arg, response)
	{
	    CallRemoteFunction("OnClientCheckResponse", "iiii", playerid, type, arg, response);
		return 1;
	}
