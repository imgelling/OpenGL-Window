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
		~Color();
		Color(const float_t r, const float_t g, const float_t b, const float_t a);
		//Color(const double_t r, const double_t g, const double_t b, const double_t a);
		//Color(const uint32_t r, const uint32_t g, const uint32_t b, const uint32_t a);
		//Color(const float_t r, const float_t g, const float_t b);
		//Color(const double_t r, const double_t g, const double_t b);
		//Color(const uint32_t r, const uint32_t g, const uint32_t b);

		void Set(const float_t r, const float_t g, const float_t b, const float_t a);
		//void Set(const double_t r, const double_t g, const double_t b, const double_t a);
		//void Set(const uint32_t r, const uint32_t g, const uint32_t b, const uint32_t a);
		//void Set(const float_t r, const float_t g, const float_t b);
		//void Set(const double_t r, const double_t g, const double_t b);
		//void Set(const uint32_t r, const uint32_t g, const uint32_t b);
		//void Set(const uint8_t r, const uint8_t b, const uint8_t g);

	private:
		const float_t _convert256toF = 1.0f / 255.0f;
	};

	inline Color::Color()
	{
		rf = 0.0f;
		gf = 0.0f;
		bf = 0.0f;
		af = 1.0f;
		r = 0;
		g = 0;
		b = 0;
		a = 255;
		packed = a << 24;
	}

	inline Color::~Color()
	{

	}

	inline Color::Color(const float_t r, const float_t g, const float_t b, const float_t a)
	{
		Set(r, g, b, a);
		this->r = (uint32_t)(r * 255.0f);
		this->g = (uint32_t)(g * 255.0f);
		this->b = (uint32_t)(b * 255.0f);
		this->a = (uint32_t)(a * 255.0f);
		this->rf = r;
		this->gf = g;
		this->bf = b;
		this->af = a;

		// this may need to be changed for amd/nvidia
		uint32_t packedR = this->r << 0;
		uint32_t packedG = this->g << 8;
		uint32_t packedB = this->b << 16;
		uint32_t packedA = this->a << 24;
		packed = ((packedA) | (packedR) | (packedG) | (packedB));
	}

	void Color::Set(const float_t r, const float_t g, const float_t b, const float_t a)
	{
		this->r = (uint32_t)(r * 255.0f);
		this->g = (uint32_t)(g * 255.0f);
		this->b = (uint32_t)(b * 255.0f);
		this->a = (uint32_t)(a * 255.0f);
		this->rf = r;
		this->gf = g;
		this->bf = b;
		this->af = a;

		uint32_t packedR = this->r << 0;
		uint32_t packedG = this->g << 8;
		uint32_t packedB = this->b << 16;
		uint32_t packedA = this->a << 24;
		packed = ((packedA) | (packedR) | (packedG) | (packedB));
	}

	namespace Colors
	{
		const Color White(1.0f, 1.0f, 1.0f, 1.0f);
		const Color Red(1.0f, 0, 0, 1.0f);
		const Color Green(0, 1.0f, 0, 1.0f);
		const Color Blue(0, 0, 1.0f, 1.0f);
		const Color Black(0, 0, 0, 1.0f);
		const Color Pink(1.0f, 0, 1.0f, 1.0f);
		const Color Gray(0.5f, 0.5f, 0.5f, 1.0f);
		const Color DarkGray(0.25f, 0.25f, 0.25, 1.0f);
	}
}