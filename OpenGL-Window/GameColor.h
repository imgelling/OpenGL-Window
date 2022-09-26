#pragma once

namespace game
{
	class Color
	{
	public:
		float_t rf;
		float_t gf;
		float_t bf;
		float_t af;
		uint32_t r;
		uint32_t g;
		uint32_t b;
		uint32_t a;
		uint32_t packed;

		Color();
		Color(float_t r, float_t g, float_t b, float_t a);
		Color(double_t r, double_t g, double_t b, double_t a);
		Color(uint32_t r, uint32_t g, uint32_t b, uint32_t a);
		Color(float_t r, float_t g, float_t b);
		Color(double_t r, double_t g, double_t b);
		Color(uint32_t r, uint32_t g, uint32_t b);
		~Color();

		void Set(float_t r, float_t g, float_t b, float_t a);
		void Set(double_t r, double_t g, double_t b, double_t a);
		void Set(uint32_t r, uint32_t g, uint32_t b, uint32_t a);
		void Set(float_t r, float_t g, float_t b);
		void Set(double_t r, double_t g, double_t b);
		void Set(uint32_t r, uint32_t g, uint32_t b);
		void Set(uint8_t r, uint8_t b, uint8_t g);

	private:
		const float_t _convert256toF = 1.0f / 255.0f;
	};

	inline Color::Color()
	{
		rf = 0.0f;
		gf = 0.0f;
		bf = 0.0f;
		r = 0;
		g = 0;
		b = 0;
		packed = 0;
	}

	inline Color::~Color()
	{

	}
}