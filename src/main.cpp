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
						return mordavokne::window_params(r4::vector2<unsigned>(320, 480));
					}()
				)
	{
		this->gui.initStandardWidgets(*this->get_res_file());
		
		this->gui.context.get().loader.mount_res_pack(*this->get_res_file("res/"));
		
		this->gui.context.get().inflater.register_widget<morda::Gauge>("Gauge");
		this->gui.context.get().inflater.register_widget<CubeWidget>("CubeWidget");
		
		auto c = this->gui.context.get().inflater.inflate(
				*this->get_res_file("res/main.gui")
			);
	
		
		auto gauge = c.get().try_get_widget_as<morda::Gauge>("gauge");
		ASSERT(gauge)
		auto weakGauge = utki::make_weak(gauge);
		
		auto slider = c.get().try_get_widget_as<morda::fraction_band_widget>("gauge_slider");
		ASSERT(slider)
		slider->set_band_fraction(0.1);

		auto cube = c.get().try_get_widget_as<CubeWidget>("cubeWidget");
		ASSERT(cube)
		auto weakCube = utki::make_weak(cube);
		auto& btn = c.get().get_widget_as<morda::push_button>("btnToggleSpinning");

		btn.click_handler = [weakCube, this](morda::push_button& b){
			if(auto p = weakCube.lock()){
				if(p->is_updating()){
					this->gui.context.get().updater.get().stop(*p);
				}else{
					this->gui.context.get().updater.get().start(p);
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
				c.to_shared_ptr()
			);

#if M_OS_NAME == M_OS_NAME_ANDROID
		this->setFullscreen(true);
#endif

	}
};

mordavokne::application_factory app_fac([](auto args){
	return std::make_unique<::application>();
});
