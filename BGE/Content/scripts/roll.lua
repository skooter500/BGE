local speed = 90

function update(delta)
	transform:Roll(global_speed * delta)
end