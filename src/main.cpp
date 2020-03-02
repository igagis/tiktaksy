#include <mordavokne/application.hpp>
#include <morda/widgets/slider/scroll_bar.hpp>
#include <morda/widgets/button/push_button.hpp>
#include <morda/widgets/slider/scroll_bar.hpp>

#include "Gauge.hpp"
#include "CubeWidget.hpp"


class application : public mordavokne::application{
public:
	application() :
			mordavokne::application(
					"tiktaksy",
					[](){
						return mordavokne::window_params(r4::vec2ui(320, 480));
					}()
				)
	{
		this->gui.initStandardWidgets(*this->get_res_file());
		
		this->gui.context->loader.mount_res_pack(*this->get_res_file("res/"));
		
		this->gui.context->inflater.register_widget<morda::Gauge>("Gauge");
		this->gui.context->inflater.register_widget<CubeWidget>("CubeWidget");
		
		auto c = this->gui.context->inflater.inflate(
				*this->get_res_file("res/main.gui")
			);
	
		
		auto gauge = c->try_get_widget_as<morda::Gauge>("gauge");
		ASSERT(gauge)
		auto weakGauge = utki::make_weak(gauge);
		
		auto slider = c->try_get_widget_as<morda::fraction_band_widget>("gauge_slider");
		ASSERT(slider)
		slider->set_band_fraction(0.1);

		auto cube = c->try_get_widget_as<CubeWidget>("cubeWidget");
		ASSERT(cube)
		auto weakCube = utki::makeWeak(cube);
		auto& btn = c->get_widget_as<morda::push_button>("btnToggleSpinning");

		btn.click_handler = [weakCube, this](morda::push_button& b){
			if(auto p = weakCube.lock()){
				if(p->is_updating()){
					this->gui.context->updater->stop(*p);
				}else{
					this->gui.context->updater->start(p);
				}
			}
		};

		slider->fraction_change_handler = [weakGauge, weakCube](morda::fraction_widget& s){
			if(auto g = weakGauge.lock()){
				g->set_fraction(s.fraction());
			}
			if(auto p = weakCube.lock()){
				p->spinSpeed = s.fraction();
			}
		};
		
		slider->set_fraction(0.1);
//		
		
		this->gui.set_root(
//				morda::inst().inflater.inflate(*stob::parse("PushButton{TextLabel{text{Hello}}}"))
				std::move(c)
			);

#if M_OS_NAME == M_OS_NAME_ANDROID
		this->setFullscreen(true);
#endif

	}
};



std::unique_ptr<mordavokne::application> mordavokne::create_application(int argc, const char** argv){
	return std::make_unique<::application>();
}
