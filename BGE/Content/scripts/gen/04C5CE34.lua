b_25424_t_ = transform 
function b_25424_move(amount, onX, onY, onZ) 
	--transform.position = Math.vec3(amount * onX, amount * onY, amount * onZ)
	transform:Yaw(100)
end
function final_update(delta)

	b_25424_move(20, 0, 0, -1)
	tranform::Roll(10)
end
function final_draw()
end
