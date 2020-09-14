#include "Gauge.hpp"

#include <morda/util/util.hpp>

using namespace morda;

Gauge::Gauge(std::shared_ptr<morda::context> c, const puu::forest& desc) :
		widget(std::move(c), desc),
		blending_widget(this->context, desc),
		fraction_widget(this->context, desc)
{
	bool shadow_attribute_found = false;

	for(const auto& p : desc){
		if(!is_property(p)){
			continue;
		}

		if(p.value == "armFraction"){
			this->armFraction = morda::get_property_value(p).to_float();
		}else if(p.value == "startAngle"){
			auto deg = morda::get_property_value(p).to_float();
			this->startAngleRad = deg * utki::pi<real>() / real(180);
		}else if(p.value == "endAngle"){
			auto deg = morda::get_property_value(p).to_float();
			this->endAngleRad = deg * utki::pi<real>() / real(180);
		}else if(p.value == "arrowImage"){
			this->arrow = this->context->loader.load<morda::res::image>(morda::get_property_value(p).to_string());
		}else if(p.value == "shadowImage"){
			shadow_attribute_found = true;
			try{
				this->shadow = this->context->loader.load<morda::res::image>(morda::get_property_value(p).to_string());
			}catch(std::runtime_error& e){
				// do nothing
			}
		}
	}

	if(!this->arrow){
		this->arrow = this->context->loader.load<morda::res::image>("morda_img_gauge_arrow");
	}

	if(!shadow_attribute_found && !this->shadow){
		this->shadow = this->context->loader.load<morda::res::image>("morda_img_gauge_arrow_shadow");
	}
	
}

void Gauge::lay_out(){
	ASSERT(this->arrow)
	auto arrowDim = this->arrow->dims();
	real armLength = arrowDim.x() * this->armFraction;
	
	if(armLength <= 0){
		return;
	}
	
	auto scale = (std::max(this->rect().d.x(), this->rect().d.y()) / 2) / armLength;
	
	this->arrowQuadTexture = this->arrow->get(arrowDim * scale);
	
	if(this->shadow){
		// TRACE(<< "this->shadow->dims() * scale = " << this->shadow->dims() * scale << std::endl)
		this->shadowQuadTexture = this->shadow->get(this->shadow->dims() * scale);
		// TRACE(<< "this->shadowQuadTexture->dims() = " << this->shadowQuadTexture->dims() << std::endl)
	}
}


void Gauge::render(const matrix4& matrix) const {
	ASSERT(this->arrowQuadTexture)
	
	if(!this->arrowQuadTexture->dims.is_positive() || this->armFraction <= 0){
		return;
	}
	
	this->set_blending_to_renderer();
	
	matrix4 matr(matrix);
	matr.translate(this->rect().d / 2);
	matr.scale(this->rect().d / 2);
	
	matrix4 mmm;
	mmm.set_identity();
	mmm.rotate(-(this->startAngleRad + (this->endAngleRad - this->startAngleRad) * this->fraction()));
	{
		auto div = this->arrowQuadTexture->dims.x() * this->armFraction;
		ASSERT(div >= 0)
		mmm.scale(1 / div);
	}
	
	if(this->shadowQuadTexture && this->shadowQuadTexture->dims.is_positive()){
		auto arrowFraction = this->arrowQuadTexture->dims.x() / this->shadowQuadTexture->dims.x();
		
		const auto shadowOffset = real(0.025f);
		
		matrix4 m(matr);
		m.right_mul(matrix4().set_identity().translate(shadowOffset, shadowOffset) * mmm);
		m.scale(this->shadowQuadTexture->dims);
		m.translate(-(1 - this->armFraction) * arrowFraction - (1 - arrowFraction) / 2, -0.5);
		this->shadowQuadTexture->render(m);
	}
	
	{
		matrix4 m(matr);
		m.right_mul(mmm);
		m.scale(this->arrowQuadTexture->dims);
		m.translate(-(1 - this->armFraction), -0.5);
		this->arrowQuadTexture->render(m);
	}
}
