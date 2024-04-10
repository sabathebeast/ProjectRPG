io.write("Player Script Loaded...\n")

playerSpeed = 200

function addToPlayerSpeed(a,b)
	return playerSpeed + a
end

function updatePlayerPosition()
	print("Lua version of updatePlayerPosition called")
	lua_updatePlayerPosition()
end