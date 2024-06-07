#pragma once
class Random
{
public:
	Random(unsigned __int64 seed)
	{
		for (int i = 0; i < 4; ++i)
		{
			unsigned __int64 temp = (seed += 0x9E3779B97f4A7C15ull);
			temp = (temp ^ (temp >> 30)) * 0xBF58476D1CE4E5B9ull;
			temp = (temp ^ (temp >> 27)) * 0x94D049BB133111EBull;
			s[i] = temp ^ (temp >> 31);
		}
	}

private:
	inline unsigned __int64 rotl(const unsigned __int64 x, __int32 k)
	{
		return (x << k) | (x >> (64 - k));
	}

public:
	unsigned __int64 next()
	{
		const unsigned __int64 result = rotl(s[0] + s[3], 23) + s[0];

		const unsigned __int64 t = s[1] << 17;

		s[2] ^= s[0];
		s[3] ^= s[1];
		s[1] ^= s[2];
		s[0] ^= s[3];

		s[2] ^= t;

		s[3] = rotl(s[3], 45);

		return result;
	}

private:
	unsigned __int64 s[4];
};