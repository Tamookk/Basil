using System;
using System.Runtime.CompilerServices;

namespace Basil
{
	public class Entity
	{
		// Default constructor
		protected Entity() { ID = 0; }

		// Regular constructor
		internal Entity(ulong id)
		{
			ID = id;
		}

		// ID variable
		public readonly ulong ID;

		// Get/set entity translation
		public Vector3 Translation
		{
			// Get translation via internal call
			get
			{
				InternalCalls.TransformComponent_GetTranslation(ID, out Vector3 result);
				return result;
			}
			// Set translation via internal call
			set
			{
				InternalCalls.TransformComponent_SetTranslation(ID, ref value);
			}
		}

		// Does entity have component?
		public bool HasComponent<T>() where T : Component, new()
		{
			Type componentType = typeof(T);
			return InternalCalls.Entity_HasComponent(ID, componentType);
		}

		// Get component from entity
		public T GetComponent<T>() where T : Component, new()
		{
			if (!HasComponent<T>())
				return null;

			T component = new T() { Entity = this };
			return component;
		}
	}
}
