#pragma once

#include <morda/widgets/widget.hpp>
#include <morda/updateable.hpp>
#include <morda/res/res_texture.hpp>
#include <morda/render/vertex_array.hpp>

class CubeWidget :
		public morda::widget,
		public morda::updateable
{
	std::shared_ptr<morda::res_texture> tex;
	
	morda::quaternion rot = morda::quaternion().identity();
public:
	std::shared_ptr<morda::vertex_array> cubeVAO;
	
	CubeWidget(std::shared_ptr<morda::context> c, const puu::forest& desc);
	
	unsigned fps = 0;
	std::uint32_t fpsSecCounter = 0;
	
	float spinSpeed = 1;
	
	void update(std::uint32_t dt) override;
	
	void render(const morda::matrix4& matrix)const override;
};
