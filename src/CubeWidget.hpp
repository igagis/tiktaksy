#pragma once

#include <ruis/widget.hpp>
#include <ruis/updateable.hpp>
#include <ruis/res/texture.hpp>
#include <ruis/render/vertex_array.hpp>

class CubeWidget :
		public ruis::widget,
		public ruis::updateable
{
	std::shared_ptr<ruis::res::texture> tex;
	
	ruis::quaternion rot = ruis::quaternion().set_identity();
public:
	std::shared_ptr<ruis::vertex_array> cubeVAO;
	
	CubeWidget(const utki::shared_ref<ruis::context>& c, const tml::forest& desc);
	
	unsigned fps = 0;
	std::uint32_t fpsSecCounter = 0;
	
	float spinSpeed = 1;
	
	void update(std::uint32_t dt) override;
	
	void render(const ruis::matrix4& matrix)const override;
};
