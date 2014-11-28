local speed = 90

function update(delta)

	if action == 1234 then
		transform:Yaw(speed * delta)
	end
	
	if action == "pitch" then
		transform:Pitch(speed * delta)
	end
end