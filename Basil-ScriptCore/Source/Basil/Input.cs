namespace Basil
{
	public class Input
	{
		// Return if key is down via internal call
		public static bool IsKeyDown(KeyCode keycode)
		{
			return InternalCalls.Input_IsKeyDown(keycode);
		}
	}
}
