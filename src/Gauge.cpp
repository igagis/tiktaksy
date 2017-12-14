#include "Gauge.hpp"

#include <morda/util/util.hpp>

using namespace morda;

Gauge::Gauge(const stob::Node* chain) :
		Widget(chain),
		BlendingWidget(chain)
{
	if(auto p = morda::getProperty(chain, "arrowImage")){
		this->arrow = morda::inst().resMan.load<morda::ResImage>(p->value());
	}else{
		this->arrow = morda::inst().resMan.load<morda::ResImage>("morda_img_gauge_arrow");
	}
	
	if(auto p = morda::getProperty(chain, "shadowImage")){
		try{
			this->shadow = morda::inst().resMan.load<morda::ResImage>(p->value());
		}catch(morda::ResourceManager::Exc& e){
			//do nothing
		}
	}else{
		this->shadow = morda::inst().resMan.load<morda::ResImage>("morda_img_gauge_arrow_shadow");
	}
	
	if(auto p = morda::getProperty(chain, "armFraction")){
		this->armFraction = real(p->asFloat());
	}else{
		this->armFraction = 1;
	}
	
	{
		real deg;
		if(auto p = morda::getProperty(chain, "startAngle")){
			deg = real(p->asFloat());
		}else{
			deg = 200;
		}
		this->startAngleRad = deg * utki::pi<real>() / real(180);
	}
	
	{
		real deg;
		if(auto p = morda::getProperty(chain, "endAngle")){
			deg = real(p->asFloat());
		}else{
			deg = -20;
		}
		this->endAngleRad = deg * utki::pi<real>() / real(180);
	}
}

void Gauge::layOut() {
	ASSERT(this->arrow)
	auto arrowDim = this->arrow->dim();
	real armLength = arrowDim.x * this->armFraction;
	
	if(armLength <= 0){
		return;
	}
	
	auto scale = (std::max(this->rect().d.x, this->rect().d.y) / 2) / armLength;
	
	this->arrowQuadTexture = this->arrow->get(arrowDim * scale);
	
	if(this->shadow){
		TRACE(<< "this->shadow->dim() * scale = " << this->shadow->dim() * scale << std::endl)
		this->shadowQuadTexture = this->shadow->get(this->shadow->dim() * scale);
		TRACE(<< "this->shadowQuadTexture->dim() = " << this->shadowQuadTexture->dim() << std::endl)
	}
}


void Gauge::render(const Matr4r& matrix) const {
	ASSERT(this->arrowQuadTexture)
	
	if(!this->arrowQuadTexture->dim().isPositive() || this->armFraction <= 0){
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
		auto div = this->arrowQuadTexture->dim().x * this->armFraction;
		ASSERT(div >= 0)
		mmm.scale(1 / div);
	}
	
	if(this->shadowQuadTexture && this->shadowQuadTexture->dim().isPositive()){
		auto arrowFraction = this->arrowQuadTexture->dim().x / this->shadowQuadTexture->dim().x;
		
		const auto shadowOffset = real(0.025f);
		
		Matr4r m(matr);
		m.rightMulBy(Matr4r().identity().translate(shadowOffset, shadowOffset) * mmm);
		m.scale(this->shadowQuadTexture->dim());
		m.translate(-(1 - this->armFraction) * arrowFraction - (1 - arrowFraction) / 2, -0.5);
		this->shadowQuadTexture->render(m);
	}
	
	{
		Matr4r m(matr);
		m.rightMulBy(mmm);
		m.scale(this->arrowQuadTexture->dim());
		m.translate(-(1 - this->armFraction), -0.5);
		this->arrowQuadTexture->render(m);
	}
}
