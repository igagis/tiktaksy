#include <mordavokne/AppFactory.hpp>
#include <morda/widgets/slider/Slider.hpp>

#include "Gauge.hpp"


class Application : public mordavokne::App{
public:
	Application() :
			App([](){
					return mordavokne::App::WindowParams(kolme::Vec2ui(320, 480));
				}())
	{
		morda::inst().initStandardWidgets(*this->createResourceFileInterface());
		
		morda::inst().resMan.mountResPack(*this->createResourceFileInterface("res/main.res"));
		
		morda::inst().inflater.addWidget<morda::Gauge>("Gauge");
		
		auto c = morda::Morda::inst().inflater.inflate(
				*this->createResourceFileInterface("res/main.gui")
			);
		
		{
			auto gauge = c->findChildByNameAs<morda::Gauge>("gauge");
			ASSERT(gauge)
			auto slider = c->findChildByNameAs<morda::AreaSlider>("gauge_slider");
			ASSERT(slider)
			auto weakGauge = utki::makeWeak(gauge);
			slider->fractionChange = [weakGauge](morda::FractionWidget& s){
				if(auto g = weakGauge.lock()){
					g->setFraction(s.fraction());
				}
			};
		}
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
