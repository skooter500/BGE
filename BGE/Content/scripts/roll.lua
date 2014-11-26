local speed = 90

function update(delta)

	transform:Roll(speed * delta)

	--if action == "yaw" then
	--	transform:Yaw(speed * delta)
	--end
	--
	--if action == "pitch" then
	--	transform:Pitch(speed * delta)
	--end
end