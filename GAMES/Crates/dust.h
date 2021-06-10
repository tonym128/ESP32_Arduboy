struct Dust: Entity
{
	uint8_t ttl = 0;
	uint8_t animFreq = 0;

	Dust() = default;

	Dust(float const &X, float const &Y, float const &Angle, float const &Speed)
	{
		x = X, y = Y;
		angle = Angle;
		speed = Speed;

		frameCount = 4;
		animFreq = 8;

		ttl = frameCount;
	}

	void animate()
	{
		if (arduboy.everyXFrames(animFreq))
		{
			if (curFrame < frameCount - 1)
			{
				curFrame += 1;
			}
			else
			{
				curFrame = 0;
			}
		}
	}

	void update()
	{
		if (arduboy.everyXFrames(animFreq)) ttl--;

		animate();
		physics();
	}

	void draw() const
	{
		sketch(DUST, curFrame);
	}
};
