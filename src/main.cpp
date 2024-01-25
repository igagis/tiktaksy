#include <ruisapp/application.hpp>
#include <ruis/widgets/slider/scroll_bar.hpp>
#include <ruis/widgets/button/push_button.hpp>
#include <ruis/widgets/slider/scroll_bar.hpp>

#include "Gauge.hpp"
#include "CubeWidget.hpp"

class application : public ruisapp::application{
public:
	application() :
			ruisapp::application(
					"tiktaksy",
					[](){
						return ruisapp::window_params(r4::vector2<unsigned>(320, 480));
					}()
				)
	{
		this->gui.init_standard_widgets(*this->get_res_file());
		
		this->gui.context.get().loader.mount_res_pack(*this->get_res_file("res/"));
		
		this->gui.context.get().inflater.register_widget<ruis::Gauge>("Gauge");
		this->gui.context.get().inflater.register_widget<CubeWidget>("CubeWidget");
		
		auto c = this->gui.context.get().inflater.inflate(
				*this->get_res_file("res/main.gui")
			);
	
		
		auto gauge = c.get().try_get_widget_as<ruis::Gauge>("gauge");
		ASSERT(gauge)
		auto weakGauge = utki::make_weak(gauge);
		
		auto slider = c.get().try_get_widget_as<ruis::fraction_band_widget>("gauge_slider");
		ASSERT(slider)
		slider->set_band_fraction(0.1);

		auto cube = c.get().try_get_widget_as<CubeWidget>("cubeWidget");
		ASSERT(cube)
		auto weakCube = utki::make_weak(cube);
		auto& btn = c.get().get_widget_as<ruis::push_button>("btnToggleSpinning");

		btn.click_handler = [weakCube, this](ruis::push_button& b){
			if(auto p = weakCube.lock()){
				if(p->is_updating()){
					this->gui.context.get().updater.get().stop(*p);
				}else{
					this->gui.context.get().updater.get().start(p);
				}
			}
		};

		slider->fraction_change_handler = [weakGauge, weakCube](ruis::fraction_widget& s){
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
//				ruis::inst().inflater.inflate(*stob::parse("PushButton{TextLabel{text{Hello}}}"))
				c
			);

#if M_OS_NAME == M_OS_NAME_ANDROID
		this->setFullscreen(true);
#endif

	}
};

ruisapp::application_factory app_fac([](auto args){
	return std::make_unique<::application>();
});
