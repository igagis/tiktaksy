#include "CubeWidget.hpp"

#include <ruis/context.hpp>

#include <utki/config.hpp>

#if M_OS_NAME == M_OS_NAME_ANDROID
#	include <GLES/gl.h>
#else
#	include <GL/gl.h>
#endif

CubeWidget::CubeWidget(const utki::shared_ref<ruis::context>& c, const tml::forest& desc) :
		ruis::widget(c, desc)
{
	std::array<ruis::vector3, 36> cubePos = {
		{
			r4::vector3<float>(-1, -1, 1), r4::vector3<float>(1, -1, 1), r4::vector3<float>(-1, 1, 1),
			r4::vector3<float>(1, -1, 1), r4::vector3<float>(1, 1, 1), r4::vector3<float>(-1, 1, 1),

			r4::vector3<float>(1, -1, 1), r4::vector3<float>(1, -1, -1), r4::vector3<float>(1, 1, 1),
			r4::vector3<float>(1, -1, -1), r4::vector3<float>(1, 1, -1), r4::vector3<float>(1, 1, 1),

			r4::vector3<float>(1, -1, -1), r4::vector3<float>(-1, -1, -1), r4::vector3<float>(1, 1, -1),
			r4::vector3<float>(-1, -1, -1), r4::vector3<float>(-1, 1, -1), r4::vector3<float>(1, 1, -1),

			r4::vector3<float>(-1, -1, -1), r4::vector3<float>(-1, -1, 1), r4::vector3<float>(-1, 1, -1),
			r4::vector3<float>(-1, -1, 1), r4::vector3<float>(-1, 1, 1), r4::vector3<float>(-1, 1, -1),

			r4::vector3<float>(-1, 1, -1), r4::vector3<float>(-1, 1, 1), r4::vector3<float>(1, 1, -1),
			r4::vector3<float>(-1, 1, 1), r4::vector3<float>(1, 1, 1), r4::vector3<float>(1, 1, -1),

			r4::vector3<float>(-1, -1, -1), r4::vector3<float>(1, -1, -1), r4::vector3<float>(-1, -1, 1),
			r4::vector3<float>(-1, -1, 1), r4::vector3<float>(1, -1, -1), r4::vector3<float>(1, -1, 1)
		}};

	auto posVBO = this->context.get().renderer.get().factory->create_vertex_buffer(utki::make_span(cubePos));

	std::array<r4::vector2<float>, 36> cubeTex = {
		{
			r4::vector2<float>(0, 0), r4::vector2<float>(0, 1), r4::vector2<float>(1, 0),
			r4::vector2<float>(0, 1), r4::vector2<float>(1, 1), r4::vector2<float>(1, 0),

			r4::vector2<float>(0, 0), r4::vector2<float>(0, 1), r4::vector2<float>(1, 0),
			r4::vector2<float>(0, 1), r4::vector2<float>(1, 1), r4::vector2<float>(1, 0),

			r4::vector2<float>(0, 0), r4::vector2<float>(0, 1), r4::vector2<float>(1, 0),
			r4::vector2<float>(0, 1), r4::vector2<float>(1, 1), r4::vector2<float>(1, 0),

			r4::vector2<float>(0, 0), r4::vector2<float>(0, 1), r4::vector2<float>(1, 0),
			r4::vector2<float>(0, 1), r4::vector2<float>(1, 1), r4::vector2<float>(1, 0),
			
			r4::vector2<float>(0, 0), r4::vector2<float>(0, 1), r4::vector2<float>(1, 0),
			r4::vector2<float>(0, 1), r4::vector2<float>(1, 1), r4::vector2<float>(1, 0),

			r4::vector2<float>(0, 0), r4::vector2<float>(0, 1), r4::vector2<float>(1, 0),
			r4::vector2<float>(0, 1), r4::vector2<float>(1, 1), r4::vector2<float>(1, 0)
		}};

	auto texVBO = this->context.get().renderer.get().factory->create_vertex_buffer(utki::make_span(cubeTex));

	std::array<std::uint16_t, 36> indices = {
		{
			0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31, 32, 33, 34, 35
		}};

	auto cubeIndices = this->context.get().renderer.get().factory->create_index_buffer(utki::make_span(indices));

	this->cubeVAO = this->context.get().renderer.get().factory->create_vertex_array({posVBO, texVBO}, cubeIndices, ruis::render::vertex_array::mode::triangles).to_shared_ptr();

	this->tex = this->context.get().loader.load<ruis::res::texture_2d>("tex_sample").to_shared_ptr();
	this->rot.set_identity();


}

void CubeWidget::update(std::uint32_t dt) {
	const float maxSpeed = 20;
	
	this->fpsSecCounter += dt;
	++this->fps;
	this->rot *= ruis::quaternion().set_rotation(r4::vector3<float>(1, 2, 1).normalize(), maxSpeed * this->spinSpeed * (float(dt) / 1000));
	if (this->fpsSecCounter >= 1000) {
		utki::log([&](auto&o){o << "fps = " << std::dec << fps << std::endl;});
		this->fpsSecCounter = 0;
		this->fps = 0;
	}
}

void CubeWidget::render(const ruis::matrix4& matrix) const {
	this->widget::render(matrix);

	float s = 1.5;
	
	ruis::matrix4 matr(matrix);
	matr.scale(this->rect().d / s);
	matr.translate(s/2, s/2);
	matr.scale(1, -1);
	matr.frustum(-2, 2, -1.5, 1.5, 2, 100);

	ruis::matrix4 m(matr);
	m.translate(0, 0, -4);

	m.rotate(this->rot);

	glEnable(GL_CULL_FACE);

	this->context.get().renderer.get().shader->pos_tex->render(m, *this->cubeVAO, this->tex->tex());

	glDisable(GL_CULL_FACE);
}
