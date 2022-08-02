namespace Basil
{
	public struct Vector3
	{
		// Class variables
		public float X, Y, Z;

		// Zero vector
		public static Vector3 Zero => new Vector3(0.0f);

		// Create vector with single scalar
		public Vector3(float scalar)
		{
			X = scalar;
			Y = scalar;
			Z = scalar;
		}

		// Create vector with individual values
		public Vector3(float x, float y, float z)
		{
			X = x;
			Y = y;
			Z = z;
		}

		// Get/Set Vector2 from Vector3
		public Vector2 XY
		{
			get => new Vector2(X, Y);
			set
			{
				X = value.X;
				Y = value.Y;
			}
		}

		// Add two vectors
		public static Vector3 operator+(Vector3 a, Vector3 b)
		{
			return new Vector3(a.X + b.X, a.Y + b.Y, a.Z + b.Z);
		}

		// Multiply vector with scalar
		public static Vector3 operator*(Vector3 vector, float scalar)
		{
			return new Vector3(vector.X * scalar, vector.Y * scalar, vector.Z * scalar);
		}
	}
}
