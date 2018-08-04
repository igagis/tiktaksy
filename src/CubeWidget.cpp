#include "CubeWidget.hpp"

#include <morda/Morda.hpp>

#include <GL/gl.h>

CubeWidget::CubeWidget(const stob::Node* desc) :
		Widget(desc)
{
	std::array<morda::Vec3r, 36> cubePos = {
		{
			kolme::Vec3f(-1, -1, 1), kolme::Vec3f(1, -1, 1), kolme::Vec3f(-1, 1, 1),
			kolme::Vec3f(1, -1, 1), kolme::Vec3f(1, 1, 1), kolme::Vec3f(-1, 1, 1),

			kolme::Vec3f(1, -1, 1), kolme::Vec3f(1, -1, -1), kolme::Vec3f(1, 1, 1),
			kolme::Vec3f(1, -1, -1), kolme::Vec3f(1, 1, -1), kolme::Vec3f(1, 1, 1),

			kolme::Vec3f(1, -1, -1), kolme::Vec3f(-1, -1, -1), kolme::Vec3f(1, 1, -1),
			kolme::Vec3f(-1, -1, -1), kolme::Vec3f(-1, 1, -1), kolme::Vec3f(1, 1, -1),

			kolme::Vec3f(-1, -1, -1), kolme::Vec3f(-1, -1, 1), kolme::Vec3f(-1, 1, -1),
			kolme::Vec3f(-1, -1, 1), kolme::Vec3f(-1, 1, 1), kolme::Vec3f(-1, 1, -1),

			kolme::Vec3f(-1, 1, -1), kolme::Vec3f(-1, 1, 1), kolme::Vec3f(1, 1, -1),
			kolme::Vec3f(-1, 1, 1), kolme::Vec3f(1, 1, 1), kolme::Vec3f(1, 1, -1),

			kolme::Vec3f(-1, -1, -1), kolme::Vec3f(1, -1, -1), kolme::Vec3f(-1, -1, 1),
			kolme::Vec3f(-1, -1, 1), kolme::Vec3f(1, -1, -1), kolme::Vec3f(1, -1, 1)
		}};

	auto posVBO = morda::inst().renderer().factory->createVertexBuffer(utki::wrapBuf(cubePos));

	std::array<kolme::Vec2f, 36> cubeTex = {
		{
			kolme::Vec2f(0, 0), kolme::Vec2f(0, 1), kolme::Vec2f(1, 0),
			kolme::Vec2f(0, 1), kolme::Vec2f(1, 1), kolme::Vec2f(1, 0),

			kolme::Vec2f(0, 0), kolme::Vec2f(0, 1), kolme::Vec2f(1, 0),
			kolme::Vec2f(0, 1), kolme::Vec2f(1, 1), kolme::Vec2f(1, 0),

			kolme::Vec2f(0, 0), kolme::Vec2f(0, 1), kolme::Vec2f(1, 0),
			kolme::Vec2f(0, 1), kolme::Vec2f(1, 1), kolme::Vec2f(1, 0),

			kolme::Vec2f(0, 0), kolme::Vec2f(0, 1), kolme::Vec2f(1, 0),
			kolme::Vec2f(0, 1), kolme::Vec2f(1, 1), kolme::Vec2f(1, 0),
			
			kolme::Vec2f(0, 0), kolme::Vec2f(0, 1), kolme::Vec2f(1, 0),
			kolme::Vec2f(0, 1), kolme::Vec2f(1, 1), kolme::Vec2f(1, 0),

			kolme::Vec2f(0, 0), kolme::Vec2f(0, 1), kolme::Vec2f(1, 0),
			kolme::Vec2f(0, 1), kolme::Vec2f(1, 1), kolme::Vec2f(1, 0)
		}};

	auto texVBO = morda::inst().renderer().factory->createVertexBuffer(utki::wrapBuf(cubeTex));

	std::array<std::uint16_t, 36> indices = {
		{
			0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31, 32, 33, 34, 35
		}};

	auto cubeIndices = morda::inst().renderer().factory->createIndexBuffer(utki::wrapBuf(indices));

	this->cubeVAO = morda::inst().renderer().factory->createVertexArray({posVBO, texVBO}, cubeIndices, morda::VertexArray::Mode_e::TRIANGLES);

	this->tex = morda::Morda::inst().resMan.load<morda::ResTexture>("tex_sample");
	this->rot.identity();


}

void CubeWidget::update(std::uint32_t dt) {
	const float maxSpeed = 20;
	
	this->fpsSecCounter += dt;
	++this->fps;
	this->rot %= morda::Quatr().initRot(kolme::Vec3f(1, 2, 1).normalize(), maxSpeed * this->spinSpeed * (float(dt) / 1000));
	if (this->fpsSecCounter >= 1000) {
		TRACE_ALWAYS( << "fps = " << std::dec << fps << std::endl)
				this->fpsSecCounter = 0;
		this->fps = 0;
	}
}

void CubeWidget::render(const morda::Matr4r& matrix) const {
	this->Widget::render(matrix);

	float s = 1.5;
	
	morda::Matr4r matr(matrix);
	matr.scale(this->rect().d / s);
	matr.translate(s/2, s/2);
	matr.scale(1, -1);
	matr.frustum(-2, 2, -1.5, 1.5, 2, 100);

	morda::Matr4r m(matr);
	m.translate(0, 0, -4);

	m.rotate(this->rot);

	glEnable(GL_CULL_FACE);

	morda::inst().renderer().shader->posTex->render(m, *this->cubeVAO, this->tex->tex());

	glDisable(GL_CULL_FACE);
}
