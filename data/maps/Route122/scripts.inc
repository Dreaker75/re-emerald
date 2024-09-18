Route122_MapScripts::
	map_script MAP_SCRIPT_ON_TRANSITION, Route122_OnTransition
	.byte 0


Route122_OnTransition::
	setflag FLAG_VISITED_ROUTE_122
	return

