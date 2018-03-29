#pragma once

#include <morda/widgets/Widget.hpp>
#include <morda/widgets/base/BlendingWidget.hpp>
#include <morda/widgets/base/FractionWidget.hpp>

#include <morda/res/ResImage.hpp>

namespace morda{
class Gauge :
		public morda::BlendingWidget,
		public morda::FractionWidget,
		virtual public morda::Widget
{
	std::shared_ptr<morda::ResImage> arrow;
	std::shared_ptr<const morda::ResImage::QuadTexture> arrowQuadTexture;
	
	std::shared_ptr<morda::ResImage> shadow;
	std::shared_ptr<const morda::ResImage::QuadTexture> shadowQuadTexture;
	
	real armFraction;
	
	real startAngleRad;
	real endAngleRad;
public:
	Gauge(const stob::Node* chain = nullptr);
	
	void layOut() override;
	
	void render(const morda::Matr4r& matrix) const override;
};
}
