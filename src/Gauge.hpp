#pragma once

#include <morda/widgets/widget.hpp>
#include <morda/widgets/base/blending_widget.hpp>
#include <morda/widgets/base/fraction_widget.hpp>

#include <morda/res/image.hpp>

namespace morda{
class Gauge :
		public morda::blending_widget,
		public morda::fraction_widget,
		virtual public morda::widget
{
	std::shared_ptr<morda::res::image> arrow;
	std::shared_ptr<const morda::res::image::texture> arrowQuadTexture;
	
	std::shared_ptr<morda::res::image> shadow;
	std::shared_ptr<const morda::res::image::texture> shadowQuadTexture;
	
	real armFraction = 1;
	
	real startAngleRad = real(200) * utki::pi<real>() / real(180);;
	real endAngleRad = real(-20) * utki::pi<real>() / real(180);;
public:
	Gauge(std::shared_ptr<morda::context> c, const treeml::forest& desc);
	
	void lay_out()override;
	
	void render(const morda::matrix4& matrix) const override;
};
}
