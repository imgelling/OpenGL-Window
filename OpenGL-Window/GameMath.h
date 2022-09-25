#pragma once

namespace game
{

#pragma region Vector2 
	template <typename T>
	class Vector2
	{
	public:
		union 
		{
			T x;
			T width;
		};
		union
		{
			T y;
			T height;
		};

		Vector2()  
		{
			x = (T)0.0;
			y = (T)0.0;
		};
		Vector2(const T& x, const T& y)
		{
			this->x = x;
			this->y = y;
		}
		Vector2 operator - (const Vector2& rhs)
		{
			Vector2<T> ret;
			ret.x = x - rhs.x;
			ret.y = y - rhs.y;
			return ret;
		}
		Vector2& operator -= (const Vector2& rhs)
		{
			x = x - rhs.x;
			y = y - rhs.y;
			return *this;
		}
		Vector2 operator + (const Vector2& rhs)
		{
			Vector2<T> ret;
			ret.x = x + rhs.x;
			ret.y = y + rhs.y;
			return ret;
		}
		Vector2& operator += (const Vector2& rhs)
		{
			x = x + rhs.x;
			y = y + rhs.y;
			return *this;
		}

	private:
	};
	typedef Vector2<int> Vector2i, Pointi;	
	typedef Vector2<float> Vector2f, Pointf;
	typedef Vector2<double> Vector2d, Pointd;
#pragma endregion


}
