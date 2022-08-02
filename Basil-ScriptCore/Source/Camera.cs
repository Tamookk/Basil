using Basil;

namespace Sandbox
{
	public class Camera : Entity
	{
		// On update function
		void OnUpdate(float ts)
		{
			// Set speed
			float speed = 1.0f;

			// Create velocity variable
			Vector3 velocity = Vector3.Zero;

			// Check for input
			if (Input.IsKeyDown(KeyCode.Up))
				velocity.Y = 1.0f;
			else if (Input.IsKeyDown(KeyCode.Down))
				velocity.Y = -1.0f;

			if (Input.IsKeyDown(KeyCode.Left))
				velocity.X = -1.0f;
			else if (Input.IsKeyDown(KeyCode.Right))
				velocity.X = 1.0f;

			// Multiply by speed
			velocity *= speed;

			// Get entity translation
			Vector3 translation = Translation;

			// Multiply velocity by frame time (time step)
			translation += velocity * ts;

			// Set entity translation
			Translation = translation;
		}
	}
}
