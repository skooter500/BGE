local scale = 0

function update(delta)
	scale = scale + delta
	s = glm.sin(scale) * 25

	if s < 0 then
		s = s * -1
	end

	transform.scale = glm.vec3(s, s, s)
end