#pragma once

namespace game
{

	// Vector2 
	template <typename T>
	class Vector2
	{
	public:
		T x;
		T y;
		Vector2() : x((T)0.0), y((T)0.0) {};
		Vector2(const T& x, const T& y)
		{
			this->x = x;
			this->y = y;
		}
		Vector2 operator- (const Vector2& rhs)
		{
			Vector2<T> ret;
			ret.x = x - rhs.x;
			ret.y = y - rhs.y;
			return ret;
		}
	private:
	};
	typedef Vector2<int> Vector2i;	
	typedef Vector2<float> Vector2f;
	typedef Vector2<double> Vector2d;
	typedef Vector2<int> Pointi;
	typedef Vector2<float> Pointf;
	typedef Vector2<double> Pointd;
}
