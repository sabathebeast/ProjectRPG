io.write("Player Script Loaded...\n")

playerSpeed = 200

function addToPlayerSpeed(a,b)
	return playerSpeed + a
end

function updatePlayerPosition()
	print("Lua version of updatePlayerPosition called")
	lua_updatePlayerPosition()
end

--Q1 starts

local function releaseStorage(player, value) --added one more argument for the value
	local storageValue = -1 --more understandable, not a magic number
	player:setStorageValue(value, storageValue) --modified function call
	end

function onLogout(player)
	if not player then
		print("Error: player not found!")
		return false --return early if player is not valid
	else
		local value = 1000 --more understandable, not a magic number

		if player:getStorageValue(value) == 1 then
			releaseStorage(player, value)
		end

		return true
	end
end

--Q1 ends

--Q2 starts

guilds = 
	{
		[1] = {members = 500, name = "Progress"},
		[2] = {members = 100, name = "Validus"}
	}

function addGuild(guildName, memberCount)
    local newIndex = #guilds + 1  --get the next available index
    guilds[newIndex] = {  
        members = memberCount,
        name = guildName
    } --create the new guild at the new index
end

function printSmallGuildNames(memberCount) -- this method is supposed to print names of all guilds that have less than memberCount max members
	for i = 1, #guilds do
		if guilds[i].members < memberCount then
			print(guilds[i].name)
		end
	end
end

--Q2 ends

--Q3 starts

function removePlayerFromParty(playerID, partyMemberName) -- better naming, self explanatory

	local player = Player(playerID) -- player can be local to the function

	if not player then
		print("Player not found.") -- return early if the player is not found
		return
	end

	local party = player:getParty()

	if not party then
		print("Player is not in a party.") -- return early if there is no party
		return
	end

	for k,v in pairs(party:getMembers()) do

		if v == Player(partyMemberName) then
			party:removeMember(v) -- simplified version
			break -- if we found the one we can break the loop
		end
	end
end

--Q3 ends

--Q4 starts

void Game::addItemToPlayer(const std::string& recipient, uint16_t itemID)
{
	Player* player = g_game.getPlayerByName(recipient); --or use std::unique_ptr<Player>

	if (!player) 
	{
		player = new Player(nullptr);

		if (!IOLoginData::loadPlayerByName(player, recipient)) 
		{
			delete player; --delete player to prevent memory leaks
			return;
		}
	}

	Item* item = Item::CreateItem(itemID); --or use std::unique_ptr<Item> item

	if (!item) 
	{
		delete item; --if Item::CreateItem(itemID) allocates memory on the heap
		delete player; --delete player to prevent memory leaks
		return;
	}

	g_game.internalAddItem(player->getInbox(), item, INDEX_WHEREEVER, FLAG_NOLIMIT);

	if (player->isOffline())
	{
		IOLoginData::savePlayer(player);
	}

	delete item; --if Item::CreateItem(itemID) allocates memory on the heap
	delete player; --delete player to prevent memory leaks
}

--Q4 ends