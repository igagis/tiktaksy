#include <mordavokne/AppFactory.hpp>
#include <morda/widgets/slider/ScrollBar.hpp>
#include <morda/widgets/button/PushButton.hpp>
#include <morda/widgets/slider/ScrollBar.hpp>

#include "Gauge.hpp"
#include "CubeWidget.hpp"


class Application : public mordavokne::App{
public:
	Application() :
			App([](){
					return mordavokne::App::WindowParams(kolme::Vec2ui(320, 480));
				}())
	{
		morda::inst().initStandardWidgets(*this->getResFile());
		
		morda::inst().resMan.mountResPack(*this->getResFile("res/"));
		
		morda::inst().inflater.addWidget<morda::Gauge>("Gauge");
		morda::inst().inflater.addWidget<CubeWidget>("CubeWidget");
		
		auto c = morda::Morda::inst().inflater.inflate(
				*this->getResFile("res/main.gui")
			);
	
		
		auto gauge = c->findByNameAs<morda::Gauge>("gauge");
		ASSERT(gauge)
		auto weakGauge = utki::makeWeak(gauge);
		
		auto slider = c->findByNameAs<morda::FractionBandWidget>("gauge_slider");
		ASSERT(slider)
		slider->setBandSizeFraction(0.1);

		auto cube = c->findByNameAs<CubeWidget>("cubeWidget");
		ASSERT(cube)
		auto weakCube = utki::makeWeak(cube);
		auto& btn = c->getByNameAs<morda::PushButton>("btnToggleSpinning");

		btn.clicked = [weakCube](morda::PushButton& b){
			if(auto p = weakCube.lock()){
				if(p->isUpdating()){
					p->stopUpdating();
				}else{
					p->startUpdating();
				}
			}
		};

		slider->fractionChange = [weakGauge, weakCube](morda::FractionWidget& s){
			if(auto g = weakGauge.lock()){
				g->setFraction(s.fraction());
			}
			if(auto p = weakCube.lock()){
				p->spinSpeed = s.fraction();
			}
		};
		
		slider->setFraction(0.1);
//		
		
		morda::Morda::inst().setRootWidget(
//				morda::inst().inflater.inflate(*stob::parse("PushButton{TextLabel{text{Hello}}}"))
				std::move(c)
			);
	}
};



std::unique_ptr<mordavokne::App> mordavokne::createApp(int argc, const char** argv, const utki::Buf<std::uint8_t> savedState){
	return utki::makeUnique<Application>();
}
