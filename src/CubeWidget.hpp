#pragma once

#include <morda/widgets/widget.hpp>
#include <morda/Updateable.hpp>
#include <morda/res/ResTexture.hpp>
#include <morda/render/VertexArray.hpp>

class CubeWidget :
		public morda::Widget,
		public morda::Updateable
{
	std::shared_ptr<morda::ResTexture> tex;
	
	morda::Quatr rot = morda::Quatr().identity();
public:
	std::shared_ptr<morda::VertexArray> cubeVAO;
	
	CubeWidget(const puu::forest& desc);
	
	unsigned fps = 0;
	std::uint32_t fpsSecCounter = 0;
	
	float spinSpeed = 1;
	
	void update(std::uint32_t dt) override;
	
	void render(const morda::Matr4r& matrix)const override;
};
