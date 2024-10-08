#include "Gauge.hpp"

#include <ruis/util/util.hpp>

using namespace ruis;

Gauge::Gauge(const utki::shared_ref<ruis::context>& c, const tml::forest& desc) :
		widget(c, desc),
		blending_widget(this->context, desc),
		fraction_widget(this->context, {})
{
	bool shadow_attribute_found = false;

	for(const auto& p : desc){
		if(!is_property(p)){
			continue;
		}

		if(p.value == "armFraction"){
			this->armFraction = ruis::get_property_value(p).to_float();
		}else if(p.value == "startAngle"){
			auto deg = ruis::get_property_value(p).to_float();
			this->startAngleRad = deg * real(utki::pi) / real(180);
		}else if(p.value == "endAngle"){
			auto deg = ruis::get_property_value(p).to_float();
			this->endAngleRad = deg * real(utki::pi) / real(180);
		}else if(p.value == "arrowImage"){
			this->arrow = this->context.get().loader.load<ruis::res::image>(ruis::get_property_value(p).string);
		}else if(p.value == "shadowImage"){
			shadow_attribute_found = true;
			try{
				this->shadow = this->context.get().loader.load<ruis::res::image>(ruis::get_property_value(p).string);
			}catch(std::runtime_error& e){
				// do nothing
			}
		}
	}

	if(!this->arrow){
		this->arrow = this->context.get().loader.load<ruis::res::image>("ruis_img_gauge_arrow");
	}

	if(!shadow_attribute_found && !this->shadow){
		this->shadow = this->context.get().loader.load<ruis::res::image>("ruis_img_gauge_arrow_shadow");
	}
	
}

void Gauge::on_lay_out(){
	ASSERT(this->arrow)
	auto arrowDim = this->arrow->dims();
	real armLength = arrowDim.x() * this->armFraction;
	
	if(armLength <= 0){
		return;
	}
	
	auto scale = (std::max(this->rect().d.x(), this->rect().d.y()) / 2) / armLength;
	
	this->arrowQuadTexture = this->arrow->get(arrowDim.to<ruis::real>() * scale).to_shared_ptr();
	
	if(this->shadow){
		// TRACE(<< "this->shadow->dims() * scale = " << this->shadow->dims() * scale << std::endl)
		this->shadowQuadTexture = this->shadow->get(this->shadow->dims().to<ruis::real>() * scale).to_shared_ptr();
		// TRACE(<< "this->shadowQuadTexture->dims() = " << this->shadowQuadTexture->dims() << std::endl)
	}
}


void Gauge::render(const matrix4& matrix) const {
	ASSERT(this->arrowQuadTexture)
	
	if(!this->arrowQuadTexture->dims().is_positive() || this->armFraction <= 0){
		return;
	}
	
	this->set_blending_to_renderer();
	
	matrix4 matr(matrix);
	matr.translate(this->rect().d / 2);
	matr.scale(this->rect().d / 2);
	
	matrix4 mmm;
	mmm.set_identity();
	mmm.rotate(ruis::quat(-(this->startAngleRad + (this->endAngleRad - this->startAngleRad) * this->get_fraction())));
	{
		auto div = ruis::real(this->arrowQuadTexture->dims().x()) * this->armFraction;
		ASSERT(div >= 0)
		mmm.scale(1 / div);
	}
	
	if(this->shadowQuadTexture && this->shadowQuadTexture->dims().is_positive()){
		auto arrowFraction = ruis::real(this->arrowQuadTexture->dims().x()) / ruis::real(this->shadowQuadTexture->dims().x());
		
		const auto shadowOffset = real(0.025f);
		
		matrix4 m(matr);
		m *= matrix4().set_identity().translate(shadowOffset, shadowOffset) * mmm;
		m.scale(this->shadowQuadTexture->dims().to<ruis::real>());
		m.translate(-(1 - this->armFraction) * arrowFraction - (1 - arrowFraction) / 2, -0.5);
		this->shadowQuadTexture->render(m);
	}
	
	{
		matrix4 m(matr);
		m *= mmm;
		m.scale(this->arrowQuadTexture->dims().to<ruis::real>());
		m.translate(-(1 - this->armFraction), -0.5);
		this->arrowQuadTexture->render(m);
	}
}
