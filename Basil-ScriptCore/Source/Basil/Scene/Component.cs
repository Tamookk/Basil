namespace Basil
{
	// Abstract parent Component class
	public abstract class Component
	{
		// Contains an Entity with public get and internal set methods
		public Entity Entity { get; internal set; }
	}

	// TransformComponent
	public class TransformComponent : Component
	{
		public Vector3 Translation
		{
			// Get translation via internal call
			get
			{
				InternalCalls.TransformComponent_GetTranslation(Entity.ID, out Vector3 translation);
				return translation;
			}
			// Set translation via internal call
			set
			{
				InternalCalls.TransformComponent_SetTranslation(Entity.ID, ref value);
			}
		}
	}

	// Rigidbody2DComponent
	public class Rigidbody2DComponent : Component
	{
		// Apply linear impulse via internal call
		public void ApplyLinearImpulse(Vector2 impulse, Vector2 worldPosition, bool wake)
		{
			InternalCalls.Rigidbody2DComponent_ApplyLinearImpulse(Entity.ID, ref impulse, ref worldPosition, wake);
		}

		// Apply linear impulse to center via internal call
		public void ApplyLinearImpulse(Vector2 impulse, bool wake)
		{
			InternalCalls.Rigidbody2DComponent_ApplyLinearImpulseToCenter(Entity.ID, ref impulse, wake);
		}
	}
}
