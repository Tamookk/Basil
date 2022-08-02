namespace Basil
{
	public struct Vector2
	{
		// Variables
		public float X, Y;

		// Zero Vector2
		public static Vector2 Zero => new Vector2(0.0f);

		// Vector2 from scalar
		public Vector2(float scalar)
		{
			X = scalar;
			Y = scalar;
		}

		// Vector2 from values
		public Vector2(float x, float y)
		{
			X = x;
			Y = y;
		}

		// Addition operator
		public static Vector2 operator+(Vector2 a, Vector2 b)
		{
			return new Vector2(a.X + b.X, a.Y + b.Y);
		}

		// Multiply Vector2 by scalar
		public static Vector2 operator*(Vector2 vector, float scalar)
		{
			return new Vector2(vector.X * scalar, vector.Y * scalar);
		}
	}
}
