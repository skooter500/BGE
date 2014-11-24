
local t_ = transform

//function move(amount, dir)
//	v = transform.position
//	v.x = v.x + (v.x * (dir.x * amount)) + transform.position.x
//	v.y = v.y + (v.y * (dir.y * amount)) + transform.position.y
//	v.z = v.z + (v.z * (dir.z * amount)) + transform.position.z
//	ttt = v.z
//	transform.position = Math.vec3(v.x, v.y, v.z)
//end

function update(delta)
	transform:Roll(0.5)
end