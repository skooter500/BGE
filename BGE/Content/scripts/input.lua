function addVec3(v1, v2)
	return glm.vec3(v1.x + v2.x, v1.y + v2.y, v1.z + v2.z)
end

function scaleVec3(v, s, c)
	if c == "x" then		
		return glm.vec3(v.x + s, v.y, v.z)
	elseif c == "y" then
		return glm.vec3(v.x, v.y + s, v.z)
	elseif c == "z" then
		return glm.vec3(v.x, v.y, v.z + s)
	else
		return addVec3(v, glm.vec3(s, s, s))
	end
end

function update(delta)

	if transform.scale.x < 1 then
			transform.scale = glm.vec3(1, transform.scale.y, transform.scale.z)
	end

	if transform.scale.y < 1 then
		transform.scale = glm.vec3(transform.scale.x, 1, transform.scale.z)
	end

	if transform.scale.z < 1 then
		transform.scale = glm.vec3(transform.scale.x, transform.scale.y, 1)
	end

	if io.keyState(SDL_SCANCODE_UP) then

		if io.keyState(SDL_SCANCODE_X) then
			transform.scale = scaleVec3(transform.scale, 1, "x")
		elseif io.keyState(SDL_SCANCODE_Y) then
			transform.scale = scaleVec3(transform.scale, 1, "y")
		elseif io.keyState(SDL_SCANCODE_Z) then
			transform.scale = scaleVec3(transform.scale, 1, "z")
		elseif io.keyState(SDL_SCANCODE_V) then
			transform.scale = scaleVec3(transform.scale, 1, "a")
		end

	end

	if io.keyState(SDL_SCANCODE_DOWN) then

		if io.keyState(SDL_SCANCODE_X) then
			transform.scale = scaleVec3(transform.scale, -1, "x")
		elseif io.keyState(SDL_SCANCODE_Y) then
			transform.scale = scaleVec3(transform.scale, -1, "y")
		elseif io.keyState(SDL_SCANCODE_Z) then
			transform.scale = scaleVec3(transform.scale, -1, "z")
		elseif io.keyState(SDL_SCANCODE_V) then
			transform.scale = scaleVec3(transform.scale, -1, "a")
		end

	end

end