using System;

using Basil;

namespace Sandbox
{
	public class Player : Entity
	{
		private TransformComponent transform;
		private Rigidbody2DComponent rigidbody;

		// On create function
		void OnCreate()
		{
			// Write message
			Console.WriteLine($"Player.OnCreate - {ID}");

			// Get components
			transform = GetComponent<TransformComponent>();
			rigidbody = GetComponent<Rigidbody2DComponent>();
		}
		
		// On update function
		void OnUpdate(float ts)
		{
			// Set speed
			float speed = 0.1f;
			
			// Create velocity vector
			Vector3 velocity = Vector3.Zero;

			// Check for input
			if (Input.IsKeyDown(KeyCode.W))
				velocity.Y = 1.0f;
			else if (Input.IsKeyDown(KeyCode.S))
				velocity.Y = -1.0f;

			if (Input.IsKeyDown(KeyCode.A))
				velocity.X = -1.0f;
			else if (Input.IsKeyDown(KeyCode.D))
				velocity.X = 1.0f;

			// Multiply velocity by speed
			velocity *= speed;

			// Apply impulse of velocity
			rigidbody.ApplyLinearImpulse(velocity.XY, true);
		}
	}
}
