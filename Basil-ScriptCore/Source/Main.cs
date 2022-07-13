/* Test file for C# scripting support in Basil/Pesto. */

using System;

namespace Basil
{
	public class Main
	{
		public float FloatVar { get; set; }

		public Main()
		{
			Console.WriteLine("Hello, world!");
		}

		public void PrintMessage()
		{
			Console.WriteLine("Hello from C#");
		}

		public void PrintInt(int value)
		{
			Console.WriteLine($"The number is {value}");
		}

		public void PrintInts(int value1, int value2)
		{
			Console.WriteLine($"The numbers are {value1} and {value2}");
		}

		public void PrintCustomMessage(string message)
		{
			Console.WriteLine($"The message is: {message}");
		}
	}
}
