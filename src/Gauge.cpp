#include "Gauge.hpp"

#include <morda/util/util.hpp>

using namespace morda;

Gauge::Gauge(const stob::Node* chain) :
		Widget(chain),
		BlendingWidget(chain)
{
	this->arrow = morda::inst().resMan.load<morda::ResImage>("morda_img_gauge_arrow");
	
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
}


void Gauge::render(const Matr4r& matrix) const {
	ASSERT(this->arrowQuadTexture)
	
	if(this->arrowQuadTexture->dim().x <= 0 || this->armFraction <= 0){
		return;
	}
	
	Matr4r matr(matrix);
	matr.translate(this->rect().d / 2);
	matr.scale(this->rect().d / 2);
	matr.rotate(this->startAngleRad + (this->endAngleRad - this->startAngleRad) * this->fraction());
	{
		auto div = this->arrowQuadTexture->dim().x * this->armFraction;
		ASSERT(div >= 0)
		matr.scale(1 / div);
	}
	matr.scale(this->arrowQuadTexture->dim());
	matr.translate(-(1 - this->armFraction), -0.5);
	
	this->applyBlending();
	
	this->arrowQuadTexture->render(matr);
}
