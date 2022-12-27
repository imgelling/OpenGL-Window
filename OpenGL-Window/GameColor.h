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
		uint32_t packedABGR;
		uint32_t packedARGB;

		Color();
		Color(const float_t r, const float_t g, const float_t b, const float_t a);
		Color(const double_t r, const double_t g, const double_t b, const double_t a);
		Color(const uint32_t r, const uint32_t g, const uint32_t b, const uint32_t a);

		void Set(const float_t r, const float_t g, const float_t b, const float_t a);
		void Set(const double_t r, const double_t g, const double_t b, const double_t a);
		void Set(const uint32_t r, const uint32_t g, const uint32_t b, const uint32_t a);

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
		packedABGR = a << 24;
		packedARGB = a << 24;
	}

	// ----- Begin float_t
	inline Color::Color(const float_t r, const float_t g, const float_t b, const float_t a)
	{
		Set(r, g, b, a);
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
		packedABGR = ((packedA) | (packedR) | (packedG) | (packedB));

		packedR = this->r << 16;
		packedB = this->b << 0;
		packedARGB = ((packedA) | (packedR) | (packedG) | (packedB));
	}
	// ----- End float_t

	// Begin double_t
	inline Color::Color(const double_t r, const double_t g, const double_t b, const double_t a)
	{
		Set(r, g, b, a);
	}

	inline void Color::Set(const double_t r, const double_t g, const double_t b, const double_t a)
	{
		this->r = (uint32_t)(r * 255.0f);
		this->g = (uint32_t)(g * 255.0f);
		this->b = (uint32_t)(b * 255.0f);
		this->a = (uint32_t)(a * 255.0f);
		this->rf = (float_t)r;
		this->gf = (float_t)g;
		this->bf = (float_t)b;
		this->af = (float_t)a;

		uint32_t packedR = this->r << 0;
		uint32_t packedG = this->g << 8;
		uint32_t packedB = this->b << 16;
		uint32_t packedA = this->a << 24;
		packedABGR = ((packedA) | (packedR) | (packedG) | (packedB));

		packedR = this->r << 16;
		packedB = this->b << 0;
		packedARGB = ((packedA) | (packedR) | (packedG) | (packedB));
	}
	// End double_t

	// Start uint32_t
	inline Color::Color(const uint32_t r, const uint32_t g, const uint32_t b, const uint32_t a)
	{
		Set(r, g, b, a);
	}

	inline void Color::Set(const uint32_t r, const uint32_t g, const uint32_t b, const uint32_t a)
	{
		this->r = r;
		this->g = g;
		this->b = b;
		this->a = a;
		this->rf = r * _convert256toF;
		this->gf = g * _convert256toF;
		this->bf = b * _convert256toF;
		this->af = a * _convert256toF;

		uint32_t packedR = r << 0;
		uint32_t packedG = g << 8;
		uint32_t packedB = b << 16;
		uint32_t packedA = a << 24;
		packedABGR = ((packedA) | (packedR) | (packedG) | (packedB));

		packedR = this->r << 16;
		packedB = this->b << 0;
		packedARGB = ((packedA) | (packedR) | (packedG) | (packedB));
	}
	// End uint32_t



	// Defined standard colors (full alpha)
	namespace Colors
	{
		const Color White(1.0f, 1.0f, 1.0f, 1.0f);
		const Color Red(1.0f, 0, 0, 1.0f);
		const Color Green(0, 1.0f, 0, 1.0f);
		const Color Blue(0, 0, 1.0f, 1.0f);
		const Color Black(0, 0, 0, 1.0f);
		const Color Pink(1.0f, 0, 1.0f, 1.0f);
		const Color Gray(0.5f, 0.5f, 0.5f, 1.0f);
		const Color DarkGray(0.25f, 0.25f, 0.25f, 1.0f);
	}
}