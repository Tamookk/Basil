/*
 * This header file contains a class for keeping track of time.
 */
#pragma once

namespace Basil
{
	class Timestep
	{
		public:
			Timestep(float time);
			float getSeconds() const;
			float getMilliseconds() const;
			operator float() const { return time; }
		private:
			float time;
	};
}
