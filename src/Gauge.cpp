#include "Gauge.hpp"

#include <morda/util/util.hpp>

using namespace morda;

Gauge::Gauge(const puu::forest& desc) :
		Widget(desc),
		BlendingWidget(desc),
		FractionWidget(desc)
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
			this->arrow = morda::inst().resMan.load<morda::ResImage>(morda::get_property_value(p).to_string());
		}else if(p.value == "shadowImage"){
			shadow_attribute_found = true;
			try{
				this->shadow = morda::inst().resMan.load<morda::ResImage>(morda::get_property_value(p).to_string());
			}catch(morda::ResourceManager::Exc& e){
				// do nothing
			}
		}
	}

	if(!this->arrow){
		this->arrow = morda::inst().resMan.load<morda::ResImage>("morda_img_gauge_arrow");
	}

	if(!shadow_attribute_found && !this->shadow){
		this->shadow = morda::inst().resMan.load<morda::ResImage>("morda_img_gauge_arrow_shadow");
	}
	
}

void Gauge::layOut() {
	ASSERT(this->arrow)
	auto arrowDim = this->arrow->dims();
	real armLength = arrowDim.x * this->armFraction;
	
	if(armLength <= 0){
		return;
	}
	
	auto scale = (std::max(this->rect().d.x, this->rect().d.y) / 2) / armLength;
	
	this->arrowQuadTexture = this->arrow->get(arrowDim * scale);
	
	if(this->shadow){
		// TRACE(<< "this->shadow->dims() * scale = " << this->shadow->dims() * scale << std::endl)
		this->shadowQuadTexture = this->shadow->get(this->shadow->dims() * scale);
		// TRACE(<< "this->shadowQuadTexture->dims() = " << this->shadowQuadTexture->dims() << std::endl)
	}
}


void Gauge::render(const Matr4r& matrix) const {
	ASSERT(this->arrowQuadTexture)
	
	if(!this->arrowQuadTexture->dims().isPositive() || this->armFraction <= 0){
		return;
	}
	
	this->applyBlending();
	
	Matr4r matr(matrix);
	matr.translate(this->rect().d / 2);
	matr.scale(this->rect().d / 2);
	
	Matr4r mmm;
	mmm.identity();
	mmm.rotate(-(this->startAngleRad + (this->endAngleRad - this->startAngleRad) * this->fraction()));
	{
		auto div = this->arrowQuadTexture->dims().x * this->armFraction;
		ASSERT(div >= 0)
		mmm.scale(1 / div);
	}
	
	if(this->shadowQuadTexture && this->shadowQuadTexture->dims().isPositive()){
		auto arrowFraction = this->arrowQuadTexture->dims().x / this->shadowQuadTexture->dims().x;
		
		const auto shadowOffset = real(0.025f);
		
		Matr4r m(matr);
		m.rightMulBy(Matr4r().identity().translate(shadowOffset, shadowOffset) * mmm);
		m.scale(this->shadowQuadTexture->dims());
		m.translate(-(1 - this->armFraction) * arrowFraction - (1 - arrowFraction) / 2, -0.5);
		this->shadowQuadTexture->render(m);
	}
	
	{
		Matr4r m(matr);
		m.rightMulBy(mmm);
		m.scale(this->arrowQuadTexture->dims());
		m.translate(-(1 - this->armFraction), -0.5);
		this->arrowQuadTexture->render(m);
	}
}
