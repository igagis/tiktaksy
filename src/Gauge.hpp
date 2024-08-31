#pragma once

#include <ruis/widget/widget.hpp>
#include <ruis/widget/base/blending_widget.hpp>
#include <ruis/widget/base/fraction_widget.hpp>

#include <ruis/res/image.hpp>

namespace ruis{
class Gauge :
		public ruis::blending_widget,
		public ruis::fraction_widget,
		virtual public ruis::widget
{
	std::shared_ptr<ruis::res::image> arrow;
	std::shared_ptr<const ruis::res::image::texture> arrowQuadTexture;
	
	std::shared_ptr<ruis::res::image> shadow;
	std::shared_ptr<const ruis::res::image::texture> shadowQuadTexture;
	
	real armFraction = 1;
	
	real startAngleRad = real(200) * real(utki::pi) / real(180);;
	real endAngleRad = real(-20) * real(utki::pi) / real(180);;
public:
	Gauge(const utki::shared_ref<ruis::context>& c, const tml::forest& desc);
	
	void on_lay_out()override;
	
	void render(const ruis::matrix4& matrix) const override;
};
}
