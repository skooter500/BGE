b_24377_t_ = transform 
function b_24377_move(amount, onX, onY, onZ) 
	--transform.position = Math.vec3(amount * onX, amount * onY, amount * onZ)
	transform:Yaw(100)
end
function final_update(delta)

	b_24377_move(20, 0, 0, -1)
	transform:Roll(10)
end
function final_draw()
end
